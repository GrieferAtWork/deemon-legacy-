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
#ifndef GUARD_DEEMON_DEX_GFX_WINDOW_SCANBOARD_H
#define GUARD_DEEMON_DEX_GFX_WINDOW_SCANBOARD_H 1
 
#include <deemon/__conf.inl>

#ifdef DEE_PLATFORM_WINDOWS
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#endif

DEE_DECL_BEGIN

#ifdef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#undef DEE_PRIVATE_DECL_DEE_INTEGRAL_TYPES
#endif


typedef Dee_uint8_t  Dee_scancode_t;
typedef Dee_uint16_t Dee_keycode_t;

//////////////////////////////////////////////////////////////////////////
// Scancode values are based on USB standard keycodes
// >> http://www.freebsddiary.org/APC/usb_hid_usages.php

#define DEE_SCANCODE_NONE       DEE_UINT8_C(0x00)
#define DEE_SCANCODE_A	         DEE_UINT8_C(0x04)	/*< Keyboard a and A. */
#define DEE_SCANCODE_B	         DEE_UINT8_C(0x05)	/*< Keyboard b and B. */
#define DEE_SCANCODE_C	         DEE_UINT8_C(0x06)	/*< Keyboard c and C. */
#define DEE_SCANCODE_D	         DEE_UINT8_C(0x07)	/*< Keyboard d and D. */
#define DEE_SCANCODE_E	         DEE_UINT8_C(0x08)	/*< Keyboard e and E. */
#define DEE_SCANCODE_F	         DEE_UINT8_C(0x09)	/*< Keyboard f and F. */
#define DEE_SCANCODE_G	         DEE_UINT8_C(0x0A)	/*< Keyboard g and G. */
#define DEE_SCANCODE_H	         DEE_UINT8_C(0x0B)	/*< Keyboard h and H. */
#define DEE_SCANCODE_I	         DEE_UINT8_C(0x0C)	/*< Keyboard i and I. */
#define DEE_SCANCODE_J	         DEE_UINT8_C(0x0D)	/*< Keyboard j and J. */
#define DEE_SCANCODE_K	         DEE_UINT8_C(0x0E)	/*< Keyboard k and K. */
#define DEE_SCANCODE_L	         DEE_UINT8_C(0x0F)	/*< Keyboard l and L. */
#define DEE_SCANCODE_M	         DEE_UINT8_C(0x10)	/*< Keyboard m and M. */
#define DEE_SCANCODE_N	         DEE_UINT8_C(0x11)	/*< Keyboard n and N. */
#define DEE_SCANCODE_O	         DEE_UINT8_C(0x12)	/*< Keyboard o and O. */
#define DEE_SCANCODE_P	         DEE_UINT8_C(0x13)	/*< Keyboard p and P. */
#define DEE_SCANCODE_Q	         DEE_UINT8_C(0x14)	/*< Keyboard q and Q. */
#define DEE_SCANCODE_R	         DEE_UINT8_C(0x15)	/*< Keyboard r and R. */
#define DEE_SCANCODE_S	         DEE_UINT8_C(0x16)	/*< Keyboard s and S. */
#define DEE_SCANCODE_T	         DEE_UINT8_C(0x17)	/*< Keyboard t and T. */
#define DEE_SCANCODE_U	         DEE_UINT8_C(0x18)	/*< Keyboard u and U. */
#define DEE_SCANCODE_V	         DEE_UINT8_C(0x19)	/*< Keyboard v and V. */
#define DEE_SCANCODE_W	         DEE_UINT8_C(0x1A)	/*< Keyboard w and W. */
#define DEE_SCANCODE_X	         DEE_UINT8_C(0x1B)	/*< Keyboard x and X. */
#define DEE_SCANCODE_Y	         DEE_UINT8_C(0x1C)	/*< Keyboard y and Y. */
#define DEE_SCANCODE_Z	         DEE_UINT8_C(0x1D)	/*< Keyboard z and Z. */
#define DEE_SCANCODE_1	         DEE_UINT8_C(0x1E)	/*< Keyboard 1 and !. */
#define DEE_SCANCODE_2	         DEE_UINT8_C(0x1F)	/*< Keyboard 2 and @. */
#define DEE_SCANCODE_3	         DEE_UINT8_C(0x20)	/*< Keyboard 3 and #. */
#define DEE_SCANCODE_4	         DEE_UINT8_C(0x21)	/*< Keyboard 4 and $. */
#define DEE_SCANCODE_5	         DEE_UINT8_C(0x22)	/*< Keyboard 5 and %. */
#define DEE_SCANCODE_6	         DEE_UINT8_C(0x23)	/*< Keyboard 6 and ^. */
#define DEE_SCANCODE_7	         DEE_UINT8_C(0x24)	/*< Keyboard 7 and &. */
#define DEE_SCANCODE_8	         DEE_UINT8_C(0x25)	/*< Keyboard 8 and *. */
#define DEE_SCANCODE_9	         DEE_UINT8_C(0x26)	/*< Keyboard 9 and (. */
#define DEE_SCANCODE_0	         DEE_UINT8_C(0x27)	/*< Keyboard 0 and ). */
#define DEE_SCANCODE_ENTER	     DEE_UINT8_C(0x28)	/*< Keyboard Return (ENTER). */
#define DEE_SCANCODE_ESCAPE	    DEE_UINT8_C(0x29)	/*< Keyboard ESCAPE. */
#define DEE_SCANCODE_BACKSPACE	 DEE_UINT8_C(0x2A)	/*< Keyboard DELETE (Backspace). */
#define DEE_SCANCODE_TAB	       DEE_UINT8_C(0x2B)	/*< Keyboard Tab. */
#define DEE_SCANCODE_SPACE	     DEE_UINT8_C(0x2C)	/*< Keyboard Spacebar. */
#define DEE_SCANCODE_MINUS	     DEE_UINT8_C(0x2D)	/*< Keyboard - and (underscore). */
#define DEE_SCANCODE_EQUALS	    DEE_UINT8_C(0x2E)	/*< Keyboard = and +. */
#define DEE_SCANCODE_LBRACKET	  DEE_UINT8_C(0x2F)	/*< Keyboard [ and {. */
#define DEE_SCANCODE_RBRACKET	  DEE_UINT8_C(0x30)	/*< Keyboard ] and }. */
#define DEE_SCANCODE_BACKSLASH	 DEE_UINT8_C(0x31)	/*< Keyboard \ and |. */
#define DEE_SCANCODE_HASH	      DEE_UINT8_C(0x32)	/*< Keyboard Non-US # and ~. */
#define DEE_SCANCODE_SEMICOLON	 DEE_UINT8_C(0x33)	/*< Keyboard ; and :. */
#define DEE_SCANCODE_APOSTROHPE	DEE_UINT8_C(0x34)	/*< Keyboard ' and ". */
#define DEE_SCANCODE_GRAVE	     DEE_UINT8_C(0x35)	/*< Keyboard Grave Accent and Tilde. */
#define DEE_SCANCODE_COMMA	     DEE_UINT8_C(0x36)	/*< Keyboard, and <. */
#define DEE_SCANCODE_PERIOD	    DEE_UINT8_C(0x37)	/*< Keyboard . and >. */
#define DEE_SCANCODE_SLASH	     DEE_UINT8_C(0x38)	/*< Keyboard / and ?. */
#define DEE_SCANCODE_CAPSLOCK	  DEE_UINT8_C(0x39)	/*< Keyboard Caps Lock. */
#define DEE_SCANCODE_F1        	DEE_UINT8_C(0x3A)	/*< Keyboard F1. */
#define DEE_SCANCODE_F2        	DEE_UINT8_C(0x3B)	/*< Keyboard F2. */
#define DEE_SCANCODE_F3        	DEE_UINT8_C(0x3C)	/*< Keyboard F3. */
#define DEE_SCANCODE_F4        	DEE_UINT8_C(0x3D)	/*< Keyboard F4. */
#define DEE_SCANCODE_F5        	DEE_UINT8_C(0x3E)	/*< Keyboard F5. */
#define DEE_SCANCODE_F6        	DEE_UINT8_C(0x3F)	/*< Keyboard F6. */
#define DEE_SCANCODE_F7        	DEE_UINT8_C(0x40)	/*< Keyboard F7. */
#define DEE_SCANCODE_F8        	DEE_UINT8_C(0x41)	/*< Keyboard F8. */
#define DEE_SCANCODE_F9        	DEE_UINT8_C(0x42)	/*< Keyboard F9. */
#define DEE_SCANCODE_F10       	DEE_UINT8_C(0x43)	/*< Keyboard F10. */
#define DEE_SCANCODE_F11       	DEE_UINT8_C(0x44)	/*< Keyboard F11. */
#define DEE_SCANCODE_F12       	DEE_UINT8_C(0x45)	/*< Keyboard F12. */
#define DEE_SCANCODE_PRINT	     DEE_UINT8_C(0x46)	/*< Keyboard PrintScreen. */
#define DEE_SCANCODE_SCROLLLOCK	DEE_UINT8_C(0x47)	/*< Keyboard Scroll Lock. */
#define DEE_SCANCODE_PAUSE	     DEE_UINT8_C(0x48)	/*< Keyboard Pause. */
#define DEE_SCANCODE_INSERT	    DEE_UINT8_C(0x49)	/*< Keyboard Insert. */
#define DEE_SCANCODE_HOME	      DEE_UINT8_C(0x4A)	/*< Keyboard Home. */
#define DEE_SCANCODE_PAGEUP	    DEE_UINT8_C(0x4B)	/*< Keyboard PageUp. */
#define DEE_SCANCODE_DELETE	    DEE_UINT8_C(0x4C)	/*< Keyboard Delete Forward. */
#define DEE_SCANCODE_END	       DEE_UINT8_C(0x4D)	/*< Keyboard End. */
#define DEE_SCANCODE_PAGEDOWN	  DEE_UINT8_C(0x4E)	/*< Keyboard PageDown. */
#define DEE_SCANCODE_RIGHT	     DEE_UINT8_C(0x4F)	/*< Keyboard RightArrow. */
#define DEE_SCANCODE_LEFT	      DEE_UINT8_C(0x50)	/*< Keyboard LeftArrow. */
#define DEE_SCANCODE_DOWN	      DEE_UINT8_C(0x51)	/*< Keyboard DownArrow. */
#define DEE_SCANCODE_UP	        DEE_UINT8_C(0x52)	/*< Keyboard UpArrow. */
#define DEE_SCANCODE_NUMLOCK	   DEE_UINT8_C(0x53)	/*< Keypad Num Lock and Clear. */
#define DEE_SCANCODE_KP_DIV	    DEE_UINT8_C(0x54)	/*< Keypad /. */
#define DEE_SCANCODE_KP_MUL	    DEE_UINT8_C(0x55)	/*< Keypad *. */
#define DEE_SCANCODE_KP_SUB	    DEE_UINT8_C(0x56)	/*< Keypad -. */
#define DEE_SCANCODE_KP_ADD	    DEE_UINT8_C(0x57)	/*< Keypad +. */
#define DEE_SCANCODE_KP_ENTER	  DEE_UINT8_C(0x58)	/*< Keypad ENTER. */
#define DEE_SCANCODE_KP_1	      DEE_UINT8_C(0x59)	/*< Keypad 1 and End. */
#define DEE_SCANCODE_KP_2	      DEE_UINT8_C(0x5A)	/*< Keypad 2 and Down Arrow. */
#define DEE_SCANCODE_KP_3	      DEE_UINT8_C(0x5B)	/*< Keypad 3 and PageDn. */
#define DEE_SCANCODE_KP_4	      DEE_UINT8_C(0x5C)	/*< Keypad 4 and Left Arrow. */
#define DEE_SCANCODE_KP_5	      DEE_UINT8_C(0x5D)	/*< Keypad 5. */
#define DEE_SCANCODE_KP_6	      DEE_UINT8_C(0x5E)	/*< Keypad 6 and Right Arrow. */
#define DEE_SCANCODE_KP_7	      DEE_UINT8_C(0x5F)	/*< Keypad 7 and Home. */
#define DEE_SCANCODE_KP_8	      DEE_UINT8_C(0x60)	/*< Keypad 8 and Up Arrow. */
#define DEE_SCANCODE_KP_9	      DEE_UINT8_C(0x61)	/*< Keypad 9 and PageUp. */
#define DEE_SCANCODE_KP_0	      DEE_UINT8_C(0x62)	/*< Keypad 0 and Insert. */
#define DEE_SCANCODE_KP_PERIOD	 DEE_UINT8_C(0x63)	/*< Keypad . and Delete. */
#define DEE_SCANCODE_NONUSBACKSLASH	DEE_UINT8_C(0x64)	/*< Keyboard Non-US \ and |. */
#define DEE_SCANCODE_APP	       DEE_UINT8_C(0x65)	/*< Keyboard Application. */
#define DEE_SCANCODE_POWER	     DEE_UINT8_C(0x66)	/*< Keyboard Power. */
#define DEE_SCANCODE_KP_EQUALS	 DEE_UINT8_C(0x67)	/*< Keypad =. */
#define DEE_SCANCODE_F13	       DEE_UINT8_C(0x68)	/*< Keyboard F13. */
#define DEE_SCANCODE_F14	       DEE_UINT8_C(0x69)	/*< Keyboard F14. */
#define DEE_SCANCODE_F15	       DEE_UINT8_C(0x6A)	/*< Keyboard F15. */
#define DEE_SCANCODE_F16	       DEE_UINT8_C(0x6B)	/*< Keyboard F16. */
#define DEE_SCANCODE_F17	       DEE_UINT8_C(0x6C)	/*< Keyboard F17. */
#define DEE_SCANCODE_F18	       DEE_UINT8_C(0x6D)	/*< Keyboard F18. */
#define DEE_SCANCODE_F19	       DEE_UINT8_C(0x6E)	/*< Keyboard F19. */
#define DEE_SCANCODE_F20	       DEE_UINT8_C(0x6F)	/*< Keyboard F20. */
#define DEE_SCANCODE_F21	       DEE_UINT8_C(0x70)	/*< Keyboard F21. */
#define DEE_SCANCODE_F22	       DEE_UINT8_C(0x71)	/*< Keyboard F22. */
#define DEE_SCANCODE_F23	       DEE_UINT8_C(0x72)	/*< Keyboard F23. */
#define DEE_SCANCODE_F24	       DEE_UINT8_C(0x73)	/*< Keyboard F24. */
#define DEE_SCANCODE_EXECUTE	   DEE_UINT8_C(0x74)	/*< Keyboard Execute. */
#define DEE_SCANCODE_HELP	      DEE_UINT8_C(0x75)	/*< Keyboard Help. */
#define DEE_SCANCODE_MENU	      DEE_UINT8_C(0x76)	/*< Keyboard Menu. */
#define DEE_SCANCODE_SELECT	    DEE_UINT8_C(0x77)	/*< Keyboard Select. */
#define DEE_SCANCODE_STOP	      DEE_UINT8_C(0x78)	/*< Keyboard Stop. */
#define DEE_SCANCODE_AGAIN	     DEE_UINT8_C(0x79)	/*< Keyboard Again. */
#define DEE_SCANCODE_UNDO	      DEE_UINT8_C(0x7A)	/*< Keyboard Undo. */
#define DEE_SCANCODE_CUT	       DEE_UINT8_C(0x7B)	/*< Keyboard Cut. */
#define DEE_SCANCODE_COPY	      DEE_UINT8_C(0x7C)	/*< Keyboard Copy. */
#define DEE_SCANCODE_PASTE	     DEE_UINT8_C(0x7D)	/*< Keyboard Paste. */
#define DEE_SCANCODE_FIND	      DEE_UINT8_C(0x7E)	/*< Keyboard Find. */
#define DEE_SCANCODE_MUTE	      DEE_UINT8_C(0x7F)	/*< Keyboard Mute. */
#define DEE_SCANCODE_VOLUP	     DEE_UINT8_C(0x80)	/*< Keyboard Volume Up. */
#define DEE_SCANCODE_VOLDOWN	   DEE_UINT8_C(0x81)	/*< Keyboard Volume Down. */
#define DEE_SCANCODE_LOCKINGCAPSLOCK	DEE_UINT8_C(0x82)	/*< Keyboard Locking Caps Lock. */
#define DEE_SCANCODE_LOCKINGNUMLOCK	DEE_UINT8_C(0x83)	/*< Keyboard Locking Num Lock. */
#define DEE_SCANCODE_LOCKINGSCROLLLOCK	DEE_UINT8_C(0x84)	/*< Keyboard Locking Scroll Lock. */
#define DEE_SCANCODE_KP_COMMA	  DEE_UINT8_C(0x85)	/*< Keypad Comma. */
#define DEE_SCANCODE_KP_EQUAL	  DEE_UINT8_C(0x86)	/*< Keypad Equal Sign. */
#define DEE_SCANCODE_INATIONAL1	DEE_UINT8_C(0x87)	/*< Keyboard International1. */
#define DEE_SCANCODE_INATIONAL2	DEE_UINT8_C(0x88)	/*< Keyboard International2. */
#define DEE_SCANCODE_INATIONAL3	DEE_UINT8_C(0x89)	/*< Keyboard International3. */
#define DEE_SCANCODE_INATIONAL4	DEE_UINT8_C(0x8A)	/*< Keyboard International4. */
#define DEE_SCANCODE_INATIONAL5	DEE_UINT8_C(0x8B)	/*< Keyboard International5. */
#define DEE_SCANCODE_INATIONAL6	DEE_UINT8_C(0x8C)	/*< Keyboard International6. */
#define DEE_SCANCODE_INATIONAL7	DEE_UINT8_C(0x8D)	/*< Keyboard International7. */
#define DEE_SCANCODE_INATIONAL8	DEE_UINT8_C(0x8E)	/*< Keyboard International8. */
#define DEE_SCANCODE_INATIONAL9	DEE_UINT8_C(0x8F)	/*< Keyboard International9. */
#define DEE_SCANCODE_LANG1	     DEE_UINT8_C(0x90)	/*< Keyboard LANG1. */
#define DEE_SCANCODE_LANG2	     DEE_UINT8_C(0x91)	/*< Keyboard LANG2. */
#define DEE_SCANCODE_LANG3	     DEE_UINT8_C(0x92)	/*< Keyboard LANG3. */
#define DEE_SCANCODE_LANG4	     DEE_UINT8_C(0x93)	/*< Keyboard LANG4. */
#define DEE_SCANCODE_LANG5	     DEE_UINT8_C(0x94)	/*< Keyboard LANG5. */
#define DEE_SCANCODE_LANG6	     DEE_UINT8_C(0x95)	/*< Keyboard LANG6. */
#define DEE_SCANCODE_LANG7	     DEE_UINT8_C(0x96)	/*< Keyboard LANG7. */
#define DEE_SCANCODE_LANG8	     DEE_UINT8_C(0x97)	/*< Keyboard LANG8. */
#define DEE_SCANCODE_LANG9	     DEE_UINT8_C(0x98)	/*< Keyboard LANG9. */
#define DEE_SCANCODE_ALTERASE	  DEE_UINT8_C(0x99)	/*< Keyboard Alternate Erase. */
#define DEE_SCANCODE_SYSREQ	    DEE_UINT8_C(0x9A)	/*< Keyboard SysReq/Attention. */
#define DEE_SCANCODE_CANCEL	    DEE_UINT8_C(0x9B)	/*< Keyboard Cancel. */
#define DEE_SCANCODE_CLEAR	     DEE_UINT8_C(0x9C)	/*< Keyboard Clear. */
#define DEE_SCANCODE_PRIOR	     DEE_UINT8_C(0x9D)	/*< Keyboard Prior. */
#define DEE_SCANCODE_RETURN	    DEE_UINT8_C(0x9E)	/*< Keyboard Return. */
#define DEE_SCANCODE_SEPARATOR	 DEE_UINT8_C(0x9F)	/*< Keyboard Separator. */
#define DEE_SCANCODE_OUT	       DEE_UINT8_C(0xA0)	/*< Keyboard Out. */
#define DEE_SCANCODE_OPER	      DEE_UINT8_C(0xA1)	/*< Keyboard Oper. */
#define DEE_SCANCODE_CLEARAGAIN	DEE_UINT8_C(0xA2)	/*< Keyboard Clear/Again. */
#define DEE_SCANCODE_CRSEL	     DEE_UINT8_C(0xA3)	/*< Keyboard CrSel/Props. */
#define DEE_SCANCODE_EXSEL	     DEE_UINT8_C(0xA4)	/*< Keyboard ExSel. */
#define DEE_SCANCODE_LCTRL	     DEE_UINT8_C(0xE0)	/*< Keyboard LeftControl. */
#define DEE_SCANCODE_LSHIFT	    DEE_UINT8_C(0xE1)	/*< Keyboard LeftShift. */
#define DEE_SCANCODE_LALT	      DEE_UINT8_C(0xE2)	/*< Keyboard LeftAlt. */
#define DEE_SCANCODE_LGUI	      DEE_UINT8_C(0xE3)	/*< Keyboard Left GUI. */
#define DEE_SCANCODE_RCTRL	     DEE_UINT8_C(0xE4)	/*< Keyboard RightControl. */
#define DEE_SCANCODE_RSHIFT	    DEE_UINT8_C(0xE5)	/*< Keyboard RightShift. */
#define DEE_SCANCODE_RALT	      DEE_UINT8_C(0xE6)	/*< Keyboard RightAlt. */
#define DEE_SCANCODE_RGUI	      DEE_UINT8_C(0xE7)	/*< Keyboard Right GUI. */


