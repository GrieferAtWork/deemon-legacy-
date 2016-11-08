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
#ifndef GUARD_DEEMON_DEX_DEBUGGER_C
#define GUARD_DEEMON_DEX_DEBUGGER_C 1
#define DEE_EXTENSION 1

//@ dex.name              = _debugger
//@ dex.require           = <ncursesw/curses.h> ($ sudo apt-get install libncurses5-dev && sudo apt-get install libncursesw5-dev)
//@ dex.version_dependent = true

#include "_debugger.h"
#include <deemon/none.h>
#include <deemon/error.h>
#include <deemon/tuple.h>
#include <deemon/type.h>
#include <deemon/optional/type_slots.h>
#include <deemon/mp/mutex.h>
#include DEE_INCLUDE_MEMORY_API()

DEE_DECL_BEGIN


DEE_A_RET_EXCEPT(-1) int DeeDebugger_Init(
 DEE_A_OUT struct DeeDebugger *self, DEE_A_INOUT DeeThreadObject *thread_self,
 DEE_A_IN struct DeeStackFrame *top_frame) {
 DEE_ASSERT(DeeObject_Check(thread_self) && DeeThread_Check(thread_self));
 DEE_ASSERT(thread_self == DeeThread_SELF());
#ifdef DEE_DEBUG
 {
  struct DeeStackFrame *frame_iter;
  frame_iter = thread_self->t_frame_last;
  while (frame_iter && frame_iter != top_frame) frame_iter = frame_iter->f_prev;
  DEE_ASSERTF(frame_iter == top_frame,"The given frame isn't part of the thread-local stack");
 }
#endif
 if DEE_UNLIKELY(DeeCurses_AcquireContext(&self->d_context,DEE_CURSES_ACQUIRECONTEXT_FLAG_PUSHSTATE) != 0) return -1;
 if DEE_UNLIKELY((self->d_cwindow = newwin(0,0,0,0)) == NULL) {
  DeeCurses_ReleaseContext(&self->d_context);
  DeeError_SET_STRING(&DeeErrorType_SystemError,
                      "Failed to enter debugger: Curses could not be started");
  return -1;
 }
 Dee_INCREF(self->d_thread = thread_self);
 self->d_frame = top_frame;
 return 0;
}
void DeeDebugger_Quit(DEE_A_IN struct DeeDebugger *self) {
 delwin(self->d_cwindow);
 Dee_DECREF(self->d_thread);
 DeeCurses_ReleaseContext(&self->d_context);
}

struct _DeeDebugger_IterLocalsData {
 struct DeeDebugger *ild_debugger;
 int                 ild_line;
};
static int DEE_CALL _DeeDebugger_IterLocalsFunc(
 DEE_A_IN Dee_uint32_t uid, DEE_A_IN_OPT char const *name,
 DEE_A_IN_OPT DeeObject *DEE_UNUSED(value), struct _DeeDebugger_IterLocalsData *data) {
 mvwprintw(data->ild_debugger->d_cwindow,data->ild_line++,0,"%I32u - %s",uid,name);
 return 0;
}



//////////////////////////////////////////////////////////////////////////
// Run through the entire process of user-interaction through the debugger
DEE_A_RET_EXCEPT(-1) int DeeDebugger_Main(
 DEE_A_INOUT struct DeeDebugger *self) {
 noecho();
 nl();
 curs_set(0);
 wtimeout(self->d_cwindow,-1);
 keypad(self->d_cwindow,1);
 DeeDebugger_ModalLoop_Locals(self);
 return 0;
}

#define SELECTED_ENTRY_HEIGHT   3
#define UNSELECTED_ENTRY_HEIGHT 1

#define DeeDebugger_PrevFrame(self,frame) ((frame)->f_prev ? (frame)->f_prev : (frame))
static struct DeeStackFrame *DeeDebugger_NextFrame(
 struct DeeDebugger *self, struct DeeStackFrame *frame) {
 struct DeeStackFrame *iter;
 iter = self->d_thread->t_frame_last;
 if (frame != iter) while (1) {
  DEE_ASSERT(iter);
  if (iter->f_prev == frame) break;
  iter = iter->f_prev;
 }
 return iter;
}


