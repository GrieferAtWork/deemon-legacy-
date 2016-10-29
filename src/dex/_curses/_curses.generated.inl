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

#ifdef __INTELLISENSE__
#include "_curses.c"
DEE_DECL_BEGIN
#endif


// BEGIN GENERATED CODE
static DeeObject *_curses_addch(DeeObject *args) {
 chtype a;
 if (DeeTuple_Unpack(args,F_chtype ":addch",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_addch(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("addch");
  DeeReturn_None;
 }
}
static DeeObject *_curses_addchnstr(DeeObject *args) {
 chtype * a; int b;
 if (DeeTuple_Unpack(args,F_chtype "*d:addchnstr",&a,&b) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_addchnstr(a,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("addchnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_addchstr(DeeObject *args) {
 chtype * a;
 if (DeeTuple_Unpack(args,F_chtype "*:addchstr",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_addchstr(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("addchstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_addnstr(DeeObject *args) {
 char * a; int b;
 if (DeeTuple_Unpack(args,"sd:addnstr",&a,&b) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_addnstr(a,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("addnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_addstr(DeeObject *args) {
 char * a;
 if (DeeTuple_Unpack(args,"s:addstr",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_addstr(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("addstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_attroff(DeeObject *args) {
 chtype a;
 if (DeeTuple_Unpack(args,F_chtype ":attroff",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_attroff(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("attroff");
  DeeReturn_None;
 }
}
static DeeObject *_curses_attron(DeeObject *args) {
 chtype a;
 if (DeeTuple_Unpack(args,F_chtype ":attron",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_attron(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("attron");
  DeeReturn_None;
 }
}
static DeeObject *_curses_attrset(DeeObject *args) {
 chtype a;
 if (DeeTuple_Unpack(args,F_chtype ":attrset",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_attrset(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("attrset");
  DeeReturn_None;
 }
}
static DeeObject *_curses_attr_get(DeeObject *args) {
 attr_t * a; short * b; void * c;
 if (DeeTuple_Unpack(args,F_attr_t "*hd*p:attr_get",&a,&b,&c) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_attr_get(a,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("attr_get");
  DeeReturn_None;
 }
}
static DeeObject *_curses_attr_off(DeeObject *args) {
 attr_t a; void * b;
 if (DeeTuple_Unpack(args,F_attr_t "p:attr_off",&a,&b) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_attr_off(a,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("attr_off");
  DeeReturn_None;
 }
}
static DeeObject *_curses_attr_on(DeeObject *args) {
 attr_t a; void * b;
 if (DeeTuple_Unpack(args,F_attr_t "p:attr_on",&a,&b) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_attr_on(a,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("attr_on");
  DeeReturn_None;
 }
}
static DeeObject *_curses_attr_set(DeeObject *args) {
 attr_t a; short b; void * c;
 if (DeeTuple_Unpack(args,F_attr_t "hdp:attr_set",&a,&b,&c) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_attr_set(a,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("attr_set");
  DeeReturn_None;
 }
}
static DeeObject *_curses_baudrate(DeeObject *args) {
 if (DeeTuple_Unpack(args,":baudrate") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_baudrate();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("baudrate");
  DeeReturn_None;
 }
}
static DeeObject *_curses_beep(DeeObject *args) {
 if (DeeTuple_Unpack(args,":beep") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_beep();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("beep");
  DeeReturn_None;
 }
}
static DeeObject *_curses_bkgd(DeeObject *args) {
 chtype a;
 if (DeeTuple_Unpack(args,F_chtype ":bkgd",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_bkgd(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("bkgd");
  DeeReturn_None;
 }
}
static DeeObject *_curses_bkgdset(DeeObject *args) {
 chtype a;
 if (DeeTuple_Unpack(args,F_chtype ":bkgdset",&a) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_bkgdset(a);
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_border(DeeObject *args) {
 chtype a; chtype b; chtype c; chtype d; chtype e; chtype f; chtype g; chtype h;
 if (DeeTuple_Unpack(args,F_chtype F_chtype F_chtype F_chtype F_chtype F_chtype F_chtype F_chtype ":border",&a,&b,&c,&d,&e,&f,&g,&h) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_border(a,b,c,d,e,f,g,h);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("border");
  DeeReturn_None;
 }
}
static DeeObject *_curses_can_change_color(DeeObject *args) {
 if (DeeTuple_Unpack(args,":can_change_color") != 0) return NULL;
 {
  int result;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  result = curses_can_change_color();
  DeeCurses_RELEASE();
  DeeReturn_Bool(result);
 }
}
static DeeObject *_curses_cbreak(DeeObject *args) {
 if (DeeTuple_Unpack(args,":cbreak") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_cbreak();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("cbreak");
  DeeReturn_None;
 }
}
static DeeObject *_curses_chgat(DeeObject *args) {
 int a; attr_t b; short c; void * d;
 if (DeeTuple_Unpack(args,"d" F_attr_t "hdp:chgat",&a,&b,&c,&d) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_chgat(a,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("chgat");
  DeeReturn_None;
 }
}
static DeeObject *_curses_clearok(DeeObject *args) {
 DeeCursesWindowObject * a; unsigned char b;
 if (DeeTuple_Unpack(args,"oI8b:clearok",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_clearok(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("clearok");
  DeeReturn_None;
 }
}
static DeeObject *_curses_clear(DeeObject *args) {
 if (DeeTuple_Unpack(args,":clear") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_clear();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("clear");
  DeeReturn_None;
 }
}
static DeeObject *_curses_clrtobot(DeeObject *args) {
 if (DeeTuple_Unpack(args,":clrtobot") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_clrtobot();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("clrtobot");
  DeeReturn_None;
 }
}
static DeeObject *_curses_clrtoeol(DeeObject *args) {
 if (DeeTuple_Unpack(args,":clrtoeol") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_clrtoeol();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("clrtoeol");
  DeeReturn_None;
 }
}
static DeeObject *_curses_color_content(DeeObject *args) {
 short a; short * b; short * c; short * d;
 if (DeeTuple_Unpack(args,"hdhd*hd*hd*:color_content",&a,&b,&c,&d) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_color_content(a,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("color_content");
  DeeReturn_None;
 }
}
static DeeObject *_curses_color_set(DeeObject *args) {
 short a; void * b;
 if (DeeTuple_Unpack(args,"hdp:color_set",&a,&b) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_color_set(a,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("color_set");
  DeeReturn_None;
 }
}
static DeeObject *_curses_copywin(DeeObject *args) {
 DeeCursesWindowObject * a; DeeCursesWindowObject * b; int c; int d; int e; int f; int g; int h; int i;
 if (DeeTuple_Unpack(args,"ooddddddd:copywin",&a,&b,&c,&d,&e,&f,&g,&h,&i) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&b,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_copywin(a->w_wnd,b->w_wnd,c,d,e,f,g,h,i);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("copywin");
  DeeReturn_None;
 }
}
static DeeObject *_curses_curs_set(DeeObject *args) {
 int a;
 if (DeeTuple_Unpack(args,"d:curs_set",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_curs_set(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("curs_set");
  DeeReturn_None;
 }
}
static DeeObject *_curses_def_prog_mode(DeeObject *args) {
 if (DeeTuple_Unpack(args,":def_prog_mode") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_def_prog_mode();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("def_prog_mode");
  DeeReturn_None;
 }
}
static DeeObject *_curses_def_shell_mode(DeeObject *args) {
 if (DeeTuple_Unpack(args,":def_shell_mode") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_def_shell_mode();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("def_shell_mode");
  DeeReturn_None;
 }
}
static DeeObject *_curses_delay_output(DeeObject *args) {
 int a;
 if (DeeTuple_Unpack(args,"d:delay_output",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_delay_output(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("delay_output");
  DeeReturn_None;
 }
}
static DeeObject *_curses_delch(DeeObject *args) {
 if (DeeTuple_Unpack(args,":delch") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_delch();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("delch");
  DeeReturn_None;
 }
}
static DeeObject *_curses_deleteln(DeeObject *args) {
 if (DeeTuple_Unpack(args,":deleteln") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_deleteln();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("deleteln");
  DeeReturn_None;
 }
}
static DeeObject *_curses_delscreen(DeeObject *args) {
 DeeCursesScreenObject * a;
 if (DeeTuple_Unpack(args,"o:delscreen",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesScreen_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_delscreen(a->s_scrn);
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_delwin(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:delwin",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_delwin(a->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("delwin");
  DeeReturn_None;
 }
}
static DeeObject *_curses_derwin(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; int d; int e;
 if (DeeTuple_Unpack(args,"odddd:derwin",&a,&b,&c,&d,&e) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_derwin(a->w_wnd,b,c,d,e);
DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_doupdate(DeeObject *args) {
 if (DeeTuple_Unpack(args,":doupdate") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_doupdate();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("doupdate");
  DeeReturn_None;
 }
}
static DeeObject *_curses_dupwin(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:dupwin",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_dupwin(a->w_wnd);
DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_echochar(DeeObject *args) {
 chtype a;
 if (DeeTuple_Unpack(args,F_chtype ":echochar",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_echochar(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("echochar");
  DeeReturn_None;
 }
}
static DeeObject *_curses_echo(DeeObject *args) {
 if (DeeTuple_Unpack(args,":echo") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_echo();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("echo");
  DeeReturn_None;
 }
}
static DeeObject *_curses_erasechar(DeeObject *args) {
 if (DeeTuple_Unpack(args,":erasechar") != 0) return NULL;
 {
  DeeObject *result;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  result = DeeObject_New(schar,curses_erasechar());
  DeeCurses_RELEASE();
  return result;
 }
}
static DeeObject *_curses_erase(DeeObject *args) {
 if (DeeTuple_Unpack(args,":erase") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_erase();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("erase");
  DeeReturn_None;
 }
}
static DeeObject *_curses_filter(DeeObject *args) {
 if (DeeTuple_Unpack(args,":filter") != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_filter();
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_flash(DeeObject *args) {
 if (DeeTuple_Unpack(args,":flash") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_flash();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("flash");
  DeeReturn_None;
 }
}
static DeeObject *_curses_flushinp(DeeObject *args) {
 if (DeeTuple_Unpack(args,":flushinp") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_flushinp();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("flushinp");
  DeeReturn_None;
 }
}
static DeeObject *_curses_getbkgd(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:getbkgd",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  DeeObject *result;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  result = DeeObject_New(T_chtype,curses_getbkgd(a->w_wnd));
  DeeCurses_RELEASE();
  return result;
 }
}
static DeeObject *_curses_getnstr(DeeObject *args) {
 char * a; int b;
 if (DeeTuple_Unpack(args,"sd:getnstr",&a,&b) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_getnstr(a,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("getnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_getstr(DeeObject *args) {
 char * a;
 if (DeeTuple_Unpack(args,"s:getstr",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_getstr(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("getstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_halfdelay(DeeObject *args) {
 int a;
 if (DeeTuple_Unpack(args,"d:halfdelay",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_halfdelay(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("halfdelay");
  DeeReturn_None;
 }
}
static DeeObject *_curses_has_colors(DeeObject *args) {
 if (DeeTuple_Unpack(args,":has_colors") != 0) return NULL;
 {
  int result;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  result = curses_has_colors();
  DeeCurses_RELEASE();
  DeeReturn_Bool(result);
 }
}
static DeeObject *_curses_has_ic(DeeObject *args) {
 if (DeeTuple_Unpack(args,":has_ic") != 0) return NULL;
 {
  int result;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  result = curses_has_ic();
  DeeCurses_RELEASE();
  DeeReturn_Bool(result);
 }
}
static DeeObject *_curses_has_il(DeeObject *args) {
 if (DeeTuple_Unpack(args,":has_il") != 0) return NULL;
 {
  int result;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  result = curses_has_il();
  DeeCurses_RELEASE();
  DeeReturn_Bool(result);
 }
}
static DeeObject *_curses_hline(DeeObject *args) {
 chtype a; int b;
 if (DeeTuple_Unpack(args,F_chtype "d:hline",&a,&b) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_hline(a,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("hline");
  DeeReturn_None;
 }
}
static DeeObject *_curses_idcok(DeeObject *args) {
 DeeCursesWindowObject * a; unsigned char b;
 if (DeeTuple_Unpack(args,"oI8b:idcok",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_idcok(a->w_wnd,b);
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_idlok(DeeObject *args) {
 DeeCursesWindowObject * a; unsigned char b;
 if (DeeTuple_Unpack(args,"oI8b:idlok",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_idlok(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("idlok");
  DeeReturn_None;
 }
}
static DeeObject *_curses_immedok(DeeObject *args) {
 DeeCursesWindowObject * a; unsigned char b;
 if (DeeTuple_Unpack(args,"oI8b:immedok",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_immedok(a->w_wnd,b);
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_inchnstr(DeeObject *args) {
 chtype * a; int b;
 if (DeeTuple_Unpack(args,F_chtype "*d:inchnstr",&a,&b) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_inchnstr(a,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("inchnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_inchstr(DeeObject *args) {
 chtype * a;
 if (DeeTuple_Unpack(args,F_chtype "*:inchstr",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_inchstr(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("inchstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_inch(DeeObject *args) {
 if (DeeTuple_Unpack(args,":inch") != 0) return NULL;
 {
  DeeObject *result;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  result = DeeObject_New(T_chtype,curses_inch());
  DeeCurses_RELEASE();
  return result;
 }
}
static DeeObject *_curses_init_color(DeeObject *args) {
 short a; short b; short c; short d;
 if (DeeTuple_Unpack(args,"hdhdhdhd:init_color",&a,&b,&c,&d) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_init_color(a,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("init_color");
  DeeReturn_None;
 }
}
static DeeObject *_curses_init_pair(DeeObject *args) {
 short a; short b; short c;
 if (DeeTuple_Unpack(args,"hdhdhd:init_pair",&a,&b,&c) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_init_pair(a,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("init_pair");
  DeeReturn_None;
 }
}
static DeeObject *_curses_innstr(DeeObject *args) {
 char * a; int b;
 if (DeeTuple_Unpack(args,"sd:innstr",&a,&b) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_innstr(a,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("innstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_insch(DeeObject *args) {
 chtype a;
 if (DeeTuple_Unpack(args,F_chtype ":insch",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_insch(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("insch");
  DeeReturn_None;
 }
}
static DeeObject *_curses_insdelln(DeeObject *args) {
 int a;
 if (DeeTuple_Unpack(args,"d:insdelln",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_insdelln(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("insdelln");
  DeeReturn_None;
 }
}
static DeeObject *_curses_insertln(DeeObject *args) {
 if (DeeTuple_Unpack(args,":insertln") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_insertln();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("insertln");
  DeeReturn_None;
 }
}
static DeeObject *_curses_insnstr(DeeObject *args) {
 char * a; int b;
 if (DeeTuple_Unpack(args,"sd:insnstr",&a,&b) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_insnstr(a,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("insnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_insstr(DeeObject *args) {
 char * a;
 if (DeeTuple_Unpack(args,"s:insstr",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_insstr(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("insstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_instr(DeeObject *args) {
 char * a;
 if (DeeTuple_Unpack(args,"s:instr",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_instr(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("instr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_intrflush(DeeObject *args) {
 DeeCursesWindowObject * a; unsigned char b;
 if (DeeTuple_Unpack(args,"oI8b:intrflush",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_intrflush(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("intrflush");
  DeeReturn_None;
 }
}
static DeeObject *_curses_isendwin(DeeObject *args) {
 if (DeeTuple_Unpack(args,":isendwin") != 0) return NULL;
 {
  int result;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  result = curses_isendwin();
  DeeCurses_RELEASE();
  DeeReturn_Bool(result);
 }
}
static DeeObject *_curses_is_linetouched(DeeObject *args) {
 DeeCursesWindowObject * a; int b;
 if (DeeTuple_Unpack(args,"od:is_linetouched",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int result;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  result = curses_is_linetouched(a->w_wnd,b);
  DeeCurses_RELEASE();
  DeeReturn_Bool(result);
 }
}
static DeeObject *_curses_is_wintouched(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:is_wintouched",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int result;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  result = curses_is_wintouched(a->w_wnd);
  DeeCurses_RELEASE();
  DeeReturn_Bool(result);
 }
}
static DeeObject *_curses_keypad(DeeObject *args) {
 DeeCursesWindowObject * a; unsigned char b;
 if (DeeTuple_Unpack(args,"oI8b:keypad",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_keypad(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("keypad");
  DeeReturn_None;
 }
}
static DeeObject *_curses_killchar(DeeObject *args) {
 if (DeeTuple_Unpack(args,":killchar") != 0) return NULL;
 {
  DeeObject *result;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  result = DeeObject_New(schar,curses_killchar());
  DeeCurses_RELEASE();
  return result;
 }
}
static DeeObject *_curses_leaveok(DeeObject *args) {
 DeeCursesWindowObject * a; unsigned char b;
 if (DeeTuple_Unpack(args,"oI8b:leaveok",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_leaveok(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("leaveok");
  DeeReturn_None;
 }
}
static DeeObject *_curses_longname(DeeObject *args) {
 if (DeeTuple_Unpack(args,":longname") != 0) return NULL;
 {
  DeeObject *result; char const *s;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  if DEE_LIKELY((s = curses_longname()) != NULL) result = DeeString_New(s);
  else result = _deecurses_error("longname");
  DeeCurses_RELEASE();
  return result;
 }
}
static DeeObject *_curses_meta(DeeObject *args) {
 DeeCursesWindowObject * a; unsigned char b;
 if (DeeTuple_Unpack(args,"oI8b:meta",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_meta(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("meta");
  DeeReturn_None;
 }
}
static DeeObject *_curses_move(DeeObject *args) {
 int a; int b;
 if (DeeTuple_Unpack(args,"dd:move",&a,&b) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_move(a,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("move");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvaddch(DeeObject *args) {
 int a; int b; chtype c;
 if (DeeTuple_Unpack(args,"dd" F_chtype ":mvaddch",&a,&b,&c) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvaddch(a,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvaddch");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvaddchnstr(DeeObject *args) {
 int a; int b; chtype * c; int d;
 if (DeeTuple_Unpack(args,"dd" F_chtype "*d:mvaddchnstr",&a,&b,&c,&d) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvaddchnstr(a,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvaddchnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvaddchstr(DeeObject *args) {
 int a; int b; chtype * c;
 if (DeeTuple_Unpack(args,"dd" F_chtype "*:mvaddchstr",&a,&b,&c) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvaddchstr(a,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvaddchstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvaddnstr(DeeObject *args) {
 int a; int b; char * c; int d;
 if (DeeTuple_Unpack(args,"ddsd:mvaddnstr",&a,&b,&c,&d) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvaddnstr(a,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvaddnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvaddstr(DeeObject *args) {
 int a; int b; char * c;
 if (DeeTuple_Unpack(args,"dds:mvaddstr",&a,&b,&c) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvaddstr(a,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvaddstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvchgat(DeeObject *args) {
 int a; int b; int c; attr_t d; short e; void * f;
 if (DeeTuple_Unpack(args,"ddd" F_attr_t "hdp:mvchgat",&a,&b,&c,&d,&e,&f) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvchgat(a,b,c,d,e,f);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvchgat");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvcur(DeeObject *args) {
 int a; int b; int c; int d;
 if (DeeTuple_Unpack(args,"dddd:mvcur",&a,&b,&c,&d) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvcur(a,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvcur");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvdelch(DeeObject *args) {
 int a; int b;
 if (DeeTuple_Unpack(args,"dd:mvdelch",&a,&b) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvdelch(a,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvdelch");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvderwin(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c;
 if (DeeTuple_Unpack(args,"odd:mvderwin",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvderwin(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvderwin");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvgetnstr(DeeObject *args) {
 int a; int b; char * c; int d;
 if (DeeTuple_Unpack(args,"ddsd:mvgetnstr",&a,&b,&c,&d) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvgetnstr(a,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvgetnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvgetstr(DeeObject *args) {
 int a; int b; char * c;
 if (DeeTuple_Unpack(args,"dds:mvgetstr",&a,&b,&c) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvgetstr(a,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvgetstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvhline(DeeObject *args) {
 int a; int b; chtype c; int d;
 if (DeeTuple_Unpack(args,"dd" F_chtype "d:mvhline",&a,&b,&c,&d) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvhline(a,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvhline");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvinch(DeeObject *args) {
 int a; int b;
 if (DeeTuple_Unpack(args,"dd:mvinch",&a,&b) != 0) return NULL;
 {
  DeeObject *result;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  result = DeeObject_New(T_chtype,curses_mvinch(a,b));
  DeeCurses_RELEASE();
  return result;
 }
}
static DeeObject *_curses_mvinchnstr(DeeObject *args) {
 int a; int b; chtype * c; int d;
 if (DeeTuple_Unpack(args,"dd" F_chtype "*d:mvinchnstr",&a,&b,&c,&d) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvinchnstr(a,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvinchnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvinchstr(DeeObject *args) {
 int a; int b; chtype * c;
 if (DeeTuple_Unpack(args,"dd" F_chtype "*:mvinchstr",&a,&b,&c) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvinchstr(a,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvinchstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvinnstr(DeeObject *args) {
 int a; int b; char * c; int d;
 if (DeeTuple_Unpack(args,"ddsd:mvinnstr",&a,&b,&c,&d) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvinnstr(a,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvinnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvinsch(DeeObject *args) {
 int a; int b; chtype c;
 if (DeeTuple_Unpack(args,"dd" F_chtype ":mvinsch",&a,&b,&c) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvinsch(a,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvinsch");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvinsnstr(DeeObject *args) {
 int a; int b; char * c; int d;
 if (DeeTuple_Unpack(args,"ddsd:mvinsnstr",&a,&b,&c,&d) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvinsnstr(a,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvinsnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvinsstr(DeeObject *args) {
 int a; int b; char * c;
 if (DeeTuple_Unpack(args,"dds:mvinsstr",&a,&b,&c) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvinsstr(a,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvinsstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvinstr(DeeObject *args) {
 int a; int b; char * c;
 if (DeeTuple_Unpack(args,"dds:mvinstr",&a,&b,&c) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvinstr(a,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvinstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvvline(DeeObject *args) {
 int a; int b; chtype c; int d;
 if (DeeTuple_Unpack(args,"dd" F_chtype "d:mvvline",&a,&b,&c,&d) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvvline(a,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvvline");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwaddchnstr(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; chtype * d; int e;
 if (DeeTuple_Unpack(args,"odd" F_chtype "*d:mvwaddchnstr",&a,&b,&c,&d,&e) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwaddchnstr(a->w_wnd,b,c,d,e);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwaddchnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwaddchstr(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; chtype * d;
 if (DeeTuple_Unpack(args,"odd" F_chtype "*:mvwaddchstr",&a,&b,&c,&d) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwaddchstr(a->w_wnd,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwaddchstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwaddch(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; chtype d;
 if (DeeTuple_Unpack(args,"odd" F_chtype ":mvwaddch",&a,&b,&c,&d) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwaddch(a->w_wnd,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwaddch");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwaddnstr(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; char * d; int e;
 if (DeeTuple_Unpack(args,"oddsd:mvwaddnstr",&a,&b,&c,&d,&e) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwaddnstr(a->w_wnd,b,c,d,e);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwaddnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwaddstr(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; char * d;
 if (DeeTuple_Unpack(args,"odds:mvwaddstr",&a,&b,&c,&d) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwaddstr(a->w_wnd,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwaddstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwchgat(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; int d; attr_t e; short f; void * g;
 if (DeeTuple_Unpack(args,"oddd" F_attr_t "hdp:mvwchgat",&a,&b,&c,&d,&e,&f,&g) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwchgat(a->w_wnd,b,c,d,e,f,g);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwchgat");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwdelch(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c;
 if (DeeTuple_Unpack(args,"odd:mvwdelch",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwdelch(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwdelch");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwgetnstr(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; char * d; int e;
 if (DeeTuple_Unpack(args,"oddsd:mvwgetnstr",&a,&b,&c,&d,&e) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwgetnstr(a->w_wnd,b,c,d,e);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwgetnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwgetstr(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; char * d;
 if (DeeTuple_Unpack(args,"odds:mvwgetstr",&a,&b,&c,&d) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwgetstr(a->w_wnd,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwgetstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwhline(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; chtype d; int e;
 if (DeeTuple_Unpack(args,"odd" F_chtype "d:mvwhline",&a,&b,&c,&d,&e) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwhline(a->w_wnd,b,c,d,e);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwhline");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwinchnstr(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; chtype * d; int e;
 if (DeeTuple_Unpack(args,"odd" F_chtype "*d:mvwinchnstr",&a,&b,&c,&d,&e) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwinchnstr(a->w_wnd,b,c,d,e);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwinchnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwinchstr(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; chtype * d;
 if (DeeTuple_Unpack(args,"odd" F_chtype "*:mvwinchstr",&a,&b,&c,&d) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwinchstr(a->w_wnd,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwinchstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwinch(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c;
 if (DeeTuple_Unpack(args,"odd:mvwinch",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  DeeObject *result;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  result = DeeObject_New(T_chtype,curses_mvwinch(a->w_wnd,b,c));
  DeeCurses_RELEASE();
  return result;
 }
}
static DeeObject *_curses_mvwinnstr(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; char * d; int e;
 if (DeeTuple_Unpack(args,"oddsd:mvwinnstr",&a,&b,&c,&d,&e) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwinnstr(a->w_wnd,b,c,d,e);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwinnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwinsch(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; chtype d;
 if (DeeTuple_Unpack(args,"odd" F_chtype ":mvwinsch",&a,&b,&c,&d) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwinsch(a->w_wnd,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwinsch");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwinsnstr(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; char * d; int e;
 if (DeeTuple_Unpack(args,"oddsd:mvwinsnstr",&a,&b,&c,&d,&e) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwinsnstr(a->w_wnd,b,c,d,e);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwinsnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwinsstr(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; char * d;
 if (DeeTuple_Unpack(args,"odds:mvwinsstr",&a,&b,&c,&d) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwinsstr(a->w_wnd,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwinsstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwinstr(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; char * d;
 if (DeeTuple_Unpack(args,"odds:mvwinstr",&a,&b,&c,&d) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwinstr(a->w_wnd,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwinstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwin(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c;
 if (DeeTuple_Unpack(args,"odd:mvwin",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwin(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwin");
  DeeReturn_None;
 }
}
static DeeObject *_curses_mvwvline(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; chtype d; int e;
 if (DeeTuple_Unpack(args,"odd" F_chtype "d:mvwvline",&a,&b,&c,&d,&e) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_mvwvline(a->w_wnd,b,c,d,e);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("mvwvline");
  DeeReturn_None;
 }
}
static DeeObject *_curses_napms(DeeObject *args) {
 int a;
 if (DeeTuple_Unpack(args,"d:napms",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_napms(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("napms");
  DeeReturn_None;
 }
}
static DeeObject *_curses_nl(DeeObject *args) {
 if (DeeTuple_Unpack(args,":nl") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_nl();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("nl");
  DeeReturn_None;
 }
}
static DeeObject *_curses_nocbreak(DeeObject *args) {
 if (DeeTuple_Unpack(args,":nocbreak") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_nocbreak();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("nocbreak");
  DeeReturn_None;
 }
}
static DeeObject *_curses_nodelay(DeeObject *args) {
 DeeCursesWindowObject * a; unsigned char b;
 if (DeeTuple_Unpack(args,"oI8b:nodelay",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_nodelay(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("nodelay");
  DeeReturn_None;
 }
}
static DeeObject *_curses_noecho(DeeObject *args) {
 if (DeeTuple_Unpack(args,":noecho") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_noecho();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("noecho");
  DeeReturn_None;
 }
}
static DeeObject *_curses_nonl(DeeObject *args) {
 if (DeeTuple_Unpack(args,":nonl") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_nonl();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("nonl");
  DeeReturn_None;
 }
}
static DeeObject *_curses_noqiflush(DeeObject *args) {
 if (DeeTuple_Unpack(args,":noqiflush") != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_noqiflush();
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_noraw(DeeObject *args) {
 if (DeeTuple_Unpack(args,":noraw") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_noraw();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("noraw");
  DeeReturn_None;
 }
}
static DeeObject *_curses_notimeout(DeeObject *args) {
 DeeCursesWindowObject * a; unsigned char b;
 if (DeeTuple_Unpack(args,"oI8b:notimeout",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_notimeout(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("notimeout");
  DeeReturn_None;
 }
}
static DeeObject *_curses_overlay(DeeObject *args) {
 DeeCursesWindowObject * a; DeeCursesWindowObject * b;
 if (DeeTuple_Unpack(args,"oo:overlay",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&b,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_overlay(a->w_wnd,b->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("overlay");
  DeeReturn_None;
 }
}
static DeeObject *_curses_overwrite(DeeObject *args) {
 DeeCursesWindowObject * a; DeeCursesWindowObject * b;
 if (DeeTuple_Unpack(args,"oo:overwrite",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&b,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_overwrite(a->w_wnd,b->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("overwrite");
  DeeReturn_None;
 }
}
static DeeObject *_curses_pair_content(DeeObject *args) {
 short a; short * b; short * c;
 if (DeeTuple_Unpack(args,"hdhd*hd*:pair_content",&a,&b,&c) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_pair_content(a,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("pair_content");
  DeeReturn_None;
 }
}
static DeeObject *_curses_pechochar(DeeObject *args) {
 DeeCursesWindowObject * a; chtype b;
 if (DeeTuple_Unpack(args,"o" F_chtype ":pechochar",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_pechochar(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("pechochar");
  DeeReturn_None;
 }
}
static DeeObject *_curses_pnoutrefresh(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; int d; int e; int f; int g;
 if (DeeTuple_Unpack(args,"odddddd:pnoutrefresh",&a,&b,&c,&d,&e,&f,&g) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_pnoutrefresh(a->w_wnd,b,c,d,e,f,g);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("pnoutrefresh");
  DeeReturn_None;
 }
}
static DeeObject *_curses_prefresh(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; int d; int e; int f; int g;
 if (DeeTuple_Unpack(args,"odddddd:prefresh",&a,&b,&c,&d,&e,&f,&g) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_prefresh(a->w_wnd,b,c,d,e,f,g);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("prefresh");
  DeeReturn_None;
 }
}
static DeeObject *_curses_qiflush(DeeObject *args) {
 if (DeeTuple_Unpack(args,":qiflush") != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_qiflush();
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_raw(DeeObject *args) {
 if (DeeTuple_Unpack(args,":raw") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_raw();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("raw");
  DeeReturn_None;
 }
}
static DeeObject *_curses_redrawwin(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:redrawwin",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_redrawwin(a->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("redrawwin");
  DeeReturn_None;
 }
}
static DeeObject *_curses_refresh(DeeObject *args) {
 if (DeeTuple_Unpack(args,":refresh") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_refresh();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("refresh");
  DeeReturn_None;
 }
}
static DeeObject *_curses_reset_prog_mode(DeeObject *args) {
 if (DeeTuple_Unpack(args,":reset_prog_mode") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_reset_prog_mode();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("reset_prog_mode");
  DeeReturn_None;
 }
}
static DeeObject *_curses_reset_shell_mode(DeeObject *args) {
 if (DeeTuple_Unpack(args,":reset_shell_mode") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_reset_shell_mode();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("reset_shell_mode");
  DeeReturn_None;
 }
}
static DeeObject *_curses_resetty(DeeObject *args) {
 if (DeeTuple_Unpack(args,":resetty") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_resetty();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("resetty");
  DeeReturn_None;
 }
}
static DeeObject *_curses_savetty(DeeObject *args) {
 if (DeeTuple_Unpack(args,":savetty") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_savetty();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("savetty");
  DeeReturn_None;
 }
}
static DeeObject *_curses_scr_dump(DeeObject *args) {
 char * a;
 if (DeeTuple_Unpack(args,"s:scr_dump",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_scr_dump(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("scr_dump");
  DeeReturn_None;
 }
}
static DeeObject *_curses_scr_init(DeeObject *args) {
 char * a;
 if (DeeTuple_Unpack(args,"s:scr_init",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_scr_init(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("scr_init");
  DeeReturn_None;
 }
}
static DeeObject *_curses_scr_restore(DeeObject *args) {
 char * a;
 if (DeeTuple_Unpack(args,"s:scr_restore",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_scr_restore(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("scr_restore");
  DeeReturn_None;
 }
}
static DeeObject *_curses_scr_set(DeeObject *args) {
 char * a;
 if (DeeTuple_Unpack(args,"s:scr_set",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_scr_set(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("scr_set");
  DeeReturn_None;
 }
}
static DeeObject *_curses_scrl(DeeObject *args) {
 int a;
 if (DeeTuple_Unpack(args,"d:scrl",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_scrl(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("scrl");
  DeeReturn_None;
 }
}
static DeeObject *_curses_scroll(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:scroll",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_scroll(a->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("scroll");
  DeeReturn_None;
 }
}
static DeeObject *_curses_scrollok(DeeObject *args) {
 DeeCursesWindowObject * a; unsigned char b;
 if (DeeTuple_Unpack(args,"oI8b:scrollok",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_scrollok(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("scrollok");
  DeeReturn_None;
 }
}
static DeeObject *_curses_setscrreg(DeeObject *args) {
 int a; int b;
 if (DeeTuple_Unpack(args,"dd:setscrreg",&a,&b) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_setscrreg(a,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("setscrreg");
  DeeReturn_None;
 }
}
static DeeObject *_curses_slk_attroff(DeeObject *args) {
 chtype a;
 if (DeeTuple_Unpack(args,F_chtype ":slk_attroff",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_slk_attroff(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("slk_attroff");
  DeeReturn_None;
 }
}
static DeeObject *_curses_slk_attr_off(DeeObject *args) {
 attr_t a; void * b;
 if (DeeTuple_Unpack(args,F_attr_t "p:slk_attr_off",&a,&b) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_slk_attr_off(a,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("slk_attr_off");
  DeeReturn_None;
 }
}
static DeeObject *_curses_slk_attron(DeeObject *args) {
 chtype a;
 if (DeeTuple_Unpack(args,F_chtype ":slk_attron",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_slk_attron(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("slk_attron");
  DeeReturn_None;
 }
}
static DeeObject *_curses_slk_attr_on(DeeObject *args) {
 attr_t a; void * b;
 if (DeeTuple_Unpack(args,F_attr_t "p:slk_attr_on",&a,&b) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_slk_attr_on(a,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("slk_attr_on");
  DeeReturn_None;
 }
}
static DeeObject *_curses_slk_attrset(DeeObject *args) {
 chtype a;
 if (DeeTuple_Unpack(args,F_chtype ":slk_attrset",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_slk_attrset(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("slk_attrset");
  DeeReturn_None;
 }
}
static DeeObject *_curses_slk_attr_set(DeeObject *args) {
 attr_t a; short b; void * c;
 if (DeeTuple_Unpack(args,F_attr_t "hdp:slk_attr_set",&a,&b,&c) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_slk_attr_set(a,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("slk_attr_set");
  DeeReturn_None;
 }
}
static DeeObject *_curses_slk_clear(DeeObject *args) {
 if (DeeTuple_Unpack(args,":slk_clear") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_slk_clear();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("slk_clear");
  DeeReturn_None;
 }
}
static DeeObject *_curses_slk_color(DeeObject *args) {
 short a;
 if (DeeTuple_Unpack(args,"hd:slk_color",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_slk_color(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("slk_color");
  DeeReturn_None;
 }
}
static DeeObject *_curses_slk_init(DeeObject *args) {
 int a;
 if (DeeTuple_Unpack(args,"d:slk_init",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_slk_init(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("slk_init");
  DeeReturn_None;
 }
}
static DeeObject *_curses_slk_label(DeeObject *args) {
 int a;
 if (DeeTuple_Unpack(args,"d:slk_label",&a) != 0) return NULL;
 {
  DeeObject *result; char const *s;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  if DEE_LIKELY((s = curses_slk_label(a)) != NULL) result = DeeString_New(s);
  else result = _deecurses_error("slk_label");
  DeeCurses_RELEASE();
  return result;
 }
}
static DeeObject *_curses_slk_noutrefresh(DeeObject *args) {
 if (DeeTuple_Unpack(args,":slk_noutrefresh") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_slk_noutrefresh();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("slk_noutrefresh");
  DeeReturn_None;
 }
}
static DeeObject *_curses_slk_refresh(DeeObject *args) {
 if (DeeTuple_Unpack(args,":slk_refresh") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_slk_refresh();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("slk_refresh");
  DeeReturn_None;
 }
}
static DeeObject *_curses_slk_restore(DeeObject *args) {
 if (DeeTuple_Unpack(args,":slk_restore") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_slk_restore();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("slk_restore");
  DeeReturn_None;
 }
}
static DeeObject *_curses_slk_set(DeeObject *args) {
 int a; char * b; int c;
 if (DeeTuple_Unpack(args,"dsd:slk_set",&a,&b,&c) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_slk_set(a,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("slk_set");
  DeeReturn_None;
 }
}
static DeeObject *_curses_slk_touch(DeeObject *args) {
 if (DeeTuple_Unpack(args,":slk_touch") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_slk_touch();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("slk_touch");
  DeeReturn_None;
 }
}
static DeeObject *_curses_standend(DeeObject *args) {
 if (DeeTuple_Unpack(args,":standend") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_standend();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("standend");
  DeeReturn_None;
 }
}
static DeeObject *_curses_standout(DeeObject *args) {
 if (DeeTuple_Unpack(args,":standout") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_standout();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("standout");
  DeeReturn_None;
 }
}
static DeeObject *_curses_start_color(DeeObject *args) {
 if (DeeTuple_Unpack(args,":start_color") != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_start_color();
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("start_color");
  DeeReturn_None;
 }
}
static DeeObject *_curses_subpad(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; int d; int e;
 if (DeeTuple_Unpack(args,"odddd:subpad",&a,&b,&c,&d,&e) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_subpad(a->w_wnd,b,c,d,e);
DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_subwin(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; int d; int e;
 if (DeeTuple_Unpack(args,"odddd:subwin",&a,&b,&c,&d,&e) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_subwin(a->w_wnd,b,c,d,e);
DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_syncok(DeeObject *args) {
 DeeCursesWindowObject * a; unsigned char b;
 if (DeeTuple_Unpack(args,"oI8b:syncok",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_syncok(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("syncok");
  DeeReturn_None;
 }
}
static DeeObject *_curses_termattrs(DeeObject *args) {
 if (DeeTuple_Unpack(args,":termattrs") != 0) return NULL;
 {
  DeeObject *result;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  result = DeeObject_New(T_chtype,curses_termattrs());
  DeeCurses_RELEASE();
  return result;
 }
}
static DeeObject *_curses_termname(DeeObject *args) {
 if (DeeTuple_Unpack(args,":termname") != 0) return NULL;
 {
  DeeObject *result; char const *s;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  if DEE_LIKELY((s = curses_termname()) != NULL) result = DeeString_New(s);
  else result = _deecurses_error("termname");
  DeeCurses_RELEASE();
  return result;
 }
}
static DeeObject *_curses_timeout(DeeObject *args) {
 int a;
 if (DeeTuple_Unpack(args,"d:timeout",&a) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_timeout(a);
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_touchline(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c;
 if (DeeTuple_Unpack(args,"odd:touchline",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_touchline(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("touchline");
  DeeReturn_None;
 }
}
static DeeObject *_curses_touchwin(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:touchwin",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_touchwin(a->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("touchwin");
  DeeReturn_None;
 }
}
static DeeObject *_curses_typeahead(DeeObject *args) {
 int a;
 if (DeeTuple_Unpack(args,"d:typeahead",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_typeahead(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("typeahead");
  DeeReturn_None;
 }
}
static DeeObject *_curses_untouchwin(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:untouchwin",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_untouchwin(a->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("untouchwin");
  DeeReturn_None;
 }
}
static DeeObject *_curses_use_env(DeeObject *args) {
 unsigned char a;
 if (DeeTuple_Unpack(args,"I8b:use_env",&a) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_use_env(a);
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_vidattr(DeeObject *args) {
 chtype a;
 if (DeeTuple_Unpack(args,F_chtype ":vidattr",&a) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_vidattr(a);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("vidattr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_vid_attr(DeeObject *args) {
 attr_t a; short b; void * c;
 if (DeeTuple_Unpack(args,F_attr_t "hdp:vid_attr",&a,&b,&c) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_vid_attr(a,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("vid_attr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_vline(DeeObject *args) {
 chtype a; int b;
 if (DeeTuple_Unpack(args,F_chtype "d:vline",&a,&b) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_vline(a,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("vline");
  DeeReturn_None;
 }
}
static DeeObject *_curses_waddchnstr(DeeObject *args) {
 DeeCursesWindowObject * a; chtype * b; int c;
 if (DeeTuple_Unpack(args,"o" F_chtype "*d:waddchnstr",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_waddchnstr(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("waddchnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_waddchstr(DeeObject *args) {
 DeeCursesWindowObject * a; chtype * b;
 if (DeeTuple_Unpack(args,"o" F_chtype "*:waddchstr",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_waddchstr(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("waddchstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_waddch(DeeObject *args) {
 DeeCursesWindowObject * a; chtype b;
 if (DeeTuple_Unpack(args,"o" F_chtype ":waddch",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_waddch(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("waddch");
  DeeReturn_None;
 }
}
static DeeObject *_curses_waddnstr(DeeObject *args) {
 DeeCursesWindowObject * a; char * b; int c;
 if (DeeTuple_Unpack(args,"osd:waddnstr",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_waddnstr(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("waddnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_waddstr(DeeObject *args) {
 DeeCursesWindowObject * a; char * b;
 if (DeeTuple_Unpack(args,"os:waddstr",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_waddstr(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("waddstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wattroff(DeeObject *args) {
 DeeCursesWindowObject * a; chtype b;
 if (DeeTuple_Unpack(args,"o" F_chtype ":wattroff",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wattroff(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wattroff");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wattron(DeeObject *args) {
 DeeCursesWindowObject * a; chtype b;
 if (DeeTuple_Unpack(args,"o" F_chtype ":wattron",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wattron(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wattron");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wattrset(DeeObject *args) {
 DeeCursesWindowObject * a; chtype b;
 if (DeeTuple_Unpack(args,"o" F_chtype ":wattrset",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wattrset(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wattrset");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wattr_get(DeeObject *args) {
 DeeCursesWindowObject * a; attr_t * b; short * c; void * d;
 if (DeeTuple_Unpack(args,"o" F_attr_t "*hd*p:wattr_get",&a,&b,&c,&d) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wattr_get(a->w_wnd,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wattr_get");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wattr_off(DeeObject *args) {
 DeeCursesWindowObject * a; attr_t b; void * c;
 if (DeeTuple_Unpack(args,"o" F_attr_t "p:wattr_off",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wattr_off(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wattr_off");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wattr_on(DeeObject *args) {
 DeeCursesWindowObject * a; attr_t b; void * c;
 if (DeeTuple_Unpack(args,"o" F_attr_t "p:wattr_on",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wattr_on(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wattr_on");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wattr_set(DeeObject *args) {
 DeeCursesWindowObject * a; attr_t b; short c; void * d;
 if (DeeTuple_Unpack(args,"o" F_attr_t "hdp:wattr_set",&a,&b,&c,&d) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wattr_set(a->w_wnd,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wattr_set");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wbkgdset(DeeObject *args) {
 DeeCursesWindowObject * a; chtype b;
 if (DeeTuple_Unpack(args,"o" F_chtype ":wbkgdset",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_wbkgdset(a->w_wnd,b);
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_wbkgd(DeeObject *args) {
 DeeCursesWindowObject * a; chtype b;
 if (DeeTuple_Unpack(args,"o" F_chtype ":wbkgd",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wbkgd(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wbkgd");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wborder(DeeObject *args) {
 DeeCursesWindowObject * a; chtype b; chtype c; chtype d; chtype e; chtype f; chtype g; chtype h; chtype i;
 if (DeeTuple_Unpack(args,"o" F_chtype F_chtype F_chtype F_chtype F_chtype F_chtype F_chtype F_chtype ":wborder",&a,&b,&c,&d,&e,&f,&g,&h,&i) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wborder(a->w_wnd,b,c,d,e,f,g,h,i);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wborder");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wchgat(DeeObject *args) {
 DeeCursesWindowObject * a; int b; attr_t c; short d; void * e;
 if (DeeTuple_Unpack(args,"od" F_attr_t "hdp:wchgat",&a,&b,&c,&d,&e) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wchgat(a->w_wnd,b,c,d,e);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wchgat");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wclear(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:wclear",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wclear(a->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wclear");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wclrtobot(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:wclrtobot",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wclrtobot(a->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wclrtobot");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wclrtoeol(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:wclrtoeol",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wclrtoeol(a->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wclrtoeol");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wcolor_set(DeeObject *args) {
 DeeCursesWindowObject * a; short b; void * c;
 if (DeeTuple_Unpack(args,"ohdp:wcolor_set",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wcolor_set(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wcolor_set");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wcursyncup(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:wcursyncup",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_wcursyncup(a->w_wnd);
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_wdelch(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:wdelch",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wdelch(a->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wdelch");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wdeleteln(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:wdeleteln",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wdeleteln(a->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wdeleteln");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wechochar(DeeObject *args) {
 DeeCursesWindowObject * a; chtype b;
 if (DeeTuple_Unpack(args,"o" F_chtype ":wechochar",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wechochar(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wechochar");
  DeeReturn_None;
 }
}
static DeeObject *_curses_werase(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:werase",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_werase(a->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("werase");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wgetnstr(DeeObject *args) {
 DeeCursesWindowObject * a; char * b; int c;
 if (DeeTuple_Unpack(args,"osd:wgetnstr",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wgetnstr(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wgetnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wgetstr(DeeObject *args) {
 DeeCursesWindowObject * a; char * b;
 if (DeeTuple_Unpack(args,"os:wgetstr",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wgetstr(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wgetstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_whline(DeeObject *args) {
 DeeCursesWindowObject * a; chtype b; int c;
 if (DeeTuple_Unpack(args,"o" F_chtype "d:whline",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_whline(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("whline");
  DeeReturn_None;
 }
}
static DeeObject *_curses_winchnstr(DeeObject *args) {
 DeeCursesWindowObject * a; chtype * b; int c;
 if (DeeTuple_Unpack(args,"o" F_chtype "*d:winchnstr",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_winchnstr(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("winchnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_winchstr(DeeObject *args) {
 DeeCursesWindowObject * a; chtype * b;
 if (DeeTuple_Unpack(args,"o" F_chtype "*:winchstr",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_winchstr(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("winchstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_winch(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:winch",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  DeeObject *result;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  result = DeeObject_New(T_chtype,curses_winch(a->w_wnd));
  DeeCurses_RELEASE();
  return result;
 }
}
static DeeObject *_curses_winnstr(DeeObject *args) {
 DeeCursesWindowObject * a; char * b; int c;
 if (DeeTuple_Unpack(args,"osd:winnstr",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_winnstr(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("winnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_winsch(DeeObject *args) {
 DeeCursesWindowObject * a; chtype b;
 if (DeeTuple_Unpack(args,"o" F_chtype ":winsch",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_winsch(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("winsch");
  DeeReturn_None;
 }
}
static DeeObject *_curses_winsdelln(DeeObject *args) {
 DeeCursesWindowObject * a; int b;
 if (DeeTuple_Unpack(args,"od:winsdelln",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_winsdelln(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("winsdelln");
  DeeReturn_None;
 }
}
static DeeObject *_curses_winsertln(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:winsertln",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_winsertln(a->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("winsertln");
  DeeReturn_None;
 }
}
static DeeObject *_curses_winsnstr(DeeObject *args) {
 DeeCursesWindowObject * a; char * b; int c;
 if (DeeTuple_Unpack(args,"osd:winsnstr",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_winsnstr(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("winsnstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_winsstr(DeeObject *args) {
 DeeCursesWindowObject * a; char * b;
 if (DeeTuple_Unpack(args,"os:winsstr",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_winsstr(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("winsstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_winstr(DeeObject *args) {
 DeeCursesWindowObject * a; char * b;
 if (DeeTuple_Unpack(args,"os:winstr",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_winstr(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("winstr");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wmove(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c;
 if (DeeTuple_Unpack(args,"odd:wmove",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wmove(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wmove");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wnoutrefresh(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:wnoutrefresh",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wnoutrefresh(a->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wnoutrefresh");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wprintw(DeeObject *args) {
 DeeCursesWindowObject * a; char * b;
 if (DeeTuple_Unpack(args,"os:wprintw",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wprintw(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wprintw");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wredrawln(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c;
 if (DeeTuple_Unpack(args,"odd:wredrawln",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wredrawln(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wredrawln");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wrefresh(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:wrefresh",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wrefresh(a->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wrefresh");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wscanw(DeeObject *args) {
 DeeCursesWindowObject * a; char * b;
 if (DeeTuple_Unpack(args,"os:wscanw",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wscanw(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wscanw");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wscrl(DeeObject *args) {
 DeeCursesWindowObject * a; int b;
 if (DeeTuple_Unpack(args,"od:wscrl",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wscrl(a->w_wnd,b);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wscrl");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wsetscrreg(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c;
 if (DeeTuple_Unpack(args,"odd:wsetscrreg",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wsetscrreg(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wsetscrreg");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wstandend(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:wstandend",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wstandend(a->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wstandend");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wstandout(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:wstandout",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wstandout(a->w_wnd);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wstandout");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wsyncdown(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:wsyncdown",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_wsyncdown(a->w_wnd);
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_wsyncup(DeeObject *args) {
 DeeCursesWindowObject * a;
 if (DeeTuple_Unpack(args,"o:wsyncup",&a) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_wsyncup(a->w_wnd);
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_wtimeout(DeeObject *args) {
 DeeCursesWindowObject * a; int b;
 if (DeeTuple_Unpack(args,"od:wtimeout",&a,&b) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 curses_wtimeout(a->w_wnd,b);
 DeeCurses_RELEASE();
 DeeReturn_None;
}
static DeeObject *_curses_wtouchln(DeeObject *args) {
 DeeCursesWindowObject * a; int b; int c; int d;
 if (DeeTuple_Unpack(args,"oddd:wtouchln",&a,&b,&c,&d) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wtouchln(a->w_wnd,b,c,d);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wtouchln");
  DeeReturn_None;
 }
}
static DeeObject *_curses_wvline(DeeObject *args) {
 DeeCursesWindowObject * a; chtype b; int c;
 if (DeeTuple_Unpack(args,"o" F_chtype "d:wvline",&a,&b,&c) != 0) return NULL;
 if (DeeObject_InplaceGetInstance(&a,&DeeCursesWindow_Type) != 0) return NULL;
 {
  int err;
  if (DeeCurses_ACQUIRE() != 0) return NULL;
  err = curses_wvline(a->w_wnd,b,c);
  DeeCurses_RELEASE();
  if (err == ERR) return _deecurses_error("wvline");
  DeeReturn_None;
 }
}
// END GENERATED CODE

static DeeObject *_curses_box(DeeObject *args) {
 DeeCursesWindowObject *a; chtype b = '|',c = '-';
 if (DeeTuple_Unpack(args,"o|" F_chtype F_chtype ":box",&a,&b,&c) != 0) return NULL;
 if (DeeError_TypeError_CheckType((DeeObject *)a,&DeeCursesWindow_Type) != 0) return NULL;
 if (curses_box(a->w_wnd,b,c) == ERR) return _deecurses_error("box");
 DeeReturn_None;
}

static DeeObject *_curses_mvgetch(DeeObject *args) {
 int y,x,result;
 if (DeeTuple_Unpack(args,"dd:mvgetch",&y,&x) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 result = mvgetch(y,x);
 DeeCurses_RELEASE();
 return DeeObject_New(int,DeeCurses_PrivateKeycodeToPublic(result));
}
static DeeObject *_curses_mvwgetch(DeeObject *args) {
 DeeCursesWindowObject *wnd; int y,x,result;
 if (DeeTuple_Unpack(args,"odd:mvwgetch",&wnd,&y,&x) != 0) return NULL;
 if (DeeError_TypeError_CheckType((DeeObject *)wnd,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 result = mvwgetch(wnd->w_wnd,y,x);
 DeeCurses_RELEASE();
 return DeeObject_New(int,DeeCurses_PrivateKeycodeToPublic(result));
}
static DeeObject *_curses_wgetch(DeeObject *args) {
 DeeCursesWindowObject *wnd; int result;
 if (DeeTuple_Unpack(args,"o:wgetch",&wnd) != 0) return NULL;
 if (DeeError_TypeError_CheckType((DeeObject *)wnd,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 result = wgetch(wnd->w_wnd);
 DeeCurses_RELEASE();
 return DeeObject_New(int,DeeCurses_PrivateKeycodeToPublic(result));
}
static DeeObject *_curses_getch(DeeObject *args) {
 int result;
 if (DeeTuple_Unpack(args,":getch") != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 result = getch();
 DeeCurses_RELEASE();
 return DeeObject_New(int,DeeCurses_PrivateKeycodeToPublic(result));
}
static DeeObject *_curses_ungetch(DeeObject *args) {
 int err,ch;
 if (DeeTuple_Unpack(args,"d:ungetch",&ch) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 err = ungetch(DeeCurses_PublicKeycodeToPrivate(ch));
 DeeCurses_RELEASE();
 if (err == ERR) { _deecurses_error("ungetch"); return NULL; }
 DeeReturn_None;
}
static DeeObject *_curses_COLOR_PAIR(DeeObject *args) {
 chtype n;
 if (DeeTuple_Unpack(args,F_chtype ":COLOR_PAIR",&n) != 0) return NULL;
 return DeeObject_New(T_chtype,COLOR_PAIR(n));
}
static DeeObject *_curses_PAIR_NUMBER(DeeObject *args) {
 chtype n;
 if (DeeTuple_Unpack(args,F_chtype ":PAIR_NUMBER",&n) != 0) return NULL;
 return DeeObject_New(uint8_t,PAIR_NUMBER(n));
}

static DeeObject *_curses_getbegx(DeeObject *args) {
 DeeCursesWindowObject *wnd; int result;
 if (DeeTuple_Unpack(args,"o:getbegx",&wnd) != 0) return NULL;
 if (DeeError_TypeError_CheckType((DeeObject *)wnd,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 result = getbegx(wnd->w_wnd);
 DeeCurses_RELEASE();
 return DeeObject_New(int,result);
}
static DeeObject *_curses_getbegy(DeeObject *args) {
 DeeCursesWindowObject *wnd; int result;
 if (DeeTuple_Unpack(args,"o:getbegy",&wnd) != 0) return NULL;
 if (DeeError_TypeError_CheckType((DeeObject *)wnd,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 result = getbegy(wnd->w_wnd);
 DeeCurses_RELEASE();
 return DeeObject_New(int,result);
}
static DeeObject *_curses_getmaxx(DeeObject *args) {
 DeeCursesWindowObject *wnd; int result;
 if (DeeTuple_Unpack(args,"o:getmaxx",&wnd) != 0) return NULL;
 if (DeeError_TypeError_CheckType((DeeObject *)wnd,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 result = getmaxx(wnd->w_wnd);
 DeeCurses_RELEASE();
 return DeeObject_New(int,result);
}
static DeeObject *_curses_getmaxy(DeeObject *args) {
 DeeCursesWindowObject *wnd; int result;
 if (DeeTuple_Unpack(args,"o:getmaxy",&wnd) != 0) return NULL;
 if (DeeError_TypeError_CheckType((DeeObject *)wnd,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 result = getmaxy(wnd->w_wnd);
 DeeCurses_RELEASE();
 return DeeObject_New(int,result);
}
static DeeObject *_curses_getparx(DeeObject *args) {
 DeeCursesWindowObject *wnd; int result;
 if (DeeTuple_Unpack(args,"o:getparx",&wnd) != 0) return NULL;
 if (DeeError_TypeError_CheckType((DeeObject *)wnd,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 result = getparx(wnd->w_wnd);
 DeeCurses_RELEASE();
 return DeeObject_New(int,result);
}
static DeeObject *_curses_getpary(DeeObject *args) {
 DeeCursesWindowObject *wnd; int result;
 if (DeeTuple_Unpack(args,"o:getpary",&wnd) != 0) return NULL;
 if (DeeError_TypeError_CheckType((DeeObject *)wnd,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 result = getpary(wnd->w_wnd);
 DeeCurses_RELEASE();
 return DeeObject_New(int,result);
}
static DeeObject *_curses_getcurx(DeeObject *args) {
 DeeCursesWindowObject *wnd; int result;
 if (DeeTuple_Unpack(args,"o:getcurx",&wnd) != 0) return NULL;
 if (DeeError_TypeError_CheckType((DeeObject *)wnd,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 result = getcurx(wnd->w_wnd);
 DeeCurses_RELEASE();
 return DeeObject_New(int,result);
}
static DeeObject *_curses_getcury(DeeObject *args) {
 DeeCursesWindowObject *wnd; int result;
 if (DeeTuple_Unpack(args,"o:getcury",&wnd) != 0) return NULL;
 if (DeeError_TypeError_CheckType((DeeObject *)wnd,&DeeCursesWindow_Type) != 0) return NULL;
 if (DeeCurses_ACQUIRE() != 0) return NULL;
 result = getcury(wnd->w_wnd);
 DeeCurses_RELEASE();
 return DeeObject_New(int,result);
}
static DeeObject *_curses_keyname(DeeObject *args) {
 int key;
 if (DeeTuple_Unpack(args,"d:keyname",&key) != 0) return NULL;
 return DeeCurses_KeyToStr(key);
}


/*TODO
#define getbegyx(w, y, x)  (y = getbegy(w), x = getbegx(w))
#define getmaxyx(w, y, x)  (y = getmaxy(w), x = getmaxx(w))
#define getparyx(w, y, x)  (y = getpary(w), x = getparx(w))
#define getyx(w, y, x)     (y = getcury(w), x = getcurx(w))

#define getsyx(y, x)       { if (curscr->_leaveit) (y)=(x)=-1; \
                             else getyx(curscr,(y),(x)); }
*/


#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
