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
#ifndef GUARD_DEEMON_DEX_HASHLIB_C
#define GUARD_DEEMON_DEX_HASHLIB_C 1
#define DEE_EXTENSION 1
#ifdef __DEEMON__
#pragma warning(disable: 16)
#endif

//@ dex.name = _hashlib

#include "_hashlib.h"
#include <deemon/__conf.inl>
#include <deemon/dex.h>
#include <deemon/error.h>
#include <deemon/list.h>
#include <deemon/none.h>
#include <deemon/memberdef.h>
#include <deemon/generic_vtable.h>
#include <deemon/integer.h>
#include <deemon/structured.h>
#include <deemon/type.h>
#include <deemon/optional/std/string.h>
#include <deemon/__bswap_intrin.inl>
#include <deemon/optional/object_malloc.h>

DEE_DECL_BEGIN

#ifdef __DEEMON__
#define DEEMON_REFLECT(x,n_bits)\
({\
 local _x = (x);\
 local _xt = _x.class;\
 local _r = (_xt)0;\
 for (local i = 0; i < (n_bits); ++i) {\
   if (_x&(_xt(1) << i))\
      _r |= _xt(1) << (((n_bits)-1)-i);\
 }\
 _r;\
})


// function DEEMON_GENERATE_CRC(int width, Integer polynom, bool reflect_input) -> list
//    Returns a 256 element list of crc values, based on a given polynom
#define DEEMON_GENERATE_CRC(width,polynom,reflect_input)\
({\
  local result = [0]*256;\
  local width_mask = uint64_t(1) << ((width)-1);\
  local type_mask  = uint64_t(1);\
  for (local i = 1; i < (width); ++i) {\
    type_mask <<= 1;\
    type_mask |= 0x1;\
  }\
  local hash_width = width;\
  if (hash_width > 32) hash_width = 64;\
  else if (hash_width > 16) hash_width = 32;\
  else if (hash_width > 8) hash_width = 16;\
  else hash_width = 8;\
  local crc_shift = 0;\
  if ((width) < 8) crc_shift = 8-(width);\
  for (local i = uint64_t(0); i < 256; ++i) {\
    local reg = uint64_t((reflect_input) ? DEEMON_REFLECT(i,8) : copy i);\
    reg <<= ((width)-8)+crc_shift;\
    for (local j = 0; j < 8; ++j) {\
      if (reg & (width_mask << crc_shift)) {\
        reg = (reg << 1) ^ ((polynom) << crc_shift);\
      } else {\
        reg <<= 1;\
      }\
    }\
    if (reflect_input) {\
      reg >>= crc_shift;\
      reg = DEEMON_REFLECT(reg,width);\
      reg <<= crc_shift;\
    }\
    reg >>= crc_shift;\
    reg &= type_mask;\
    reg <<= (hash_width-(width));\
    result[i] = reg;\
  }\
  /*for (local i = 0; i < 256; ++i) {\
    result[i] = (result[i] >> 8) ^ result[result[i] & 0xff];\
  }*/\
  result;\
})

#endif

struct DeeCRC8Algorithm  { struct DeeCRCAlgorithm crc_algo; Dee_uint8_t  start,outmod,widthmask; Dee_uint8_t  crc_table[256]; };
struct DeeCRC16Algorithm { struct DeeCRCAlgorithm crc_algo; Dee_uint16_t start,outmod,widthmask; Dee_uint16_t crc_table[256]; };
struct DeeCRC32Algorithm { struct DeeCRCAlgorithm crc_algo; Dee_uint32_t start,outmod,widthmask; Dee_uint32_t crc_table[256]; };
struct DeeCRC64Algorithm { struct DeeCRCAlgorithm crc_algo; Dee_uint64_t start,outmod,widthmask; Dee_uint64_t crc_table[256]; };

//////////////////////////////////////////////////////////////////////////
// As reference for most crc algorithms, the following web-site was used:
// >> http://reveng.sourceforge.net/crc-catalogue/


