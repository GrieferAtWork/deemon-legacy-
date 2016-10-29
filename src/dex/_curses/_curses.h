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
#ifndef GUARD_DEEMON_DEX_CURSES_H
#define GUARD_DEEMON_DEX_CURSES_H 1
 
#include <deemon/__conf.inl>

DEE_COMPILER_MSVC_WARNING_PUSH(4005 4201 4820 4255 4668)
#ifdef DEE_PLATFORM_WINDOWS
#define INCLUDE_WINDOWS_H
#include <Windows.h>
#include "win32/curses.h"
#else
#include <ncursesw/curses.h>
#endif
DEE_COMPILER_MSVC_WARNING_POP


#ifdef GUARD_DEEMON_DEX_CURSES_C
#include <deemon/dex.h>
#include <deemon/type.h>
#include <deemon/string.h>
#include <deemon/error.h>
#include <deemon/object.h>
#include <deemon/optional/std/string.h>
#else /* GUARD_DEEMON_DEX_CURSES_C */
#include <deemon/optional/atomic_once.h>
#endif /* GUARD_DEEMON_DEX_CURSES_C */

#ifndef DEE_PLATFORM_WINDOWS
#define term_attrs termattrs
#endif


DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif

#ifdef GUARD_DEEMON_DEX_CURSES_C
DEE_OBJECT_DEF(DeeCursesWindowObject);
DEE_OBJECT_DEF(DeeCursesScreenObject);

struct DeeCursesWindowObject {
 DEE_OBJECT_HEAD
 WINDOW *w_wnd;
};
struct DeeCursesScreenObject {
 DEE_OBJECT_HEAD
 SCREEN *s_scrn;
};
#define DeeCursesWindow_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeCursesWindow_Type)
#define DeeCursesWindow_Check          DeeCursesWindow_CheckExact
#define DeeCursesScreen_CheckExact(ob) DeeObject_InstanceOfExact(ob,&DeeCursesScreen_Type)
#define DeeCursesScreen_Check          DeeCursesWindow_CheckExact

extern DeeTypeObject DeeCursesWindow_Type;
extern DeeTypeObject DeeCursesScreen_Type;
extern DeeTypeObject DeeCursesError_Type;

//////////////////////////////////////////////////////////////////////////
// Returns a new curses window, inheriting the given non-NULL 'wnd' object
extern DEE_A_RET_EXCEPT_REF DeeCursesWindowObject *
 DeeCursesWindow_New(DEE_A_IN WINDOW *wnd);

//////////////////////////////////////////////////////////////////////////
// in "_curses.keycode.inl"
// Public keys: 'CURSES_PUBLIC_KEY_*' (s.a.: further below in this file)
// Private keys: 'KEY_*' (s.a.: '<curses.h>')
extern DEE_A_RET_WUNUSED int DeeCurses_PrivateKeycodeToPublic(DEE_A_IN int key);
extern DEE_A_RET_WUNUSED int DeeCurses_PublicKeycodeToPrivate(DEE_A_IN int key);

// Returns 'none' for unknown keys
extern DEE_A_RET_EXCEPT_REF DeeObject *DeeCurses_KeyToStr(DEE_A_IN int key);
// Returns '-1' for unknown keys
extern DEE_A_RET_WUNUSED int DeeCurses_StrToKey(DEE_A_IN_Z char const *key);
extern DEE_A_RET_EXCEPT(-1) int DeeCurses_StrToKeyOb(DEE_A_INOUT DeeObject *ob, DEE_A_OUT int *result);
#endif /* GUARD_DEEMON_DEX_CURSES_C */

#define DEE_CURSES_API_SIZE 0xAD

struct DeeCursesContext {
#ifdef GUARD_DEEMON_DEX_CURSES_C
#define DEE_CURSESCONTEXT_FLAG_NONE         0x00000000
#define DEE_CURSESCONTEXT_FLAG_HASPROGMODE  0x00000001 /*< def_prog_mode() was called during context acquisition. */
#define DEE_CURSESCONTEXT_FLAG_HASSHELLMODE 0x00000002 /*< def_shell_mode() was called during context acquisition. */
#define DEE_CURSESCONTEXT_FLAG_WASENDWIN    0x00000004 /*< isendwin() was true during context acquisition. */
 Dee_uint32_t cc_flags;
#else
 Dee_uint32_t _cc_data[8]; /*< 32 bytes of Padding data. */
#endif
};

#define DEE_CURSES_ACQUIRECONTEXT_FLAG_NONE      0x00000000
#define DEE_CURSES_ACQUIRECONTEXT_FLAG_PUSHSTATE 0x00000001 /*< Store the previous curses state. */

//////////////////////////////////////////////////////////////////////////
// Acquire/Release a curses context that will grant the calling
// thread exclusive access to the entire curses API, while also
// ensuring that curses is initialized, allowing for recursion
// and storing/restoring the previous curses state.
// NOTE: 'flags' is a set of 'DEE_CURSES_ACQUIRECONTEXT_FLAG_*'
// NOTE: Undefined behavior if contexts are released in an order
//       other than that in which they were acquired.
#ifdef GUARD_DEEMON_DEX_CURSES_C
extern DEE_ATTRIBUTE_DLLEXPORT DEE_A_RET_EXCEPT(-1) int DEE_CALL DeeCurses_AcquireContext(
 DEE_A_OUT struct DeeCursesContext *context, DEE_A_IN Dee_uint32_t flags);
extern DEE_ATTRIBUTE_DLLEXPORT void DEE_CALL DeeCurses_ReleaseContext(
 DEE_A_IN struct DeeCursesContext *context);
extern DEE_ATTRIBUTE_DLLEXPORT void *DeeCurses_API[DEE_CURSES_API_SIZE];
#define DEE_CURSES_API(id,T) (*((T)DeeCurses_API[id]))
#else /* GUARD_DEEMON_DEX_CURSES_C */
#define DeeCurses_AcquireContext   (*P_DeeCurses_AcquireContext)
#define DeeCurses_ReleaseContext   (*P_DeeCurses_ReleaseContext)
#define DEE_CURSES_API(id,T) (*((T)(*P_DeeCurses_API)[id]))

static DEE_A_RET_EXCEPT(-1) int (DEE_CALL *P_DeeCurses_AcquireContext)(
 DEE_A_OUT struct DeeCursesContext *context, DEE_A_IN Dee_uint32_t flags);
static void (DEE_CALL *P_DeeCurses_ReleaseContext)(
 DEE_A_IN struct DeeCursesContext *context);
