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

// Some random hash algorithm.
// >> Passed down from throughout the generations and gifted from our ancestors.
// (I have no idea what this one is. It should be somewhere in _hashlib, but I can't be bothered to look it up)

#define I DEE_UINT64_C
static Dee_hash_t const _DeeHashTable[256] = {
 I(0x0000000000000000),I(0x7ad870c830358979),I(0xf5b0e190606b12f2),I(0x8f689158505e9b8b),
 I(0xc038e5739841b68f),I(0xbae095bba8743ff6),I(0x358804e3f82aa47d),I(0x4f50742bc81f2d04),
 I(0xab28ecb46814fe75),I(0xd1f09c7c5821770c),I(0x5e980d24087fec87),I(0x24407dec384a65fe),
 I(0x6b1009c7f05548fa),I(0x11c8790fc060c183),I(0x9ea0e857903e5a08),I(0xe478989fa00bd371),
 I(0x7d08ff3b88be6f81),I(0x07d08ff3b88be6f8),I(0x88b81eabe8d57d73),I(0xf2606e63d8e0f40a),
 I(0xbd301a4810ffd90e),I(0xc7e86a8020ca5077),I(0x4880fbd87094cbfc),I(0x32588b1040a14285),
 I(0xd620138fe0aa91f4),I(0xacf86347d09f188d),I(0x2390f21f80c18306),I(0x594882d7b0f40a7f),
 I(0x1618f6fc78eb277b),I(0x6cc0863448deae02),I(0xe3a8176c18803589),I(0x997067a428b5bcf0),
 I(0xfa11fe77117cdf02),I(0x80c98ebf2149567b),I(0x0fa11fe77117cdf0),I(0x75796f2f41224489),
 I(0x3a291b04893d698d),I(0x40f16bccb908e0f4),I(0xcf99fa94e9567b7f),I(0xb5418a5cd963f206),
 I(0x513912c379682177),I(0x2be1620b495da80e),I(0xa489f35319033385),I(0xde51839b2936bafc),
 I(0x9101f7b0e12997f8),I(0xebd98778d11c1e81),I(0x64b116208142850a),I(0x1e6966e8b1770c73),
 I(0x8719014c99c2b083),I(0xfdc17184a9f739fa),I(0x72a9e0dcf9a9a271),I(0x08719014c99c2b08),
 I(0x4721e43f0183060c),I(0x3df994f731b68f75),I(0xb29105af61e814fe),I(0xc849756751dd9d87),
 I(0x2c31edf8f1d64ef6),I(0x56e99d30c1e3c78f),I(0xd9810c6891bd5c04),I(0xa3597ca0a188d57d),
 I(0xec09088b6997f879),I(0x96d1784359a27100),I(0x19b9e91b09fcea8b),I(0x636199d339c963f2),
 I(0xdf7adabd7a6e2d6f),I(0xa5a2aa754a5ba416),I(0x2aca3b2d1a053f9d),I(0x50124be52a30b6e4),
 I(0x1f423fcee22f9be0),I(0x659a4f06d21a1299),I(0xeaf2de5e82448912),I(0x902aae96b271006b),
 I(0x74523609127ad31a),I(0x0e8a46c1224f5a63),I(0x81e2d7997211c1e8),I(0xfb3aa75142244891),
 I(0xb46ad37a8a3b6595),I(0xceb2a3b2ba0eecec),I(0x41da32eaea507767),I(0x3b024222da65fe1e),
 I(0xa2722586f2d042ee),I(0xd8aa554ec2e5cb97),I(0x57c2c41692bb501c),I(0x2d1ab4dea28ed965),
 I(0x624ac0f56a91f461),I(0x1892b03d5aa47d18),I(0x97fa21650afae693),I(0xed2251ad3acf6fea),
 I(0x095ac9329ac4bc9b),I(0x7382b9faaaf135e2),I(0xfcea28a2faafae69),I(0x8632586aca9a2710),
 I(0xc9622c4102850a14),I(0xb3ba5c8932b0836d),I(0x3cd2cdd162ee18e6),I(0x460abd1952db919f),
 I(0x256b24ca6b12f26d),I(0x5fb354025b277b14),I(0xd0dbc55a0b79e09f),I(0xaa03b5923b4c69e6),
 I(0xe553c1b9f35344e2),I(0x9f8bb171c366cd9b),I(0x10e3202993385610),I(0x6a3b50e1a30ddf69),
 I(0x8e43c87e03060c18),I(0xf49bb8b633338561),I(0x7bf329ee636d1eea),I(0x012b592653589793),
 I(0x4e7b2d0d9b47ba97),I(0x34a35dc5ab7233ee),I(0xbbcbcc9dfb2ca865),I(0xc113bc55cb19211c),
 I(0x5863dbf1e3ac9dec),I(0x22bbab39d3991495),I(0xadd33a6183c78f1e),I(0xd70b4aa9b3f20667),
 I(0x985b3e827bed2b63),I(0xe2834e4a4bd8a21a),I(0x6debdf121b863991),I(0x1733afda2bb3b0e8),
 I(0xf34b37458bb86399),I(0x8993478dbb8deae0),I(0x06fbd6d5ebd3716b),I(0x7c23a61ddbe6f812),
 I(0x3373d23613f9d516),I(0x49aba2fe23cc5c6f),I(0xc6c333a67392c7e4),I(0xbc1b436e43a74e9d),
 I(0x95ac9329ac4bc9b5),I(0xef74e3e19c7e40cc),I(0x601c72b9cc20db47),I(0x1ac40271fc15523e),
 I(0x5594765a340a7f3a),I(0x2f4c0692043ff643),I(0xa02497ca54616dc8),I(0xdafce7026454e4b1),
 I(0x3e847f9dc45f37c0),I(0x445c0f55f46abeb9),I(0xcb349e0da4342532),I(0xb1eceec59401ac4b),
 I(0xfebc9aee5c1e814f),I(0x8464ea266c2b0836),I(0x0b0c7b7e3c7593bd),I(0x71d40bb60c401ac4),
 I(0xe8a46c1224f5a634),I(0x927c1cda14c02f4d),I(0x1d148d82449eb4c6),I(0x67ccfd4a74ab3dbf),
 I(0x289c8961bcb410bb),I(0x5244f9a98c8199c2),I(0xdd2c68f1dcdf0249),I(0xa7f41839ecea8b30),
 I(0x438c80a64ce15841),I(0x3954f06e7cd4d138),I(0xb63c61362c8a4ab3),I(0xcce411fe1cbfc3ca),
 I(0x83b465d5d4a0eece),I(0xf96c151de49567b7),I(0x76048445b4cbfc3c),I(0x0cdcf48d84fe7545),
 I(0x6fbd6d5ebd3716b7),I(0x15651d968d029fce),I(0x9a0d8ccedd5c0445),I(0xe0d5fc06ed698d3c),
 I(0xaf85882d2576a038),I(0xd55df8e515432941),I(0x5a3569bd451db2ca),I(0x20ed197575283bb3),
 I(0xc49581ead523e8c2),I(0xbe4df122e51661bb),I(0x3125607ab548fa30),I(0x4bfd10b2857d7349),
 I(0x04ad64994d625e4d),I(0x7e7514517d57d734),I(0xf11d85092d094cbf),I(0x8bc5f5c11d3cc5c6),
 I(0x12b5926535897936),I(0x686de2ad05bcf04f),I(0xe70573f555e26bc4),I(0x9ddd033d65d7e2bd),
 I(0xd28d7716adc8cfb9),I(0xa85507de9dfd46c0),I(0x273d9686cda3dd4b),I(0x5de5e64efd965432),
 I(0xb99d7ed15d9d8743),I(0xc3450e196da80e3a),I(0x4c2d9f413df695b1),I(0x36f5ef890dc31cc8),
 I(0x79a59ba2c5dc31cc),I(0x037deb6af5e9b8b5),I(0x8c157a32a5b7233e),I(0xf6cd0afa9582aa47),
 I(0x4ad64994d625e4da),I(0x300e395ce6106da3),I(0xbf66a804b64ef628),I(0xc5bed8cc867b7f51),
 I(0x8aeeace74e645255),I(0xf036dc2f7e51db2c),I(0x7f5e4d772e0f40a7),I(0x05863dbf1e3ac9de),
 I(0xe1fea520be311aaf),I(0x9b26d5e88e0493d6),I(0x144e44b0de5a085d),I(0x6e963478ee6f8124),
 I(0x21c640532670ac20),I(0x5b1e309b16452559),I(0xd476a1c3461bbed2),I(0xaeaed10b762e37ab),
 I(0x37deb6af5e9b8b5b),I(0x4d06c6676eae0222),I(0xc26e573f3ef099a9),I(0xb8b627f70ec510d0),
 I(0xf7e653dcc6da3dd4),I(0x8d3e2314f6efb4ad),I(0x0256b24ca6b12f26),I(0x788ec2849684a65f),
 I(0x9cf65a1b368f752e),I(0xe62e2ad306bafc57),I(0x6946bb8b56e467dc),I(0x139ecb4366d1eea5),
 I(0x5ccebf68aecec3a1),I(0x2616cfa09efb4ad8),I(0xa97e5ef8cea5d153),I(0xd3a62e30fe90582a),
 I(0xb0c7b7e3c7593bd8),I(0xca1fc72bf76cb2a1),I(0x45775673a732292a),I(0x3faf26bb9707a053),
 I(0x70ff52905f188d57),I(0x0a2722586f2d042e),I(0x854fb3003f739fa5),I(0xff97c3c80f4616dc),
 I(0x1bef5b57af4dc5ad),I(0x61372b9f9f784cd4),I(0xee5fbac7cf26d75f),I(0x9487ca0fff135e26),
 I(0xdbd7be24370c7322),I(0xa10fceec0739fa5b),I(0x2e675fb4576761d0),I(0x54bf2f7c6752e8a9),
 I(0xcdcf48d84fe75459),I(0xb71738107fd2dd20),I(0x387fa9482f8c46ab),I(0x42a7d9801fb9cfd2),
 I(0x0df7adabd7a6e2d6),I(0x772fdd63e7936baf),I(0xf8474c3bb7cdf024),I(0x829f3cf387f8795d),
 I(0x66e7a46c27f3aa2c),I(0x1c3fd4a417c62355),I(0x935745fc4798b8de),I(0xe98f353477ad31a7),
 I(0xa6df411fbfb21ca3),I(0xdc0731d78f8795da),I(0x536fa08fdfd90e51),I(0x29b7d047efec8728)
};
#undef I

DEE_A_RET_WUNUSED Dee_hash_t DeeHash_Memory(
 DEE_A_IN_RB(s) void const *p, DEE_A_IN Dee_size_t s, DEE_A_IN Dee_hash_t start) {
 DEE_ASSERT(!s || p);
 if DEE_LIKELY(s) while (1) {
  start = _DeeHashTable[(Dee_uint8_t)start ^ *(Dee_uint8_t const*)p] ^ (start >> 8);
  if DEE_UNLIKELY(!--s) break;
  ++*(Dee_uint8_t const**)&p;
 }
 return start;
}



#ifdef __INTELLISENSE__
DEE_DECL_END
#endif

