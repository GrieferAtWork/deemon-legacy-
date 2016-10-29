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
#include "xast.c"
#endif


DEE_DECL_BEGIN

//////////////////////////////////////////////////////////////////////////
// Optimizer Assumptions
//////////////////////////////////////////////////////////////////////////
DEE_A_RET_EXCEPT(-1) int DeeOptimizerAssumptions_AddVarAssumption(
 DEE_A_INOUT struct DeeOptimizerAssumptions *self, DEE_A_INOUT DeeLocalVarObject *local_var,
 DEE_A_IN Dee_uint32_t kind, DEE_A_INOUT DeeObject *arg) {
#ifndef __INTELLISENSE__
 DEE_STATIC_ASSERT(offsetof(struct DeeOptimizerAssumption,oa_vartype.ovta_var) ==
                   offsetof(struct DeeOptimizerAssumption,oa_varinit.ovia_var));
 DEE_STATIC_ASSERT(offsetof(struct DeeOptimizerAssumption,oa_vartype.ovta_type) ==
                   offsetof(struct DeeOptimizerAssumption,oa_varinit.ovia_init));
#endif
 struct DeeOptimizerAssumption *iter,*end; int result;
 struct DeeOptimizerAssumptions *assumptions_iter; DeeTypeObject *old_type;
 DEE_ASSERT(self);
 DEE_ASSERTF(kind == DEE_OPTIMIZER_ASSUMPTION_KIND_VARINIT ||
             kind == DEE_OPTIMIZER_ASSUMPTION_KIND_VARTYPE,
             "Invalid kind of assumption");
 DEE_ASSERT(DeeObject_Check(local_var) && DeeLocalVar_Check(local_var));
 DEE_ASSERT(DeeObject_Check(arg));
 DEE_ASSERTF(kind != DEE_OPTIMIZER_ASSUMPTION_KIND_VARTYPE || DeeType_Check(arg),
             "'DEE_OPTIMIZER_ASSUMPTION_KIND_VARTYPE' requires a type as argument");
 DEE_ASSERTF(kind != DEE_OPTIMIZER_ASSUMPTION_KIND_VARINIT ||
             DeeType_IsImmutable(Dee_TYPE(arg)) || DeeLocalVar_GET_USES(local_var) <= 1,
             "'DEE_OPTIMIZER_ASSUMPTION_KIND_VARINIT' requires an immutable object as argument, "
             "or needs the variable to not be used more than once");
 DEE_ASSERTF(DeeScope_SameWeakScope((DeeObject *)local_var->lv_scope,(DeeObject *)self->oas_scope),
             "The given variable originates from a different strong scope");
 if ((self->oas_flags&DEE_OPTIMIZER_ASSUMPTIONS_FLAG_LABEL)!=0 ||
     (local_var->lv_flags&DEE_LOCALVAR_FLAG_VOLATILE)!=0
     ) return DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_NOOP; // no-op
 // Search for some other mention of this variable
 assumptions_iter = self;
 result = DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_NOOP;
 while (1) {
  if ((assumptions_iter->oas_flags&DEE_OPTIMIZER_ASSUMPTIONS_FLAG_LABEL)==0) {
   end = (iter = assumptions_iter->oas_assumptionv)+assumptions_iter->oas_assumptionc;
   while (iter != end) {
    switch (iter->oa_kind) {
     case DEE_OPTIMIZER_ASSUMPTION_KIND_VARINIT:
      DEE_ASSERT(DeeObject_Check(iter->oa_varinit.ovia_init));
      if (iter->oa_varinit.ovia_var == local_var) {
       if (kind == DEE_OPTIMIZER_ASSUMPTION_KIND_VARINIT) {
        int init_matches;
        // Re-initialization:
        // >> local x;
        // >> x = 10;
        // >> x = 20; //<< here
        // If the values don't match, we have to turn
        // the init-assumptions into a type assumption.
        init_matches = DeeObject_DeepEquals(iter->oa_varinit.ovia_init,arg);
        if DEE_UNLIKELY(init_matches < 0) return init_matches;
        if DEE_UNLIKELY(init_matches == DEE_OBJECT_DEEPEQUALS_LIKELY)
         init_matches = DEE_OBJECT_DEEPEQUALS_FALSE; // We need to know for sure
        if (init_matches) {
         DEE_ASSERTF(result == DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_NOOP,
                     "Perfect match found after finding something else?");
         // The initialization matches, meaning that we're already done.
         // Note though, that the caller should probably delete this
         // vardecl, as it is simply redundant.
         return DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_MATCHOLD;
        }
        // The initializers didn't match.
        // If the previous assumption was made in our own scope, we
        // can simply override it. But if it wasn't we need to create
        // a new (more dominant) assumption in our own frame:
        // >> {
        // >>   local a;
        // >>   a = "foo";
        // >>   a = "bar"; // Previous assumption can be overwritten
        // >>   if (foo()) {
        // >>     a = "foobar"; // Create a new assumption in this sub-scope
        // >>     print a; // Here we know that a is "foobar"
        // >>   }
        // >>   print a; // Here we can still assume the type
        // >> }
        if (assumptions_iter == self) {
         // Override the previous assumption
         if (result == DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_NOOP)
          result = DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_DEGRADE;
         Dee_INCREF(arg);
         Dee_DECREF(iter->oa_varinit.ovia_init);
         iter->oa_varinit.ovia_init = arg;
        } else {
         if (result == DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_NOOP)
          result = DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_ADDED;
         // Create a new (more dominant) assumption
         // NOTE: Also need to degrade all previously made assumptions
         if (Dee_TYPE(iter->oa_varinit.ovia_var) == Dee_TYPE(arg)) {
          // Must degrade old assumption
          Dee_INCREF(Dee_TYPE(arg));
          Dee_DECREF(iter->oa_varinit.ovia_init);
          iter->oa_vartype.ovta_type = Dee_TYPE(arg);
          iter->oa_kind = DEE_OPTIMIZER_ASSUMPTION_KIND_VARTYPE;
         } else {
          // Different typing --> delete old assumption
          goto do_delete_old;
         }
        }
       } else {
        DEE_ASSERT(kind == DEE_OPTIMIZER_ASSUMPTION_KIND_VARTYPE);
        DEE_ASSERT(DeeObject_Check(iter->oa_varinit.ovia_init));
        old_type = Dee_TYPE(iter->oa_varinit.ovia_init);
        // Re-initialization, but can only deduce the new type of variable.
        // >> If the types miss-match
        if (assumptions_iter == self) {
         // The previous assumption exists in the current scope, meaning we can override it.
         if (old_type == (DeeTypeObject *)arg) goto degrade_old;
         // The types don't match, but since the previous assumption
         // was made in the same scope, we can simply override it: init --> type
         Dee_INCREF(arg);
         Dee_DECREF(iter->oa_varinit.ovia_init);
         iter->oa_vartype.ovta_type = old_type; // Inherit reference
         iter->oa_kind = DEE_OPTIMIZER_ASSUMPTION_KIND_VARTYPE;
         DEE_ASSERT(result == DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_NOOP);
         result = DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_REPLACE;
        } else {
         // The previous assumption exists outside the current scope,
         // meaning that from now on we can:
         //   - Either only assume the type (as long as they match)
         //   - Or have to delete the previous assumption, as it no longer applies
         if (old_type == (DeeTypeObject *)arg) {
          // The types match, meaning we can still assume that
          // by degrading the assumption to a type-only
degrade_old:
          DEE_ASSERT(result == DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_NOOP);
          result = DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_DEGRADE;
          Dee_INCREF(old_type);
          Dee_DECREF(iter->oa_varinit.ovia_init);
          Dee_INHERIT_REF(iter->oa_vartype.ovta_type,old_type);
          iter->oa_kind = DEE_OPTIMIZER_ASSUMPTION_KIND_VARTYPE;
         } else {
          if (result == DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_NOOP)
           result = DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_ADDED;
do_delete_old:
          // The types don't match, meaning the previous assumption is no longer valid at all
          _DeeOptimizerAssumption_Quit(iter);
          memmove(iter,iter+1,(Dee_size_t)((end-iter)-1)*
                  sizeof(struct DeeOptimizerAssumption));
          --assumptions_iter->oas_assumptionc;
         }
        } /* assumptions_iter != self */
       } /* kind != DEE_OPTIMIZER_ASSUMPTION_KIND_VARINIT */
      }
      break;

     case DEE_OPTIMIZER_ASSUMPTION_KIND_VARTYPE:
      DEE_ASSERT(DeeObject_Check(iter->oa_vartype.ovta_type) &&
                 DeeType_Check(iter->oa_vartype.ovta_type));
      if (iter->oa_vartype.ovta_var == local_var) {
       if (kind == DEE_OPTIMIZER_ASSUMPTION_KIND_VARINIT) {
        if (result == DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_NOOP)
         result = DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_ADDED;
        // Existing type assumption
        if (Dee_TYPE(arg) != iter->oa_vartype.ovta_type) {
         // Types didn't match, so we need to delete this assumption and create a new one
         goto do_delete_old;
        } else if (assumptions_iter == self) {
         // With the types matching, we need to override
         // the assumption if it was made in our scope.
         DEE_ASSERT(result == DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_NOOP);
         result = DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_DEGRADE;
         Dee_INCREF(arg);
         Dee_DECREF(iter->oa_vartype.ovta_type);
         iter->oa_kind = DEE_OPTIMIZER_ASSUMPTION_KIND_VARINIT;
         iter->oa_varinit.ovia_init = arg; // Inherit reference
        } else {
         // Types did match, but the assumption was made elsewhere.
         // >> So we need to create a more dominant one in our own scope
         if (result == DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_NOOP)
          result = DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_ADDED;
        }
       } else {
        DEE_ASSERT(kind == DEE_OPTIMIZER_ASSUMPTION_KIND_VARTYPE);
        DEE_ASSERT(DeeType_Check(arg)); // Reminder...
        if (iter->oa_vartype.ovta_type != (DeeTypeObject *)arg) {
         // Incorrect assumption (override it if it's local, or delete it)
         if (assumptions_iter == self) {
          // We can upgrade this assumption since it is local
          DEE_ASSERT(result == DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_NOOP);
          result = DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_UPGRADE;
          Dee_INCREF(arg);
          Dee_DECREF(iter->oa_vartype.ovta_type);
          iter->oa_vartype.ovta_type = (DeeTypeObject *)arg;
          DEE_ASSERT(iter->oa_kind == DEE_OPTIMIZER_ASSUMPTION_KIND_VARTYPE);
         } else {
          // Delete it, but add a new one later
          if (result == DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_NOOP)
           result = DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_ADDED;
          goto do_delete_old;
         }
        }
       }
      }
      break;

     default: break;
    }
    ++iter;
   }
  }
  if ((assumptions_iter->oas_flags&DEE_OPTIMIZER_ASSUMPTIONS_FLAG_STRONG)!=0) break;
  DEE_ASSERT(assumptions_iter->oas_prev);
  assumptions_iter = assumptions_iter->oas_prev;
 }
 if (result == DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_NOOP)
  result = DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_ADDED;
 else if (result != DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_ADDED)
  return result; // Don't add a new assumption
 // Add a new assumption
 while DEE_UNLIKELY((iter = (struct DeeOptimizerAssumption *)realloc_nz(
  self->oas_assumptionv,(self->oas_assumptionc+1)*sizeof(struct DeeOptimizerAssumption))) == NULL) {
  if DEE_LIKELY(Dee_CollectMemory()) continue;
  DeeError_NoMemory();
  return DEE_OPTIMIZER_ASSUMPTION_ADDVARASSUMPTION_ERROR;
 }
 self->oas_assumptionv = iter;
 iter += self->oas_assumptionc++;
 iter->oa_kind = kind;
 Dee_INCREF(iter->oa_varinit.ovia_var = local_var);
 Dee_INCREF(iter->oa_varinit.ovia_init = arg);
 return result;
}