static void *(*P_DeeCurses_API)[DEE_CURSES_API_SIZE];
static struct DeeAtomicOnceFlag P_DeeCurses_ApiLoaded = DeeAtomicOnceFlag_INIT();

#define DeeCurses_LOAD_API(on_error) \
DeeAtomicOnceFlag_RUN(&P_DeeCurses_ApiLoaded,{\
 DeeObject *_curses_dex;\
 if ((_curses_dex = DeeDex_Open("_curses")) == NULL) {on_error;}\
 else {\
  if ((*(void **)&P_DeeCurses_AcquireContext = DeeDexModule_GetNativeAddress(_curses_dex,"DeeCurses_AcquireContext")) == NULL || \
      (*(void **)&P_DeeCurses_ReleaseContext = DeeDexModule_GetNativeAddress(_curses_dex,"DeeCurses_ReleaseContext")) == NULL || \
      (*(void **)&P_DeeCurses_API = DeeDexModule_GetNativeAddress(_curses_dex,"DeeCurses_API")) == NULL\
  ) {Dee_DECREF(_curses_dex);{on_error;}}\
    else Dee_DECREF(_curses_dex);\
 }\
})

#endif /* GUARD_DEEMON_DEX_CURSES_C */

#define DEE_CURSES_CALL /* nothing */