/*[[[deemon
#include <file>
#include <fs>
fs::chdir(fs::path::head(__FILE__));
const algo_folder = "algorithms";

#define hex(x) ("%#x" % (x))

try fs::mkdir(algo_folder); catch (...);

crc_algos = [];

known_hashfuncs = dict {};
function get_hashfunc(width,revin,revout) {
  local key = pack width,revin,revout;
  if (key in known_hashfuncs) return known_hashfuncs[key];
  local name = "_hashimpl_"+width;
  if (revin && revout) name += "_ioref";
  else if (revout) name += "_oref";
  else if (revin) name += "_iref";
  print "#define WIDTH",width;
  print "#define IN_REFLECTED",revin ? "1" : "0";
  print "#define OUT_REFLECTED",revout ? "1" : "0";
  print "#include",repr "hashfunc.inl";
  known_hashfuncs[key] = name;
  return name;
}

function def_crc(names,width,poly,args...) {
  local init = 0x0,refin = false,refout = false,xorout = 0x0;
  switch (#args) {
    case 4: xorout   = args[3]; // fallthrough
    case 3: refout   = args[2]; // fallthrough
    case 2: refin    = args[1]; // fallthrough
    case 1: init     = args[0]; // fallthrough
    default: break;
  }
  local size = (width/8)+((width % 8) ? 1 : 0);
  switch (size) {
    case 3: size = 4; break;
    case 5: case 6: case 7: size = 8; break;
    default: break;
  }
  local poly_type = switch (size) {1:uint8_t,2:uint16_t,4:uint32_t,8:uint64_t};
  local main_name = names.pop_front().upper();
  print file.io.stderr: "Calculating:",main_name;
  local algo_name = "_crca_"+main_name.lower().replace("-","_").replace("/","__");
  crc_algos.append(algo_name);
  local hashfunc = get_hashfunc(width,refin,refout);
  local alias_names = "NULL";
  if (names) {
    alias_names = algo_name+"_alias_names";
    print "static char const *const",alias_names+"[] = {",
          ",".join(for(local name: names) repr name.upper())+",NULL","};";
  }
  flag_names = "DEE_CRCALGORITHM_FLAG_NONE";
  if (refin)  flag_names += "|DEE_CRCALGORITHM_FLAG_REVIN";
  if (refout) flag_names += "|DEE_CRCALGORITHM_FLAG_REVOUT";
  flag_names = flag_names.lsstrip("DEE_CRCALGORITHM_FLAG_NONE|");
  print "static struct DeeCRC"+(size*8)+"Algorithm const",algo_name,"= {";
#define CODEINT(x)  (("DEE_UINT"+(size*8)+"_C(%#."+(size*2)+"I"+(size*8)+"x)") % (x))
  local width_mask = poly_type(1);
  for (local i = 1; i < width; ++i) {
    width_mask <<= 1;
    width_mask |= 0x1;
  }
  print " {"+repr main_name+","+alias_names+","+width+",DEE_CRCHASHER_SIZE_"+(size*8)+","+flag_names+",\n"
        " {(DeeCRCHashFunc8)&"+hashfunc+"}},"+CODEINT(init)+","+CODEINT(xorout)+","+CODEINT(width_mask)+",";
  algo_filename = fs::path::join(algo_folder,"algorithm."+main_name.replace("\\","_").replace("/","_")+".inl");
  algo_file = file.io(algo_filename,"w");
  print algo_file: "{";
  local line = 0;
  for (x: DEEMON_GENERATE_CRC(width,(poly_type)poly,refin)) {
    if (!line) print algo_file: " ",;
    print algo_file: CODEINT(x)+",",;
    if (++line == 8) { print algo_file:; line = 0; }
  }
  print algo_file: "}";
  algo_file.close();
  print "#include",repr(algo_filename.replace("\\","/"));
  print "};";
  print;
}

// Define all whole bunch of crc algorithms
def_crc(["CRC-1"],                     1,0x01);
def_crc(["CRC-3/ROHC"],                3,0x03,0x7,true,true,0x0);
def_crc(["CRC-4/INTERLAKEN"],          4,0x03,0xf,false,false,0xf);
def_crc(["CRC-4/ITU"],                 4,0x03,0x0,true,true,0x0);
def_crc(["CRC-5/EPC"],                 5,0x09,0x9,false,false,0x00);
def_crc(["CRC-5/ITU"],                 5,0x15,0x0,true,true,0x00);
def_crc(["CRC-5/USB"],                 5,0x05,0x1f,true,true,0x1f);
def_crc(["CRC-6/CDMA2000-A"],          6,0x27,0x3f,false,false,0x00);
def_crc(["CRC-6/CDMA2000-B"],          6,0x07,0x3f,false,false,0x00);
def_crc(["CRC-6/DARC"],                6,0x19,0x00,true,true,0x00);
def_crc(["CRC-6/ITU"],                 6,0x03,0x00,true,true,0x00);
def_crc(["CRC-7"],                     7,0x09,0x00,false,false,0x00);
def_crc(["CRC-7/ROHC"],                7,0x4f,0x7f,true,true,0x00);
def_crc(["CRC-7/UMTS"],                7,0x45,0x00,false,false,0x00);
def_crc(["CRC-8"],                     8,0x07,0x00,false,false,0x00);
def_crc(["CRC-8/AUTOSAR"],             8,0x2f,0xff,false,false,0xff);
def_crc(["CRC-8/CDMA2000"],            8,0x9b,0xff,false,false,0x00);
def_crc(["CRC-8/DARC"],                8,0x39,0x00,true,true,0x00);
def_crc(["CRC-8/DVB-S2"],              8,0xd5,0x00,false,false,0x00);
def_crc(["CRC-8/EBU","CRC-8/AES"],     8,0x1d,0xff,true,true,0x00);
def_crc(["CRC-8/I-CODE"],              8,0x1d,0xfd,false,false,0x00);
def_crc(["CRC-8/ITU"],                 8,0x07,0x00,false,false,0x55);
def_crc(["CRC-8/LTE"],                 8,0x9b,0x00,false,false,0x00);

// NOTE: 'CRC-8/DALLAS' was added because of it being mentioned here:
// >> https://en.wikipedia.org/wiki/Polynomial_representations_of_cyclic_redundancy_checks
def_crc(["CRC-8/MAXIM","DOW-CRC","CRC-8/DALLAS"],8,0x31,0x00,true,true,0x00);

def_crc(["CRC-8/OPENSAFETY"],          8,0x2f,0x00,false,false,0x00);
def_crc(["CRC-8/ROHC"],                8,0x07,0xff,true,true,0x00);
def_crc(["CRC-8/SAE-J1850"],           8,0x1d,0xff,false,false,0xff);
def_crc(["CRC-8/WCDMA"],               8,0x9b,0x00,true,true,0x00);
def_crc(["CRC-10"],                   10,0x233,0x000,false,false,0x000);
def_crc(["CRC-10/CDMA2000"],          10,0x3d9,0x3ff,false,false,0x000);
def_crc(["CRC-11"],                   11,0x385,0x01a,false,false,0x000);
def_crc(["CRC-11/UMTS"],              11,0x307,0x000,false,false,0x000);
def_crc(["CRC-12/CDMA2000"],          12,0xf13,0xfff,false,false,0x000);
def_crc(["CRC-12/DECT","X-CRC-12"],   12,0x80f,0x000,false,false,0x000);
def_crc(["CRC-12/UMTS","CRC-12/3GPP"],12,0x80f,0x000,false,true,0x000);
def_crc(["CRC-13/BBC"],               13,0x1cf5,0x0000,false,false,0x0000);
def_crc(["CRC-14/DARC"],              14,0x0805,0x0000,true,true,0x0000);
def_crc(["CRC-15"],                   15,0x4599,0x0000,false,false,0x0000);
def_crc(["CRC-15/MPT1327"],           15,0x6815,0x0000,false,false,0x0001);

// Added 'CRC-16/IBM' alias, as referenced here: https://users.ece.cmu.edu/~koopman/crc/crc16.html
def_crc(["ARC","CRC-16","CRC-IBM","CRC-16/ARC","CRC-16/LHA","CRC-16/IBM"],16,0x8005,0x0000,true,true,0x0000);

// this was is named 'CRC-CCITT (0x1D0F)' on https://www.lammertbies.nl/comm/info/crc-calculation.html
//   >> With the crc-name compare function and the listed aliases, "CRC-CCITT (0x1D0F)" is covered
def_crc(["CRC-16/AUG-CCITT","CRC-16/SPI-FUJITSU",
            "CRC-CCITT/0x1D0F",   "CRC-CCITT/1D0F",
         "CRC-16/CCITT-0x1D0F","CRC-16/CCITT-1D0F",
        ],16,0x1021,0x1d0f,false,false,0x0000);

def_crc(["CRC-16/BUYPASS","CRC-16/VERIFONE","CRC-16/UMTS"],16,0x8005,0x0000,false,false,0x0000);

// v This one is listed as 'CRC-CCITT (0xFFFF)' under https://www.lammertbies.nl/comm/info/crc-calculation.html
//   >> With the crc-name compare function and the listed aliases, "CRC-CCITT (0xFFFF)" is covered
def_crc(["CRC-16/CCITT-FALSE",
            "CRC-CCITT/0xFFFF",   "CRC-CCITT/FFFF",
         "CRC-16/CCITT-0xFFFF","CRC-16/CCITT-FFFF",
        ],       16,0x1021,0xffff,false,false,0x0000);

def_crc(["CRC-16/CDMA2000"],          16,0xc867,0xffff,false,false,0x0000);
def_crc(["CRC-16/CMS"],               16,0x8005,0xffff,false,false,0x0000);
def_crc(["CRC-16/DDS-110"],           16,0x8005,0x800d,false,false,0x0000);
def_crc(["CRC-16/DECT-R","R-CRC-16"], 16,0x0589,0x0000,false,false,0x0001);
def_crc(["CRC-16/DECT-X","X-CRC-16"], 16,0x0589,0x0000,false,false,0x0000);

// v On https://www.lammertbies.nl/comm/info/crc-calculation.html this one was named "CRC-DNP"
//   Just like with 'KERMIT', it seemed like this one needed to get its bytes swapped.
//   EDIT: After writing test cases, it seems like that web-page has an endian problem...
def_crc(["CRC-16/DNP","CRC-DNP"],     16,0x3d65,0x0000,true,true,0xffff);

def_crc(["CRC-16/EN-13757"],          16,0x3d65,0x0000,false,false,0xffff);
def_crc(["CRC-16/GENIBUS","CRC-16/EPC","CRC-16/I-CODE","CRC-16/DARC"],16,0x1021,0xffff,false,false,0xffff);
def_crc(["CRC-16/LJ1200"],            16,0x6f63,0x0000,false,false,0x0000);
def_crc(["CRC-16/MAXIM"],             16,0x8005,0x0000,true,true,0xffff);
def_crc(["CRC-16/MCRF4XX"],           16,0x1021,0xffff,true,true,0x0000);
def_crc(["CRC-16/OPENSAFETY-A"],      16,0x5935,0x0000,false,false,0x0000);
def_crc(["CRC-16/OPENSAFETY-B"],      16,0x755b,0x0000,false,false,0x0000);
def_crc(["CRC-16/PROFIBUS","CRC-16/IEC-61158-2"],16,0x1dcf,0xffff,false,false,0xffff);

// v This one gave me quite the headache...
//   - While the best source for all the other algorithms was
//     http://reveng.sourceforge.net/crc-catalogue/ It has a wrong init hash listed.
//     But since that page tells you that it can't be trusted, I guess it's ok...
//   - After quite some extensive searching, I found the correct init hash here:
//     http://crcmod.sourceforge.net/crcmod.predefined.html
//   NOTE: The wrong version is commented out, but kept as a reminder.
//   NOTE: After looking at the numbers, I noticed that '0x554d
//         is '0xb2aa' when reading the bits in reverse...
//         Dunno why someone did that, though, but it might be 
//   EDIT: Thank goodness I noticed that bit-order, because as
//         it turns out, that bug is present in a bunch of algos.
assert DEEMON_REFLECT(0xb2aa,16) == 0x554d; // Just so you know...
def_crc(["CRC-16/RIELLO"],            16,0x1021,0x554d,true,true,0x0000);
//def_crc(["CRC-16/RIELLO"],            16,0x1021,0xb2aa,true,true,0x0000);

def_crc(["CRC-16/T10-DIF"],           16,0x8bb7,0x0000,false,false,0x0000);
def_crc(["CRC-16/TELEDISK"],          16,0xa097,0x0000,false,false,0x0000);

// Again: Same incorrect bit-order as with 'CRC-16/RIELLO'
assert DEEMON_REFLECT(0x89ec,16) == 0x3791; // Just so you know...
def_crc(["CRC-16/TMS37157"],          16,0x1021,0x3791,true,true,0x0000);
//def_crc(["CRC-16/TMS37157"],          16,0x1021,0x89ec,true,true,0x0000);

def_crc(["CRC-16/USB"],               16,0x8005,0xffff,true,true,0xffff);

// After figuring out what the deal was with 'CRC-16/RIELLO', I tried the
// same thing with this one's intial value. And lookie-lookie: it worked
assert DEEMON_REFLECT(0xc6c6,16) == 0x6363; // Just so you know...
def_crc(["CRC-A"],                    16,0x1021,0x6363,true,true,0x0000);
//def_crc(["CRC-A"],                    16,0x1021,0xc6c6,true,true,0x0000);

// v After tinkering with https://www.lammertbies.nl/comm/info/crc-calculation.html,
//   it seemed like this one needed to get its bytes swapped.
//   EDIT: After writing test cases, it seems like that web-page has an endian problem...
def_crc(["KERMIT","CRC-16/CCITT","CRC-16/CCITT-TRUE","CRC-CCITT"],16,0x1021,0x0000,true,true,0x0000);

def_crc(["MODBUS"],                   16,0x8005,0xffff,true,true,0x0000);
def_crc(["X-25","CRC-16/IBM-SDLC","CRC-16/ISO-HDLC","CRC-B"],16,0x1021,0xffff,true,true,0xffff);
def_crc(["XMODEM","ZMODEM","CRC-16/ACORN","CRC-16/LTE"],16,0x1021,0x0000,false,false,0x0000);
def_crc(["CRC-24","CRC-24/OPENPGP"],  24,0x864cfb,0xb704ce,false,false,0x000000);
def_crc(["CRC-24/BLE"],               24,0x00065b,0x555555,true,true,0x000000);
def_crc(["CRC-24/FLEXRAY-A"],         24,0x5d6dcb,0xfedcba,false,false,0x000000);
def_crc(["CRC-24/FLEXRAY-B"],         24,0x5d6dcb,0xabcdef,false,false,0x000000);
def_crc(["CRC-24/INTERLAKEN"],        24,0x328b63,0xffffff,false,false,0xffffff);
def_crc(["CRC-24/LTE-A"],             24,0x864cfb,0x000000,false,false,0x000000);
def_crc(["CRC-24/LTE-B"],             24,0x800063,0x000000,false,false,0x000000);
def_crc(["CRC-30/CDMA"],              30,0x2030b9c7,0x3fffffff,false,false,0x3fffffff);
def_crc(["CRC-31/PHILIPS"],           31,0x04c11db7,0x7fffffff,false,false,0x7fffffff);

// >> https://users.ece.cmu.edu/~koopman/crc/crc32.html (referenced this as 'CRC-32/IEEE-802.3')
// >> https://www.autosar.org/fileadmin/files/releases/4-2/software-architecture/safety-and-security/standard/AUTOSAR_SWS_CRCLibrary.pdf (contains freaking readable IEEE-802.3 specs on page 25/48)
def_crc(["CRC-32","CRC-32/ADCCP","PKZIP","CRC-32/IEEE-802.3",
         "IEEE-802.3"],32,0x04c11db7,0xffffffff,true,true,0xffffffff);

def_crc(["CRC-32/AUTOSAR"],           32,0xf4acfb13,0xffffffff,true,true,0xffffffff);
def_crc(["CRC-32/BZIP2","CRC-32/AAL5","CRC-32/DECT-B","B-CRC-32"],32,0x04c11db7,0xffffffff,false,false,0xffffffff);
def_crc(["CRC-32C"],                  32,0x1edc6f41,0xffffffff,true,true,0xffffffff);
def_crc(["CRC-32D"],                  32,0xa833982b,0xffffffff,true,true,0xffffffff);
def_crc(["CRC-32/MPEG-2"],            32,0x04c11db7,0xffffffff,false,false,0x00000000);

// NOTE: The 'POSIX' alternate name was assigned because of the name given here:
//       >> http://crcmod.sourceforge.net/crcmod.predefined.html
def_crc(["CRC-32/POSIX","POSIX"],     32,0x04c11db7,0x00000000,false,false,0xffffffff);

def_crc(["CRC-32Q"],                  32,0x814141ab,0x00000000,false,false,0x00000000);
def_crc(["JAMCRC"],                   32,0x04c11db7,0xffffffff,true,true,0x00000000);
def_crc(["XFER"],                     32,0x000000af,0x00000000,false,false,0x00000000);
def_crc(["CRC-40/GSM"],               40,0x0004820009ui64,0x0000000000ui64,false,false,0xffffffffffui64);
def_crc(["CRC-64"],                   64,0x42f0e1eba9ea3693ui64,0x0000000000000000ui64,false,false,0x0000000000000000ui64);
def_crc(["CRC-64/WE"],                64,0x42f0e1eba9ea3693ui64,0xffffffffffffffffui64,false,false,0xffffffffffffffffui64);
def_crc(["CRC-64/XZ"],                64,0x42f0e1eba9ea3693ui64,0xffffffffffffffffui64,true,true,0xffffffffffffffffui64);
//def_crc(["CRC-82/DARC"],            82,0x0308c0111011401440411ui128,0x000000000000000000000ui128,true,true,0x000000000000000000000ui128);

// Some more unspecific crcs from here: http://crcmod.sourceforge.net/crcmod.predefined.html
// NOTE: These we fitted to match the reveng name format
//crc-64 	0x1000000000000001B 	True 	0x0000000000000000 	0x0000000000000000 	0x46A5A9388A5BEFFE
def_crc(["CRC-64/JONES"],             64,0xAD93D23594C935A9ui64,0xffffffffffffffffui64,true,true,0x0000000000000000ui64);

// Some more asorted CRC algorithms from across the internet

// >> https://en.wikipedia.org/wiki/Polynomial_representations_of_cyclic_redundancy_checks
// >> https://github.com/culvertsoft/mgen/blob/master/mgen-api/src/main/java/se/culvertsoft/mgen/api/util/CRC64.java (code indicates reflection)
def_crc(["CRC-64/ECMA","CRC-64/ECMA-182"],64,0x42f0e1eba9ea3693ui64,0x0000000000000000ui64,true,true,0x0000000000000000ui64);

// >> https://en.wikipedia.org/wiki/Polynomial_representations_of_cyclic_redundancy_checks
// >> http://de.slideshare.net/cloudflare/cloud-flare-jgc-bigo-meetup-rolling-hashes  (barely readable code example indicates reflection)
// >> https://users.ece.cmu.edu/~koopman/crc/crc64.html  (alternative name: 'FP-64')
def_crc(["CRC-64/ISO","FP-64"],       64,0x000000000000001bui64,0x0000000000000000ui64,true,true,0x0000000000000000ui64);



print;
print "static struct DeeCRCAlgorithm const *const _crc_algorithms[] = {";
for (local algo: crc_algos) {
  print " (struct DeeCRCAlgorithm const *)&"+algo+",";
}
print "};";
]]]*/
#define WIDTH 1
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static struct DeeCRC8Algorithm const _crca_crc_1 = {
 {"CRC-1",NULL,1,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_1}},DEE_UINT8_C(0x00),DEE_UINT8_C(0x00),DEE_UINT8_C(0x01),
