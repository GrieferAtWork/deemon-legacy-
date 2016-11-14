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
#ifndef GUARD_DEEMON_DEX_GFX_WINDOW_WINDOW_H
#define GUARD_DEEMON_DEX_GFX_WINDOW_WINDOW_H 1
 
#include <deemon/__conf.inl>
#include <dex/_gfx/_gfx.h>
#include <deemon/error.h>
#include <deemon/optional/string_forward.h>

#ifdef DEE_PLATFORM_WINDOWS
DEE_COMPILER_MSVC_WARNING_PUSH(4201 4820 4255 4668)
#include <Windows.h>
DEE_COMPILER_MSVC_WARNING_POP
#endif

DEE_DECL_BEGIN

#ifdef DEE_PLATFORM_WINDOWS
#define DeeWindowPCoord LONG
#define DeeWindowSCoord LONG

struct DeeWindowPos { POINT w32_pos; };
#define DeeWindowPos_Init(ob,x_,y_) \
 ((ob)->w32_pos.x = (x_),(ob)->w32_pos.y = (y_),(void)0)
#define DeeWindowPos_INIT(x,y) {{x,y}}
#define DeeWindowPos_X(ob) (ob)->w32_pos.x
#define DeeWindowPos_Y(ob) (ob)->w32_pos.y

struct DeeWindowSize { POINT w32_size; };
#define DeeWindowSize_Init(ob,sx,sy) \
 ((ob)->w32_size.x = (sx),(ob)->w32_size.x = (sy),(void)0)
#define DeeWindowSize_INIT(sx,sy) {{sx,sy}}
#define DeeWindowSize_SX(ob) (ob)->w32_size.x
#define DeeWindowSize_SY(ob) (ob)->w32_size.y

struct DeeWindowRect { RECT w32_rect; };
#define DeeWindowRect_Init(ob,xbegin,ybegin,xend,yend) \
 ((ob)->w32_rect.left = (LONG)(xbegin),(ob)->w32_rect.top = (LONG)(ybegin)\
 ,(ob)->w32_rect.right = (LONG)(xend),(ob)->w32_rect.bottom = (LONG)(xend),(void)0)
#define DeeWindowRect_INIT(xbegin,ybegin,xend,yend) {{(LONG)(xbegin),(LONG)(ybegin),(LONG)(xend),(LONG)(yend)}}
#define DeeWindowRect_XBEGIN(ob) (ob)->w32_rect.left
#define DeeWindowRect_YBEGIN(ob) (ob)->w32_rect.top
#define DeeWindowRect_XEND(ob)   (ob)->w32_rect.right
#define DeeWindowRect_YEND(ob)   (ob)->w32_rect.bottom
#define DeeWindowRect_SIZEX(ob)  ((ob)->w32_rect.right-(ob)->w32_rect.left)
#define DeeWindowRect_SIZEY(ob)  ((ob)->w32_rect.bottom-(ob)->w32_rect.top)
#endif /* DEE_PLATFORM_WINDOWS */


#ifdef DEE_PLATFORM_WINDOWS
#define DeeWin32Window_SetWindowLong(hwnd,id,v)   (SetLastError(0),(SetWindowLongW(hwnd,id,v) != 0) || GetLastError() == 0)
#define DeeWin32Window_GetStyle(hwnd)              GetWindowLongW(hwnd,GWL_STYLE)
#define DeeWin32Window_IsMinimized(hwnd)         ((DeeWin32Window_GetStyle(hwnd)&(WS_MINIMIZE))!=0)
#define DeeWin32Window_IsMaximized(hwnd)         ((DeeWin32Window_GetStyle(hwnd)&(WS_MAXIMIZE))!=0)
#define DeeWin32Window_IsVisible(hwnd)           ((DeeWin32Window_GetStyle(hwnd)&(WS_VISIBLE))!=0)
#define DeeWin32Window_IsResizeable(hwnd)        ((DeeWin32Window_GetStyle(hwnd)&(WS_THICKFRAME))!=0)
#define DeeWin32Window_IsFocused(hwnd)           ((hwnd) == GetFocus())
#define DeeWin32Window_IsFullscreen(hwnd)        ((DeeWin32Window_GetStyle(hwnd)&(WS_OVERLAPPEDWINDOW))==0)
#endif /* DEE_PLATFORM_WINDOWS */

