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
#include "_curses.generated.inl"
DEE_DECL_BEGIN
struct DeeDexExportDef ___stub_DeeDex_Exports_stub___[] = {
#endif

// BEGIN GENERATED CODE
 DeeDex_EXPORT_FUNCTION("addch",&_curses_addch,"addch(chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("addchnstr",&_curses_addchnstr,"addchnstr(chtype *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("addchstr",&_curses_addchstr,"addchstr(chtype *) -> none"),
 DeeDex_EXPORT_FUNCTION("addnstr",&_curses_addnstr,"addnstr(signed char *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("addstr",&_curses_addstr,"addstr(signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("attroff",&_curses_attroff,"attroff(chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("attron",&_curses_attron,"attron(chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("attrset",&_curses_attrset,"attrset(chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("attr_get",&_curses_attr_get,"attr_get(attr_t *,short *,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("attr_off",&_curses_attr_off,"attr_off(attr_t,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("attr_on",&_curses_attr_on,"attr_on(attr_t,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("attr_set",&_curses_attr_set,"attr_set(attr_t,short,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("baudrate",&_curses_baudrate,"baudrate() -> none"),
 DeeDex_EXPORT_FUNCTION("beep",&_curses_beep,"beep() -> none"),
 DeeDex_EXPORT_FUNCTION("bkgd",&_curses_bkgd,"bkgd(chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("bkgdset",&_curses_bkgdset,"bkgdset(chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("border",&_curses_border,"border(chtype,chtype,chtype,chtype,chtype,chtype,chtype,chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("can_change_color",&_curses_can_change_color,"can_change_color() -> bool"),
 DeeDex_EXPORT_FUNCTION("cbreak",&_curses_cbreak,"cbreak() -> none"),
 DeeDex_EXPORT_FUNCTION("chgat",&_curses_chgat,"chgat(int,attr_t,short,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("clearok",&_curses_clearok,"clearok(WINDOW,bool) -> none"),
 DeeDex_EXPORT_FUNCTION("clear",&_curses_clear,"clear() -> none"),
 DeeDex_EXPORT_FUNCTION("clrtobot",&_curses_clrtobot,"clrtobot() -> none"),
 DeeDex_EXPORT_FUNCTION("clrtoeol",&_curses_clrtoeol,"clrtoeol() -> none"),
 DeeDex_EXPORT_FUNCTION("color_content",&_curses_color_content,"color_content(short,short *,short *,short *) -> none"),
 DeeDex_EXPORT_FUNCTION("color_set",&_curses_color_set,"color_set(short,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("copywin",&_curses_copywin,"copywin(WINDOW,WINDOW,int,int,int,int,int,int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("curs_set",&_curses_curs_set,"curs_set(int) -> none"),
 DeeDex_EXPORT_FUNCTION("def_prog_mode",&_curses_def_prog_mode,"def_prog_mode() -> none"),
 DeeDex_EXPORT_FUNCTION("def_shell_mode",&_curses_def_shell_mode,"def_shell_mode() -> none"),
 DeeDex_EXPORT_FUNCTION("delay_output",&_curses_delay_output,"delay_output(int) -> none"),
 DeeDex_EXPORT_FUNCTION("delch",&_curses_delch,"delch() -> none"),
 DeeDex_EXPORT_FUNCTION("deleteln",&_curses_deleteln,"deleteln() -> none"),
 DeeDex_EXPORT_FUNCTION("delscreen",&_curses_delscreen,"delscreen(SCREEN) -> none"),
 DeeDex_EXPORT_FUNCTION("delwin",&_curses_delwin,"delwin(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("derwin",&_curses_derwin,"derwin(WINDOW,int,int,int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("doupdate",&_curses_doupdate,"doupdate() -> none"),
 DeeDex_EXPORT_FUNCTION("dupwin",&_curses_dupwin,"dupwin(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("echochar",&_curses_echochar,"echochar(chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("echo",&_curses_echo,"echo() -> none"),
 DeeDex_EXPORT_FUNCTION("erasechar",&_curses_erasechar,"erasechar() -> signed char"),
 DeeDex_EXPORT_FUNCTION("erase",&_curses_erase,"erase() -> none"),
 DeeDex_EXPORT_FUNCTION("filter",&_curses_filter,"filter() -> none"),
 DeeDex_EXPORT_FUNCTION("flash",&_curses_flash,"flash() -> none"),
 DeeDex_EXPORT_FUNCTION("flushinp",&_curses_flushinp,"flushinp() -> none"),
 DeeDex_EXPORT_FUNCTION("getbkgd",&_curses_getbkgd,"getbkgd(WINDOW) -> chtype"),
 DeeDex_EXPORT_FUNCTION("getnstr",&_curses_getnstr,"getnstr(signed char *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("getstr",&_curses_getstr,"getstr(signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("halfdelay",&_curses_halfdelay,"halfdelay(int) -> none"),
 DeeDex_EXPORT_FUNCTION("has_colors",&_curses_has_colors,"has_colors() -> bool"),
 DeeDex_EXPORT_FUNCTION("has_ic",&_curses_has_ic,"has_ic() -> bool"),
 DeeDex_EXPORT_FUNCTION("has_il",&_curses_has_il,"has_il() -> bool"),
 DeeDex_EXPORT_FUNCTION("hline",&_curses_hline,"hline(chtype,int) -> none"),
 DeeDex_EXPORT_FUNCTION("idcok",&_curses_idcok,"idcok(WINDOW,bool) -> none"),
 DeeDex_EXPORT_FUNCTION("idlok",&_curses_idlok,"idlok(WINDOW,bool) -> none"),
 DeeDex_EXPORT_FUNCTION("immedok",&_curses_immedok,"immedok(WINDOW,bool) -> none"),
 DeeDex_EXPORT_FUNCTION("inchnstr",&_curses_inchnstr,"inchnstr(chtype *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("inchstr",&_curses_inchstr,"inchstr(chtype *) -> none"),
 DeeDex_EXPORT_FUNCTION("inch",&_curses_inch,"inch() -> chtype"),
 DeeDex_EXPORT_FUNCTION("init_color",&_curses_init_color,"init_color(short,short,short,short) -> none"),
 DeeDex_EXPORT_FUNCTION("init_pair",&_curses_init_pair,"init_pair(short,short,short) -> none"),
 DeeDex_EXPORT_FUNCTION("innstr",&_curses_innstr,"innstr(signed char *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("insch",&_curses_insch,"insch(chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("insdelln",&_curses_insdelln,"insdelln(int) -> none"),
 DeeDex_EXPORT_FUNCTION("insertln",&_curses_insertln,"insertln() -> none"),
 DeeDex_EXPORT_FUNCTION("insnstr",&_curses_insnstr,"insnstr(signed char *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("insstr",&_curses_insstr,"insstr(signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("instr",&_curses_instr,"instr(signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("intrflush",&_curses_intrflush,"intrflush(WINDOW,bool) -> none"),
 DeeDex_EXPORT_FUNCTION("isendwin",&_curses_isendwin,"isendwin() -> bool"),
 DeeDex_EXPORT_FUNCTION("is_linetouched",&_curses_is_linetouched,"is_linetouched(WINDOW,int) -> bool"),
 DeeDex_EXPORT_FUNCTION("is_wintouched",&_curses_is_wintouched,"is_wintouched(WINDOW) -> bool"),
 DeeDex_EXPORT_FUNCTION("keyname",&_curses_keyname,"keyname(int) -> string"),
 DeeDex_EXPORT_FUNCTION("keypad",&_curses_keypad,"keypad(WINDOW,bool) -> none"),
 DeeDex_EXPORT_FUNCTION("killchar",&_curses_killchar,"killchar() -> signed char"),
 DeeDex_EXPORT_FUNCTION("leaveok",&_curses_leaveok,"leaveok(WINDOW,bool) -> none"),
 DeeDex_EXPORT_FUNCTION("longname",&_curses_longname,"longname() -> string"),
 DeeDex_EXPORT_FUNCTION("meta",&_curses_meta,"meta(WINDOW,bool) -> none"),
 DeeDex_EXPORT_FUNCTION("move",&_curses_move,"move(int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvaddch",&_curses_mvaddch,"mvaddch(int,int,chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("mvaddchnstr",&_curses_mvaddchnstr,"mvaddchnstr(int,int,chtype *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvaddchstr",&_curses_mvaddchstr,"mvaddchstr(int,int,chtype *) -> none"),
 DeeDex_EXPORT_FUNCTION("mvaddnstr",&_curses_mvaddnstr,"mvaddnstr(int,int,signed char *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvaddstr",&_curses_mvaddstr,"mvaddstr(int,int,signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("mvchgat",&_curses_mvchgat,"mvchgat(int,int,int,attr_t,short,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("mvcur",&_curses_mvcur,"mvcur(int,int,int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvdelch",&_curses_mvdelch,"mvdelch(int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvderwin",&_curses_mvderwin,"mvderwin(WINDOW,int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvgetnstr",&_curses_mvgetnstr,"mvgetnstr(int,int,signed char *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvgetstr",&_curses_mvgetstr,"mvgetstr(int,int,signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("mvhline",&_curses_mvhline,"mvhline(int,int,chtype,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvinch",&_curses_mvinch,"mvinch(int,int) -> chtype"),
 DeeDex_EXPORT_FUNCTION("mvinchnstr",&_curses_mvinchnstr,"mvinchnstr(int,int,chtype *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvinchstr",&_curses_mvinchstr,"mvinchstr(int,int,chtype *) -> none"),
 DeeDex_EXPORT_FUNCTION("mvinnstr",&_curses_mvinnstr,"mvinnstr(int,int,signed char *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvinsch",&_curses_mvinsch,"mvinsch(int,int,chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("mvinsnstr",&_curses_mvinsnstr,"mvinsnstr(int,int,signed char *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvinsstr",&_curses_mvinsstr,"mvinsstr(int,int,signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("mvinstr",&_curses_mvinstr,"mvinstr(int,int,signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("mvvline",&_curses_mvvline,"mvvline(int,int,chtype,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwaddchnstr",&_curses_mvwaddchnstr,"mvwaddchnstr(WINDOW,int,int,chtype *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwaddchstr",&_curses_mvwaddchstr,"mvwaddchstr(WINDOW,int,int,chtype *) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwaddch",&_curses_mvwaddch,"mvwaddch(WINDOW,int,int,chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwaddnstr",&_curses_mvwaddnstr,"mvwaddnstr(WINDOW,int,int,signed char *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwaddstr",&_curses_mvwaddstr,"mvwaddstr(WINDOW,int,int,signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwchgat",&_curses_mvwchgat,"mvwchgat(WINDOW,int,int,int,attr_t,short,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwdelch",&_curses_mvwdelch,"mvwdelch(WINDOW,int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwgetnstr",&_curses_mvwgetnstr,"mvwgetnstr(WINDOW,int,int,signed char *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwgetstr",&_curses_mvwgetstr,"mvwgetstr(WINDOW,int,int,signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwhline",&_curses_mvwhline,"mvwhline(WINDOW,int,int,chtype,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwinchnstr",&_curses_mvwinchnstr,"mvwinchnstr(WINDOW,int,int,chtype *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwinchstr",&_curses_mvwinchstr,"mvwinchstr(WINDOW,int,int,chtype *) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwinch",&_curses_mvwinch,"mvwinch(WINDOW,int,int) -> chtype"),
 DeeDex_EXPORT_FUNCTION("mvwinnstr",&_curses_mvwinnstr,"mvwinnstr(WINDOW,int,int,signed char *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwinsch",&_curses_mvwinsch,"mvwinsch(WINDOW,int,int,chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwinsnstr",&_curses_mvwinsnstr,"mvwinsnstr(WINDOW,int,int,signed char *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwinsstr",&_curses_mvwinsstr,"mvwinsstr(WINDOW,int,int,signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwinstr",&_curses_mvwinstr,"mvwinstr(WINDOW,int,int,signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwin",&_curses_mvwin,"mvwin(WINDOW,int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwvline",&_curses_mvwvline,"mvwvline(WINDOW,int,int,chtype,int) -> none"),
 DeeDex_EXPORT_FUNCTION("napms",&_curses_napms,"napms(int) -> none"),
 DeeDex_EXPORT_FUNCTION("nl",&_curses_nl,"nl() -> none"),
 DeeDex_EXPORT_FUNCTION("nocbreak",&_curses_nocbreak,"nocbreak() -> none"),
 DeeDex_EXPORT_FUNCTION("nodelay",&_curses_nodelay,"nodelay(WINDOW,bool) -> none"),
 DeeDex_EXPORT_FUNCTION("noecho",&_curses_noecho,"noecho() -> none"),
 DeeDex_EXPORT_FUNCTION("nonl",&_curses_nonl,"nonl() -> none"),
 DeeDex_EXPORT_FUNCTION("noqiflush",&_curses_noqiflush,"noqiflush() -> none"),
 DeeDex_EXPORT_FUNCTION("noraw",&_curses_noraw,"noraw() -> none"),
 DeeDex_EXPORT_FUNCTION("notimeout",&_curses_notimeout,"notimeout(WINDOW,bool) -> none"),
 DeeDex_EXPORT_FUNCTION("overlay",&_curses_overlay,"overlay(WINDOW,WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("overwrite",&_curses_overwrite,"overwrite(WINDOW,WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("pair_content",&_curses_pair_content,"pair_content(short,short *,short *) -> none"),
 DeeDex_EXPORT_FUNCTION("pechochar",&_curses_pechochar,"pechochar(WINDOW,chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("pnoutrefresh",&_curses_pnoutrefresh,"pnoutrefresh(WINDOW,int,int,int,int,int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("prefresh",&_curses_prefresh,"prefresh(WINDOW,int,int,int,int,int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("qiflush",&_curses_qiflush,"qiflush() -> none"),
 DeeDex_EXPORT_FUNCTION("raw",&_curses_raw,"raw() -> none"),
 DeeDex_EXPORT_FUNCTION("redrawwin",&_curses_redrawwin,"redrawwin(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("refresh",&_curses_refresh,"refresh() -> none"),
 DeeDex_EXPORT_FUNCTION("reset_prog_mode",&_curses_reset_prog_mode,"reset_prog_mode() -> none"),
 DeeDex_EXPORT_FUNCTION("reset_shell_mode",&_curses_reset_shell_mode,"reset_shell_mode() -> none"),
 DeeDex_EXPORT_FUNCTION("resetty",&_curses_resetty,"resetty() -> none"),
 DeeDex_EXPORT_FUNCTION("savetty",&_curses_savetty,"savetty() -> none"),
 DeeDex_EXPORT_FUNCTION("scr_dump",&_curses_scr_dump,"scr_dump(signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("scr_init",&_curses_scr_init,"scr_init(signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("scr_restore",&_curses_scr_restore,"scr_restore(signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("scr_set",&_curses_scr_set,"scr_set(signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("scrl",&_curses_scrl,"scrl(int) -> none"),
 DeeDex_EXPORT_FUNCTION("scroll",&_curses_scroll,"scroll(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("scrollok",&_curses_scrollok,"scrollok(WINDOW,bool) -> none"),
 DeeDex_EXPORT_FUNCTION("setscrreg",&_curses_setscrreg,"setscrreg(int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("slk_attroff",&_curses_slk_attroff,"slk_attroff(chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("slk_attr_off",&_curses_slk_attr_off,"slk_attr_off(attr_t,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("slk_attron",&_curses_slk_attron,"slk_attron(chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("slk_attr_on",&_curses_slk_attr_on,"slk_attr_on(attr_t,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("slk_attrset",&_curses_slk_attrset,"slk_attrset(chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("slk_attr_set",&_curses_slk_attr_set,"slk_attr_set(attr_t,short,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("slk_clear",&_curses_slk_clear,"slk_clear() -> none"),
 DeeDex_EXPORT_FUNCTION("slk_color",&_curses_slk_color,"slk_color(short) -> none"),
 DeeDex_EXPORT_FUNCTION("slk_init",&_curses_slk_init,"slk_init(int) -> none"),
 DeeDex_EXPORT_FUNCTION("slk_label",&_curses_slk_label,"slk_label(int) -> string"),
 DeeDex_EXPORT_FUNCTION("slk_noutrefresh",&_curses_slk_noutrefresh,"slk_noutrefresh() -> none"),
 DeeDex_EXPORT_FUNCTION("slk_refresh",&_curses_slk_refresh,"slk_refresh() -> none"),
 DeeDex_EXPORT_FUNCTION("slk_restore",&_curses_slk_restore,"slk_restore() -> none"),
 DeeDex_EXPORT_FUNCTION("slk_set",&_curses_slk_set,"slk_set(int,signed char *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("slk_touch",&_curses_slk_touch,"slk_touch() -> none"),
 DeeDex_EXPORT_FUNCTION("standend",&_curses_standend,"standend() -> none"),
 DeeDex_EXPORT_FUNCTION("standout",&_curses_standout,"standout() -> none"),
 DeeDex_EXPORT_FUNCTION("start_color",&_curses_start_color,"start_color() -> none"),
 DeeDex_EXPORT_FUNCTION("subpad",&_curses_subpad,"subpad(WINDOW,int,int,int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("subwin",&_curses_subwin,"subwin(WINDOW,int,int,int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("syncok",&_curses_syncok,"syncok(WINDOW,bool) -> none"),
 DeeDex_EXPORT_FUNCTION("termattrs",&_curses_termattrs,"termattrs() -> chtype"),
 DeeDex_EXPORT_FUNCTION("termname",&_curses_termname,"termname() -> string"),
 DeeDex_EXPORT_FUNCTION("timeout",&_curses_timeout,"timeout(int) -> none"),
 DeeDex_EXPORT_FUNCTION("touchline",&_curses_touchline,"touchline(WINDOW,int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("touchwin",&_curses_touchwin,"touchwin(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("typeahead",&_curses_typeahead,"typeahead(int) -> none"),
 DeeDex_EXPORT_FUNCTION("untouchwin",&_curses_untouchwin,"untouchwin(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("use_env",&_curses_use_env,"use_env(bool) -> none"),
 DeeDex_EXPORT_FUNCTION("vidattr",&_curses_vidattr,"vidattr(chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("vid_attr",&_curses_vid_attr,"vid_attr(attr_t,short,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("vline",&_curses_vline,"vline(chtype,int) -> none"),
 DeeDex_EXPORT_FUNCTION("waddchnstr",&_curses_waddchnstr,"waddchnstr(WINDOW,chtype *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("waddchstr",&_curses_waddchstr,"waddchstr(WINDOW,chtype *) -> none"),
 DeeDex_EXPORT_FUNCTION("waddch",&_curses_waddch,"waddch(WINDOW,chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("waddnstr",&_curses_waddnstr,"waddnstr(WINDOW,signed char *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("waddstr",&_curses_waddstr,"waddstr(WINDOW,signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("wattroff",&_curses_wattroff,"wattroff(WINDOW,chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("wattron",&_curses_wattron,"wattron(WINDOW,chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("wattrset",&_curses_wattrset,"wattrset(WINDOW,chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("wattr_get",&_curses_wattr_get,"wattr_get(WINDOW,attr_t *,short *,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("wattr_off",&_curses_wattr_off,"wattr_off(WINDOW,attr_t,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("wattr_on",&_curses_wattr_on,"wattr_on(WINDOW,attr_t,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("wattr_set",&_curses_wattr_set,"wattr_set(WINDOW,attr_t,short,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("wbkgdset",&_curses_wbkgdset,"wbkgdset(WINDOW,chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("wbkgd",&_curses_wbkgd,"wbkgd(WINDOW,chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("wborder",&_curses_wborder,"wborder(WINDOW,chtype,chtype,chtype,chtype,chtype,chtype,chtype,chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("wchgat",&_curses_wchgat,"wchgat(WINDOW,int,attr_t,short,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("wclear",&_curses_wclear,"wclear(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("wclrtobot",&_curses_wclrtobot,"wclrtobot(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("wclrtoeol",&_curses_wclrtoeol,"wclrtoeol(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("wcolor_set",&_curses_wcolor_set,"wcolor_set(WINDOW,short,none *) -> none"),
 DeeDex_EXPORT_FUNCTION("wcursyncup",&_curses_wcursyncup,"wcursyncup(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("wdelch",&_curses_wdelch,"wdelch(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("wdeleteln",&_curses_wdeleteln,"wdeleteln(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("wechochar",&_curses_wechochar,"wechochar(WINDOW,chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("werase",&_curses_werase,"werase(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("wgetnstr",&_curses_wgetnstr,"wgetnstr(WINDOW,signed char *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("wgetstr",&_curses_wgetstr,"wgetstr(WINDOW,signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("whline",&_curses_whline,"whline(WINDOW,chtype,int) -> none"),
 DeeDex_EXPORT_FUNCTION("winchnstr",&_curses_winchnstr,"winchnstr(WINDOW,chtype *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("winchstr",&_curses_winchstr,"winchstr(WINDOW,chtype *) -> none"),
 DeeDex_EXPORT_FUNCTION("winch",&_curses_winch,"winch(WINDOW) -> chtype"),
 DeeDex_EXPORT_FUNCTION("winnstr",&_curses_winnstr,"winnstr(WINDOW,signed char *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("winsch",&_curses_winsch,"winsch(WINDOW,chtype) -> none"),
 DeeDex_EXPORT_FUNCTION("winsdelln",&_curses_winsdelln,"winsdelln(WINDOW,int) -> none"),
 DeeDex_EXPORT_FUNCTION("winsertln",&_curses_winsertln,"winsertln(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("winsnstr",&_curses_winsnstr,"winsnstr(WINDOW,signed char *,int) -> none"),
 DeeDex_EXPORT_FUNCTION("winsstr",&_curses_winsstr,"winsstr(WINDOW,signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("winstr",&_curses_winstr,"winstr(WINDOW,signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("wmove",&_curses_wmove,"wmove(WINDOW,int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("wnoutrefresh",&_curses_wnoutrefresh,"wnoutrefresh(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("wprintw",&_curses_wprintw,"wprintw(WINDOW,signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("wredrawln",&_curses_wredrawln,"wredrawln(WINDOW,int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("wrefresh",&_curses_wrefresh,"wrefresh(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("wscanw",&_curses_wscanw,"wscanw(WINDOW,signed char *) -> none"),
 DeeDex_EXPORT_FUNCTION("wscrl",&_curses_wscrl,"wscrl(WINDOW,int) -> none"),
 DeeDex_EXPORT_FUNCTION("wsetscrreg",&_curses_wsetscrreg,"wsetscrreg(WINDOW,int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("wstandend",&_curses_wstandend,"wstandend(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("wstandout",&_curses_wstandout,"wstandout(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("wsyncdown",&_curses_wsyncdown,"wsyncdown(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("wsyncup",&_curses_wsyncup,"wsyncup(WINDOW) -> none"),
 DeeDex_EXPORT_FUNCTION("wtimeout",&_curses_wtimeout,"wtimeout(WINDOW,int) -> none"),
 DeeDex_EXPORT_FUNCTION("wtouchln",&_curses_wtouchln,"wtouchln(WINDOW,int,int,int) -> none"),
 DeeDex_EXPORT_FUNCTION("wvline",&_curses_wvline,"wvline(WINDOW,chtype,int) -> none"),
// END GENERATED CODE

 DeeDex_EXPORT_FUNCTION("box",&_curses_box,"box(WINDOW wnd, chtype = '|',chtype = '-') -> none"),
 DeeDex_EXPORT_FUNCTION("mvgetch",&_curses_mvgetch,"mvgetch(int y, int x) -> string\nmvgetch(int y, int x) -> none"),
 DeeDex_EXPORT_FUNCTION("mvwgetch",&_curses_mvwgetch,"mvwgetch(WINDOW wnd, int y, int x) -> string\nmvwgetch(WINDOW wnd, int y, int x) -> none"),
 DeeDex_EXPORT_FUNCTION("wgetch",&_curses_wgetch,"wgetch(WINDOW wnd) -> string\nwgetch(WINDOW wnd) -> none"),
 DeeDex_EXPORT_FUNCTION("getch",&_curses_getch,"getch() -> string\ngetch() -> none"),
 DeeDex_EXPORT_FUNCTION("ungetch",&_curses_ungetch,"ungetch(string ch) -> none"),
 DeeDex_EXPORT_FUNCTION("COLOR_PAIR",&_curses_COLOR_PAIR,"COLOR_PAIR(chtype n) -> chtype"),
 DeeDex_EXPORT_FUNCTION("PAIR_NUMBER",&_curses_PAIR_NUMBER,"PAIR_NUMBER(chtype n) -> uint8_t"),
 DeeDex_EXPORT_FUNCTION("getbegx",&_curses_getbegx,"getbegx(WINDOW wnd) -> int"),
 DeeDex_EXPORT_FUNCTION("getbegy",&_curses_getbegy,"getbegy(WINDOW wnd) -> int"),
 DeeDex_EXPORT_FUNCTION("getmaxx",&_curses_getmaxx,"getmaxx(WINDOW wnd) -> int"),
 DeeDex_EXPORT_FUNCTION("getmaxy",&_curses_getmaxy,"getmaxy(WINDOW wnd) -> int"),
 DeeDex_EXPORT_FUNCTION("getparx",&_curses_getparx,"getparx(WINDOW wnd) -> int"),
 DeeDex_EXPORT_FUNCTION("getpary",&_curses_getpary,"getpary(WINDOW wnd) -> int"),
 DeeDex_EXPORT_FUNCTION("getcurx",&_curses_getcurx,"getcurx(WINDOW wnd) -> int"),
 DeeDex_EXPORT_FUNCTION("getcury",&_curses_getcury,"getcury(WINDOW wnd) -> int"),


DEE_COMPILER_MSVC_WARNING_PUSH(4574)
#ifdef A_NORMAL
 DeeDex_EXPORT_CONST("A_NORMAL",T_chtype,A_NORMAL),
#endif
#ifdef A_CHARTEXT
 DeeDex_EXPORT_CONST("A_CHARTEXT",T_chtype,A_CHARTEXT),
#endif
#ifdef A_ALTCHARSET
 DeeDex_EXPORT_CONST("A_ALTCHARSET",T_chtype,A_ALTCHARSET),
#endif
#ifdef A_RIGHTLINE
 DeeDex_EXPORT_CONST("A_RIGHTLINE",T_chtype,A_RIGHTLINE),
#endif
#ifdef A_LEFTLINE
 DeeDex_EXPORT_CONST("A_LEFTLINE",T_chtype,A_LEFTLINE),
#endif
#ifdef A_INVIS
 DeeDex_EXPORT_CONST("A_INVIS",T_chtype,A_INVIS),
#endif
#ifdef A_UNDERLINE
 DeeDex_EXPORT_CONST("A_UNDERLINE",T_chtype,A_UNDERLINE),
#endif
#ifdef A_REVERSE
 DeeDex_EXPORT_CONST("A_REVERSE",T_chtype,A_REVERSE),
#endif
#ifdef A_BLINK
 DeeDex_EXPORT_CONST("A_BLINK",T_chtype,A_BLINK),
#endif
#ifdef A_BOLD
 DeeDex_EXPORT_CONST("A_BOLD",T_chtype,A_BOLD),
#endif
#ifdef A_OVERLINE
 DeeDex_EXPORT_CONST("A_OVERLINE",T_chtype,A_OVERLINE),
#endif
#ifdef A_STRIKEOUT
 DeeDex_EXPORT_CONST("A_STRIKEOUT",T_chtype,A_STRIKEOUT),
#endif
#ifdef A_DIM
 DeeDex_EXPORT_CONST("A_DIM",T_chtype,A_DIM),
#endif
#ifdef A_FUTURE_2
 DeeDex_EXPORT_CONST("A_FUTURE_2",T_chtype,A_FUTURE_2),
#endif
#ifdef A_COLOR
 DeeDex_EXPORT_CONST("A_COLOR",T_chtype,A_COLOR),
#endif
#ifdef A_RGB_COLOR
 DeeDex_EXPORT_CONST("A_RGB_COLOR",T_chtype,A_RGB_COLOR),
#endif
#ifdef A_ATTRIBUTES
 DeeDex_EXPORT_CONST("A_ATTRIBUTES",T_chtype,A_ATTRIBUTES),
#endif
#ifdef A_ITALIC
 DeeDex_EXPORT_CONST("A_ITALIC",T_chtype,A_ITALIC),
#endif
#ifdef A_PROTECT
 DeeDex_EXPORT_CONST("A_PROTECT",T_chtype,A_PROTECT),
#endif
#ifdef A_STANDOUT
 DeeDex_EXPORT_CONST("A_STANDOUT",T_chtype,A_STANDOUT),
#endif

#ifdef COLOR_BLACK
 DeeDex_EXPORT_CONST("COLOR_BLACK",int,COLOR_BLACK),
#endif
#ifdef COLOR_BLUE
 DeeDex_EXPORT_CONST("COLOR_BLUE",int,COLOR_BLUE),
#endif
#ifdef COLOR_GREEN
 DeeDex_EXPORT_CONST("COLOR_GREEN",int,COLOR_GREEN),
#endif
#ifdef COLOR_RED
 DeeDex_EXPORT_CONST("COLOR_RED",int,COLOR_RED),
#endif
#ifdef COLOR_CYAN
 DeeDex_EXPORT_CONST("COLOR_CYAN",int,COLOR_CYAN),
#endif
#ifdef COLOR_MAGENTA
 DeeDex_EXPORT_CONST("COLOR_MAGENTA",int,COLOR_MAGENTA),
#endif
#ifdef COLOR_YELLOW
 DeeDex_EXPORT_CONST("COLOR_YELLOW",int,COLOR_YELLOW),
#endif
#ifdef COLOR_WHITE
 DeeDex_EXPORT_CONST("COLOR_WHITE",int,COLOR_WHITE),
#endif

 
 
 DEE_COMPILER_MSVC_WARNING_POP

#ifdef __INTELLISENSE__
}
DEE_DECL_END
#endif
