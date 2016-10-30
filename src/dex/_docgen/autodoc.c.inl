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
#ifndef GUARD_DEEMON_DEX_DOCGEN_AUTODOC_C_INL
#define GUARD_DEEMON_DEX_DOCGEN_AUTODOC_C_INL 1

#include "autodoc.h.inl"
#include <deemon/dict.h>
#include <deemon/string.h>
#include <deemon/optional/std/string.h>

DEE_DECL_BEGIN

#define DOC(name,doc)\
static char const name[] = doc
DOC(docfor_at,"(size_t idx) -> object\n"
    "@param idx: The index to look up\n"
    "@return: The idx'th object contained in @this\n"
    "@throws Error.ValueError.IndexError: @idx is out-of-bounds\n"
    "\tReturns the idx'th element of a given sequence, or throws an error if the index is out of bounds.");
DOC(docfor_back,"() -> object\n"
    "@return: The last element in @this\n"
    "@throws Error.ValueError: The given sequence is empty\n"
    "\tReturns the last element of a given sequence.");
DOC(docfor_count,"(object elem, callable pred = none) -> size_t\n"
    "@throws object: Same as calling @pred or #(operator ==)\n"
    "@param pred: User-defined predicate, #none or #__eq__\n"
    "@return: The amount of occurances found\n"
    "\tCounts the number of occurances of @elem in @(this).\n"
    "\tThe given predicate is called to compare two objects, where @elem will be the second argument."
    "\tIf @pred is #none or #__eq__, #(operator ==) will be used to compare objects.");
DOC(docfor_empty,"() -> bool\n"
    "@return: #true or #false if the sequence is empty\n"
    "\tReturns #true if @this sequence is empty; #false otherwise.");
DOC(docfor_find,"(object elem, callable pred = none) -> ssize_t\n"
    "@param pred: User-defined predicate, #none or #__eq__\n"
    "@throws object: Same as calling @pred or #(operator ==)\n"
    "@return: Index of the found object, where 0 references #this.front\n"
    "\tSearches for the first instance of @elem in @(this). If no such object exists, -1 is returned.\n"
    "\tThe given predicate is called to compare two objects, where @elem will be the second argument Once an occurance is found, iteration is aborted."
    "\tIf @pred is #none, #__eq__ or #(operator ==) will be used to compare objects.");
DOC(docfor_front,"() -> object\n"
    "@throws Error.ValueError: The given sequence is empty\n"
    "@return: The first element of the given sequence\n"
    "\tReturns the first element of a given sequence.\n"
    "\tIteration will abort after one element.");
DOC(docfor_index,"(object elem, callable pred = none) -> size_t\n"
    "@throws Error.ValueError: No object was identical to the specified @elem\n"
    "@param pred: User-defined predicate, #none or #__eq__\n"
    "@throws object: Same as calling @pred or #(operator ==)\n"
    "@return: The index of the found object, where 0 references #this.front\n"
    "\tSearches for the first instance of @elem in @(this).\n"
    "\tThe given predicate is called to compare two objects, where @elem will be the second argument"
    "\tOnce an occurance is found, iteration is aborted.\n"
    "\tIf @pred is #none or #__eq__, #(operator ==) will be used to compare objects.");
DOC(docfor_locate,"(callable pred) -> object\n"
    "@throws Error.ValueError: No object matching the given predicate was found\n"
    "@throws object: Same as @pred\n"
    "@param pred: User-defined predicate, #none or #__eq__\n"
    "@return: The first object satisfying #(true == pred(object))\n"
    "\tSearches the givens sequence for the first element conforming to @pred.");
DOC(docfor_locate_all,"(callable pred) -> iterable\n"
    "@throws object: Same as @pred\n"
    "@param pred: User-defined predicate, #none or #__eq__\n"
    "@return: A sequence of all objects from @this, satisfying #(true == pred(object))\n"
    "\tReturns an iterable object containing all elements from @this that satisfy the given predicate @(pred).\n"
    "\tCalling this function is equivalent to the following code:\n"
    "\t>> result = for (local elem: this) if (pred(elem)) elem;");
DOC(docfor_non_empty,"() -> bool\n"
    "@return: #true or #false if the sequence is not empty\n"
    "\tReturns #true if @this sequence is not empty; #false otherwise.");
DOC(docfor_reversed,"() -> iterable\n"
    "@return: An iterable object containing all elements from @this, but in a reversed order\n"
    "\tReturns an iterable object containing all elements from @this, but in a reversed order.");
DOC(docfor_rfind,"(object elem, callable pred = none) -> ssize_t\n"
    "@throws object: Same as calling @pred or #(operator ==)\n"
    "@param pred: User-defined predicate, #none or #__eq__\n"
    "@return: The index of the found object, where 0 references @(this.front())\n"
    "\tSearches for the last instance of @elem in @(this). If no such object exists, -1 is returned.\n"
    "\tThe given predicate is called to compare two objects, where @elem will be the second argument.\n"
    "\tIf @pred is #none or #__eq__, #(operator ==) will be used to compare objects.");
DOC(docfor_rindex,"(object elem, callable pred = none) -> size_t\n"
    "@throws Error.ValueError: No object was identical to the specified @elem\n"
    "@param pred: User-defined predicate, #none or #__eq__\n"
    "@throws object: Same as calling @pred or #(operator ==)\n"
    "@return: The index of the found object, where 0 references @(this.front())\n"
    "\tSearches for the last instance of @elem in @(this).\n"
    "\tThe given predicate is called to compare two objects, where @elem will be the second argument.\n"
    "\tIf @pred is #none or #__eq__, #(operator ==) will be used to compare objects.");
DOC(docfor_rlocate,"(callable pred) -> object\n"
    "@throws Error.ValueError: No object matching the given predicate was found\n"
    "@param pred: User-defined predicate, #none or #__eq__\n"
    "@throws object: Same as @pred\n"
    "@return: The last object satisfying #(true == pred(object))\n"
    "\tSearches the givens sequence for the last element conforming to @(pred).");
DOC(docfor_sorted,"(callable pred = none) -> iterable\n"
    "@throws object: Same as calling @pred or #(operator <)\n"
    "@param pred: User-defined predicate, #none or #__lo__\n"
    "@return: An interable object containing the same elements as @this, but in the order defined by @(pred).\n"
    "\tReturns an interable object, that is the sorted version of @(this).\n"
    "\tIf @pred is #none or #__lo__, #(operator <) will be used to compare objects\n"
    "\tThe sort order is ascending, meaning that the given predicate should return #true, if its first argument is lower than its second.");
DOC(docfor_transform,"(callable pred) -> iterable\n"
    "@throws object: Same as calling @pred\n"
    "@param pred: User-defined predicate, #none or #__eq__\n"
    "@return: An interable object containing the same elements as @this, with each being the result of calling @pred on the original\n"
    "\tReturns a transformed sequence containing the same elements as @this, but each trasformed by calling @pred with it.\n"
    "\tCalling this function is equivalent to the following code:\n"
    "\t>> result = for (local elem: this) pred(pred);");
DOC(docfor_segments,"(size_t n) -> iterable\n"
    "@return: A sequence of sequences, containing the elements of @this evenly spread over @n containers\n"
    "\tReturns an n-long sequence of iterable objects, that contain the elements from @this evenly speard out, with the last sequence possibly containing less than the others do.");
DOC(docfor_contains,"(object elem, callable pred = none) -> bool\n"
    "@throws object: Same as calling @pred or #(operator ==)\n"
    "@param elem: Object to search for\n"
    "@param pred: User-defined predicate, #none or #__eq__\n"
    "@return: #true or #false, indicating the existance of @elem inside of @this\n"
    "\tSearches for the any instance of @elem in @(this). If no such object exists, #false is returned, #true otherwise.\n"
    "\tThe given predicate is called to compare two objects, where @elem will be the second argument.\n"
    "\tOnce an occurance is found, iteration is aborted and #true is returned."
    "\tIf @pred is #none or #__eq__, #(operator ==) will be used to compare objects.");
#undef DOC