#include "algorithms/algorithm.CRC-1.inl"
};

#define WIDTH 3
#define IN_REFLECTED 1
#define OUT_REFLECTED 1
#include "hashfunc.inl"
static struct DeeCRC8Algorithm const _crca_crc_3__rohc = {
 {"CRC-3/ROHC",NULL,3,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_3_ioref}},DEE_UINT8_C(0x07),DEE_UINT8_C(0x00),DEE_UINT8_C(0x07),
#include "algorithms/algorithm.CRC-3_ROHC.inl"
};

#define WIDTH 4
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static struct DeeCRC8Algorithm const _crca_crc_4__interlaken = {
 {"CRC-4/INTERLAKEN",NULL,4,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_4}},DEE_UINT8_C(0x0f),DEE_UINT8_C(0x0f),DEE_UINT8_C(0x0f),
#include "algorithms/algorithm.CRC-4_INTERLAKEN.inl"
};

#define WIDTH 4
#define IN_REFLECTED 1
#define OUT_REFLECTED 1
#include "hashfunc.inl"
static struct DeeCRC8Algorithm const _crca_crc_4__itu = {
 {"CRC-4/ITU",NULL,4,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_4_ioref}},DEE_UINT8_C(0x00),DEE_UINT8_C(0x00),DEE_UINT8_C(0x0f),
#include "algorithms/algorithm.CRC-4_ITU.inl"
};

#define WIDTH 5
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static struct DeeCRC8Algorithm const _crca_crc_5__epc = {
 {"CRC-5/EPC",NULL,5,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_5}},DEE_UINT8_C(0x09),DEE_UINT8_C(0x00),DEE_UINT8_C(0x1f),
#include "algorithms/algorithm.CRC-5_EPC.inl"
};

#define WIDTH 5
#define IN_REFLECTED 1
#define OUT_REFLECTED 1
#include "hashfunc.inl"
static struct DeeCRC8Algorithm const _crca_crc_5__itu = {
 {"CRC-5/ITU",NULL,5,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_5_ioref}},DEE_UINT8_C(0x00),DEE_UINT8_C(0x00),DEE_UINT8_C(0x1f),
#include "algorithms/algorithm.CRC-5_ITU.inl"
};

static struct DeeCRC8Algorithm const _crca_crc_5__usb = {
 {"CRC-5/USB",NULL,5,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_5_ioref}},DEE_UINT8_C(0x1f),DEE_UINT8_C(0x1f),DEE_UINT8_C(0x1f),
#include "algorithms/algorithm.CRC-5_USB.inl"
};

#define WIDTH 6
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static struct DeeCRC8Algorithm const _crca_crc_6__cdma2000_a = {
 {"CRC-6/CDMA2000-A",NULL,6,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_6}},DEE_UINT8_C(0x3f),DEE_UINT8_C(0x00),DEE_UINT8_C(0x3f),
#include "algorithms/algorithm.CRC-6_CDMA2000-A.inl"
};

static struct DeeCRC8Algorithm const _crca_crc_6__cdma2000_b = {
 {"CRC-6/CDMA2000-B",NULL,6,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_6}},DEE_UINT8_C(0x3f),DEE_UINT8_C(0x00),DEE_UINT8_C(0x3f),
#include "algorithms/algorithm.CRC-6_CDMA2000-B.inl"
};

#define WIDTH 6
#define IN_REFLECTED 1
#define OUT_REFLECTED 1
#include "hashfunc.inl"
static struct DeeCRC8Algorithm const _crca_crc_6__darc = {
 {"CRC-6/DARC",NULL,6,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_6_ioref}},DEE_UINT8_C(0x00),DEE_UINT8_C(0x00),DEE_UINT8_C(0x3f),
#include "algorithms/algorithm.CRC-6_DARC.inl"
};

static struct DeeCRC8Algorithm const _crca_crc_6__itu = {
 {"CRC-6/ITU",NULL,6,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_6_ioref}},DEE_UINT8_C(0x00),DEE_UINT8_C(0x00),DEE_UINT8_C(0x3f),
#include "algorithms/algorithm.CRC-6_ITU.inl"
};

#define WIDTH 7
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static struct DeeCRC8Algorithm const _crca_crc_7 = {
 {"CRC-7",NULL,7,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_7}},DEE_UINT8_C(0x00),DEE_UINT8_C(0x00),DEE_UINT8_C(0x7f),
#include "algorithms/algorithm.CRC-7.inl"
};

#define WIDTH 7
#define IN_REFLECTED 1
#define OUT_REFLECTED 1
#include "hashfunc.inl"
static struct DeeCRC8Algorithm const _crca_crc_7__rohc = {
 {"CRC-7/ROHC",NULL,7,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_7_ioref}},DEE_UINT8_C(0x7f),DEE_UINT8_C(0x00),DEE_UINT8_C(0x7f),
#include "algorithms/algorithm.CRC-7_ROHC.inl"
};

static struct DeeCRC8Algorithm const _crca_crc_7__umts = {
 {"CRC-7/UMTS",NULL,7,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_7}},DEE_UINT8_C(0x00),DEE_UINT8_C(0x00),DEE_UINT8_C(0x7f),
#include "algorithms/algorithm.CRC-7_UMTS.inl"
};

#define WIDTH 8
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static struct DeeCRC8Algorithm const _crca_crc_8 = {
 {"CRC-8",NULL,8,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_8}},DEE_UINT8_C(0x00),DEE_UINT8_C(0x00),DEE_UINT8_C(0xff),
#include "algorithms/algorithm.CRC-8.inl"
};

static struct DeeCRC8Algorithm const _crca_crc_8__autosar = {
 {"CRC-8/AUTOSAR",NULL,8,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_8}},DEE_UINT8_C(0xff),DEE_UINT8_C(0xff),DEE_UINT8_C(0xff),
#include "algorithms/algorithm.CRC-8_AUTOSAR.inl"
};

static struct DeeCRC8Algorithm const _crca_crc_8__cdma2000 = {
 {"CRC-8/CDMA2000",NULL,8,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_8}},DEE_UINT8_C(0xff),DEE_UINT8_C(0x00),DEE_UINT8_C(0xff),
#include "algorithms/algorithm.CRC-8_CDMA2000.inl"
};

#define WIDTH 8
#define IN_REFLECTED 1
#define OUT_REFLECTED 1
#include "hashfunc.inl"
static struct DeeCRC8Algorithm const _crca_crc_8__darc = {
 {"CRC-8/DARC",NULL,8,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_8_ioref}},DEE_UINT8_C(0x00),DEE_UINT8_C(0x00),DEE_UINT8_C(0xff),
#include "algorithms/algorithm.CRC-8_DARC.inl"
};

static struct DeeCRC8Algorithm const _crca_crc_8__dvb_s2 = {
 {"CRC-8/DVB-S2",NULL,8,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_8}},DEE_UINT8_C(0x00),DEE_UINT8_C(0x00),DEE_UINT8_C(0xff),
#include "algorithms/algorithm.CRC-8_DVB-S2.inl"
};

static char const *const _crca_crc_8__ebu_alias_names[] = { "CRC-8/AES",NULL };
static struct DeeCRC8Algorithm const _crca_crc_8__ebu = {
 {"CRC-8/EBU",_crca_crc_8__ebu_alias_names,8,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_8_ioref}},DEE_UINT8_C(0xff),DEE_UINT8_C(0x00),DEE_UINT8_C(0xff),
#include "algorithms/algorithm.CRC-8_EBU.inl"
};

static struct DeeCRC8Algorithm const _crca_crc_8__i_code = {
 {"CRC-8/I-CODE",NULL,8,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_8}},DEE_UINT8_C(0xfd),DEE_UINT8_C(0x00),DEE_UINT8_C(0xff),
#include "algorithms/algorithm.CRC-8_I-CODE.inl"
};

static struct DeeCRC8Algorithm const _crca_crc_8__itu = {
 {"CRC-8/ITU",NULL,8,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_8}},DEE_UINT8_C(0x00),DEE_UINT8_C(0x55),DEE_UINT8_C(0xff),
#include "algorithms/algorithm.CRC-8_ITU.inl"
};

static struct DeeCRC8Algorithm const _crca_crc_8__lte = {
 {"CRC-8/LTE",NULL,8,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_8}},DEE_UINT8_C(0x00),DEE_UINT8_C(0x00),DEE_UINT8_C(0xff),
#include "algorithms/algorithm.CRC-8_LTE.inl"
};

static char const *const _crca_crc_8__maxim_alias_names[] = { "DOW-CRC","CRC-8/DALLAS",NULL };
static struct DeeCRC8Algorithm const _crca_crc_8__maxim = {
 {"CRC-8/MAXIM",_crca_crc_8__maxim_alias_names,8,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_8_ioref}},DEE_UINT8_C(0x00),DEE_UINT8_C(0x00),DEE_UINT8_C(0xff),
#include "algorithms/algorithm.CRC-8_MAXIM.inl"
};

