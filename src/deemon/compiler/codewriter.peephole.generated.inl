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

#ifdef OP_LOAD_NONE
 TARGET(OP_LOAD_NONE) {
  RULE("OP_LOAD_NONE,OP_RET",
       "OP_RETNONE,OP_NOOP") {
   if (RT_PROTECTED() || (RT_GET_OP() != OP_RET)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_RETNONE);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_LOAD_NONE */

#ifdef OP_ROT_2
 TARGET(OP_ROT_2) {
  RULE("OP_ROT_2,OP_ROT_2",
       "OP_NOOP,OP_NOOP") {
   if (RT_PROTECTED() || (RT_GET_OP() != OP_ROT_2)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_ROT_2 */

#ifdef OP_LROT_3
 TARGET(OP_LROT_3) {
  RULE("OP_LROT_3,OP_RROT_3",
       "OP_NOOP,OP_NOOP") {
   if (RT_PROTECTED() || (RT_GET_OP() != OP_RROT_3)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_LROT_3 */

#ifdef OP_STORE_RET
 TARGET(OP_STORE_RET) {
  RULE("OP_STORE_RET,OP_POP",
       "OP_STORE_RET_POP,OP_NOOP") {
   if (RT_PROTECTED() || (RT_GET_OP() != OP_POP)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_STORE_RET_POP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_STORE_RET */

#ifdef OP_STORE_RET_POP
 TARGET(OP_STORE_RET_POP) {
  RULE("OP_STORE_RET_POP,OP_LOAD_RET",
       "OP_STORE_RET,OP_NOOP") {
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_RET)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_STORE_RET);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_STORE_RET_POP,OP_LOAD_CST|OP_LOAD_CST_COPY|OP_LOAD_CST_DCOPY|OP_LOAD_LOC|OP_LOAD_ARG|OP_LOAD_BLTIN,$locid2,OP_LOAD_RET",
       "OP_STORE_RET,*,$locid2,OP_ROT_2") {
   Dee_uint16_t locid2;
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_CST && RT_GET_OP() != OP_LOAD_CST_COPY && RT_GET_OP() != OP_LOAD_CST_DCOPY && RT_GET_OP() != OP_LOAD_LOC && RT_GET_OP() != OP_LOAD_ARG && RT_GET_OP() != OP_LOAD_BLTIN)) RULE_BREAK(); RT_NEXT_OP();
   locid2 = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_RET)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_STORE_RET);
   RT_WRITE_SKIPOP();
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_ROT_2);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_STORE_RET_POP */

#ifdef OP_DUP
 TARGET(OP_DUP) {
  RULE("OP_DUP,OP_POP",
       "OP_NOOP,OP_NOOP") {
   if (RT_PROTECTED() || (RT_GET_OP() != OP_POP)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_DUP,OP_ROT_2",
       "*,OP_NOOP") {
   if (RT_PROTECTED() || (RT_GET_OP() != OP_ROT_2)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_SKIPOP();
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_DUP,OP_YIELD|OP_PRINT_ONE|OP_PRINT_ONE_SEP|OP_PRINT_ONE_END|OP_PRINT_ALL|OP_PRINT_ALL_SEP|OP_PRINT_ALL_END|OP_PRINT_END_F,OP_POP",
       "OP_NOOP,*,OP_NOOP") {
   if (RT_PROTECTED() || (RT_GET_OP() != OP_YIELD && RT_GET_OP() != OP_PRINT_ONE && RT_GET_OP() != OP_PRINT_ONE_SEP && RT_GET_OP() != OP_PRINT_ONE_END && RT_GET_OP() != OP_PRINT_ALL && RT_GET_OP() != OP_PRINT_ALL_SEP && RT_GET_OP() != OP_PRINT_ALL_END && RT_GET_OP() != OP_PRINT_END_F)) RULE_BREAK(); RT_NEXT_OP();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_POP)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_SKIPOP();
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_DUP */

#ifdef OP_LROT_4
 TARGET(OP_LROT_4) {
  RULE("OP_LROT_4,OP_RROT_4",
       "OP_NOOP,OP_NOOP") {
   if (RT_PROTECTED() || (RT_GET_OP() != OP_RROT_4)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_LROT_4 */

#ifdef OP_RROT_3
 TARGET(OP_RROT_3) {
  RULE("OP_RROT_3,OP_LROT_3",
       "OP_NOOP,OP_NOOP") {
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LROT_3)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_RROT_3 */

#ifdef OP_RROT_4
 TARGET(OP_RROT_4) {
  RULE("OP_RROT_4,OP_LROT_4",
       "OP_NOOP,OP_NOOP") {
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LROT_4)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_RROT_4 */

#ifdef OP_LOAD_LOC
 TARGET(OP_LOAD_LOC) {
  RULE("OP_LOAD_LOC,$arg,OP_POP",
       "OP_NOOP,OP_NOOP,OP_NOOP,OP_NOOP") {
   RT_SKIP_ARG(); // arg
   if (RT_PROTECTED() || (RT_GET_OP() != OP_POP)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_LOAD_LOC,$arg,OP_LOAD_LOC,$arg",
       "*,$arg,OP_DUP,OP_NOOP,OP_NOOP") {
   Dee_uint16_t arg;
   arg = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_LOC)) RULE_BREAK(); RT_NEXT_OP();
   if (arg != RT_READ_ARG()) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_SKIPOP();
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_DUP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_LOAD_LOC */

#ifdef OP_LOAD_REF
 TARGET(OP_LOAD_REF) {
  RULE("OP_LOAD_REF,$arg,OP_POP",
       "OP_NOOP,OP_NOOP,OP_NOOP,OP_NOOP") {
   RT_SKIP_ARG(); // arg
   if (RT_PROTECTED() || (RT_GET_OP() != OP_POP)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_LOAD_REF,$arg,OP_LOAD_REF,$arg",
       "*,$arg,OP_DUP,OP_NOOP,OP_NOOP") {
   Dee_uint16_t arg;
   arg = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_REF)) RULE_BREAK(); RT_NEXT_OP();
   if (arg != RT_READ_ARG()) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_SKIPOP();
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_DUP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_LOAD_REF */

#ifdef OP_LOAD_CST
 TARGET(OP_LOAD_CST) {
  RULE("OP_LOAD_CST,$arg,OP_POP",
       "OP_NOOP,OP_NOOP,OP_NOOP,OP_NOOP") {
   RT_SKIP_ARG(); // arg
   if (RT_PROTECTED() || (RT_GET_OP() != OP_POP)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_LOAD_CST,$arg,OP_LOAD_CST,$arg",
       "*,$arg,OP_DUP,OP_NOOP,OP_NOOP") {
   Dee_uint16_t arg;
   arg = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_CST)) RULE_BREAK(); RT_NEXT_OP();
   if (arg != RT_READ_ARG()) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_SKIPOP();
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_DUP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_LOAD_CST */

#ifdef OP_LOAD_CST_COPY
 TARGET(OP_LOAD_CST_COPY) {
  RULE("OP_LOAD_CST_COPY,$arg,OP_POP",
       "OP_NOOP,OP_NOOP,OP_NOOP,OP_NOOP") {
   RT_SKIP_ARG(); // arg
   if (RT_PROTECTED() || (RT_GET_OP() != OP_POP)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_LOAD_CST_COPY,$arg,OP_LOAD_CST_COPY,$arg",
       "*,$arg,OP_DUP,OP_NOOP,OP_NOOP") {
   Dee_uint16_t arg;
   arg = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_CST_COPY)) RULE_BREAK(); RT_NEXT_OP();
   if (arg != RT_READ_ARG()) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_SKIPOP();
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_DUP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_LOAD_CST_COPY */

#ifdef OP_LOAD_CST_DCOPY
 TARGET(OP_LOAD_CST_DCOPY) {
  RULE("OP_LOAD_CST_DCOPY,$arg,OP_POP",
       "OP_NOOP,OP_NOOP,OP_NOOP,OP_NOOP") {
   RT_SKIP_ARG(); // arg
   if (RT_PROTECTED() || (RT_GET_OP() != OP_POP)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_LOAD_CST_DCOPY,$arg,OP_LOAD_CST_DCOPY,$arg",
       "*,$arg,OP_DUP,OP_NOOP,OP_NOOP") {
   Dee_uint16_t arg;
   arg = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_CST_DCOPY)) RULE_BREAK(); RT_NEXT_OP();
   if (arg != RT_READ_ARG()) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_SKIPOP();
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_DUP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_LOAD_CST_DCOPY */

#ifdef OP_LOAD_BLTIN
 TARGET(OP_LOAD_BLTIN) {
  RULE("OP_LOAD_BLTIN,$arg,OP_POP",
       "OP_NOOP,OP_NOOP,OP_NOOP,OP_NOOP") {
   RT_SKIP_ARG(); // arg
   if (RT_PROTECTED() || (RT_GET_OP() != OP_POP)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_LOAD_BLTIN,$arg,OP_LOAD_BLTIN,$arg",
       "*,$arg,OP_DUP,OP_NOOP,OP_NOOP") {
   Dee_uint16_t arg;
   arg = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_BLTIN)) RULE_BREAK(); RT_NEXT_OP();
   if (arg != RT_READ_ARG()) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_SKIPOP();
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_DUP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_LOAD_BLTIN */

#ifdef OP_LOAD_ARG
 TARGET(OP_LOAD_ARG) {
  RULE("OP_LOAD_ARG,$arg,OP_POP",
       "OP_NOOP,OP_NOOP,OP_NOOP,OP_NOOP") {
   RT_SKIP_ARG(); // arg
   if (RT_PROTECTED() || (RT_GET_OP() != OP_POP)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_LOAD_ARG,$arg,OP_LOAD_ARG,$arg",
       "*,$arg,OP_DUP,OP_NOOP,OP_NOOP") {
   Dee_uint16_t arg;
   arg = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_ARG)) RULE_BREAK(); RT_NEXT_OP();
   if (arg != RT_READ_ARG()) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_SKIPOP();
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_DUP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_LOAD_ARG */

#ifdef OP_STORE_LOC
 TARGET(OP_STORE_LOC) {
  RULE("OP_STORE_LOC,$locid,OP_POP",
       "OP_STORE_LOC_POP,$locid,OP_NOOP") {
   Dee_uint16_t locid;
   locid = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_POP)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_STORE_LOC_POP);
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_STORE_LOC */

#ifdef OP_STORE_LOC_POP
 TARGET(OP_STORE_LOC_POP) {
  RULE("OP_STORE_LOC_POP,$locid,OP_LOAD_LOC,$locid",
       "OP_STORE_LOC,$locid,OP_NOOP,OP_NOOP,OP_NOOP") {
   Dee_uint16_t locid;
   locid = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_LOC)) RULE_BREAK(); RT_NEXT_OP();
   if (locid != RT_READ_ARG()) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_STORE_LOC);
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_STORE_LOC_POP,$locid,OP_LOAD_CST|OP_LOAD_CST_COPY|OP_LOAD_CST_DCOPY|OP_LOAD_LOC|OP_LOAD_ARG|OP_LOAD_BLTIN,$locid2,OP_LOAD_LOC,$locid",
       "OP_STORE_LOC,$locid,*,$locid2,OP_ROT_2,OP_NOOP,OP_NOOP") {
   Dee_uint16_t locid,locid2;
   locid = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_CST && RT_GET_OP() != OP_LOAD_CST_COPY && RT_GET_OP() != OP_LOAD_CST_DCOPY && RT_GET_OP() != OP_LOAD_LOC && RT_GET_OP() != OP_LOAD_ARG && RT_GET_OP() != OP_LOAD_BLTIN)) RULE_BREAK(); RT_NEXT_OP();
   locid2 = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_LOC)) RULE_BREAK(); RT_NEXT_OP();
   if (locid != RT_READ_ARG()) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_STORE_LOC);
   RT_WRITE_SKIPARG();
   RT_WRITE_SKIPOP();
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_ROT_2);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_STORE_LOC_POP */

#ifdef OP_STORE_ARG
 TARGET(OP_STORE_ARG) {
  RULE("OP_STORE_ARG,$argid,OP_POP",
       "OP_STORE_ARG_POP,$argid,OP_NOOP") {
   Dee_uint16_t argid;
   argid = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_POP)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_STORE_ARG_POP);
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_STORE_ARG */

#ifdef OP_STORE_ARG_POP
 TARGET(OP_STORE_ARG_POP) {
  RULE("OP_STORE_ARG_POP,$argid,OP_LOAD_ARG,$argid",
       "OP_STORE_ARG,$argid,OP_NOOP,OP_NOOP,OP_NOOP") {
   Dee_uint16_t argid;
   argid = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_ARG)) RULE_BREAK(); RT_NEXT_OP();
   if (argid != RT_READ_ARG()) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_STORE_ARG);
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_STORE_ARG_POP,$locid,OP_LOAD_CST|OP_LOAD_CST_COPY|OP_LOAD_CST_DCOPY|OP_LOAD_LOC|OP_LOAD_ARG|OP_LOAD_BLTIN,$locid2,OP_LOAD_ARG,$locid",
       "OP_STORE_ARG,$locid,*,$locid2,OP_ROT_2,OP_NOOP,OP_NOOP") {
   Dee_uint16_t locid,locid2;
   locid = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_CST && RT_GET_OP() != OP_LOAD_CST_COPY && RT_GET_OP() != OP_LOAD_CST_DCOPY && RT_GET_OP() != OP_LOAD_LOC && RT_GET_OP() != OP_LOAD_ARG && RT_GET_OP() != OP_LOAD_BLTIN)) RULE_BREAK(); RT_NEXT_OP();
   locid2 = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_ARG)) RULE_BREAK(); RT_NEXT_OP();
   if (locid != RT_READ_ARG()) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_STORE_ARG);
   RT_WRITE_SKIPARG();
   RT_WRITE_SKIPOP();
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_ROT_2);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_STORE_ARG_POP */

#ifdef OP_STORE_CST
 TARGET(OP_STORE_CST) {
  RULE("OP_STORE_CST,$cstid,OP_POP",
       "OP_STORE_CST_POP,$cstid,OP_NOOP") {
   Dee_uint16_t cstid;
   cstid = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_POP)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_STORE_CST_POP);
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_STORE_CST */

#ifdef OP_STORE_CST_POP
 TARGET(OP_STORE_CST_POP) {
  RULE("OP_STORE_CST_POP,$cstid,OP_LOAD_CST|OP_LOAD_CST_COPY|OP_LOAD_CST_DCOPY,$cstid",
       "OP_STORE_CST,$cstid,OP_NOOP,OP_NOOP,OP_NOOP") {
   Dee_uint16_t cstid;
   cstid = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_CST && RT_GET_OP() != OP_LOAD_CST_COPY && RT_GET_OP() != OP_LOAD_CST_DCOPY)) RULE_BREAK(); RT_NEXT_OP();
   if (cstid != RT_READ_ARG()) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_STORE_CST);
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_STORE_CST_POP,$locid,OP_LOAD_CST|OP_LOAD_CST_COPY|OP_LOAD_CST_DCOPY|OP_LOAD_LOC|OP_LOAD_ARG|OP_LOAD_BLTIN,$locid2,OP_LOAD_CST|OP_LOAD_CST_COPY|OP_LOAD_CST_DCOPY,$locid",
       "OP_STORE_CST,$locid,*,$locid2,OP_ROT_2,OP_NOOP,OP_NOOP") {
   Dee_uint16_t locid,locid2;
   locid = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_CST && RT_GET_OP() != OP_LOAD_CST_COPY && RT_GET_OP() != OP_LOAD_CST_DCOPY && RT_GET_OP() != OP_LOAD_LOC && RT_GET_OP() != OP_LOAD_ARG && RT_GET_OP() != OP_LOAD_BLTIN)) RULE_BREAK(); RT_NEXT_OP();
   locid2 = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LOAD_CST && RT_GET_OP() != OP_LOAD_CST_COPY && RT_GET_OP() != OP_LOAD_CST_DCOPY)) RULE_BREAK(); RT_NEXT_OP();
   if (locid != RT_READ_ARG()) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_STORE_CST);
   RT_WRITE_SKIPARG();
   RT_WRITE_SKIPOP();
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_ROT_2);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_STORE_CST_POP */