//////////////////////////////////////////////////////////////////////////
// Keycode values (keyboard independent codes)
// e.g.: DEE_KEYCODE_Y is always 'y', solving quirks like z-y swaps
#define DEE_KEYCODE_NONE       0x00
#define DEE_KEYCODE_RETURN     '\n'
#define DEE_KEYCODE_ESCAPE     '\x1B' // '\e'
#define DEE_KEYCODE_BACKSPACE  '\b'
#define DEE_KEYCODE_DELETE     '\x7f'
#define DEE_KEYCODE_TAB        '\t'
#define DEE_KEYCODE_SPACE      ' '
#define DEE_KEYCODE_DBLQUOTE   '\"'
#define DEE_KEYCODE_QUOTE      '\''
#define DEE_KEYCODE_BACKQUOTE  '`'
#define DEE_KEYCODE_EXCLAIM    '!'
#define DEE_KEYCODE_DOLLAR     '$'
#define DEE_KEYCODE_PERCENT    '%'
#define DEE_KEYCODE_AMPERSAND  '&'
#define DEE_KEYCODE_SLASH      '/'
#define DEE_KEYCODE_EQUALS     '='
#define DEE_KEYCODE_QUESTION   '?'
#define DEE_KEYCODE_PLUS       '+'
#define DEE_KEYCODE_MINUS      '-'
#define DEE_KEYCODE_ASTERISK   '*'
#define DEE_KEYCODE_COMMA      ','
#define DEE_KEYCODE_PERIOD     '.'
#define DEE_KEYCODE_COLON      ':'
#define DEE_KEYCODE_SEMICOLON  ';'
#define DEE_KEYCODE_HASH       '#'
#define DEE_KEYCODE_AT         '@'
#define DEE_KEYCODE_LPAREN     '('
#define DEE_KEYCODE_RPAREN     ')'
#define DEE_KEYCODE_LESS       '<'
#define DEE_KEYCODE_GREATER    '>'
#define DEE_KEYCODE_LBRACKET   '['
#define DEE_KEYCODE_RBRACKET   ']'
#define DEE_KEYCODE_BACKSLASH  '\\'
#define DEE_KEYCODE_CARET      '^'
#define DEE_KEYCODE_UNDERSCORE '_'

