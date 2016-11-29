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
#ifndef GUARD_DEEMON_DEX_GFX_WINDOW_SCANBOARD_C
#define GUARD_DEEMON_DEX_GFX_WINDOW_SCANBOARD_C 1
#define DEE_EXTENSION 1
#define DEE_LIMITED_DEX_GFX 1

#include <deemon/__conf.inl>
#include "keyboard.h"


DEE_DECL_BEGIN


#ifdef DEE_PLATFORM_WINDOWS
Dee_scancode_t const DeeMap_Win32Scancode2Scancode[0x80] = { 
 /* 0x00 */DEE_SCANCODE_NONE,
 /* 0x01 */DEE_SCANCODE_ESCAPE,
 /* 0x02 */DEE_SCANCODE_1,
 /* 0x03 */DEE_SCANCODE_2,
 /* 0x04 */DEE_SCANCODE_3,
 /* 0x05 */DEE_SCANCODE_4,
 /* 0x06 */DEE_SCANCODE_5,
 /* 0x07 */DEE_SCANCODE_6,
 /* 0x08 */DEE_SCANCODE_7,
 /* 0x09 */DEE_SCANCODE_8,
 /* 0x0a */DEE_SCANCODE_9,
 /* 0x0b */DEE_SCANCODE_0,
 /* 0x0c */DEE_SCANCODE_MINUS,
 /* 0x0d */DEE_SCANCODE_EQUALS,
 /* 0x0e */DEE_SCANCODE_BACKSPACE,
 /* 0x0f */DEE_SCANCODE_TAB,
 /* 0x10 */DEE_SCANCODE_Q,
 /* 0x11 */DEE_SCANCODE_W,
 /* 0x12 */DEE_SCANCODE_E,
 /* 0x13 */DEE_SCANCODE_R,
 /* 0x14 */DEE_SCANCODE_T,
 /* 0x15 */DEE_SCANCODE_Y,
 /* 0x16 */DEE_SCANCODE_U,
 /* 0x17 */DEE_SCANCODE_I,
 /* 0x18 */DEE_SCANCODE_O,
 /* 0x19 */DEE_SCANCODE_P,
 /* 0x1a */DEE_SCANCODE_LBRACKET,
 /* 0x1b */DEE_SCANCODE_RBRACKET,
 /* 0x1c */DEE_SCANCODE_RETURN,
 /* 0x1d */DEE_SCANCODE_LCTRL,
 /* 0x1e */DEE_SCANCODE_A,
 /* 0x1f */DEE_SCANCODE_S,
 /* 0x20 */DEE_SCANCODE_D,
 /* 0x21 */DEE_SCANCODE_F,
 /* 0x22 */DEE_SCANCODE_G,
 /* 0x23 */DEE_SCANCODE_H,
 /* 0x24 */DEE_SCANCODE_J,
 /* 0x25 */DEE_SCANCODE_K,
 /* 0x26 */DEE_SCANCODE_L,
 /* 0x27 */DEE_SCANCODE_SEMICOLON,
 /* 0x28 */DEE_SCANCODE_APOSTROHPE,
 /* 0x29 */DEE_SCANCODE_GRAVE,
 /* 0x2a */DEE_SCANCODE_LSHIFT,
 /* 0x2b */DEE_SCANCODE_BACKSLASH,
 /* 0x2c */DEE_SCANCODE_Z,
 /* 0x2d */DEE_SCANCODE_X,
 /* 0x2e */DEE_SCANCODE_C,
 /* 0x2f */DEE_SCANCODE_V,
 /* 0x30 */DEE_SCANCODE_B,
 /* 0x31 */DEE_SCANCODE_N,
 /* 0x32 */DEE_SCANCODE_M,
 /* 0x33 */DEE_SCANCODE_COMMA,
 /* 0x34 */DEE_SCANCODE_PERIOD,
 /* 0x35 */DEE_SCANCODE_SLASH,
 /* 0x36 */DEE_SCANCODE_RSHIFT,
 /* 0x37 */DEE_SCANCODE_PRINT,
 /* 0x38 */DEE_SCANCODE_LALT,
 /* 0x39 */DEE_SCANCODE_SPACE,
 /* 0x3a */DEE_SCANCODE_CAPSLOCK,
 /* 0x3b */DEE_SCANCODE_F1,
 /* 0x3c */DEE_SCANCODE_F2,
 /* 0x3d */DEE_SCANCODE_F3,
 /* 0x3e */DEE_SCANCODE_F4,
 /* 0x3f */DEE_SCANCODE_F5,
 /* 0x40 */DEE_SCANCODE_F6,
 /* 0x41 */DEE_SCANCODE_F7,
 /* 0x42 */DEE_SCANCODE_F8,
 /* 0x43 */DEE_SCANCODE_F9,
 /* 0x44 */DEE_SCANCODE_F10,
 /* 0x45 */DEE_SCANCODE_NUMLOCK,
 /* 0x46 */DEE_SCANCODE_SCROLLLOCK,
 /* 0x47 */DEE_SCANCODE_HOME,
 /* 0x48 */DEE_SCANCODE_UP,
 /* 0x49 */DEE_SCANCODE_PAGEUP,
 /* 0x4a */DEE_SCANCODE_KP_SUB,
 /* 0x4b */DEE_SCANCODE_LEFT,
 /* 0x4c */DEE_SCANCODE_KP_5,
 /* 0x4d */DEE_SCANCODE_RIGHT,
 /* 0x4e */DEE_SCANCODE_KP_ADD,
 /* 0x4f */DEE_SCANCODE_END,
 /* 0x50 */DEE_SCANCODE_DOWN,
 /* 0x51 */DEE_SCANCODE_PAGEDOWN,
 /* 0x52 */DEE_SCANCODE_INSERT,
 /* 0x53 */DEE_SCANCODE_DELETE,
 /* 0x54 */DEE_SCANCODE_NONE,
 /* 0x55 */DEE_SCANCODE_NONE,
 /* 0x56 */DEE_SCANCODE_NONUSBACKSLASH,
 /* 0x57 */DEE_SCANCODE_F11,
 /* 0x58 */DEE_SCANCODE_F12,
 /* 0x59 */DEE_SCANCODE_PAUSE,
 /* 0x5a */DEE_SCANCODE_NONE,
 /* 0x5b */DEE_SCANCODE_LGUI,
 /* 0x5c */DEE_SCANCODE_RGUI,
 /* 0x5d */DEE_SCANCODE_APP,
 /* 0x5e */DEE_SCANCODE_NONE,
 /* 0x5f */DEE_SCANCODE_NONE,
 /* 0x60 */DEE_SCANCODE_NONE,
 /* 0x61 */DEE_SCANCODE_NONE,
 /* 0x62 */DEE_SCANCODE_NONE,
 /* 0x63 */DEE_SCANCODE_NONE,
 /* 0x64 */DEE_SCANCODE_F13,
 /* 0x65 */DEE_SCANCODE_F14,
 /* 0x66 */DEE_SCANCODE_F15,
 /* 0x67 */DEE_SCANCODE_F16,
 /* 0x68 */DEE_SCANCODE_F17,
 /* 0x69 */DEE_SCANCODE_F18,
 /* 0x6a */DEE_SCANCODE_F19,
 /* 0x6b */DEE_SCANCODE_NONE,
 /* 0x6c */DEE_SCANCODE_NONE,
 /* 0x6d */DEE_SCANCODE_NONE,
 /* 0x6e */DEE_SCANCODE_NONE,
 /* 0x6f */DEE_SCANCODE_NONE,
 /* 0x70 */DEE_SCANCODE_NONE,
 /* 0x71 */DEE_SCANCODE_NONE,
 /* 0x72 */DEE_SCANCODE_NONE,
 /* 0x73 */DEE_SCANCODE_NONE,
 /* 0x74 */DEE_SCANCODE_NONE,
 /* 0x75 */DEE_SCANCODE_NONE,
 /* 0x76 */DEE_SCANCODE_NONE,
 /* 0x77 */DEE_SCANCODE_NONE,
 /* 0x78 */DEE_SCANCODE_NONE,
 /* 0x79 */DEE_SCANCODE_NONE,
 /* 0x7a */DEE_SCANCODE_NONE,
 /* 0x7b */DEE_SCANCODE_NONE,
 /* 0x7c */DEE_SCANCODE_NONE,
 /* 0x7d */DEE_SCANCODE_NONE,
 /* 0x7e */DEE_SCANCODE_NONE,
 /* 0x7f */DEE_SCANCODE_NONE
};
#endif