#ifdef GUARD_DEEMON_DEX_CURSES_C
#include "_curses.link_direct.inl"
#else
//@autoexport begin
#define curses_curscr          (DEE_CURSES_API(0x00,WINDOW *(DEE_CALL *)(void))())
#define curses_stdscr          (DEE_CURSES_API(0x01,WINDOW *(DEE_CALL *)(void))())
#define curses_ttytype         (DEE_CURSES_API(0x02,char *(DEE_CALL *)(void))())
#define curses_COLORS          (DEE_CURSES_API(0x03,int (DEE_CALL *)(void))())
#define curses_COLOR_PAIRS     (DEE_CURSES_API(0x04,int (DEE_CALL *)(void))())
#define curses_COLS            (DEE_CURSES_API(0x05,int (DEE_CALL *)(void))())
#define curses_LINES           (DEE_CURSES_API(0x06,int (DEE_CALL *)(void))())
#define curses_TABSIZE         (DEE_CURSES_API(0x07,int (DEE_CALL *)(void))())
#define curses_def_prog_mode    DEE_CURSES_API(0x08,int (DEE_CURSES_CALL *)(void))
#define curses_def_shell_mode   DEE_CURSES_API(0x09,int (DEE_CURSES_CALL *)(void))
#define curses_reset_prog_mode  DEE_CURSES_API(0x0A,int (DEE_CURSES_CALL *)(void))
#define curses_reset_shell_mode DEE_CURSES_API(0x0B,int (DEE_CURSES_CALL *)(void))
#define curses_resetty          DEE_CURSES_API(0x0C,int (DEE_CURSES_CALL *)(void))
#define curses_savetty          DEE_CURSES_API(0x0D,int (DEE_CURSES_CALL *)(void))
#define curses_getsyx_impl_     DEE_CURSES_API(0x0E,void (DEE_CURSES_CALL *)(int *y, int *x))
#define curses_getsyx(y,x)      curses_getsyx_impl_(&(y),&(x))
#define curses_ripoffline       DEE_CURSES_API(0x0F,int (DEE_CURSES_CALL *)(int line, int (*init)(WINDOW *, int)))
#define curses_curs_set         DEE_CURSES_API(0x10,int (DEE_CURSES_CALL *)(int visibility))
#define curses_napms            DEE_CURSES_API(0x11,int (DEE_CURSES_CALL *)(int ms))
#define curses_wrefresh         DEE_CURSES_API(0x12,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_wnoutrefresh     DEE_CURSES_API(0x13,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_doupdate         DEE_CURSES_API(0x14,int (DEE_CURSES_CALL *)(void))
#define curses_redrawwin        DEE_CURSES_API(0x15,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_wredrawln        DEE_CURSES_API(0x16,int (DEE_CURSES_CALL *)(WINDOW *win, int beg_line, int num_lines))
#define curses_getbegx          DEE_CURSES_API(0x17,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_getbegy          DEE_CURSES_API(0x18,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_getmaxx          DEE_CURSES_API(0x19,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_getmaxy          DEE_CURSES_API(0x1A,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_getparx          DEE_CURSES_API(0x1B,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_getpary          DEE_CURSES_API(0x1C,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_getcurx          DEE_CURSES_API(0x1D,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_getcury          DEE_CURSES_API(0x1E,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_clearok          DEE_CURSES_API(0x1F,int (DEE_CURSES_CALL *)(WINDOW *win, unsigned char bf))
#define curses_idlok            DEE_CURSES_API(0x20,int (DEE_CURSES_CALL *)(WINDOW *win, unsigned char bf))
#define curses_idcok            DEE_CURSES_API(0x21,void (DEE_CURSES_CALL *)(WINDOW *win, unsigned char bf))
#define curses_immedok          DEE_CURSES_API(0x22,void (DEE_CURSES_CALL *)(WINDOW *win, unsigned char bf))
#define curses_leaveok          DEE_CURSES_API(0x23,int (DEE_CURSES_CALL *)(WINDOW *win, unsigned char bf))
#define curses_wsetscrreg       DEE_CURSES_API(0x24,int (DEE_CURSES_CALL *)(WINDOW *win, int top, int bot))
#define curses_scrollok         DEE_CURSES_API(0x25,int (DEE_CURSES_CALL *)(WINDOW *win, unsigned char bf))
#define curses_nl               DEE_CURSES_API(0x26,int (DEE_CURSES_CALL *)(void))
#define curses_nonl             DEE_CURSES_API(0x27,int (DEE_CURSES_CALL *)(void))
#define curses_start_color      DEE_CURSES_API(0x28,int (DEE_CURSES_CALL *)(void))
#define curses_init_pair        DEE_CURSES_API(0x29,int (DEE_CURSES_CALL *)(short pair, short f, short b))
#define curses_init_color       DEE_CURSES_API(0x2A,int (DEE_CURSES_CALL *)(short color, short r, short g, short b))
#define curses_has_colors       DEE_CURSES_API(0x2B,unsigned char (DEE_CURSES_CALL *)(void))
#define curses_can_change_color DEE_CURSES_API(0x2C,unsigned char (DEE_CURSES_CALL *)(void))
#define curses_color_content    DEE_CURSES_API(0x2D,int (DEE_CURSES_CALL *)(short color, short *r, short *g, short *b))
#define curses_pair_content     DEE_CURSES_API(0x2E,int (DEE_CURSES_CALL *)(short pair, short *f, short *b))
#define curses_overlay          DEE_CURSES_API(0x2F,int (DEE_CURSES_CALL *)(const WINDOW *srcwin, WINDOW *dstwin))
#define curses_overwrite        DEE_CURSES_API(0x30,int (DEE_CURSES_CALL *)(const WINDOW *srcwin, WINDOW *dstwin))
#define curses_copywin          DEE_CURSES_API(0x31,int (DEE_CURSES_CALL *)(const WINDOW *srcwin, WINDOW *dstwin, int sminrow, int smincol, int dminrow, int dmincol, int dmaxrow, int dmaxcol, int overlay_))
#define curses_keyname          DEE_CURSES_API(0x32,char   * (DEE_CURSES_CALL *)(int c))
#define curses_filter           DEE_CURSES_API(0x33,void (DEE_CURSES_CALL *)(void))
#define curses_use_env          DEE_CURSES_API(0x34,void (DEE_CURSES_CALL *)(unsigned char f))
#define curses_putwin           DEE_CURSES_API(0x35,int (DEE_CURSES_CALL *)(WINDOW *win, FILE *filep))
#define curses_getwin           DEE_CURSES_API(0x36,WINDOW * (DEE_CURSES_CALL *)(FILE *filep))
#define curses_delay_output     DEE_CURSES_API(0x37,int (DEE_CURSES_CALL *)(int ms))
#define curses_flushinp         DEE_CURSES_API(0x38,int (DEE_CURSES_CALL *)(void))
#define curses_initscr          DEE_CURSES_API(0x39,WINDOW * (DEE_CURSES_CALL *)(void))
#define curses_endwin           DEE_CURSES_API(0x3A,int (DEE_CURSES_CALL *)(void))
#define curses_isendwin         DEE_CURSES_API(0x3B,unsigned char (DEE_CURSES_CALL *)(void))
#define curses_newterm          DEE_CURSES_API(0x3C,SCREEN * (DEE_CURSES_CALL *)(char *type, FILE *outfd, FILE *infd))
#define curses_set_term         DEE_CURSES_API(0x3D,SCREEN * (DEE_CURSES_CALL *)(SCREEN *new_))
#define curses_delscreen        DEE_CURSES_API(0x3E,void (DEE_CURSES_CALL *)(SCREEN* sp))
#define curses_newwin           DEE_CURSES_API(0x3F,WINDOW * (DEE_CURSES_CALL *)(int nlines, int ncols, int begin_y, int begin_x))
#define curses_delwin           DEE_CURSES_API(0x40,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_mvwin            DEE_CURSES_API(0x41,int (DEE_CURSES_CALL *)(WINDOW *win, int y, int x))
#define curses_subwin           DEE_CURSES_API(0x42,WINDOW * (DEE_CURSES_CALL *)(WINDOW *orig, int nlines, int ncols, int begin_y, int begin_x))
#define curses_derwin           DEE_CURSES_API(0x43,WINDOW * (DEE_CURSES_CALL *)(WINDOW *orig, int nlines, int ncols, int begin_y, int begin_x))
#define curses_mvderwin         DEE_CURSES_API(0x44,int (DEE_CURSES_CALL *)(WINDOW *win, int par_y, int par_x))
#define curses_dupwin           DEE_CURSES_API(0x45,WINDOW * (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_wsyncup          DEE_CURSES_API(0x46,void (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_syncok           DEE_CURSES_API(0x47,int (DEE_CURSES_CALL *)(WINDOW *win, unsigned char bf))
#define curses_wcursyncup       DEE_CURSES_API(0x48,void (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_wsyncdown        DEE_CURSES_API(0x49,void (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_wbkgdset         DEE_CURSES_API(0x4A,void (DEE_CURSES_CALL *)(WINDOW *win, chtype ch))
#define curses_wbkgd            DEE_CURSES_API(0x4B,int (DEE_CURSES_CALL *)(WINDOW *win, chtype ch))
#define curses_getbkgd          DEE_CURSES_API(0x4C,chtype (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_cbreak           DEE_CURSES_API(0x4D,int (DEE_CURSES_CALL *)(void))
#define curses_nocbreak         DEE_CURSES_API(0x4E,int (DEE_CURSES_CALL *)(void))
#define curses_echo             DEE_CURSES_API(0x4F,int (DEE_CURSES_CALL *)(void))
#define curses_noecho           DEE_CURSES_API(0x50,int (DEE_CURSES_CALL *)(void))
#define curses_halfdelay        DEE_CURSES_API(0x51,int (DEE_CURSES_CALL *)(int tenths))
#define curses_intrflush        DEE_CURSES_API(0x52,int (DEE_CURSES_CALL *)(WINDOW *win, unsigned char bf))
#define curses_keypad           DEE_CURSES_API(0x53,int (DEE_CURSES_CALL *)(WINDOW *win, unsigned char bf))
#define curses_meta             DEE_CURSES_API(0x54,int (DEE_CURSES_CALL *)(WINDOW *win, unsigned char bf))
#define curses_nodelay          DEE_CURSES_API(0x55,int (DEE_CURSES_CALL *)(WINDOW *win, unsigned char bf))
#define curses_raw              DEE_CURSES_API(0x56,int (DEE_CURSES_CALL *)(void))
#define curses_noraw            DEE_CURSES_API(0x57,int (DEE_CURSES_CALL *)(void))
#define curses_noqiflush        DEE_CURSES_API(0x58,void (DEE_CURSES_CALL *)(void))
#define curses_qiflush          DEE_CURSES_API(0x59,void (DEE_CURSES_CALL *)(void))
#define curses_notimeout        DEE_CURSES_API(0x5A,int (DEE_CURSES_CALL *)(WINDOW *win, unsigned char bf))
#define curses_wtimeout         DEE_CURSES_API(0x5B,void (DEE_CURSES_CALL *)(WINDOW *win, int delay))
#define curses_typeahead        DEE_CURSES_API(0x5C,int (DEE_CURSES_CALL *)(int fd))
#define curses_baudrate         DEE_CURSES_API(0x5D,int (DEE_CURSES_CALL *)(void))
#define curses_erasechar        DEE_CURSES_API(0x5E,char (DEE_CURSES_CALL *)(void))
#define curses_has_ic           DEE_CURSES_API(0x5F,unsigned char (DEE_CURSES_CALL *)(void))
#define curses_has_il           DEE_CURSES_API(0x60,unsigned char (DEE_CURSES_CALL *)(void))
#define curses_killchar         DEE_CURSES_API(0x61,char (DEE_CURSES_CALL *)(void))
#define curses_longname         DEE_CURSES_API(0x62,char   * (DEE_CURSES_CALL *)(void))
#define curses_term_attrs       DEE_CURSES_API(0x63,attr_t (DEE_CURSES_CALL *)(void))
#define curses_termattrs        DEE_CURSES_API(0x64,chtype (DEE_CURSES_CALL *)(void))
#define curses_termname         DEE_CURSES_API(0x65,char   * (DEE_CURSES_CALL *)(void))
#define curses_beep             DEE_CURSES_API(0x66,int (DEE_CURSES_CALL *)(void))
#define curses_flash            DEE_CURSES_API(0x67,int (DEE_CURSES_CALL *)(void))
#define curses_wtouchln         DEE_CURSES_API(0x68,int (DEE_CURSES_CALL *)(WINDOW *win, int y, int n, int changed))
#define curses_is_linetouched   DEE_CURSES_API(0x69,unsigned char (DEE_CURSES_CALL *)(WINDOW *win, int line))
#define curses_is_wintouched    DEE_CURSES_API(0x6A,unsigned char (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_vidputs          DEE_CURSES_API(0x6B,int (DEE_CURSES_CALL *)(chtype attrs, int (*putc)(int)))
#define curses_vidattr          DEE_CURSES_API(0x6C,int (DEE_CURSES_CALL *)(chtype attrs))
//#define curses_vid_puts       DEE_CURSES_API(0x6D,int (DEE_CURSES_CALL *)(attr_t attrs, short pair, void *opts, int (*putc)(char)))
#define curses_vid_attr         DEE_CURSES_API(0x6E,int (DEE_CURSES_CALL *)(attr_t attrs, short pair, void *opts))
#define curses_mvcur            DEE_CURSES_API(0x6F,int (DEE_CURSES_CALL *)(int oldrow, int oldcol, int newrow, int newcol))
#define curses_printw           DEE_CURSES_API(0x70,int (DEE_CURSES_CALL *)(const char *fmt, ...))
#define curses_wprintw          DEE_CURSES_API(0x71,int (DEE_CURSES_CALL *)(WINDOW *win, const char *fmt, ...))
#define curses_mvprintw         DEE_CURSES_API(0x72,int (DEE_CURSES_CALL *)(int y, int x, const char *fmt, ...))
#define curses_mvwprintw        DEE_CURSES_API(0x73,int (DEE_CURSES_CALL *)(WINDOW *win, int y, int x, const char *fmt, ...))
#define curses_vwprintw         DEE_CURSES_API(0x74,int (DEE_CURSES_CALL *)(WINDOW *win, const char *fmt, va_list varglist))
#define curses_scanw            DEE_CURSES_API(0x75,int (DEE_CURSES_CALL *)(char *fmt, ...))
#define curses_wscanw           DEE_CURSES_API(0x76,int (DEE_CURSES_CALL *)(WINDOW *win, char *fmt, ...))
#define curses_mvscanw          DEE_CURSES_API(0x77,int (DEE_CURSES_CALL *)(int y, int x, char *fmt, ...))
#define curses_mvwscanw         DEE_CURSES_API(0x78,int (DEE_CURSES_CALL *)(WINDOW *win, int y, int x, char *fmt, ...))
#define curses_vwscanw          DEE_CURSES_API(0x79,int (DEE_CURSES_CALL *)(WINDOW *win, char *fmt, va_list varglist))
#define curses_newpad           DEE_CURSES_API(0x7A,WINDOW * (DEE_CURSES_CALL *)(int nlines, int ncols))
#define curses_subpad           DEE_CURSES_API(0x7B,WINDOW * (DEE_CURSES_CALL *)(WINDOW *orig, int nlines, int ncols, int begin_y, int begin_x))
#define curses_prefresh         DEE_CURSES_API(0x7C,int (DEE_CURSES_CALL *)(WINDOW *pad, int pminrow, int pmincol, int sminrow, int smincol, int smaxrow, int smaxcol))
#define curses_pnoutrefresh     DEE_CURSES_API(0x7D,int (DEE_CURSES_CALL *)(WINDOW *pad, int pminrow, int pmincol, int sminrow, int smincol, int smaxrow, int smaxcol))
#define curses_pechochar        DEE_CURSES_API(0x7E,int (DEE_CURSES_CALL *)(WINDOW *pad, chtype ch))
#define curses_scr_dump         DEE_CURSES_API(0x7F,int (DEE_CURSES_CALL *)(const char *filename))
#define curses_scr_restore      DEE_CURSES_API(0x80,int (DEE_CURSES_CALL *)(const char *filename))
#define curses_scr_init         DEE_CURSES_API(0x81,int (DEE_CURSES_CALL *)(const char *filename))
#define curses_scr_set          DEE_CURSES_API(0x82,int (DEE_CURSES_CALL *)(const char *filename))
#define curses_slk_init         DEE_CURSES_API(0x83,int (DEE_CURSES_CALL *)(int fmt))
#define curses_slk_set          DEE_CURSES_API(0x84,int (DEE_CURSES_CALL *)(int labnum, const char *label, int fmt))
#define curses_slk_refresh      DEE_CURSES_API(0x85,int (DEE_CURSES_CALL *)(void))
#define curses_slk_noutrefresh  DEE_CURSES_API(0x86,int (DEE_CURSES_CALL *)(void))
#define curses_slk_label        DEE_CURSES_API(0x87,char   * (DEE_CURSES_CALL *)(int labnum))
#define curses_slk_clear        DEE_CURSES_API(0x88,int (DEE_CURSES_CALL *)(void))
#define curses_slk_restore      DEE_CURSES_API(0x89,int (DEE_CURSES_CALL *)(void))
#define curses_slk_touch        DEE_CURSES_API(0x8A,int (DEE_CURSES_CALL *)(void))
#define curses_slk_attron       DEE_CURSES_API(0x8B,int (DEE_CURSES_CALL *)(const chtype attrs))
#define curses_slk_attroff      DEE_CURSES_API(0x8C,int (DEE_CURSES_CALL *)(const chtype attrs))
#define curses_slk_attrset      DEE_CURSES_API(0x8D,int (DEE_CURSES_CALL *)(const chtype attrs))
#define curses_slk_attr_set     DEE_CURSES_API(0x8E,int (DEE_CURSES_CALL *)(const attr_t attrs, short color_pair_number, void* opts))
#define curses_slk_color        DEE_CURSES_API(0x8F,int (DEE_CURSES_CALL *)(short color_pair_number))
#define curses_waddchnstr       DEE_CURSES_API(0x90,int (DEE_CURSES_CALL *)(WINDOW *win, const chtype *chstr, int n))
#define curses_waddch           DEE_CURSES_API(0x91,int (DEE_CURSES_CALL *)(WINDOW *win, const chtype ch))
#define curses_wechochar        DEE_CURSES_API(0x92,int (DEE_CURSES_CALL *)(WINDOW *win, const chtype ch))
#define curses_waddnstr         DEE_CURSES_API(0x93,int (DEE_CURSES_CALL *)(WINDOW *win, const char *str, int n))
#define curses_wattrset         DEE_CURSES_API(0x94,int (DEE_CURSES_CALL *)(WINDOW *win, int attrs))
#define curses_wcolor_set       DEE_CURSES_API(0x95,int (DEE_CURSES_CALL *)(WINDOW *win, short color_pair_number, void* opts))
#define curses_wattr_get        DEE_CURSES_API(0x96,int (DEE_CURSES_CALL *)(WINDOW *win, attr_t *attrs, short *pair, void *opts))
#define curses_wattr_off        DEE_CURSES_API(0x97,int (DEE_CURSES_CALL *)(WINDOW *win, attr_t attrs, void *opts))
#define curses_wattr_on         DEE_CURSES_API(0x98,int (DEE_CURSES_CALL *)(WINDOW *win, attr_t attrs, void *opts))
#define curses_wattr_set        DEE_CURSES_API(0x99,int (DEE_CURSES_CALL *)(WINDOW *win, attr_t attrs, short pair, void *opts))
#define curses_wchgat           DEE_CURSES_API(0x9A,int (DEE_CURSES_CALL *)(WINDOW *win, int n, attr_t attr, short color, const void *opts))
#define curses_wborder          DEE_CURSES_API(0x9B,int (DEE_CURSES_CALL *)(WINDOW *win, chtype ls, chtype rs, chtype ts, chtype bs, chtype tl, chtype tr, chtype bl, chtype br))
#define curses_whline           DEE_CURSES_API(0x9C,int (DEE_CURSES_CALL *)(WINDOW *win, chtype ch, int n))
#define curses_wvline           DEE_CURSES_API(0x9D,int (DEE_CURSES_CALL *)(WINDOW *win, chtype ch, int n))
#define curses_werase           DEE_CURSES_API(0x9E,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_wclear           DEE_CURSES_API(0x9F,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_wclrtobot        DEE_CURSES_API(0xA0,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_wclrtoeol        DEE_CURSES_API(0xA1,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_wdelch           DEE_CURSES_API(0xA2,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_wgetch           DEE_CURSES_API(0xA3,int (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_wgetnstr         DEE_CURSES_API(0xA4,int (DEE_CURSES_CALL *)(WINDOW *win, char *str, int n))
#define curses_winchnstr        DEE_CURSES_API(0xA5,int (DEE_CURSES_CALL *)(WINDOW *win, chtype *chstr, int n))
#define curses_winch            DEE_CURSES_API(0xA6,chtype (DEE_CURSES_CALL *)(WINDOW *win))
#define curses_winnstr          DEE_CURSES_API(0xA7,int (DEE_CURSES_CALL *)(WINDOW *win, char *str, int n))
#define curses_winsch           DEE_CURSES_API(0xA8,int (DEE_CURSES_CALL *)(WINDOW *win, chtype ch))
#define curses_winsdelln        DEE_CURSES_API(0xA9,int (DEE_CURSES_CALL *)(WINDOW *win, int n))
#define curses_winsnstr         DEE_CURSES_API(0xAA,int (DEE_CURSES_CALL *)(WINDOW *win, const char *str, int n))
#define curses_wmove            DEE_CURSES_API(0xAB,int (DEE_CURSES_CALL *)(WINDOW *win, int y, int x))
#define curses_wscrl            DEE_CURSES_API(0xAC,int (DEE_CURSES_CALL *)(WINDOW *win, int n))
//@autoexport end
#endif