static DEE_A_RET_Z_OPT char const *
_DeeMemberName_GetDoc(DEE_A_IN_Z char const *member_name) {
 if (strcmp(member_name,"at") == 0) return docfor_at;
 if (strcmp(member_name,"back") == 0) return docfor_back;
 if (strcmp(member_name,"count") == 0) return docfor_count;
 if (strcmp(member_name,"empty") == 0) return docfor_empty;
 if (strcmp(member_name,"find") == 0) return docfor_find;
 if (strcmp(member_name,"front") == 0) return docfor_front;
 if (strcmp(member_name,"index") == 0) return docfor_index;
 if (strcmp(member_name,"locate") == 0) return docfor_locate;
 if (strcmp(member_name,"locate_all") == 0) return docfor_locate_all;
 if (strcmp(member_name,"non_empty") == 0) return docfor_non_empty;
 if (strcmp(member_name,"reversed") == 0) return docfor_reversed;
 if (strcmp(member_name,"rfind") == 0) return docfor_rfind;
 if (strcmp(member_name,"rindex") == 0) return docfor_rindex;
 if (strcmp(member_name,"rlocate") == 0) return docfor_rlocate;
 if (strcmp(member_name,"sorted") == 0) return docfor_sorted;
 if (strcmp(member_name,"transform") == 0) return docfor_transform;
 if (strcmp(member_name,"segments") == 0) return docfor_segments;
 if (strcmp(member_name,"contains") == 0) return docfor_contains;
#define DOC(func,doc) if (strcmp(member_name,func) == 0) return doc;
 // Builtin 'object' operators
 DOC("__deepcopy__","() -> object\n"
     "\tRecursively copies tuple/dict/list objects and their elements");
 DOC("__deepequals__","(object right) -> int\n"
     "\tDeeply compares two objects for equality, returning 0 if they are not equal, 1 if they are and 2 if they are likely to be");
 DOC("__copy__",    "() -> object\n@return: The result of #(copy this)");
 DOC("__move__",    "() -> object\n@return: The result of #(move this)");
 DOC("__assign__",  "(object right) -> object\n@return: The result of #(this := right)");
 DOC("__str__",     "() -> string\n@return: The result of #(str this)");
 DOC("__repr__",    "() -> string\n@return: The result of #(repr this)");
 DOC("__bool__",    "() -> object\n@return: The result of #(!!this)");
 DOC("__not__",     "() -> object\n@return: The result of #(!this)");
 DOC("__neg__",     "() -> object\n@return: The result of #(-this)");
 DOC("__pos__",     "() -> object\n@return: The result of #(+this)");
 DOC("__inv__",     "() -> object\n@return: The result of #(~this)");
 DOC("__inc__",     "() -> object\n@return: The result of #(++this)");
 DOC("__dec__",     "() -> object\n@return: The result of #(--this)");
 DOC("__incpost__", "() -> object\n@return: The result of #(this++)");
 DOC("__decpost__", "() -> object\n@return: The result of #(this--)");
 DOC("__lo__",      "(object right) -> object\n@return: The result of #(this < right)");
 DOC("__le__",      "(object right) -> object\n@return: The result of #(this <= right)");
 DOC("__eq__",      "(object right) -> object\n@return: The result of #(this == right)");
 DOC("__ne__",      "(object right) -> object\n@return: The result of #(this != right)");
 DOC("__gr__",      "(object right) -> object\n@return: The result of #(this > right)");
 DOC("__ge__",      "(object right) -> object\n@return: The result of #(this >= right)");
 DOC("__add__",     "(object right) -> object\n@return: The result of #(this + right)");
 DOC("__iadd__",    "(object right) -> object\n@return: The result of #(this += right)");
 DOC("__sub__",     "(object right) -> object\n@return: The result of #(this - right)");
 DOC("__isub__",    "(object right) -> object\n@return: The result of #(this -= right)");
 DOC("__mul__",     "(object right) -> object\n@return: The result of #(this * right)");
 DOC("__imul__",    "(object right) -> object\n@return: The result of #(this *= right)");
 DOC("__div__",     "(object right) -> object\n@return: The result of #(this / right)");
 DOC("__idiv__",    "(object right) -> object\n@return: The result of #(this /= right)");
 DOC("__mod__",     "(object right) -> object\n@return: The result of #(this % right)");
 DOC("__imod__",    "(object right) -> object\n@return: The result of #(this %= right)");
 DOC("__shl__",     "(object right) -> object\n@return: The result of #(this << right)");
 DOC("__ishl__",    "(object right) -> object\n@return: The result of #(this <<= right)");
 DOC("__shr__",     "(object right) -> object\n@return: The result of #(this >> right)");
 DOC("__ishr__",    "(object right) -> object\n@return: The result of #(this >>= right)");
 DOC("__and__",     "(object right) -> object\n@return: The result of #(this & right)");
 DOC("__iand__",    "(object right) -> object\n@return: The result of #(this &= right)");
 DOC("__or__",      "(object right) -> object\n@return: The result of #(this | right)");
 DOC("__ior__",     "(object right) -> object\n@return: The result of #(this |= right)");
 DOC("__xor__",     "(object right) -> object\n@return: The result of #(this ^ right)");
 DOC("__ixor__",    "(object right) -> object\n@return: The result of #(this ^= right)");
 DOC("__pow__",     "(object right) -> object\n@return: The result of #(this ** right)");
 DOC("__ipow__",    "(object right) -> object\n@return: The result of #(this **= right)");
 DOC("__call__",    "(tuple args) -> object\n@return: The result of #(this(args...))");
 DOC("__thiscall__","(object this_arg, tuple args) -> object\n@return: The result of #(this(this_arg,args...))");
 DOC("__size__",    "() -> object\n@return: The result of #(#this)");
 DOC("__getitem__", "(object i) -> object\n@return: The result of #(this[right])");
 DOC("__setitem__", "(object i, object v) -> object\n@return: The result of #(this[i] = v)");
 DOC("__delitem__", "(object i) -> none\n@return: The result of #(del this[i];)");
 DOC("__getrange__","(object lo, object hi) -> object\n@return: The result of #(this[lo:hi])");
 DOC("__setrange__","(object lo, object hi, object v) -> object\n@return: The result of #(this[lo:hi] = v)");
 DOC("__delrange__","(object lo, object hi) -> none\n@return: The result of #(del this[lo:hi];)");
 DOC("__is__",      "(type object ty) -> bool\n@return: The result of #(this is ty)");
 DOC("__contains__","(object ob) -> object\n@return: The result of #(ob in this)");
 DOC("__getattr__", "(string attr) -> object\n@return: The result of #(this.operator . (attr))");
 DOC("__delattr__", "(string attr) -> none\n@return: The result of #(this.operator del. (attr))");
 DOC("__setattr__", "(string attr, object v) -> none\n@return: The result of #(this.operator .= (attr,v))");
 DOC("__iterself__","() -> object\n@return: The result of #(this.operator __iterself__())");
 DOC("__iternext__","() -> object\n@return: The result of #(this.operator __iternext__())");
 DOC("__hash__",    "(size_t start = " DEE_PP_STR(DEE_HASH_START) ") -> size_t\n@return: The result of hashing @this");
 DOC("__ref__",     "() -> object\n@return: The result of #(&this)");
 DOC("__deref__",   "() -> object\n@return: The result of #(*this)");

 // Common container/cell functions
 DOC("get",         "() -> object\n"
                    "(object fallback) -> object\n"
     "@return: The currently stored value\n"
     "@throws Error.ValueError: No object is stored and no fallback object was specified\n"
     "\tReturns the stored object.\n"
     "\tIf no object is stored, return @fallback or throw an Error.ValueError");
 DOC("del",         "() -> none\n"
     "\tDelete a stored object. No-op if no object is stored");
 DOC("pop",         "() -> object\n"
                    "(object fallback) -> object\n"
     "@return: The previously stored value\n"
     "@throws Error.ValueError: No object was stored and no fallback object was specified\n"
     "\tReturns the stored object, deleting it afterwards.\n"
     "\tIf no object is stored, return @fallback or throw an Error.ValueError");
 DOC("set",         "(object v) -> none\n"
     "\tSet/Override the stored object");
 DOC("exchange",    "(object v) -> object\n"
                    "(object v, object fallback) -> object\n"
     "@return: The previously stored value\n"
     "@throws Error.ValueError: No object was stored and no fallback object was specified\n"
     "\tExchange the stored object with v, returning the old object.\n"
     "\tIf no object is stored, return @fallback or throw an Error.ValueError");

 // Common sequence functions
 DOC("clear",       "() -> none\n"
     "\tClears @this sequence of all elements, reducing its effective size to 0");
 DOC("resize",      "(size_t n, object filler = none) -> none\n"
     "\tResizes @this sequence to fit exactly @n objects.\n"
     "\tIf the previous size was greater, old items are removed.\n"
     "\tIf the previous size was lower, new items will be filled with @filler");
 DOC("reserve",     "(size_t n) -> none\n"
     "\tGives @this sequence a hint for pre-allocating enough memory to fit at least @n total elements\n"
     "\tIf @n is too large, or not enough memory is available to provide a pre-allocated buffer, the error is silently ignored.\n"
     "\tSpecifying less than the effective size of the sequence results in a no-op.\n"
     "\tMore can be specified than what will ever be used.\n"
     "\tA call to this function will not influence the visible behaviour of any following call to any other function, other than through inspection functions like #(this.allocated()).\n"
     "\tThis function will not change the effective size of @this sequence");
 DOC("shrink_to_fit","() -> none\n"
     "\tReleases any unused buffer memory, either resulting from clearing/removing items from @this sequence, or from a prior call to #(this.reserve()).\n"
     "\tThis function will not change the effective size of @this sequence");
 DOC("reverse",     "() -> none\n"
     "\tReverses the order of all stored elements, resulting in any element that was previous accessible through @(this.front()) now being located at #(this.back()).\n"
     "\tUnless the size of @this sequence is greater than, or equal to 2, calling this function has no effect.");
 DOC("sort",        "(callable pred = none) -> none\n"
    "@throws object: Same as calling @pred or #(operator <)\n"
    "@param pred: User-defined predicate, #none or #__lo__\n"
    "\tSorts all elements stored in @this sequence in an ascending order according to @pred, or #(operator <).\n"
    "\tIf @pred is #none or #__lo__, #(operator <) will be used to compare objects");
 DOC("allocated",   "() -> size_t\n"
    "\t@return The size of the pre-allocated buffer of @this sequence.\n"
    "\tThe returned buffersize will never be lower than the effective size of @this sequence.");
 DOC("push_front",  "(object elem...) -> none\n"
     "\tInserts all elements given through varargs at the front of @this sequence"
     "\tAfter this function returns, the first of the given elements is accessible through @(this.front())");
 DOC("push_back",   "(object elem...) -> none\n"
     "\tInserts all elements given through varargs at the back of @this sequence"
     "\tAfter this function returns, the last of the given elements is accessible through #(this.back())");
 DOC("pop_front",   "() -> object\n"
     "@return: The first element of the sequence\n"
     "@throws Error.ValueError: The sequence was empty\n"
     "\tRemoves the first element of @this, returning it and decrementing the size of @this by one");
 DOC("pop_back",    "() -> object\n"
     "@return: The last element of the sequence\n"
     "@throws Error.ValueError: The sequence was empty\n"
     "\tRemoves the last element of @this, returning it and decrementing the size of @this by one");
 DOC("unique",      "(callable pred = none) -> none\n"
     "@throws object: Same as @pred or #(operator ==)\n"
     "@param pred: User-defined predicate, #none or #__eq__\n"
     "\tRemoves all consecutive objects that compare equal, keeping only the first instance and deleting the second.\n"
     "\tIf provided, @pred is used to compare two relative objects for equality.\n"
     "\tIf @pred is #none or #__eq__, #(operator ==) will be used for comparison");
 DOC("tounique",    "(callable pred = none) -> iterable\n"
     "@return: Returns an iterable object, not containing consecutive items comparing equal according to @pred or #(operator ==).\n"
     "@param pred: User-defined predicate, #none or #__eq__\n"
     "@throws object: Same as @pred or #(operator ==)\n"
     "\tReturns an iterable object who's consecutive items don't compare equal accoring to @pred.\n"
     "\tIf provided, @pred is used to compare two relative objects for equality.\n"
     "\tIf @pred is #none or #__eq__, #(operator ==) will be used for comparison");
 DOC("remove_if",   "(callable pred) -> size_t\n"
     "@return: The amount of removed items\n"
     "@param pred: User-defined predicate to returning a boolean for a given item, if that item should be removed\n"
     "@throws object: Same as @pred\n"
     "\tReturns all items matching @pred from @this sequence.");
 DOC("remove",      "(object elem, callable pred = none) -> bool\n"
     "@return: #true if an object was found, #false otherwise\n"
     "@param elem: The element to search for\n"
     "@param pred: A user-defined predicate for comparing equality, or #none or #__eq__ if #(operator ==) should be used\n"
     "@throws object: Same as calling #pred, or #(operator ==)\n"
     "\tSearches for, and removes the first object matching @pred or #(operator ==).");
 DOC("insert",      "(ssize_t i, object elem) -> none\n"
     "@param i: Index to insert @elem at, where 0 referrs to the front and #(#this-1) to the back\n"
     "@param elem: A @object that should be inserted at @i\n"
     "\tInserts the given object @elem at the given index @i, increasing the "
       "index of any existing items at positions greater than, or equal to @i by 1.\n"
     "\tThe given index @i is be clamped to #(0..#this).");
 DOC("insert_list", "(ssize_t i, iterable list) -> none\n"
     "@param i: Index to insert @elem at, where 0 referrs to the front and #(#this-1) to the back\n"
     "@param list: A sequence of objects that should be inserted at @i\n"
     "\tInserts all elements from a given sequence @list at the given index @i, increasing the "
       "index of any existing items at positions greater than, or equal to @i by 1."
     "\tThe given index @i is be clamped to #(0..#this).");
 DOC("insert_iter", "(ssize_t i, iterator iter) -> none\n"
     "@param i: Index to insert @elem at, where 0 referrs to the front and #(#this-1) to the back\n"
     "@param iter: An iterator of objects that should be inserted at @i\n"
     "\tInserts all elements from a given iterator @iter at the given index @i, increasing the "
       "index of any existing items at positions greater than, or equal to @i by 1."
     "\tThe given index @i is be clamped to #(0..#this).");
 DOC("sorted_insert","(object elem, callable pred = none) -> none\n"
     "@param elem: The element to be inserted\n"
     "@param pred: A user-defined predicate used for comparison, or #none or #__lo__ if #(operator <) should be used\n"
     "\tInserts a given element @elem into @this, maintaining a sort order using @pred or the #(operator <).");
 DOC("append","(object elem...) -> none\n"
     "@param elem: Any number of object to be appended at the end of #this\n"
     "\tAppends all given @elem at the end of @this sequence.");
 DOC("extend","(iterable seqs...) -> none\n"
     "@param seqs: Any number of sequence, who's elements should be appended at the end of #this\n"
     "\tAppends all elements from all given @seqs at the end of @this sequence.");
 DOC("erase","(ssize_t i, size_t n = 1) -> none\n"
     "@param i: The index of the first element to be erased\n"
     "@param n: The amount of elements to be erased, defaulting to 1 if not given\n"
     "\tErases up to @n elements from @this, starting at @(i).\n"
     "\tIf not enough items are available, meaning that @i or @i+@n is out of bounds, as many items as possible are deleted.");
 DOC("pop","(ssize_t i = #this-1) -> object\n"
     "@throws Error.ValueError: @this sequence is empty\n"
     "@param i: The index of the item that should be popped, defaulting to the last item of @this sequence\n"
     "@return: The popped item, originating from the closest index to @i, that is valid\n"
     "\tIf the given index @i is out of bounds, but @this sequence is not empty, @i is clamped to #(0..#this).");
 DOC("fill","(object elem, size_t size = #this) -> none\n"
     "@param elem: The object to fill @this sequence with.\n"
     "@param size: The size of @this sequence after the function returns. If not given, the size of @this sequence remains unchanged.\n"
     "\tFills @this sequence with a given @elem, overwriting any existing items before discarding them.");
 DOC("extend_unique","(sequence seq, callable pred = none) -> none\n"
     "@throws object: Same as calling @pred, or #(operator ==)\n"
     "@param seq: Input sequence to extend the list with\n"
     "@param pred: A user-defined predicate, or #none or #__eq__ if #(operator ==) should be used\n"
     "\tExtends the current list with all elements from @seq, skipping equivalent consecutive elements");

 // Misc.
 DOC("asmrepr",     "() -> string\n"
     "@return: A string representation of the assembly instructions\n"
     "\tReturns a string representation of this function's assembly instructions")
#undef DOC
 return NULL;
}