//////////////////////////////////////////////////////////////////////////
// Returns the scancode for a given letter.
// >> switch (code) {
// >>   case DEE_KEYCODE_LETTER('w'): move_up(); break;
// >>   case DEE_KEYCODE_LETTER('s'): move_down(); break;
// >>   case DEE_KEYCODE_LETTER('a'): move_left(); break;
// >>   case DEE_KEYCODE_LETTER('d'): move_right(); break;
// >>   default: break;
// >> }
#define DEE_KEYCODE_LETTER(x) (((x)>='A'&&(x)<='Z')?((x)+('a'-'A')):(x))

//////////////////////////////////////////////////////////////////////////
// Returns the scancode for special keycodes (non-printable)
#define DEE_KEYCODE_OTHER(keycode) ((Dee_scancode_t)(keycode)|DEE_UINT16_C(0x8000))

#define DEE_KEYCODE_CAPSLOCK    DEE_KEYCODE_OTHER(DEE_SCANCODE_CAPSLOCK)
#define DEE_KEYCODE_F1          DEE_KEYCODE_OTHER(DEE_SCANCODE_F1)
#define DEE_KEYCODE_F2          DEE_KEYCODE_OTHER(DEE_SCANCODE_F2)
#define DEE_KEYCODE_F3          DEE_KEYCODE_OTHER(DEE_SCANCODE_F3)
#define DEE_KEYCODE_F4          DEE_KEYCODE_OTHER(DEE_SCANCODE_F4)
#define DEE_KEYCODE_F5          DEE_KEYCODE_OTHER(DEE_SCANCODE_F5)
#define DEE_KEYCODE_F6          DEE_KEYCODE_OTHER(DEE_SCANCODE_F6)
#define DEE_KEYCODE_F7          DEE_KEYCODE_OTHER(DEE_SCANCODE_F7)
#define DEE_KEYCODE_F8          DEE_KEYCODE_OTHER(DEE_SCANCODE_F8)
#define DEE_KEYCODE_F9          DEE_KEYCODE_OTHER(DEE_SCANCODE_F9)
#define DEE_KEYCODE_F10         DEE_KEYCODE_OTHER(DEE_SCANCODE_F10)
#define DEE_KEYCODE_F11         DEE_KEYCODE_OTHER(DEE_SCANCODE_F11)
#define DEE_KEYCODE_F12         DEE_KEYCODE_OTHER(DEE_SCANCODE_F12)
#define DEE_KEYCODE_PRINT       DEE_KEYCODE_OTHER(DEE_SCANCODE_PRINT)
#define DEE_KEYCODE_SCROLLLOCK  DEE_KEYCODE_OTHER(DEE_SCANCODE_SCROLLLOCK)
#define DEE_KEYCODE_PAUSE       DEE_KEYCODE_OTHER(DEE_SCANCODE_PAUSE)
#define DEE_KEYCODE_INSERT      DEE_KEYCODE_OTHER(DEE_SCANCODE_INSERT)
#define DEE_KEYCODE_HOME        DEE_KEYCODE_OTHER(DEE_SCANCODE_HOME)
#define DEE_KEYCODE_PAGEUP      DEE_KEYCODE_OTHER(DEE_SCANCODE_PAGEUP)
#define DEE_KEYCODE_END         DEE_KEYCODE_OTHER(DEE_SCANCODE_END)
#define DEE_KEYCODE_PAGEDOWN    DEE_KEYCODE_OTHER(DEE_SCANCODE_PAGEDOWN)
#define DEE_KEYCODE_RIGHT       DEE_KEYCODE_OTHER(DEE_SCANCODE_RIGHT)
#define DEE_KEYCODE_LEFT        DEE_KEYCODE_OTHER(DEE_SCANCODE_LEFT)
#define DEE_KEYCODE_DOWN        DEE_KEYCODE_OTHER(DEE_SCANCODE_DOWN)
#define DEE_KEYCODE_UP          DEE_KEYCODE_OTHER(DEE_SCANCODE_UP)
#define DEE_KEYCODE_NUMLOCK     DEE_KEYCODE_OTHER(DEE_SCANCODE_NUMLOCK)
#define DEE_KEYCODE_KP_DIV      DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_DIV)
#define DEE_KEYCODE_KP_MUL      DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_MUL)
#define DEE_KEYCODE_KP_SUB      DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_SUB)
#define DEE_KEYCODE_KP_ADD      DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_ADD)
#define DEE_KEYCODE_KP_ENTER    DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_ENTER)
#define DEE_KEYCODE_KP_1        DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_1)
#define DEE_KEYCODE_KP_2        DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_2)
#define DEE_KEYCODE_KP_3        DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_3)
#define DEE_KEYCODE_KP_4        DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_4)
#define DEE_KEYCODE_KP_5        DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_5)
#define DEE_KEYCODE_KP_6        DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_6)
#define DEE_KEYCODE_KP_7        DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_7)
#define DEE_KEYCODE_KP_8        DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_8)
#define DEE_KEYCODE_KP_9        DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_9)
#define DEE_KEYCODE_KP_0        DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_0)
#define DEE_KEYCODE_KP_PERIOD   DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_PERIOD)
#define DEE_KEYCODE_APP         DEE_KEYCODE_OTHER(DEE_SCANCODE_APP)
#define DEE_KEYCODE_POWER       DEE_KEYCODE_OTHER(DEE_SCANCODE_POWER)
#define DEE_KEYCODE_KP_EQUALS   DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_EQUALS)
#define DEE_KEYCODE_F13         DEE_KEYCODE_OTHER(DEE_SCANCODE_F13)
#define DEE_KEYCODE_F14         DEE_KEYCODE_OTHER(DEE_SCANCODE_F14)
#define DEE_KEYCODE_F15         DEE_KEYCODE_OTHER(DEE_SCANCODE_F15)
#define DEE_KEYCODE_F16         DEE_KEYCODE_OTHER(DEE_SCANCODE_F16)
#define DEE_KEYCODE_F17         DEE_KEYCODE_OTHER(DEE_SCANCODE_F17)
#define DEE_KEYCODE_F18         DEE_KEYCODE_OTHER(DEE_SCANCODE_F18)
#define DEE_KEYCODE_F19         DEE_KEYCODE_OTHER(DEE_SCANCODE_F19)
#define DEE_KEYCODE_F20         DEE_KEYCODE_OTHER(DEE_SCANCODE_F20)
#define DEE_KEYCODE_F21         DEE_KEYCODE_OTHER(DEE_SCANCODE_F21)
#define DEE_KEYCODE_F22         DEE_KEYCODE_OTHER(DEE_SCANCODE_F22)
#define DEE_KEYCODE_F23         DEE_KEYCODE_OTHER(DEE_SCANCODE_F23)
#define DEE_KEYCODE_F24         DEE_KEYCODE_OTHER(DEE_SCANCODE_F24)
#define DEE_KEYCODE_EXECUTE     DEE_KEYCODE_OTHER(DEE_SCANCODE_EXECUTE)
#define DEE_KEYCODE_HELP        DEE_KEYCODE_OTHER(DEE_SCANCODE_HELP)
#define DEE_KEYCODE_MENU        DEE_KEYCODE_OTHER(DEE_SCANCODE_MENU)
#define DEE_KEYCODE_SELECT      DEE_KEYCODE_OTHER(DEE_SCANCODE_SELECT)
#define DEE_KEYCODE_STOP        DEE_KEYCODE_OTHER(DEE_SCANCODE_STOP)
#define DEE_KEYCODE_AGAIN       DEE_KEYCODE_OTHER(DEE_SCANCODE_AGAIN)
#define DEE_KEYCODE_UNDO        DEE_KEYCODE_OTHER(DEE_SCANCODE_UNDO)
#define DEE_KEYCODE_CUT         DEE_KEYCODE_OTHER(DEE_SCANCODE_CUT)
#define DEE_KEYCODE_COPY        DEE_KEYCODE_OTHER(DEE_SCANCODE_COPY)
#define DEE_KEYCODE_PASTE       DEE_KEYCODE_OTHER(DEE_SCANCODE_PASTE)
#define DEE_KEYCODE_FIND        DEE_KEYCODE_OTHER(DEE_SCANCODE_FIND)
#define DEE_KEYCODE_MUTE        DEE_KEYCODE_OTHER(DEE_SCANCODE_MUTE)
#define DEE_KEYCODE_VOLUP       DEE_KEYCODE_OTHER(DEE_SCANCODE_VOLUP)
#define DEE_KEYCODE_VOLDOWN     DEE_KEYCODE_OTHER(DEE_SCANCODE_VOLDOWN)
#define DEE_KEYCODE_KP_COMMA    DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_COMMA)
#define DEE_KEYCODE_KP_EQUAL    DEE_KEYCODE_OTHER(DEE_SCANCODE_KP_EQUAL)
#define DEE_KEYCODE_ALTERASE    DEE_KEYCODE_OTHER(DEE_SCANCODE_ALTERASE)
#define DEE_KEYCODE_SYSREQ      DEE_KEYCODE_OTHER(DEE_SCANCODE_SYSREQ)
#define DEE_KEYCODE_CANCEL      DEE_KEYCODE_OTHER(DEE_SCANCODE_CANCEL)
#define DEE_KEYCODE_CLEAR       DEE_KEYCODE_OTHER(DEE_SCANCODE_CLEAR)
#define DEE_KEYCODE_PRIOR       DEE_KEYCODE_OTHER(DEE_SCANCODE_PRIOR)
/*#define DEE_KEYCODE_RETURN      DEE_KEYCODE_OTHER(DEE_SCANCODE_RETURN)*/
#define DEE_KEYCODE_SEPARATOR   DEE_KEYCODE_OTHER(DEE_SCANCODE_SEPARATOR)
#define DEE_KEYCODE_OUT         DEE_KEYCODE_OTHER(DEE_SCANCODE_OUT)
#define DEE_KEYCODE_OPER        DEE_KEYCODE_OTHER(DEE_SCANCODE_OPER)
#define DEE_KEYCODE_CLEARAGAIN  DEE_KEYCODE_OTHER(DEE_SCANCODE_CLEARAGAIN)
#define DEE_KEYCODE_CRSEL       DEE_KEYCODE_OTHER(DEE_SCANCODE_CRSEL)
#define DEE_KEYCODE_EXSEL       DEE_KEYCODE_OTHER(DEE_SCANCODE_EXSEL)
#define DEE_KEYCODE_LCTRL       DEE_KEYCODE_OTHER(DEE_SCANCODE_LCTRL)
#define DEE_KEYCODE_LSHIFT      DEE_KEYCODE_OTHER(DEE_SCANCODE_LSHIFT)
#define DEE_KEYCODE_LALT        DEE_KEYCODE_OTHER(DEE_SCANCODE_LALT)
#define DEE_KEYCODE_LGUI        DEE_KEYCODE_OTHER(DEE_SCANCODE_LGUI)
#define DEE_KEYCODE_RCTRL       DEE_KEYCODE_OTHER(DEE_SCANCODE_RCTRL)
#define DEE_KEYCODE_RSHIFT      DEE_KEYCODE_OTHER(DEE_SCANCODE_RSHIFT)
#define DEE_KEYCODE_RALT        DEE_KEYCODE_OTHER(DEE_SCANCODE_RALT)
#define DEE_KEYCODE_RGUI        DEE_KEYCODE_OTHER(DEE_SCANCODE_RGUI)



//////////////////////////////////////////////////////////////////////////
// Convert between host-dependent scan/key-codes
extern DEE_A_RET_WUNUSED Dee_scancode_t DeeKeycode_ToScancode(DEE_A_IN Dee_keycode_t kc);
extern DEE_A_RET_WUNUSED Dee_keycode_t DeeScancode_ToKeycode(DEE_A_IN Dee_scancode_t sc);


#ifdef DEE_PLATFORM_WINDOWS
extern Dee_scancode_t const DeeMap_Win32Scancode2Scancode[0x80];
extern DEE_A_RET_WUNUSED Dee_scancode_t DeeScancode_FromWin32ScanCode(
 DEE_A_IN LPARAM lParam, DEE_A_IN WPARAM wParam);
#endif




DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_GFX_WINDOW_SCANBOARD_H */