#define curses_getyx(win,y,x)   	             (y=curses_getcury(win),x=curses_getcurx(win))
#define curses_getbegyx(win,y,x)	             (y=curses_getbegy(win),x=curses_getbegx(win))
#define curses_getmaxyx(win,y,x)	             (y=curses_getmaxy(win),x=curses_getmaxx(win))
#define curses_getparyx(win,y,x)	             (y=curses_getpary(win),x=curses_getparx(win))
#define curses_wgetstr(w,s)		                  curses_wgetnstr(w,s,-1)
#define curses_getnstr(s,n)		                  curses_wgetnstr(curses_stdscr,s,n)
#define curses_wstandout(win)                 (curses_wattrset(win,A_STANDOUT))
#define curses_wstandend(win)                 (curses_wattrset(win,A_NORMAL))
#define curses_wattron(win,at)		               curses_wattr_on(win,(attr_t)(at),NULL)
#define curses_wattroff(win,at)	               curses_wattr_off(win,(attr_t)(at),NULL)
#define curses_scroll(win)		                   curses_wscrl(win,1)
#define curses_touchwin(win)		                 curses_wtouchln(win,0,curses_getmaxy(win),1)
#define curses_touchline(win,s,c)             	curses_wtouchln(win,s,c,1)
#define curses_untouchwin(win)		               curses_wtouchln(win,0,curses_getmaxy(win),0)
#define curses_box(win,v,h)		                  curses_wborder(win,v,v,h,h,0,0,0,0)
#define curses_border(ls,rs,ts,bs,tl,tr,bl,br)	curses_wborder(curses_stdscr,ls,rs,ts,bs,tl,tr,bl,br)
#define curses_hline(ch,n)		                   curses_whline(curses_stdscr,ch,n)
#define curses_vline(ch,n)		                   curses_wvline(curses_stdscr,ch,n)
#define curses_winstr(w,s)		                   curses_winnstr(w,s,-1)
#define curses_winchstr(w,s)		                 curses_winchnstr(w,s,-1)
#define curses_winsstr(w,s)		                  curses_winsnstr(w,s,-1)
#define curses_waddstr(win,str)	               curses_waddnstr(win,str,-1)
#define curses_waddchstr(win,str)	             curses_waddchnstr(win,str,-1)
#define curses_addch(ch)		                     curses_waddch(curses_stdscr,ch)
#define curses_addchnstr(str,n)	               curses_waddchnstr(curses_stdscr,str,n)
#define curses_addchstr(str)		                 curses_waddchstr(curses_stdscr,str)
#define curses_addnstr(str,n)		                curses_waddnstr(curses_stdscr,str,n)
#define curses_addstr(str)		                   curses_waddnstr(curses_stdscr,str,-1)
#define curses_attr_get(ap,cp,o)	              curses_wattr_get(curses_stdscr,ap,cp,o)
#define curses_attr_off(a,o)		                 curses_wattr_off(curses_stdscr,a,o)
#define curses_attr_on(a,o)		                  curses_wattr_on(curses_stdscr,a,o)
#define curses_attr_set(a,c,o)		               curses_wattr_set(curses_stdscr,a,c,o)
#define curses_attroff(at)		                   curses_wattroff(curses_stdscr,at)
#define curses_attron(at)		                    curses_wattron(curses_stdscr,at)
#define curses_attrset(at)		                   curses_wattrset(curses_stdscr,at)
#define curses_bkgd(ch)		                      curses_wbkgd(curses_stdscr,ch)
#define curses_bkgdset(ch)		                   curses_wbkgdset(curses_stdscr,ch)
#define curses_chgat(n,a,c,o)		                curses_wchgat(curses_stdscr,n,a,c,o)
#define curses_clear()			                      curses_wclear(curses_stdscr)
#define curses_clrtobot()		                    curses_wclrtobot(curses_stdscr)
#define curses_clrtoeol()		                    curses_wclrtoeol(curses_stdscr)
#define curses_color_set(c,o)		                curses_wcolor_set(curses_stdscr,c,o)
#define curses_delch()			                      curses_wdelch(curses_stdscr)
#define curses_deleteln()		                    curses_winsdelln(curses_stdscr,-1)
#define curses_echochar(c)		                   curses_wechochar(curses_stdscr,c)
#define curses_erase()			                      curses_werase(curses_stdscr)
#define curses_getch()			                      curses_wgetch(curses_stdscr)
#define curses_getstr(str)		                   curses_wgetstr(curses_stdscr,str)
#define curses_inch()			                       curses_winch(curses_stdscr)
#define curses_inchnstr(s,n)                   curses_winchnstr(curses_stdscr,s,n)
#define curses_inchstr(s)		                    curses_winchstr(curses_stdscr,s)
#define curses_innstr(s,n)		                   curses_winnstr(curses_stdscr,s,n)
#define curses_insch(c)		                      curses_winsch(curses_stdscr,c)
#define curses_insdelln(n)		                   curses_winsdelln(curses_stdscr,n)
#define curses_insertln()		                    curses_winsdelln(curses_stdscr,1)
#define curses_insnstr(s,n)		                  curses_winsnstr(curses_stdscr,s,n)
#define curses_insstr(s)		                     curses_winsstr(curses_stdscr,s)
#define curses_instr(s)		                      curses_winstr(curses_stdscr,s)
#define curses_move(y,x)		                     curses_wmove(curses_stdscr,y,x)
#define curses_refresh()		                     curses_wrefresh(curses_stdscr)
#define curses_scrl(n)			                      curses_wscrl(curses_stdscr,n)
#define curses_setscrreg(t,b)		                curses_wsetscrreg(curses_stdscr,t,b)
#define curses_standend()		                    curses_wstandend(curses_stdscr)
#define curses_standout()		                    curses_wstandout(curses_stdscr)
#define curses_timeout(delay)		                curses_wtimeout(curses_stdscr,delay)
#define curses_wdeleteln(win)		                curses_winsdelln(win,-1)
#define curses_winsertln(win)		                curses_winsdelln(win,1)
#define curses_mvwaddch(win,y,x,ch)		         (curses_wmove(win,y,x) == ERR ? ERR : curses_waddch(win,ch))
#define curses_mvwaddchnstr(win,y,x,str,n)   	(curses_wmove(win,y,x) == ERR ? ERR : curses_waddchnstr(win,str,n))
#define curses_mvwaddchstr(win,y,x,str)	      (curses_wmove(win,y,x) == ERR ? ERR : curses_waddchnstr(win,str,-1))
#define curses_mvwaddnstr(win,y,x,str,n)	     (curses_wmove(win,y,x) == ERR ? ERR : curses_waddnstr(win,str,n))
#define curses_mvwaddstr(win,y,x,str)		       (curses_wmove(win,y,x) == ERR ? ERR : curses_waddnstr(win,str,-1))
#define curses_mvwchgat(win,y,x,n,a,c,o)	     (curses_wmove(win,y,x) == ERR ? ERR : curses_wchgat(win,n,a,c,o))
#define curses_mvwdelch(win,y,x)		            (curses_wmove(win,y,x) == ERR ? ERR : curses_wdelch(win))
#define curses_mvwgetch(win,y,x)		            (curses_wmove(win,y,x) == ERR ? ERR : curses_wgetch(win))
#define curses_mvwgetnstr(win,y,x,str,n)	     (curses_wmove(win,y,x) == ERR ? ERR : curses_wgetnstr(win,str,n))
#define curses_mvwgetstr(win,y,x,str)		       (curses_wmove(win,y,x) == ERR ? ERR : curses_wgetstr(win,str))
#define curses_mvwhline(win,y,x,c,n)		        (curses_wmove(win,y,x) == ERR ? ERR : curses_whline(win,c,n))
#define curses_mvwinch(win,y,x)		             (curses_wmove(win,y,x) == ERR ? (chtype)ERR : curses_winch(win))
#define curses_mvwinchnstr(win,y,x,s,n)	      (curses_wmove(win,y,x) == ERR ? ERR : curses_winchnstr(win,s,n))
#define curses_mvwinchstr(win,y,x,s)		        (curses_wmove(win,y,x) == ERR ? ERR : curses_winchstr(win,s))
#define curses_mvwinnstr(win,y,x,s,n)		       (curses_wmove(win,y,x) == ERR ? ERR : curses_winnstr(win,s,n))
#define curses_mvwinsch(win,y,x,c)		          (curses_wmove(win,y,x) == ERR ? ERR : curses_winsch(win,c))
#define curses_mvwinsnstr(win,y,x,s,n)		      (curses_wmove(win,y,x) == ERR ? ERR : curses_winsnstr(win,s,n))
#define curses_mvwinsstr(win,y,x,s)		         (curses_wmove(win,y,x) == ERR ? ERR : curses_winsstr(win,s))
#define curses_mvwinstr(win,y,x,s)		          (curses_wmove(win,y,x) == ERR ? ERR : curses_winstr(win,s))
#define curses_mvwvline(win,y,x,c,n)		        (curses_wmove(win,y,x) == ERR ? ERR : curses_wvline(win,c,n))
#define curses_mvaddch(y,x,ch)			              curses_mvwaddch(curses_stdscr,y,x,ch)
#define curses_mvaddchnstr(y,x,str,n)		        curses_mvwaddchnstr(curses_stdscr,y,x,str,n)
#define curses_mvaddchstr(y,x,str)		           curses_mvwaddchstr(curses_stdscr,y,x,str)
#define curses_mvaddnstr(y,x,str,n)		          curses_mvwaddnstr(curses_stdscr,y,x,str,n)
#define curses_mvaddstr(y,x,str)		             curses_mvwaddstr(curses_stdscr,y,x,str)
#define curses_mvchgat(y,x,n,a,c,o)		          curses_mvwchgat(curses_stdscr,y,x,n,a,c,o)
#define curses_mvdelch(y,x)			                 curses_mvwdelch(curses_stdscr,y,x)
#define curses_mvgetch(y,x)			                 curses_mvwgetch(curses_stdscr,y,x)
#define curses_mvgetnstr(y,x,str,n)		          curses_mvwgetnstr(curses_stdscr,y,x,str,n)
#define curses_mvgetstr(y,x,str)		             curses_mvwgetstr(curses_stdscr,y,x,str)
#define curses_mvhline(y,x,c,n)		              curses_mvwhline(curses_stdscr,y,x,c,n)
#define curses_mvinch(y,x)			                  curses_mvwinch(curses_stdscr,y,x)
#define curses_mvinchnstr(y,x,s,n)		           curses_mvwinchnstr(curses_stdscr,y,x,s,n)
#define curses_mvinchstr(y,x,s)		              curses_mvwinchstr(curses_stdscr,y,x,s)
#define curses_mvinnstr(y,x,s,n)		             curses_mvwinnstr(curses_stdscr,y,x,s,n)
#define curses_mvinsch(y,x,c)			               curses_mvwinsch(curses_stdscr,y,x,c)
#define curses_mvinsnstr(y,x,s,n)		            curses_mvwinsnstr(curses_stdscr,y,x,s,n)
#define curses_mvinsstr(y,x,s)			              curses_mvwinsstr(curses_stdscr,y,x,s)
#define curses_mvinstr(y,x,s)			               curses_mvwinstr(curses_stdscr,y,x,s)
#define curses_mvvline(y,x,c,n)		              curses_mvwvline(curses_stdscr,y,x,c,n)
#define curses_slk_attr_off(a,v)		             ((v) ? ERR : curses_slk_attroff(a))
#define curses_slk_attr_on(a,v)		              ((v) ? ERR : curses_slk_attron(a))
#define curses_vw_printw                       curses_vwprintw
#define curses_vw_scanw                        curses_vwscanw