void DeeDebugger_ModalLoop_Locals(
 DEE_A_INOUT struct DeeDebugger *self) {
 int ch; WINDOW *locals_window;
 DeeObject *location_text;
 Dee_size_t total_entries;
 Dee_size_t first_entry = 0;
 Dee_size_t selected_entry = 0;
 int max_display_entries;
 locals_window = subwin(self->d_cwindow,
                        getmaxy(self->d_cwindow)-8,
                        getmaxx(self->d_cwindow),4,0);
 max_display_entries = (int)(getmaxy(locals_window)-
                            (SELECTED_ENTRY_HEIGHT-UNSELECTED_ENTRY_HEIGHT));
 start_color();
 init_pair(1,COLOR_BLACK,COLOR_CYAN);
 init_pair(2,COLOR_BLACK,COLOR_WHITE);
 init_pair(3,COLOR_WHITE,COLOR_MAGENTA);
 wbkgd(locals_window,COLOR_PAIR(1));
 while (1) {
  wclear(self->d_cwindow);
  wclear(locals_window);
  //box(locals_window,'|','-');
  DeeDebugger_PrintLocals(locals_window,self->d_frame,
                          first_entry,selected_entry,
                          &total_entries,NULL);
  location_text = DeeString_Newf("%O(%I32d) : %O",
                                 _DeeStackFrame_File(self->d_frame),
                                 _DeeStackFrame_Line(self->d_frame)+1,
                                 _DeeStackFrame_Func(self->d_frame));
  mvwaddnstr(self->d_cwindow,0,0,DeeString_STR(location_text),(int)DeeString_SIZE(location_text));
  Dee_DECREF(location_text);
  wrefresh(locals_window);
  ch = wgetch(self->d_cwindow);

  switch (ch) {
   case KEY_LEFT:
    self->d_frame = DeeDebugger_PrevFrame(self,self->d_frame);
    break;
   case KEY_RIGHT:
    self->d_frame = DeeDebugger_NextFrame(self,self->d_frame);
    break;
   case KEY_DOWN:
    if (selected_entry+1 < total_entries) {
     if (++selected_entry >= first_entry+max_display_entries)
      first_entry = (selected_entry-max_display_entries)+1;
    }
    break;
   case KEY_UP:
    if (selected_entry && --selected_entry < first_entry) first_entry = selected_entry;
    break;
   case 'q': goto end;
   default: break;
  }
 }
end:
 delwin(locals_window);
}



struct print_locals_data {
 WINDOW    *wnd;
 Dee_size_t current_entry;
 Dee_size_t first_entry;
 Dee_size_t selected_entry;
 DeeVarUID  selected_uid;
};
static char const *uid_typename(DeeVarUID uid) {
 switch (DEE_STACKFRAME_VARUID_TYPE(uid)) {
  case DEE_STACKFRAME_VARTYPE_LOCALS: return "local";
  case DEE_STACKFRAME_VARTYPE_STATIC: return "static";
  case DEE_STACKFRAME_VARTYPE_ARGS  : return "<arg>";
  case DEE_STACKFRAME_VARTYPE_REFS  : return "<ref>";
  case DEE_STACKFRAME_VARTYPE_THIS  : return "this";
  default: break;
 }
 return "?";
}