#ifdef DEE_PLATFORM_WINDOWS
extern DEE_A_RET_WUNUSED BOOL DeeWin32Window_TryGetPos(DEE_A_IN HWND hwnd, DEE_A_OUT struct DeeWindowPos *result);
extern DEE_A_RET_WUNUSED BOOL DeeWin32Window_TryGetSize(DEE_A_IN HWND hwnd, DEE_A_OUT struct DeeWindowSize *result);
#define DeeWin32Window_TrySetPos(hwnd,pos)                SetWindowPos(hwnd,NULL,(pos)->w32wp_x,(pos)->w32wp_y,0,0,SWP_NOSIZE|SWP_NOZORDER)
#define DeeWin32Window_TrySetSize(hwnd,size)              SetWindowPos(hwnd,NULL,0,0,(size)->w32ws_x,(size)->w32ws_y,SWP_NOMOVE|SWP_NOZORDER)
#define DeeWin32Window_TryGetRect(hwnd,rect)              GetWindowRect(hwnd,(LPRECT)(rect))
#define DeeWin32Window_TrySetRect(hwnd,rect)              SetWindowPos(hwnd,NULL,(rect)->w32wa_xmin,(rect)->w32wa_ymin,DeeWindowRect_SIZEX(rect),DeeWindowRect_SIZEY(rect),SWP_NOZORDER)
#define DeeWin32Window_TryShow(hwnd)                      ShowWindow(hwnd,SW_SHOW)
#define DeeWin32Window_TryHide(hwnd)                      ShowWindow(hwnd,SW_HIDE)
#define DeeWin32Window_TryRaise(hwnd)                     SetWindowPos(hwnd,DeeWin32Window_IsFullscreen(ob) ? HWND_TOPMOST : HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE)
#define DeeWin32Window_TryMinimize(hwnd)                  ShowWindow(hwnd,SW_MINIMIZE)
#define DeeWin32Window_TryMaximize(hwnd)                  ShowWindow(hwnd,SW_MAXIMIZE)
#define DeeWin32Window_TryRestore(hwnd)                   ShowWindow(hwnd,SW_NORMAL)
#define DeeWin32Window_TryFocus(hwnd)                     SetFocus(hwnd)
extern DEE_A_RET_WUNUSED BOOL _DeeWin32Window_TryEnterFullscreen_impl(DEE_A_IN HWND hwnd);
#define DeeWin32Window_TryEnterFullscreen(hwnd,oldplace) \
 (GetWindowPlacement(hwnd,oldplace) && _DeeWin32Window_TryEnterFullscreen_impl(hwnd))
#define DeeWin32Window_TryLeaveFullscreen(hwnd,oldplace) \
 (DeeWin32Window_SetWindowLong(hwnd,GWL_STYLE,DeeWin32Window_GetStyle(hwnd)|WS_OVERLAPPEDWINDOW) && \
 (SetWindowPlacement(hwnd,oldplace) ? TRUE : (SetWindowLongW(hwnd,GWL_STYLE,\
  DeeWin32Window_GetStyle(hwnd)&~(WS_OVERLAPPEDWINDOW)),FALSE)) && \
  SetWindowPos(hwnd,NULL,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|\
               SWP_NOZORDER|SWP_NOOWNERZORDER|SWP_FRAMECHANGED))