static struct DeeCRC8Algorithm const _crca_crc_8__opensafety = {
 {"CRC-8/OPENSAFETY",NULL,8,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_8}},DEE_UINT8_C(0x00),DEE_UINT8_C(0x00),DEE_UINT8_C(0xff),
#include "algorithms/algorithm.CRC-8_OPENSAFETY.inl"
};

static struct DeeCRC8Algorithm const _crca_crc_8__rohc = {
 {"CRC-8/ROHC",NULL,8,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_8_ioref}},DEE_UINT8_C(0xff),DEE_UINT8_C(0x00),DEE_UINT8_C(0xff),
#include "algorithms/algorithm.CRC-8_ROHC.inl"
};

static struct DeeCRC8Algorithm const _crca_crc_8__sae_j1850 = {
 {"CRC-8/SAE-J1850",NULL,8,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_8}},DEE_UINT8_C(0xff),DEE_UINT8_C(0xff),DEE_UINT8_C(0xff),
#include "algorithms/algorithm.CRC-8_SAE-J1850.inl"
};

static struct DeeCRC8Algorithm const _crca_crc_8__wcdma = {
 {"CRC-8/WCDMA",NULL,8,DEE_CRCHASHER_SIZE_8,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_8_ioref}},DEE_UINT8_C(0x00),DEE_UINT8_C(0x00),DEE_UINT8_C(0xff),
#include "algorithms/algorithm.CRC-8_WCDMA.inl"
};

#define WIDTH 10
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static struct DeeCRC16Algorithm const _crca_crc_10 = {
 {"CRC-10",NULL,10,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_10}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0000),DEE_UINT16_C(0x03ff),
#include "algorithms/algorithm.CRC-10.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_10__cdma2000 = {
 {"CRC-10/CDMA2000",NULL,10,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_10}},DEE_UINT16_C(0x03ff),DEE_UINT16_C(0x0000),DEE_UINT16_C(0x03ff),
#include "algorithms/algorithm.CRC-10_CDMA2000.inl"
};

#define WIDTH 11
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static struct DeeCRC16Algorithm const _crca_crc_11 = {
 {"CRC-11",NULL,11,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_11}},DEE_UINT16_C(0x001a),DEE_UINT16_C(0x0000),DEE_UINT16_C(0x07ff),
#include "algorithms/algorithm.CRC-11.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_11__umts = {
 {"CRC-11/UMTS",NULL,11,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_11}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0000),DEE_UINT16_C(0x07ff),
#include "algorithms/algorithm.CRC-11_UMTS.inl"
};

#define WIDTH 12
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static struct DeeCRC16Algorithm const _crca_crc_12__cdma2000 = {
 {"CRC-12/CDMA2000",NULL,12,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_12}},DEE_UINT16_C(0x0fff),DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0fff),
#include "algorithms/algorithm.CRC-12_CDMA2000.inl"
};

static char const *const _crca_crc_12__dect_alias_names[] = { "X-CRC-12",NULL };
static struct DeeCRC16Algorithm const _crca_crc_12__dect = {
 {"CRC-12/DECT",_crca_crc_12__dect_alias_names,12,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_12}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0fff),
#include "algorithms/algorithm.CRC-12_DECT.inl"
};

#define WIDTH 12
#define IN_REFLECTED 0
#define OUT_REFLECTED 1
#include "hashfunc.inl"
static char const *const _crca_crc_12__umts_alias_names[] = { "CRC-12/3GPP",NULL };
static struct DeeCRC16Algorithm const _crca_crc_12__umts = {
 {"CRC-12/UMTS",_crca_crc_12__umts_alias_names,12,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_12_oref}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0fff),
#include "algorithms/algorithm.CRC-12_UMTS.inl"
};

#define WIDTH 13
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static struct DeeCRC16Algorithm const _crca_crc_13__bbc = {
 {"CRC-13/BBC",NULL,13,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_13}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0000),DEE_UINT16_C(0x1fff),
#include "algorithms/algorithm.CRC-13_BBC.inl"
};

#define WIDTH 14
#define IN_REFLECTED 1
#define OUT_REFLECTED 1
#include "hashfunc.inl"
static struct DeeCRC16Algorithm const _crca_crc_14__darc = {
 {"CRC-14/DARC",NULL,14,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_14_ioref}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0000),DEE_UINT16_C(0x3fff),
#include "algorithms/algorithm.CRC-14_DARC.inl"
};

#define WIDTH 15
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static struct DeeCRC16Algorithm const _crca_crc_15 = {
 {"CRC-15",NULL,15,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_15}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0000),DEE_UINT16_C(0x7fff),
#include "algorithms/algorithm.CRC-15.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_15__mpt1327 = {
 {"CRC-15/MPT1327",NULL,15,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_15}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0001),DEE_UINT16_C(0x7fff),
#include "algorithms/algorithm.CRC-15_MPT1327.inl"
};

#define WIDTH 16
#define IN_REFLECTED 1
#define OUT_REFLECTED 1
#include "hashfunc.inl"
static char const *const _crca_arc_alias_names[] = { "CRC-16","CRC-IBM","CRC-16/ARC","CRC-16/LHA","CRC-16/IBM",NULL };
static struct DeeCRC16Algorithm const _crca_arc = {
 {"ARC",_crca_arc_alias_names,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_16_ioref}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.ARC.inl"
};

#define WIDTH 16
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static char const *const _crca_crc_16__aug_ccitt_alias_names[] = { "CRC-16/SPI-FUJITSU","CRC-CCITT/0X1D0F","CRC-CCITT/1D0F","CRC-16/CCITT-0X1D0F","CRC-16/CCITT-1D0F",NULL };
static struct DeeCRC16Algorithm const _crca_crc_16__aug_ccitt = {
 {"CRC-16/AUG-CCITT",_crca_crc_16__aug_ccitt_alias_names,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_16}},DEE_UINT16_C(0x1d0f),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_AUG-CCITT.inl"
};

static char const *const _crca_crc_16__buypass_alias_names[] = { "CRC-16/VERIFONE","CRC-16/UMTS",NULL };
static struct DeeCRC16Algorithm const _crca_crc_16__buypass = {
 {"CRC-16/BUYPASS",_crca_crc_16__buypass_alias_names,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_16}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_BUYPASS.inl"
};

static char const *const _crca_crc_16__ccitt_false_alias_names[] = { "CRC-CCITT/0XFFFF","CRC-CCITT/FFFF","CRC-16/CCITT-0XFFFF","CRC-16/CCITT-FFFF",NULL };
static struct DeeCRC16Algorithm const _crca_crc_16__ccitt_false = {
 {"CRC-16/CCITT-FALSE",_crca_crc_16__ccitt_false_alias_names,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_16}},DEE_UINT16_C(0xffff),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_CCITT-FALSE.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_16__cdma2000 = {
 {"CRC-16/CDMA2000",NULL,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_16}},DEE_UINT16_C(0xffff),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_CDMA2000.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_16__cms = {
 {"CRC-16/CMS",NULL,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_16}},DEE_UINT16_C(0xffff),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_CMS.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_16__dds_110 = {
 {"CRC-16/DDS-110",NULL,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_16}},DEE_UINT16_C(0x800d),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_DDS-110.inl"
};

static char const *const _crca_crc_16__dect_r_alias_names[] = { "R-CRC-16",NULL };
static struct DeeCRC16Algorithm const _crca_crc_16__dect_r = {
 {"CRC-16/DECT-R",_crca_crc_16__dect_r_alias_names,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_16}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0001),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_DECT-R.inl"
};

static char const *const _crca_crc_16__dect_x_alias_names[] = { "X-CRC-16",NULL };
static struct DeeCRC16Algorithm const _crca_crc_16__dect_x = {
 {"CRC-16/DECT-X",_crca_crc_16__dect_x_alias_names,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_16}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_DECT-X.inl"
};

static char const *const _crca_crc_16__dnp_alias_names[] = { "CRC-DNP",NULL };
static struct DeeCRC16Algorithm const _crca_crc_16__dnp = {
 {"CRC-16/DNP",_crca_crc_16__dnp_alias_names,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_16_ioref}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_DNP.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_16__en_13757 = {
 {"CRC-16/EN-13757",NULL,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_16}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_EN-13757.inl"
};

static char const *const _crca_crc_16__genibus_alias_names[] = { "CRC-16/EPC","CRC-16/I-CODE","CRC-16/DARC",NULL };
static struct DeeCRC16Algorithm const _crca_crc_16__genibus = {
 {"CRC-16/GENIBUS",_crca_crc_16__genibus_alias_names,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_16}},DEE_UINT16_C(0xffff),DEE_UINT16_C(0xffff),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_GENIBUS.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_16__lj1200 = {
 {"CRC-16/LJ1200",NULL,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_16}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_LJ1200.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_16__maxim = {
 {"CRC-16/MAXIM",NULL,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_16_ioref}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_MAXIM.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_16__mcrf4xx = {
 {"CRC-16/MCRF4XX",NULL,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_16_ioref}},DEE_UINT16_C(0xffff),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_MCRF4XX.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_16__opensafety_a = {
 {"CRC-16/OPENSAFETY-A",NULL,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_16}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_OPENSAFETY-A.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_16__opensafety_b = {
 {"CRC-16/OPENSAFETY-B",NULL,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_16}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_OPENSAFETY-B.inl"
};

static char const *const _crca_crc_16__profibus_alias_names[] = { "CRC-16/IEC-61158-2",NULL };
static struct DeeCRC16Algorithm const _crca_crc_16__profibus = {
 {"CRC-16/PROFIBUS",_crca_crc_16__profibus_alias_names,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_16}},DEE_UINT16_C(0xffff),DEE_UINT16_C(0xffff),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_PROFIBUS.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_16__riello = {
 {"CRC-16/RIELLO",NULL,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_16_ioref}},DEE_UINT16_C(0x554d),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_RIELLO.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_16__t10_dif = {
 {"CRC-16/T10-DIF",NULL,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_16}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_T10-DIF.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_16__teledisk = {
 {"CRC-16/TELEDISK",NULL,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_16}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_TELEDISK.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_16__tms37157 = {
 {"CRC-16/TMS37157",NULL,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_16_ioref}},DEE_UINT16_C(0x3791),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_TMS37157.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_16__usb = {
 {"CRC-16/USB",NULL,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_16_ioref}},DEE_UINT16_C(0xffff),DEE_UINT16_C(0xffff),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-16_USB.inl"
};

static struct DeeCRC16Algorithm const _crca_crc_a = {
 {"CRC-A",NULL,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_16_ioref}},DEE_UINT16_C(0x6363),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.CRC-A.inl"
};

static char const *const _crca_kermit_alias_names[] = { "CRC-16/CCITT","CRC-16/CCITT-TRUE","CRC-CCITT",NULL };
static struct DeeCRC16Algorithm const _crca_kermit = {
 {"KERMIT",_crca_kermit_alias_names,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_16_ioref}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.KERMIT.inl"
};

static struct DeeCRC16Algorithm const _crca_modbus = {
 {"MODBUS",NULL,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_16_ioref}},DEE_UINT16_C(0xffff),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.MODBUS.inl"
};

static char const *const _crca_x_25_alias_names[] = { "CRC-16/IBM-SDLC","CRC-16/ISO-HDLC","CRC-B",NULL };
static struct DeeCRC16Algorithm const _crca_x_25 = {
 {"X-25",_crca_x_25_alias_names,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_16_ioref}},DEE_UINT16_C(0xffff),DEE_UINT16_C(0xffff),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.X-25.inl"
};

static char const *const _crca_xmodem_alias_names[] = { "ZMODEM","CRC-16/ACORN","CRC-16/LTE",NULL };
static struct DeeCRC16Algorithm const _crca_xmodem = {
 {"XMODEM",_crca_xmodem_alias_names,16,DEE_CRCHASHER_SIZE_16,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_16}},DEE_UINT16_C(0x0000),DEE_UINT16_C(0x0000),DEE_UINT16_C(0xffff),
#include "algorithms/algorithm.XMODEM.inl"
};

#define WIDTH 24
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static char const *const _crca_crc_24_alias_names[] = { "CRC-24/OPENPGP",NULL };
static struct DeeCRC32Algorithm const _crca_crc_24 = {
 {"CRC-24",_crca_crc_24_alias_names,24,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_24}},DEE_UINT32_C(0x00b704ce),DEE_UINT32_C(0x00000000),DEE_UINT32_C(0x00ffffff),
#include "algorithms/algorithm.CRC-24.inl"
};

