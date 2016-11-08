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

#ifndef DEE_STRING_TYPE_NAME
#error "Must #define DEE_STRING_TYPE_NAME before including this file"
#endif

#ifndef DEE_STRING_ENC
#error "Must #define DEE_STRING_ENC before including this file"
#endif

#define DEE_STRING_MEMBER(name) DEE_PP_CAT_4(Dee,DEE_STRING_ENC,String_,name)

 public: /* public types */
  typedef DEE_PP_CAT_3(Dee_,DEE_STRING_ENC,Char) character;
 public: /* public methods */
  inline DEE_A_RET_WUNUSED character const *c_str() const DEE_CXX11_NOEXCEPT { return DEE_STRING_MEMBER(STR)(this->ob_ptr); }
 public: /* public methods */
  inline DEE_A_RET_WUNUSED Dee_size_t size() const DEE_CXX11_NOEXCEPT { return DEE_STRING_MEMBER(SIZE)(this->ob_ptr); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME capitalize() const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(Capitalize)(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME center(DEE_A_IN Dee_size_t width) const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(Center)(this->ob_ptr,width,(character)' '),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME center(DEE_A_IN Dee_size_t width, DEE_A_IN character fill_char) const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(Center)(this->ob_ptr,width,fill_char),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_IN DEE_STRING_TYPE_NAME const &sub) const DEE_CXX11_NOEXCEPT { return ::DEE_STRING_MEMBER(Count)(this->ob_ptr,sub,0,DEE_STRING_MEMBER(SIZE)(this->ob_ptr)); }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_IN DEE_STRING_TYPE_NAME const &sub, DEE_A_IN Dee_size_t begin) const DEE_CXX11_NOEXCEPT { return ::DEE_STRING_MEMBER(Count)(this->ob_ptr,sub,begin,DEE_STRING_MEMBER(SIZE)(this->ob_ptr)); }
  inline DEE_A_RET_WUNUSED Dee_size_t count(DEE_A_IN DEE_STRING_TYPE_NAME const &sub, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) const DEE_CXX11_NOEXCEPT { return ::DEE_STRING_MEMBER(Count)(this->ob_ptr,sub,begin,end); }
  inline DEE_A_RET_WUNUSED bool endswith(DEE_A_IN DEE_STRING_TYPE_NAME const &sub) const DEE_CXX11_NOEXCEPT { return !!::DEE_STRING_MEMBER(EndsWith)(this->ob_ptr,sub,0,DEE_STRING_MEMBER(SIZE)(this->ob_ptr)); }
  inline DEE_A_RET_WUNUSED bool endswith(DEE_A_IN DEE_STRING_TYPE_NAME const &sub, DEE_A_IN Dee_size_t begin) const DEE_CXX11_NOEXCEPT { return !!::DEE_STRING_MEMBER(EndsWith)(this->ob_ptr,sub,begin,DEE_STRING_MEMBER(SIZE)(this->ob_ptr)); }
  inline DEE_A_RET_WUNUSED bool endswith(DEE_A_IN DEE_STRING_TYPE_NAME const &sub, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) const DEE_CXX11_NOEXCEPT { return !!::DEE_STRING_MEMBER(EndsWith)(this->ob_ptr,sub,begin,end); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME expandtabs() const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(ExpandTabs)(this->ob_ptr,8),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME expandtabs(DEE_A_IN Dee_size_t tabsize) const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(ExpandTabs)(this->ob_ptr,tabsize),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED bool empty() const DEE_CXX11_NOEXCEPT { return DEE_STRING_MEMBER(SIZE)(this->ob_ptr) == 0; }
  inline DEE_A_RET_WUNUSED bool non_empty() const DEE_CXX11_NOEXCEPT { return DEE_STRING_MEMBER(SIZE)(this->ob_ptr) != 0; }
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_IN DEE_STRING_TYPE_NAME const &sub) const DEE_CXX11_NOEXCEPT { return ::DEE_STRING_MEMBER(Find)(this->ob_ptr,sub,0,DEE_STRING_MEMBER(SIZE)(this->ob_ptr)); }
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_IN DEE_STRING_TYPE_NAME const &sub, DEE_A_IN Dee_size_t begin) const DEE_CXX11_NOEXCEPT { return ::DEE_STRING_MEMBER(Find)(this->ob_ptr,sub,begin,DEE_STRING_MEMBER(SIZE)(this->ob_ptr)); }
  inline DEE_A_RET_WUNUSED Dee_ssize_t find(DEE_A_IN DEE_STRING_TYPE_NAME const &sub, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) const DEE_CXX11_NOEXCEPT { return ::DEE_STRING_MEMBER(Find)(this->ob_ptr,sub,begin,end); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME format(DEE_A_IN tuple const &args) const;
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_IN DEE_STRING_TYPE_NAME const &sub) const { Dee_size_t result; if ((result = ::DEE_STRING_MEMBER(Index)(this->ob_ptr,sub,0,DEE_STRING_MEMBER(SIZE)(this->ob_ptr))) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_IN DEE_STRING_TYPE_NAME const &sub, DEE_A_IN Dee_size_t begin) const { Dee_size_t result; if ((result = ::DEE_STRING_MEMBER(Index)(this->ob_ptr,sub,begin,DEE_STRING_MEMBER(SIZE)(this->ob_ptr))) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t index(DEE_A_IN DEE_STRING_TYPE_NAME const &sub, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) const { Dee_size_t result; if ((result = ::DEE_STRING_MEMBER(Index)(this->ob_ptr,sub,begin,end)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED bool isalnum() const DEE_CXX11_NOEXCEPT { return !!DEE_STRING_MEMBER(IsAlnum)(this->ob_ptr); }
  inline DEE_A_RET_WUNUSED bool isalpha() const DEE_CXX11_NOEXCEPT { return !!DEE_STRING_MEMBER(IsAlpha)(this->ob_ptr); }
  inline DEE_A_RET_WUNUSED bool isdigit() const DEE_CXX11_NOEXCEPT { return !!DEE_STRING_MEMBER(IsDigit)(this->ob_ptr); }
  inline DEE_A_RET_WUNUSED bool islower() const DEE_CXX11_NOEXCEPT { return !!DEE_STRING_MEMBER(IsLower)(this->ob_ptr); }
  inline DEE_A_RET_WUNUSED bool isspace() const DEE_CXX11_NOEXCEPT { return !!DEE_STRING_MEMBER(IsSpace)(this->ob_ptr); }
  inline DEE_A_RET_WUNUSED bool istitle() const DEE_CXX11_NOEXCEPT { return !!DEE_STRING_MEMBER(IsTitle)(this->ob_ptr); }
  inline DEE_A_RET_WUNUSED bool isupper() const DEE_CXX11_NOEXCEPT { return !!DEE_STRING_MEMBER(IsUpper)(this->ob_ptr); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME join(DEE_A_INOUT DeeObject *seq) const DEE_CXX11_NOEXCEPT { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(Join)(this->ob_ptr,seq),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME ljust(DEE_A_IN Dee_size_t width) const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(LJust)(this->ob_ptr,width,' '),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME ljust(DEE_A_IN Dee_size_t width, DEE_A_IN character fill_char) const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(LJust)(this->ob_ptr,width,fill_char),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME lower() const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(Lower)(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME lstrip() const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(LStrip)(this->ob_ptr,DEE_STRING_TYPE_NAME(DEE_EMPTY_CHARS_STR)),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME lstrip(DEE_A_IN DEE_STRING_TYPE_NAME const &chars) const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(LStrip)(this->ob_ptr,chars),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED tuple partition(DEE_A_IN DEE_STRING_TYPE_NAME const &sep) const;
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME replace(DEE_A_IN DEE_STRING_TYPE_NAME const &find, DEE_A_IN DEE_STRING_TYPE_NAME const &replace) const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(Replace)(this->ob_ptr,find,replace),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED Dee_ssize_t rfind(DEE_A_IN DEE_STRING_TYPE_NAME const &sub) const DEE_CXX11_NOEXCEPT { return ::DEE_STRING_MEMBER(RFind)(this->ob_ptr,sub,0,DEE_STRING_MEMBER(SIZE)(this->ob_ptr)); }
  inline DEE_A_RET_WUNUSED Dee_ssize_t rfind(DEE_A_IN DEE_STRING_TYPE_NAME const &sub, DEE_A_IN Dee_size_t begin) const DEE_CXX11_NOEXCEPT { return ::DEE_STRING_MEMBER(RFind)(this->ob_ptr,sub,begin,DEE_STRING_MEMBER(SIZE)(this->ob_ptr)); }
  inline DEE_A_RET_WUNUSED Dee_ssize_t rfind(DEE_A_IN DEE_STRING_TYPE_NAME const &sub, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) const DEE_CXX11_NOEXCEPT { return ::DEE_STRING_MEMBER(RFind)(this->ob_ptr,sub,begin,end); }
  inline DEE_A_RET_WUNUSED Dee_size_t rindex(DEE_A_IN DEE_STRING_TYPE_NAME const &sub) const { Dee_size_t result; if ((result = ::DEE_STRING_MEMBER(RIndex)(this->ob_ptr,sub,0,DEE_STRING_MEMBER(SIZE)(this->ob_ptr))) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t rindex(DEE_A_IN DEE_STRING_TYPE_NAME const &sub, DEE_A_IN Dee_size_t begin) const { Dee_size_t result; if ((result = ::DEE_STRING_MEMBER(RIndex)(this->ob_ptr,sub,begin,DEE_STRING_MEMBER(SIZE)(this->ob_ptr))) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED Dee_size_t rindex(DEE_A_IN DEE_STRING_TYPE_NAME const &sub, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) const { Dee_size_t result; if ((result = ::DEE_STRING_MEMBER(RIndex)(this->ob_ptr,sub,begin,end)) == (Dee_size_t)-1) detail::throw_last_error(); return result; }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME rjust(DEE_A_IN Dee_size_t width) const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(RJust)(this->ob_ptr,width,(character)' '),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME rjust(DEE_A_IN Dee_size_t width, DEE_A_IN character fill_char) const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(RJust)(this->ob_ptr,width,fill_char),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED tuple rpartition(DEE_A_IN DEE_STRING_TYPE_NAME const &sep) const;
  inline DEE_A_RET_WUNUSED list rsplit(DEE_A_IN DEE_STRING_TYPE_NAME const &sep) const;
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME rstrip() const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(RStrip)(this->ob_ptr,DEE_STRING_TYPE_NAME(DEE_EMPTY_CHARS_STR)),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME rstrip(DEE_A_IN DEE_STRING_TYPE_NAME const &chars) const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(RStrip)(this->ob_ptr,chars),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED list split(DEE_A_IN DEE_STRING_TYPE_NAME const &sep) const;
  inline DEE_A_RET_WUNUSED list splitlines() const;
  inline DEE_A_RET_WUNUSED list splitlines(DEE_A_IN bool keepends) const;
  inline DEE_A_RET_WUNUSED bool startswith(DEE_A_IN DEE_STRING_TYPE_NAME const &sub) const DEE_CXX11_NOEXCEPT { return !!::DEE_STRING_MEMBER(StartsWith)(this->ob_ptr,sub,0,DEE_STRING_MEMBER(SIZE)(this->ob_ptr)); }
  inline DEE_A_RET_WUNUSED bool startswith(DEE_A_IN DEE_STRING_TYPE_NAME const &sub, DEE_A_IN Dee_size_t begin) const DEE_CXX11_NOEXCEPT { return !!::DEE_STRING_MEMBER(StartsWith)(this->ob_ptr,sub,begin,DEE_STRING_MEMBER(SIZE)(this->ob_ptr)); }
  inline DEE_A_RET_WUNUSED bool startswith(DEE_A_IN DEE_STRING_TYPE_NAME const &sub, DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) const DEE_CXX11_NOEXCEPT { return !!::DEE_STRING_MEMBER(StartsWith)(this->ob_ptr,sub,begin,end); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME strip() const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(Strip)(this->ob_ptr,DEE_STRING_TYPE_NAME(DEE_EMPTY_CHARS_STR)),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME strip(DEE_A_IN DEE_STRING_TYPE_NAME const &chars) const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(Strip)(this->ob_ptr,chars),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME swapcase() const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(SwapCase)(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME title() const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(Title)(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME upper() const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(Upper)(this->ob_ptr),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME zfill(DEE_A_IN Dee_size_t width) const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(ZFill)(this->ob_ptr,width),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED bool wmatch(DEE_A_IN DEE_STRING_TYPE_NAME const &pattern) const { return !!::DEE_PP_CAT_3(Dee_,DEE_STRING_ENC,StrWMatchLen)(DEE_STRING_MEMBER(SIZE)(this->ob_ptr),DEE_STRING_MEMBER(STR)(this->ob_ptr),DEE_STRING_MEMBER(SIZE)(pattern),DEE_STRING_MEMBER(STR)(pattern)); }
  inline DEE_A_RET_WUNUSED bool wmatch(DEE_A_IN_Z character const *pattern) const { return !!::DEE_PP_CAT_3(Dee_,DEE_STRING_ENC,StrWMatchLen)(DEE_STRING_MEMBER(SIZE)(this->ob_ptr),DEE_STRING_MEMBER(STR)(this->ob_ptr),DEE_PP_CAT_3(Dee_,DEE_STRING_ENC,StrLen)(pattern),pattern); }
  inline DEE_A_RET_WUNUSED bool wmatch(DEE_A_IN Dee_size_t pattern_size, DEE_A_IN_R(pattern_size) character const *pattern) const { return !!::DEE_PP_CAT_3(Dee_,DEE_STRING_ENC,StrWMatchLen)(DEE_STRING_MEMBER(SIZE)(this->ob_ptr),DEE_STRING_MEMBER(STR)(this->ob_ptr),pattern_size,pattern); }
  inline DEE_A_RET_WUNUSED bool casewmatch(DEE_A_IN DEE_STRING_TYPE_NAME const &pattern) const { return !!::DEE_PP_CAT_3(Dee_,DEE_STRING_ENC,StrCaseWMatchLen)(DEE_STRING_MEMBER(SIZE)(this->ob_ptr),DEE_STRING_MEMBER(STR)(this->ob_ptr),DEE_STRING_MEMBER(SIZE)(pattern),DEE_STRING_MEMBER(STR)(pattern)); }
  inline DEE_A_RET_WUNUSED bool casewmatch(DEE_A_IN_Z character const *pattern) const { return !!::DEE_PP_CAT_3(Dee_,DEE_STRING_ENC,StrCaseWMatchLen)(DEE_STRING_MEMBER(SIZE)(this->ob_ptr),DEE_STRING_MEMBER(STR)(this->ob_ptr),DEE_PP_CAT_3(Dee_,DEE_STRING_ENC,StrLen)(pattern),pattern); }
  inline DEE_A_RET_WUNUSED bool casewmatch(DEE_A_IN Dee_size_t pattern_size, DEE_A_IN_R(pattern_size) character const *pattern) const { return !!::DEE_PP_CAT_3(Dee_,DEE_STRING_ENC,StrCaseWMatchLen)(DEE_STRING_MEMBER(SIZE)(this->ob_ptr),DEE_STRING_MEMBER(STR)(this->ob_ptr),pattern_size,pattern); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME sstrip(DEE_A_IN DEE_STRING_TYPE_NAME const &chars) const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(SStrip)(this->ob_ptr,chars),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME lsstrip(DEE_A_IN DEE_STRING_TYPE_NAME const &chars) const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(LSStrip)(this->ob_ptr,chars),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME rsstrip(DEE_A_IN DEE_STRING_TYPE_NAME const &chars) const { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(RSStrip)(this->ob_ptr,chars),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME &substr() DEE_CXX11_NOEXCEPT { return *this; }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME const &substr() const DEE_CXX11_NOEXCEPT { return *this; }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME substr(DEE_A_IN Dee_size_t begin) const DEE_CXX11_NOEXCEPT { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(NewWithLength)(DEE_STRING_MEMBER(SIZE)(this->ob_ptr)-begin,DEE_STRING_MEMBER(STR)(this->ob_ptr)+begin),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME substr(DEE_A_IN Dee_size_t begin, DEE_A_IN Dee_size_t end) const DEE_CXX11_NOEXCEPT { return DEE_STRING_TYPE_NAME(::DEE_STRING_MEMBER(NewWithLength)((Dee_size_t)(end-begin),DEE_STRING_MEMBER(STR)(this->ob_ptr)+begin),detail::tag_ref_or_err()); }
  inline DEE_A_RET_WUNUSED int compare(DEE_A_IN DEE_STRING_TYPE_NAME const &right) const DEE_CXX11_NOEXCEPT { return DEE_PP_CAT_3(Dee_,DEE_STRING_ENC,StrCmpLen)(DEE_STRING_MEMBER(SIZE)(this->ob_ptr),DEE_STRING_MEMBER(STR)(this->ob_ptr),DEE_STRING_MEMBER(SIZE)(right.ob_ptr),DEE_STRING_MEMBER(STR)(right.ob_ptr)); }
  inline DEE_A_RET_WUNUSED int compare(DEE_A_IN Dee_size_t pos, DEE_A_IN Dee_size_t len, DEE_A_IN DEE_STRING_TYPE_NAME const &right) const DEE_CXX11_NOEXCEPT { return DEE_PP_CAT_3(Dee_,DEE_STRING_ENC,StrCmpLen)(len,DEE_STRING_MEMBER(STR)(this->ob_ptr)+pos,DEE_STRING_MEMBER(SIZE)(right.ob_ptr),DEE_STRING_MEMBER(STR)(right.ob_ptr)); }
  inline DEE_A_RET_WUNUSED int compare(DEE_A_IN Dee_size_t pos, DEE_A_IN Dee_size_t len, DEE_A_IN DEE_STRING_TYPE_NAME const &right, DEE_A_IN Dee_size_t sub_pos, DEE_A_IN Dee_size_t sub_len) const DEE_CXX11_NOEXCEPT { return DEE_PP_CAT_3(Dee_,DEE_STRING_ENC,StrCmpLen)(len,DEE_STRING_MEMBER(STR)(this->ob_ptr)+pos,sub_len,DEE_STRING_MEMBER(STR)(right.ob_ptr)+sub_pos); }
  inline DEE_A_RET_WUNUSED list segments(DEE_A_IN Dee_size_t n) const { return list(::DEE_STRING_MEMBER(Segments)(this->ob_ptr,n),detail::tag_ref_or_err()); }
 public: /* public operators */
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME operator + (DEE_A_IN character const *right) const {
   return DEE_STRING_TYPE_NAME(DEE_STRING_MEMBER(ConcatWithLength)(this->ob_ptr,DEE_PP_CAT_3(Dee_,DEE_STRING_ENC,StrLen)(right),right),detail::tag_ref_or_err());
  }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME operator + (DEE_A_IN DEE_STRING_TYPE_NAME const &right) const {
   return DEE_STRING_TYPE_NAME(DEE_STRING_MEMBER(ConcatWithLength)(this->ob_ptr,DEE_STRING_MEMBER(SIZE)(right),DEE_STRING_MEMBER(STR)(right)),detail::tag_ref_or_err());
  }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME operator + (DEE_A_INOUT DeeObject *right) const {
   DeeObject *result;
   if ((right = ::DeeObject_Str(right)) == NULL) detail::throw_last_error();
   result = DEE_STRING_MEMBER(ConcatWithLength)(this->ob_ptr,DEE_STRING_MEMBER(SIZE)(right),DEE_STRING_MEMBER(STR)(right));
   Dee_DECREF(right);
   return DEE_STRING_TYPE_NAME(result,detail::tag_ref_or_err());
  }
  inline DEE_STRING_TYPE_NAME &operator += (DEE_A_IN character const *right) {
   this->priv_assign_ptr(DEE_STRING_MEMBER(ConcatWithLength)(this->ob_ptr,DEE_PP_CAT_3(Dee_,DEE_STRING_ENC,StrLen)(right),right),detail::tag_ref_or_err());
   return *this;
  }
  inline DEE_STRING_TYPE_NAME &operator += (DEE_A_IN DEE_STRING_TYPE_NAME const &right) {
   this->priv_assign_ptr(DEE_STRING_MEMBER(ConcatWithLength)(this->ob_ptr,DEE_STRING_MEMBER(SIZE)(right),DEE_STRING_MEMBER(STR)(right)),detail::tag_ref_or_err());
   return *this;
  }
  inline DEE_STRING_TYPE_NAME &operator += (DEE_A_INOUT DeeObject *right) {
   if (DeeObject_InplaceAddVar(&this->ob_ptr,right) == -1) detail::throw_last_error();
   return *this;
  }
  inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME operator * (DEE_A_IN Dee_size_t n) const {
   return DEE_STRING_TYPE_NAME(DEE_STRING_MEMBER(Repeat)(this->ob_ptr,n),detail::tag_ref_or_err());
  }
  inline DEE_STRING_TYPE_NAME &operator *= (DEE_A_IN Dee_size_t n) {
   this->priv_assign_ptr(DEE_STRING_MEMBER(Repeat)(this->ob_ptr,n),detail::tag_ref_or_err());
   return *this;
  }
 public: /* public methods */
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
  static inline DEE_A_RET_WUNUSED DEE_STRING_TYPE_NAME format(DEE_A_IN_PRINTF character const *fmt, ...) {
   DeeObject *result; va_list args; DEE_VA_START(args,fmt);
   result = DEE_STRING_MEMBER(VNewf)(fmt,args); DEE_VA_END(args);
   return DEE_STRING_TYPE_NAME(result,detail::tag_ref_or_err());
  }
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */
 public: /* public constructor / destructor */
  inline DEE_STRING_TYPE_NAME() DEE_CXX11_NOEXCEPT: object(DEE_PP_CAT_3(Dee_Empty,DEE_STRING_ENC,String)) {}
  inline DEE_STRING_TYPE_NAME(DEE_A_IN_Z ::Dee_WideChar const *str): object(::DEE_STRING_MEMBER(FromWideString)(str),detail::tag_ref_or_err()) {}
  inline DEE_STRING_TYPE_NAME(DEE_A_IN_Z ::Dee_Utf8Char const *str): object(::DEE_STRING_MEMBER(FromUtf8String)(str),detail::tag_ref_or_err()) {}
  inline DEE_STRING_TYPE_NAME(DEE_A_IN_Z ::Dee_Utf16Char const *str): object(::DEE_STRING_MEMBER(FromUtf16String)(str),detail::tag_ref_or_err()) {}
  inline DEE_STRING_TYPE_NAME(DEE_A_IN_Z ::Dee_Utf32Char const *str): object(::DEE_STRING_MEMBER(FromUtf32String)(str),detail::tag_ref_or_err()) {}
  inline explicit DEE_STRING_TYPE_NAME(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) ::Dee_WideChar const *str): object(::DEE_STRING_MEMBER(FromWideStringWithLength)(len,str),detail::tag_ref_or_err()) {}
  inline explicit DEE_STRING_TYPE_NAME(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) ::Dee_Utf8Char const *str): object(::DEE_STRING_MEMBER(FromUtf8StringWithLength)(len,str),detail::tag_ref_or_err()) {}
  inline explicit DEE_STRING_TYPE_NAME(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) ::Dee_Utf16Char const *str): object(::DEE_STRING_MEMBER(FromUtf16StringWithLength)(len,str),detail::tag_ref_or_err()) {}
  inline explicit DEE_STRING_TYPE_NAME(DEE_A_IN Dee_size_t len, DEE_A_IN_R(len) ::Dee_Utf32Char const *str): object(::DEE_STRING_MEMBER(FromUtf32StringWithLength)(len,str),detail::tag_ref_or_err()) {}
#if DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H
  inline explicit DEE_STRING_TYPE_NAME(DEE_A_IN_Z character const *fmt, DEE_A_IN va_list args): object(::DEE_STRING_MEMBER(VNewf)(fmt,args),detail::tag_ref_or_err()) {}
#endif /* DEE_ENVIRONMENT_HAVE_INCLUDE_STDARG_H */
  DEE_OBJECT_SUBCLASS_DEFAULT_CTOR(DEE_STRING_TYPE_NAME,object,DEE_STRING_MEMBER(Check))

#undef DEE_STRING_MEMBER
#undef DEE_STRING_TYPE_NAME
#undef DEE_STRING_ENC