#ifndef GUARD_DEEMON_DEX_CURSES_C
// Override the curses library names with our's
#include "_curses.link_override.inl"
#endif


//////////////////////////////////////////////////////////////////////////
// Linux keycodes that our curses implementation uses
// >> These are the same as those specified in lib: <dex/curses>
// NOTE: Also included is the original copyright notice from <curses.h>

/****************************************************************************
 * Copyright (c) 1998-2015,2016 Free Software Foundation, Inc.              *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, distribute with modifications, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is    *
 * furnished to do so, subject to the following conditions:                 *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,   *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR    *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR    *
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.                               *
 *                                                                          *
 * Except as contained in this notice, the name(s) of the above copyright   *
 * holders shall not be used in advertising or otherwise to promote the     *
 * sale, use or other dealings in this Software without prior written       *
 * authorization.                                                           *
 ****************************************************************************/

/****************************************************************************
 *  Author: Zeyd M. Ben-Halim <zmbenhal@netcom.com> 1992,1995               *
 *     and: Eric S. Raymond <esr@snark.thyrsus.com>                         *
 *     and: Thomas E. Dickey                        1996-on                 *
 ****************************************************************************/
#define CURSES_PUBLIC_KEY_CODE_YES   0400         /* A wchar_t contains a key code */
#define CURSES_PUBLIC_KEY_MIN        0401         /* Minimum curses key */
#define CURSES_PUBLIC_KEY_BREAK      0401         /* Break key (unreliable) */
#define CURSES_PUBLIC_KEY_SRESET     0530         /* Soft (partial) reset (unreliable) */
#define CURSES_PUBLIC_KEY_RESET      0531         /* Reset or hard reset (unreliable) */
#define CURSES_PUBLIC_KEY_DOWN       0402         /* down-arrow key */
#define CURSES_PUBLIC_KEY_UP         0403         /* up-arrow key */
#define CURSES_PUBLIC_KEY_LEFT       0404         /* left-arrow key */
#define CURSES_PUBLIC_KEY_RIGHT      0405         /* right-arrow key */
#define CURSES_PUBLIC_KEY_HOME       0406         /* home key */
#define CURSES_PUBLIC_KEY_BACKSPACE  0407         /* backspace key */
#define CURSES_PUBLIC_KEY_F0         0410         /* Function keys.  Space for 64 */
/* hide form generator */#define CURSES_PUBLIC_KEY_F(n) (CURSES_PUBLIC_KEY_F0+(n))
#define CURSES_PUBLIC_KEY_DL         0510         /* delete-line key */
#define CURSES_PUBLIC_KEY_IL         0511         /* insert-line key */
#define CURSES_PUBLIC_KEY_DC         0512         /* delete-character key */
#define CURSES_PUBLIC_KEY_IC         0513         /* insert-character key */
#define CURSES_PUBLIC_KEY_EIC        0514         /* sent by rmir or smir in insert mode */
#define CURSES_PUBLIC_KEY_CLEAR      0515         /* clear-screen or erase key */
#define CURSES_PUBLIC_KEY_EOS        0516         /* clear-to-end-of-screen key */
#define CURSES_PUBLIC_KEY_EOL        0517         /* clear-to-end-of-line key */
#define CURSES_PUBLIC_KEY_SF         0520         /* scroll-forward key */
#define CURSES_PUBLIC_KEY_SR         0521         /* scroll-backward key */
#define CURSES_PUBLIC_KEY_NPAGE      0522         /* next-page key */
#define CURSES_PUBLIC_KEY_PPAGE      0523         /* previous-page key */
#define CURSES_PUBLIC_KEY_STAB       0524         /* set-tab key */
#define CURSES_PUBLIC_KEY_CTAB       0525         /* clear-tab key */
#define CURSES_PUBLIC_KEY_CATAB      0526         /* clear-all-tabs key */
#define CURSES_PUBLIC_KEY_ENTER      0527         /* enter/send key */
#define CURSES_PUBLIC_KEY_PRINT      0532         /* print key */
#define CURSES_PUBLIC_KEY_LL         0533         /* lower-left key (home down) */
#define CURSES_PUBLIC_KEY_A1         0534         /* upper left of keypad */
#define CURSES_PUBLIC_KEY_A3         0535         /* upper right of keypad */
#define CURSES_PUBLIC_KEY_B2         0536         /* center of keypad */
#define CURSES_PUBLIC_KEY_C1         0537         /* lower left of keypad */
#define CURSES_PUBLIC_KEY_C3         0540         /* lower right of keypad */
#define CURSES_PUBLIC_KEY_BTAB       0541         /* back-tab key */
#define CURSES_PUBLIC_KEY_BEG        0542         /* begin key */
#define CURSES_PUBLIC_KEY_CANCEL     0543         /* cancel key */
#define CURSES_PUBLIC_KEY_CLOSE      0544         /* close key */
#define CURSES_PUBLIC_KEY_COMMAND    0545         /* command key */
#define CURSES_PUBLIC_KEY_COPY       0546         /* copy key */
#define CURSES_PUBLIC_KEY_CREATE     0547         /* create key */
#define CURSES_PUBLIC_KEY_END        0550         /* end key */
#define CURSES_PUBLIC_KEY_EXIT       0551         /* exit key */
#define CURSES_PUBLIC_KEY_FIND       0552         /* find key */
#define CURSES_PUBLIC_KEY_HELP       0553         /* help key */
#define CURSES_PUBLIC_KEY_MARK       0554         /* mark key */
#define CURSES_PUBLIC_KEY_MESSAGE    0555         /* message key */
#define CURSES_PUBLIC_KEY_MOVE       0556         /* move key */
#define CURSES_PUBLIC_KEY_NEXT       0557         /* next key */
#define CURSES_PUBLIC_KEY_OPEN       0560         /* open key */
#define CURSES_PUBLIC_KEY_OPTIONS    0561         /* options key */
#define CURSES_PUBLIC_KEY_PREVIOUS   0562         /* previous key */
#define CURSES_PUBLIC_KEY_REDO       0563         /* redo key */
#define CURSES_PUBLIC_KEY_REFERENCE  0564         /* reference key */
#define CURSES_PUBLIC_KEY_REFRESH    0565         /* refresh key */
#define CURSES_PUBLIC_KEY_REPLACE    0566         /* replace key */
#define CURSES_PUBLIC_KEY_RESTART    0567         /* restart key */
#define CURSES_PUBLIC_KEY_RESUME     0570         /* resume key */
#define CURSES_PUBLIC_KEY_SAVE       0571         /* save key */
#define CURSES_PUBLIC_KEY_SBEG       0572         /* shifted begin key */
#define CURSES_PUBLIC_KEY_SCANCEL    0573         /* shifted cancel key */
#define CURSES_PUBLIC_KEY_SCOMMAND   0574         /* shifted command key */
#define CURSES_PUBLIC_KEY_SCOPY      0575         /* shifted copy key */
#define CURSES_PUBLIC_KEY_SCREATE    0576         /* shifted create key */
#define CURSES_PUBLIC_KEY_SDC        0577         /* shifted delete-character key */
#define CURSES_PUBLIC_KEY_SDL        0600         /* shifted delete-line key */
#define CURSES_PUBLIC_KEY_SELECT     0601         /* select key */
#define CURSES_PUBLIC_KEY_SEND       0602         /* shifted end key */
#define CURSES_PUBLIC_KEY_SEOL       0603         /* shifted clear-to-end-of-line key */
#define CURSES_PUBLIC_KEY_SEXIT      0604         /* shifted exit key */
#define CURSES_PUBLIC_KEY_SFIND      0605         /* shifted find key */
#define CURSES_PUBLIC_KEY_SHELP      0606         /* shifted help key */
#define CURSES_PUBLIC_KEY_SHOME      0607         /* shifted home key */
#define CURSES_PUBLIC_KEY_SIC        0610         /* shifted insert-character key */
#define CURSES_PUBLIC_KEY_SLEFT      0611         /* shifted left-arrow key */
#define CURSES_PUBLIC_KEY_SMESSAGE   0612         /* shifted message key */
#define CURSES_PUBLIC_KEY_SMOVE      0613         /* shifted move key */
#define CURSES_PUBLIC_KEY_SNEXT      0614         /* shifted next key */
#define CURSES_PUBLIC_KEY_SOPTIONS   0615         /* shifted options key */
#define CURSES_PUBLIC_KEY_SPREVIOUS  0616         /* shifted previous key */
#define CURSES_PUBLIC_KEY_SPRINT     0617         /* shifted print key */
#define CURSES_PUBLIC_KEY_SREDO      0620         /* shifted redo key */
#define CURSES_PUBLIC_KEY_SREPLACE   0621         /* shifted replace key */
#define CURSES_PUBLIC_KEY_SRIGHT     0622         /* shifted right-arrow key */
#define CURSES_PUBLIC_KEY_SRSUME     0623         /* shifted resume key */
#define CURSES_PUBLIC_KEY_SSAVE      0624         /* shifted save key */
#define CURSES_PUBLIC_KEY_SSUSPEND   0625         /* shifted suspend key */
#define CURSES_PUBLIC_KEY_SUNDO      0626         /* shifted undo key */
#define CURSES_PUBLIC_KEY_SUSPEND    0627         /* suspend key */
#define CURSES_PUBLIC_KEY_UNDO       0630         /* undo key */
#define CURSES_PUBLIC_KEY_MOUSE      0631         /* Mouse event has occurred */
#define CURSES_PUBLIC_KEY_RESIZE     0632         /* Terminal resize event */
#define CURSES_PUBLIC_KEY_EVENT      0633         /* We were interrupted by an event */
#define CURSES_PUBLIC_KEY_MAX        0777         /* Maximum key value is 0633 */


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_CURSES_H */