DEE_A_RET_WUNUSED Dee_scancode_t DeeKeycode_ToScancode(DEE_A_IN Dee_keycode_t kc) {
 (void)kc;
 return 0; // TODO
}
DEE_A_RET_WUNUSED Dee_keycode_t DeeScancode_ToKeycode(DEE_A_IN Dee_scancode_t sc) {
 // Simple case: This is a keycode-wrapped scancode
 if ((sc&0x8000)!=0) return (Dee_keycode_t)(sc&~(0x8000));
 return 0; // TODO
}


#ifdef DEE_PLATFORM_WINDOWS
DEE_A_RET_WUNUSED Dee_scancode_t DeeScancode_FromWin32ScanCode(
 DEE_A_IN LPARAM lParam, DEE_A_IN WPARAM wParam) {
 Dee_scancode_t result; int w32scancode = (lParam >> 16) & 0xFF;
 if (w32scancode == 0 || w32scancode == 0x45) switch (wParam) {
  case VK_CLEAR: return DEE_SCANCODE_CLEAR;
  case VK_SELECT: return DEE_SCANCODE_SELECT;
  case VK_EXECUTE: return DEE_SCANCODE_EXECUTE;
  case VK_HELP: return DEE_SCANCODE_HELP;
  case VK_PAUSE: return DEE_SCANCODE_PAUSE;
  case VK_NUMLOCK: return DEE_SCANCODE_NUMLOCK;
  case VK_F13: return DEE_SCANCODE_F13;
  case VK_F14: return DEE_SCANCODE_F14;
  case VK_F15: return DEE_SCANCODE_F15;
  case VK_F16: return DEE_SCANCODE_F16;
  case VK_F17: return DEE_SCANCODE_F17;
  case VK_F18: return DEE_SCANCODE_F18;
  case VK_F19: return DEE_SCANCODE_F19;
  case VK_F20: return DEE_SCANCODE_F20;
  case VK_F21: return DEE_SCANCODE_F21;
  case VK_F22: return DEE_SCANCODE_F22;
  case VK_F23: return DEE_SCANCODE_F23;
  case VK_F24: return DEE_SCANCODE_F24;
  case VK_OEM_NEC_EQUAL: return DEE_SCANCODE_KP_EQUALS;
  case VK_OEM_102: return DEE_SCANCODE_NONUSBACKSLASH;
  case VK_ATTN: return DEE_SCANCODE_SYSREQ;
  case VK_CRSEL: return DEE_SCANCODE_CRSEL;
  case VK_EXSEL: return DEE_SCANCODE_EXSEL;
  case VK_OEM_CLEAR: return DEE_SCANCODE_CLEAR;
  default: return DEE_SCANCODE_NONE;
 }
 if (w32scancode > 127) return DEE_SCANCODE_NONE;
 result = DeeMap_Win32Scancode2Scancode[w32scancode];
 // Fix extended scancodes
 if ((lParam & (1 << 24)) == 0) switch (result) {
  case DEE_SCANCODE_INSERT  : result = DEE_SCANCODE_KP_0;      break;
  case DEE_SCANCODE_PRINT   : result = DEE_SCANCODE_KP_MUL;    break;
  case DEE_SCANCODE_HOME    : result = DEE_SCANCODE_KP_7;      break;
  case DEE_SCANCODE_PAGEUP  : result = DEE_SCANCODE_KP_9;      break;
  case DEE_SCANCODE_DELETE  : result = DEE_SCANCODE_KP_PERIOD; break;
  case DEE_SCANCODE_END     : result = DEE_SCANCODE_KP_1;      break;
  case DEE_SCANCODE_PAGEDOWN: result = DEE_SCANCODE_KP_3;      break;
  case DEE_SCANCODE_RIGHT   : result = DEE_SCANCODE_KP_6;      break;
  case DEE_SCANCODE_LEFT    : result = DEE_SCANCODE_KP_4;      break;
  case DEE_SCANCODE_DOWN    : result = DEE_SCANCODE_KP_2;      break;
  case DEE_SCANCODE_UP      : result = DEE_SCANCODE_KP_8;      break;
  default: break;
 } else switch (result) {
  case DEE_SCANCODE_SLASH   : result = DEE_SCANCODE_KP_DIV;   break;
  case DEE_SCANCODE_CAPSLOCK: result = DEE_SCANCODE_KP_ADD;   break;
  case DEE_SCANCODE_RETURN  : result = DEE_SCANCODE_KP_ENTER; break;
  case DEE_SCANCODE_LALT    : result = DEE_SCANCODE_RALT;     break;
  case DEE_SCANCODE_LCTRL   : result = DEE_SCANCODE_RCTRL;    break;
  default: break;
 }
 return result;
}
#endif





DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_GFX_WINDOW_SCANBOARD_C */