#ifdef OP_TUPLE
 TARGET(OP_TUPLE) {
  RULE("OP_TUPLE,$size,OP_CAST_TUPLE",
       "OP_TUPLE,$size,OP_NOOP") {
   Dee_uint16_t size;
   size = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_CAST_TUPLE)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_SKIPOP();
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_TUPLE,$size,OP_CAST_LIST",
       "OP_LIST,$size,OP_NOOP") {
   Dee_uint16_t size;
   size = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_CAST_LIST)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_LIST);
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_TUPLE,$size,OP_CAST_SET",
       "OP_SET,$size,OP_NOOP") {
   Dee_uint16_t size;
   size = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_CAST_SET)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_SET);
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_TUPLE */

#ifdef OP_LIST
 TARGET(OP_LIST) {
  RULE("OP_LIST,$size,OP_CAST_TUPLE",
       "OP_TUPLE,$size,OP_NOOP") {
   Dee_uint16_t size;
   size = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_CAST_TUPLE)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_TUPLE);
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_LIST,$size,OP_CAST_LIST",
       "OP_LIST,$size,OP_NOOP") {
   Dee_uint16_t size;
   size = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_CAST_LIST)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_SKIPOP();
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_LIST,$size,OP_CAST_SET",
       "OP_SET,$size,OP_NOOP") {
   Dee_uint16_t size;
   size = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_CAST_SET)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_SET);
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_LIST */