DEE_A_RET_NOEXCEPT(NULL) DeeObject *DeeOptimizerAssumptions_GetAssumedVarInit(
 DEE_A_INOUT struct DeeOptimizerAssumptions *self,
 DEE_A_INOUT DeeLocalVarObject *local_var) {
 struct DeeOptimizerAssumption *iter,*end;
 struct DeeOptimizerAssumptions *assumptions_iter;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(local_var) && DeeLocalVar_Check(local_var));
#if 0
 DEE_ASSERTF(DeeScope_SameWeakScope((DeeObject *)local_var->lv_scope,(DeeObject *)self->oas_scope),
             "The given variable originates from a different strong scope");
#endif
 if ((self->oas_flags&DEE_OPTIMIZER_ASSUMPTIONS_FLAG_LABEL)!=0) return NULL;
 assumptions_iter = self;
 while (1) {
  if ((assumptions_iter->oas_flags&DEE_OPTIMIZER_ASSUMPTIONS_FLAG_LABEL)==0) {
   end = (iter = assumptions_iter->oas_assumptionv)+assumptions_iter->oas_assumptionc;
   while (iter != end) {
    if (iter->oa_kind == DEE_OPTIMIZER_ASSUMPTION_KIND_VARINIT &&
        iter->oa_varinit.ovia_var == local_var) return iter->oa_varinit.ovia_init;
    ++iter;
   }
  }
  if ((assumptions_iter->oas_flags&DEE_OPTIMIZER_ASSUMPTIONS_FLAG_STRONG)!=0) break;
  DEE_ASSERT(assumptions_iter->oas_prev);
  assumptions_iter = assumptions_iter->oas_prev;
 }
 return NULL;
}
DEE_A_RET_NOEXCEPT(NULL) DeeTypeObject *DeeOptimizerAssumptions_GetAssumedVarType(
 DEE_A_INOUT struct DeeOptimizerAssumptions *self,
 DEE_A_INOUT DeeLocalVarObject *local_var) {
 struct DeeOptimizerAssumption *iter,*end;
 struct DeeOptimizerAssumptions *assumptions_iter;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(local_var) && DeeLocalVar_Check(local_var));