#define DeeWin32Window_TryFlip(hwnd)                      UpdateWindow(hwnd)
#define DeeWin32Window_TryGetAttr(hwnd,attr)     ((void *)GetPropA(hwnd,attr))
#define DeeWin32Window_TrySetAttr(hwnd,attr,v)            SetPropA(hwnd,attr,(HANDLE)(v))
#define DeeWin32Window_TryUtf8SetTitle(hwnd,title)        SetWindowTextA(hwnd,title)
#define DeeWin32Window_TryWideSetTitle(hwnd,title)        SetWindowTextW(hwnd,title)

#define DeeWin32Window_SetWindowLong(hwnd,id,v)   (SetLastError(0),(SetWindowLongW(hwnd,id,v) != 0) || GetLastError() == 0)
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_SetWindowStyle(DEE_A_IN HWND hwnd, DEE_A_IN LONG value);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_GetRect(DEE_A_IN HWND hwnd, DEE_A_OUT struct DeeWindowRect *result);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_GetPos(DEE_A_IN HWND hwnd, DEE_A_OUT struct DeeWindowPos *result);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_GetSize(DEE_A_IN HWND hwnd, DEE_A_OUT struct DeeWindowSize *result);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_SetRect(DEE_A_IN HWND hwnd, DEE_A_IN struct DeeWindowRect const *rect);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_SetPos(DEE_A_IN HWND hwnd, DEE_A_IN struct DeeWindowPos const *pos);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_SetSize(DEE_A_IN HWND hwnd, DEE_A_IN struct DeeWindowSize const *size);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Show(DEE_A_IN HWND hwnd);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Hide(DEE_A_IN HWND hwnd);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Raise(DEE_A_IN HWND hwnd);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Minimize(DEE_A_IN HWND hwnd);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Maximize(DEE_A_IN HWND hwnd);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Restore(DEE_A_IN HWND hwnd);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Focus(DEE_A_IN HWND hwnd);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_EnterFullscreen(DEE_A_IN HWND hwnd, DEE_A_OUT WINDOWPLACEMENT *oldplace);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_LeaveFullscreen(DEE_A_IN HWND hwnd, DEE_A_IN WINDOWPLACEMENT const *oldplace);
#define DeeWin32Window_GetAttr(hwnd,attr,result) (*(result)=DeeWin32Window_TryGetAttr(hwnd,attr),0)
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_SetAttr(DEE_A_IN HWND hwnd, DEE_A_IN_Z char const *attr, DEE_A_IN_OPT void *value);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Flip(DEE_A_IN HWND hwnd);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeWin32Window_Utf8GetTitle(DEE_A_IN HWND hwnd);
extern DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) *DeeWin32Window_WideGetTitle(DEE_A_IN HWND hwnd);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_Utf8SetTitle(DEE_A_IN HWND hwnd, DEE_A_IN_Z Dee_Utf8Char const *title);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_WideSetTitle(DEE_A_IN HWND hwnd, DEE_A_IN_Z Dee_WideChar const *title);
extern DEE_A_RET_EXCEPT(-1) int DeeWin32Window_SetIcon(DEE_A_IN HWND hwnd, DEE_A_IN DeeSurfaceObject const *icon);
#endif /* DEE_PLATFORM_WINDOWS */




typedef Dee_uint32_t Dee_windowflag_t;
#define DEE_WINDOWFLAG_NONE       DEE_UINT32_C(0x00000000) /*< Regular, non-special window. */
#define DEE_WINDOWFLAG_NORMAL     DEE_WINDOWFLAG_NONE      /*< Normal window. */
#define DEE_WINDOWFLAG_OPENGL     DEE_UINT32_C(0x00000001) /*< OpenGL based window. */
#define DEE_WINDOWFLAG_HIDDEN     DEE_UINT32_C(0x00000002) /*< Window isn't visible. */
#define DEE_WINDOWFLAG_RESIZEABLE DEE_UINT32_C(0x00000004) /*< Window can be resized. */
#define DEE_WINDOWFLAG_MINIMIZED  DEE_UINT32_C(0x00000008) /*< Window is minimized. */
#define DEE_WINDOWFLAG_MAXIMIZED  DEE_UINT32_C(0x00000010) /*< Window is maximized. */
#define DEE_WINDOWFLAG_FULLSCREEN DEE_UINT32_C(0x00000030) /*< Window is fullscreen (implies maximized). */
#define DEE_WINDOWFLAG_FOCUSED    DEE_UINT32_C(0x00000040) /*< Window is focused. */

