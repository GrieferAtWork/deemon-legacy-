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
#undef curscr
#undef stdscr
#undef ttytype
#undef COLORS
#undef COLOR_PAIRS
#undef COLS
#undef LINES
#undef TABSIZE
#undef def_prog_mode
#undef def_shell_mode
#undef reset_prog_mode
#undef reset_shell_mode
#undef resetty
#undef savetty
#undef getsyx
#undef ripoffline
#undef curs_set
#undef napms
#undef wrefresh
#undef wnoutrefresh
#undef doupdate
#undef redrawwin
#undef wredrawln
#undef getbegx
#undef getbegy
#undef getmaxx
#undef getmaxy
#undef getparx
#undef getpary
#undef getcurx
#undef getcury
#undef clearok
#undef idlok
#undef idcok
#undef immedok
#undef leaveok
#undef wsetscrreg
#undef scrollok
#undef nl
#undef nonl
#undef start_color
#undef init_pair
#undef init_color
#undef has_colors
#undef can_change_color
#undef color_content
#undef pair_content
#undef overlay
#undef overwrite
#undef copywin
#undef keyname
#undef filter
#undef use_env
#undef putwin
#undef getwin
#undef delay_output
#undef flushinp
#undef initscr
#undef endwin
#undef isendwin
#undef newterm
#undef set_term
#undef delscreen
#undef newwin
#undef delwin
#undef mvwin
#undef subwin
#undef derwin
#undef mvderwin
#undef dupwin
#undef wsyncup
#undef syncok
#undef wcursyncup
#undef wsyncdown
#undef wbkgdset
#undef wbkgd
#undef getbkgd
#undef cbreak
#undef nocbreak
#undef echo
#undef noecho
#undef halfdelay
#undef intrflush
#undef keypad
#undef meta
#undef nodelay
#undef raw
#undef noraw
#undef noqiflush
#undef qiflush
#undef notimeout
#undef wtimeout
#undef typeahead
#undef baudrate
#undef erasechar
#undef has_ic
#undef has_il
#undef killchar
#undef longname
#undef term_attrs
#undef termattrs
#undef termname
#undef beep
#undef flash
#undef wtouchln
#undef is_linetouched
#undef is_wintouched
#undef vidputs
#undef vidattr
//#undef vid_puts
#undef vid_attr
#undef mvcur
#undef printw
#undef wprintw
#undef mvprintw
#undef mvwprintw
#undef vwprintw
#undef scanw
#undef wscanw
#undef mvscanw
#undef mvwscanw
#undef vwscanw
#undef newpad
#undef subpad
#undef prefresh
#undef pnoutrefresh
#undef pechochar
#undef scr_dump
#undef scr_restore
#undef scr_init
#undef scr_set
#undef slk_init
#undef slk_set
#undef slk_refresh
#undef slk_noutrefresh
#undef slk_label
#undef slk_clear
#undef slk_restore
#undef slk_touch
#undef slk_attron
#undef slk_attroff
#undef slk_attrset
#undef slk_attr_set
#undef slk_color
#undef waddchnstr
#undef waddch
#undef wechochar
#undef waddnstr
#undef wattrset
#undef wcolor_set
#undef wattr_get
#undef wattr_off
#undef wattr_on
#undef wattr_set
#undef wchgat
#undef wborder
#undef whline
#undef wvline
#undef werase
#undef wclear
#undef wclrtobot
#undef wclrtoeol
#undef wdelch
#undef wgetch
#undef wgetnstr
#undef winchnstr
#undef winch
#undef winnstr
#undef winsch
#undef winsdelln
#undef winsnstr
#undef wmove
#undef wscrl
#undef getyx
#undef getbegyx
#undef getmaxyx
#undef getparyx
#undef wgetstr
#undef getnstr
#undef wstandout
#undef wstandend
#undef wattron
#undef wattroff
#undef scroll
#undef touchwin
#undef touchline
#undef untouchwin
#undef box
#undef border
#undef hline
#undef vline
#undef winstr
#undef winchstr
#undef winsstr
#undef waddstr
#undef waddchstr
#undef addch
#undef addchnstr
#undef addchstr
#undef addnstr
#undef addstr
#undef attr_get
#undef attr_off
#undef attr_on
#undef attr_set
#undef attroff
#undef attron
#undef attrset
#undef bkgd
#undef bkgdset
#undef chgat
#undef clear
#undef clrtobot
#undef clrtoeol
#undef color_set
#undef delch
#undef deleteln
#undef echochar
#undef erase
#undef getch
#undef getstr
#undef inch
#undef inchnstr
#undef inchstr
#undef innstr
#undef insch
#undef insdelln
#undef insertln
#undef insnstr
#undef insstr
#undef instr
#undef move
#undef refresh
#undef scrl
#undef setscrreg
#undef standend
#undef standout
#undef timeout
#undef wdeleteln
#undef winsertln
#undef mvwaddch
#undef mvwaddchnstr
#undef mvwaddchstr
#undef mvwaddnstr
#undef mvwaddstr
#undef mvwchgat
#undef mvwdelch
#undef mvwgetch
#undef mvwgetnstr
#undef mvwgetstr
#undef mvwhline
#undef mvwinch
#undef mvwinchnstr
#undef mvwinchstr
#undef mvwinnstr
#undef mvwinsch
#undef mvwinsnstr
#undef mvwinsstr
#undef mvwinstr
#undef mvwvline
#undef mvaddch
#undef mvaddchnstr
#undef mvaddchstr
#undef mvaddnstr
#undef mvaddstr
#undef mvchgat
#undef mvdelch
#undef mvgetch
#undef mvgetnstr
#undef mvgetstr
#undef mvhline
#undef mvinch
#undef mvinchnstr
#undef mvinchstr
#undef mvinnstr
#undef mvinsch
#undef mvinsnstr
#undef mvinsstr
#undef mvinstr
#undef mvvline
#undef slk_attr_off
#undef slk_attr_on
#undef vw_printw
#undef vw_scanw
#define curscr           curses_curscr
#define stdscr           curses_stdscr
#define ttytype          curses_ttytype
#define COLORS           curses_COLORS
#define COLOR_PAIRS      curses_COLOR_PAIRS
#define COLS             curses_COLS
#define LINES            curses_LINES
#define TABSIZE          curses_TABSIZE
#define def_prog_mode    curses_def_prog_mode
#define def_shell_mode   curses_def_shell_mode
#define reset_prog_mode  curses_reset_prog_mode
#define reset_shell_mode curses_reset_shell_mode
#define resetty          curses_resetty
#define savetty          curses_savetty
#define getsyx           curses_getsyx
#define ripoffline       curses_ripoffline
#define curs_set         curses_curs_set
#define napms            curses_napms
#define wrefresh         curses_wrefresh
#define wnoutrefresh     curses_wnoutrefresh
#define doupdate         curses_doupdate
#define redrawwin        curses_redrawwin
#define wredrawln        curses_wredrawln
#define getbegx          curses_getbegx
#define getbegy          curses_getbegy
#define getmaxx          curses_getmaxx
#define getmaxy          curses_getmaxy
#define getparx          curses_getparx
#define getpary          curses_getpary
#define getcurx          curses_getcurx
#define getcury          curses_getcury
#define clearok          curses_clearok
#define idlok            curses_idlok
#define idcok            curses_idcok
#define immedok          curses_immedok
#define leaveok          curses_leaveok
#define wsetscrreg       curses_wsetscrreg
#define scrollok         curses_scrollok
#define nl               curses_nl
#define nonl             curses_nonl
#define start_color      curses_start_color
#define init_pair        curses_init_pair
#define init_color       curses_init_color
#define has_colors       curses_has_colors
#define can_change_color curses_can_change_color
#define color_content    curses_color_content
#define pair_content     curses_pair_content
#define overlay          curses_overlay
#define overwrite        curses_overwrite
#define copywin          curses_copywin
#define keyname          curses_keyname
#define filter           curses_filter
#define use_env          curses_use_env
#define putwin           curses_putwin
#define getwin           curses_getwin
#define delay_output     curses_delay_output
#define flushinp         curses_flushinp
#define initscr          curses_initscr
#define endwin           curses_endwin
#define isendwin         curses_isendwin
#define newterm          curses_newterm
#define set_term         curses_set_term
#define delscreen        curses_delscreen
#define newwin           curses_newwin
#define delwin           curses_delwin
#define mvwin            curses_mvwin
#define subwin           curses_subwin
#define derwin           curses_derwin
#define mvderwin         curses_mvderwin
#define dupwin           curses_dupwin
#define wsyncup          curses_wsyncup
#define syncok           curses_syncok
#define wcursyncup       curses_wcursyncup
#define wsyncdown        curses_wsyncdown
#define wbkgdset         curses_wbkgdset
#define wbkgd            curses_wbkgd
#define getbkgd          curses_getbkgd
#define cbreak           curses_cbreak
#define nocbreak         curses_nocbreak
#define echo             curses_echo
#define noecho           curses_noecho
#define halfdelay        curses_halfdelay
#define intrflush        curses_intrflush
#define keypad           curses_keypad
#define meta             curses_meta
#define nodelay          curses_nodelay
#define raw              curses_raw
#define noraw            curses_noraw
#define noqiflush        curses_noqiflush
#define qiflush          curses_qiflush
#define notimeout        curses_notimeout
#define wtimeout         curses_wtimeout
#define typeahead        curses_typeahead
#define baudrate         curses_baudrate
#define erasechar        curses_erasechar
#define has_ic           curses_has_ic
#define has_il           curses_has_il
#define killchar         curses_killchar
#define longname         curses_longname
#define term_attrs       curses_term_attrs
#define termattrs        curses_termattrs
#define termname         curses_termname
#define beep             curses_beep
#define flash            curses_flash
#define wtouchln         curses_wtouchln
#define is_linetouched   curses_is_linetouched
#define is_wintouched    curses_is_wintouched
#define vidputs          curses_vidputs
#define vidattr          curses_vidattr
//#define vid_puts       curses_vid_puts
#define vid_attr         curses_vid_attr
#define mvcur            curses_mvcur
#define printw           curses_printw
#define wprintw          curses_wprintw
#define mvprintw         curses_mvprintw
#define mvwprintw        curses_mvwprintw
#define vwprintw         curses_vwprintw
#define scanw            curses_scanw
#define wscanw           curses_wscanw
#define mvscanw          curses_mvscanw
#define mvwscanw         curses_mvwscanw
#define vwscanw          curses_vwscanw
#define newpad           curses_newpad
#define subpad           curses_subpad
#define prefresh         curses_prefresh
#define pnoutrefresh     curses_pnoutrefresh
#define pechochar        curses_pechochar
#define scr_dump         curses_scr_dump
#define scr_restore      curses_scr_restore
#define scr_init         curses_scr_init
#define scr_set          curses_scr_set
#define slk_init         curses_slk_init
#define slk_set          curses_slk_set
#define slk_refresh      curses_slk_refresh
#define slk_noutrefresh  curses_slk_noutrefresh
#define slk_label        curses_slk_label
#define slk_clear        curses_slk_clear
#define slk_restore      curses_slk_restore
#define slk_touch        curses_slk_touch
#define slk_attron       curses_slk_attron
#define slk_attroff      curses_slk_attroff
#define slk_attrset      curses_slk_attrset
#define slk_attr_set     curses_slk_attr_set
#define slk_color        curses_slk_color
#define waddchnstr       curses_waddchnstr
#define waddch           curses_waddch
#define wechochar        curses_wechochar
#define waddnstr         curses_waddnstr
#define wattrset         curses_wattrset
#define wcolor_set       curses_wcolor_set
#define wattr_get        curses_wattr_get
#define wattr_off        curses_wattr_off
#define wattr_on         curses_wattr_on
#define wattr_set        curses_wattr_set
#define wchgat           curses_wchgat
#define wborder          curses_wborder
#define whline           curses_whline
#define wvline           curses_wvline
#define werase           curses_werase
#define wclear           curses_wclear
#define wclrtobot        curses_wclrtobot
#define wclrtoeol        curses_wclrtoeol
#define wdelch           curses_wdelch
#define wgetch           curses_wgetch
#define wgetnstr         curses_wgetnstr
#define winchnstr        curses_winchnstr
#define winch            curses_winch
#define winnstr          curses_winnstr
#define winsch           curses_winsch
#define winsdelln        curses_winsdelln
#define winsnstr         curses_winsnstr
#define wmove            curses_wmove
#define wscrl            curses_wscrl
#define getyx            curses_getyx
#define getbegyx         curses_getbegyx
#define getmaxyx         curses_getmaxyx
#define getparyx         curses_getparyx
#define wgetstr          curses_wgetstr
#define getnstr          curses_getnstr
#define wstandout        curses_wstandout
#define wstandend        curses_wstandend
#define wattron          curses_wattron
#define wattroff         curses_wattroff
#define scroll           curses_scroll
#define touchwin         curses_touchwin
#define touchline        curses_touchline
#define untouchwin       curses_untouchwin
#define box              curses_box
#define border           curses_border
#define hline            curses_hline
#define vline            curses_vline
#define winstr           curses_winstr
#define winchstr         curses_winchstr
#define winsstr          curses_winsstr
#define waddstr          curses_waddstr
#define waddchstr        curses_waddchstr
#define addch            curses_addch
#define addchnstr        curses_addchnstr
#define addchstr         curses_addchstr
#define addnstr          curses_addnstr
#define addstr           curses_addstr
#define attr_get         curses_attr_get
#define attr_off         curses_attr_off
#define attr_on          curses_attr_on
#define attr_set         curses_attr_set
#define attroff          curses_attroff
#define attron           curses_attron
#define attrset          curses_attrset
#define bkgd             curses_bkgd
#define bkgdset          curses_bkgdset
#define chgat            curses_chgat
#define clear            curses_clear
#define clrtobot         curses_clrtobot
#define clrtoeol         curses_clrtoeol
#define color_set        curses_color_set
#define delch            curses_delch
#define deleteln         curses_deleteln
#define echochar         curses_echochar
#define erase            curses_erase
#define getch            curses_getch
#define getstr           curses_getstr
#define inch             curses_inch
#define inchnstr         curses_inchnstr
#define inchstr          curses_inchstr
#define innstr           curses_innstr
#define insch            curses_insch
#define insdelln         curses_insdelln
#define insertln         curses_insertln
#define insnstr          curses_insnstr
#define insstr           curses_insstr
#define instr            curses_instr
#define move             curses_move
#define refresh          curses_refresh
#define scrl             curses_scrl
#define setscrreg        curses_setscrreg
#define standend         curses_standend
#define standout         curses_standout
#define timeout          curses_timeout
#define wdeleteln        curses_wdeleteln
#define winsertln        curses_winsertln
#define mvwaddch         curses_mvwaddch
#define mvwaddchnstr     curses_mvwaddchnstr
#define mvwaddchstr      curses_mvwaddchstr
#define mvwaddnstr       curses_mvwaddnstr
#define mvwaddstr        curses_mvwaddstr
#define mvwchgat         curses_mvwchgat
#define mvwdelch         curses_mvwdelch
#define mvwgetch         curses_mvwgetch
#define mvwgetnstr       curses_mvwgetnstr
#define mvwgetstr        curses_mvwgetstr
#define mvwhline         curses_mvwhline
#define mvwinch          curses_mvwinch
#define mvwinchnstr      curses_mvwinchnstr
#define mvwinchstr       curses_mvwinchstr
#define mvwinnstr        curses_mvwinnstr
#define mvwinsch         curses_mvwinsch
#define mvwinsnstr       curses_mvwinsnstr
#define mvwinsstr        curses_mvwinsstr
#define mvwinstr         curses_mvwinstr
#define mvwvline         curses_mvwvline
#define mvaddch          curses_mvaddch
#define mvaddchnstr      curses_mvaddchnstr
#define mvaddchstr       curses_mvaddchstr
#define mvaddnstr        curses_mvaddnstr
#define mvaddstr         curses_mvaddstr
#define mvchgat          curses_mvchgat
#define mvdelch          curses_mvdelch
#define mvgetch          curses_mvgetch
#define mvgetnstr        curses_mvgetnstr
#define mvgetstr         curses_mvgetstr
#define mvhline          curses_mvhline
#define mvinch           curses_mvinch
#define mvinchnstr       curses_mvinchnstr
#define mvinchstr        curses_mvinchstr
#define mvinnstr         curses_mvinnstr
#define mvinsch          curses_mvinsch
#define mvinsnstr        curses_mvinsnstr
#define mvinsstr         curses_mvinsstr
#define mvinstr          curses_mvinstr
#define mvvline          curses_mvvline
#define slk_attr_off     curses_slk_attr_off
#define slk_attr_on      curses_slk_attr_on
#define vw_printw        curses_vw_printw
#define vw_scanw         curses_vw_scanw