static DEE_A_RET_EXCEPT_FAIL(-1,1) int _DeeMemberName_GetDocWithArgs(
 DEE_A_IN_Z char const *member_name, DEE_A_IN DeeObject *args,
 DEE_A_REF DEE_A_OUT DeeObject **result) {
#define DOC(func,doc)\
 if (strcmp(member_name,func) == 0) {\
  DeeString_NEW_STATIC(_doc,doc);\
  return ((*result = DeeString_Format((DeeObject *)&_doc,args)) != NULL) ? 0 : -1;\
 }
 // String functions
 DOC("format","(mapping args) -> {T}\n"
              "(iterable args) -> {T}\n"
     "@return: A string formated according to the format of @this and the given arguments\n"
     "\tFormats a given format string according to the specified arguments\n"
     "\tFormating uses the {{brace}}-style notation and arguments can be any iterable, or mapping object\n"
     "\t>> print \"Hello {{you}}. I'm {{me}}!\".format({{ .you = \"User\", .me = \"Admin\" }});\n"
     "\t-> Hello User. I'm Admin!\n"
     "\t>> print \"{{}}, {{}}, {{}}\".format({{ 10,20,30 }});\n"
     "\t-> 10, 20, 30!\n"
     "\t>> print \"{{2}}, {{1}}, {{0}}\".format({{ 10,20,30 }});\n"
     "\t-> 30, 20, 10\n"
     "\tThe operator used to convert an item to a string can be specified at the end.\n"
     "\tAccepted operators are #(\"!s\") or #(\"!a\") for #(operator __str__) and #(\"!r\") for #(operator __repr__):\n"
     "\t>> print \"{{name!r}}\".format({{ .name = \"Foobar\" }});\n"
     "\t-> \"Foobar\"\n"
     "\tAdditionally, attributes, sub-ranges, items and call-results of input arguments can be used:\n"
     "\t>> print \"Made by {{name.upper().center({{width}})!r}}\".format({{ .name = \"Griefer@Work\", .width = 14 }});\n"
     "\t-> Made by \" GRIEFER@WORK \"\n"
     "\tType-specific formating can be written after a #(\":\") at the end, where the following string will be used in a call to the #__format__ member function:\n"
     "\t>> #include <time>\n"
     "\t>> print \"The time is {{time:%Y-%m-%d %H:%M}}\".format({{ .time = time.now() }});\n"
     "\tBraces can be escaped by writing them twice:\n"
     "\t>> print \"{{{{}}}} The {{{{answer}}}} is {{}}\".format({{ 42 }});\n"
     "\t-> {{}} The {{answer}} is 42");
 DOC("ord","() -> {T}\n"
     "@return: The ordinal character value of a given 1-character string");
 DOC("lower","() -> {T}\n"
     "@return: @this string with all upper-case characters converted to lower-case\n"
     "\tReturns the lower-case version of @this string:\n"
     "\t>> print \"Rise and shine Mr. Freeman!\".lower();\n"
     "\t-> rise and shine mr. freeman!");
 DOC("upper","() -> {T}\n"
     "@return: @this string with all lower-case characters converted to upper-case\n"
     "\tReturns the upper-case version of @this string:\n"
     "\t>> print \"Rise and shine Mr. Freeman!\".upper();\n"
     "\t-> RISE AND SHINE MR. FREEMAN!");
 DOC("title","() -> {T}\n"
     "@return: @this string converted to title casing\n"
     "\tReturns @this string in title casing:\n"
     "\t>> print \"Rise and shine Mr. Freeman!\".title();\n"
     "\t-> Rise and shine mr. freeman!");
 DOC("capitalize","() -> {T}\n"
     "@return: @this string with all words capitalized\n"
     "\tReturns the capitalized version of @this string:\n"
     "\t>> print \"Rise and shine Mr. Freeman!\".capitalize();\n"
     "\t-> Rise And Shine Mr. Freeman!");
 DOC("split",     "({T} sep) -> iterable\n"
     "@return: An iterable object containing all split strings\n"
     "\tSplits @this at every occurance of @sep into a iterable.");
 DOC("rsplit",    "({T} sep) -> iterable\n"
     "@return: An iterable object containing all split strings\n"
     "\tSplits @this at every occurance of @sep into a iterable, with all elements listed in reverse order.");
 DOC("splitlines","(bool keepends = false) -> iterable\n"
     "@param keepends: Indicates whether or not terminating linefeeds should be kept\n"
     "@return: An iterable object yielding lines\n"
     "\tSplits @this string into a sequence of lines.\n"
     "\tIf @keepends is #true, terminating linefeeds are kept in the returned strings.");
 DOC("sstrip",    "({T} sub) -> {T}\n"
     "\tSimilar to #(this.strip()), but removes string sequences. Equivalent to:\n"
     "\t>> constexpr function sstrip(s,sub) {{\n"
     "\t>>\t local result = s.startswith(sub) ? s[#sub:] : s;\n"
     "\t>>\t if (result.endswith(sub)) result = result[:#result-#sub];\n"
     "\t>>\t return result;\n"
     "\t>> }}");
 DOC("lsstrip",   "({T} sub) -> {T}\n"
     "\tSimilar to #(this.lstrip()), but removes string sequences. Equivalent to:\n"
     "\t>> constexpr function lsstrip(s,sub) -> s.startswith(sub) ? s[#sub:] : s;");
 DOC("rsstrip",   "({T} sub) -> {T}\n"
     "\tSimilar to #(this.lstrip()), but removes string sequences. Equivalent to:\n"
     "\t>> constexpr function rsstrip(s,sub) -> s.endswith(sub) ? s[:#s-#sub] : s;");

 // Atomic integer members
 DOC("init",                       "({T} v) -> none\n"
     "@param v: The initial value for @this\n"
     "\tInitializes the #(atomic {T}) to @v");
 DOC("store",                      "({T} v, int order = memory_order_seq_cst) -> none\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param v: The new value for @this\n"
     "@param order: #memory_order_relaxed, #memory_order_release or #memory_order_seq_cst\n"
     "\tStores @v in @this using the specified @order.\n"
     "\tThe old value will be discarded.");
 DOC("load",                       "(int order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param order: #memory_order_relaxed, #memory_order_consume, #memory_order_acquire or #memory_order_seq_cst\n"
     "@return: The loaded value\n"
     "\tLoads the atomic value of @this using the specified @order.");
 DOC("exchange",                   "({T} v, int order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param v: The new value for @this\n"
     "@param order: #memory_order_relaxed, #memory_order_acquire, #memory_order_release, #memory_order_acq_rel or #memory_order_seq_cst\n"
     "@return: The previous value of @this\n"
     "\tExchanges the current value of @this with @v, returning the old value.");
 DOC("compare_exchange_weak",      "({T} old_v, {T} new_v, int success_order = memory_order_seq_cst, int failure_order = memory_order_seq_cst) -> bool\n"
     "@throws Error.ValueError: An invalid @success_order or @failure_order was specified\n"
     "@param old_v: The value to comapre @this against\n"
     "@param new_v: The value to store if @old_v is equal to @this\n"
     "@param success_order: The memory order used if @old_v is equal to @this\n"
     "@param failure_order: The memory order used if @old_v is not equal to @this\n"
     "@return: #true if @old_v was equal to @this and @new_v was stored in @this, #false otherwise\n"
     "\tCompares the current value of @this against @old_v and replaces it with @new_v if they are equal, as well as returning #true.\n"
     "\tNOTE: #(atomic {T}::compare_exchange_weak) might fail sporadicly, but may be faster. Use #(atomic {T}::compare_exchange_strong) if this is not wanted.");
 DOC("compare_exchange_strong",    "({T} old_v, {T} new_v, int success_order = memory_order_seq_cst, int failure_order = memory_order_seq_cst) -> bool\n"
     "@throws Error.ValueError: An invalid @success_order or @failure_order was specified\n"
     "@param old_v: The value to comapre @this against\n"
     "@param new_v: The value to store if @old_v is equal to @this\n"
     "@param success_order: The memory order used if @old_v is equal to @this\n"
     "@param failure_order: The memory order used if @old_v is not equal to @this\n"
     "@return: #true if @old_v was equal to @this and @new_v was stored in @this, #false otherwise\n"
     "\tCompares the current value of @this against @old_v and replaces it with @new_v if they are equal, as well as returning #true.");
 DOC("compare_exchange_weak_val",  "({T} old_v, {T} new_v, int success_order = memory_order_seq_cst, int failure_order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @success_order or @failure_order was specified\n"
     "@param old_v: The value to comapre @this against\n"
     "@param new_v: The value to store if @old_v is equal to @this\n"
     "@param success_order: The memory order used if @old_v is equal to @this\n"
     "@param failure_order: The memory order used if @old_v is not equal to @this\n"
     "@return: The previous value of @this, whether or not that value was exchanged\n"
     "\tCompares the current value of @this against @old_v and replaces it with @new_v if they are equal, as well as always returning the old value.\n"
     "\tNOTE: #(atomic {T}::compare_exchange_weak_val) might fail sporadicly, but may be faster. Use #(atomic {T}::compare_exchange_strong_val) if this is not wanted.");
 DOC("compare_exchange_strong_val","({T} old_v, {T} new_v, int success_order = memory_order_seq_cst, int failure_order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @success_order or @failure_order was specified\n"
     "@param old_v: The value to comapre @this against\n"
     "@param new_v: The value to store if @old_v is equal to @this\n"
     "@param success_order: The memory order used if @old_v is equal to @this\n"
     "@param failure_order: The memory order used if @old_v is not equal to @this\n"
     "@return: The previous value of @this, whether or not that value was exchanged\n"
     "\tCompares the current value of @this against @old_v and replaces it with @new_v if they are equal, as well as always returning the old value.");
 DOC("compare_exchange",           "({T} old_v, {T} new_v, int success_order = memory_order_seq_cst, int failure_order = memory_order_seq_cst) -> bool\n"
     "@throws Error.ValueError: An invalid @success_order or @failure_order was specified\n"
     "@param old_v: The value to comapre @this against\n"
     "@param new_v: The value to store if @old_v is equal to @this\n"
     "@param success_order: The memory order used if @old_v is equal to @this\n"
     "@param failure_order: The memory order used if @old_v is not equal to @this\n"
     "@return: #true if @old_v was equal to @this and @new_v was stored in @this, #false otherwise\n"
     "\tAlias for #(atomic {T}::compare_exchange_strong).\n"
     "\tCompares the current value of @this against @old_v and replaces it with @new_v if they are equal, as well as returning #(true).");
 DOC("compare_exchange_val",       "({T} old_v, {T} new_v, int success_order = memory_order_seq_cst, int failure_order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @success_order or @failure_order was specified\n"
     "@param old_v: The value to comapre @this against\n"
     "@param new_v: The value to store if @old_v is equal to @this\n"
     "@param success_order: The memory order used if @old_v is equal to @this\n"
     "@param failure_order: The memory order used if @old_v is not equal to @this\n"
     "@return: The previous value of @this, whether or not that value was exchanged\n"
     "\tAlias for #(atomic {T}::compare_exchange_strong_val).\n"
     "\tCompares the current value of @this against @old_v and replaces it with @new_v if they are equal, as well as always returning the old value.");
 DOC("fetch_inc",                  "(int order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param order: #memory_order_relaxed, #memory_order_acquire, #memory_order_release or #memory_order_seq_cst\n"
     "@return: The previous value of @this\n"
     "\tFetch and increment the value of @this atomicly. Return the old value.\n"
     "\tSame as #(this.fetch_add(1,order)), but may be faster.");
 DOC("inc_fetch",                  "(int order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param order: #memory_order_relaxed, #memory_order_acquire, #memory_order_release or #memory_order_seq_cst\n"
     "@return: The new value of @this\n"
     "\tIncrement and fetch the value of @this atomicly. Return the new value.\n"
     "\tSame as #(this.add_fetch(1,order)), but may be faster.");
 DOC("fetch_dec",                  "(int order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param order: #memory_order_relaxed, #memory_order_acquire, #memory_order_release or #memory_order_seq_cst\n"
     "@return: The previous value of @this\n"
     "\tFetch and decrement the value of @this atomicly. Return the old value.\n"
     "\tSame as #(this.fetch_sub(1,order)), but may be faster.");
 DOC("dec_fetch",                  "(int order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param order: #memory_order_relaxed, #memory_order_acquire, #memory_order_release or #memory_order_seq_cst\n"
     "@return: The new value of @this\n"
     "\tDecrement and fetch the value of @this atomicly. Return the new value.\n"
     "\tSame as #(this.sub_fetch(1,order)), but may be faster.");
 DOC("fetch_add",                  "({T} v, int order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param v: The value that should be added to @this\n"
     "@param order: #memory_order_relaxed, #memory_order_acquire, #memory_order_release or #memory_order_seq_cst\n"
     "@return: The old value of @this\n"
     "\tAdd @v to @this atomicly. Return the old value.\n"
     "\tEquivalent to the following, but may be faster:\n"
     "\t>> function fetch_add(self,v,order): result {{\n"
     "\t>>\t do result = self.load();\n"
     "\t>>\t while (!self.compare_exchange_weak(result,result+v,memory_order_relaxed,order));\n"
     "\t>> }}");
 DOC("add_fetch",                  "({T} v, int order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param v: The value that should be added to @this\n"
     "@param order: #memory_order_relaxed, #memory_order_acquire, #memory_order_release or #memory_order_seq_cst\n"
     "@return: The new value of @this\n"
     "\tAdd @v to @this atomicly. Return the new value."
     "\tEquivalent to the following, but may be faster:\n"
     "\t>> function add_fetch(self,v,order) -> self.fetch_add(v,order)+v;");
 DOC("fetch_sub",                  "({T} v, int order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param v: The value that should be subtracted from @this\n"
     "@param order: #memory_order_relaxed, #memory_order_acquire, #memory_order_release or #memory_order_seq_cst\n"
     "@return: The old value of @this\n"
     "\tSubtract @v to @this atomicly. Return the old value."
     "\tEquivalent to the following, but may be faster:\n"
     "\t>> function fetch_sub(self,v,order): result {{\n"
     "\t>>\t do result = self.load();\n"
     "\t>>\t while (!self.compare_exchange_weak(result,result-v,memory_order_relaxed,order));\n"
     "\t>> }}");
 DOC("sub_fetch",                  "({T} v, int order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param v: The value that should be subtracted from @this\n"
     "@param order: #memory_order_relaxed, #memory_order_acquire, #memory_order_release or #memory_order_seq_cst\n"
     "@return: The new value of @this\n"
     "\tSubtract @v to @this atomicly. Return the new value."
     "\tEquivalent to the following, but may be faster:\n"
     "\t>> function sub_fetch(self,v,order) -> self.fetch_sub(v,order)-v;");
 DOC("fetch_and",                  "({T} v, int order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param v: The value that should be joined with @this using #(operator __and__)\n"
     "@param order: #memory_order_relaxed, #memory_order_acquire, #memory_order_release or #memory_order_seq_cst\n"
     "@return: The old value of @this\n"
     "\tJoin @v with @this using #(operator __and__) atomicly. Return the old value."
     "\tEquivalent to the following, but may be faster:\n"
     "\t>> function fetch_and(self,v,order): result {{\n"
     "\t>>\t do result = self.load();\n"
     "\t>>\t while (!self.compare_exchange_weak(result,result & v,memory_order_relaxed,order));\n"
     "\t>> }}");
 DOC("and_fetch",                  "({T} v, int order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param v: The value that should be joined with @this using #(operator __and__)\n"
     "@param order: #memory_order_relaxed, #memory_order_acquire, #memory_order_release or #memory_order_seq_cst\n"
     "@return: The new value of @this\n"
     "\tJoin @v with @this using #(operator __and__) atomicly. Return the new value."
     "\tEquivalent to the following, but may be faster:\n"
     "\t>> function and_fetch(self,v,order) -> self.fetch_and(v,order) & v;");
 DOC("fetch_or",                    "({T} v, int order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param v: The value that should be joined with @this using #(operator __or__)\n"
     "@param order: #memory_order_relaxed, #memory_order_acquire, #memory_order_release or #memory_order_seq_cst\n"
     "@return: The old value of @this\n"
     "\tJoin @v with @this using #(operator __or__) atomicly. Return the old value."
     "\tEquivalent to the following, but may be faster:\n"
     "\t>> function fetch_or(self,v,order): result {{\n"
     "\t>>\t do result = self.load();\n"
     "\t>>\t while (!self.compare_exchange_weak(result,result | v,memory_order_relaxed,order));\n"
     "\t>> }}");
 DOC("or_fetch",                    "({T} v, int order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param v: The value that should be joined with @this using #(operator __or__)\n"
     "@param order: #memory_order_relaxed, #memory_order_acquire, #memory_order_release or #memory_order_seq_cst\n"
     "@return: The new value of @this\n"
     "\tJoin @v with @this using #(operator __or__) atomicly. Return the new value."
     "\tEquivalent to the following, but may be faster:\n"
     "\t>> function or_fetch(self,v,order) -> self.fetch_or(v,order) | v;");
 DOC("fetch_xor",                  "({T} v, int order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param v: The value that should be joined with @this using #(operator __xor__)\n"
     "@param order: #memory_order_relaxed, #memory_order_acquire, #memory_order_release or #memory_order_seq_cst\n"
     "@return: The old value of @this\n"
     "\tJoin @v with @this using #(operator __xor__) atomicly. Return the old value."
     "\tEquivalent to the following, but may be faster:\n"
     "\t>> function fetch_or(self,v,order): result {{\n"
     "\t>>\t do result = self.load();\n"
     "\t>>\t while (!self.compare_exchange_weak(result,result ^ v,memory_order_relaxed,order));\n"
     "\t>> }}");
 DOC("xor_fetch",                  "({T} v, int order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param v: The value that should be joined with @this using #(operator __xor__)\n"
     "@param order: #memory_order_relaxed, #memory_order_acquire, #memory_order_release or #memory_order_seq_cst\n"
     "@return: The new value of @this\n"
     "\tJoin @v with @this using #(operator __xor__) atomicly. Return the new value."
     "\tEquivalent to the following, but may be faster:\n"
     "\t>> function or_fetch(self,v,order) -> self.fetch_or(v,order) ^ v;");
 DOC("clear",                      "(int order = memory_order_seq_cst) -> none\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param order: #memory_order_relaxed, #memory_order_release or #memory_order_seq_cst\n"
     "\tStores 0 in @this using the specified @order.\n"
     "\tThe old value will be discarded.\n"
     "\tTogether with #(atomic {T}::test_and_set), this function is meant to allow use of @this #(atomic {T}) as an atomic_flag.\n"
     "\tEquivalent to the following, but may be faster:\n"
     "\t>> function clear(self,order) -> self.store(0,order);");
 DOC("test_and_set",               "(int order = memory_order_seq_cst) -> {T}\n"
     "@throws Error.ValueError: An invalid @order was specified\n"
     "@param order: #memory_order_relaxed, #memory_order_release or #memory_order_seq_cst\n"
     "@return: The old value of @(this). 0 if this was the first call to #(atomic {T}::test_and_set) after calling #(atomic {T}::clear)\n"
     "\tExchanges the value of @this with 1 and returns the previous value.\n"
     "\tTogether with #(atomic {T}::clear), this function is meant to allow use of @this #(atomic {T}) as an atomic_flag\n"
     "\tEquivalent to the following, but may be faster:\n"
     "\t>> function clear(self,order) -> self.exchange(1,order);");
#undef DOC
 {
  char const *fallback;
  if ((fallback = _DeeMemberName_GetDoc(member_name)) == NULL) return 1;
  if ((*result = DeeString_New(fallback)) == NULL) return -1;
  return 0;
 }
}