#define DEE_WINDOWFLAG_ISNORMAL(flags)     (((flags)&DEE_WINDOWFLAG_OPENGL)==DEE_WINDOWFLAG_NORMAL)
#define DEE_WINDOWFLAG_ISOPENGL(flags)     (((flags)&DEE_WINDOWFLAG_OPENGL)==DEE_WINDOWFLAG_OPENGL)
#define DEE_WINDOWFLAG_ISVISIBLE(flags)    (((flags)&DEE_WINDOWFLAG_HIDDEN)==DEE_WINDOWFLAG_NORMAL)
#define DEE_WINDOWFLAG_ISHIDDEN(flags)     (((flags)&DEE_WINDOWFLAG_HIDDEN)==DEE_WINDOWFLAG_HIDDEN)
#define DEE_WINDOWFLAG_ISRESIZEABLE(flags) (((flags)&DEE_WINDOWFLAG_RESIZEABLE)==DEE_WINDOWFLAG_RESIZEABLE)
#define DEE_WINDOWFLAG_ISMOVEABLE(flags)   (((flags)&(DEE_WINDOWFLAG_MINIMIZED|DEE_WINDOWFLAG_MAXIMIZED))==DEE_WINDOWFLAG_NORMAL)
#define DEE_WINDOWFLAG_ISMINIMIZED(flags)  (((flags)&(DEE_WINDOWFLAG_MINIMIZED|DEE_WINDOWFLAG_MAXIMIZED))==DEE_WINDOWFLAG_MINIMIZED)
#define DEE_WINDOWFLAG_ISMAXIMIZED(flags)  (((flags)&(DEE_WINDOWFLAG_MINIMIZED|DEE_WINDOWFLAG_MAXIMIZED))==DEE_WINDOWFLAG_MAXIMIZED)
#define DEE_WINDOWFLAG_ISFULLSCREEN(flags) (((flags)&DEE_WINDOWFLAG_FULLSCREEN)==DEE_WINDOWFLAG_FULLSCREEN)
#define DEE_WINDOWFLAG_ISFOCUSED(flags)    (((flags)&DEE_WINDOWFLAG_FOCUSED)==DEE_WINDOWFLAG_FOCUSED)


extern DeeSurfaceTypeObject DeeWindowSurface_Type;
#define DeeWindowSurface_CheckExact(ob) DeeObject_InstanceOfExact(ob,(DeeTypeObject *)&DeeWindowSurface_Type)
#define DeeWindowSurface_Check          DeeWindowSurface_CheckExact
struct DeeWindowSurfaceObject {
#define DeeWindowSurface_PIXELADDR(self,x,y) \
 (void *)((self)->ws_pixels+(x)*(self)->ws_pixlsize+\
                            (y)*(self)->ws_scanline)
 DEE_SURFACE_HEAD
#define ws_pixlsize ws_spec.st_pixelbytes /*< Amount of bytes per pixel. */
 Dee_size_t                 ws_scanline;  /*< Amount of bytes per scanline (== s_sizex*s_pixlsize). */
 struct DeeSurfacePixelSpec ws_spec;      /*< Pixel-specs. */
 Dee_uint8_t               *ws_pixels;    /*< [1..1] Actual, writable pixel-data. */
 // This would have been so much simpler if windows allowed
 // the user to specify the address of the pixeldata...
 HDC                        nw_w32mdc;
 HGDIOBJ                    nw_w32hbm;
};
struct DeeNormalWindowData {
 DEE_A_REF struct DeeWindowSurfaceObject *nw_surface; /*< [1..1] Surface surface. */
           HDC                            nw_w32hdc;
};
struct DeeOpenGLWindowData { int placeholder; };
struct DeeWindow {
#ifdef DEE_PLATFORM_WINDOWS
 HWND             w_w32hwnd;     /*< Window HWND handle. */
 WINDOWPLACEMENT  w_w32oldplace; /*< Old window placement (before fullscreen was entered). */
#endif /* DEE_PLATFORM_WINDOWS */
 Dee_windowflag_t w_flags;       /*< Active window flags. */
 union{
  struct DeeNormalWindowData w_normal; /*< Normal window data. */
  struct DeeOpenGLWindowData w_opengl; /*< OpenGL window data. */
 };
};

