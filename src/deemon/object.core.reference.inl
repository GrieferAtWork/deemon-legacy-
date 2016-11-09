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
#include "object.c"
DEE_DECL_BEGIN
#endif


#undef Dee_IncRef
#undef Dee_DecRef
#undef Dee_XIncRef
#undef Dee_XDecRef
#undef Dee_WeakIncRef
#undef Dee_WeakDecRef
#undef Dee_WeakXIncRef
#undef Dee_WeakXDecRef
void Dee_IncRef(DEE_A_INOUT struct DeeObject *ob) { Dee_INCREF(ob); }
void Dee_DecRef(DEE_A_INOUT struct DeeObject *ob) { Dee_DECREF(ob); }
void Dee_XIncRef(DEE_A_INOUT_OPT struct DeeObject *ob) { Dee_XINCREF(ob); }
void Dee_XDecRef(DEE_A_INOUT_OPT struct DeeObject *ob) { Dee_XDECREF(ob); }
void Dee_WeakIncRef(DEE_A_INOUT struct DeeObject *ob) { Dee_WEAKINCREF(ob); }
void Dee_WeakDecRef(DEE_A_INOUT struct DeeObject *ob) { Dee_WEAKDECREF(ob); }
void Dee_WeakXIncRef(DEE_A_INOUT_OPT struct DeeObject *ob) { Dee_WEAKXINCREF(ob); }
void Dee_WeakXDecRef(DEE_A_INOUT_OPT struct DeeObject *ob) { Dee_WEAKXDECREF(ob); }

#ifdef DEE_DEBUG
static void _dee_warn_link_error(void) {
 DEE_ATOMIC_ONCE({
  DEE_LDEBUG("Link inconsistency: deemon was compiled in debug mode, "
             "but is loaded through release mode\n");
 });
}
#endif

#ifdef DEE_DEBUG
void _DeeObject_InternalDestroyWeak(
 DEE_A_INOUT DEE_A_OUT_INVALID struct DeeObject *ob) {
 _DeeObject_InternalDestroyWeak_d(ob,"??" "?",0);
}
void _DeeObject_InternalDestroyWeak_d(
 DEE_A_INOUT DEE_A_OUT_INVALID struct DeeObject *ob,
 DEE_A_IN_Z char const *file, DEE_A_IN int line)
#else
void _DeeObject_InternalDestroyWeak_d(
 DEE_A_INOUT DEE_A_OUT_INVALID struct DeeObject *ob,
 DEE_A_IN_Z char const *DEE_UNUSED(file), DEE_A_IN int DEE_UNUSED(line)) {
 _DeeObject_InternalDestroyWeak(ob);
}
void _DeeObject_InternalDestroyWeak(
 DEE_A_INOUT DEE_A_OUT_INVALID struct DeeObject *ob)
#endif
{
 DeeTypeObject *tp = Dee_TYPE(ob); // Inherit reference
 DEE_ASSERT(DeeObject_Check(tp) && DeeType_Check(tp));
 DEE_ASSERT(DeeType_GET_SLOT(tp,tp_free) && "No free function defined");
#ifdef DEE_DEBUG
 if (_DeeFlag_Verbose >= 4) _Dee_VerboseOut(
  "%s(%d) : Freeing Object %s:%p\n",
  file,line,DeeType_NAME(tp),ob);
#endif
 // Make sure no type is set while we're in the free function
 // >> enforced '!DeeObject_Check(ob)'
 ob->__ob_type = NULL;
 (*DeeType_GET_SLOT(tp,tp_free))(tp,ob);
 // Drop the reference to the type that we were still holding
 // NOTE: Custom free-function types are deallocated here as well.
 Dee_DECREF(tp);
}


DEE_A_RET_NOEXCEPT_REF struct DeeObject *
DeeObject_LockWeakref(DEE_A_INOUT struct DeeObject *self) {
 unsigned int v;
 // NOTE: DeeObject_Check(self) would fail if the object is dead
 //       So this check works around that fact and accepts dead objects
 do {
  DEE_ASSERT(self && DeeAtomicN_Load(DEE_TYPES_SIZEOF_WEAKCNT,self->__ob_weakcnt,memory_order_seq_cst) != 0
             // v can't check the type, as it's value might be inconsistent if 'v == 0'
             /*&& DeeObject_Check(Dee_TYPE(self))*/);
  v = _DeeRefCounter_Fetch(self->__ob_refcnt);
  if (!v) return NULL; // Dead object
 } while (!DeeAtomicN_CompareExchangeWeak(DEE_TYPES_SIZEOF_REFCNT,
  self->__ob_refcnt,v,v+1,memory_order_seq_cst,memory_order_seq_cst));
 // Do some sanity checks now that we've locked the object.
 DEE_ASSERT(DeeObject_Check(self) && DeeType_Check(Dee_TYPE(self)));
 return self;
}

#ifdef __INTELLISENSE__
DEE_DECL_END
#endif