#define WIDTH 24
#define IN_REFLECTED 1
#define OUT_REFLECTED 1
#include "hashfunc.inl"
static struct DeeCRC32Algorithm const _crca_crc_24__ble = {
 {"CRC-24/BLE",NULL,24,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_24_ioref}},DEE_UINT32_C(0x00555555),DEE_UINT32_C(0x00000000),DEE_UINT32_C(0x00ffffff),
#include "algorithms/algorithm.CRC-24_BLE.inl"
};

static struct DeeCRC32Algorithm const _crca_crc_24__flexray_a = {
 {"CRC-24/FLEXRAY-A",NULL,24,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_24}},DEE_UINT32_C(0x00fedcba),DEE_UINT32_C(0x00000000),DEE_UINT32_C(0x00ffffff),
#include "algorithms/algorithm.CRC-24_FLEXRAY-A.inl"
};

static struct DeeCRC32Algorithm const _crca_crc_24__flexray_b = {
 {"CRC-24/FLEXRAY-B",NULL,24,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_24}},DEE_UINT32_C(0x00abcdef),DEE_UINT32_C(0x00000000),DEE_UINT32_C(0x00ffffff),
#include "algorithms/algorithm.CRC-24_FLEXRAY-B.inl"
};

static struct DeeCRC32Algorithm const _crca_crc_24__interlaken = {
 {"CRC-24/INTERLAKEN",NULL,24,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_24}},DEE_UINT32_C(0x00ffffff),DEE_UINT32_C(0x00ffffff),DEE_UINT32_C(0x00ffffff),
#include "algorithms/algorithm.CRC-24_INTERLAKEN.inl"
};

static struct DeeCRC32Algorithm const _crca_crc_24__lte_a = {
 {"CRC-24/LTE-A",NULL,24,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_24}},DEE_UINT32_C(0x00000000),DEE_UINT32_C(0x00000000),DEE_UINT32_C(0x00ffffff),
#include "algorithms/algorithm.CRC-24_LTE-A.inl"
};

static struct DeeCRC32Algorithm const _crca_crc_24__lte_b = {
 {"CRC-24/LTE-B",NULL,24,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_24}},DEE_UINT32_C(0x00000000),DEE_UINT32_C(0x00000000),DEE_UINT32_C(0x00ffffff),
#include "algorithms/algorithm.CRC-24_LTE-B.inl"
};

#define WIDTH 30
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static struct DeeCRC32Algorithm const _crca_crc_30__cdma = {
 {"CRC-30/CDMA",NULL,30,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_30}},DEE_UINT32_C(0x3fffffff),DEE_UINT32_C(0x3fffffff),DEE_UINT32_C(0x3fffffff),
#include "algorithms/algorithm.CRC-30_CDMA.inl"
};

#define WIDTH 31
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static struct DeeCRC32Algorithm const _crca_crc_31__philips = {
 {"CRC-31/PHILIPS",NULL,31,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_31}},DEE_UINT32_C(0x7fffffff),DEE_UINT32_C(0x7fffffff),DEE_UINT32_C(0x7fffffff),
#include "algorithms/algorithm.CRC-31_PHILIPS.inl"
};

#define WIDTH 32
#define IN_REFLECTED 1
#define OUT_REFLECTED 1
#include "hashfunc.inl"
static char const *const _crca_crc_32_alias_names[] = { "CRC-32/ADCCP","PKZIP","CRC-32/IEEE-802.3","IEEE-802.3",NULL };
static struct DeeCRC32Algorithm const _crca_crc_32 = {
 {"CRC-32",_crca_crc_32_alias_names,32,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_32_ioref}},DEE_UINT32_C(0xffffffff),DEE_UINT32_C(0xffffffff),DEE_UINT32_C(0xffffffff),
#include "algorithms/algorithm.CRC-32.inl"
};

static struct DeeCRC32Algorithm const _crca_crc_32__autosar = {
 {"CRC-32/AUTOSAR",NULL,32,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_32_ioref}},DEE_UINT32_C(0xffffffff),DEE_UINT32_C(0xffffffff),DEE_UINT32_C(0xffffffff),
#include "algorithms/algorithm.CRC-32_AUTOSAR.inl"
};

#define WIDTH 32
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static char const *const _crca_crc_32__bzip2_alias_names[] = { "CRC-32/AAL5","CRC-32/DECT-B","B-CRC-32",NULL };
static struct DeeCRC32Algorithm const _crca_crc_32__bzip2 = {
 {"CRC-32/BZIP2",_crca_crc_32__bzip2_alias_names,32,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_32}},DEE_UINT32_C(0xffffffff),DEE_UINT32_C(0xffffffff),DEE_UINT32_C(0xffffffff),
#include "algorithms/algorithm.CRC-32_BZIP2.inl"
};

static struct DeeCRC32Algorithm const _crca_crc_32c = {
 {"CRC-32C",NULL,32,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_32_ioref}},DEE_UINT32_C(0xffffffff),DEE_UINT32_C(0xffffffff),DEE_UINT32_C(0xffffffff),
#include "algorithms/algorithm.CRC-32C.inl"
};

static struct DeeCRC32Algorithm const _crca_crc_32d = {
 {"CRC-32D",NULL,32,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_32_ioref}},DEE_UINT32_C(0xffffffff),DEE_UINT32_C(0xffffffff),DEE_UINT32_C(0xffffffff),
#include "algorithms/algorithm.CRC-32D.inl"
};

static struct DeeCRC32Algorithm const _crca_crc_32__mpeg_2 = {
 {"CRC-32/MPEG-2",NULL,32,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_32}},DEE_UINT32_C(0xffffffff),DEE_UINT32_C(0x00000000),DEE_UINT32_C(0xffffffff),
#include "algorithms/algorithm.CRC-32_MPEG-2.inl"
};

static char const *const _crca_crc_32__posix_alias_names[] = { "POSIX",NULL };
static struct DeeCRC32Algorithm const _crca_crc_32__posix = {
 {"CRC-32/POSIX",_crca_crc_32__posix_alias_names,32,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_32}},DEE_UINT32_C(0x00000000),DEE_UINT32_C(0xffffffff),DEE_UINT32_C(0xffffffff),
#include "algorithms/algorithm.CRC-32_POSIX.inl"
};

static struct DeeCRC32Algorithm const _crca_crc_32q = {
 {"CRC-32Q",NULL,32,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_32}},DEE_UINT32_C(0x00000000),DEE_UINT32_C(0x00000000),DEE_UINT32_C(0xffffffff),
#include "algorithms/algorithm.CRC-32Q.inl"
};

static struct DeeCRC32Algorithm const _crca_jamcrc = {
 {"JAMCRC",NULL,32,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_32_ioref}},DEE_UINT32_C(0xffffffff),DEE_UINT32_C(0x00000000),DEE_UINT32_C(0xffffffff),
#include "algorithms/algorithm.JAMCRC.inl"
};

static struct DeeCRC32Algorithm const _crca_xfer = {
 {"XFER",NULL,32,DEE_CRCHASHER_SIZE_32,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_32}},DEE_UINT32_C(0x00000000),DEE_UINT32_C(0x00000000),DEE_UINT32_C(0xffffffff),
#include "algorithms/algorithm.XFER.inl"
};

#define WIDTH 40
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static struct DeeCRC64Algorithm const _crca_crc_40__gsm = {
 {"CRC-40/GSM",NULL,40,DEE_CRCHASHER_SIZE_64,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_40}},DEE_UINT64_C(0x0000000000000000),DEE_UINT64_C(0x000000ffffffffff),DEE_UINT64_C(0x000000ffffffffff),
#include "algorithms/algorithm.CRC-40_GSM.inl"
};

#define WIDTH 64
#define IN_REFLECTED 0
#define OUT_REFLECTED 0
#include "hashfunc.inl"
static struct DeeCRC64Algorithm const _crca_crc_64 = {
 {"CRC-64",NULL,64,DEE_CRCHASHER_SIZE_64,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_64}},DEE_UINT64_C(0x0000000000000000),DEE_UINT64_C(0x0000000000000000),DEE_UINT64_C(0xffffffffffffffff),
#include "algorithms/algorithm.CRC-64.inl"
};

static struct DeeCRC64Algorithm const _crca_crc_64__we = {
 {"CRC-64/WE",NULL,64,DEE_CRCHASHER_SIZE_64,DEE_CRCALGORITHM_FLAG_NONE,
 {(DeeCRCHashFunc8)&_hashimpl_64}},DEE_UINT64_C(0xffffffffffffffff),DEE_UINT64_C(0xffffffffffffffff),DEE_UINT64_C(0xffffffffffffffff),
#include "algorithms/algorithm.CRC-64_WE.inl"
};

#define WIDTH 64
#define IN_REFLECTED 1
#define OUT_REFLECTED 1
#include "hashfunc.inl"
static struct DeeCRC64Algorithm const _crca_crc_64__xz = {
 {"CRC-64/XZ",NULL,64,DEE_CRCHASHER_SIZE_64,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_64_ioref}},DEE_UINT64_C(0xffffffffffffffff),DEE_UINT64_C(0xffffffffffffffff),DEE_UINT64_C(0xffffffffffffffff),
#include "algorithms/algorithm.CRC-64_XZ.inl"
};

static struct DeeCRC64Algorithm const _crca_crc_64__jones = {
 {"CRC-64/JONES",NULL,64,DEE_CRCHASHER_SIZE_64,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_64_ioref}},DEE_UINT64_C(0xffffffffffffffff),DEE_UINT64_C(0x0000000000000000),DEE_UINT64_C(0xffffffffffffffff),