#if 0
 DEE_ASSERTF(DeeScope_SameWeakScope((DeeObject *)local_var->lv_scope,(DeeObject *)self->oas_scope),
             "The given variable originates from a different strong scope");
#endif
 if ((self->oas_flags&DEE_OPTIMIZER_ASSUMPTIONS_FLAG_LABEL)!=0) return NULL;
 assumptions_iter = self;
 while (1) {
  if ((assumptions_iter->oas_flags&DEE_OPTIMIZER_ASSUMPTIONS_FLAG_LABEL)==0) {
   end = (iter = assumptions_iter->oas_assumptionv)+assumptions_iter->oas_assumptionc;
   while (iter != end) {
    switch (iter->oa_kind) {
     case DEE_OPTIMIZER_ASSUMPTION_KIND_VARINIT:
      if (iter->oa_varinit.ovia_var == local_var)
       return Dee_TYPE(iter->oa_varinit.ovia_init);
      break;
     case DEE_OPTIMIZER_ASSUMPTION_KIND_VARTYPE:
      if (iter->oa_vartype.ovta_var == local_var)
       return iter->oa_vartype.ovta_type;
      break;
    }
    ++iter;
   }
  }
  if ((assumptions_iter->oas_flags&DEE_OPTIMIZER_ASSUMPTIONS_FLAG_STRONG)!=0) break;
  DEE_ASSERT(assumptions_iter->oas_prev);
  assumptions_iter = assumptions_iter->oas_prev;
 }
 return NULL;
}