#ifdef DEE_PLATFORM_WINDOWS
extern struct DeeWindowSurfaceObject *DeeWindow_Win32CreateWindowSurface(
 struct DeeWindow *self, Dee_size_t sx, Dee_size_t sy);
#endif

//////////////////////////////////////////////////////////////////////////
// Special attribute: Set to a pointer to the window's own data structure
#define DEE_WINDOW_SELFATTR  "_dself"


#define DeeWindow_Surface(ob)   ((DeeSurfaceObject *)(ob)->w_normal.nw_surface)
#ifdef DEE_PLATFORM_WINDOWS
#define DeeWindow_Win32HWND(ob) ((ob)->w_w32hwnd)
#endif /* DEE_PLATFORM_WINDOWS */

#ifdef DEE_PLATFORM_WINDOWS
#define /* DEE_A_RET_NOEXCEPT(0) */DeeWindow_DoTryShow(ob)            DeeWin32Window_TryShow(DeeWindow_Win32HWND(ob))
#define /* DEE_A_RET_NOEXCEPT(0) */DeeWindow_DoTryHide(ob)            DeeWin32Window_TryHide(DeeWindow_Win32HWND(ob))
#define /* DEE_A_RET_NOEXCEPT(0) */DeeWindow_DoTryMinimize(ob)        DeeWin32Window_TryMinimize(DeeWindow_Win32HWND(ob))
#define /* DEE_A_RET_NOEXCEPT(0) */DeeWindow_DoTryMaximize(ob)        DeeWin32Window_TryMaximize(DeeWindow_Win32HWND(ob))
#define /* DEE_A_RET_NOEXCEPT(0) */DeeWindow_DoTryRestore(ob)         DeeWin32Window_TryRestore(DeeWindow_Win32HWND(ob))
#define /* DEE_A_RET_NOEXCEPT(0) */DeeWindow_DoTryFocus(ob)           DeeWin32Window_TryFocus(DeeWindow_Win32HWND(ob))
#define /* DEE_A_RET_NOEXCEPT(0) */DeeWindow_DoTryEnterFullscreen(ob) DeeWin32Window_TryEnterFullscreen(DeeWindow_Win32HWND(ob),&(ob)->w_w32oldplace)
#define /* DEE_A_RET_NOEXCEPT(0) */DeeWindow_DoTryLeaveFullscreen(ob) DeeWin32Window_TryLeaveFullscreen(DeeWindow_Win32HWND(ob),&(ob)->w_w32oldplace)
#define /* DEE_A_RET_EXCEPT(-1)  */DeeWindow_DoShow(ob)               DeeWin32Window_Show(DeeWindow_Win32HWND(ob))
#define /* DEE_A_RET_EXCEPT(-1)  */DeeWindow_DoHide(ob)               DeeWin32Window_Hide(DeeWindow_Win32HWND(ob))
#define /* DEE_A_RET_EXCEPT(-1)  */DeeWindow_DoMinimize(ob)           DeeWin32Window_Minimize(DeeWindow_Win32HWND(ob))
#define /* DEE_A_RET_EXCEPT(-1)  */DeeWindow_DoMaximize(ob)           DeeWin32Window_Maximize(DeeWindow_Win32HWND(ob))
#define /* DEE_A_RET_EXCEPT(-1)  */DeeWindow_DoRestore(ob)            DeeWin32Window_Restore(DeeWindow_Win32HWND(ob))
#define /* DEE_A_RET_EXCEPT(-1)  */DeeWindow_DoFocus(ob)              DeeWin32Window_Focus(DeeWindow_Win32HWND(ob))
#define /* DEE_A_RET_EXCEPT(-1)  */DeeWindow_DoEnterFullscreen(ob)    DeeWin32Window_EnterFullscreen(DeeWindow_Win32HWND(ob),&(ob)->w_w32oldplace)
#define /* DEE_A_RET_EXCEPT(-1)  */DeeWindow_DoLeaveFullscreen(ob)    DeeWin32Window_LeaveFullscreen(DeeWindow_Win32HWND(ob),&(ob)->w_w32oldplace)
#endif /* DEE_PLATFORM_WINDOWS */