#include "algorithms/algorithm.CRC-64_JONES.inl"
};

static char const *const _crca_crc_64__ecma_alias_names[] = { "CRC-64/ECMA-182",NULL };
static struct DeeCRC64Algorithm const _crca_crc_64__ecma = {
 {"CRC-64/ECMA",_crca_crc_64__ecma_alias_names,64,DEE_CRCHASHER_SIZE_64,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_64_ioref}},DEE_UINT64_C(0x0000000000000000),DEE_UINT64_C(0x0000000000000000),DEE_UINT64_C(0xffffffffffffffff),
#include "algorithms/algorithm.CRC-64_ECMA.inl"
};

static char const *const _crca_crc_64__iso_alias_names[] = { "FP-64",NULL };
static struct DeeCRC64Algorithm const _crca_crc_64__iso = {
 {"CRC-64/ISO",_crca_crc_64__iso_alias_names,64,DEE_CRCHASHER_SIZE_64,DEE_CRCALGORITHM_FLAG_REVIN|DEE_CRCALGORITHM_FLAG_REVOUT,
 {(DeeCRCHashFunc8)&_hashimpl_64_ioref}},DEE_UINT64_C(0x0000000000000000),DEE_UINT64_C(0x0000000000000000),DEE_UINT64_C(0xffffffffffffffff),
#include "algorithms/algorithm.CRC-64_ISO.inl"
};


static struct DeeCRCAlgorithm const *const _crc_algorithms[] = {
 (struct DeeCRCAlgorithm const *)&_crca_crc_1,
 (struct DeeCRCAlgorithm const *)&_crca_crc_3__rohc,
 (struct DeeCRCAlgorithm const *)&_crca_crc_4__interlaken,
 (struct DeeCRCAlgorithm const *)&_crca_crc_4__itu,
 (struct DeeCRCAlgorithm const *)&_crca_crc_5__epc,
 (struct DeeCRCAlgorithm const *)&_crca_crc_5__itu,
 (struct DeeCRCAlgorithm const *)&_crca_crc_5__usb,
 (struct DeeCRCAlgorithm const *)&_crca_crc_6__cdma2000_a,
 (struct DeeCRCAlgorithm const *)&_crca_crc_6__cdma2000_b,
 (struct DeeCRCAlgorithm const *)&_crca_crc_6__darc,
 (struct DeeCRCAlgorithm const *)&_crca_crc_6__itu,
 (struct DeeCRCAlgorithm const *)&_crca_crc_7,
 (struct DeeCRCAlgorithm const *)&_crca_crc_7__rohc,
 (struct DeeCRCAlgorithm const *)&_crca_crc_7__umts,
 (struct DeeCRCAlgorithm const *)&_crca_crc_8,
 (struct DeeCRCAlgorithm const *)&_crca_crc_8__autosar,
 (struct DeeCRCAlgorithm const *)&_crca_crc_8__cdma2000,
 (struct DeeCRCAlgorithm const *)&_crca_crc_8__darc,
 (struct DeeCRCAlgorithm const *)&_crca_crc_8__dvb_s2,
 (struct DeeCRCAlgorithm const *)&_crca_crc_8__ebu,
 (struct DeeCRCAlgorithm const *)&_crca_crc_8__i_code,
 (struct DeeCRCAlgorithm const *)&_crca_crc_8__itu,
 (struct DeeCRCAlgorithm const *)&_crca_crc_8__lte,
 (struct DeeCRCAlgorithm const *)&_crca_crc_8__maxim,
 (struct DeeCRCAlgorithm const *)&_crca_crc_8__opensafety,
 (struct DeeCRCAlgorithm const *)&_crca_crc_8__rohc,
 (struct DeeCRCAlgorithm const *)&_crca_crc_8__sae_j1850,
 (struct DeeCRCAlgorithm const *)&_crca_crc_8__wcdma,
 (struct DeeCRCAlgorithm const *)&_crca_crc_10,
 (struct DeeCRCAlgorithm const *)&_crca_crc_10__cdma2000,
 (struct DeeCRCAlgorithm const *)&_crca_crc_11,
 (struct DeeCRCAlgorithm const *)&_crca_crc_11__umts,
 (struct DeeCRCAlgorithm const *)&_crca_crc_12__cdma2000,
 (struct DeeCRCAlgorithm const *)&_crca_crc_12__dect,
 (struct DeeCRCAlgorithm const *)&_crca_crc_12__umts,
 (struct DeeCRCAlgorithm const *)&_crca_crc_13__bbc,
 (struct DeeCRCAlgorithm const *)&_crca_crc_14__darc,
 (struct DeeCRCAlgorithm const *)&_crca_crc_15,
 (struct DeeCRCAlgorithm const *)&_crca_crc_15__mpt1327,
 (struct DeeCRCAlgorithm const *)&_crca_arc,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__aug_ccitt,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__buypass,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__ccitt_false,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__cdma2000,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__cms,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__dds_110,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__dect_r,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__dect_x,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__dnp,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__en_13757,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__genibus,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__lj1200,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__maxim,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__mcrf4xx,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__opensafety_a,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__opensafety_b,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__profibus,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__riello,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__t10_dif,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__teledisk,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__tms37157,
 (struct DeeCRCAlgorithm const *)&_crca_crc_16__usb,
 (struct DeeCRCAlgorithm const *)&_crca_crc_a,
 (struct DeeCRCAlgorithm const *)&_crca_kermit,
 (struct DeeCRCAlgorithm const *)&_crca_modbus,
 (struct DeeCRCAlgorithm const *)&_crca_x_25,
 (struct DeeCRCAlgorithm const *)&_crca_xmodem,
 (struct DeeCRCAlgorithm const *)&_crca_crc_24,
 (struct DeeCRCAlgorithm const *)&_crca_crc_24__ble,
 (struct DeeCRCAlgorithm const *)&_crca_crc_24__flexray_a,
 (struct DeeCRCAlgorithm const *)&_crca_crc_24__flexray_b,
 (struct DeeCRCAlgorithm const *)&_crca_crc_24__interlaken,
 (struct DeeCRCAlgorithm const *)&_crca_crc_24__lte_a,
 (struct DeeCRCAlgorithm const *)&_crca_crc_24__lte_b,
 (struct DeeCRCAlgorithm const *)&_crca_crc_30__cdma,
 (struct DeeCRCAlgorithm const *)&_crca_crc_31__philips,
 (struct DeeCRCAlgorithm const *)&_crca_crc_32,
 (struct DeeCRCAlgorithm const *)&_crca_crc_32__autosar,
 (struct DeeCRCAlgorithm const *)&_crca_crc_32__bzip2,
 (struct DeeCRCAlgorithm const *)&_crca_crc_32c,
 (struct DeeCRCAlgorithm const *)&_crca_crc_32d,
 (struct DeeCRCAlgorithm const *)&_crca_crc_32__mpeg_2,
 (struct DeeCRCAlgorithm const *)&_crca_crc_32__posix,
 (struct DeeCRCAlgorithm const *)&_crca_crc_32q,
 (struct DeeCRCAlgorithm const *)&_crca_jamcrc,
 (struct DeeCRCAlgorithm const *)&_crca_xfer,
 (struct DeeCRCAlgorithm const *)&_crca_crc_40__gsm,
 (struct DeeCRCAlgorithm const *)&_crca_crc_64,
 (struct DeeCRCAlgorithm const *)&_crca_crc_64__we,
 (struct DeeCRCAlgorithm const *)&_crca_crc_64__xz,
 (struct DeeCRCAlgorithm const *)&_crca_crc_64__jones,
 (struct DeeCRCAlgorithm const *)&_crca_crc_64__ecma,
 (struct DeeCRCAlgorithm const *)&_crca_crc_64__iso,
};
//[[[end]]]

#define _crcname_is_nondef_sep(ch)\
((ch) == '\t' || (ch) == ' ' || \
 (ch) == '_' || (ch) == '/' || (ch) == '\\')

static int _crcname_equals(
 DEE_A_IN char const *a, DEE_A_IN char const *b) {
 char cha,chb;
 DEE_ASSERT(a); DEE_ASSERT(b);
 while (1) {
  cha = *a++;
  do chb = *b++; while (chb == '(' || chb == ')');
  if (chb >= 'a' && chb <= 'z') chb -= ('a'-'A');
  if (cha != chb) {
   if (cha == '/') cha = '-';
   if (_crcname_is_nondef_sep(chb)) {
    while (1) {
     chb = *b;
     if (!_crcname_is_nondef_sep(chb)) break; ++b;
    }
    chb = '-';
   }
   if (cha != chb) return 0;
  }
  if (!cha) break;
 }
 return 1;
}

DEE_A_RET_NOEXCEPT(NULL) struct DeeCRCAlgorithm const *
DeeCRCAlgorithm_Open(DEE_A_IN_Z char const *name) {
 struct DeeCRCAlgorithm const *const *iter,*const *end,*algo;
 char const *const *alias_iter; DEE_ASSERT(name);
 end = (iter = _crc_algorithms)+(sizeof(_crc_algorithms)/sizeof(*_crc_algorithms));
 while (iter != end) {
  algo = *iter++;
  if (_crcname_equals(algo->crc_name,name)) return algo;
  if ((alias_iter = algo->crc_alias) != NULL) while (*alias_iter) {
   if (_crcname_equals(*alias_iter,name)) return algo;
   ++alias_iter;
  }
 }
 return NULL;
}
DEE_A_RET_EXCEPT(NULL) struct DeeCRCAlgorithm const *
DeeCRCAlgorithm_OpenWithError(DEE_A_IN_Z char const *name) {
 struct DeeCRCAlgorithm const *result;
 DEE_ASSERT(name);
 if DEE_UNLIKELY((result = DeeCRCAlgorithm_Open(name)) == NULL) {
  DeeError_SetStringf(&DeeErrorType_ValueError,
                      "Unknown crc algorithm: %q",name);
  return NULL;
 }
 return result;
}

DEE_A_RET_EXCEPT(-1) int DeeCRCAlgorithm_Enum(
 DEE_A_IN Dee_uint32_t flags, DEE_A_IN DeeEnumCrcAlgorithmsFunc func, void *closure) {
 struct DeeCRCAlgorithm const *const *iter,*const *end,*algo;
 char const *const *alias_iter; int temp;
 DEE_ASSERT(func);
 end = (iter = _crc_algorithms)+(sizeof(_crc_algorithms)/sizeof(*_crc_algorithms));
 while (iter != end) {
  algo = *iter++;
  temp = (*func)(algo,DEE_CRCALGORITHM_ENUM_FLAG_NONE,
                 algo->crc_name,closure);
  if DEE_UNLIKELY(temp != 0) return temp;
  if ((flags&DEE_CRCALGORITHM_ENUM_FLAG_ALIAS)!=0) {
   if ((alias_iter = algo->crc_alias) != NULL) while (*alias_iter) {
    temp = (*func)(algo,DEE_CRCALGORITHM_ENUM_FLAG_ALIAS,
                   *alias_iter,closure);
    if DEE_UNLIKELY(temp != 0) return temp;
    ++alias_iter;
   }
  }
 }
 return 0;
}



