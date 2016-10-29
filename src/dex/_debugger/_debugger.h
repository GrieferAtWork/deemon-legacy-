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
#ifndef GUARD_DEEMON_DEX_DEBUGGER_H
#define GUARD_DEEMON_DEX_DEBUGGER_H 1
 
#include <deemon/dex.h>
#include <deemon/mp/thread.h>
#include <deemon/runtime/execute.h>
#include <deemon/compiler/code.h>

#include "../_curses/_curses.h"

DEE_DECL_BEGIN



struct DeeDebugger {
 // NOTE: While initialized, this structure holds a symbolic lock to '_DeeDebugger_GlobalLock'.
 struct DeeCursesContext    d_context; /*< Curses context (also used as lock to only have one debugger). */
 DEE_A_REF DeeThreadObject *d_thread;  /*< [1..1] The thread currently being debugged. */
 struct DeeStackFrame      *d_frame;   /*< [1..1] The stackframe currently being inspected. */
 WINDOW                    *d_cwindow; /*< [1..1] The Curses window. */
};

extern DEE_A_RET_EXCEPT(-1) int DeeDebugger_Init(
 DEE_A_OUT struct DeeDebugger *self, DEE_A_INOUT DeeThreadObject *thread_self,
 DEE_A_IN struct DeeStackFrame *top_frame);
extern void DeeDebugger_Quit(DEE_A_IN struct DeeDebugger *self);


//////////////////////////////////////////////////////////////////////////
// Run through the entire process of user-interaction through the debugger
extern DEE_A_RET_EXCEPT(-1) int DeeDebugger_Main(DEE_A_INOUT struct DeeDebugger *self);


extern void DeeDebugger_ModalLoop_Locals(
 DEE_A_INOUT struct DeeDebugger *self);

extern void DeeDebugger_PrintLocals(
 DEE_A_IN WINDOW *wnd, DEE_A_IN struct DeeStackFrame *frame,
 DEE_A_IN Dee_size_t first_entry, DEE_A_IN Dee_size_t selected_entry,
 DEE_A_OUT_OPT Dee_size_t *total_entries, DEE_A_OUT_OPT Dee_uint32_t *selected_uid);


#ifdef GUARD_DEEMON_DEX_DEBUGGER_C
extern DEE_ATTRIBUTE_DLLEXPORT DEE_A_RET_EXCEPT(-1) int DEE_CALL DeeDebugger_Run(
 DEE_A_INOUT DeeThreadObject *thread_self, DEE_A_INOUT struct DeeStackFrame *frame);
#else
extern DEE_ATTRIBUTE_DLLIMPORT DEE_A_RET_EXCEPT(-1) int DEE_CALL DeeDebugger_Run(
 DEE_A_INOUT DeeThreadObject *thread_self, DEE_A_INOUT struct DeeStackFrame *frame);
#endif

DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DEBUGGER_H */