#ifdef DEE_PLATFORM_WINDOWS
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TryGetPos(ob,pos)         DeeWin32Window_TryGetPos(DeeWindow_Win32HWND(ob),pos)
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TrySetPos(ob,pos)         DeeWin32Window_TrySetPos(DeeWindow_Win32HWND(ob),pos)
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TryGetSize(ob,size)       DeeWin32Window_TryGetSize(DeeWindow_Win32HWND(ob),size)
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TrySetSize(ob,size)       DeeWin32Window_TrySetSize(DeeWindow_Win32HWND(ob),size)
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TryGetRect(ob,rect)       DeeWin32Window_TryGetRect(DeeWindow_Win32HWND(ob),rect)
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TrySetRect(ob,rect)       DeeWin32Window_TrySetRect(DeeWindow_Win32HWND(ob),rect)
#endif /* DEE_PLATFORM_WINDOWS */
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TryShow(ob)            (((ob)->w_flags&DEE_WINDOWFLAG_HIDDEN)!=0 ? (DeeWindow_DoTryShow(ob) ? ((ob)->w_flags &= ~(DEE_WINDOWFLAG_HIDDEN),1) : 0) : 0)
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TryHide(ob)            (((ob)->w_flags&DEE_WINDOWFLAG_HIDDEN)==0 ? (DeeWindow_DoTryHide(ob) ? ((ob)->w_flags |=   DEE_WINDOWFLAG_HIDDEN ,1) : 0) : 0)
#ifdef DEE_PLATFORM_WINDOWS
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TryRaise(ob)              DeeWin32Window_TryRaise(DeeWindow_Win32HWND(ob))
#endif /* DEE_PLATFORM_WINDOWS */
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TryMinimize(ob)        (((ob)->w_flags&DEE_WINDOWFLAG_MINIMIZED)==0 ? ((ob)->w_flags = (DeeWindow_DoTryMinimize(ob) ? ((ob)->w_flags|DEE_WINDOWFLAG_MINIMIZED)&~(DEE_WINDOWFLAG_MAXIMIZED),1) : 0) : 0)
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TryMaximize(ob)        (((ob)->w_flags&DEE_WINDOWFLAG_MAXIMIZED)==0 ? ((ob)->w_flags = (DeeWindow_DoTryMaximize(ob) ? ((ob)->w_flags|DEE_WINDOWFLAG_MAXIMIZED)&~(DEE_WINDOWFLAG_MINIMIZED),1) : 0) : 0)
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TryRestore(ob)         (((ob)->w_flags&(DEE_WINDOWFLAG_MINIMIZED|DEE_WINDOWFLAG_MAXIMIZED))!=0 ? (DeeWindow_DoTryRestore(ob) ? ((ob)->w_flags &= ~(DEE_WINDOWFLAG_MINIMIZED|DEE_WINDOWFLAG_MAXIMIZED),1) : 0) : 0)
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TryFocus(ob)           (((ob)->w_flags&DEE_WINDOWFLAG_FOCUSED)==0 ? (DeeWindow_DoTryFocus(ob) ? ((ob)->w_flags |= DEE_WINDOWFLAG_FOCUSED,1) : 0) : 0)
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TryEnterFullscreen(ob) (((ob)->w_flags&DEE_WINDOWFLAG_FULLSCREEN)==0 ? (DeeWindow_DoTryEnterFullscreen(ob) ? ((ob)->w_flags |=   DEE_WINDOWFLAG_FULLSCREEN ,1) : 0) : 0)
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TryLeaveFullscreen(ob) (((ob)->w_flags&DEE_WINDOWFLAG_FULLSCREEN)!=0 ? (DeeWindow_DoTryLeaveFullscreen(ob) ? ((ob)->w_flags &= ~(DEE_WINDOWFLAG_FULLSCREEN),1) : 0) : 0)
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_Flags(ob)                (ob)->w_flags
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_IsMinimized(ob)           DEE_WINDOWFLAG_ISMINIMIZED(DeeWindow_Flags(ob))
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_IsMaximized(ob)           DEE_WINDOWFLAG_ISMAXIMIZED(DeeWindow_Flags(ob))
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_IsVisible(ob)             DEE_WINDOWFLAG_ISVISIBLE(DeeWindow_Flags(ob))
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_IsHidden(ob)              DEE_WINDOWFLAG_ISHIDDEN(DeeWindow_Flags(ob))
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_IsResizeable(ob)          DEE_WINDOWFLAG_ISRESIZEABLE(DeeWindow_Flags(ob))
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_IsFocused(ob)             DEE_WINDOWFLAG_ISFOCUSED(DeeWindow_Flags(ob))
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_IsFullscreen(ob)          DEE_WINDOWFLAG_ISFULLSCREEN(DeeWindow_Flags(ob))
#ifdef DEE_PLATFORM_WINDOWS
#define /* DEE_A_RET_NOEXCEPT(NULL) */DeeWindow_TryGetAttr(ob,attr)       DeeWin32Window_TryGetAttr(DeeWindow_Win32HWND(ob),attr)
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TrySetAttr(ob,attr,v)     DeeWin32Window_TrySetAttr(DeeWindow_Win32HWND(ob),attr,v)
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TryUtf8SetTitle(ob,title) DeeWin32Window_TryUtf8SetTitle(DeeWindow_Win32HWND(ob),title)
#define /* DEE_A_RET_NOEXCEPT(0)    */DeeWindow_TryWideSetTitle(ob,title) DeeWin32Window_TryWideSetTitle(DeeWindow_Win32HWND(ob),title)
#endif /* DEE_PLATFORM_WINDOWS */