static int DEE_CALL print_locals(
 DEE_A_IN DeeVarUID uid, DEE_A_IN_OPT char const *name,
 DEE_A_IN_OPT DeeObject *value, struct print_locals_data *data) {
 Dee_size_t idx,missing; int screen_line,screen_width,temp;
 if (data->current_entry >= data->first_entry) {
  DeeObject *formatted_line;
  idx = data->current_entry;
  screen_line = (int)((idx-data->first_entry)*UNSELECTED_ENTRY_HEIGHT);
  if (idx > data->selected_entry) screen_line += (SELECTED_ENTRY_HEIGHT-UNSELECTED_ENTRY_HEIGHT);
  if (screen_line < getmaxy(data->wnd)) {
   if (idx == data->selected_entry) {
    data->selected_uid = uid;
    wattron(data->wnd,COLOR_PAIR(2));
   }
   formatted_line = DeeString_Newf("%-20s | %-10s | 0x%-10I16X | %-20s",
                                   name,uid_typename(uid),DEE_STACKFRAME_VARUID_ID(uid),
                                   value ? DeeType_NAME(Dee_TYPE(value)) : "UNSET");
   if (!formatted_line) return -1;
   screen_width = getmaxx(data->wnd);
   mvwaddnstr(data->wnd,screen_line,0,DeeString_STR(formatted_line),
              (int)DeeString_SIZE(formatted_line));
   if (screen_width > (int)DeeString_SIZE(formatted_line)) {
    missing = (Dee_size_t)(screen_width-DeeString_SIZE(formatted_line));
    while (missing--) waddch(data->wnd,' ');
   }
   Dee_DECREF(formatted_line);
   if (idx == data->selected_entry) {
    wattroff(data->wnd,COLOR_PAIR(2));
    wattron(data->wnd,COLOR_PAIR(3));
    wmove(data->wnd,screen_line+1,0); temp = screen_width; while (temp--) waddch(data->wnd,' ');
    wmove(data->wnd,screen_line+2,0); temp = screen_width; while (temp--) waddch(data->wnd,' ');
    if (value) {
     if ((formatted_line = DeeObject_Repr(value)) == NULL) {
      if (!DeeError_GetOccurred(&formatted_line,NULL)) goto no_value;
      if (!formatted_line) goto no_value;
      Dee_INCREF(formatted_line);
      DeeError_Handled();
     }
     mvwaddnstr(data->wnd,screen_line+1,0,DeeString_STR(formatted_line),
               (int)DeeString_SIZE(formatted_line));
     Dee_DECREF(formatted_line);

    }
no_value:
    wattroff(data->wnd,COLOR_PAIR(3));
   }
  }
 }
 ++data->current_entry;
 return 0;
}


void DeeDebugger_PrintLocals(
 DEE_A_IN WINDOW *wnd, DEE_A_IN struct DeeStackFrame *frame,
 DEE_A_IN Dee_size_t first_entry, DEE_A_IN Dee_size_t selected_entry,
 DEE_A_OUT_OPT Dee_size_t *total_entries, DEE_A_OUT_OPT DeeVarUID *selected_uid) {
 struct print_locals_data data;
 data.wnd = wnd;
 data.current_entry = 0;
 data.first_entry = first_entry;
 data.selected_entry = selected_entry;
 data.selected_uid = 0;
 if (DeeStackFrame_EnumLocals(frame,(DeeStackFrame_LocalsIterFunc)
  &print_locals,&data,total_entries) != 0) { DeeError_Print(NULL,1); return; }
 if (selected_uid) *selected_uid = data.selected_uid;
}






DEE_A_RET_EXCEPT(-1) int DEE_CALL DeeDebugger_Run(
 DEE_A_INOUT DeeThreadObject *thread_self,
 DEE_A_INOUT struct DeeStackFrame *frame) {
 struct DeeDebugger debugger; int result;
 DeeCurses_LOAD_API({ return -1; });
 if (DeeDebugger_Init(&debugger,thread_self,frame) != 0) return -1;
 result = DeeDebugger_Main(&debugger);
 DeeDebugger_Quit(&debugger);
 return result;
}



static DeeObject *_debugger_breakpoint(DeeObject *args) {
 DeeThreadObject *thread_self;
 if (DeeTuple_Unpack(args,":breakpoint") != 0) return NULL;
 if ((thread_self = DeeThread_SELF()) == NULL) return NULL;
 if (!thread_self->t_frame_last) {
  DeeError_SET_STRING(&DeeErrorType_ValueError,
                      "Failed to enter debugger: No execution frame found");
  return NULL;
 }
 if (DeeDebugger_Run(thread_self,thread_self->t_frame_last) != 0) return NULL;
 DeeReturn_None;
}


//////////////////////////////////////////////////////////////////////////
// Module exports
struct DeeDexExportDef DeeDex_Exports[] = {
 DeeDex_EXPORT_FUNCTION("breakpoint",&_debugger_breakpoint,"breakpoint() -> none"),
 DeeDex_EXPORT_END
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DEBUGGER_C */