DEE_A_RET_NOEXCEPT(-1) int DeeCRCHasher_InitWithName(
 DEE_A_OUT struct DeeCRCHasher *self, DEE_A_IN_Z char const *name) {
 struct DeeCRCAlgorithm const *_algo;
 if DEE_UNLIKELY((_algo = DeeCRCAlgorithm_Open(name)) == NULL) return -1;
 DeeCRCHasher_InitWithAlgorithm(self,_algo);
 return 0;
}
DEE_A_RET_EXCEPT(-1) int DeeCRCHasher_InitWithNameAndError(
 DEE_A_OUT struct DeeCRCHasher *self, DEE_A_IN_Z char const *name) {
 struct DeeCRCAlgorithm const *_algo;
 if DEE_UNLIKELY((_algo = DeeCRCAlgorithm_OpenWithError(name)) == NULL) return -1;
 DeeCRCHasher_InitWithAlgorithm(self,_algo);
 return 0;
}

#define _algo(T) ((T*)algo)
#define _data    (*(Dee_uint8_t **)&p)
void DeeCRCHasher_AddData(
 DEE_A_INOUT struct DeeCRCHasher *self,
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s) {
 struct DeeCRCAlgorithm const *algo;
 DEE_ASSERT(self); DEE_ASSERT(!s || p);
 algo = self->crc_algo;
 switch (DeeCRCAlgorithm_Size(algo)) {
  case DEE_CRCHASHER_SIZE_8:  self->crc_result8  = (*algo->crc_hash8 )(algo,self->crc_result8, p,s); break;
  case DEE_CRCHASHER_SIZE_16: self->crc_result16 = (*algo->crc_hash16)(algo,self->crc_result16,p,s); break;
  case DEE_CRCHASHER_SIZE_32: self->crc_result32 = (*algo->crc_hash32)(algo,self->crc_result32,p,s); break;
  case DEE_CRCHASHER_SIZE_64: self->crc_result64 = (*algo->crc_hash64)(algo,self->crc_result64,p,s); break;
  default: DEE_BUILTIN_UNREACHABLE();
 }
}
void DeeCRCHasher_AddString(
 DEE_A_INOUT struct DeeCRCHasher *self, DEE_A_IN_Z char const *s) {
 DEE_ASSERT(self); DEE_ASSERT(s);
 DeeCRCHasher_AddData(self,s,strlen(s));
}
#undef _data
#undef _algo




static DeeObject *DEE_CALL _hashlib_crc(DeeObject *args) {
 DeeObject *algo,*data,*size = NULL; void *p; Dee_size_t s;
 struct DeeCRCHasher hasher; struct DeeCRCAlgorithm const *used_algo;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"oo|o:crc",&algo,&data,&size) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (!DeeString_Check(data)) {
  if DEE_UNLIKELY(!size) { DeeError_TypeError_UnexpectedType(data,&DeeString_Type); return NULL; }
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&s) != 0) return NULL;
 } else
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 {
  p = DeeString_STR(data);
  s = DeeString_SIZE(data)*sizeof(char);
  if (size) {
   Dee_size_t s2;
   if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&s2) != 0) return NULL;
   if (s2 < s) s = s2;
  }
 }
 if (DeeCRCAlgorithm_Check(algo))
  used_algo = DeeCRCAlgorithm_Algo(algo);
 else {
  if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(algo,&DeeString_Type) != 0) return NULL;
  used_algo = DeeCRCAlgorithm_OpenWithError(DeeString_STR(algo));
  if DEE_UNLIKELY(!used_algo) return NULL;
 }
 DeeCRCHasher_InitWithAlgorithm(&hasher,used_algo);
 DeeCRCHasher_AddData(&hasher,p,s);
 switch (DeeCRCHasher_Size(&hasher)) {
  case DEE_CRCHASHER_SIZE_8:  return DeeObject_New(Dee_uint8_t, DeeCRCHasher_Result8 (&hasher));
  case DEE_CRCHASHER_SIZE_16: return DeeObject_New(Dee_uint16_t,DeeCRCHasher_Result16(&hasher));
  case DEE_CRCHASHER_SIZE_32: return DeeObject_New(Dee_uint32_t,DeeCRCHasher_Result32(&hasher));
  case DEE_CRCHASHER_SIZE_64: return DeeObject_New(Dee_uint64_t,DeeCRCHasher_Result64(&hasher));
  default: DEE_BUILTIN_UNREACHABLE();
 }
}



static int DEE_CALL _deecrcalgorithm_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeCRCAlgorithmObject *self, DeeCRCAlgorithmObject *right) {
 self->ca_algo = right->ca_algo;
 return 0;
}
static int DEE_CALL _deecrcalgorithm_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self), DeeCRCAlgorithmObject *self, DeeObject *args) {
 DeeObject *name;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:crc_algorithm",&name) != 0) return -1;
 if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(name,&DeeString_Type) != 0) return -1;
 if DEE_UNLIKELY((self->ca_algo = DeeCRCAlgorithm_OpenWithError(DeeString_STR(name))) == NULL) return -1;
 return 0;
}


#if 1
static DeeCRCAlgorithmIteratorObject *DEE_CALL _deecrcalgorithmclass_enum(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeCRCAlgorithmIteratorObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":enum") != 0) return NULL;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeCRCAlgorithmIteratorObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeCRCAlgorithmIterator_Type);
 result->cai_pos = _crc_algorithms;
 result->cai_end = _crc_algorithms+(sizeof(_crc_algorithms)/sizeof(*_crc_algorithms));
 return result;
}
#else
static int DEE_CALL _deecrcalgorithm_enum_callback(
 DEE_A_IN struct DeeCRCAlgorithm const *algo,
 DEE_A_IN Dee_uint32_t DEE_UNUSED(flags),
 DEE_A_IN_Z char const *DEE_UNUSED(name), DeeObject *closure) {
 DeeCRCAlgorithmObject *algo_ob; int result;
 if DEE_UNLIKELY((algo_ob = DeeObject_MALLOC(DeeCRCAlgorithmObject)) == NULL) return -1;
 DeeObject_INIT(algo_ob,&DeeCRCAlgorithm_Type);
 algo_ob->ca_algo = algo;
 result = DeeList_Append(closure,(DeeObject *)algo_ob);
 Dee_DECREF(algo_ob);
 return result;
}
static DeeObject *DEE_CALL _deecrcalgorithmclass_enum(
 DeeTypeObject *DEE_UNUSED(self), DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *result;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,":enum") != 0) return NULL;
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) return NULL;
 if DEE_UNLIKELY(DeeCRCAlgorithm_Enum(DEE_CRCALGORITHM_ENUM_FLAG_NONE,
  (DeeEnumCrcAlgorithmsFunc)&_deecrcalgorithm_enum_callback,
  result) != 0) Dee_CLEAR(result);
 return result;
}
#endif
static DeeObject *DEE_CALL _deecrcalgorithm_tp_str(DeeCRCAlgorithmObject *self) {
 return DeeString_New(self->ca_algo->crc_name);
}

static struct DeeMethodDef const _deecrcalgorithm_tp_class_methods[] = {
 DEE_METHODDEF_v100("enum",member(&_deecrcalgorithmclass_enum),
  "() -> iterable\n"
  "@return: Returns a sequence of all known crc-algorithms"),
 DEE_METHODDEF_END_v100
};

static DeeObject *DEE_CALL _deecrcalgorithm_name_get(
 DeeCRCAlgorithmObject *self, void *DEE_UNUSED(closure)) {
 return DeeString_New(self->ca_algo->crc_name);
}
static DeeObject *DEE_CALL _deecrcalgorithm_alias_get(
 DeeCRCAlgorithmObject *self, void *DEE_UNUSED(closure)) {
 DeeObject *result,*name; char const *const *iter; int temp;
 if DEE_UNLIKELY((result = DeeList_New()) == NULL) return NULL;
 if DEE_UNLIKELY((iter = self->ca_algo->crc_alias) != NULL) {
  while (*iter) {
   if DEE_UNLIKELY((name = DeeString_New(*iter)) == NULL) {
err_r: Dee_DECREF(result); return NULL;
   }
   temp = DeeList_Append(result,name);
   Dee_DECREF(name);
   if DEE_UNLIKELY(temp != 0) goto err_r;
   ++iter;
  }
 }
 return result;
}
static struct DeeGetSetDef const _deecrcalgorithm_tp_getsets[] = {
 DEE_GETSETDEF_v100("name",member(&_deecrcalgorithm_name_get),null,null,"-> string\n"
  "@return: The internal name of @this crc #crc_algorithm"),
 DEE_GETSETDEF_v100("alias",member(&_deecrcalgorithm_alias_get),null,null,"-> iterable\n"
  "@return: An iterable object listing all name aliases for @this #crc_algorithm"),
 DEE_GETSETDEF_END_v100
};


static int DEE_CALL _deecrchasher_tp_any_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeCRCHasherObject *self, DeeObject *args) {
 struct DeeCRCAlgorithm const *used_algo; DeeObject *algo;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o:crc_hasher",&algo) != 0) return -1;
 if DEE_UNLIKELY(DeeCRCAlgorithm_Check(algo)) used_algo = DeeCRCAlgorithm_Algo(algo);
 else {
  if DEE_UNLIKELY(DeeError_TypeError_CheckTypeExact(algo,&DeeString_Type) != 0) return -1;
  if DEE_UNLIKELY((used_algo = DeeCRCAlgorithm_OpenWithError(DeeString_STR(algo))) == NULL) return -1;
 }
 DeeCRCHasher_InitWithAlgorithm(&self->ch_hasher,used_algo);
 return 0;
}

static DeeObject *DEE_CALL _deecrchasher_add(
 DeeCRCHasherObject *self, DeeObject *args, void *DEE_UNUSED(closure)) {
 DeeObject *data,*size = NULL; void *p; Dee_size_t s;
 if DEE_UNLIKELY(DeeTuple_Unpack(args,"o|o:add",&data,&size) != 0) return NULL;
#if DEE_CONFIG_RUNTIME_HAVE_POINTERS
 if (!DeeString_Check(data)) {
  if DEE_UNLIKELY(!size) { DeeError_TypeError_UnexpectedType(data,&DeeString_Type); return NULL; }
  if DEE_UNLIKELY(DeeObject_GetVoidPointerEx(data,&p) != 0) return NULL;
  if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&s) != 0) return NULL;
 } else