DEE_A_RET_EXCEPT(-1) int DeeOptimizerAssumptions_DelVarAssumptions(
 DEE_A_INOUT struct DeeOptimizerAssumptions *self,
 DEE_A_INOUT DeeLocalVarObject *local_var) {
 struct DeeOptimizerAssumption *iter,*end;
 struct DeeOptimizerAssumptions *assumptions_iter;
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(local_var) && DeeLocalVar_Check(local_var));
 DEE_ASSERTF(DeeScope_SameWeakScope((DeeObject *)local_var->lv_scope,(DeeObject *)self->oas_scope),
             "The given variable originates from a different strong scope");
 assumptions_iter = self;
 while (1) {
  end = (iter = assumptions_iter->oas_assumptionv)+assumptions_iter->oas_assumptionc;
  while (iter != end) {
   switch (iter->oa_kind) {
    case DEE_OPTIMIZER_ASSUMPTION_KIND_VARINIT:
     if (iter->oa_varinit.ovia_var == local_var) {
del_assumption:
      _DeeOptimizerAssumption_Quit(iter);
      memmove(iter,iter+1,(Dee_size_t)((end-iter)-1)*
              sizeof(struct DeeOptimizerAssumption));
      // todo: reallocate the memory?
      --assumptions_iter->oas_assumptionc;
     }
     break;
    case DEE_OPTIMIZER_ASSUMPTION_KIND_VARTYPE:
     if (iter->oa_vartype.ovta_var == local_var) goto del_assumption;
     break;
   }
   ++iter;
  }
  if ((assumptions_iter->oas_flags&DEE_OPTIMIZER_ASSUMPTIONS_FLAG_STRONG)!=0) break;
  DEE_ASSERT(assumptions_iter->oas_prev);
  assumptions_iter = assumptions_iter->oas_prev;
 }
 return 0;
}

DEE_A_RET_EXCEPT(-1) int DeeOptimizerAssumptions_AssumeExpr(
 DEE_A_INOUT struct DeeOptimizerAssumptions *self,
 DEE_A_INOUT DeeXAstObject *expr) {
 DEE_ASSERT(self);
 DEE_ASSERT(DeeObject_Check(expr) && DeeXAst_Check(expr));
 (void)self,expr; // TODO
 return 0;
}


DEE_DECL_END

