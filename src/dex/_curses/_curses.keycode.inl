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


/*[[[deemon
#include <file>
#include <util>

const MAX_F_KEYS = 64;

private_keys = [];
for (local line: file.io("_curses.h","r")) {
  line = line.strip();
  if (line.startswith("#define CURSES_PUBLIC_KEY_")) {
    line = line[#"#define CURSES_PUBLIC_":];
    line = line.scanf("%[^ ]")...;
    if (line !in private_keys)
      private_keys.append(line);
  }
}
private_keys.sort();

print "DEE_A_RET_WUNUSED int DeeCurses_PrivateKeycodeToPublic(DEE_A_IN int key) {";
print " switch (key) {";
for (local name: private_keys) {
  print "#if defined("+name+") && "+name+" != CURSES_PUBLIC_"+name;
  print "  case "+name+": return CURSES_PUBLIC_"+name+";";
  print "#endif";
}
print "#if defined(KEY_F) && (KEY_F(0) != CURSES_PUBLIC_KEY_F(0) || KEY_F(7) != CURSES_PUBLIC_KEY_F(7))";
for (local x: util::range(1,MAX_F_KEYS)) {
  print "  case KEY_F("+x+"): return CURSES_PUBLIC_KEY_F("+x+");";
}
print "#endif";
print "  default: break;";
print " }";
print " return key;";
print "}";
print;
print "DEE_A_RET_WUNUSED int DeeCurses_PublicKeycodeToPrivate(DEE_A_IN int key) {";
print " switch (key) {";
for (local name: private_keys) {
  print "#if defined("+name+") && "+name+" != CURSES_PUBLIC_"+name;
  print "  case CURSES_PUBLIC_"+name+": return "+name+";";
  print "#endif";
}
print "#if defined(KEY_F) && (KEY_F(0) != CURSES_PUBLIC_KEY_F(0) || KEY_F(7) != CURSES_PUBLIC_KEY_F(7))";
for (local x: util::range(1,MAX_F_KEYS)) {
  print "  case CURSES_PUBLIC_KEY_F("+x+"): return KEY_F("+x+");";
}
print "#endif";
print "  default: break;";
print " }";
print " return key;";
print "}";
]]]*/
DEE_A_RET_WUNUSED int DeeCurses_PrivateKeycodeToPublic(DEE_A_IN int key) {
 switch (key) {
#if defined(KEY_A1) && KEY_A1 != CURSES_PUBLIC_KEY_A1
  case KEY_A1: return CURSES_PUBLIC_KEY_A1;
#endif
#if defined(KEY_A3) && KEY_A3 != CURSES_PUBLIC_KEY_A3
  case KEY_A3: return CURSES_PUBLIC_KEY_A3;
#endif
#if defined(KEY_B2) && KEY_B2 != CURSES_PUBLIC_KEY_B2
  case KEY_B2: return CURSES_PUBLIC_KEY_B2;
#endif
#if defined(KEY_BACKSPACE) && KEY_BACKSPACE != CURSES_PUBLIC_KEY_BACKSPACE
  case KEY_BACKSPACE: return CURSES_PUBLIC_KEY_BACKSPACE;
#endif
#if defined(KEY_BEG) && KEY_BEG != CURSES_PUBLIC_KEY_BEG
  case KEY_BEG: return CURSES_PUBLIC_KEY_BEG;
#endif
#if defined(KEY_BREAK) && KEY_BREAK != CURSES_PUBLIC_KEY_BREAK
  case KEY_BREAK: return CURSES_PUBLIC_KEY_BREAK;
#endif
#if defined(KEY_BTAB) && KEY_BTAB != CURSES_PUBLIC_KEY_BTAB
  case KEY_BTAB: return CURSES_PUBLIC_KEY_BTAB;
#endif
#if defined(KEY_C1) && KEY_C1 != CURSES_PUBLIC_KEY_C1
  case KEY_C1: return CURSES_PUBLIC_KEY_C1;
#endif
#if defined(KEY_C3) && KEY_C3 != CURSES_PUBLIC_KEY_C3
  case KEY_C3: return CURSES_PUBLIC_KEY_C3;
#endif
#if defined(KEY_CANCEL) && KEY_CANCEL != CURSES_PUBLIC_KEY_CANCEL
  case KEY_CANCEL: return CURSES_PUBLIC_KEY_CANCEL;
#endif
#if defined(KEY_CATAB) && KEY_CATAB != CURSES_PUBLIC_KEY_CATAB
  case KEY_CATAB: return CURSES_PUBLIC_KEY_CATAB;
#endif
#if defined(KEY_CLEAR) && KEY_CLEAR != CURSES_PUBLIC_KEY_CLEAR
  case KEY_CLEAR: return CURSES_PUBLIC_KEY_CLEAR;
#endif
#if defined(KEY_CLOSE) && KEY_CLOSE != CURSES_PUBLIC_KEY_CLOSE
  case KEY_CLOSE: return CURSES_PUBLIC_KEY_CLOSE;
#endif
#if defined(KEY_CODE_YES) && KEY_CODE_YES != CURSES_PUBLIC_KEY_CODE_YES
  case KEY_CODE_YES: return CURSES_PUBLIC_KEY_CODE_YES;
#endif
#if defined(KEY_COMMAND) && KEY_COMMAND != CURSES_PUBLIC_KEY_COMMAND
  case KEY_COMMAND: return CURSES_PUBLIC_KEY_COMMAND;
#endif
#if defined(KEY_COPY) && KEY_COPY != CURSES_PUBLIC_KEY_COPY
  case KEY_COPY: return CURSES_PUBLIC_KEY_COPY;
#endif
#if defined(KEY_CREATE) && KEY_CREATE != CURSES_PUBLIC_KEY_CREATE
  case KEY_CREATE: return CURSES_PUBLIC_KEY_CREATE;
#endif
#if defined(KEY_CTAB) && KEY_CTAB != CURSES_PUBLIC_KEY_CTAB
  case KEY_CTAB: return CURSES_PUBLIC_KEY_CTAB;
#endif
#if defined(KEY_DC) && KEY_DC != CURSES_PUBLIC_KEY_DC
  case KEY_DC: return CURSES_PUBLIC_KEY_DC;
#endif
#if defined(KEY_DL) && KEY_DL != CURSES_PUBLIC_KEY_DL
  case KEY_DL: return CURSES_PUBLIC_KEY_DL;
#endif
#if defined(KEY_DOWN) && KEY_DOWN != CURSES_PUBLIC_KEY_DOWN
  case KEY_DOWN: return CURSES_PUBLIC_KEY_DOWN;
#endif
#if defined(KEY_EIC) && KEY_EIC != CURSES_PUBLIC_KEY_EIC
  case KEY_EIC: return CURSES_PUBLIC_KEY_EIC;
#endif
#if defined(KEY_END) && KEY_END != CURSES_PUBLIC_KEY_END
  case KEY_END: return CURSES_PUBLIC_KEY_END;
#endif
#if defined(KEY_ENTER) && KEY_ENTER != CURSES_PUBLIC_KEY_ENTER
  case KEY_ENTER: return CURSES_PUBLIC_KEY_ENTER;
#endif
#if defined(KEY_EOL) && KEY_EOL != CURSES_PUBLIC_KEY_EOL
  case KEY_EOL: return CURSES_PUBLIC_KEY_EOL;
#endif
#if defined(KEY_EOS) && KEY_EOS != CURSES_PUBLIC_KEY_EOS
  case KEY_EOS: return CURSES_PUBLIC_KEY_EOS;
#endif
#if defined(KEY_EVENT) && KEY_EVENT != CURSES_PUBLIC_KEY_EVENT
  case KEY_EVENT: return CURSES_PUBLIC_KEY_EVENT;
#endif
#if defined(KEY_EXIT) && KEY_EXIT != CURSES_PUBLIC_KEY_EXIT
  case KEY_EXIT: return CURSES_PUBLIC_KEY_EXIT;
#endif
#if defined(KEY_F0) && KEY_F0 != CURSES_PUBLIC_KEY_F0
  case KEY_F0: return CURSES_PUBLIC_KEY_F0;
#endif
#if defined(KEY_FIND) && KEY_FIND != CURSES_PUBLIC_KEY_FIND
  case KEY_FIND: return CURSES_PUBLIC_KEY_FIND;
#endif
#if defined(KEY_HELP) && KEY_HELP != CURSES_PUBLIC_KEY_HELP
  case KEY_HELP: return CURSES_PUBLIC_KEY_HELP;
#endif
#if defined(KEY_HOME) && KEY_HOME != CURSES_PUBLIC_KEY_HOME
  case KEY_HOME: return CURSES_PUBLIC_KEY_HOME;
#endif
#if defined(KEY_IC) && KEY_IC != CURSES_PUBLIC_KEY_IC
  case KEY_IC: return CURSES_PUBLIC_KEY_IC;
#endif
#if defined(KEY_IL) && KEY_IL != CURSES_PUBLIC_KEY_IL
  case KEY_IL: return CURSES_PUBLIC_KEY_IL;
#endif
#if defined(KEY_LEFT) && KEY_LEFT != CURSES_PUBLIC_KEY_LEFT
  case KEY_LEFT: return CURSES_PUBLIC_KEY_LEFT;
#endif
#if defined(KEY_LL) && KEY_LL != CURSES_PUBLIC_KEY_LL
  case KEY_LL: return CURSES_PUBLIC_KEY_LL;
#endif
#if defined(KEY_MARK) && KEY_MARK != CURSES_PUBLIC_KEY_MARK
  case KEY_MARK: return CURSES_PUBLIC_KEY_MARK;
#endif
#if defined(KEY_MAX) && KEY_MAX != CURSES_PUBLIC_KEY_MAX
  case KEY_MAX: return CURSES_PUBLIC_KEY_MAX;
#endif
#if defined(KEY_MESSAGE) && KEY_MESSAGE != CURSES_PUBLIC_KEY_MESSAGE
  case KEY_MESSAGE: return CURSES_PUBLIC_KEY_MESSAGE;
#endif
#if defined(KEY_MIN) && KEY_MIN != CURSES_PUBLIC_KEY_MIN
  case KEY_MIN: return CURSES_PUBLIC_KEY_MIN;
#endif
#if defined(KEY_MOUSE) && KEY_MOUSE != CURSES_PUBLIC_KEY_MOUSE
  case KEY_MOUSE: return CURSES_PUBLIC_KEY_MOUSE;
#endif
#if defined(KEY_MOVE) && KEY_MOVE != CURSES_PUBLIC_KEY_MOVE
  case KEY_MOVE: return CURSES_PUBLIC_KEY_MOVE;
#endif
#if defined(KEY_NEXT) && KEY_NEXT != CURSES_PUBLIC_KEY_NEXT
  case KEY_NEXT: return CURSES_PUBLIC_KEY_NEXT;
#endif
#if defined(KEY_NPAGE) && KEY_NPAGE != CURSES_PUBLIC_KEY_NPAGE
  case KEY_NPAGE: return CURSES_PUBLIC_KEY_NPAGE;
#endif
#if defined(KEY_OPEN) && KEY_OPEN != CURSES_PUBLIC_KEY_OPEN
  case KEY_OPEN: return CURSES_PUBLIC_KEY_OPEN;
#endif
#if defined(KEY_OPTIONS) && KEY_OPTIONS != CURSES_PUBLIC_KEY_OPTIONS
  case KEY_OPTIONS: return CURSES_PUBLIC_KEY_OPTIONS;
#endif
#if defined(KEY_PPAGE) && KEY_PPAGE != CURSES_PUBLIC_KEY_PPAGE
  case KEY_PPAGE: return CURSES_PUBLIC_KEY_PPAGE;
#endif
#if defined(KEY_PREVIOUS) && KEY_PREVIOUS != CURSES_PUBLIC_KEY_PREVIOUS
  case KEY_PREVIOUS: return CURSES_PUBLIC_KEY_PREVIOUS;
#endif
#if defined(KEY_PRINT) && KEY_PRINT != CURSES_PUBLIC_KEY_PRINT
  case KEY_PRINT: return CURSES_PUBLIC_KEY_PRINT;
#endif
#if defined(KEY_REDO) && KEY_REDO != CURSES_PUBLIC_KEY_REDO
  case KEY_REDO: return CURSES_PUBLIC_KEY_REDO;
#endif
#if defined(KEY_REFERENCE) && KEY_REFERENCE != CURSES_PUBLIC_KEY_REFERENCE
  case KEY_REFERENCE: return CURSES_PUBLIC_KEY_REFERENCE;
#endif
#if defined(KEY_REFRESH) && KEY_REFRESH != CURSES_PUBLIC_KEY_REFRESH
  case KEY_REFRESH: return CURSES_PUBLIC_KEY_REFRESH;
#endif
#if defined(KEY_REPLACE) && KEY_REPLACE != CURSES_PUBLIC_KEY_REPLACE
  case KEY_REPLACE: return CURSES_PUBLIC_KEY_REPLACE;
#endif
#if defined(KEY_RESET) && KEY_RESET != CURSES_PUBLIC_KEY_RESET
  case KEY_RESET: return CURSES_PUBLIC_KEY_RESET;
#endif
#if defined(KEY_RESIZE) && KEY_RESIZE != CURSES_PUBLIC_KEY_RESIZE
  case KEY_RESIZE: return CURSES_PUBLIC_KEY_RESIZE;
#endif
#if defined(KEY_RESTART) && KEY_RESTART != CURSES_PUBLIC_KEY_RESTART
  case KEY_RESTART: return CURSES_PUBLIC_KEY_RESTART;
#endif
#if defined(KEY_RESUME) && KEY_RESUME != CURSES_PUBLIC_KEY_RESUME
  case KEY_RESUME: return CURSES_PUBLIC_KEY_RESUME;
#endif
#if defined(KEY_RIGHT) && KEY_RIGHT != CURSES_PUBLIC_KEY_RIGHT
  case KEY_RIGHT: return CURSES_PUBLIC_KEY_RIGHT;
#endif
#if defined(KEY_SAVE) && KEY_SAVE != CURSES_PUBLIC_KEY_SAVE
  case KEY_SAVE: return CURSES_PUBLIC_KEY_SAVE;
#endif
#if defined(KEY_SBEG) && KEY_SBEG != CURSES_PUBLIC_KEY_SBEG
  case KEY_SBEG: return CURSES_PUBLIC_KEY_SBEG;
#endif
#if defined(KEY_SCANCEL) && KEY_SCANCEL != CURSES_PUBLIC_KEY_SCANCEL
  case KEY_SCANCEL: return CURSES_PUBLIC_KEY_SCANCEL;
#endif
#if defined(KEY_SCOMMAND) && KEY_SCOMMAND != CURSES_PUBLIC_KEY_SCOMMAND
  case KEY_SCOMMAND: return CURSES_PUBLIC_KEY_SCOMMAND;
#endif
#if defined(KEY_SCOPY) && KEY_SCOPY != CURSES_PUBLIC_KEY_SCOPY
  case KEY_SCOPY: return CURSES_PUBLIC_KEY_SCOPY;
#endif
#if defined(KEY_SCREATE) && KEY_SCREATE != CURSES_PUBLIC_KEY_SCREATE
  case KEY_SCREATE: return CURSES_PUBLIC_KEY_SCREATE;
#endif
#if defined(KEY_SDC) && KEY_SDC != CURSES_PUBLIC_KEY_SDC
  case KEY_SDC: return CURSES_PUBLIC_KEY_SDC;
#endif
#if defined(KEY_SDL) && KEY_SDL != CURSES_PUBLIC_KEY_SDL
  case KEY_SDL: return CURSES_PUBLIC_KEY_SDL;
#endif
#if defined(KEY_SELECT) && KEY_SELECT != CURSES_PUBLIC_KEY_SELECT
  case KEY_SELECT: return CURSES_PUBLIC_KEY_SELECT;
#endif
#if defined(KEY_SEND) && KEY_SEND != CURSES_PUBLIC_KEY_SEND
  case KEY_SEND: return CURSES_PUBLIC_KEY_SEND;
#endif
#if defined(KEY_SEOL) && KEY_SEOL != CURSES_PUBLIC_KEY_SEOL
  case KEY_SEOL: return CURSES_PUBLIC_KEY_SEOL;
#endif
#if defined(KEY_SEXIT) && KEY_SEXIT != CURSES_PUBLIC_KEY_SEXIT
  case KEY_SEXIT: return CURSES_PUBLIC_KEY_SEXIT;
#endif
#if defined(KEY_SF) && KEY_SF != CURSES_PUBLIC_KEY_SF
  case KEY_SF: return CURSES_PUBLIC_KEY_SF;
#endif
#if defined(KEY_SFIND) && KEY_SFIND != CURSES_PUBLIC_KEY_SFIND
  case KEY_SFIND: return CURSES_PUBLIC_KEY_SFIND;
#endif
#if defined(KEY_SHELP) && KEY_SHELP != CURSES_PUBLIC_KEY_SHELP
  case KEY_SHELP: return CURSES_PUBLIC_KEY_SHELP;
#endif
#if defined(KEY_SHOME) && KEY_SHOME != CURSES_PUBLIC_KEY_SHOME
  case KEY_SHOME: return CURSES_PUBLIC_KEY_SHOME;
#endif
#if defined(KEY_SIC) && KEY_SIC != CURSES_PUBLIC_KEY_SIC
  case KEY_SIC: return CURSES_PUBLIC_KEY_SIC;
#endif
#if defined(KEY_SLEFT) && KEY_SLEFT != CURSES_PUBLIC_KEY_SLEFT
  case KEY_SLEFT: return CURSES_PUBLIC_KEY_SLEFT;
#endif
#if defined(KEY_SMESSAGE) && KEY_SMESSAGE != CURSES_PUBLIC_KEY_SMESSAGE
  case KEY_SMESSAGE: return CURSES_PUBLIC_KEY_SMESSAGE;
#endif
#if defined(KEY_SMOVE) && KEY_SMOVE != CURSES_PUBLIC_KEY_SMOVE
  case KEY_SMOVE: return CURSES_PUBLIC_KEY_SMOVE;
#endif
#if defined(KEY_SNEXT) && KEY_SNEXT != CURSES_PUBLIC_KEY_SNEXT
  case KEY_SNEXT: return CURSES_PUBLIC_KEY_SNEXT;
#endif
#if defined(KEY_SOPTIONS) && KEY_SOPTIONS != CURSES_PUBLIC_KEY_SOPTIONS
  case KEY_SOPTIONS: return CURSES_PUBLIC_KEY_SOPTIONS;
#endif
#if defined(KEY_SPREVIOUS) && KEY_SPREVIOUS != CURSES_PUBLIC_KEY_SPREVIOUS
  case KEY_SPREVIOUS: return CURSES_PUBLIC_KEY_SPREVIOUS;
#endif
#if defined(KEY_SPRINT) && KEY_SPRINT != CURSES_PUBLIC_KEY_SPRINT
  case KEY_SPRINT: return CURSES_PUBLIC_KEY_SPRINT;
#endif
#if defined(KEY_SR) && KEY_SR != CURSES_PUBLIC_KEY_SR
  case KEY_SR: return CURSES_PUBLIC_KEY_SR;
#endif
#if defined(KEY_SREDO) && KEY_SREDO != CURSES_PUBLIC_KEY_SREDO
  case KEY_SREDO: return CURSES_PUBLIC_KEY_SREDO;
#endif
#if defined(KEY_SREPLACE) && KEY_SREPLACE != CURSES_PUBLIC_KEY_SREPLACE
  case KEY_SREPLACE: return CURSES_PUBLIC_KEY_SREPLACE;
#endif
#if defined(KEY_SRESET) && KEY_SRESET != CURSES_PUBLIC_KEY_SRESET
  case KEY_SRESET: return CURSES_PUBLIC_KEY_SRESET;
#endif
#if defined(KEY_SRIGHT) && KEY_SRIGHT != CURSES_PUBLIC_KEY_SRIGHT
  case KEY_SRIGHT: return CURSES_PUBLIC_KEY_SRIGHT;
#endif
#if defined(KEY_SRSUME) && KEY_SRSUME != CURSES_PUBLIC_KEY_SRSUME
  case KEY_SRSUME: return CURSES_PUBLIC_KEY_SRSUME;
#endif
#if defined(KEY_SSAVE) && KEY_SSAVE != CURSES_PUBLIC_KEY_SSAVE
  case KEY_SSAVE: return CURSES_PUBLIC_KEY_SSAVE;
#endif
#if defined(KEY_SSUSPEND) && KEY_SSUSPEND != CURSES_PUBLIC_KEY_SSUSPEND
  case KEY_SSUSPEND: return CURSES_PUBLIC_KEY_SSUSPEND;
#endif
#if defined(KEY_STAB) && KEY_STAB != CURSES_PUBLIC_KEY_STAB
  case KEY_STAB: return CURSES_PUBLIC_KEY_STAB;
#endif
#if defined(KEY_SUNDO) && KEY_SUNDO != CURSES_PUBLIC_KEY_SUNDO
  case KEY_SUNDO: return CURSES_PUBLIC_KEY_SUNDO;
#endif
#if defined(KEY_SUSPEND) && KEY_SUSPEND != CURSES_PUBLIC_KEY_SUSPEND
  case KEY_SUSPEND: return CURSES_PUBLIC_KEY_SUSPEND;
#endif
#if defined(KEY_UNDO) && KEY_UNDO != CURSES_PUBLIC_KEY_UNDO
  case KEY_UNDO: return CURSES_PUBLIC_KEY_UNDO;
#endif
#if defined(KEY_UP) && KEY_UP != CURSES_PUBLIC_KEY_UP
  case KEY_UP: return CURSES_PUBLIC_KEY_UP;
#endif
#if defined(KEY_F) && (KEY_F(0) != CURSES_PUBLIC_KEY_F(0) || KEY_F(7) != CURSES_PUBLIC_KEY_F(7))
  case KEY_F(1): return CURSES_PUBLIC_KEY_F(1);
  case KEY_F(2): return CURSES_PUBLIC_KEY_F(2);
  case KEY_F(3): return CURSES_PUBLIC_KEY_F(3);
  case KEY_F(4): return CURSES_PUBLIC_KEY_F(4);
  case KEY_F(5): return CURSES_PUBLIC_KEY_F(5);
  case KEY_F(6): return CURSES_PUBLIC_KEY_F(6);
  case KEY_F(7): return CURSES_PUBLIC_KEY_F(7);
  case KEY_F(8): return CURSES_PUBLIC_KEY_F(8);
  case KEY_F(9): return CURSES_PUBLIC_KEY_F(9);
  case KEY_F(10): return CURSES_PUBLIC_KEY_F(10);
  case KEY_F(11): return CURSES_PUBLIC_KEY_F(11);
  case KEY_F(12): return CURSES_PUBLIC_KEY_F(12);
  case KEY_F(13): return CURSES_PUBLIC_KEY_F(13);
  case KEY_F(14): return CURSES_PUBLIC_KEY_F(14);
  case KEY_F(15): return CURSES_PUBLIC_KEY_F(15);
  case KEY_F(16): return CURSES_PUBLIC_KEY_F(16);
  case KEY_F(17): return CURSES_PUBLIC_KEY_F(17);
  case KEY_F(18): return CURSES_PUBLIC_KEY_F(18);
  case KEY_F(19): return CURSES_PUBLIC_KEY_F(19);
  case KEY_F(20): return CURSES_PUBLIC_KEY_F(20);
  case KEY_F(21): return CURSES_PUBLIC_KEY_F(21);
  case KEY_F(22): return CURSES_PUBLIC_KEY_F(22);
  case KEY_F(23): return CURSES_PUBLIC_KEY_F(23);
  case KEY_F(24): return CURSES_PUBLIC_KEY_F(24);
  case KEY_F(25): return CURSES_PUBLIC_KEY_F(25);
  case KEY_F(26): return CURSES_PUBLIC_KEY_F(26);
  case KEY_F(27): return CURSES_PUBLIC_KEY_F(27);
  case KEY_F(28): return CURSES_PUBLIC_KEY_F(28);
  case KEY_F(29): return CURSES_PUBLIC_KEY_F(29);
  case KEY_F(30): return CURSES_PUBLIC_KEY_F(30);
  case KEY_F(31): return CURSES_PUBLIC_KEY_F(31);
  case KEY_F(32): return CURSES_PUBLIC_KEY_F(32);
  case KEY_F(33): return CURSES_PUBLIC_KEY_F(33);
  case KEY_F(34): return CURSES_PUBLIC_KEY_F(34);
  case KEY_F(35): return CURSES_PUBLIC_KEY_F(35);
  case KEY_F(36): return CURSES_PUBLIC_KEY_F(36);
  case KEY_F(37): return CURSES_PUBLIC_KEY_F(37);
  case KEY_F(38): return CURSES_PUBLIC_KEY_F(38);
  case KEY_F(39): return CURSES_PUBLIC_KEY_F(39);
  case KEY_F(40): return CURSES_PUBLIC_KEY_F(40);
  case KEY_F(41): return CURSES_PUBLIC_KEY_F(41);
  case KEY_F(42): return CURSES_PUBLIC_KEY_F(42);
  case KEY_F(43): return CURSES_PUBLIC_KEY_F(43);
  case KEY_F(44): return CURSES_PUBLIC_KEY_F(44);
  case KEY_F(45): return CURSES_PUBLIC_KEY_F(45);
  case KEY_F(46): return CURSES_PUBLIC_KEY_F(46);
  case KEY_F(47): return CURSES_PUBLIC_KEY_F(47);
  case KEY_F(48): return CURSES_PUBLIC_KEY_F(48);
  case KEY_F(49): return CURSES_PUBLIC_KEY_F(49);
  case KEY_F(50): return CURSES_PUBLIC_KEY_F(50);
  case KEY_F(51): return CURSES_PUBLIC_KEY_F(51);
  case KEY_F(52): return CURSES_PUBLIC_KEY_F(52);
  case KEY_F(53): return CURSES_PUBLIC_KEY_F(53);
  case KEY_F(54): return CURSES_PUBLIC_KEY_F(54);
  case KEY_F(55): return CURSES_PUBLIC_KEY_F(55);
  case KEY_F(56): return CURSES_PUBLIC_KEY_F(56);
  case KEY_F(57): return CURSES_PUBLIC_KEY_F(57);
  case KEY_F(58): return CURSES_PUBLIC_KEY_F(58);
  case KEY_F(59): return CURSES_PUBLIC_KEY_F(59);
  case KEY_F(60): return CURSES_PUBLIC_KEY_F(60);
  case KEY_F(61): return CURSES_PUBLIC_KEY_F(61);
  case KEY_F(62): return CURSES_PUBLIC_KEY_F(62);
  case KEY_F(63): return CURSES_PUBLIC_KEY_F(63);
#endif
  default: break;
 }
 return key;
}
DEE_A_RET_WUNUSED int DeeCurses_PublicKeycodeToPrivate(DEE_A_IN int key) {
 switch (key) {
#if defined(KEY_A1) && KEY_A1 != CURSES_PUBLIC_KEY_A1
  case CURSES_PUBLIC_KEY_A1: return KEY_A1;
#endif
#if defined(KEY_A3) && KEY_A3 != CURSES_PUBLIC_KEY_A3
  case CURSES_PUBLIC_KEY_A3: return KEY_A3;
#endif
#if defined(KEY_B2) && KEY_B2 != CURSES_PUBLIC_KEY_B2
  case CURSES_PUBLIC_KEY_B2: return KEY_B2;
#endif
#if defined(KEY_BACKSPACE) && KEY_BACKSPACE != CURSES_PUBLIC_KEY_BACKSPACE
  case CURSES_PUBLIC_KEY_BACKSPACE: return KEY_BACKSPACE;
#endif
#if defined(KEY_BEG) && KEY_BEG != CURSES_PUBLIC_KEY_BEG
  case CURSES_PUBLIC_KEY_BEG: return KEY_BEG;
#endif
#if defined(KEY_BREAK) && KEY_BREAK != CURSES_PUBLIC_KEY_BREAK
  case CURSES_PUBLIC_KEY_BREAK: return KEY_BREAK;
#endif
#if defined(KEY_BTAB) && KEY_BTAB != CURSES_PUBLIC_KEY_BTAB
  case CURSES_PUBLIC_KEY_BTAB: return KEY_BTAB;
#endif
#if defined(KEY_C1) && KEY_C1 != CURSES_PUBLIC_KEY_C1
  case CURSES_PUBLIC_KEY_C1: return KEY_C1;
#endif
#if defined(KEY_C3) && KEY_C3 != CURSES_PUBLIC_KEY_C3
  case CURSES_PUBLIC_KEY_C3: return KEY_C3;
#endif
#if defined(KEY_CANCEL) && KEY_CANCEL != CURSES_PUBLIC_KEY_CANCEL
  case CURSES_PUBLIC_KEY_CANCEL: return KEY_CANCEL;
#endif
#if defined(KEY_CATAB) && KEY_CATAB != CURSES_PUBLIC_KEY_CATAB
  case CURSES_PUBLIC_KEY_CATAB: return KEY_CATAB;
#endif
#if defined(KEY_CLEAR) && KEY_CLEAR != CURSES_PUBLIC_KEY_CLEAR
  case CURSES_PUBLIC_KEY_CLEAR: return KEY_CLEAR;
#endif
#if defined(KEY_CLOSE) && KEY_CLOSE != CURSES_PUBLIC_KEY_CLOSE
  case CURSES_PUBLIC_KEY_CLOSE: return KEY_CLOSE;
#endif
#if defined(KEY_CODE_YES) && KEY_CODE_YES != CURSES_PUBLIC_KEY_CODE_YES
  case CURSES_PUBLIC_KEY_CODE_YES: return KEY_CODE_YES;
#endif
#if defined(KEY_COMMAND) && KEY_COMMAND != CURSES_PUBLIC_KEY_COMMAND
  case CURSES_PUBLIC_KEY_COMMAND: return KEY_COMMAND;
#endif
#if defined(KEY_COPY) && KEY_COPY != CURSES_PUBLIC_KEY_COPY
  case CURSES_PUBLIC_KEY_COPY: return KEY_COPY;
#endif
#if defined(KEY_CREATE) && KEY_CREATE != CURSES_PUBLIC_KEY_CREATE
  case CURSES_PUBLIC_KEY_CREATE: return KEY_CREATE;
#endif
#if defined(KEY_CTAB) && KEY_CTAB != CURSES_PUBLIC_KEY_CTAB
  case CURSES_PUBLIC_KEY_CTAB: return KEY_CTAB;
#endif
#if defined(KEY_DC) && KEY_DC != CURSES_PUBLIC_KEY_DC
  case CURSES_PUBLIC_KEY_DC: return KEY_DC;
#endif
#if defined(KEY_DL) && KEY_DL != CURSES_PUBLIC_KEY_DL
  case CURSES_PUBLIC_KEY_DL: return KEY_DL;
#endif
#if defined(KEY_DOWN) && KEY_DOWN != CURSES_PUBLIC_KEY_DOWN
  case CURSES_PUBLIC_KEY_DOWN: return KEY_DOWN;
#endif
#if defined(KEY_EIC) && KEY_EIC != CURSES_PUBLIC_KEY_EIC
  case CURSES_PUBLIC_KEY_EIC: return KEY_EIC;
#endif
#if defined(KEY_END) && KEY_END != CURSES_PUBLIC_KEY_END
  case CURSES_PUBLIC_KEY_END: return KEY_END;
#endif
#if defined(KEY_ENTER) && KEY_ENTER != CURSES_PUBLIC_KEY_ENTER
  case CURSES_PUBLIC_KEY_ENTER: return KEY_ENTER;
#endif
#if defined(KEY_EOL) && KEY_EOL != CURSES_PUBLIC_KEY_EOL
  case CURSES_PUBLIC_KEY_EOL: return KEY_EOL;
#endif
#if defined(KEY_EOS) && KEY_EOS != CURSES_PUBLIC_KEY_EOS
  case CURSES_PUBLIC_KEY_EOS: return KEY_EOS;
#endif
#if defined(KEY_EVENT) && KEY_EVENT != CURSES_PUBLIC_KEY_EVENT
  case CURSES_PUBLIC_KEY_EVENT: return KEY_EVENT;
#endif
#if defined(KEY_EXIT) && KEY_EXIT != CURSES_PUBLIC_KEY_EXIT
  case CURSES_PUBLIC_KEY_EXIT: return KEY_EXIT;
#endif
#if defined(KEY_F0) && KEY_F0 != CURSES_PUBLIC_KEY_F0
  case CURSES_PUBLIC_KEY_F0: return KEY_F0;
#endif
#if defined(KEY_FIND) && KEY_FIND != CURSES_PUBLIC_KEY_FIND
  case CURSES_PUBLIC_KEY_FIND: return KEY_FIND;
#endif
#if defined(KEY_HELP) && KEY_HELP != CURSES_PUBLIC_KEY_HELP
  case CURSES_PUBLIC_KEY_HELP: return KEY_HELP;
#endif
#if defined(KEY_HOME) && KEY_HOME != CURSES_PUBLIC_KEY_HOME
  case CURSES_PUBLIC_KEY_HOME: return KEY_HOME;
#endif
#if defined(KEY_IC) && KEY_IC != CURSES_PUBLIC_KEY_IC
  case CURSES_PUBLIC_KEY_IC: return KEY_IC;
#endif
#if defined(KEY_IL) && KEY_IL != CURSES_PUBLIC_KEY_IL
  case CURSES_PUBLIC_KEY_IL: return KEY_IL;
#endif
#if defined(KEY_LEFT) && KEY_LEFT != CURSES_PUBLIC_KEY_LEFT
  case CURSES_PUBLIC_KEY_LEFT: return KEY_LEFT;
#endif
#if defined(KEY_LL) && KEY_LL != CURSES_PUBLIC_KEY_LL
  case CURSES_PUBLIC_KEY_LL: return KEY_LL;
#endif
#if defined(KEY_MARK) && KEY_MARK != CURSES_PUBLIC_KEY_MARK
  case CURSES_PUBLIC_KEY_MARK: return KEY_MARK;
#endif
#if defined(KEY_MAX) && KEY_MAX != CURSES_PUBLIC_KEY_MAX
  case CURSES_PUBLIC_KEY_MAX: return KEY_MAX;
#endif
#if defined(KEY_MESSAGE) && KEY_MESSAGE != CURSES_PUBLIC_KEY_MESSAGE
  case CURSES_PUBLIC_KEY_MESSAGE: return KEY_MESSAGE;
#endif
#if defined(KEY_MIN) && KEY_MIN != CURSES_PUBLIC_KEY_MIN
  case CURSES_PUBLIC_KEY_MIN: return KEY_MIN;
#endif
#if defined(KEY_MOUSE) && KEY_MOUSE != CURSES_PUBLIC_KEY_MOUSE
  case CURSES_PUBLIC_KEY_MOUSE: return KEY_MOUSE;
#endif
#if defined(KEY_MOVE) && KEY_MOVE != CURSES_PUBLIC_KEY_MOVE
  case CURSES_PUBLIC_KEY_MOVE: return KEY_MOVE;
#endif
#if defined(KEY_NEXT) && KEY_NEXT != CURSES_PUBLIC_KEY_NEXT
  case CURSES_PUBLIC_KEY_NEXT: return KEY_NEXT;
#endif
#if defined(KEY_NPAGE) && KEY_NPAGE != CURSES_PUBLIC_KEY_NPAGE
  case CURSES_PUBLIC_KEY_NPAGE: return KEY_NPAGE;
#endif
#if defined(KEY_OPEN) && KEY_OPEN != CURSES_PUBLIC_KEY_OPEN
  case CURSES_PUBLIC_KEY_OPEN: return KEY_OPEN;
#endif
#if defined(KEY_OPTIONS) && KEY_OPTIONS != CURSES_PUBLIC_KEY_OPTIONS
  case CURSES_PUBLIC_KEY_OPTIONS: return KEY_OPTIONS;
#endif
#if defined(KEY_PPAGE) && KEY_PPAGE != CURSES_PUBLIC_KEY_PPAGE
  case CURSES_PUBLIC_KEY_PPAGE: return KEY_PPAGE;
#endif
#if defined(KEY_PREVIOUS) && KEY_PREVIOUS != CURSES_PUBLIC_KEY_PREVIOUS
  case CURSES_PUBLIC_KEY_PREVIOUS: return KEY_PREVIOUS;
#endif
#if defined(KEY_PRINT) && KEY_PRINT != CURSES_PUBLIC_KEY_PRINT
  case CURSES_PUBLIC_KEY_PRINT: return KEY_PRINT;
#endif
#if defined(KEY_REDO) && KEY_REDO != CURSES_PUBLIC_KEY_REDO
  case CURSES_PUBLIC_KEY_REDO: return KEY_REDO;
#endif
#if defined(KEY_REFERENCE) && KEY_REFERENCE != CURSES_PUBLIC_KEY_REFERENCE
  case CURSES_PUBLIC_KEY_REFERENCE: return KEY_REFERENCE;
#endif
#if defined(KEY_REFRESH) && KEY_REFRESH != CURSES_PUBLIC_KEY_REFRESH
  case CURSES_PUBLIC_KEY_REFRESH: return KEY_REFRESH;
#endif
#if defined(KEY_REPLACE) && KEY_REPLACE != CURSES_PUBLIC_KEY_REPLACE
  case CURSES_PUBLIC_KEY_REPLACE: return KEY_REPLACE;
#endif
#if defined(KEY_RESET) && KEY_RESET != CURSES_PUBLIC_KEY_RESET
  case CURSES_PUBLIC_KEY_RESET: return KEY_RESET;
#endif
#if defined(KEY_RESIZE) && KEY_RESIZE != CURSES_PUBLIC_KEY_RESIZE
  case CURSES_PUBLIC_KEY_RESIZE: return KEY_RESIZE;
#endif
#if defined(KEY_RESTART) && KEY_RESTART != CURSES_PUBLIC_KEY_RESTART
  case CURSES_PUBLIC_KEY_RESTART: return KEY_RESTART;
#endif
#if defined(KEY_RESUME) && KEY_RESUME != CURSES_PUBLIC_KEY_RESUME
  case CURSES_PUBLIC_KEY_RESUME: return KEY_RESUME;
#endif
#if defined(KEY_RIGHT) && KEY_RIGHT != CURSES_PUBLIC_KEY_RIGHT
  case CURSES_PUBLIC_KEY_RIGHT: return KEY_RIGHT;
#endif
#if defined(KEY_SAVE) && KEY_SAVE != CURSES_PUBLIC_KEY_SAVE
  case CURSES_PUBLIC_KEY_SAVE: return KEY_SAVE;
#endif
#if defined(KEY_SBEG) && KEY_SBEG != CURSES_PUBLIC_KEY_SBEG
  case CURSES_PUBLIC_KEY_SBEG: return KEY_SBEG;
#endif
#if defined(KEY_SCANCEL) && KEY_SCANCEL != CURSES_PUBLIC_KEY_SCANCEL
  case CURSES_PUBLIC_KEY_SCANCEL: return KEY_SCANCEL;
#endif
#if defined(KEY_SCOMMAND) && KEY_SCOMMAND != CURSES_PUBLIC_KEY_SCOMMAND
  case CURSES_PUBLIC_KEY_SCOMMAND: return KEY_SCOMMAND;
#endif
#if defined(KEY_SCOPY) && KEY_SCOPY != CURSES_PUBLIC_KEY_SCOPY
  case CURSES_PUBLIC_KEY_SCOPY: return KEY_SCOPY;
#endif
#if defined(KEY_SCREATE) && KEY_SCREATE != CURSES_PUBLIC_KEY_SCREATE
  case CURSES_PUBLIC_KEY_SCREATE: return KEY_SCREATE;
#endif
#if defined(KEY_SDC) && KEY_SDC != CURSES_PUBLIC_KEY_SDC
  case CURSES_PUBLIC_KEY_SDC: return KEY_SDC;
#endif
#if defined(KEY_SDL) && KEY_SDL != CURSES_PUBLIC_KEY_SDL
  case CURSES_PUBLIC_KEY_SDL: return KEY_SDL;
#endif
#if defined(KEY_SELECT) && KEY_SELECT != CURSES_PUBLIC_KEY_SELECT
  case CURSES_PUBLIC_KEY_SELECT: return KEY_SELECT;
#endif
#if defined(KEY_SEND) && KEY_SEND != CURSES_PUBLIC_KEY_SEND
  case CURSES_PUBLIC_KEY_SEND: return KEY_SEND;
#endif
#if defined(KEY_SEOL) && KEY_SEOL != CURSES_PUBLIC_KEY_SEOL
  case CURSES_PUBLIC_KEY_SEOL: return KEY_SEOL;
#endif
#if defined(KEY_SEXIT) && KEY_SEXIT != CURSES_PUBLIC_KEY_SEXIT
  case CURSES_PUBLIC_KEY_SEXIT: return KEY_SEXIT;
#endif
#if defined(KEY_SF) && KEY_SF != CURSES_PUBLIC_KEY_SF
  case CURSES_PUBLIC_KEY_SF: return KEY_SF;
#endif
#if defined(KEY_SFIND) && KEY_SFIND != CURSES_PUBLIC_KEY_SFIND
  case CURSES_PUBLIC_KEY_SFIND: return KEY_SFIND;
#endif
#if defined(KEY_SHELP) && KEY_SHELP != CURSES_PUBLIC_KEY_SHELP
  case CURSES_PUBLIC_KEY_SHELP: return KEY_SHELP;
#endif
#if defined(KEY_SHOME) && KEY_SHOME != CURSES_PUBLIC_KEY_SHOME
  case CURSES_PUBLIC_KEY_SHOME: return KEY_SHOME;
#endif
#if defined(KEY_SIC) && KEY_SIC != CURSES_PUBLIC_KEY_SIC
  case CURSES_PUBLIC_KEY_SIC: return KEY_SIC;
#endif
#if defined(KEY_SLEFT) && KEY_SLEFT != CURSES_PUBLIC_KEY_SLEFT
  case CURSES_PUBLIC_KEY_SLEFT: return KEY_SLEFT;
#endif
#if defined(KEY_SMESSAGE) && KEY_SMESSAGE != CURSES_PUBLIC_KEY_SMESSAGE
  case CURSES_PUBLIC_KEY_SMESSAGE: return KEY_SMESSAGE;
#endif
#if defined(KEY_SMOVE) && KEY_SMOVE != CURSES_PUBLIC_KEY_SMOVE
  case CURSES_PUBLIC_KEY_SMOVE: return KEY_SMOVE;
#endif
#if defined(KEY_SNEXT) && KEY_SNEXT != CURSES_PUBLIC_KEY_SNEXT
  case CURSES_PUBLIC_KEY_SNEXT: return KEY_SNEXT;
#endif
#if defined(KEY_SOPTIONS) && KEY_SOPTIONS != CURSES_PUBLIC_KEY_SOPTIONS
  case CURSES_PUBLIC_KEY_SOPTIONS: return KEY_SOPTIONS;
#endif
#if defined(KEY_SPREVIOUS) && KEY_SPREVIOUS != CURSES_PUBLIC_KEY_SPREVIOUS
  case CURSES_PUBLIC_KEY_SPREVIOUS: return KEY_SPREVIOUS;
#endif
#if defined(KEY_SPRINT) && KEY_SPRINT != CURSES_PUBLIC_KEY_SPRINT
  case CURSES_PUBLIC_KEY_SPRINT: return KEY_SPRINT;
#endif
#if defined(KEY_SR) && KEY_SR != CURSES_PUBLIC_KEY_SR
  case CURSES_PUBLIC_KEY_SR: return KEY_SR;
#endif
#if defined(KEY_SREDO) && KEY_SREDO != CURSES_PUBLIC_KEY_SREDO
  case CURSES_PUBLIC_KEY_SREDO: return KEY_SREDO;
#endif
#if defined(KEY_SREPLACE) && KEY_SREPLACE != CURSES_PUBLIC_KEY_SREPLACE
  case CURSES_PUBLIC_KEY_SREPLACE: return KEY_SREPLACE;
#endif
#if defined(KEY_SRESET) && KEY_SRESET != CURSES_PUBLIC_KEY_SRESET
  case CURSES_PUBLIC_KEY_SRESET: return KEY_SRESET;
#endif
#if defined(KEY_SRIGHT) && KEY_SRIGHT != CURSES_PUBLIC_KEY_SRIGHT
  case CURSES_PUBLIC_KEY_SRIGHT: return KEY_SRIGHT;
#endif
#if defined(KEY_SRSUME) && KEY_SRSUME != CURSES_PUBLIC_KEY_SRSUME
  case CURSES_PUBLIC_KEY_SRSUME: return KEY_SRSUME;
#endif
#if defined(KEY_SSAVE) && KEY_SSAVE != CURSES_PUBLIC_KEY_SSAVE
  case CURSES_PUBLIC_KEY_SSAVE: return KEY_SSAVE;
#endif
#if defined(KEY_SSUSPEND) && KEY_SSUSPEND != CURSES_PUBLIC_KEY_SSUSPEND
  case CURSES_PUBLIC_KEY_SSUSPEND: return KEY_SSUSPEND;
#endif
#if defined(KEY_STAB) && KEY_STAB != CURSES_PUBLIC_KEY_STAB
  case CURSES_PUBLIC_KEY_STAB: return KEY_STAB;
#endif
#if defined(KEY_SUNDO) && KEY_SUNDO != CURSES_PUBLIC_KEY_SUNDO
  case CURSES_PUBLIC_KEY_SUNDO: return KEY_SUNDO;
#endif
#if defined(KEY_SUSPEND) && KEY_SUSPEND != CURSES_PUBLIC_KEY_SUSPEND
  case CURSES_PUBLIC_KEY_SUSPEND: return KEY_SUSPEND;
#endif
#if defined(KEY_UNDO) && KEY_UNDO != CURSES_PUBLIC_KEY_UNDO
  case CURSES_PUBLIC_KEY_UNDO: return KEY_UNDO;
#endif
#if defined(KEY_UP) && KEY_UP != CURSES_PUBLIC_KEY_UP
  case CURSES_PUBLIC_KEY_UP: return KEY_UP;
#endif
#if defined(KEY_F) && (KEY_F(0) != CURSES_PUBLIC_KEY_F(0) || KEY_F(7) != CURSES_PUBLIC_KEY_F(7))
  case CURSES_PUBLIC_KEY_F(1): return KEY_F(1);
  case CURSES_PUBLIC_KEY_F(2): return KEY_F(2);
  case CURSES_PUBLIC_KEY_F(3): return KEY_F(3);
  case CURSES_PUBLIC_KEY_F(4): return KEY_F(4);
  case CURSES_PUBLIC_KEY_F(5): return KEY_F(5);
  case CURSES_PUBLIC_KEY_F(6): return KEY_F(6);
  case CURSES_PUBLIC_KEY_F(7): return KEY_F(7);
  case CURSES_PUBLIC_KEY_F(8): return KEY_F(8);
  case CURSES_PUBLIC_KEY_F(9): return KEY_F(9);
  case CURSES_PUBLIC_KEY_F(10): return KEY_F(10);
  case CURSES_PUBLIC_KEY_F(11): return KEY_F(11);
  case CURSES_PUBLIC_KEY_F(12): return KEY_F(12);
  case CURSES_PUBLIC_KEY_F(13): return KEY_F(13);
  case CURSES_PUBLIC_KEY_F(14): return KEY_F(14);
  case CURSES_PUBLIC_KEY_F(15): return KEY_F(15);
  case CURSES_PUBLIC_KEY_F(16): return KEY_F(16);
  case CURSES_PUBLIC_KEY_F(17): return KEY_F(17);
  case CURSES_PUBLIC_KEY_F(18): return KEY_F(18);
  case CURSES_PUBLIC_KEY_F(19): return KEY_F(19);
  case CURSES_PUBLIC_KEY_F(20): return KEY_F(20);
  case CURSES_PUBLIC_KEY_F(21): return KEY_F(21);
  case CURSES_PUBLIC_KEY_F(22): return KEY_F(22);
  case CURSES_PUBLIC_KEY_F(23): return KEY_F(23);
  case CURSES_PUBLIC_KEY_F(24): return KEY_F(24);
  case CURSES_PUBLIC_KEY_F(25): return KEY_F(25);
  case CURSES_PUBLIC_KEY_F(26): return KEY_F(26);
  case CURSES_PUBLIC_KEY_F(27): return KEY_F(27);
  case CURSES_PUBLIC_KEY_F(28): return KEY_F(28);
  case CURSES_PUBLIC_KEY_F(29): return KEY_F(29);
  case CURSES_PUBLIC_KEY_F(30): return KEY_F(30);
  case CURSES_PUBLIC_KEY_F(31): return KEY_F(31);
  case CURSES_PUBLIC_KEY_F(32): return KEY_F(32);
  case CURSES_PUBLIC_KEY_F(33): return KEY_F(33);
  case CURSES_PUBLIC_KEY_F(34): return KEY_F(34);
  case CURSES_PUBLIC_KEY_F(35): return KEY_F(35);
  case CURSES_PUBLIC_KEY_F(36): return KEY_F(36);
  case CURSES_PUBLIC_KEY_F(37): return KEY_F(37);
  case CURSES_PUBLIC_KEY_F(38): return KEY_F(38);
  case CURSES_PUBLIC_KEY_F(39): return KEY_F(39);
  case CURSES_PUBLIC_KEY_F(40): return KEY_F(40);
  case CURSES_PUBLIC_KEY_F(41): return KEY_F(41);
  case CURSES_PUBLIC_KEY_F(42): return KEY_F(42);
  case CURSES_PUBLIC_KEY_F(43): return KEY_F(43);
  case CURSES_PUBLIC_KEY_F(44): return KEY_F(44);
  case CURSES_PUBLIC_KEY_F(45): return KEY_F(45);
  case CURSES_PUBLIC_KEY_F(46): return KEY_F(46);
  case CURSES_PUBLIC_KEY_F(47): return KEY_F(47);
  case CURSES_PUBLIC_KEY_F(48): return KEY_F(48);
  case CURSES_PUBLIC_KEY_F(49): return KEY_F(49);
  case CURSES_PUBLIC_KEY_F(50): return KEY_F(50);
  case CURSES_PUBLIC_KEY_F(51): return KEY_F(51);
  case CURSES_PUBLIC_KEY_F(52): return KEY_F(52);
  case CURSES_PUBLIC_KEY_F(53): return KEY_F(53);
  case CURSES_PUBLIC_KEY_F(54): return KEY_F(54);
  case CURSES_PUBLIC_KEY_F(55): return KEY_F(55);
  case CURSES_PUBLIC_KEY_F(56): return KEY_F(56);
  case CURSES_PUBLIC_KEY_F(57): return KEY_F(57);
  case CURSES_PUBLIC_KEY_F(58): return KEY_F(58);
  case CURSES_PUBLIC_KEY_F(59): return KEY_F(59);
  case CURSES_PUBLIC_KEY_F(60): return KEY_F(60);
  case CURSES_PUBLIC_KEY_F(61): return KEY_F(61);
  case CURSES_PUBLIC_KEY_F(62): return KEY_F(62);
  case CURSES_PUBLIC_KEY_F(63): return KEY_F(63);
#endif
  default: break;
 }
 return key;
}
//[[[end]]]


#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