static DEE_A_RET_Z_OPT char const *
_DeeMemberFunction_GetDoc(DEE_A_IN DeeMemberFunction func) {
 //////////////////////////////////////////////////////////////////////////
 // Documentation strings for the generic iterable member functions
 if (func == &_deegenericiterable_at) return docfor_at;
 if (func == &_deegenericiterable_back) return docfor_back;
 if (func == &_deegenericiterable_count) return docfor_count;
 if (func == &_deegenericiterable_empty) return docfor_empty;
 if (func == &_deegenericiterable_find) return docfor_find;
 if (func == &_deegenericiterable_front) return docfor_front;
 if (func == &_deegenericiterable_index) return docfor_index;
 if (func == &_deegenericiterable_locate) return docfor_locate;
 if (func == &_deegenericiterable_locate_all) return docfor_locate_all;
 if (func == &_deegenericiterable_non_empty) return docfor_non_empty;
 if (func == &_deegenericiterable_reversed) return docfor_reversed;
 if (func == &_deegenericiterable_rfind) return docfor_rfind;
 if (func == &_deegenericiterable_rindex) return docfor_rindex;
 if (func == &_deegenericiterable_rlocate) return docfor_rlocate;
 if (func == &_deegenericiterable_sorted) return docfor_sorted;
 if (func == &_deegenericiterable_transform) return docfor_transform;
 if (func == &_deegenericiterable_segments) return docfor_segments;
 if (func == &_deegenericiterable_contains) return docfor_contains;
 //////////////////////////////////////////////////////////////////////////

 return NULL;
}

static DEE_A_RET_EXCEPT_REF DeeObject *
_DeeMemberName_GetDocArgs(DEE_A_IN_Z char const *args) {
 DeeObject *result,*key,*item; int error;
 char const *key_begin,*key_end,*item_begin;
 if ((result = DeeDict_New()) == NULL) return NULL;
 while (*args) {
  key_begin = args;
  while (*args && *args != '=') ++args;
  key_end = args++;
  item_begin = args;
  while (*args && *args != ',') ++args;
  if ((key = DeeString_NewWithLength((Dee_size_t)(
   key_end-key_begin),key_begin)) == NULL) {
err_r: Dee_DECREF(result); return NULL;
  }
  if ((item = DeeString_NewWithLength((Dee_size_t)(
   args-item_begin),item_begin)) == NULL) {
/*err_value:*/ Dee_DECREF(key); goto err_r;
  }
  error = DeeDict_SetItem(result,key,item);
  Dee_DECREF(item);
  Dee_DECREF(key);
  if (error != 0) goto err_r;
 }
 return result;
}





