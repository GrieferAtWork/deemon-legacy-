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
#ifndef GUARD_DEEMON_DEX_CURSES_API_INL
#define GUARD_DEEMON_DEX_CURSES_API_INL 1
#ifdef __DEEMON__
#pragma warning(disable: 16)
#endif

#include "_curses.h"

DEE_DECL_BEGIN

#define EXPORT_NAME_PREFIX "xcurses_"

#ifdef DEE_PLATFORM_WINDOWS
#define vid_puts(attrs,pair,opts,putc) vid_puts(attrs,pair,opts,(int(*)(int))(putc))
#endif

/*[[[deemon
#include <file>
#include <fs>

fs::chdir(fs::path::head(__FILE__));

found_exports = []; // list { tuple { int id, string name, string rett, string callc, string args } }
local in_exports = false;
for (local line: file.open("_curses.h")) {
  line = line.strip();
  if (in_exports) {
    if (line == "//@autoexport end") in_exports = false;
    else if (line.startswith("#define curses_")) {
      local name,id,rett,callc,argtstr;
      line = line[#"#define curses_":];
      try name,line = line.scanf("%[^ ] %[^]")...;
      catch (...) continue;
      line = line.lstrip("(");
      try id,rett,callc,argtstr = line.scanf("DEE_CURSES_API(%[^,] , %[^(](%[^*]*)(%[^]")...;
      catch (...) continue;
      id = id.strip(),rett = rett.strip();
      callc = callc.strip(),argtstr = argtstr.strip();
      argtstr = argtstr[:#argtstr-2].strip();
      if (argtstr.endswith("(")) {
        argtstr = argtstr[:#argtstr-1].strip();
        argtstr = argtstr[:#argtstr-1].strip();
        argtstr = argtstr[:#argtstr-1].strip();
        argtstr = argtstr.rsstrip("void");
      }
      id = (int)id;
      if (id >= #found_exports) found_exports.resize(id+1,none);
      else if (found_exports[id] != none) {
        print file.io.stderr: "Id",id,"used more than once";
      }
      found_exports[id] = pack id,name,rett,callc,argtstr;
      if (line.endswith("())")) {
        // Special handling for variables
        print "static",rett,callc,EXPORT_NAME_PREFIX+name+"(void) { return "+name+"; }";
      }
    }
  } else {
    if (line == "//@autoexport begin") in_exports = true;
  }
}

if (DEE_CURSES_API_SIZE != #found_exports) {
  print file.io.stderr: "Invalid '"+DEE_CURSES_API_SIZE+"' (actual export size:",#found_exports,")";
}

function remove_typenames(args) {
  args = args.replace("(","");
  args = args.replace(")","");
  args = args.replace("FILE","");
  args = args.replace("SCREEN","");
  args = args.replace("WINDOW","");
  args = args.replace("bool","");
  args = args.replace("char","");
  args = args.replace("chtype","");
  args = args.replace("attr_t","");
  args = args.replace("const","");
  args = args.replace("int","");
  args = args.replace("short","");
  args = args.replace("void","");
  args = args.replace("va_list","");
  args = args.replace("unsigned","");
  args = args.replace("*","");
  args = args.replace(" ","");
  args = args.replace(",,",",");
  return args.strip(",");
}

for (local entry: found_exports) {
  if (entry !is none) {
    local id,name,rett,callc,args = entry...;
    if (args) {
      local check_defined = name != "getsyx_impl_";
      if (check_defined) print "#ifdef",name;
      print "static",rett,callc,EXPORT_NAME_PREFIX+name,"("+args+") { ",;
      if (args.endswith("...")) {
        args = args.rsstrip("...").rstrip().rstrip(",");
        argnames = remove_typenames(args);
        if (rett != "void") print rett,"result;",;
        print "va_list args; va_start(args,"+("," in argnames ? argnames.rpartition(",")[2] : argnames)+"); ",;
        if (rett != "void") print "result = ",;
        print name+"("+argnames+"); va_end(args); ",;
        if (rett != "void") print "return result; ",;
        print "}";
      } else {
        if (rett != "void") print "return ",;
        if (name == "getsyx_impl_") {
          print "getsyx(*y,*x); }";
        } else {
          print name+"("+remove_typenames(args)+"); }";
        }
      }
      if (check_defined) print "#else";
      if (check_defined) print "#define",EXPORT_NAME_PREFIX+name,name;
      if (check_defined) print "#endif";
    }
  }
}

print "void *DeeCurses_API[DEE_CURSES_API_SIZE] = {";

local i = 0;
for (local entry: found_exports) {
  print "/" "* 0x%.2X *" "/" % i,;
  if (entry is none) {
    print "NULL /" "* UNUSED *" "/",;
  } else {
    local id,name,rett,callc,args = entry...;
    print "(void *)&"+EXPORT_NAME_PREFIX+name,;
  }
  ++i;
  print ",";
}
print "};";
]]]*/
static WINDOW * DEE_CALL xcurses_curscr(void) { return curscr; }
static WINDOW * DEE_CALL xcurses_stdscr(void) { return stdscr; }
static char * DEE_CALL xcurses_ttytype(void) { return ttytype; }
static int DEE_CALL xcurses_COLORS(void) { return COLORS; }
static int DEE_CALL xcurses_COLOR_PAIRS(void) { return COLOR_PAIRS; }
static int DEE_CALL xcurses_COLS(void) { return COLS; }
static int DEE_CALL xcurses_LINES(void) { return LINES; }
static int DEE_CALL xcurses_TABSIZE(void) { return TABSIZE; }
#ifdef def_prog_mode
static int DEE_CURSES_CALL xcurses_def_prog_mode (void) { return def_prog_mode(); }
#else
#define xcurses_def_prog_mode def_prog_mode
#endif
#ifdef def_shell_mode
static int DEE_CURSES_CALL xcurses_def_shell_mode (void) { return def_shell_mode(); }
#else
#define xcurses_def_shell_mode def_shell_mode
#endif
#ifdef reset_prog_mode
static int DEE_CURSES_CALL xcurses_reset_prog_mode (void) { return reset_prog_mode(); }
#else
#define xcurses_reset_prog_mode reset_prog_mode
#endif
#ifdef reset_shell_mode
static int DEE_CURSES_CALL xcurses_reset_shell_mode (void) { return reset_shell_mode(); }
#else
#define xcurses_reset_shell_mode reset_shell_mode
#endif
#ifdef resetty
static int DEE_CURSES_CALL xcurses_resetty (void) { return resetty(); }
#else
#define xcurses_resetty resetty
#endif
#ifdef savetty
static int DEE_CURSES_CALL xcurses_savetty (void) { return savetty(); }
#else
#define xcurses_savetty savetty
#endif
static void DEE_CURSES_CALL xcurses_getsyx_impl_ (int *y, int *x) { getsyx(*y,*x); }
#ifdef ripoffline
static int DEE_CURSES_CALL xcurses_ripoffline (int line, int (*init)(WINDOW *, int)) { return ripoffline(line,init); }
#else
#define xcurses_ripoffline ripoffline
#endif
#ifdef curs_set
static int DEE_CURSES_CALL xcurses_curs_set (int visibility) { return curs_set(visibility); }
#else
#define xcurses_curs_set curs_set
#endif
#ifdef napms
static int DEE_CURSES_CALL xcurses_napms (int ms) { return napms(ms); }
#else
#define xcurses_napms napms
#endif
#ifdef wrefresh
static int DEE_CURSES_CALL xcurses_wrefresh (WINDOW *win) { return wrefresh(win); }
#else
#define xcurses_wrefresh wrefresh
#endif
#ifdef wnoutrefresh
static int DEE_CURSES_CALL xcurses_wnoutrefresh (WINDOW *win) { return wnoutrefresh(win); }
#else
#define xcurses_wnoutrefresh wnoutrefresh
#endif
#ifdef doupdate
static int DEE_CURSES_CALL xcurses_doupdate (void) { return doupdate(); }
#else
#define xcurses_doupdate doupdate
#endif
#ifdef redrawwin
static int DEE_CURSES_CALL xcurses_redrawwin (WINDOW *win) { return redrawwin(win); }
#else
#define xcurses_redrawwin redrawwin
#endif
#ifdef wredrawln
static int DEE_CURSES_CALL xcurses_wredrawln (WINDOW *win, int beg_line, int num_lines) { return wredrawln(win,beg_line,num_lines); }
#else
#define xcurses_wredrawln wredrawln
#endif
#ifdef getbegx
static int DEE_CURSES_CALL xcurses_getbegx (WINDOW *win) { return getbegx(win); }
#else
#define xcurses_getbegx getbegx
#endif
#ifdef getbegy
static int DEE_CURSES_CALL xcurses_getbegy (WINDOW *win) { return getbegy(win); }
#else
#define xcurses_getbegy getbegy
#endif
#ifdef getmaxx
static int DEE_CURSES_CALL xcurses_getmaxx (WINDOW *win) { return getmaxx(win); }
#else
#define xcurses_getmaxx getmaxx
#endif
#ifdef getmaxy
static int DEE_CURSES_CALL xcurses_getmaxy (WINDOW *win) { return getmaxy(win); }
#else
#define xcurses_getmaxy getmaxy
#endif
#ifdef getparx
static int DEE_CURSES_CALL xcurses_getparx (WINDOW *win) { return getparx(win); }
#else
#define xcurses_getparx getparx
#endif
#ifdef getpary
static int DEE_CURSES_CALL xcurses_getpary (WINDOW *win) { return getpary(win); }
#else
#define xcurses_getpary getpary
#endif
#ifdef getcurx
static int DEE_CURSES_CALL xcurses_getcurx (WINDOW *win) { return getcurx(win); }
#else
#define xcurses_getcurx getcurx
#endif
#ifdef getcury
static int DEE_CURSES_CALL xcurses_getcury (WINDOW *win) { return getcury(win); }
#else
#define xcurses_getcury getcury
#endif
#ifdef clearok
static int DEE_CURSES_CALL xcurses_clearok (WINDOW *win, unsigned char bf) { return clearok(win,bf); }
#else
#define xcurses_clearok clearok
#endif
#ifdef idlok
static int DEE_CURSES_CALL xcurses_idlok (WINDOW *win, unsigned char bf) { return idlok(win,bf); }
#else
#define xcurses_idlok idlok
#endif
#ifdef idcok
static void DEE_CURSES_CALL xcurses_idcok (WINDOW *win, unsigned char bf) { idcok(win,bf); }
#else
#define xcurses_idcok idcok
#endif
#ifdef immedok
static void DEE_CURSES_CALL xcurses_immedok (WINDOW *win, unsigned char bf) { immedok(win,bf); }
#else
#define xcurses_immedok immedok
#endif
#ifdef leaveok
static int DEE_CURSES_CALL xcurses_leaveok (WINDOW *win, unsigned char bf) { return leaveok(win,bf); }
#else
#define xcurses_leaveok leaveok
#endif
#ifdef wsetscrreg
static int DEE_CURSES_CALL xcurses_wsetscrreg (WINDOW *win, int top, int bot) { return wsetscrreg(win,top,bot); }
#else
#define xcurses_wsetscrreg wsetscrreg
#endif
#ifdef scrollok
static int DEE_CURSES_CALL xcurses_scrollok (WINDOW *win, unsigned char bf) { return scrollok(win,bf); }
#else
#define xcurses_scrollok scrollok
#endif
#ifdef nl
static int DEE_CURSES_CALL xcurses_nl (void) { return nl(); }
#else
#define xcurses_nl nl
#endif
#ifdef nonl
static int DEE_CURSES_CALL xcurses_nonl (void) { return nonl(); }
#else
#define xcurses_nonl nonl
#endif
#ifdef start_color
static int DEE_CURSES_CALL xcurses_start_color (void) { return start_color(); }
#else
#define xcurses_start_color start_color
#endif
#ifdef init_pair
static int DEE_CURSES_CALL xcurses_init_pair (short pair, short f, short b) { return init_pair(pair,f,b); }
#else
#define xcurses_init_pair init_pair
#endif
#ifdef init_color
static int DEE_CURSES_CALL xcurses_init_color (short color, short r, short g, short b) { return init_color(color,r,g,b); }
#else
#define xcurses_init_color init_color
#endif
#ifdef has_colors
static unsigned char DEE_CURSES_CALL xcurses_has_colors (void) { return has_colors(); }
#else
#define xcurses_has_colors has_colors
#endif
#ifdef can_change_color
static unsigned char DEE_CURSES_CALL xcurses_can_change_color (void) { return can_change_color(); }
#else
#define xcurses_can_change_color can_change_color
#endif
#ifdef color_content
static int DEE_CURSES_CALL xcurses_color_content (short color, short *r, short *g, short *b) { return color_content(color,r,g,b); }
#else
#define xcurses_color_content color_content
#endif
#ifdef pair_content
static int DEE_CURSES_CALL xcurses_pair_content (short pair, short *f, short *b) { return pair_content(pair,f,b); }
#else
#define xcurses_pair_content pair_content
#endif
#ifdef overlay
static int DEE_CURSES_CALL xcurses_overlay (const WINDOW *srcwin, WINDOW *dstwin) { return overlay(srcwin,dstwin); }
#else
#define xcurses_overlay overlay
#endif
#ifdef overwrite
static int DEE_CURSES_CALL xcurses_overwrite (const WINDOW *srcwin, WINDOW *dstwin) { return overwrite(srcwin,dstwin); }
#else
#define xcurses_overwrite overwrite
#endif
#ifdef copywin
static int DEE_CURSES_CALL xcurses_copywin (const WINDOW *srcwin, WINDOW *dstwin, int sminrow, int smincol, int dminrow, int dmincol, int dmaxrow, int dmaxcol, int overlay_) { return copywin(srcwin,dstwin,sminrow,smincol,dminrow,dmincol,dmaxrow,dmaxcol,overlay_); }
#else
#define xcurses_copywin copywin
#endif
#ifdef keyname
static char   * DEE_CURSES_CALL xcurses_keyname (int c) { return keyname(c); }
#else
#define xcurses_keyname keyname
#endif
#ifdef filter
static void DEE_CURSES_CALL xcurses_filter (void) { filter(); }
#else
#define xcurses_filter filter
#endif
#ifdef use_env
static void DEE_CURSES_CALL xcurses_use_env (unsigned char f) { use_env(f); }
#else
#define xcurses_use_env use_env
#endif
#ifdef putwin
static int DEE_CURSES_CALL xcurses_putwin (WINDOW *win, FILE *filep) { return putwin(win,filep); }
#else
#define xcurses_putwin putwin
#endif
#ifdef getwin
static WINDOW * DEE_CURSES_CALL xcurses_getwin (FILE *filep) { return getwin(filep); }
#else
#define xcurses_getwin getwin
#endif
#ifdef delay_output
static int DEE_CURSES_CALL xcurses_delay_output (int ms) { return delay_output(ms); }
#else
#define xcurses_delay_output delay_output
#endif
#ifdef flushinp
static int DEE_CURSES_CALL xcurses_flushinp (void) { return flushinp(); }
#else
#define xcurses_flushinp flushinp
#endif
#ifdef initscr
static WINDOW * DEE_CURSES_CALL xcurses_initscr (void) { return initscr(); }
#else
#define xcurses_initscr initscr
#endif
#ifdef endwin
static int DEE_CURSES_CALL xcurses_endwin (void) { return endwin(); }
#else
#define xcurses_endwin endwin
#endif
#ifdef isendwin
static unsigned char DEE_CURSES_CALL xcurses_isendwin (void) { return isendwin(); }
#else
#define xcurses_isendwin isendwin
#endif
#ifdef newterm
static SCREEN * DEE_CURSES_CALL xcurses_newterm (char *type, FILE *outfd, FILE *infd) { return newterm(type,outfd,infd); }
#else
#define xcurses_newterm newterm
#endif
#ifdef set_term
static SCREEN * DEE_CURSES_CALL xcurses_set_term (SCREEN *new_) { return set_term(new_); }
#else
#define xcurses_set_term set_term
#endif
#ifdef delscreen
static void DEE_CURSES_CALL xcurses_delscreen (SCREEN* sp) { delscreen(sp); }
#else
#define xcurses_delscreen delscreen
#endif
#ifdef newwin
static WINDOW * DEE_CURSES_CALL xcurses_newwin (int nlines, int ncols, int begin_y, int begin_x) { return newwin(nlines,ncols,begin_y,begin_x); }
#else
#define xcurses_newwin newwin
#endif
#ifdef delwin
static int DEE_CURSES_CALL xcurses_delwin (WINDOW *win) { return delwin(win); }
#else
#define xcurses_delwin delwin
#endif
#ifdef mvwin
static int DEE_CURSES_CALL xcurses_mvwin (WINDOW *win, int y, int x) { return mvwin(win,y,x); }
#else
#define xcurses_mvwin mvwin
#endif
#ifdef subwin
static WINDOW * DEE_CURSES_CALL xcurses_subwin (WINDOW *orig, int nlines, int ncols, int begin_y, int begin_x) { return subwin(orig,nlines,ncols,begin_y,begin_x); }
#else
#define xcurses_subwin subwin
#endif
#ifdef derwin
static WINDOW * DEE_CURSES_CALL xcurses_derwin (WINDOW *orig, int nlines, int ncols, int begin_y, int begin_x) { return derwin(orig,nlines,ncols,begin_y,begin_x); }
#else
#define xcurses_derwin derwin
#endif
#ifdef mvderwin
static int DEE_CURSES_CALL xcurses_mvderwin (WINDOW *win, int par_y, int par_x) { return mvderwin(win,par_y,par_x); }
#else
#define xcurses_mvderwin mvderwin
#endif
#ifdef dupwin
static WINDOW * DEE_CURSES_CALL xcurses_dupwin (WINDOW *win) { return dupwin(win); }
#else
#define xcurses_dupwin dupwin
#endif
#ifdef wsyncup
static void DEE_CURSES_CALL xcurses_wsyncup (WINDOW *win) { wsyncup(win); }
#else
#define xcurses_wsyncup wsyncup
#endif
#ifdef syncok
static int DEE_CURSES_CALL xcurses_syncok (WINDOW *win, unsigned char bf) { return syncok(win,bf); }
#else
#define xcurses_syncok syncok
#endif
#ifdef wcursyncup
static void DEE_CURSES_CALL xcurses_wcursyncup (WINDOW *win) { wcursyncup(win); }
#else
#define xcurses_wcursyncup wcursyncup
#endif
#ifdef wsyncdown
static void DEE_CURSES_CALL xcurses_wsyncdown (WINDOW *win) { wsyncdown(win); }
#else
#define xcurses_wsyncdown wsyncdown
#endif
#ifdef wbkgdset
static void DEE_CURSES_CALL xcurses_wbkgdset (WINDOW *win, chtype ch) { wbkgdset(win,ch); }
#else
#define xcurses_wbkgdset wbkgdset
#endif
#ifdef wbkgd
static int DEE_CURSES_CALL xcurses_wbkgd (WINDOW *win, chtype ch) { return wbkgd(win,ch); }
#else
#define xcurses_wbkgd wbkgd
#endif
#ifdef getbkgd
static chtype DEE_CURSES_CALL xcurses_getbkgd (WINDOW *win) { return getbkgd(win); }
#else
#define xcurses_getbkgd getbkgd
#endif
#ifdef cbreak
static int DEE_CURSES_CALL xcurses_cbreak (void) { return cbreak(); }
#else
#define xcurses_cbreak cbreak
#endif
#ifdef nocbreak
static int DEE_CURSES_CALL xcurses_nocbreak (void) { return nocbreak(); }
#else
#define xcurses_nocbreak nocbreak
#endif
#ifdef echo
static int DEE_CURSES_CALL xcurses_echo (void) { return echo(); }
#else
#define xcurses_echo echo
#endif
#ifdef noecho
static int DEE_CURSES_CALL xcurses_noecho (void) { return noecho(); }
#else
#define xcurses_noecho noecho
#endif
#ifdef halfdelay
static int DEE_CURSES_CALL xcurses_halfdelay (int tenths) { return halfdelay(tenths); }
#else
#define xcurses_halfdelay halfdelay
#endif
#ifdef intrflush
static int DEE_CURSES_CALL xcurses_intrflush (WINDOW *win, unsigned char bf) { return intrflush(win,bf); }
#else
#define xcurses_intrflush intrflush
#endif
#ifdef keypad
static int DEE_CURSES_CALL xcurses_keypad (WINDOW *win, unsigned char bf) { return keypad(win,bf); }
#else
#define xcurses_keypad keypad
#endif
#ifdef meta
static int DEE_CURSES_CALL xcurses_meta (WINDOW *win, unsigned char bf) { return meta(win,bf); }
#else
#define xcurses_meta meta
#endif
#ifdef nodelay
static int DEE_CURSES_CALL xcurses_nodelay (WINDOW *win, unsigned char bf) { return nodelay(win,bf); }
#else
#define xcurses_nodelay nodelay
#endif
#ifdef raw
static int DEE_CURSES_CALL xcurses_raw (void) { return raw(); }
#else
#define xcurses_raw raw
#endif
#ifdef noraw
static int DEE_CURSES_CALL xcurses_noraw (void) { return noraw(); }
#else
#define xcurses_noraw noraw
#endif
#ifdef noqiflush
static void DEE_CURSES_CALL xcurses_noqiflush (void) { noqiflush(); }
#else
#define xcurses_noqiflush noqiflush
#endif
#ifdef qiflush
static void DEE_CURSES_CALL xcurses_qiflush (void) { qiflush(); }
#else
#define xcurses_qiflush qiflush
#endif
#ifdef notimeout
static int DEE_CURSES_CALL xcurses_notimeout (WINDOW *win, unsigned char bf) { return notimeout(win,bf); }
#else
#define xcurses_notimeout notimeout
#endif
#ifdef wtimeout
static void DEE_CURSES_CALL xcurses_wtimeout (WINDOW *win, int delay) { wtimeout(win,delay); }
#else
#define xcurses_wtimeout wtimeout
#endif
#ifdef typeahead
static int DEE_CURSES_CALL xcurses_typeahead (int fd) { return typeahead(fd); }
#else
#define xcurses_typeahead typeahead
#endif
#ifdef baudrate
static int DEE_CURSES_CALL xcurses_baudrate (void) { return baudrate(); }
#else
#define xcurses_baudrate baudrate
#endif
#ifdef erasechar
static char DEE_CURSES_CALL xcurses_erasechar (void) { return erasechar(); }
#else
#define xcurses_erasechar erasechar
#endif
#ifdef has_ic
static unsigned char DEE_CURSES_CALL xcurses_has_ic (void) { return has_ic(); }
#else
#define xcurses_has_ic has_ic
#endif
#ifdef has_il
static unsigned char DEE_CURSES_CALL xcurses_has_il (void) { return has_il(); }
#else
#define xcurses_has_il has_il
#endif
#ifdef killchar
static char DEE_CURSES_CALL xcurses_killchar (void) { return killchar(); }
#else
#define xcurses_killchar killchar
#endif
#ifdef longname
static char   * DEE_CURSES_CALL xcurses_longname (void) { return longname(); }
#else
#define xcurses_longname longname
#endif
#ifdef term_attrs
static attr_t DEE_CURSES_CALL xcurses_term_attrs (void) { return term_attrs(); }
#else
#define xcurses_term_attrs term_attrs
#endif
#ifdef termattrs
static chtype DEE_CURSES_CALL xcurses_termattrs (void) { return termattrs(); }
#else
#define xcurses_termattrs termattrs
#endif
#ifdef termname
static char   * DEE_CURSES_CALL xcurses_termname (void) { return termname(); }
#else
#define xcurses_termname termname
#endif
#ifdef beep
static int DEE_CURSES_CALL xcurses_beep (void) { return beep(); }
#else
#define xcurses_beep beep
#endif
#ifdef flash
static int DEE_CURSES_CALL xcurses_flash (void) { return flash(); }
#else
#define xcurses_flash flash
#endif
#ifdef wtouchln
static int DEE_CURSES_CALL xcurses_wtouchln (WINDOW *win, int y, int n, int changed) { return wtouchln(win,y,n,changed); }
#else
#define xcurses_wtouchln wtouchln
#endif
#ifdef is_linetouched
static unsigned char DEE_CURSES_CALL xcurses_is_linetouched (WINDOW *win, int line) { return is_linetouched(win,line); }
#else
#define xcurses_is_linetouched is_linetouched
#endif
#ifdef is_wintouched
static unsigned char DEE_CURSES_CALL xcurses_is_wintouched (WINDOW *win) { return is_wintouched(win); }
#else
#define xcurses_is_wintouched is_wintouched
#endif
#ifdef vidputs
static int DEE_CURSES_CALL xcurses_vidputs (chtype attrs, int (*putc)(int)) { return vidputs(attrs,putc); }
#else
#define xcurses_vidputs vidputs
#endif
#ifdef vidattr
static int DEE_CURSES_CALL xcurses_vidattr (chtype attrs) { return vidattr(attrs); }
#else
#define xcurses_vidattr vidattr
#endif
#ifdef vid_attr
static int DEE_CURSES_CALL xcurses_vid_attr (attr_t attrs, short pair, void *opts) { return vid_attr(attrs,pair,opts); }
#else
#define xcurses_vid_attr vid_attr
#endif
#ifdef mvcur
static int DEE_CURSES_CALL xcurses_mvcur (int oldrow, int oldcol, int newrow, int newcol) { return mvcur(oldrow,oldcol,newrow,newcol); }
#else
#define xcurses_mvcur mvcur
#endif
#ifdef printw
static int DEE_CURSES_CALL xcurses_printw (const char *fmt, ...) { int result;va_list args; va_start(args,fmt); result = printw(fmt); va_end(args); return result; }
#else
#define xcurses_printw printw
#endif
#ifdef wprintw
static int DEE_CURSES_CALL xcurses_wprintw (WINDOW *win, const char *fmt, ...) { int result;va_list args; va_start(args,fmt); result = wprintw(win,fmt); va_end(args); return result; }
#else
#define xcurses_wprintw wprintw
#endif
#ifdef mvprintw
static int DEE_CURSES_CALL xcurses_mvprintw (int y, int x, const char *fmt, ...) { int result;va_list args; va_start(args,fmt); result = mvprintw(y,x,fmt); va_end(args); return result; }
#else
#define xcurses_mvprintw mvprintw
#endif
#ifdef mvwprintw
static int DEE_CURSES_CALL xcurses_mvwprintw (WINDOW *win, int y, int x, const char *fmt, ...) { int result;va_list args; va_start(args,fmt); result = mvwprintw(win,y,x,fmt); va_end(args); return result; }
#else
#define xcurses_mvwprintw mvwprintw
#endif
#ifdef vwprintw
static int DEE_CURSES_CALL xcurses_vwprintw (WINDOW *win, const char *fmt, va_list varglist) { return vwprintw(win,fmt,varglist); }
#else
#define xcurses_vwprintw vwprintw
#endif
#ifdef scanw
static int DEE_CURSES_CALL xcurses_scanw (char *fmt, ...) { int result;va_list args; va_start(args,fmt); result = scanw(fmt); va_end(args); return result; }
#else
#define xcurses_scanw scanw
#endif
#ifdef wscanw
static int DEE_CURSES_CALL xcurses_wscanw (WINDOW *win, char *fmt, ...) { int result;va_list args; va_start(args,fmt); result = wscanw(win,fmt); va_end(args); return result; }
#else
#define xcurses_wscanw wscanw
#endif
#ifdef mvscanw
static int DEE_CURSES_CALL xcurses_mvscanw (int y, int x, char *fmt, ...) { int result;va_list args; va_start(args,fmt); result = mvscanw(y,x,fmt); va_end(args); return result; }
#else
#define xcurses_mvscanw mvscanw
#endif
#ifdef mvwscanw
static int DEE_CURSES_CALL xcurses_mvwscanw (WINDOW *win, int y, int x, char *fmt, ...) { int result;va_list args; va_start(args,fmt); result = mvwscanw(win,y,x,fmt); va_end(args); return result; }
#else
#define xcurses_mvwscanw mvwscanw
#endif
#ifdef vwscanw
static int DEE_CURSES_CALL xcurses_vwscanw (WINDOW *win, char *fmt, va_list varglist) { return vwscanw(win,fmt,varglist); }
#else
#define xcurses_vwscanw vwscanw
#endif
#ifdef newpad
static WINDOW * DEE_CURSES_CALL xcurses_newpad (int nlines, int ncols) { return newpad(nlines,ncols); }
#else
#define xcurses_newpad newpad
#endif
#ifdef subpad
static WINDOW * DEE_CURSES_CALL xcurses_subpad (WINDOW *orig, int nlines, int ncols, int begin_y, int begin_x) { return subpad(orig,nlines,ncols,begin_y,begin_x); }
#else
#define xcurses_subpad subpad
#endif
#ifdef prefresh
static int DEE_CURSES_CALL xcurses_prefresh (WINDOW *pad, int pminrow, int pmincol, int sminrow, int smincol, int smaxrow, int smaxcol) { return prefresh(pad,pminrow,pmincol,sminrow,smincol,smaxrow,smaxcol); }
#else
#define xcurses_prefresh prefresh
#endif
#ifdef pnoutrefresh
static int DEE_CURSES_CALL xcurses_pnoutrefresh (WINDOW *pad, int pminrow, int pmincol, int sminrow, int smincol, int smaxrow, int smaxcol) { return pnoutrefresh(pad,pminrow,pmincol,sminrow,smincol,smaxrow,smaxcol); }
#else
#define xcurses_pnoutrefresh pnoutrefresh
#endif
#ifdef pechochar
static int DEE_CURSES_CALL xcurses_pechochar (WINDOW *pad, chtype ch) { return pechochar(pad,ch); }
#else
#define xcurses_pechochar pechochar
#endif
#ifdef scr_dump
static int DEE_CURSES_CALL xcurses_scr_dump (const char *filename) { return scr_dump(filename); }
#else
#define xcurses_scr_dump scr_dump
#endif
#ifdef scr_restore
static int DEE_CURSES_CALL xcurses_scr_restore (const char *filename) { return scr_restore(filename); }
#else
#define xcurses_scr_restore scr_restore
#endif
#ifdef scr_init
static int DEE_CURSES_CALL xcurses_scr_init (const char *filename) { return scr_init(filename); }
#else
#define xcurses_scr_init scr_init
#endif
#ifdef scr_set
static int DEE_CURSES_CALL xcurses_scr_set (const char *filename) { return scr_set(filename); }
#else
#define xcurses_scr_set scr_set
#endif
#ifdef slk_init
static int DEE_CURSES_CALL xcurses_slk_init (int fmt) { return slk_init(fmt); }
#else
#define xcurses_slk_init slk_init
#endif
#ifdef slk_set
static int DEE_CURSES_CALL xcurses_slk_set (int labnum, const char *label, int fmt) { return slk_set(labnum,label,fmt); }
#else
#define xcurses_slk_set slk_set
#endif
#ifdef slk_refresh
static int DEE_CURSES_CALL xcurses_slk_refresh (void) { return slk_refresh(); }
#else
#define xcurses_slk_refresh slk_refresh
#endif
#ifdef slk_noutrefresh
static int DEE_CURSES_CALL xcurses_slk_noutrefresh (void) { return slk_noutrefresh(); }
#else
#define xcurses_slk_noutrefresh slk_noutrefresh
#endif
#ifdef slk_label
static char   * DEE_CURSES_CALL xcurses_slk_label (int labnum) { return slk_label(labnum); }
#else
#define xcurses_slk_label slk_label
#endif
#ifdef slk_clear
static int DEE_CURSES_CALL xcurses_slk_clear (void) { return slk_clear(); }
#else
#define xcurses_slk_clear slk_clear
#endif
#ifdef slk_restore
static int DEE_CURSES_CALL xcurses_slk_restore (void) { return slk_restore(); }
#else
#define xcurses_slk_restore slk_restore
#endif
#ifdef slk_touch
static int DEE_CURSES_CALL xcurses_slk_touch (void) { return slk_touch(); }
#else
#define xcurses_slk_touch slk_touch
#endif
#ifdef slk_attron
static int DEE_CURSES_CALL xcurses_slk_attron (const chtype attrs) { return slk_attron(attrs); }
#else
#define xcurses_slk_attron slk_attron
#endif
#ifdef slk_attroff
static int DEE_CURSES_CALL xcurses_slk_attroff (const chtype attrs) { return slk_attroff(attrs); }
#else
#define xcurses_slk_attroff slk_attroff
#endif
#ifdef slk_attrset
static int DEE_CURSES_CALL xcurses_slk_attrset (const chtype attrs) { return slk_attrset(attrs); }
#else
#define xcurses_slk_attrset slk_attrset
#endif
#ifdef slk_attr_set
static int DEE_CURSES_CALL xcurses_slk_attr_set (const attr_t attrs, short color_pair_number, void* opts) { return slk_attr_set(attrs,color_pair_number,opts); }
#else
#define xcurses_slk_attr_set slk_attr_set
#endif
#ifdef slk_color
static int DEE_CURSES_CALL xcurses_slk_color (short color_pair_number) { return slk_color(color_pair_number); }
#else
#define xcurses_slk_color slk_color
#endif
#ifdef waddchnstr
static int DEE_CURSES_CALL xcurses_waddchnstr (WINDOW *win, const chtype *chstr, int n) { return waddchnstr(win,chstr,n); }
#else
#define xcurses_waddchnstr waddchnstr
#endif
#ifdef waddch
static int DEE_CURSES_CALL xcurses_waddch (WINDOW *win, const chtype ch) { return waddch(win,ch); }
#else
#define xcurses_waddch waddch
#endif
#ifdef wechochar
static int DEE_CURSES_CALL xcurses_wechochar (WINDOW *win, const chtype ch) { return wechochar(win,ch); }
#else
#define xcurses_wechochar wechochar
#endif
#ifdef waddnstr
static int DEE_CURSES_CALL xcurses_waddnstr (WINDOW *win, const char *str, int n) { return waddnstr(win,str,n); }
#else
#define xcurses_waddnstr waddnstr
#endif
#ifdef wattrset
static int DEE_CURSES_CALL xcurses_wattrset (WINDOW *win, int attrs) { return wattrset(win,attrs); }
#else
#define xcurses_wattrset wattrset
#endif
#ifdef wcolor_set
static int DEE_CURSES_CALL xcurses_wcolor_set (WINDOW *win, short color_pair_number, void* opts) { return wcolor_set(win,color_pair_number,opts); }
#else
#define xcurses_wcolor_set wcolor_set
#endif
#ifdef wattr_get
static int DEE_CURSES_CALL xcurses_wattr_get (WINDOW *win, attr_t *attrs, short *pair, void *opts) { return wattr_get(win,attrs,pair,opts); }
#else
#define xcurses_wattr_get wattr_get
#endif
#ifdef wattr_off
static int DEE_CURSES_CALL xcurses_wattr_off (WINDOW *win, attr_t attrs, void *opts) { return wattr_off(win,attrs,opts); }
#else
#define xcurses_wattr_off wattr_off
#endif
#ifdef wattr_on
static int DEE_CURSES_CALL xcurses_wattr_on (WINDOW *win, attr_t attrs, void *opts) { return wattr_on(win,attrs,opts); }
#else
#define xcurses_wattr_on wattr_on
#endif
#ifdef wattr_set
static int DEE_CURSES_CALL xcurses_wattr_set (WINDOW *win, attr_t attrs, short pair, void *opts) { return wattr_set(win,attrs,pair,opts); }
#else
#define xcurses_wattr_set wattr_set
#endif
#ifdef wchgat
static int DEE_CURSES_CALL xcurses_wchgat (WINDOW *win, int n, attr_t attr, short color, const void *opts) { return wchgat(win,n,attr,color,opts); }
#else
#define xcurses_wchgat wchgat
#endif
#ifdef wborder
static int DEE_CURSES_CALL xcurses_wborder (WINDOW *win, chtype ls, chtype rs, chtype ts, chtype bs, chtype tl, chtype tr, chtype bl, chtype br) { return wborder(win,ls,rs,ts,bs,tl,tr,bl,br); }
#else
#define xcurses_wborder wborder
#endif
#ifdef whline
static int DEE_CURSES_CALL xcurses_whline (WINDOW *win, chtype ch, int n) { return whline(win,ch,n); }
#else
#define xcurses_whline whline
#endif
#ifdef wvline
static int DEE_CURSES_CALL xcurses_wvline (WINDOW *win, chtype ch, int n) { return wvline(win,ch,n); }
#else
#define xcurses_wvline wvline
#endif
#ifdef werase
static int DEE_CURSES_CALL xcurses_werase (WINDOW *win) { return werase(win); }
#else
#define xcurses_werase werase
#endif
#ifdef wclear
static int DEE_CURSES_CALL xcurses_wclear (WINDOW *win) { return wclear(win); }
#else
#define xcurses_wclear wclear
#endif
#ifdef wclrtobot
static int DEE_CURSES_CALL xcurses_wclrtobot (WINDOW *win) { return wclrtobot(win); }
#else
#define xcurses_wclrtobot wclrtobot
#endif
#ifdef wclrtoeol
static int DEE_CURSES_CALL xcurses_wclrtoeol (WINDOW *win) { return wclrtoeol(win); }
#else
#define xcurses_wclrtoeol wclrtoeol
#endif
#ifdef wdelch
static int DEE_CURSES_CALL xcurses_wdelch (WINDOW *win) { return wdelch(win); }
#else
#define xcurses_wdelch wdelch
#endif
#ifdef wgetch
static int DEE_CURSES_CALL xcurses_wgetch (WINDOW *win) { return wgetch(win); }
#else
#define xcurses_wgetch wgetch
#endif
#ifdef wgetnstr
static int DEE_CURSES_CALL xcurses_wgetnstr (WINDOW *win, char *str, int n) { return wgetnstr(win,str,n); }
#else
#define xcurses_wgetnstr wgetnstr
#endif
#ifdef winchnstr
static int DEE_CURSES_CALL xcurses_winchnstr (WINDOW *win, chtype *chstr, int n) { return winchnstr(win,chstr,n); }
#else
#define xcurses_winchnstr winchnstr
#endif
#ifdef winch
static chtype DEE_CURSES_CALL xcurses_winch (WINDOW *win) { return winch(win); }
#else
#define xcurses_winch winch
#endif
#ifdef winnstr
static int DEE_CURSES_CALL xcurses_winnstr (WINDOW *win, char *str, int n) { return winnstr(win,str,n); }
#else
#define xcurses_winnstr winnstr
#endif
#ifdef winsch
static int DEE_CURSES_CALL xcurses_winsch (WINDOW *win, chtype ch) { return winsch(win,ch); }
#else
#define xcurses_winsch winsch
#endif
#ifdef winsdelln
static int DEE_CURSES_CALL xcurses_winsdelln (WINDOW *win, int n) { return winsdelln(win,n); }
#else
#define xcurses_winsdelln winsdelln
#endif
#ifdef winsnstr
static int DEE_CURSES_CALL xcurses_winsnstr (WINDOW *win, const char *str, int n) { return winsnstr(win,str,n); }
#else
#define xcurses_winsnstr winsnstr
#endif
#ifdef wmove
static int DEE_CURSES_CALL xcurses_wmove (WINDOW *win, int y, int x) { return wmove(win,y,x); }
#else
#define xcurses_wmove wmove
#endif
#ifdef wscrl
static int DEE_CURSES_CALL xcurses_wscrl (WINDOW *win, int n) { return wscrl(win,n); }
#else
#define xcurses_wscrl wscrl
#endif
void *DeeCurses_API[DEE_CURSES_API_SIZE] = {
/* 0x00 */(void *)&xcurses_curscr,
/* 0x01 */(void *)&xcurses_stdscr,
/* 0x02 */(void *)&xcurses_ttytype,
/* 0x03 */(void *)&xcurses_COLORS,
/* 0x04 */(void *)&xcurses_COLOR_PAIRS,
/* 0x05 */(void *)&xcurses_COLS,
/* 0x06 */(void *)&xcurses_LINES,
/* 0x07 */(void *)&xcurses_TABSIZE,
/* 0x08 */(void *)&xcurses_def_prog_mode,
/* 0x09 */(void *)&xcurses_def_shell_mode,
/* 0x0A */(void *)&xcurses_reset_prog_mode,
/* 0x0B */(void *)&xcurses_reset_shell_mode,
/* 0x0C */(void *)&xcurses_resetty,
/* 0x0D */(void *)&xcurses_savetty,
/* 0x0E */(void *)&xcurses_getsyx_impl_,
/* 0x0F */(void *)&xcurses_ripoffline,
/* 0x10 */(void *)&xcurses_curs_set,
/* 0x11 */(void *)&xcurses_napms,
/* 0x12 */(void *)&xcurses_wrefresh,
/* 0x13 */(void *)&xcurses_wnoutrefresh,
/* 0x14 */(void *)&xcurses_doupdate,
/* 0x15 */(void *)&xcurses_redrawwin,
/* 0x16 */(void *)&xcurses_wredrawln,
/* 0x17 */(void *)&xcurses_getbegx,
/* 0x18 */(void *)&xcurses_getbegy,
/* 0x19 */(void *)&xcurses_getmaxx,
/* 0x1A */(void *)&xcurses_getmaxy,
/* 0x1B */(void *)&xcurses_getparx,
/* 0x1C */(void *)&xcurses_getpary,
/* 0x1D */(void *)&xcurses_getcurx,
/* 0x1E */(void *)&xcurses_getcury,
/* 0x1F */(void *)&xcurses_clearok,
/* 0x20 */(void *)&xcurses_idlok,
/* 0x21 */(void *)&xcurses_idcok,
/* 0x22 */(void *)&xcurses_immedok,
/* 0x23 */(void *)&xcurses_leaveok,
/* 0x24 */(void *)&xcurses_wsetscrreg,
/* 0x25 */(void *)&xcurses_scrollok,
/* 0x26 */(void *)&xcurses_nl,
/* 0x27 */(void *)&xcurses_nonl,
/* 0x28 */(void *)&xcurses_start_color,
/* 0x29 */(void *)&xcurses_init_pair,
/* 0x2A */(void *)&xcurses_init_color,
/* 0x2B */(void *)&xcurses_has_colors,
/* 0x2C */(void *)&xcurses_can_change_color,
/* 0x2D */(void *)&xcurses_color_content,
/* 0x2E */(void *)&xcurses_pair_content,
/* 0x2F */(void *)&xcurses_overlay,
/* 0x30 */(void *)&xcurses_overwrite,
/* 0x31 */(void *)&xcurses_copywin,
/* 0x32 */(void *)&xcurses_keyname,
/* 0x33 */(void *)&xcurses_filter,
/* 0x34 */(void *)&xcurses_use_env,
/* 0x35 */(void *)&xcurses_putwin,
/* 0x36 */(void *)&xcurses_getwin,
/* 0x37 */(void *)&xcurses_delay_output,
/* 0x38 */(void *)&xcurses_flushinp,
/* 0x39 */(void *)&xcurses_initscr,
/* 0x3A */(void *)&xcurses_endwin,
/* 0x3B */(void *)&xcurses_isendwin,
/* 0x3C */(void *)&xcurses_newterm,
/* 0x3D */(void *)&xcurses_set_term,
/* 0x3E */(void *)&xcurses_delscreen,
/* 0x3F */(void *)&xcurses_newwin,
/* 0x40 */(void *)&xcurses_delwin,
/* 0x41 */(void *)&xcurses_mvwin,
/* 0x42 */(void *)&xcurses_subwin,
/* 0x43 */(void *)&xcurses_derwin,
/* 0x44 */(void *)&xcurses_mvderwin,
/* 0x45 */(void *)&xcurses_dupwin,
/* 0x46 */(void *)&xcurses_wsyncup,
/* 0x47 */(void *)&xcurses_syncok,
/* 0x48 */(void *)&xcurses_wcursyncup,
/* 0x49 */(void *)&xcurses_wsyncdown,
/* 0x4A */(void *)&xcurses_wbkgdset,
/* 0x4B */(void *)&xcurses_wbkgd,
/* 0x4C */(void *)&xcurses_getbkgd,
/* 0x4D */(void *)&xcurses_cbreak,
/* 0x4E */(void *)&xcurses_nocbreak,
/* 0x4F */(void *)&xcurses_echo,
/* 0x50 */(void *)&xcurses_noecho,
/* 0x51 */(void *)&xcurses_halfdelay,
/* 0x52 */(void *)&xcurses_intrflush,
/* 0x53 */(void *)&xcurses_keypad,
/* 0x54 */(void *)&xcurses_meta,
/* 0x55 */(void *)&xcurses_nodelay,
/* 0x56 */(void *)&xcurses_raw,
/* 0x57 */(void *)&xcurses_noraw,
/* 0x58 */(void *)&xcurses_noqiflush,
/* 0x59 */(void *)&xcurses_qiflush,
/* 0x5A */(void *)&xcurses_notimeout,
/* 0x5B */(void *)&xcurses_wtimeout,
/* 0x5C */(void *)&xcurses_typeahead,
/* 0x5D */(void *)&xcurses_baudrate,
/* 0x5E */(void *)&xcurses_erasechar,
/* 0x5F */(void *)&xcurses_has_ic,
/* 0x60 */(void *)&xcurses_has_il,
/* 0x61 */(void *)&xcurses_killchar,
/* 0x62 */(void *)&xcurses_longname,
/* 0x63 */(void *)&xcurses_term_attrs,
/* 0x64 */(void *)&xcurses_termattrs,
/* 0x65 */(void *)&xcurses_termname,
/* 0x66 */(void *)&xcurses_beep,
/* 0x67 */(void *)&xcurses_flash,
/* 0x68 */(void *)&xcurses_wtouchln,
/* 0x69 */(void *)&xcurses_is_linetouched,
/* 0x6A */(void *)&xcurses_is_wintouched,
/* 0x6B */(void *)&xcurses_vidputs,
/* 0x6C */(void *)&xcurses_vidattr,
/* 0x6D */NULL /* UNUSED */,
/* 0x6E */(void *)&xcurses_vid_attr,
/* 0x6F */(void *)&xcurses_mvcur,
/* 0x70 */(void *)&xcurses_printw,
/* 0x71 */(void *)&xcurses_wprintw,
/* 0x72 */(void *)&xcurses_mvprintw,
/* 0x73 */(void *)&xcurses_mvwprintw,
/* 0x74 */(void *)&xcurses_vwprintw,
/* 0x75 */(void *)&xcurses_scanw,
/* 0x76 */(void *)&xcurses_wscanw,
/* 0x77 */(void *)&xcurses_mvscanw,
/* 0x78 */(void *)&xcurses_mvwscanw,
/* 0x79 */(void *)&xcurses_vwscanw,
/* 0x7A */(void *)&xcurses_newpad,
/* 0x7B */(void *)&xcurses_subpad,
/* 0x7C */(void *)&xcurses_prefresh,
/* 0x7D */(void *)&xcurses_pnoutrefresh,
/* 0x7E */(void *)&xcurses_pechochar,
/* 0x7F */(void *)&xcurses_scr_dump,
/* 0x80 */(void *)&xcurses_scr_restore,
/* 0x81 */(void *)&xcurses_scr_init,
/* 0x82 */(void *)&xcurses_scr_set,
/* 0x83 */(void *)&xcurses_slk_init,
/* 0x84 */(void *)&xcurses_slk_set,
/* 0x85 */(void *)&xcurses_slk_refresh,
/* 0x86 */(void *)&xcurses_slk_noutrefresh,
/* 0x87 */(void *)&xcurses_slk_label,
/* 0x88 */(void *)&xcurses_slk_clear,
/* 0x89 */(void *)&xcurses_slk_restore,
/* 0x8A */(void *)&xcurses_slk_touch,
/* 0x8B */(void *)&xcurses_slk_attron,
/* 0x8C */(void *)&xcurses_slk_attroff,
/* 0x8D */(void *)&xcurses_slk_attrset,
/* 0x8E */(void *)&xcurses_slk_attr_set,
/* 0x8F */(void *)&xcurses_slk_color,
/* 0x90 */(void *)&xcurses_waddchnstr,
/* 0x91 */(void *)&xcurses_waddch,
/* 0x92 */(void *)&xcurses_wechochar,
/* 0x93 */(void *)&xcurses_waddnstr,
/* 0x94 */(void *)&xcurses_wattrset,
/* 0x95 */(void *)&xcurses_wcolor_set,
/* 0x96 */(void *)&xcurses_wattr_get,
/* 0x97 */(void *)&xcurses_wattr_off,
/* 0x98 */(void *)&xcurses_wattr_on,
/* 0x99 */(void *)&xcurses_wattr_set,
/* 0x9A */(void *)&xcurses_wchgat,
/* 0x9B */(void *)&xcurses_wborder,
/* 0x9C */(void *)&xcurses_whline,
/* 0x9D */(void *)&xcurses_wvline,
/* 0x9E */(void *)&xcurses_werase,
/* 0x9F */(void *)&xcurses_wclear,
/* 0xA0 */(void *)&xcurses_wclrtobot,
/* 0xA1 */(void *)&xcurses_wclrtoeol,
/* 0xA2 */(void *)&xcurses_wdelch,
/* 0xA3 */(void *)&xcurses_wgetch,
/* 0xA4 */(void *)&xcurses_wgetnstr,
/* 0xA5 */(void *)&xcurses_winchnstr,
/* 0xA6 */(void *)&xcurses_winch,
/* 0xA7 */(void *)&xcurses_winnstr,
/* 0xA8 */(void *)&xcurses_winsch,
/* 0xA9 */(void *)&xcurses_winsdelln,
/* 0xAA */(void *)&xcurses_winsnstr,
/* 0xAB */(void *)&xcurses_wmove,
/* 0xAC */(void *)&xcurses_wscrl,
};
//[[[end]]]

DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_CURSES_API_INL */