#endif /* DEE_CONFIG_RUNTIME_HAVE_POINTERS */
 {
  p = DeeString_STR(data);
  s = DeeString_SIZE(data)*sizeof(char);
  if (size) {
   Dee_size_t s2;
   if DEE_UNLIKELY(DeeObject_Cast(Dee_size_t,size,&s2) != 0) return NULL;
   if (s2 < s) s = s2;
  }
 }
 DeeCRCHasher_AddData(&self->ch_hasher,p,s);
 DeeReturn_None;
}
static DeeObject *DEE_CALL _deecrchasher_result_get(
 DeeCRCHasherObject *self, void *DEE_UNUSED(closure)) {
 switch (DeeCRCHasher_Size(&self->ch_hasher)) {
  case DEE_CRCHASHER_SIZE_8:  return DeeObject_New(Dee_uint8_t, DeeCRCHasher_Result8 (&self->ch_hasher));
  case DEE_CRCHASHER_SIZE_16: return DeeObject_New(Dee_uint16_t,DeeCRCHasher_Result16(&self->ch_hasher));
  case DEE_CRCHASHER_SIZE_32: return DeeObject_New(Dee_uint32_t,DeeCRCHasher_Result32(&self->ch_hasher));
  case DEE_CRCHASHER_SIZE_64: return DeeObject_New(Dee_uint64_t,DeeCRCHasher_Result64(&self->ch_hasher));
  default: DEE_BUILTIN_UNREACHABLE();
 }
}
static DeeCRCAlgorithmObject *DEE_CALL _deecrchasher_algorithm_get(
 DeeCRCHasherObject *self, void *DEE_UNUSED(closure)) {
 DeeCRCAlgorithmObject *result;
 if DEE_UNLIKELY((result = DeeObject_MALLOC(DeeCRCAlgorithmObject)) == NULL) return NULL;
 DeeObject_INIT(result,&DeeCRCAlgorithm_Type);
 result->ca_algo = self->ch_hasher.crc_algo;
 return result;
}

static struct DeeMethodDef const _deecrchasher_tp_methods[] = {
 DEE_METHODDEF_v100("add",member(&_deecrchasher_add),
  "(none *p, size_t s) -> none\n"
  "(string data, size_t s = size_t(-1)) -> none\n"
  "\tAdds additional data to the crc generation process.\n"
  "\tHow data is split does not matter for the crc result.\n"
  "\tOnly order and content of packages matters\n"),
 DEE_METHODDEF_END_v100
};
static struct DeeGetSetDef const _deecrchasher_tp_getsets[] = {
 DEE_GETSETDEF_v100("result",member(&_deecrchasher_result_get),null,null,"-> Integer\n"
  "@return: The current hash result value\n"
  "\tNote, that the type returned by this property depends on the used #(crc_algorithm).\n"),
 DEE_GETSETDEF_v100("algorithm",member(&_deecrchasher_algorithm_get),null,null,"-> crc_algorithm\n"
  "@return: The crc_algorithm that is used by this hasher"),
 DEE_GETSETDEF_END_v100
};




DeeTypeObject DeeCRCAlgorithm_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("crc_algorithm"),member(
  "Descriptor class for a specific crc algorithm\n"

  "(string name)\n"
    "@throws Error.ValueError: Unknown CRC algorithm\n"
    "@return: The crc algorithms associated with a given @name\n"
    "\tUse 'crc_algorithm.enum()' for listing all known algorithms.\n"
    "\tNote, that when checking the given @name against the actual name, case is ignored, as well as having some symbol characters, such as '_', '-' or ' ' all compare equal when compared against another symbol.\n"
    "\tMultiple consecutive symbols are ignored. Parenthesis is ignored.\n"
    "\t>> // returns the same as 'crc_algorithm(\"CRC-32\")'\n"
    "\t>> crc_algorithm(\"CRC (32)\");\n"
    "\t-> CRC-32\n"
    "\tNOTE: Other functions/types that accept a string instead of a crc_algorithm, use the same special string-compare function, allowing for the same name aliasing.\n"
  "operator __str__() -> string\n"
    "\tReturns the actual name of the crc (same as returned by #crc_algorithm.name)\n"
  ),null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeCRCAlgorithmObject),null,
  null,member(&_deecrcalgorithm_tp_copy_ctor),
  null,member(&_deecrcalgorithm_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(
  member(&_deecrcalgorithm_tp_str),null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,
  member(_deecrcalgorithm_tp_getsets),null,null,null,
  member(_deecrcalgorithm_tp_class_methods)),
 DEE_TYPE_OBJECT_FOOTER_v100
};


static int _deecrcalgorithmiterator_tp_copy_ctor(
 DeeTypeObject *DEE_UNUSED(tp_self),
 DeeCRCAlgorithmIteratorObject *self,
 DeeCRCAlgorithmIteratorObject *right) {
 self->cai_pos = (struct DeeCRCAlgorithm const *const *)
  DeeAtomicPtr_Load(right->cai_pos,memory_order_seq_cst);
 self->cai_end = right->cai_end;
 return 0;
}
static int _deecrcalgorithmiterator_tp_seq_iter_next(
 DeeCRCAlgorithmIteratorObject *self, DeeCRCAlgorithmObject **result) {
 struct DeeCRCAlgorithm const *const *iterpos;
 iterpos = (struct DeeCRCAlgorithm const *const *)DeeAtomicPtr_FetchAdd(
  self->cai_pos,sizeof(struct DeeCRCAlgorithm const *),memory_order_seq_cst);
 if (iterpos >= self->cai_end) {
  DeeAtomicPtr_SubFetch(self->cai_pos,
                        sizeof(struct DeeCRCAlgorithm const *),
                        memory_order_seq_cst);
  return 1;
 }
 if DEE_UNLIKELY((*result = DeeObject_MALLOC(DeeCRCAlgorithmObject)) == NULL) return -1;
 DeeObject_INIT(*result,&DeeCRCAlgorithm_Type);
 (*result)->ca_algo = *iterpos;
 return 0;
}

static struct DeeMethodDef const _deecrcalgorithmiterator_tp_methods[] = {
 DEE_METHODDEF_CONST_v100("at",member(&_deegenericiterable_at),NULL),
 DEE_METHODDEF_CONST_v100("back",member(&_deegenericiterable_back),NULL),
 DEE_METHODDEF_CONST_v100("contains",member(&_deegenericiterable_contains),NULL),
 DEE_METHODDEF_CONST_v100("count",member(&_deegenericiterable_count),NULL),
 DEE_METHODDEF_CONST_v100("empty",member(&_deegenericiterable_empty),NULL),
 DEE_METHODDEF_CONST_v100("find",member(&_deegenericiterable_find),NULL),
 DEE_METHODDEF_CONST_v100("front",member(&_deegenericiterable_front),NULL),
 DEE_METHODDEF_CONST_v100("index",member(&_deegenericiterable_index),NULL),
 DEE_METHODDEF_CONST_v100("locate",member(&_deegenericiterable_locate),NULL),
 DEE_METHODDEF_CONST_v100("locate_all",member(&_deegenericiterable_locate_all),NULL),
 DEE_METHODDEF_CONST_v100("non_empty",member(&_deegenericiterable_non_empty),NULL),
 DEE_METHODDEF_CONST_v100("reversed",member(&_deegenericiterable_reversed),NULL),
 DEE_METHODDEF_CONST_v100("rfind",member(&_deegenericiterable_rfind),NULL),
 DEE_METHODDEF_CONST_v100("rindex",member(&_deegenericiterable_rindex),NULL),
 DEE_METHODDEF_CONST_v100("rlocate",member(&_deegenericiterable_rlocate),NULL),
 DEE_METHODDEF_CONST_v100("segments",member(&_deegenericiterable_segments),NULL),
 DEE_METHODDEF_CONST_v100("sorted",member(&_deegenericiterable_sorted),NULL),
 DEE_METHODDEF_CONST_v100("transform",member(&_deegenericiterable_transform),NULL),
 DEE_METHODDEF_END_v100
};
DeeTypeObject DeeCRCAlgorithmIterator_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("crc_algorithm.enum.iterator"),null,null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeCRCAlgorithmIteratorObject),null,
  null,member(&_deecrcalgorithmiterator_tp_copy_ctor),null,null),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,
  member(&DeeObject_Copy),member(&_deecrcalgorithmiterator_tp_seq_iter_next)),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,null,
  member(_deecrcalgorithmiterator_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};


DeeTypeObject DeeCRCHasher_Type = {
 DEE_TYPE_OBJECT_HEAD_v100(member("crc_hasher"),member(
  "Helper object for creating hashes of raw data\n"

  "(crc_algorithm algo)\n"
  "(string algo)\n"
    "@throws Error.ValueError: Unknown CRC algorithm\n"
    "\tCreates a new crc hasher, that can be used for generating hashes.\n"
  ),null,null),
 DEE_TYPE_OBJECT_CONSTRUCTOR_v100(sizeof(DeeCRCHasherObject),
  null,null,null,null,member(&_deecrchasher_tp_any_ctor)),
 DEE_TYPE_OBJECT_DESTRUCTOR_v100(null,null),
 DEE_TYPE_OBJECT_ASSIGN_v100(null,null,null),
 DEE_TYPE_OBJECT_CAST_v101(null,null,null,null,null),
 DEE_TYPE_OBJECT_OBJECT_v100(null,null),
 DEE_TYPE_OBJECT_MATH_v101(
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null,null,
  null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_COMPARE_v100(null,null,null,null,null,null),
 DEE_TYPE_OBJECT_SEQ_v101(null,null,null,null,null,null,null,null,null,null),
 DEE_TYPE_OBJECT_ATTRIBUTE_v100(null,null,null,null,
  member(_deecrchasher_tp_getsets),
  member(_deecrchasher_tp_methods),null,null,null),
 DEE_TYPE_OBJECT_FOOTER_v100
};


//////////////////////////////////////////////////////////////////////////
// Module exports
struct DeeDexExportDef DeeDex_Exports[] = {
 DeeDex_EXPORT_OBJECT("crc_algorithm",&DeeCRCAlgorithm_Type),
 DeeDex_EXPORT_OBJECT("crc_hasher",&DeeCRCHasher_Type),
 DeeDex_EXPORT_FUNCTION("crc",&_hashlib_crc,
  "(string algo, none *p, size_t s) -> Integer\n"
  "(string algo, string data, size_t s = size_t(-1)) -> Integer\n"
  "(crc_algorithm algo, none *p, size_t s) -> Integer\n"
  "(crc_algorithm algo, string data, size_t s = size_t(-1)) -> Integer\n"
    "@throws Error.ValueError: Unknown CRC algorithm\n"
    "\tSimplified function for quickly generating hashes.\n"
    "\tImplementation is equivalent to the following:\n"
    "\t>> #include <hashlib>\n"
    "\t>> function crc(algo,args...) {\n"
    "\t>>\t local hasher = hashlib::crc_hasher(algo);\n"
    "\t>>\t hasher.add(args...);\n"
    "\t>>\t return hasher.result;\n"
    "\t>> }\n"),
 DeeDex_EXPORT_END
};


DEE_DECL_END

#endif /* !GUARD_DEEMON_DEX_HASHLIB_C */