static DEE_A_RET_Z_OPT char const *_DeeBuiltin_GetDoc(
 DEE_A_IN Dee_uint16_t builtin_id, DEE_A_OUT_OPT char const **name) {
 if (name) *name = NULL;
 switch (builtin_id) {
#define DOC(id,name_,text) case id: if (name) *name = name_; return text
  DOC(0x0102,"__builtin_min","(object args...) -> object\n"
      "@throws object: Same as #(operator <) performed on the given arguments\n"
      "@return: The lowers element of @args\n"
      "\tObjects are always compared with the current winner as lhs operand.");
  DOC(0x0103,"__builtin_max","(object args...) -> object\n"
      "@throws object: Same as #(operator <) performed on the given arguments\n"
      "@return: The greatest element of @args\n"
      "\tObjects are always compared with the current winner as lhs operand.");
  DOC(0x0104,"__builtin_avg","(object args...) -> double\n"
      "@throws object: Same as #(operator double) performed on the given arguments\n"
      "@return: The average of all elements in @args\n"
      "\tAll elements are casted to double, added and devided by #(#args).");
  DOC(0x0105,"__builtin_all","(object args...) -> bool\n"
      "@return: #true if all elements in @args evaluate to #true\n"
      "\t>> constexpr function all(args...) {\n"
      "\t>>\t for (local x: args) {\n"
      "\t>>\t\t if (!(!!x)) return false;\n"
      "\t>>\t }\n"
      "\t>>\t return true;\n"
      "\t>> }");
  DOC(0x0106,"__builtin_any","(object args...) -> bool\n"
      "\tReturns #true if any element in @args evaluates to #true\n"
      "\t>> constexpr function any(args...) {\n"
      "\t>>\t for (local x: args) {\n"
      "\t>>\t\t if (!!x) return true;\n"
      "\t>>\t }\n"
      "\t>>\t return false;\n"
      "\t>> }");
  DOC(0x0107,"__builtin_sum","(object args...) -> object\n"
      "@return: The sum of all arguments by adding them together using #(operator +)\n"
      "\tThe result is always replaced with the addition of #(result+next).\n"
      "\tReturns #none if no arguments are given.\n"
      "\t>> constexpr function sum(args...) {\n"
      "\t>>\t if (!args) return none;\n"
      "\t>>\t local result = args[0];\n"
      "\t>>\t for (local x: args[1:]) {\n"
      "\t>>\t\t result = result+x;\n"
      "\t>>\t }\n"
      "\t>>\t return result;\n"
      "\t>> }");
  DOC(0x0109,"__builtin_ord","(string::utf8 s) -> char\n"
                             "(string::wide s) -> wchar_t\n"
                             "(string::utf16 s) -> char16_t\n"
                             "(string::utf32 s) -> char32_t\n"
      "\tReturns the ordinal value of a one-character string. Equivalent to #(s.ord()).");
  DOC(0x0108,"__builtin_sqr","(object x) -> object\n"
      "\tReturns the sqare of @x, that is #(x*x)\n"
      "\t>> constexpr function sqr(x) -> x*x;");
  DOC(0x010B,"__builtin_ceil","(double x) -> int\n"
      "@return: The ceiled integral value of a given double @(x).");
  DOC(0x010C,"__builtin_floor","(double x) -> int\n"
      "@return: The floored integral value of a given double @(x).");
  DOC(0x010D,"__builtin_round","(double x) -> int\n"
      "@return: The rounded integral value of a given double @(x).");

  DOC(0x0117,"__builtin_range",
      "(int8_t count) -> int8_t.range\n"
      "(int16_t count) -> int16_t.range\n"
      "(int32_t count) -> int32_t.range\n"
      "(int64_t count) -> int64_t.range\n"
      "(uint8_t count) -> uint8_t.range\n"
      "(uint16_t count) -> uint16_t.range\n"
      "(uint32_t count) -> uint32_t.range\n"
      "(uint64_t count) -> uint64_t.range\n"
      "(object count) -> generic_range_iterator\n"
      "@return: A generator that yields all values between 0 or a default-initialized instance of @count\n"
      "(int8_t begin, int8_t end) -> int8_t.range\n"
      "(int16_t begin, int16_t end) -> int16_t.range\n"
      "(int32_t begin, int32_t end) -> int32_t.range\n"
      "(int64_t begin, int64_t end) -> int64_t.range\n"
      "(uint8_t begin, uint8_t end) -> uint8_t.range\n"
      "(uint16_t begin, uint16_t end) -> uint16_t.range\n"
      "(uint32_t begin, uint32_t end) -> uint32_t.range\n"
      "(uint64_t begin, uint64_t end) -> uint64_t.range\n"
      "(object begin, object end) -> generic_range_iterator\n"
      "@return: A generator that yields all values between @begin and @end where the iterator is moved with #(operator ++)\n"
      "(int8_t begin, int8_t end, int8_t step) -> int8_t.range\n"
      "(int16_t begin, int16_t end, int16_t step) -> int16_t.range\n"
      "(int32_t begin, int32_t end, int32_t step) -> int32_t.range\n"
      "(int64_t begin, int64_t end, int64_t step) -> int64_t.range\n"
      "(uint8_t begin, uint8_t end, uint8_t step) -> uint8_t.range\n"
      "(uint16_t begin, uint16_t end, uint16_t step) -> uint16_t.range\n"
      "(uint32_t begin, uint32_t end, uint32_t step) -> uint32_t.range\n"
      "(uint64_t begin, uint64_t end, uint64_t step) -> uint64_t.range\n"
      "(object begin, object end, object step) -> generic_range_iterator\n"
      "@return: A generator that yields all values between @begin and @end where the iterator is moved with #(operator += (step))");
  DOC(0x0118,"__builtin_id","(object ob) -> size_t\n"
      "@return: An id that is unique to the given object @ob.\n"
      "\tReturns an id unique to @ob and not shared by any other object during the lifetime of @ob\n"
      "\tNOTE: Since v101, this is no longer the pointer in non-developer builds. "
              "And yes, obviously it's based on the pointer, but is now being xor'd "
              "with a random seed that changes every time deemon is started. "
              "So it's still unique, but no longer a gaping security hole "
              "for accessing otherwise protected parts of the runtime.");
  DOC(0x01F1,"__builtin_enumerate","(object iterable) -> enumiterator\n"
      "(size_t start_index, object iterable) -> enumiterator");
  DOC(0x0175,"__builtin_swap","(object a, object b) -> none\n"
      "@throws Error.TypeError: #(type b) isn't equal to #(type a)\n"
      "\tSwaps the data of two given objects @a and @b of the same type.\n"
      "\t#(type a) must implement #(operator __move__) and #(operator __moveassign__).");
  DOC(0x010A,"__builtin_abs","(int8_t x) -> int8_t\n"
      "(int16_t x) -> int16_t\n"
      "(int32_t x) -> int32_t\n"
      "(int64_t x) -> int64_t\n"
      "(uint8_t x) -> uint8_t\n"
      "(uint16_t x) -> uint16_t\n"
      "(uint32_t x) -> uint32_t\n"
      "(uint64_t x) -> uint64_t\n"
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double\n"
      "@return: The absolute value of @x\n"
      "(object x) -> object\n"
      "@throws object: Same as the function below\n"
      "@return: The absolute value of @x\n"
      "\tEquivalent to the following code:\n"
      "\t>> constexpr function abs(x) {\n"
      "\t>>\t if (x < (int)0) return -x;\n"
      "\t>>\t return x;\n"
      "\t>> }");
  DOC(0x01B2,"__builtin_seq_find","(sequence seq, object elem, callable pred = none) -> ssize_t\n"
      "@throws object: Same as @pred or #(operator ==)\n"
      "@return: The index of @elem in @seq or #(-1) if it wasn't found.\n"
      "\tGeneric, iterator-based find function.\n"
      "\tIf given, use @pred to compare two objects for equality.\n"
      "\tNOTE: Tries to call #(seq.find(elem)) / #(seq.find(elem,pred)) before using iterators.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function find(seq,elem,pred...) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.find(elem,pred...);\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t local index = 0;\n"
      "\t>>\t for (local x: seq) {\n"
      "\t>>\t\t if (pred ? pred[0](x,elem) : x == elem)\n"
      "\t>>\t\t\t return index;\n"
      "\t>>\t\t ++index;\n"
      "\t>>\t }\n"
      "\t>>\t return -1;\n"
      "\t>> }");
  DOC(0x01B3,"__builtin_seq_rfind","(sequence seq, object elem, callable pred = none) -> ssize_t\n"
      "@throws object: Same as @pred or #(operator ==)\n"
      "@return: The index of @elem in @seq or #(-1) if it wasn't found.\n"
      "\tGeneric, iterator-based rfind function.\n"
      "\tpreferring higer indexes over lower ones.\n"
      "\tIf given, use @pred to compare two objects for equality.\n"
      "\tNOTE: Tries to call #(seq.rfind(elem)) / #(seq.rfind(elem,pred)) before using iterators.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function rfind(seq,elem,pred...) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.rfind(elem,pred...);\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t local result = -1;\n"
      "\t>>\t local index = 0;\n"
      "\t>>\t for (local x: seq) {\n"
      "\t>>\t\t if (pred ? pred[0](x,elem) : x == elem)\n"
      "\t>>\t\t\t result = index;\n"
      "\t>>\t\t ++index;\n"
      "\t>>\t }\n"
      "\t>>\t return result;\n"
      "\t>> }");
  DOC(0x01B4,"__builtin_seq_index","(sequence seq, object elem, callable pred = none) -> size_t\n"
      "@throws object: Same as @pred or #(operator ==)\n"
      "@throws Error.ValueError: @elem could not be found in @seq\n"
      "@return: The index of @elem in @seq\n"
      "\tGeneric, iterator-based index function.\n"
      "\tIf given, use @pred to compare two objects for equality.\n"
      "\tNOTE: Tries to call #(seq.index(elem)) / #(seq.index(elem,pred)) before using iterators.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function index(seq,elem,pred...) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.index(elem,pred...);\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t local index = 0;\n"
      "\t>>\t for (x: seq) {\n"
      "\t>>\t\t if (pred ? pred[0](x,elem) : x == elem)\n"
      "\t>>\t\t\t return index;\n"
      "\t>>\t\t ++index;\n"
      "\t>>\t }\n"
      "\t>>\t throw Error.ValueError(\"...\");\n"
      "\t>> }");
  DOC(0x01B5,"__builtin_seq_rindex","(sequence seq, object elem, callable pred = none) -> size_t\n"
      "@throws object: Same as @pred or #(operator ==)\n"
      "@throws Error.ValueError: @elem could not be found in @seq\n"
      "@return: The index of @elem in @seq\n"
      "\tGeneric, iterator-based rindex function.\n"
      "\tif it wasn't found, preferring higer indexes over lower ones.\n"
      "\tIf given, use @pred to compare two objects for equality.\n"
      "\tNOTE: Tries to call #(seq.rindex(elem)) / #(seq.rindex(elem,pred)) before using iterators.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function rindex(seq,elem,pred...) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.rindex(elem,pred...);\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t local result;\n"
      "\t>>\t local index = 0;\n"
      "\t>>\t for (x: seq) {\n"
      "\t>>\t\t if (pred ? pred[0](x,elem) : x == elem)\n"
      "\t>>\t\t\t result = index;\n"
      "\t>>\t\t ++index;\n"
      "\t>>\t }\n"
      "\t>>\t if (!__builtin_bound(result));\n"
      "\t>>\t\t throw Error.ValueError(\"...\");\n"
      "\t>>\t return result;\n"
      "\t>> }");
  DOC(0x0189,"__builtin_seq_contains","(sequence seq, object elem, callable pred = none) -> bool\n"
      "@return: #true or #false, indicating @elem being contained in @seq\n"
      "\tGeneric, iterator-based contains function.\n"
      "\tIf given, use @pred to compare two objects for equality\n"
      "\tNOTE: Tries to call #(seq.contains(elem)) / #(seq.contains(elem,pred)) before using iterators.\n"
      "\tIf iterators are not implemented either, and no predicate is given, call #(elem in seq).\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function contains(seq,elem,pred...) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.contains(elem,pred...);\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t try {\n"
      "\t>>\t\t for (local x: seq) {\n"
      "\t>>\t\t\t if (pred ? pred[0](x,elem) : x == elem)\n"
      "\t>>\t\t\t\t return true;\n"
      "\t>>\t\t }\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented e) {\n"
      "\t>>\t\t if (pred) throw;\n"
      "\t>>\t\t return elem in seq;\n"
      "\t>>\t }\n"
      "\t>>\t return false;\n"
      "\t>> }");
  DOC(0x01B6,"__builtin_seq_count","(sequence seq, object elem, callable pred = none) -> size_t\n"
      "@return: The amount of objects equivalent to @elem in @seq\n"
      "\tGeneric, iterator-based count function.\n"
      "\tIf given, use @pred to compare two objects for equality.\n"
      "\tNOTE: Tries to call #(seq.count(elem)) / #(seq.count(elem,pred)) before using iterators.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function count(seq,elem,pred...) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.count(elem,pred...);\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t local result = (__SIZE_TYPE__)0;\n"
      "\t>>\t for (x: seq) {\n"
      "\t>>\t\t if (pred ? pred[0](x,elem) : x == elem)\n"
      "\t>>\t\t\t ++result;\n"
      "\t>>\t }\n"
      "\t>>\t return result;\n"
      "\t>> }");
  DOC(0x01B7,"__builtin_seq_locate","(sequence seq, callable pred) -> object\n"
      "@return: The first object for which #(pred(object)) returned #true\n"
      "@throws Error.ValueError: No such object matching @pred exists in @seq\n"
      "\tGeneric, iterator-based locate function.\n"
      "\tNOTE: Tries to call #(seq.locate(pred)) before using iterators.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function locate(seq,pred) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.locate(pred);\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t for (local x: seq) {\n"
      "\t>>\t\t if (pred(x))\n"
      "\t>>\t\t\t return x;\n"
      "\t>>\t }\n"
      "\t>>\t throw Error.ValueError(\"...\");\n"
      "\t>> }");
  DOC(0x01B8,"__builtin_seq_rlocate","(sequence seq, callable pred) -> object\n"
      "@return: The last object for which #(pred(object)) returned #true\n"
      "@throws Error.ValueError: No such object matching @pred exists in @seq\n"
      "\tGeneric, iterator-based rlocate function.\n"
      "\tNOTE: Tries to call #(seq.rlocate(pred)) before using iterators.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function rlocate(seq,pred) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.rlocate(pred);\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t local result;\n"
      "\t>>\t for (local x: seq) {\n"
      "\t>>\t\t if (pred(x))\n"
      "\t>>\t\t\t result = x;\n"
      "\t>>\t }\n"
      "\t>>\t if (!__builtin_bound(result))\n"
      "\t>>\t\t throw Error.ValueError(\"...\");\n"
      "\t>>\t return result;\n"
      "\t>> }");
  DOC(0x01B9,"__builtin_seq_locate_all","(sequence seq, callable pred) -> object\n"
      "@return: A sequence of objects for which #(pred(object)) returned #true\n"
      "\tGeneric, iterator-based locate_all function.\n"
      "\tUsually a #list is returned, but the exact typing depends on @(seq).\n"
      "\tNOTE: Tries to call #(seq.locate_all(pred)) before using iterators.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function locate_all(seq,pred) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.locate_all(pred);\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t return for (local x: seq) if (pred(x)) x;\n"
      "\t>> }");
  DOC(0x01BA,"__builtin_seq_transform","(sequence seq, callable pred) -> object\n"
      "@return: A new sequence containing all elemented from @seq, but modified @pred\n"
      "\tGeneric, iterator-based transform function.\n"
      "\tNOTE: Tries to call #(seq.transform(pred)) before using iterators.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function transform(seq,pred) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.transform(pred);\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t return for (x: seq) pred(x);\n"
      "\t>> }");
  DOC(0x01BB,"__builtin_seq_empty","(sequence seq) -> bool\n"
      "@return: true/false if the sequence is empty\n"
      "\tGeneric, iterator-based empty function.\n"
      "\tNOTE: Tries to call #(seq.empty()) before using iterators.\n"
      "\tNOTE: Tries to call #(#seq) if iterators are not implemented.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function empty(seq) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.empty();\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t local iter = seq.operator __iterself__();\n"
      "\t>>\t try {\n"
      "\t>>\t\t iter.operator __iternext__();\n"
      "\t>>\t } catch (Signal.StopIteration) {\n"
      "\t>>\t\t return true;\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t\t return #seq == 0;\n"
      "\t>>\t }\n"
      "\t>>\t return false;\n"
      "\t>> }");
  DOC(0x01BC,"__builtin_seq_non_empty","(sequence seq) -> bool\n"
      "@return: true/false if the sequence is empty\n"
      "\tGeneric, iterator-based non_empty function.\n"
      "\tNOTE: Tries to call #(seq.non_empty()) before using iterators.\n"
      "\tNOTE: Tries to call #(#seq) if iterators are not implemented.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function non_empty(seq) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.non_empty();\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t local iter = seq.operator __iterself__();\n"
      "\t>>\t try {\n"
      "\t>>\t\t iter.operator __iternext__();\n"
      "\t>>\t } catch (Signal.StopIteration) {\n"
      "\t>>\t\t return false;\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t\t return #seq != 0;\n"
      "\t>>\t }\n"
      "\t>>\t return true;\n"
      "\t>> }");
  DOC(0x01BD,"__builtin_seq_front","(sequence seq) -> object\n"
      "\tGeneric, iterator-based front function.\n"
      "\tNOTE: Tries to call #(seq.front()) before using iterators.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function front(seq) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.front();\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t local iter = seq.operator __iterself__();\n"
      "\t>>\t try {\n"
      "\t>>\t\t return iter.operator __iternext__();\n"
      "\t>>\t } catch (Signal.StopIteration) {\n"
      "\t>>\t }\n"
      "\t>>\t\t throw Error.ValueError(\"...\");\n"
      "\t>> }");
  DOC(0x01BE,"__builtin_seq_back","(sequence seq) -> object\n"
      "\tGeneric, iterator-based back function.\n"
      "\tNOTE: Tries to call #(seq.back()) before using iterators.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function back(seq) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.back();\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t local result;\n"
      "\t>>\t for (local x: seq)\n"
      "\t>>\t\t result = x;\n"
      "\t>>\t if (!__builtin_bound(result))\n"
      "\t>>\t\t throw Error.ValueError(\"...\");\n"
      "\t>>\t return result;\n"
      "\t>> }");
  DOC(0x01BF,"__builtin_seq_get","(sequence seq, ssize_t idx) -> object\n"
      "\tGeneric, iterator-based getitem function.\n"
      "\tNOTE: Tries to call #(seq[idx]) before using iterators.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function get(seq,idx) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq[idx];\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t return [seq...][idx];\n"
      "\t>> }");
  DOC(0x01C0,"__builtin_seq_at","(sequence seq, size_t idx) -> object\n"
      "\tGeneric, iterator-based at function.\n"
      "\tNOTE: Tries to call #(seq.at(idx)) before using iterators.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function at(seq,idx) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.at(idx);\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t idx = copy((size_t)idx);\n"
      "\t>>\t for (local x: seq) {\n"
      "\t>>\t\t if (!idx--)\n"
      "\t>>\t\t\t return x;\n"
      "\t>>\t }\n"
      "\t>>\t throw Error.ValueError.IndexError(\"...\");\n"
      "\t>> }");
  DOC(0x01C1,"__builtin_seq_reversed","(sequence seq) -> object\n"
      "\tGeneric, iterator-based reversed function.\n"
      "\tNOTE: Tries to call #(seq.reversed()) before using iterators.\n"
      "\tNOTE: If iterators are used, a #list is returned.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function reversed(seq) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.reversed();\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t return [seq...].reversed();\n"
      "\t>> }");
  DOC(0x01C2,"__builtin_seq_sorted","(sequence seq) -> object\n"
      "(sequence seq, callable pred) -> object\n"
      "\tGeneric, iterator-based sorted function.\n"
      "\tNOTE: Tries to call #(seq.sorted()) / #(seq.sorted(pred)) before using iterators.\n"
      "\tNOTE: If iterators are used, a #list is returned.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function sorted(seq,pred...) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.sorted(pred...);\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t return [seq...].sorted(pred...);\n"
      "\t>> }");
  DOC(0x01C3,"__builtin_seq_segments","(sequence seq, size_t n) -> object\n"
      "\tGeneric, iterator-based segments function.\n"
      "\tNOTE: Tries to call #(seq.segments(n)) before using iterators.\n"
      "\tNOTE: If iterators are used, a #list is returned.\n"
      "\t>> #include <error>\n"
      "\t>> constexpr function segments(seq,n) {\n"
      "\t>>\t try {\n"
      "\t>>\t\t return seq.segments(n);\n"
      "\t>>\t } catch (Error.AttributeError) {\n"
      "\t>>\t } catch (Error.RuntimeError.NotImplemented) {\n"
      "\t>>\t }\n"
      "\t>>\t return [seq...].segments(n);\n"
      "\t>> }");



  //////////////////////////////////////////////////////////////////////////
  // FS-API Documentation
  DOC(0x009A,"__builtin_fs_getcwd","() -> string\n"
      "@throws Error.SystemError: Failed to retrieve the current working directory\n"
      "@return: The current working directory, as indicated by the OS");
  DOC(0x0199,"__builtin_fs_wgetcwd","() -> string.wide\n"
      "@throws Error.SystemError: Failed to retrieve the current working directory\n"
      "@return: The current working directory, as indicated by the OS");
  DOC(0x009B,"__builtin_fs_chdir","(string cwd) -> none\n"
      "(string.wide cwd) -> none\n"
      "@param cwd: The new current working directory\n"
      "@throws Error.SystemError: Failed to change the current working directory\n"
      "\tChanges the current working directory to @cwd");
  DOC(0x0164,"__builtin_fs_tmp","() -> string\n"
      "@throws Error.SystemError: Failed to retrieve the tmp-directory\n"
      "@return: The path to a folder that can be used for temporary files.\n"
      "\t1. Checks for a #($TMPDIR) environmental variable\n"
      "\t2. Checks for a #($TEMP) environmental variable\n"
      "\t3. Checks for a #($TMP) environmental variable\n"
      "\t4. [#__WINDOWS__] Use #GetTempPath to get the tmp path as designated by the OS\n"
      "\t4. [#__POSIX__]   Use the #P_tmpdir macro\n"
      "\t5. [#__POSIX__]   Return \"/tmp\"\n"
      "\tNOTE: If the vfs is enabled, you can use \"/tmp\" in regular paths under windows");
  DOC(0x019A,"__builtin_fs_wtmp","() -> string.wide\n"
      "@throws Error.SystemError: Failed to retrieve the tmp-directory\n"
      "@return: The path to a folder that can be used for temporary files.\n"
      "\t1. Checks for a #($TMPDIR) environmental variable\n"
      "\t2. Checks for a #($TEMP) environmental variable\n"
      "\t3. Checks for a #($TMP) environmental variable\n"
      "\t4. [#__WINDOWS__] Use #GetTempPath to get the tmp path as designated by the OS\n"
      "\t4. [#__POSIX__]   Use the #P_tmpdir macro\n"
      "\t5. [#__POSIX__]   Return \"/tmp\"\n"
      "\tNOTE: If the vfs is enabled, you can use \"/tmp\" in regular paths under windows");
  DOC(0x0168,"__builtin_fs_tmpname",
      "(string.wide path, string.wide prefix = L\"unnamed\") -> string.wide\n"
      "(string path = fs::tmp(), string prefix = \"unnamed\") -> string\n"
      "@throws Error.SystemError: Too many temporary files created\n"
      "@return: A filename that can be used for a temporary file\n"
      "\tThe filename is build like this:\n"
      "\t>> \"<path>/<prefix><XXXX>.tmp\"\n"
      "\tWhere XXXX is a four-digit hex-code used to ensure that the filename is unique.");
  DOC(0x01A3,"__builtin_fs_wtmpname",
      "(string path, string prefix = L\"unnamed\") -> string\n"
      "(string.wide path = fs::wtmp(), string.wide prefix = L\"unnamed\") -> string.wide\n"
      "@throws Error.SystemError: Too many temporary files created\n"
      "@return: A filename that can be used for a temporary file\n"
      "\tThe filename is build like this (where XXXX is a four-digit hex-code used to ensure that the filename is unique):\n"
      "\t>> \"<path>/<prefix><XXXX>.tmp\"");
  DOC(0x009C,"__builtin_fs_home",
      "(none user = none) -> string\n"
      "(string user) -> string\n"
      "(string.wide user) -> string.wide\n"
      "@throws Error.SystemError: Failed to retrieve the home directory\n"
      "@return: The home directory of a given user or the current user");
  DOC(0x019B,"__builtin_fs_whome",
      "(none user = none) -> string.wide\n"
      "(string user) -> string\n"
      "(string.wide user) -> string.wide\n"
      "@throws Error.SystemError: Failed to retrieve the home directory\n"
      "@return: The home directory of a given user or the current user");
  DOC(0x009D,"__builtin_fs_listenv","() -> [(string,string)...]\n"
      "@throws Error.SystemError: Failed to enumerate environment variables\n"
      "@return: A sequence of 2-tuples enumerating all environmental variables"
      "\tGenerates and returns a sequence of name-value pairs for all environment variables.\n"
      "\tNOTE: Changes to environment variables during, or after a call to this function may not be visible in its result");
  DOC(0x019C,"__builtin_fs_wlistenv","() -> [(string.wide,string.wide)...]\n"
      "@throws Error.SystemError: Failed to enumerate environment variables\n"
      "@return: A sequence of 2-tuples enumerating all environmental variables\n"
      "\tGenerates and returns a sequence of name-value pairs for all environment variables.\n"
      "\tNOTE: Changes to environment variables during, or after a call to this function may not be visible in its result");
  DOC(0x009E,"__builtin_fs_getenv","(string name) -> string\n"
      "(string.wide name) -> string.wide\n"
      "@throws Error.SystemError: Failed lookup the @name environmental variable\n"
      "@return: The value of the given environmental variable @name");
  DOC(0x00A0,"__builtin_fs_delenv","(string name) -> none\n"
      "(string.wide name) -> none\n"
      "@throws Error.SystemError: Failed to delete the @name environmental variable\n"
      "\tDeletes a given environmental variable @name");
  DOC(0x00A1,"__builtin_fs_setenv","(string name, string value) -> none\n"
      "(string.wide name, string.wide value) -> none\n"
      "@throws Error.SystemError: Failed to set the @name environmental variable to @value\n"
      "\tSets a given environmental variable @name to @value");
  DOC(0x0195,"__builtin_fs_user","() -> string\n"
      "@throws Error.SystemError: Failed to retrieve the name of the current user\n"
      "@return: The name of the user that the deemon process is running under");
  DOC(0x0197,"__builtin_fs_wuser","() -> string.wide\n"
      "@throws Error.SystemError: Failed to retrieve the name of the current user\n"
      "@return: The name of the user that the deemon process is running under");
  DOC(0x0196,"__builtin_fs_machine","() -> string\n"
      "@throws Error.SystemError: Failed to retrieve the name of the current machine\n"
      "@return: The name of the machine that deemon is running on");
  DOC(0x0198,"__builtin_fs_wmachine","() -> string.wide\n"
      "@throws Error.SystemError: Failed to retrieve the name of the current machine\n"
      "@return: The name of the machine that deemon is running on");

  DOC(0x00A2,"__builtin_fs_path_head","(string path) -> string\n"
      "(string.wide path) -> string.wide\n"
      "@return: The head of a path, that is the directory without the filename.\n"
      "\tReturns the head of a path, stripping away the filename and extension:\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::head(\"/foo/bar.txt\");\n"
      "\t-> /foo/\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::head(\"C:\\\\foo\\\\bar.txt\");\n"
      "\t-> C:\\foo\\");
  DOC(0x00A3,"__builtin_fs_path_tail","(string path) -> string\n"
      "(string.wide path) -> string.wide\n"
      "@return: The tail of a path, that is the filename and extension.\n"
      "\tReturns the tail of a path, stripping away the head and only leaving the filename and extension:\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::tail(\"/foo/bar.txt\");\n"
      "\t-> bar.txt\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::tail(\"C:\\\\foo\\\\bar.txt\");\n"
      "\t-> bar.txt");
  DOC(0x00A4,"__builtin_fs_path_file","(string path) -> string\n"
      "(string.wide path) -> string.wide\n"
      "@return: The file portion of a path, excluding the file head and extension.\n"
      "\tReturns the filename of a path, stripping away the head and extension:\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::file(\"/foo/bar.txt\");\n"
      "\t-> bar\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::file(\"C:\\\\foo\\\\bar.txt\");\n"
      "\t-> bar");
  DOC(0x00A5,"__builtin_fs_path_ext","(string path) -> string\n"
      "(string.wide path) -> string.wide\n"
      "@return: The extension of a filename, including the leading \".\" character.\n"
      "\tReturns the extensions of a given path, stripping away the head and filename:\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::ext(\"/foo/bar.txt\");\n"
      "\t-> .txt\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::ext(\"C:\\\\foo\\\\bar.txt\");\n"
      "\t-> .txt");
  DOC(0x00A6,"__builtin_fs_path_drive","(string path) -> string\n"
      "(string.wide path) -> string.wide\n"
      "@return: The drive portion of an absolute path on windows, or returns \"/\" on posix\n"
      "\tReturns the drive associated with a given absolute path:\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::drive(\"/foo/bar.txt\");\n"
      "\t-> /\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::drive(\"C:\\\\foo\\\\bar.txt\");\n"
      "\t-> C:\\");
  DOC(0x00A7,"__builtin_fs_path_inctrail","(string path) -> string\n"
      "(string.wide path) -> string.wide\n"
      "@return: The path with a trailing slash included\n"
      "\tReturns the given path with a trailing slash/backspace attached:\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::inctrail(\"/foo/bar/\");\n"
      "\t-> /foo/bar/\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::inctrail(\"/foo/bar\");\n"
      "\t-> /foo/bar/");
  DOC(0x00A8,"__builtin_fs_path_exctrail","(string path) -> string\n"
      "(string.wide path) -> string.wide\n"
      "@return: The path without a trailing slash included\n"
      "\tReturns the given path without a trailing slash/backspace attached:\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::inctrail(\"/foo/bar/\");\n"
      "\t-> /foo/bar\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::inctrail(\"/foo/bar\");\n"
      "\t-> /foo/bar");
  DOC(0x00A9,"__builtin_fs_path_abs","(string path, string cwd = \".\") -> string\n"
      "(string.wide path, string.wide cwd = L\".\") -> string.wide\n"
      "@return: The new path\n"
      "\tMakes @path an absolute path, using @cwd as the base point for the relative disposition.\n"
      "\tIf @path is already absolute, it will be forced relative with #(__builtin_fs_path_rel)(@path)\n"
      "\tIf @cwd is relative, if will be forced absolute with #(__builtin_fs_path_abs)(@cwd,#(__builtin_fs_getcwd)())\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::abs(\"../user/bar\",\"/home/foobar\");\n"
      "\t-> /home/user/bar");
  DOC(0x00AA,"__builtin_fs_path_rel","(string path, string cwd = \".\") -> string\n"
      "(string.wide path, string.wide cwd = L\".\") -> string.wide\n"
      "@return: The new path\n"
      "\tCreates a relative path leading to @path, when being looked at from @cwd.\n"
      "\tIf 'path' and 'cwd' are on different drives on windows, 'path' is returned unchanged.\n"
      "\tIf 'path' was already relative, if will be forced absolute with #(__builtin_fs_path_abs)(@path).\n"
      "\tIf 'cwd' is relative, if will be forced absolute with #(__builtin_fs_path_abs)(@cwd,#(__builtin_fs_getcwd)()).\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::rel(\"/home/user/bar\",\"/home/foobar\");\n"
      "\t-> ../user/bar");
  DOC(0x00AB,"__builtin_fs_path_join","(string paths...) -> string\n"
      "(string.wide paths...) -> string.wide\n"
      "@return: The joined path\n"
      "\tJoins a list of path strings into a full path.\n"
      "\tAll path elements are joined with \"\\\" on windows and \"/\" on linux, before all consecutive path seps are removed.\n"
      "\tAn implementation would look like this:\n"
      "\t>> constexpr function fs::path::join(paths...) {\n"
      "\t>> #ifdef __WINDOWS__\n"
      "\t>>\t result = \"\\\\\".join(paths...).replace(\"/\",\"\\\\\");\n"
      "\t>>\t while (\"\\\\\\\\\" in result) result = result.replace(\"\\\\\\\\\",\"\\\\\");\n"
      "\t>> #else\n"
      "\t>>\t result = \"/\".join(paths...).replace(\"\\\\\",\"/\");\n"
      "\t>>\t while (\"//\" in result) result = result.replace(\"//\",\"/\");\n"
      "\t>> #endif\n"
      "\t>>\t return result;\n"
      "\t>> }");
  DOC(0x00AC,"__builtin_fs_path_expanduser","(string path) -> string\n"
      "(string.wide path) -> string.wide\n"
      "@return: The expanded path\n"
      "@throws Error.SystemError: Failed to retrieve the home path\n"
      "\tExpand the '~' prefix indicating the home path.\n"
      "\tIf a name immediatly follows the '~' character, that name will be used as profile name instead of the standard current user name.\n"
      "\tThis function is automatically called before passing any path string to the operating system.\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::expanduser(\"~/Documents\");\n"
      "\t-> /home/awesome_user/Documents\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::path::expanduser(\"~root/Documents\");\n"
      "\t-> /root/Documents");
  DOC(0x00AD,"__builtin_fs_path_expandvars","(string path) -> string\n"
      "(string.wide path) -> string.wide\n"
      "@return: The expanded path\n"
      "@throws Error.SystemError: Failed to retrieve an env variable\n"
      "\tExpand environmental vars in a string\n"
      "\tAccepted var formats are (on all platforms):\n"
      "\t>> \"%VAR%\";  // Windows style\n"
      "\t>> \"$VAR\";   // Posix style #1\n"
      "\t>> \"${VAR}\"; // Posix style #2");

  DOC(0x00AE,"__builtin_fs_getatime",
      "(string path) -> time\n"
      "(string.wide path) -> time\n"
      "(file.io path) -> time\n"
      "@throws Error.SystemError: Failed to retrieve the time\n"
      "\tReturns the last access time of a given @(path).");
  DOC(0x00AF,"__builtin_fs_getctime",
      "(string path) -> time\n"
      "(string.wide path) -> time\n"
      "(file.io path) -> time\n"
      "@throws Error.SystemError: Failed to retrieve the time\n"
      "\tReturns the creation time of a given @(path).");
  DOC(0x00B0,"__builtin_fs_getmtime",
      "(string path) -> time\n"
      "(string.wide path) -> time\n"
      "(file.io path) -> time\n"
      "@throws Error.SystemError: Failed to retrieve the time.\n"
      "\tReturns the time of the last modification to a given @(path).");
  DOC(0x00B1,"__builtin_fs_gettimes",
      "(string path) -> (time,time,time)\n"
      "(string.wide path) -> (time,time,time)\n"
      "(file.io path) -> (time,time,time)\n"
      "@throws Error.SystemError: Failed to retrieve the time\n"
      "\tReturns a #tuple of times for a given @(path).\n"
      "\tThe returned #tuple is equivalent to (#(__builtin_fs_getatime)(path),#(__builtin_fs_getctime)(path),#(__builtin_fs_getmtime)(path)).");
  DOC(0x0180,"__builtin_fs_setatime",
      "(string path, time access_time) -> none\n"
      "(string.wide path, time access_time) -> none\n"
      "(file.io path, time access_time) -> none\n"
      "\t@throws Error.SystemError: Failed to set the time\n"
      "\tSets the @access_time of a given @path");
  DOC(0x0181,"__builtin_fs_setctime",
      "(string path, time creation_time) -> none\n"
      "(string.wide path, time creation_time) -> none\n"
      "(file.io path, time creation_time) -> none\n"
      "@throws Error.SystemError: Failed to set the time\n"
      "\tSets the @creation_time of a given @path\n"
      "\tNOTE: no-op on unix-based operating systems");
  DOC(0x0182,"__builtin_fs_setmtime",
      "(string path, time modification_time) -> none\n"
      "(string.wide path, time modification_time) -> none\n"
      "(file.io path, time modification_time) -> none\n"
      "@throws Error.SystemError: Failed to set the time\n"
      "\tSets the @modification_time of a given @path");
  DOC(0x0183,"__builtin_fs_settimes",
      "(string path, time access_time = none, time creation_time = none, time modification_time = none) -> none\n"
      "(string.wide path, time access_time = none, time creation_time = none, time modification_time = none) -> none\n"
      "(file.io path, time access_time = none, time creation_time = none, time modification_time = none) -> none\n"
      "@throws Error.SystemError: Failed to set the time\n"
      "\tSets a the times for a given @path");
  DOC(0x00B2,"__builtin_fs_isabs",
      "(string path) -> bool\n"
      "(string.wide path) -> bool\n"
      "@return: #true if the given path is absolute, #false otherwise");
  DOC(0x00B3,"__builtin_fs_isfile",
      "(string path) -> bool\n"
      "(string.wide path) -> bool\n"
      "(file.io path) -> bool\n"
      "@throws Error.SystemError: Failed to stat the given @path\n"
      "@return: #true if @path is a file, #false otherwise");
  DOC(0x00B4,"__builtin_fs_isdir",
      "(string path) -> bool\n"
      "(string.wide path) -> bool\n"
      "(file.io path) -> bool\n"
      "@throws Error.SystemError: Failed to stat the given @path\n"
      "@return: #true if @path is a directory, #false otherwise");
  DOC(0x00B5,"__builtin_fs_islink",
      "(string path) -> bool\n"
      "(string.wide path) -> bool\n"
      "(file.io path) -> bool\n"
      "@throws Error.SystemError: Failed to stat the given @path\n"
      "@return: #true if @path is a symbolic link, #false otherwise");
  DOC(0x00B6,"__builtin_fs_isdrive",
      "(string path) -> bool\n"
      "(string.wide path) -> bool\n"
      "(file.io path) -> bool\n"
      "@return: #true if @path is a drive, #false otherwise");
  DOC(0x00B7,"__builtin_fs_ismount",
      "(string path) -> bool\n"
      "(string.wide path) -> bool\n"
      "(file.io path) -> bool\n"
      "@throws Error.SystemError: Failed to stat the given @path\n"
      "@return: #true if @path is a mount point, #false otherwise");
  DOC(0x00B8,"__builtin_fs_ishidden",
      "(string path) -> bool\n"
      "(string.wide path) -> bool\n"
      "(file.io path) -> bool\n"
      "@throws Error.SystemError: Failed to stat the given @path\n"
      "@return: #true if @path is hidden, #false otherwise");
  DOC(0x00E7,"__builtin_fs_isxfile",
      "(string path) -> bool\n"
      "(string.wide path) -> bool\n"
      "(file.io path) -> bool\n"
      "@throws Error.SystemError: Failed to stat the given @path\n"
      "@return: #true if @path is executable, #false otherwise");
  DOC(0x016E,"__builtin_fs_ischrdev",
      "(string path) -> bool\n"
      "(string.wide path) -> bool\n"
      "(file.io path) -> bool\n"
      "@throws Error.SystemError: Failed to stat the given @path\n"
      "@return: #true if @path is a character device, #false otherwise");
  DOC(0x016F,"__builtin_fs_isblkdev",
      "(string path) -> bool\n"
      "(string.wide path) -> bool\n"
      "(file.io path) -> bool\n"
      "@throws Error.SystemError: Failed to stat the given @path\n"
      "@return: #true if @path is a block device, #false otherwise");
  DOC(0x0170,"__builtin_fs_isfifo",
      "(string path) -> bool\n"
      "(string.wide path) -> bool\n"
      "(file.io path) -> bool\n"
      "@throws Error.SystemError: Failed to stat the given @path\n"
      "@return: #true if @path is a named pipe, #false otherwise");
  DOC(0x0171,"__builtin_fs_issocket",
      "(string path) -> bool\n"
      "(string.wide path) -> bool\n"
      "(file.io path) -> bool\n"
      "@throws Error.SystemError: Failed to stat the given @path\n"
      "@return: #true if @path is a socket, #false otherwise");
  DOC(0x00B9,"__builtin_fs_exists",
      "(string path) -> bool\n"
      "(string.wide path) -> bool\n"
      "@throws Error.SystemError: Failed to stat the given @path\n"
      "@return: #true if @path exists (is a file or directory). #false otherwise");
  DOC(0x00BA,"__builtin_fs_remove",
      "(string path) -> none\n"
      "(string.wide path) -> none\n"
      "@throws Error.SystemError: Failed to remove the given file/path\n"
      "\tRemoves a given @path (can be a file or directory).");
  DOC(0x00BB,"__builtin_fs_rmfile",
      "(string path) -> none\n"
      "(string.wide path) -> none\n"
      "@throws Error.SystemError: Failed to remove the given file\n"
      "\tRemoves a given @path (must be a file).");
  DOC(0x00BC,"__builtin_fs_mkdir",
      "(string path, mode_t mode = 0755) -> none\n"
      "(string.wide path, mode_t mode = 0755) -> none\n"
      "@throws Error.SystemError: Failed to create the given path\n"
      "\tCreates a new directory @path with @(mode) as permissions.");
  DOC(0x00BD,"__builtin_fs_rmdir",
      "(string path) -> none\n"
      "(string.wide path) -> none\n"
      "@throws Error.SystemError: Failed to remove the given @path\n"
      "\tRemoves a given @path (must be a directory).");
  DOC(0x00BE,"__builtin_fs_copy",
      "(string src, string dst) -> none\n"
      "(string.wide src, string.wide dst) -> none\n"
      "(string src, string dst, callable progress) -> none\n"
      "(string.wide src, string.wide dst, callable progress) -> none\n"
      "@throws Error.SystemError: Failed to copy the given file\n"
      "\tCopies a file locates at @src to @(dst).\n"
      "\tThe progress callback is executed like this:\n"
      "\t>> progress((double)x); // where 'x' is in 0.0 - 1.0\n"
      "\tIf given, @progress will be executed at least twice with 0.0 beforehand and 1.0 afterwards.\n"
      "\tNOTE: For forward compatibility, it is recommended that @progress take optional variadic arguments:\n"
      "\t>> function on_progress(prct,...) {\n"
      "\t>>\t print \"\rProgress:\",(int)(prct*100),;\n"
      "\t>> }\n"
      "\t>> fs::copy_(src,dst,on_progress);\n"
      "\t>> print;");
  DOC(0x00BF,"__builtin_fs_move",
      "(string src, string dst) -> none\n"
      "(string.wide src, string.wide dst) -> none\n"
      "@throws Error.SystemError: Failed to move/rename the given file\n"
      "\tMoves/renames a file locates at @src to @(dst).");
  DOC(0x00C0,"__builtin_fs_link",
      "(string src, string dst) -> none\n"
      "(string.wide src, string.wide dst) -> none\n"
      "@throws Error.SystemError: Failed to link the given file\n"
      "\tCreates a link to @src at @(dst).\n"
      "\tNOTE: Under windows, the process must be granted the \"SeCreateSymbolicLinkPrivilege\" right.");
  DOC(0x00C1,"__builtin_fs_readlink",
      "(string path) -> string\n"
      "(string.wide path) -> string.wide\n"
      "@throws Error.SystemError: Failed to link the read the given link\n"
      "\tReads a link located at @path and returns its destination");

  DOC(0x01C5,"__builtin_fs_chmod",
      "(string path, string mode) -> none\n"
      "(string path, mode_t mode) -> none\n"
      "(string.wide path, string mode) -> none\n"
      "(string.wide path, mode_t mode) -> none\n"
      "(file.io path, string mode) -> none\n"
      "(file.io path, mode_t mode) -> none\n"
      "@throws Error.SystemError: Failed to change access to the given @path\n"
      "@throws Error.ValueError: Invalid @mode string\n"
      "\tChanges permissions of a given @path.\n"
      "\tOn windows the only thing you're sadly able to do, is to control the read-only "
        "flag of the associated @path, which is set/removed if nobody has write-access ((@mode & 0222) == 0).\n"
      "\tMode can be a string that follows the same format as the chmod cmd command. e.g.:\n"
      "\t>> chmod(\"file.txt\",\"+x\"); // Add execute permissions for everyone");
  DOC(0x01C6,"__builtin_fs_getmod",
      "(string path) -> mode_t\n"
      "(string.wide path) -> mode_t\n"
      "(file.io path) -> mode_t\n"
      "@throws Error.SystemError: Failed to stat the given @path\n"
      "@return: Permissions of the given @path");
  DOC(0x01C7,"__builtin_fs_chown",
      "(string path, string user_and_group) -> none\n"
      "(string path, string user, string group) -> none\n"
      "(string path, string user, gid_t group) -> none\n"
      "(string path, uid_t user, string group) -> none\n"
      "(string path, uid_t user, gid_t group) -> none\n"
      "(string.wide path, string.wide user_and_group) -> none\n"
      "(string.wide path, string.wide user, string.wide group) -> none\n"
      "(string.wide path, string.wide user, gid_t group) -> none\n"
      "(string.wide path, uid_t user, string.wide group) -> none\n"
      "(string.wide path, uid_t user, gid_t group) -> none\n"
      "(file.io path, string user, string group) -> none\n"
      "(file.io path, string user, gid_t group) -> none\n"
      "(file.io path, uid_t user, string group) -> none\n"
      "(file.io path, uid_t user, gid_t group) -> none\n"
      "(file.io path, string.wide user, string.wide group) -> none\n"
      "(file.io path, string.wide user, gid_t group) -> none\n"
      "(file.io path, uid_t user, string.wide group) -> none\n"
      "@throws Error.SystemError: Failed to change access to the given @path\n"
      "\tChanges ownership of a given @path\n"
      "\tUser and group can be a string describing a password database entry, or can be the #uid_t or #gid_t directly.\n"
      "\tUsing this function on windows requires deemon to run as administrator or to be given the #(\"SeRestorePrivilege\") privilege.\n"
      "\tThe @user_and_group parameter should separate its user and group with a ':', where the the owner should be listed first\n"
      "\t>> #include <fs>\n"
      "\t>> fs::chmod(\"my_file\",\"root:www-data\");");
  DOC(0x01C8,"__builtin_fs_getown",
      "(string file) -> (uid_t,gid_t)\n"
      "(string.wide file) -> (uid_t,gid_t)\n"
      "(file.io file) -> (uid_t,gid_t)\n"
      "@throws Error.SystemError: Failed to stat the given @file\n"
      "@return: The owner and group of a given @file");
  DOC(0x01C9,"__builtin_fs_uidof",
      "(string user) -> uid_t\n"
      "(string user) -> win32_sid\n"
      "(string.wide user) -> uid_t\n"
      "(string.wide user) -> win32_sid\n"
      "@throws Error.SystemError: Failed to retrieve the #uid_t to a given username\n"
      "@return: The #uid_t associated with a given user (On windows, a #win32_sid is returned).\n"
      "\tOn windows, an SID-object is returned that can be used everywhere a #uid_t is required.\n"
      "\tThe user name may contain an optional domain name that should be appended alongside an '@' character.\n"
      "\t>> #include <fs>\n"
      "\t>> fs::uidof(\"cool@dudepc\");");
  DOC(0x01CA,"__builtin_fs_gidof",
      "(string group) -> gid_t\n"
      "(string group) -> win32_sid\n"
      "(string.wide group) -> gid_t\n"
      "(string.wide group) -> win32_sid\n"
      "@throws Error.SystemError: Failed to retrieve the #gid_t to a given group name\n"
      "@return: The #gid_t associated with a given group (On windows, a #win32_sid is returned).\n"
      "\tOn windows, an SID-object is returned that can be used everywhere a #gid_t is required.\n"
      "\tThe user name may contain an optional domain name that should be appended alongside an '@' character.\n"
      "\t>> #include <fs>\n"
      "\t>> fs::gidof(\"cool@dudepc\");");
  DOC(0x01CB,"__builtin_fs_uidname",
      "(uid_t uid) -> string\n"
      "@throws Error.SystemError: Failed to retrieve the name of a given user @uid\n"
      "@return: the name of a given user @uid");
  DOC(0x01CC,"__builtin_fs_wuidname",
      "(uid_t uid) -> string.wide\n"
      "@throws Error.SystemError: Failed to retrieve the name of a given user @uid\n"
      "@return: the name of a given user @uid");

  DOC(0x01CD,"__builtin_fs_gidname",
      "(gid_t gid) -> string\n"
      "@throws Error.SystemError: Failed to retrieve the name of a given group @gid\n"
      "@return: the name of a given group @gid");
  DOC(0x01CE,"__builtin_fs_wgidname",
      "(gid_t gid) -> string.wide\n"
      "@throws Error.SystemError: Failed to retrieve the name of a given group @gid\n"
      "@return: the name of a given group @gid");

#ifdef DEE_PLATFORM_WINDOWS
  DOC(0x00ED,"__builtin_fs_win32hardlink",
      "(string src, string dst) -> none\n"
      "(string.wide src, string.wide dst) -> none\n"
      "@throws Error.SystemError: Failed to create a hard link\n"
      "\tCreates a win32 hardlink from @src to @(dst).");
  DOC(0x00EE,"__builtin_fs_win32modulename","(none *mod) -> string\n"
      "@throws Error.SystemError: Failed to retrive the name of the given module @mod\n"
      "@return: The name of the given module @(mod).");
  DOC(0x019D,"__builtin_fs_win32wmodulename","(none *mod) -> string.wide\n"
      "@throws Error.SystemError: Failed to retrive the name of the given module @mod\n"
      "@return: The name of the given module @(mod).");
  DOC(0x0162,"__builtin_fs_win32dlldirectory","() -> string\n"
      "@throws Error.SystemError: Failed to retrive the dll directory\n"
      "@return: The Win32 DllDirectory\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::win32::_dlldirectory();\n"
      "\t->");
  DOC(0x019E,"__builtin_fs_win32wdlldirectory","() -> string.wide\n"
      "@throws Error.SystemError: Failed to retrive the dll directory\n"
      "@return: The Win32 DllDirectory\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::win32::_wdlldirectory();\n"
      "\t->");
  DOC(0x0163,"__builtin_fs_win32systemdirectory","() -> string\n"
      "@throws Error.SystemError: Failed to retrive the system directory\n"
      "\t    Returns the Win32 SystemDirectory.\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::win32::_systemdirectory();\n"
      "\t-> C:\\Windows\\System32");
  DOC(0x019F,"__builtin_fs_win32wsystemdirectory","() -> string.wide\n"
      "@throws Error.SystemError: Failed to retrive the system directory\n"
      "\t    Returns the Win32 SystemDirectory.\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::win32::_wsystemdirectory();\n"
      "\t-> C:\\Windows\\System32");
  DOC(0x0165,"__builtin_fs_win32windowsdirectory","() -> string\n"
      "@throws Error.SystemError: Failed to retrive the windows directory\n"
      "@return: The Win32 WindowsDirectory.\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::win32::_windowsdirectory();\n"
      "\t-> C:\\Windows");
  DOC(0x01A0,"__builtin_fs_win32wwindowsdirectory",
      "() -> string.wide\n"
      "@throws Error.SystemError: Failed to retrive the windows directory\n"
      "@return: The Win32 WindowsDirectory.\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::win32::_wwindowsdirectory();\n"
      "\t-> C:\\Windows");
  DOC(0x0166,"__builtin_fs_win32systemwindowsdirectory","() -> string\n"
      "@throws Error.SystemError: Failed to retrive the system windows directory\n"
      "@return: The Win32 SystemWindowsDirectory\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::win32::_systemwindowsdirectory();\n"
      "\t-> C:\\Windows");
  DOC(0x01A1,"__builtin_fs_win32wsystemwindowsdirectory","() -> string.wide\n"
      "@throws Error.SystemError: Failed to retrive the system windows directory\n"
      "@return: The Win32 SystemWindowsDirectory\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::win32::_wsystemwindowsdirectory();\n"
      "\t-> C:\\Windows");
  DOC(0x0167,"__builtin_fs_win32syswow64directory","() -> string\n"
      "@throws Error.SystemError: Failed to retrive the syswow64 directory\n"
      "@return: The Win32 SysWow64Directory\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::win32::_syswow64directory();\n"
      "\t-> C:\\Windows\\SysWOW64");
  DOC(0x01A2,"__builtin_fs_win32wsyswow64directory","() -> string.wide\n"
      "@throws Error.SystemError: Failed to retrive the syswow64 directory\n"
      "@return: The Win32 SysWow64Directory\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::win32::_wsyswow64directory();\n"
      "\t-> C:\\Windows\\SysWOW64");
  DOC(0x01CF,"__builtin_fs_win32drives","() -> [string...]\n"
      "@throws Error.SystemError: Couldn't generate list of drives\n"
      "@return: A sequence of available drives\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::win32::drives();\n"
      "\t-> [C:\\,D:\\]");
  DOC(0x01D0,"__builtin_fs_win32wdrives","() -> [string.wide...]\n"
      "@throws Error.SystemError: Couldn't generate list of drives\n"
      "@return: A sequence of available drives\n"
      "\t>> #include <fs>\n"
      "\t>> print fs::win32::drives();\n"
      "\t-> [C:\\,D:\\]");
#endif

  DOC(0x00C3,NULL,
      "(string path) -> dir\n"
      "(string.wide path) -> dir\n"
      "(file.io path) -> dir\n"
      "@return: Returns a new sequence that can be used to iterate all files and folders in a given @path\n"
      "@throws Error.SystemError: Failed to stat the given @path\n"
      "\tThe pathnames yieled by the associated iterator have the same encoding as the given @(path).\n"
      "\t>> #include <fs>\n"
      "\t>> function ls(path) {\n"
      "\t>>\t for (local name: fs::dir(path)) {\n"
      "\t>>\t\t print fs::path::join(path,name);\n"
      "\t>>\t }\n"
      "\t>> }\n"
      "\t>> ls(\"~\");\n"
      "\t-> Documents\n"
      "\t-> Downloads\n"
      "\t-> Pictures\n"
      "\t-> Videos\n"
      "\t-> ...");
  DOC(0x00EB,NULL,
      "(string query) -> query\n"
      "(string.wide query) -> query\n"
      "@throws Error.SystemError: Failed to stat the path of the given @query\n"
      "\tLike #dir, but the constructor takes a wildcard-enabled @query string "
        "used to determine which items should be displayed.\n"
      "\t>> #include <fs>\n"
      "\t>> // List all posix-style hidden objects in the user home directory\n"
      "\t>> for (local f: fs::query(\"~/.*\")) {\n"
      "\t>>\t print f;\n"
      "\t>> }\n"
      "\tEquivalent to:\n"
      "\t>> #include <fs>\n"
      "\t>> for (local f: fs::dir(\"~\")) {\n"
      "\t>>\t if (f.wmatch(\".*\")) {\n"
      "\t>>\t\t print f;\n"
      "\t>>\t }\n"
      "\t>> }\n"
      "\tNOTE: If the query is a widestring, items are yielded as widestrings");

  DOC(0x0119,"__builtin_sin",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x011A,"__builtin_cos",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x011B,"__builtin_tan",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x011C,"__builtin_sincos",
      "(float x) -> (float,float)\n"
      "(double x) -> (double,double)\n"
      "(long double x) -> (long double,long double)");
  DOC(0x011D,"__builtin_asin",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x011E,"__builtin_acos",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x011F,"__builtin_atan",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0120,"__builtin_atan2",
      "(float y, float x) -> float\n"
      "(double y, double x) -> double\n"
      "(long double y, long double x) -> long double");
  DOC(0x0121,"__builtin_asincos",
      "(float x) -> (float,float)\n"
      "(double x) -> (double,double)\n"
      "(long double x) -> (long double,long double)");
  DOC(0x0122,"__builtin_sinh",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0123,"__builtin_cosh",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0124,"__builtin_tanh",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0125,"__builtin_sincosh",
      "(float x) -> (float,float)\n"
      "(double x) -> (double,double)\n"
      "(long double x) -> (long double,long double)");
  DOC(0x0126,"__builtin_asinh",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0127,"__builtin_acosh",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0128,"__builtin_atanh",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0129,"__builtin_asincosh",
      "(float x) -> (float,float)\n"
      "(double x) -> (double,double)\n"
      "(long double x) -> (long double,long double)");
  DOC(0x012A,"__builtin_exp",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x012B,"__builtin_frexp",
      "(float x) -> (float,int)\n"
      "(double x) -> (double,int)\n"
      "(long double x) -> (long double,int)");
  DOC(0x012C,"__builtin_log",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x012D,"__builtin_log10",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x012E,"__builtin_modf",
      "(float x) -> (float,float)\n"
      "(double x) -> (double,double)\n"
      "(long double x) -> (long double,long double)");
  DOC(0x012F,"__builtin_exp2",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0130,"__builtin_expm1",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0131,"__builtin_ilogb",
      "(float x) -> int\n"
      "(double x) -> int\n"
      "(long double x) -> int");
  DOC(0x0132,"__builtin_log1p",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0133,"__builtin_log2",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0134,"__builtin_logb",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0135,"__builtin_scalbn",
      "(float x, int n) -> float\n"
      "(double x, int n) -> double\n"
      "(long double x, int n) -> long double");
  DOC(0x0136,"__builtin_scalbi32n",
      "(float x, int32_t n) -> float\n"
      "(double x, int32_t n) -> double\n"
      "(long double x, int32_t n) -> long double");
  DOC(0x0055,"__builtin_pow",
      "(float x, float y) -> float\n"
      "(double x, double y) -> double\n"
      "(long double x, long double y) -> long double");
  DOC(0x0137,"__builtin_sqrt",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0138,"__builtin_cbrt",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0139,"__builtin_hypot",
      "(float x, float y) -> float\n"
      "(double x, double y) -> double\n"
      "(long double x, long double y) -> long double");
  DOC(0x013A,"__builtin_erf",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x013B,"__builtin_tgamma",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x013C,"__builtin_lgamma",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x013D,"__builtin_ceilf",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x013E,"__builtin_floorf",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x013F,"__builtin_truncf",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0140,"__builtin_roundf",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0141,"__builtin_i32round",
      "(float x) -> int32_t\n"
      "(double x) -> int32_t\n"
      "(long double x) -> int32_t");
  DOC(0x0142,"__builtin_i64round",
      "(float x) -> int64_t\n"
      "(double x) -> int64_t\n"
      "(long double x) -> int64_t");
  DOC(0x0143,"__builtin_fmod",
      "(float number, float denom) -> float\n"
      "(double number, double denom) -> double\n"
      "(long double number, long double denom) -> long double");
  DOC(0x0144,"__builtin_rint",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0145,"__builtin_i32rint",
      "(float x) -> int32_t\n"
      "(double x) -> int32_t\n"
      "(long double x) -> int32_t");
  DOC(0x0146,"__builtin_i64rint",
      "(float x) -> int64_t\n"
      "(double x) -> int64_t\n"
      "(long double x) -> int64_t");
  DOC(0x0147,"__builtin_nearbyint",
      "(float x) -> float\n"
      "(double x) -> double\n"
      "(long double x) -> long double");
  DOC(0x0148,"__builtin_remainder",
      "(float number, float denom) -> float\n"
      "(double number, double denom) -> double\n"
      "(long double number, long double denom) -> long double");
  DOC(0x0149,"__builtin_remquo",
      "(float x, float y) -> (float,int)\n"
      "(double x, double y) -> (double,int)\n"
      "(long double x, long double y) -> (long double,int)");
  DOC(0x014A,"__builtin_copysign",
      "(float x, float y) -> float\n"
      "(double x, double y) -> double\n"
      "(long double x, long double y) -> long double");
  DOC(0x014B,"__builtin_nextafter",
      "(float x, float y) -> float\n"
      "(double x, double y) -> double\n"
      "(long double x, long double y) -> long double");
  DOC(0x014C,"__builtin_nexttoward",
      "(float x, long double y) -> float\n"
      "(double x, long double y) -> double\n"
      "(long double x, long double y) -> long double");
  DOC(0x014D,"__builtin_fdim",
      "(float x, float y) -> float\n"
      "(double x, double y) -> double\n"
      "(long double x, long double y) -> long double");
  DOC(0x014E,"__builtin_isfinite",
      "(float x) -> bool\n"
      "(double x) -> bool\n"
      "(long double x) -> bool");
  DOC(0x014F,"__builtin_isinf",
      "(float x) -> bool\n"
      "(double x) -> bool\n"
      "(long double x) -> bool");
  DOC(0x0150,"__builtin_isnan",
      "(float x) -> bool\n"
      "(double x) -> bool\n"
      "(long double x) -> bool");
  DOC(0x0151,"__builtin_isnormal",
      "(float x) -> bool\n"
      "(double x) -> bool\n"
      "(long double x) -> bool");
  DOC(0x0152,"__builtin_signbit",
      "(float x) -> bool\n"
      "(double x) -> bool\n"
      "(long double x) -> bool");
  DOC(0x0153,"__builtin_isgreater",
      "(float x, float y) -> bool\n"
      "(double x, double y) -> bool\n"
      "(long double x, long double y) -> bool");
  DOC(0x0154,"__builtin_isgreaterequal",
      "(float x, float y) -> bool\n"
      "(double x, double y) -> bool\n"
      "(long double x, long double y) -> bool");
  DOC(0x0155,"__builtin_isless",
      "(float x, float y) -> bool\n"
      "(double x, double y) -> bool\n"
      "(long double x, long double y) -> bool");
  DOC(0x0156,"__builtin_islessequal",
      "(float x, float y) -> bool\n"
      "(double x, double y) -> bool\n"
      "(long double x, long double y) -> bool");
  DOC(0x0157,"__builtin_islessgreater",
      "(float x, float y) -> bool\n"
      "(double x, double y) -> bool\n"
      "(long double x, long double y) -> bool");
  DOC(0x0158,"__builtin_isunordered",
      "(float x, float y) -> bool\n"
      "(double x, double y) -> bool\n"
      "(long double x, long double y) -> bool");

#if DEE_CONFIG_RUNTIME_HAVE_EXCEPTIONS
  DOC(0x002E,"__builtin_last_exception",
      "() -> object\n"
      "(thread t) -> object");
  DOC(0x002F,"__builtin_last_traceback",
      "() -> traceback\n"
      "(thread t) -> traceback");
#endif

  DOC(0x01D7,"__dual_posoradd__",
      "(object ob) -> object\n"
      "@return: The result of #(+ob)\n"
      "(object lhs, object rhs) -> object\n"
      "@return: The result of #(lhs+rhs)\n"
      "\tRuntime-overloading of #(operator +) with the argument count not known at compile-time.");
  DOC(0x01D8,"__dual_negorsub__",
      "(object ob) -> object\n"
      "@return: The result of #(-ob)\n"
      "(object lhs, object rhs) -> object\n"
      "@return: The result of #(lhs-rhs)\n"
      "\tRuntime-overloading of #(operator -) with the argument count not known at compile-time.");
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  DOC(0x01D9,"__dual_dereformul__",
      "(object ob) -> object\n"
      "@return: The result of #(*ob)\n"
      "(object lhs, object rhs) -> object\n"
      "@return: The result of #(lhs*rhs)\n"
      "\tRuntime-overloading of #(operator *) with the argument count not known at compile-time.");
  DOC(0x01DA,"__dual_reforand__",
      "(object ob) -> object\n"
      "@return: The result of #(&ob)\n"
      "(object lhs, object rhs) -> object\n"
      "@return: The result of #(lhs&rhs)\n"
      "\tRuntime-overloading of #(operator &) with the argument count not known at compile-time.");
#endif /* !DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  DOC(0x01DB,"__dual_getitemorgetrange__",
      "(object ob, object key) -> object\n"
      "@return: The result of #(ob[key])\n"
      "(object ob, object begin, object end) -> object\n"
      "@return: The result of #(ob[begin:end])\n"
      "\tRuntime-overloading of #(operator []) with the argument count not known at compile-time.");
  DOC(0x01DC,"__dual_delitemordelrange__",
      "(object ob, object key) -> object\n"
      "@return: The result of #(del ob[key];)\n"
      "(object ob, object begin, object end) -> object\n"
      "@return: The result of #(del ob[begin:end];)\n"
      "\tRuntime-overloading of #(operator del []) with the argument count not known at compile-time.");
  DOC(0x01DD,"__dual_setitemorsetrange__",
      "(object ob, object key, object value) -> object\n"
      "@return: The result of #(ob[key] = value)\n"
      "(object ob, object begin, object end, object value) -> object\n"
      "@return: The result of #(ob[begin:end] = value)\n"
      "\tRuntime-overloading of #(operator []=) with the argument count not known at compile-time.");

  DOC(0x0161,"__builtin_argv",
      "() -> iterable\n"
      "@return: A sequence of all arguments passed from the commandline to the usercode\n"
      "\tNOTE: The returned sequence may not actually be the commandline in embedded environments");


  DOC(0x01DE,"__builtin_dex",
      "(string name) -> dexmodule\n"
      "@throws: Error.RuntimeError.DexError: Failed to find the specified dex\n"
      "@throws: Error.ValueError: The specified dex is broken\n"
      "@param name: The name of the module that should be loaded\n"
      "@return: The loaded dex module\n"
      "\tLooks up the dex module associated with a given @(name).\n"
      "\tIf no module matching the given @name was loaded, the module "
        "will be searched for, loaded and then returned.");

#if DEE_CONFIG_RUNTIME_HAVE_EXTERN
  DOC(0x01E0,"__builtin_extern",
      "(string object_name, type object object_type) -> object_type &\n"
      "(string libfile, string object_name, type object object_type) -> object_type &\n"
      "(string.wide libfile, string object_name, type object object_type) -> object_type &\n"
      "@return: An lvalue pointing to the newly loaded object\n"
      "\tLoads a specified object @object_name, either from the pool of "
        "linked libraries, or from an explicitly specified @(libfile).");
#endif /* DEE_CONFIG_RUNTIME_HAVE_EXTERN */
#if DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE
  DOC(0x01E1,"__builtin_ff_closure",
      "(type object ff_type, callable callback) -> foreign_function_closure\n"
      "\tCreates a new foreign-function closure, callable through a native function "
        "pointer, as specified by @ff_type and executing @(callback).");
#endif /* DEE_CONFIG_RUNTIME_HAVE_FOREIGNFUNCTIONCLOSURE */
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
  DOC(0x0086,"__builtin_pointer_add",
      "(structured_type ty) -> pointer_type\n"
      "@throws Error.TypeError: The specified type @ty isn't a viable #structured_type\n"
      "@return: The result of #(ty *)");
  DOC(0x0087,"__builtin_pointer_rem",
      "(pointer_type ty) -> structured_type\n"
      "@throws Error.TypeError: The specified type @ty isn't a viable #pointer_type\n"
      "@return: The base of a given #pointer_type @ty");
  DOC(0x0088,"__builtin_lvalue_add",
      "(structured_type ty) -> lvalue_type\n"
      "@throws Error.TypeError: The specified type @ty isn't a viable #structured_type\n"
      "@return: The result of #(ty &)");
  DOC(0x0089,"__builtin_lvalue_rem",
      "(lvalue_type ty) -> structured_type\n"
      "@throws Error.TypeError: The specified type @ty isn't a viable #lvalue_type\n"
      "@return: The base of a given #lvalue_type @ty");
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
  DOC(0x00C2,"__builtin_assertion_failed",
      "(object expr = none, object msg = none) -> none\n"
      "@throws Error.RuntimeError.AssertionError: An assertion has failed\n"
      "\tThis function does not return normally, but instead throws an assertion-error.\n"
      "\tThis builtin function is implicitly called whenever an assertion statement fails in usercode:\n"
      "\t>> volatile local fails = false;\n"
      "\t>> assert fails: \"Oh No!\"; // Implicitly calls the assertion-failure handler");

 DOC(0x00E6,"__builtin_gc_collect_now",
     "() -> size_t\n"
     "@return: A symbolic value indicating how much garbage was collected.\n"
     "\tCollects garbage and return non-zero if at least ~something~ was done.");
 DOC(0x0173,"__builtin_class",
     "(type object base = object, string name = \"\") -> class\n"
     "@param base: The base for the class to-be created\n"
     "@param name: An optional name for the class\n"
     "@return: The new class object\n"
     "\tCreates a new class with the specified base and name.\n"
     "\tThis function is used internally by the runtime.");


 DOC(0x00EF,"__builtin_exit",
     "(int exitcode) -> none\n"
     "@param exitcode: The exit-code signaled to the hosting system\n"
     "@return: This function does not return\n"
     "\tTermintes the process that is running deemon, signaling @exitcode to the system as return value.\n"
     "\tNOTE: Callbacks registered with #__builtin_atexit will be executed.\n"
     "\tWarning: This function might not be available in some embedded environments.");
 DOC(0x00F0,"__builtin__exit",
     "(int exitcode) -> none\n"
     "@param exitcode: The exit-code signaled to the hosting system\n"
     "@return: This function does not return\n"
     "\tTermintes the process that is running deemon, signaling @exitcode to the system as return value.\n"
     "\tNOTE: No callbacks will be executed.\n"
     "\tWarning: This function might not be available in some embedded environments.");
 DOC(0x00F1,"__builtin_fatal",
     "() -> none\n"
     "@return: This function does not return\n"
     "\tTermintes the process that is running deemon, signaling a host-specific return "
       "value that indicates a fatal application error to the system as return value.\n"
     "\tNOTE: No callbacks will be executed.\n"
     "\tNOTE: Change in behavior: prior to v101, callbacks registered with #__builtin_atexit were executed.\n"
     "\tWarning: This function might not be available in some embedded environments.");
 DOC(0x00F2,"__builtin_atexit",
     "(callable func, object args...) -> none\n"
     "@param func: The function that should be executed before shutdown\n"
     "@param args: A variadic list of argument passed to @func during execution\n"
     "\tRegister a callback that will either be executed when #__builtin_exit is called, or when deemon exits normally\n"
     "\tWarning: This function might not be available in some embedded environments.");
#undef DOC
  default: break;
 }
 return NULL;
}


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_DOCGEN_AUTODOC_C_INL */