#ifdef DEE_PLATFORM_WINDOWS
#define /* DEE_A_RET_EXCEPT(-1) */DeeWindow_GetPos(ob,pos)          DeeWin32Window_GetPos(DeeWindow_Win32HWND(ob),pos)
#define /* DEE_A_RET_EXCEPT(-1) */DeeWindow_SetPos(ob,pos)          DeeWin32Window_SetPos(DeeWindow_Win32HWND(ob),pos)
#define /* DEE_A_RET_EXCEPT(-1) */DeeWindow_GetSize(ob,size)        DeeWin32Window_GetSize(DeeWindow_Win32HWND(ob),size)
#define /* DEE_A_RET_EXCEPT(-1) */DeeWindow_SetSize(ob,size)        DeeWin32Window_SetSize(DeeWindow_Win32HWND(ob),size)
#define /* DEE_A_RET_EXCEPT(-1) */DeeWindow_GetRect(ob,rect)        DeeWin32Window_GetRect(DeeWindow_Win32HWND(ob),rect)
#define /* DEE_A_RET_EXCEPT(-1) */DeeWindow_SetRect(ob,rect)        DeeWin32Window_SetRect(DeeWindow_Win32HWND(ob),rect)
#define /* DEE_A_RET_EXCEPT(-1) */DeeWindow_Raise(ob)               DeeWin32Window_Raise(DeeWindow_Win32HWND(ob))
#endif /* DEE_PLATFORM_WINDOWS */
extern DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_Show(DEE_A_INOUT struct DeeWindow *self);
extern DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_Hide(DEE_A_INOUT struct DeeWindow *self);
extern DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_Minimize(DEE_A_INOUT struct DeeWindow *self);
extern DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_Maximize(DEE_A_INOUT struct DeeWindow *self);
extern DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_Restore(DEE_A_INOUT struct DeeWindow *self);
extern DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_Focus(DEE_A_INOUT struct DeeWindow *self);
extern DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_EnterFullscreen(DEE_A_INOUT struct DeeWindow *self);
extern DEE_A_RET_EXCEPT_FAIL(-1,1) int DeeWindow_LeaveFullscreen(DEE_A_INOUT struct DeeWindow *self);
#ifdef DEE_PLATFORM_WINDOWS
#define /* DEE_A_RET_EXCEPT(-1) */DeeWindow_GetAttr(ob,attr,result) DeeWin32Window_GetAttr(DeeWindow_Win32HWND(ob),attr,result)
#define /* DEE_A_RET_EXCEPT(-1) */DeeWindow_SetAttr(ob,attr,v)      DeeWin32Window_SetAttr(DeeWindow_Win32HWND(ob),attr,v)
#define /* DEE_A_RET_OBJECT_EXCEPT_REF(DeeUtf8StringObject) */DeeWindow_Utf8GetTitle(ob) DeeWin32Window_Utf8GetTitle(DeeWindow_Win32HWND(ob))
#define /* DEE_A_RET_OBJECT_EXCEPT_REF(DeeWideStringObject) */DeeWindow_WideGetTitle(ob) DeeWin32Window_WideGetTitle(DeeWindow_Win32HWND(ob))
#define /* DEE_A_RET_EXCEPT(-1) */DeeWindow_Utf8SetTitle(ob,title)  DeeWin32Window_Utf8SetTitle(DeeWindow_Win32HWND(ob),title)
#define /* DEE_A_RET_EXCEPT(-1) */DeeWindow_WideSetTitle(ob,title)  DeeWin32Window_WideSetTitle(DeeWindow_Win32HWND(ob),title)
#define /* DEE_A_RET_EXCEPT(-1) */DeeWindow_SetIcon(ob,icon)        DeeWin32Window_SetIcon(DeeWindow_Win32HWND(ob),icon)
#endif /* DEE_PLATFORM_WINDOWS */


extern DEE_A_RET_EXCEPT(-1) int DeeWindow_Init(
 DEE_A_OUT struct DeeWindow *self, DEE_A_IN struct DeeWindowRect const *rect,
 DEE_A_IN Dee_windowflag_t flags, DEE_A_IN_Z_OPT char const *title,
 DEE_A_IN_OPT DeeSurfaceObject *icon);
extern void DeeWindow_Quit(DEE_A_IN struct DeeWindow *self);
extern DEE_A_RET_EXCEPT(-1) int DeeWindow_Update(DEE_A_INOUT struct DeeWindow *self);
extern DEE_A_RET_EXCEPT(-1) int DeeWindow_UpdateRects(
 DEE_A_INOUT struct DeeWindow *self, DEE_A_IN Dee_size_t rectc,
 DEE_A_IN_R(rectc) struct DeeWindowRect const *rectv);





struct DeeWindowObject {
 DEE_OBJECT_HEAD
 struct DeeWindow w_window; /*< Stored window object. */
};

extern DeeTypeObject DeeWindow_Type;

                                                           
DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_GFX_WINDOW_WINDOW_H */