#ifdef OP_LROT_N
 TARGET(OP_LROT_N) {
  RULE("OP_LROT_N,$shift,OP_RROT_N,$shift",
       "OP_NOOP,OP_NOOP,OP_NOOP,OP_NOOP,OP_NOOP,OP_NOOP") {
   Dee_uint16_t shift;
   shift = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_RROT_N)) RULE_BREAK(); RT_NEXT_OP();
   if (shift != RT_READ_ARG()) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_LROT_N */

#ifdef OP_RROT_N
 TARGET(OP_RROT_N) {
  RULE("OP_RROT_N,$shift,OP_LROT_N,$shift",
       "OP_NOOP,OP_NOOP,OP_NOOP,OP_NOOP,OP_NOOP,OP_NOOP") {
   Dee_uint16_t shift;
   shift = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_LROT_N)) RULE_BREAK(); RT_NEXT_OP();
   if (shift != RT_READ_ARG()) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_RROT_N */

#ifdef OP_SET
 TARGET(OP_SET) {
  RULE("OP_SET,$size,OP_CAST_TUPLE",
       "OP_TUPLE,$size,OP_NOOP") {
   Dee_uint16_t size;
   size = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_CAST_TUPLE)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_TUPLE);
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_SET,$size,OP_CAST_LIST",
       "OP_LIST,$size,OP_NOOP") {
   Dee_uint16_t size;
   size = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_CAST_LIST)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_OP(OP_LIST);
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  RULE("OP_SET,$size,OP_CAST_SET",
       "OP_SET,$size,OP_NOOP") {
   Dee_uint16_t size;
   size = RT_READ_ARG();
   if (RT_PROTECTED() || (RT_GET_OP() != OP_CAST_SET)) RULE_BREAK();
   RT_WRITE_BEGIN();
   RT_WRITE_SKIPOP();
   RT_WRITE_SKIPARG();
   RT_WRITE_OP(OP_NOOP);
   DISPATCH_OPTIMIZED();
  }
  DISPATCH();
 }
#endif /* OP_SET */

