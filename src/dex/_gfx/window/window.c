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
#ifndef GUARD_DEEMON_DEX_GFX_WINDOW_WINDOW_C
#define GUARD_DEEMON_DEX_GFX_WINDOW_WINDOW_C 1
#define DEE_EXTENSION 1
#define DEE_LIMITED_DEX_GFX 1

#include <deemon/__conf.inl>
#include "window.h"
#include <deemon/string.h>
#include <deemon/bool.h>
#include <deemon/none.h>
#include <deemon/deemonrun.h>
#include <deemon/memberdef.h>
#include <deemon/tuple.h>
#include <deemon/type.h>
#include <deemon/optional/object_malloc.h>
#include <deemon/optional/object_visit.h>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN

DEE_A_RET_EXCEPT(-1) int DeeWin32Window_PumpEventsNoInterrupt(void) {
 DWORD start_ticks = GetTickCount(); MSG msg;
 while (PeekMessageW(&msg,0,0,0,PM_REMOVE)) {
  TranslateMessage(&msg);
  DispatchMessageW(&msg);
  // Don't continue handling events that occurred after we've started
  if (start_ticks < msg.time) break;
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int
DeeWin32Window_SetWindowStyle(DEE_A_IN HWND hwnd, DEE_A_IN LONG value) {
 SetLastError(0);
 if DEE_UNLIKELY(SetWindowLongW(hwnd,GWL_STYLE,value) == 0) {
  DWORD last_error = GetLastError();
  if DEE_UNLIKELY(last_error != 0) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "SetWindowLongW(%p,GWL_STYLE,%ld) : %K",hwnd,value,
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
   return -1;
  }
 }
 return 0;
}
DEE_A_RET_WUNUSED BOOL DeeWin32Window_TryGetPos(
 DEE_A_IN HWND hwnd, DEE_A_OUT struct DeeWindowPos *result) {
 struct DeeWindowRect rect;
 if DEE_UNLIKELY(!GetWindowRect(hwnd,(LPRECT)&rect)) return FALSE;
 result->w32_pos.x = rect.w32_rect.left;
 result->w32_pos.y = rect.w32_rect.top;
 return TRUE;
}
DEE_A_RET_WUNUSED BOOL DeeWin32Window_TryGetSize(
 DEE_A_IN HWND hwnd, DEE_A_OUT struct DeeWindowSize *result) {
 struct DeeWindowRect rect;
 if DEE_UNLIKELY(!GetWindowRect(hwnd,(LPRECT)&rect)) return FALSE;
 result->w32_size.x = DeeWindowRect_SIZEX(&rect);
 result->w32_size.y = DeeWindowRect_SIZEY(&rect);
 return TRUE;
}
DEE_A_RET_WUNUSED BOOL _DeeWin32Window_TryEnterFullscreen_impl(DEE_A_IN HWND hwnd) {
 MONITORINFO mi; mi.cbSize = sizeof(mi);
 if DEE_UNLIKELY(!GetMonitorInfoW(MonitorFromWindow(hwnd,MONITOR_DEFAULTTOPRIMARY),&mi)) return FALSE;
 SetLastError(0);
 if DEE_UNLIKELY(SetWindowLongW(hwnd,GWL_STYLE,(LONG)((GetWindowLongW(hwnd,GWL_STYLE)
  &~(WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX))
  | (WS_POPUP))) == 0 && GetLastError() != 0) return FALSE;
 return SetWindowPos(hwnd,HWND_TOPMOST,
                     mi.rcMonitor.left,mi.rcMonitor.top,
                     mi.rcMonitor.right-mi.rcMonitor.left,
                     mi.rcMonitor.bottom-mi.rcMonitor.top,
                     SWP_NOOWNERZORDER|SWP_FRAMECHANGED);
}




DEE_A_RET_WUNUSED int DeeWin32Window_GetRect(
 DEE_A_IN HWND hwnd, DEE_A_OUT struct DeeWindowRect *result) {
 if DEE_UNLIKELY(!GetWindowRect(hwnd,(LPRECT)result)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "GetWindowRect(%p) : %K",hwnd,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeWin32Window_GetPos(
 DEE_A_IN HWND hwnd, DEE_A_OUT struct DeeWindowPos *result) {
 struct DeeWindowRect rect;
 if DEE_UNLIKELY(DeeWin32Window_GetRect(hwnd,&rect) != 0) return -1;
 result->w32_pos.x = rect.w32_rect.left;
 result->w32_pos.y = rect.w32_rect.top;
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeWin32Window_GetSize(
 DEE_A_IN HWND hwnd, DEE_A_OUT struct DeeWindowSize *result) {
 struct DeeWindowRect rect;
 if DEE_UNLIKELY(DeeWin32Window_GetRect(hwnd,&rect) != 0) return -1;
 result->w32_size.x = DeeWindowRect_SIZEX(&rect);
 result->w32_size.y = DeeWindowRect_SIZEY(&rect);
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeWin32Window_SetRect(
 DEE_A_IN HWND hwnd, DEE_A_IN struct DeeWindowRect const *rect) {
 if DEE_UNLIKELY(!SetWindowPos(hwnd,NULL,rect->w32_rect.left,rect->w32_rect.top,
  DeeWindowRect_SIZEX(rect),DeeWindowRect_SIZEY(rect),SWP_NOZORDER)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "SetWindowPos(%p,NULL,%ld,%ld,%ld,%ld,SWP_NOZORDER) : %K",
                      hwnd,rect->w32_rect.left,rect->w32_rect.top,
                      DeeWindowRect_SIZEX(rect),DeeWindowRect_SIZEY(rect),
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeWin32Window_SetPos(
 DEE_A_IN HWND hwnd, DEE_A_IN struct DeeWindowPos const *pos) {
 if DEE_UNLIKELY(!SetWindowPos(hwnd,NULL,pos->w32_pos.x,pos->w32_pos.y,0,0,SWP_NOSIZE|SWP_NOZORDER)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "SetWindowPos(%p,NULL,%ld,%ld,0,0,SWP_NOSIZE|SWP_NOZORDER) : %K",
                      hwnd,pos->w32_pos.x,pos->w32_pos.y,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeWin32Window_SetSize(
 DEE_A_IN HWND hwnd, DEE_A_IN struct DeeWindowSize const *size) {
 if DEE_UNLIKELY(!SetWindowPos(hwnd,NULL,0,0,size->w32_size.x,size->w32_size.y,SWP_NOMOVE|SWP_NOZORDER)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "SetWindowPos(%p,NULL,0,0,%ld,%ld,SWP_NOSIZE|SWP_NOZORDER) : %K",
                      hwnd,size->w32_size.x,size->w32_size.y,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}


DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Show(DEE_A_IN HWND hwnd) {
 if DEE_UNLIKELY(!ShowWindow(hwnd,SW_SHOW)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "ShowWindow(%p,SW_SHOW) : %K",hwnd,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Hide(DEE_A_IN HWND hwnd) {
 if DEE_UNLIKELY(!ShowWindow(hwnd,SW_HIDE)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "ShowWindow(%p,SW_HIDE) : %K",hwnd,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Raise(DEE_A_IN HWND hwnd) {
 if DEE_UNLIKELY(!SetWindowPos(hwnd,DeeWin32Window_IsFullscreen(hwnd)
  ? HWND_TOPMOST : HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "SetWindowPos(%p,%s,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE) : %K",hwnd,
                      DeeWin32Window_IsFullscreen(hwnd) ? "HWND_TOPMOST" : "HWND_TOP",
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Minimize(DEE_A_IN HWND hwnd) {
 if DEE_UNLIKELY(!ShowWindow(hwnd,SW_MINIMIZE)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "ShowWindow(%p,SW_MINIMIZE) : %K",hwnd,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Maximize(DEE_A_IN HWND hwnd) {
 if DEE_UNLIKELY(!ShowWindow(hwnd,SW_MAXIMIZE)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "ShowWindow(%p,SW_MAXIMIZE) : %K",hwnd,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Restore(DEE_A_IN HWND hwnd) {
 if DEE_UNLIKELY(!ShowWindow(hwnd,SW_NORMAL)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "ShowWindow(%p,SW_NORMAL) : %K",hwnd,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Focus(DEE_A_IN HWND hwnd) {
 if DEE_UNLIKELY(!SetFocus(hwnd)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "SetFocus(%p) : %K",hwnd,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeWin32Window_EnterFullscreen(
 DEE_A_IN HWND hwnd, DEE_A_OUT WINDOWPLACEMENT *oldplace) {
 MONITORINFO mi; HMONITOR hmon; LONG newstyle;
 if DEE_UNLIKELY(!GetWindowPlacement(hwnd,oldplace)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "GetWindowPlacement(%p) : %K",hwnd,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 mi.cbSize = sizeof(mi);
 hmon = MonitorFromWindow(hwnd,MONITOR_DEFAULTTOPRIMARY);
 if DEE_UNLIKELY(!GetMonitorInfoW(hmon,&mi)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "GetMonitorInfoW(%p) : %K",hmon,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 newstyle = (LONG)((GetWindowLongW(hwnd,GWL_STYLE)&~(WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX))|(WS_POPUP));
 if DEE_UNLIKELY(DeeWin32Window_SetWindowStyle(hwnd,newstyle) != 0) return -1;
 if DEE_UNLIKELY(!SetWindowPos(hwnd,HWND_TOPMOST,
  mi.rcMonitor.left,mi.rcMonitor.top,
  mi.rcMonitor.right-mi.rcMonitor.left,
  mi.rcMonitor.bottom-mi.rcMonitor.top,
  SWP_NOOWNERZORDER|SWP_FRAMECHANGED)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "SetWindowPos(%p,HWND_TOPMOST,%ld,%ld,%ld,%ld,SWP_NOOWNERZORDER|SWP_FRAMECHANGED) : %K",
                      hwnd,(LONG)mi.rcMonitor.left,(LONG)mi.rcMonitor.top,
                      (LONG)(mi.rcMonitor.right-mi.rcMonitor.left),
                      (LONG)(mi.rcMonitor.bottom-mi.rcMonitor.top),
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeWin32Window_LeaveFullscreen(
 DEE_A_IN HWND hwnd, DEE_A_IN WINDOWPLACEMENT const *oldplace) {
 LONG newstyle,oldstyle;
 oldstyle = DeeWin32Window_GetStyle(hwnd);
 newstyle = (LONG)((oldstyle&~(WS_POPUP))|(WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX));
 if DEE_UNLIKELY(DeeWin32Window_SetWindowStyle(hwnd,newstyle) != 0) return -1;
 if DEE_UNLIKELY(!SetWindowPlacement(hwnd,oldplace)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "SetWindowPlacement(%p,...) : %K",hwnd,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  // Try to undo the style change
  SetWindowLongW(hwnd,GWL_STYLE,oldstyle);
  return -1;
 }
 if DEE_UNLIKELY(!SetWindowPos(hwnd,NULL,0,0,0,0,
  SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_FRAMECHANGED)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "SetWindowPos(%p,NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_FRAMECHANGED) : %K",hwnd,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}


DEE_A_RET_EXCEPT(-1) int DeeWin32Window_SetAttr(
 DEE_A_IN HWND hwnd, DEE_A_IN_Z char const *attr, DEE_A_IN_OPT void *value) {
 if DEE_UNLIKELY(!SetPropA(hwnd,attr,(HANDLE)value)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "SetPropA(%p,%s,%p) : %K",hwnd,attr,value,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Flip(DEE_A_IN HWND hwnd) {
 if (!UpdateWindow(hwnd)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "UpdateWindow(%p) : %K",hwnd,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}

DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeWin32Window_Utf8GetTitle(DEE_A_IN HWND hwnd) {
 int len,len2; DeeObject *result; DWORD err;
again:
 if DEE_UNLIKELY((len = GetWindowTextLengthA(hwnd)) <= 0) {
  if DEE_LIKELY((err = GetLastError()) == 0) DeeReturn_EmptyUtf8String;
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "GetWindowTextLengthA(%p) : %K",hwnd,
                      DeeSystemError_Win32ToString(err));
  return NULL;
 }
 if DEE_UNLIKELY((result = DeeUtf8String_NewSized((Dee_size_t)len)) == NULL) return NULL;
 len2 = GetWindowTextA(hwnd,DeeUtf8String_STR(result),len+1);
 if (len2 != len+1) {
  Dee_DECREF(result);
  if (len2 > 0) goto again;
  if DEE_LIKELY((err = GetLastError()) == 0) DeeReturn_EmptyUtf8String;
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "GetWindowTextA(%p,...,%d) : %K",hwnd,len+1,
                      DeeSystemError_Win32ToString(err));
  return NULL;
 }
 return result;
}
DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *
DeeWin32Window_WideGetTitle(DEE_A_IN HWND hwnd) {
 int len,len2; DeeObject *result; DWORD err;
again:
 if DEE_UNLIKELY((len = GetWindowTextLengthW(hwnd)) <= 0) {
  if DEE_LIKELY((err = GetLastError()) == 0) DeeReturn_EmptyWideString;
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "GetWindowTextLengthW(%p) : %K",hwnd,
                      DeeSystemError_Win32ToString(err));
  return NULL;
 }
 if DEE_UNLIKELY((result = DeeWideString_NewSized((Dee_size_t)len)) == NULL) return NULL;
 len2 = GetWindowTextW(hwnd,DeeWideString_STR(result),len+1);
 if (len2 != len+1) {
  Dee_DECREF(result);
  if (len2 > 0) goto again;
  if DEE_LIKELY((err = GetLastError()) == 0) DeeReturn_EmptyWideString;
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "GetWindowTextW(%p,...,%d) : %K",hwnd,len+1,
                      DeeSystemError_Win32ToString(err));
  return NULL;
 }
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Utf8SetTitle(
 DEE_A_IN HWND hwnd, DEE_A_IN_Z Dee_Utf8Char const *title) {
 if (!SetWindowTextA(hwnd,title)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "SetWindowTextA(%p,%s) : %K",hwnd,title,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeWin32Window_WideSetTitle(
 DEE_A_IN HWND hwnd, DEE_A_IN_Z Dee_WideChar const *title) {
 if (!SetWindowTextW(hwnd,title)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "SetWindowTextW(%p,%lds) : %K",hwnd,title,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeWin32Window_SetIcon(
 DEE_A_IN HWND hwnd, DEE_A_IN DeeSurfaceObject const *icon) {
 struct BitmapPixel { Dee_uint8_t a,r,g,b; }; // ARGB8888
 HICON icon_handle; BYTE *bmp_data; struct DeePixel pixel;
 struct BitmapPixel *bmp_start,*line; Dee_size_t bmp_scanline;
 Dee_size_t icon_size,pixelsize,sizex,sizey; Dee_size_t x,y,invy;
 PDeeSurfaceGetPixel getter;
 sizey = DeeSurface_SIZEY(icon),sizex = DeeSurface_SIZEX(icon);
 icon_size = 40+(pixelsize = (sizey*(sizex*4)));
#if DEE_TYPES_SIZEOF_SIZE_T > 4
 // Make sure this icon fits into a BMP
 if DEE_UNLIKELY(DeeSurface_SIZEX(icon) > 0xFFFFFFFF
              || DeeSurface_SIZEY(icon) > 0xFFFFFFFF/2
              || pixelsize > 0xFFFFFFFF) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Given surface is too bit to use as a window icon (%Iu*%Iu exceeds maximum of %I32u*%I32u)",
                      DeeSurface_SIZEX(icon),DeeSurface_SIZEY(icon),
                      (Dee_uint32_t)0xFFFFFFFF,(Dee_uint32_t)0xFFFFFFFF/2);
  return -1;
 }
#endif
 while DEE_UNLIKELY((bmp_data = (BYTE *)malloc_nz(icon_size)) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return -1;
 }
 ((LPBITMAPINFOHEADER)bmp_data)->biSize = sizeof(BITMAPINFOHEADER);
 ((LPBITMAPINFOHEADER)bmp_data)->biWidth = (LONG)DeeSurface_SIZEX(icon);
 ((LPBITMAPINFOHEADER)bmp_data)->biHeight = (LONG)DeeSurface_SIZEY(icon)*2;
 ((LPBITMAPINFOHEADER)bmp_data)->biPlanes = 1;
 ((LPBITMAPINFOHEADER)bmp_data)->biBitCount = 32;
 ((LPBITMAPINFOHEADER)bmp_data)->biCompression = BI_RGB;
 ((LPBITMAPINFOHEADER)bmp_data)->biSizeImage = (DWORD)pixelsize;
 ((LPBITMAPINFOHEADER)bmp_data)->biXPelsPerMeter = 0;
 ((LPBITMAPINFOHEADER)bmp_data)->biYPelsPerMeter = 0;
 ((LPBITMAPINFOHEADER)bmp_data)->biClrUsed = 0;
 ((LPBITMAPINFOHEADER)bmp_data)->biClrImportant = 0;

 // NOTE: Pixels must be written upside-down (y follows correct coords)
 bmp_start = (struct BitmapPixel *)(((LPBITMAPINFOHEADER)bmp_data)+1);
 bmp_scanline = sizex*4,getter = DeeSurface_TYPE(icon)->st_getpixel;
 for (y = 0; y < sizey; ++y) {
  invy = sizey-(y+1);
  line = bmp_start+invy*bmp_scanline;
  for (x = 0; x < sizex; ++x) {
   (*getter)(icon,x,y,&pixel);
   line->r = pixel.r,line->g = pixel.g;
   line->b = pixel.b,line->a = pixel.a;
   ++line;
  }
 }
 icon_handle = CreateIconFromResource(bmp_data,icon_size,TRUE,0x00030000);
 free_nn(bmp_data);
 // Post the new icon
 SendMessageW(hwnd,WM_SETICON,ICON_SMALL,(LPARAM)icon_handle);
 SendMessageW(hwnd,WM_SETICON,ICON_BIG,(LPARAM)icon_handle);
#if _WIN32_WINNT >= 0x0501
 // TODO: What windows version is this?
 //SendMessageW(hwnd,WM_SETICON,ICON_SMALL2,(LPARAM)icon_handle);
#endif
 // XXX: Shouldn't we free the icon-handle at some point?
 return 0;
}



DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_Show(DEE_A_INOUT struct DeeWindowObject *self) {
 if (DeeWindow_IsVisible(self)) return 1;
 if (DeeWindow_DoShow(self) != 0) return -1;
 self->w_flags &= ~(DEE_WINDOWFLAG_HIDDEN);
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_Hide(DEE_A_INOUT struct DeeWindowObject *self) {
 if (DeeWindow_IsHidden(self)) return 1;
 if (DeeWindow_DoHide(self) != 0) return -1;
 self->w_flags |= DEE_WINDOWFLAG_HIDDEN;
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_Minimize(DEE_A_INOUT struct DeeWindowObject *self) {
 if (DeeWindow_IsMinimized(self)) return 1;
 if (DeeWindow_DoMinimize(self) != 0) return -1;
 self->w_flags = (self->w_flags|DEE_WINDOWFLAG_MINIMIZED)&~(DEE_WINDOWFLAG_MAXIMIZED);
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_Maximize(DEE_A_INOUT struct DeeWindowObject *self) {
 if (DeeWindow_IsMaximized(self)) return 1;
 if (DeeWindow_DoMaximize(self) != 0) return -1;
 self->w_flags = (self->w_flags|DEE_WINDOWFLAG_MAXIMIZED)&~(DEE_WINDOWFLAG_MINIMIZED);
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_Restore(DEE_A_INOUT struct DeeWindowObject *self) {
 if ((_DeeWindow_Flags(self)&(DEE_WINDOWFLAG_MINIMIZED|DEE_WINDOWFLAG_MAXIMIZED))==0) return 1;
 if (DeeWindow_DoRestore(self) != 0) return -1;
 self->w_flags &= ~(DEE_WINDOWFLAG_MINIMIZED|DEE_WINDOWFLAG_MAXIMIZED);
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_Focus(DEE_A_INOUT struct DeeWindowObject *self) {
 if (DeeWindow_IsFocused(self)) return 1;
 if (DeeWindow_DoFocus(self) != 0) return -1;
 self->w_flags |= DEE_WINDOWFLAG_FOCUSED;
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_AllowResizeable(DEE_A_INOUT struct DeeWindowObject *self) {
 if (DeeWindow_IsResizeable(self)) return 1;
 if (DeeWindow_DoAllowResizeable(self) != 0) return -1;
 self->w_flags |= DEE_WINDOWFLAG_RESIZEABLE;
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_PreventResizeable(DEE_A_INOUT struct DeeWindowObject *self) {
 if (!DeeWindow_IsResizeable(self)) return 1;
 if (DeeWindow_DoPreventResizeable(self) != 0) return -1;
 self->w_flags &= ~(DEE_WINDOWFLAG_RESIZEABLE);
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_EnterFullscreen(DEE_A_INOUT struct DeeWindowObject *self) {
 if (DeeWindow_IsFullscreen(self)) return 1;
 if (DeeWindow_DoEnterFullscreen(self) != 0) return -1;
 self->w_flags |= DEE_WINDOWFLAG_FULLSCREEN;
 return 0;
}
DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_LeaveFullscreen(DEE_A_INOUT struct DeeWindowObject *self) {
 if (!DeeWindow_IsFullscreen(self)) return 1;
 if (DeeWindow_DoLeaveFullscreen(self) != 0) return -1;
 self->w_flags |= DEE_WINDOWFLAG_FULLSCREEN;
 return 0;
}


static void DEE_CALL _deewindowsurface_st_getpixel(
 DeeWindowSurfaceObject const *dst, Dee_size_t x, Dee_size_t y, struct DeePixel *pixel) {
 void *datap;
 DEE_ASSERT(DeeObject_Check(dst) && DeeWindowSurface_Check(dst));
 DEE_ASSERT(x < DeeSurface_SIZEX(dst));
 DEE_ASSERT(y < DeeSurface_SIZEY(dst));
 datap = DeeWindowSurface_PIXELADDR(dst,x,y);
 DEEPIXELSURFACE_GENERIC_GETPIXEL(&dst->ws_spec,datap,pixel);
}
static void DEE_CALL _deewindowsurface_st_setpixel(
 DeeWindowSurfaceObject *dst, Dee_size_t x, Dee_size_t y, struct DeePixel const *pixel,
 DEE_A_IN Dee_blendinfo_t blend) {
 void *datap; struct DeePixel pix;
 DEE_ASSERT(DeeObject_Check(dst) && DeeWindowSurface_Check(dst));
 DEE_ASSERT(x < DeeSurface_SIZEX(dst));
 DEE_ASSERT(y < DeeSurface_SIZEY(dst));
 datap = DeeWindowSurface_PIXELADDR(dst,x,y);
 DEEPIXELSURFACE_GENERIC_GETPIXEL(&dst->ws_spec,datap,&pix);
 DeePixel_Blend(&pix,pixel,blend);
 DEEPIXELSURFACE_GENERIC_SETPIXEL(&dst->ws_spec,datap,&pix);
}

static void DEE_CALL _deewindowsurface_tp_dtor(DeeWindowSurfaceObject *self) {
 if (self->nw_w32mdc) DeleteDC(self->nw_w32mdc);
 if (self->nw_w32hbm) DeleteObject(self->nw_w32hbm);
}


DeeSurfaceTypeObject DeeWindowSurface_Type = {
 {DEE_TYPE_OBJECT_HEAD_EX_v100(
   member(&DeeSurfaceType_Type),member("windowsurface"),null,
   member(DEE_TYPE_FLAG_NO_SUBCLASS),member((DeeTypeObject *)&DeeSurface_Type)),
  DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeWindowSurfaceObject),null,null,null,null,null),
  DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deewindowsurface_tp_dtor)),
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
 },
 DEE_SURFACETYPE_FORMAT_KIND_PIXEL, // st_format
 NULL,                              // st_next
 {DeeSurfacePixelSpec_INIT(0,(0,0,0),(0,0,0),(0,0,0),(0,0,0))},
 (void (DEE_CALL *)(DeeSurfaceObject const *,Dee_size_t,Dee_size_t,struct DeePixel *))&_deewindowsurface_st_getpixel, // st_getpixel
 (void (DEE_CALL *)(DeeSurfaceObject *,Dee_size_t,Dee_size_t,struct DeePixel const *,Dee_blendinfo_t))&_deewindowsurface_st_setpixel, // st_setpixel
 &_deesurface_generic_st_fill,         // st_fill
 &_deesurface_generic_st_fillrect,     // st_fillrect
 &_deesurface_generic_st_xline,        // st_xline
 &_deesurface_generic_st_yline,        // st_yline
 &_deesurface_generic_st_linellhh,     // st_linellhh
 &_deesurface_generic_st_linelhhl,     // st_linelhhl
 &_deesurface_generic_st_blit,         // st_blit
 &_deesurface_generic_st_stretchblit,  // st_stretchblit
 &_deesurface_generic_st_flipx,        // st_flipx
 &_deesurface_generic_st_flipy,        // st_flipy
 &_deesurface_generic_st_pixelmaskmsb, // st_pixelmaskmsb
 &_deesurface_generic_st_pixelmasklsb, // st_pixelmasklsb
};


struct DEE_A_RET_EXCEPT_REF DeeWindowSurfaceObject *DeeWindow_Win32CreateWindowSurface(
 DEE_A_IN HDC hdc, DEE_A_IN Dee_size_t sx, DEE_A_IN Dee_size_t sy) {
 struct InfoData { BITMAPINFOHEADER bmiHeader; RGBQUAD bmiColors[256]; };
 struct DeeWindowSurfaceObject *result; Dee_uint64_t format;
 struct InfoData info; HBITMAP hbitmap;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(struct DeeWindowSurfaceObject)) == NULL) return NULL;
 DeeObject_INIT(result,(DeeTypeObject *)&DeeWindowSurface_Type);
 memset(&info,0,sizeof(info));
 info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
 hbitmap = CreateCompatibleBitmap(hdc, 1, 1);
 GetDIBits(hdc,hbitmap,0,0,NULL,(LPBITMAPINFO)&info,DIB_RGB_COLORS);
 GetDIBits(hdc,hbitmap,0,0,NULL,(LPBITMAPINFO)&info,DIB_RGB_COLORS);
 if (!DeleteObject(hbitmap)) SetLastError(0);
 if (info.bmiHeader.biCompression == BI_BITFIELDS) {
  int bpp; Dee_uint32_t *masks;
  bpp = (info.bmiHeader.biPlanes*info.bmiHeader.biBitCount);
  masks = (Dee_uint32_t *)((Dee_uint8_t *)&info+info.bmiHeader.biSize);
  if DEE_UNLIKELY(bpp >= 256) {
   DeeError_SetStringf(&DeeErrorType_ValueError,
                       "Unsupported bit-depth: %I16u",
                       bpp);
   goto err_r;
  }
  format = DEE_SURFACETYPE_FORMAT_PIXEL_((Dee_uint8_t)bpp,
                                         masks[0],masks[1],
                                         masks[2],0);
 } else {
  format = DEE_SURFACETYPE_FORMAT_PIXEL_(32,0x00FF0000,0x0000FF00,0x000000FF,0); // RGB0888
  memset(&info,0,sizeof(info));
  info.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
  info.bmiHeader.biPlanes      = 1;
  info.bmiHeader.biBitCount    = 32;
  info.bmiHeader.biCompression = BI_RGB;
 }
 // Calculate the pixel specifications
 if (DeeSurfacePixelSpec_Init(&result->ws_spec,format) != 0) {
err_r: _DeeObject_DELETE(&DeeWindowSurface_Type,result);
  return NULL;
 }
 result->s_sizex = sx;
 result->s_sizey = sy;
 // Scanline is a bit more complicated here...
 result->ws_scanline = ((sx*result->ws_pixlsize)+3)&~3;
 info.bmiHeader.biWidth = (LONG)sx;
 info.bmiHeader.biHeight = -(LONG)sy;
 info.bmiHeader.biSizeImage = (LONG)sy*result->ws_scanline;
 result->nw_w32mdc = CreateCompatibleDC(hdc);
 if DEE_UNLIKELY(!result->nw_w32mdc) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "CreateCompatibleDC(%p) : %K",hdc,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  goto err_r;
 }
 result->nw_w32hbm = CreateDIBSection(hdc,(BITMAPINFO const *)&info,
                                      DIB_RGB_COLORS,(void **)&result->ws_pixels,
                                      NULL,0);
 if DEE_UNLIKELY(!result->nw_w32hbm) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "CreateDIBSection(%p,...,DIB_RGB_COLORS,...,NULL,0) : Can't create DIB : %K",
                      hdc,DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  if (result->nw_w32mdc && !DeleteDC(result->nw_w32mdc)) SetLastError(0);
  goto err_r;
 }
 SelectObject(result->nw_w32mdc,result->nw_w32hbm);
 return result;
}


static LRESULT CALLBACK DeeWindow_Win32Main(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


static Dee_Utf8Char const _DeeWindow_ClassNameA[] = {'D','E','E','M','O','N','_','W','I','N','D','O','W',0};
static Dee_WideChar const _DeeWindow_ClassNameW[] = {'D','E','E','M','O','N','_','W','I','N','D','O','W',0};
static /*atomic*/Dee_uint16_t _DeeWindow_ClassInitialized = 0;

static DEE_A_RET_EXCEPT(-1) int _DeeWindow_Win32InitClass(HMODULE class_module) {
 WNDCLASSW class_;
 if (DeeAtomic16_FetchInc(_DeeWindow_ClassInitialized,memory_order_seq_cst) != 0) return 0;
 class_.hCursor = 0;
 class_.hIcon = NULL;
 class_.lpszMenuName = NULL;
 class_.lpszClassName = _DeeWindow_ClassNameW;
 class_.hbrBackground = NULL;
 class_.hInstance = class_module;
 class_.style = 0;
 class_.lpfnWndProc = &DeeWindow_Win32Main;
 class_.cbWndExtra = 0;
 class_.cbClsExtra = 0;
 if (!RegisterClassW(&class_)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "RegisterClassW(%lq) : %K",_DeeWindow_ClassNameW,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 DEE_LVERBOSE2("Created Window Class: %lq\n",_DeeWindow_ClassNameW);
 return 0;
}
static void _DeeWindow_Win32QuitClass(void) {
 HMODULE class_module;
 if (DeeAtomic16_DecFetch(_DeeWindow_ClassInitialized,memory_order_seq_cst) != 0) return;
 class_module = GetModuleHandleW(NULL);
 if (!UnregisterClassW(_DeeWindow_ClassNameW,class_module)) SetLastError(0);
 DEE_LVERBOSE2("Destroyed Window Class: %lq\n",_DeeWindow_ClassNameW);
}


DEE_A_RET_EXCEPT(-1) int DeeWindow_Init(
 DEE_A_OUT struct DeeWindowObject *self, DEE_A_IN struct DeeWindowRect const *rect,
 DEE_A_IN Dee_windowflag_t flags, DEE_A_IN_Z_OPT char const *title,
 DEE_A_IN_OPT DeeSurfaceObject *icon) {
 RECT adjusted_rect; DWORD used_style,used_styleex;
 HINSTANCE window_module; int window_width,window_height;
 used_style = DEE_WINDOWFLAG_W32GETSTYLE(flags);
 used_styleex = DEE_WINDOWFLAG_W32GETSTYLEEX(flags);
 if (DEE_WINDOWFLAG_ISFULLSCREEN(flags)) {
  MONITORINFO mi; HMONITOR used_monitor;
  POINT monitor_center;
  // Adjust to fill the entire monitor
  // NOTE: We prefer to use the monitor at the center of the given windowrect
  monitor_center.x = rect->w32_rect.left+(rect->w32_rect.right-rect->w32_rect.left)/2;
  monitor_center.y = rect->w32_rect.top+(rect->w32_rect.bottom-rect->w32_rect.top)/2;
  used_monitor = MonitorFromPoint(monitor_center,MONITOR_DEFAULTTONEAREST);
  mi.cbSize = sizeof(mi);
  if DEE_UNLIKELY(!GetMonitorInfoW(used_monitor,&mi)) {
   DeeError_SetStringf(&DeeErrorType_SystemError,
                       "GetMonitorInfoW(%q) : %K",used_monitor,
                       DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
   return -1;
  }
  adjusted_rect = mi.rcMonitor;
  // Calculate the window position when fullscreen is left
  self->w_w32oldplace.length = sizeof(self->w_w32oldplace);
  self->w_w32oldplace.flags = 0;
  self->w_w32oldplace.showCmd = SW_SHOWNORMAL;
  self->w_w32oldplace.ptMinPosition.x = 0;
  self->w_w32oldplace.ptMinPosition.y = 0;
  self->w_w32oldplace.ptMaxPosition.x = 0;
  self->w_w32oldplace.ptMaxPosition.y = 0;
  self->w_w32oldplace.rcNormalPosition = rect->w32_rect;
  AdjustWindowRectEx(&self->w_w32oldplace.rcNormalPosition,
                     DEE_WINDOWFLAG_W32GETSTYLE(flags)&~(DEE_WINDOWFLAG_FULLSCREEN),FALSE,
                     DEE_WINDOWFLAG_W32GETSTYLEEX(flags)&~(DEE_WINDOWFLAG_FULLSCREEN));
#ifdef _MAC /* ??? */
  memset(&self->w_w32oldplace.rcDevice,0,sizeof(self->w_w32oldplace.rcDevice));
#endif
 } else {
  adjusted_rect = rect->w32_rect;
  AdjustWindowRectEx(&adjusted_rect,used_style,FALSE,used_styleex);
 }
 if (adjusted_rect.right <= adjusted_rect.left) adjusted_rect.right = adjusted_rect.left+1;
 if (adjusted_rect.bottom <= adjusted_rect.top) adjusted_rect.bottom = adjusted_rect.top+1;
 window_width = (int)(adjusted_rect.right-adjusted_rect.left);
 window_height = (int)(adjusted_rect.bottom-adjusted_rect.top);
 window_module = GetModuleHandleW(NULL);
 if (_DeeWindow_Win32InitClass(window_module) != 0) return -1;
 self->w_w32hwnd = CreateWindowExA(used_styleex,_DeeWindow_ClassNameA,title,used_style,
                                   adjusted_rect.left,adjusted_rect.top,
                                   window_width,window_height,
                                   NULL,NULL,window_module,NULL);
 if DEE_UNLIKELY(!self->w_w32hwnd) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "CreateWindowA(%q,%s,%lu,%d,%d,%d,%d,NULL,NULL,%p,NULL) : %K",
                      _DeeWindow_ClassNameA,title,used_style,(int)adjusted_rect.left,
                      (int)adjusted_rect.top,window_width,window_height,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 if DEE_UNLIKELY(DeeWindow_SetAttr(self,DEE_WINDOW_SELFATTR,self) != 0) {
err_hwnd:
  if DEE_UNLIKELY(!DestroyWindow(self->w_w32hwnd)) SetLastError(0);
  return -1;
 }
 self->w_w32hdc = GetDC(self->w_w32hwnd);
 if (!GetClientRect(self->w_w32hwnd,&adjusted_rect)) {
  adjusted_rect.left = 0;
  adjusted_rect.top = 0;
  adjusted_rect.right = window_width;
  adjusted_rect.bottom = window_height;
 }
 DEE_LVERBOSE1("Created window %p: %Iu x %Iu\n",self->w_w32hwnd,
               (Dee_size_t)(adjusted_rect.right-adjusted_rect.left),
               (Dee_size_t)(adjusted_rect.bottom-adjusted_rect.top));
 if ((self->w_normal.nw_surface = DeeWindow_Win32CreateWindowSurface(
  self->w_w32hdc,(Dee_size_t)(adjusted_rect.right-adjusted_rect.left),
  (Dee_size_t)(adjusted_rect.bottom-adjusted_rect.top))) == NULL) {
err_dc:
  ReleaseDC(self->w_w32hwnd,self->w_w32hdc);
  goto err_hwnd;
 }
 if (icon && DEE_UNLIKELY(DeeWindow_SetIcon(self,icon) != 0)) {
  Dee_DECREF(self->w_normal.nw_surface);
  goto err_dc;
 }
 return 0;
}

void DeeWindow_Quit(DEE_A_IN struct DeeWindowObject *self) {
 ReleaseDC(self->w_w32hwnd,self->w_w32hdc);
 if (!DestroyWindow(self->w_w32hwnd)) SetLastError(0);
 Dee_DECREF(self->w_normal.nw_surface);
 _DeeWindow_Win32QuitClass();
}


DEE_A_RET_EXCEPT(-1) int DeeWindow_Update(DEE_A_INOUT struct DeeWindowObject *self) {
 struct DeeWindowSurfaceObject *windowsurface = self->w_normal.nw_surface;
 if (!BitBlt(self->w_w32hdc,0,0,
  (int)windowsurface->s_sizex,(int)windowsurface->s_sizey,
  windowsurface->nw_w32mdc,0,0,SRCCOPY)) {
  DeeError_SetStringf(&DeeErrorType_SystemError,
                      "BitBlt(%p,0,0,%d,%d,%p,0,0,SRCCOPY) : %K",
                      self->w_w32hdc,windowsurface->s_sizex,
                      windowsurface->s_sizey,windowsurface->nw_w32mdc,
                      DeeSystemError_Win32ToString(DeeSystemError_Win32Consume()));
  return -1;
 }
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeWindow_UpdateRects(
 DEE_A_INOUT struct DeeWindowObject *self, DEE_A_IN Dee_size_t rectc,
 DEE_A_IN_R(rectc) struct DeeWindowRect const *rectv) {
 (void)rectc,rectv;
 return DeeWindow_Update(self); // TODO
}



static LRESULT CALLBACK DeeWindow_Win32Main(
 HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
 DEE_LVERBOSE4("DeeWindow_Win32Main(%p,%u,%u,%Iu)\n",hWnd,msg,wParam,lParam);
 switch (msg) {
  case WM_SIZE:
   DEE_LVERBOSE1("WM_SIZE --> %d*%d\n",
                 LOWORD(lParam),HIWORD(lParam));
   break;
  default: break;
 }
 return CallWindowProcW(DefWindowProcW,hWnd,msg,wParam,lParam);
}

















static int DEE_CALL _deewindow_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeWindowObject *self, DeeObject *args) {
 Dee_ssize_t x = -1,y = -1; Dee_size_t w = 800,h = 600; struct DeeWindowRect rect;
 DeeObject *title = NULL; DeeSurfaceObject *icon = NULL; int error;
 Dee_windowflag_t flags = DEE_WINDOWFLAG_NORMAL|DEE_WINDOWFLAG_RESIZEABLE/*|DEE_WINDOWFLAG_FULLSCREEN*/;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"|IdIdIuIuooI32u:window",&x,&y,&w,&h,&title,&icon,&flags) != 0) return -1;
 if (icon && DEE_UNLIKELY(DeeError_TypeError_CheckType((DeeObject *)icon,(DeeTypeObject *)&DeeSurface_Type) != 0)) return -1;
 if (title && DEE_UNLIKELY((title = DeeString_Cast(title)) == NULL)) return -1;
 DeeWindowRect_Init(&rect,x,y,x+w,y+h);
 error = DeeWindow_Init(self,&rect,flags,
                        title ? DeeString_STR(title) : NULL,icon);
 Dee_XDECREF(title);
 return error;
}
static void DEE_CALL _deewindow_tp_dtor(DeeWindowObject *self) { DeeWindow_Quit(self); }
DEE_VISIT_PROC(_deewindow_tp_visit,DeeWindowObject *self) { Dee_VISIT(self->w_normal.nw_surface); }



static DeeWindowSurfaceObject *DEE_CALL _deewindow_surface_get(
 DeeWindowObject *self, void *DEE_UNUSED(closure)) {
 Dee_INCREF(self->w_normal.nw_surface);
 return self->w_normal.nw_surface;
}
static DeeObject *DEE_CALL _deewindow_minimized_get(
 DeeWindowObject *self, void *DEE_UNUSED(closure)) {
 DeeReturn_Bool(DeeWindow_IsMinimized(self));
}
static int DEE_CALL _deewindow_minimized_del(
 DeeWindowObject *self, void *DEE_UNUSED(closure)) {
 int error = DeeWindow_Restore(self);
 return error < 0 ? error : 0;
}
static int DEE_CALL _deewindow_minimized_set(
 DeeWindowObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 int new_state,error;
 if DEE_UNLIKELY((new_state = DeeObject_Bool(v)) < 0) return -1;
 error = new_state ? DeeWindow_Minimize(self) : DeeWindow_Restore(self);
 return error < 0 ? error : 0;
}
static DeeObject *DEE_CALL _deewindow_maximized_get(
 DeeWindowObject *self, void *DEE_UNUSED(closure)) {
 DeeReturn_Bool(DeeWindow_IsMaximized(self));
}
#define _deewindow_maximized_del _deewindow_minimized_del
static int DEE_CALL _deewindow_maximized_set(
 DeeWindowObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 int new_state,error;
 if DEE_UNLIKELY((new_state = DeeObject_Bool(v)) < 0) return -1;
 error = new_state ? DeeWindow_Maximize(self) : DeeWindow_Restore(self);
 return error < 0 ? error : 0;
}
static DeeObject *DEE_CALL _deewindow_visible_get(
 DeeWindowObject *self, void *DEE_UNUSED(closure)) {
 DeeReturn_Bool(DeeWindow_IsVisible(self));
}
static int DEE_CALL _deewindow_visible_del(
 DeeWindowObject *self, void *DEE_UNUSED(closure)) {
 int error = DeeWindow_Hide(self);
 return error < 0 ? error : 0;
}
static int DEE_CALL _deewindow_visible_set(
 DeeWindowObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 int new_state,error;
 if DEE_UNLIKELY((new_state = DeeObject_Bool(v)) < 0) return -1;
 error = new_state ? DeeWindow_Show(self) : DeeWindow_Hide(self);
 return error < 0 ? error : 0;
}
static DeeObject *DEE_CALL _deewindow_hidden_get(
 DeeWindowObject *self, void *DEE_UNUSED(closure)) {
 DeeReturn_Bool(DeeWindow_IsHidden(self));
}
static int DEE_CALL _deewindow_hidden_del(
 DeeWindowObject *self, void *DEE_UNUSED(closure)) {
 int error = DeeWindow_Show(self);
 return error < 0 ? error : 0;
}
static int DEE_CALL _deewindow_hidden_set(
 DeeWindowObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 int new_state,error;
 if DEE_UNLIKELY((new_state = DeeObject_Bool(v)) < 0) return -1;
 error = new_state ? DeeWindow_Hide(self) : DeeWindow_Show(self);
 return error < 0 ? error : 0;
}
static DeeObject *DEE_CALL _deewindow_resizeable_get(
 DeeWindowObject *self, void *DEE_UNUSED(closure)) {
 DeeReturn_Bool(DeeWindow_IsResizeable(self));
}
static int DEE_CALL _deewindow_resizeable_del(
 DeeWindowObject *self, void *DEE_UNUSED(closure)) {
 int error = DeeWindow_PreventResizeable(self);
 return error < 0 ? error : 0;
}
static int DEE_CALL _deewindow_resizeable_set(
 DeeWindowObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 int new_state,error;
 if DEE_UNLIKELY((new_state = DeeObject_Bool(v)) < 0) return -1;
 error = new_state ? DeeWindow_AllowResizeable(self) : DeeWindow_PreventResizeable(self);
 return error < 0 ? error : 0;
}
static DeeObject *DEE_CALL _deewindow_focused_get(
 DeeWindowObject *self, void *DEE_UNUSED(closure)) {
 DeeReturn_Bool(DeeWindow_IsFocused(self));
}
static int DEE_CALL _deewindow_focused_set(
 DeeWindowObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 int new_state,error;
 if DEE_UNLIKELY((new_state = DeeObject_Bool(v)) < 0) return -1;
 error = new_state ? DeeWindow_Focus(self) : 0;
 return error < 0 ? error : 0;
}
static DeeObject *DEE_CALL _deewindow_fullscreen_get(
 DeeWindowObject *self, void *DEE_UNUSED(closure)) {
 DeeReturn_Bool(DeeWindow_IsFullscreen(self));
}
static int DEE_CALL _deewindow_fullscreen_del(
 DeeWindowObject *self, void *DEE_UNUSED(closure)) {
 int error = DeeWindow_LeaveFullscreen(self);
 return error < 0 ? error : 0;
}
static int DEE_CALL _deewindow_fullscreen_set(
 DeeWindowObject *self, DeeObject *v, void *DEE_UNUSED(closure)) {
 int new_state,error;
 if DEE_UNLIKELY((new_state = DeeObject_Bool(v)) < 0) return -1;
 error = new_state ? DeeWindow_EnterFullscreen(self) : DeeWindow_LeaveFullscreen(self);
 return error < 0 ? error : 0;
}



static DeeObject *DEE_CALL _deewindow_refresh(
 DeeWindowObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":refresh") != 0) return NULL;
 if DEE_UNLIKELY(DeeWindow_Update(self) != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deewindow_pump(
 DeeWindowObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":pump") != 0) return NULL;
 if DEE_UNLIKELY(DeeWin32Window_PumpEvents() != 0) return NULL;
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deewindow_show(
 DeeWindowObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int error;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":show") != 0) return NULL;
 if DEE_UNLIKELY((error = DeeWindow_Show(self)) < 0) return NULL;
 DeeReturn_Bool(!error);
}
static DeeObject *DEE_CALL _deewindow_hide(
 DeeWindowObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int error;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":hide") != 0) return NULL;
 if DEE_UNLIKELY((error = DeeWindow_Hide(self)) < 0) return NULL;
 DeeReturn_Bool(!error);
}
static DeeObject *DEE_CALL _deewindow_minimize(
 DeeWindowObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int error;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":minimize") != 0) return NULL;
 if DEE_UNLIKELY((error = DeeWindow_Minimize(self)) < 0) return NULL;
 DeeReturn_Bool(!error);
}
static DeeObject *DEE_CALL _deewindow_maximize(
 DeeWindowObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int error;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":maximize") != 0) return NULL;
 if DEE_UNLIKELY((error = DeeWindow_Maximize(self)) < 0) return NULL;
 DeeReturn_Bool(!error);
}
static DeeObject *DEE_CALL _deewindow_restore(
 DeeWindowObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int error;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":restore") != 0) return NULL;
 if DEE_UNLIKELY((error = DeeWindow_Restore(self)) < 0) return NULL;
 DeeReturn_Bool(!error);
}
static DeeObject *DEE_CALL _deewindow_focus(
 DeeWindowObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 int error;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":focus") != 0) return NULL;
 if DEE_UNLIKELY((error = DeeWindow_Focus(self)) < 0) return NULL;
 DeeReturn_Bool(!error);
}


static struct DeeGetSetDef const _deewindow_tp_getsets[] = {
 DEE_GETSETDEF_v100("surface",member(&_deewindow_surface_get),null,null,"-> window_surface"),
 DEE_GETSETDEF_v100("minimized",member(&_deewindow_minimized_get),member(&_deewindow_minimized_del),member(&_deewindow_minimized_set),"-> bool"),
 DEE_GETSETDEF_v100("maximized",member(&_deewindow_maximized_get),member(&_deewindow_maximized_del),member(&_deewindow_maximized_set),"-> bool"),
 DEE_GETSETDEF_v100("visible",member(&_deewindow_visible_get),member(&_deewindow_visible_del),member(&_deewindow_visible_set),"-> bool"),
 DEE_GETSETDEF_v100("hidden",member(&_deewindow_hidden_get),member(&_deewindow_hidden_del),member(&_deewindow_hidden_set),"-> bool"),
 DEE_GETSETDEF_v100("resizeable",member(&_deewindow_resizeable_get),member(&_deewindow_resizeable_del),member(&_deewindow_resizeable_set),"-> bool"),
 DEE_GETSETDEF_v100("focused",member(&_deewindow_focused_get),null,member(&_deewindow_focused_set),"-> bool"),
 DEE_GETSETDEF_v100("fullscreen",member(&_deewindow_fullscreen_get),member(&_deewindow_fullscreen_del),member(&_deewindow_fullscreen_set),"-> bool"),
 DEE_GETSETDEF_END_v100
};
static struct DeeMethodDef const _deewindow_tp_methods[] = {
 DEE_METHODDEF_v100("refresh",member(&_deewindow_refresh),"() -> none"),
 DEE_METHODDEF_v100("pump",member(&_deewindow_pump),"() -> none"),
 DEE_METHODDEF_v100("show",member(&_deewindow_show),"() -> bool"),
 DEE_METHODDEF_v100("hide",member(&_deewindow_hide),"() -> bool"),
 DEE_METHODDEF_v100("minimize",member(&_deewindow_minimize),"() -> bool"),
 DEE_METHODDEF_v100("maximize",member(&_deewindow_maximize),"() -> bool"),
 DEE_METHODDEF_v100("restore",member(&_deewindow_restore),"() -> bool"),
 DEE_METHODDEF_v100("focus",member(&_deewindow_focus),"() -> bool"),
 DEE_METHODDEF_END_v100
};

DeeTypeObject DeeWindow_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("window"),null,member(DEE_TYPE_FLAG_NO_SUBCLASS),null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(
  sizeof(DeeWindowObject),null,null,null,null,
  member(&_deewindow_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,member(&_deewindow_tp_dtor)),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,member(&_deewindow_tp_visit)),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,
  null,member(_deewindow_tp_getsets),
  member(_deewindow_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_GFX_WINDOW_WINDOW_C */
