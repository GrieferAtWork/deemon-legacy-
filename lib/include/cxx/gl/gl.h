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
#pragma once
#ifndef __DEEMON_GUARD_CXX_GL_GL_H
#define __DEEMON_GUARD_CXX_GL_GL_H 1

#include <__cdefault.dee>

// NOTE: This file is less meant to be used and more meant as a
//       showcase of how easy it is to convert a c header into deemon

module gl {
 
local const GLenum = unsigned int;
local const GLboolean = unsigned char;
local const GLbitfield = unsigned int;
local const GLbyte = signed char;
local const GLshort = short;
local const GLint = int;
local const GLsizei = int;
local const GLubyte = unsigned char;
local const GLushort = unsigned short;
local const GLuint = unsigned int;
local const GLfloat = float;
local const GLclampf = float;
local const GLdouble = double;
local const GLclampd = double;
local const GLvoid = void;

/*************************************************************/

/* Version */
local const GL_VERSION_1_1 =                    1;

/* AccumOp */
local const GL_ACCUM =                          0x0100;
local const GL_LOAD =                           0x0101;
local const GL_RETURN =                         0x0102;
local const GL_MULT =                           0x0103;
local const GL_ADD =                            0x0104;

/* AlphaFunction */
local const GL_NEVER =                          0x0200;
local const GL_LESS =                           0x0201;
local const GL_EQUAL =                          0x0202;
local const GL_LEQUAL =                         0x0203;
local const GL_GREATER =                        0x0204;
local const GL_NOTEQUAL =                       0x0205;
local const GL_GEQUAL =                         0x0206;
local const GL_ALWAYS =                         0x0207;

/* AttribMask */
local const GL_CURRENT_BIT =                    0x00000001;
local const GL_POINT_BIT =                      0x00000002;
local const GL_LINE_BIT =                       0x00000004;
local const GL_POLYGON_BIT =                    0x00000008;
local const GL_POLYGON_STIPPLE_BIT =            0x00000010;
local const GL_PIXEL_MODE_BIT =                 0x00000020;
local const GL_LIGHTING_BIT =                   0x00000040;
local const GL_FOG_BIT =                        0x00000080;
local const GL_DEPTH_BUFFER_BIT =               0x00000100;
local const GL_ACCUM_BUFFER_BIT =               0x00000200;
local const GL_STENCIL_BUFFER_BIT =             0x00000400;
local const GL_VIEWPORT_BIT =                   0x00000800;
local const GL_TRANSFORM_BIT =                  0x00001000;
local const GL_ENABLE_BIT =                     0x00002000;
local const GL_COLOR_BUFFER_BIT =               0x00004000;
local const GL_HINT_BIT =                       0x00008000;
local const GL_EVAL_BIT =                       0x00010000;
local const GL_LIST_BIT =                       0x00020000;
local const GL_TEXTURE_BIT =                    0x00040000;
local const GL_SCISSOR_BIT =                    0x00080000;
local const GL_ALL_ATTRIB_BITS =                0x000fffff;

/* BeginMode */
local const GL_POINTS =                         0x0000;
local const GL_LINES =                          0x0001;
local const GL_LINE_LOOP =                      0x0002;
local const GL_LINE_STRIP =                     0x0003;
local const GL_TRIANGLES =                      0x0004;
local const GL_TRIANGLE_STRIP =                 0x0005;
local const GL_TRIANGLE_FAN =                   0x0006;
local const GL_QUADS =                          0x0007;
local const GL_QUAD_STRIP =                     0x0008;
local const GL_POLYGON =                        0x0009;

/* BlendingFactorDest */
local const GL_ZERO =                           0;
local const GL_ONE =                            1;
local const GL_SRC_COLOR =                      0x0300;
local const GL_ONE_MINUS_SRC_COLOR =            0x0301;
local const GL_SRC_ALPHA =                      0x0302;
local const GL_ONE_MINUS_SRC_ALPHA =            0x0303;
local const GL_DST_ALPHA =                      0x0304;
local const GL_ONE_MINUS_DST_ALPHA =            0x0305;

/* BlendingFactorSrc */
/*      GL_ZERO */
/*      GL_ONE */
local const GL_DST_COLOR =                      0x0306;
local const GL_ONE_MINUS_DST_COLOR =            0x0307;
local const GL_SRC_ALPHA_SATURATE =             0x0308;
/*      GL_SRC_ALPHA */
/*      GL_ONE_MINUS_SRC_ALPHA */
/*      GL_DST_ALPHA */
/*      GL_ONE_MINUS_DST_ALPHA */

/* Boolean */
local const GL_TRUE =                           1;
local const GL_FALSE =                          0;

/* ClearBufferMask */
/*      GL_COLOR_BUFFER_BIT */
/*      GL_ACCUM_BUFFER_BIT */
/*      GL_STENCIL_BUFFER_BIT */
/*      GL_DEPTH_BUFFER_BIT */

/* ClientArrayType */
/*      GL_VERTEX_ARRAY */
/*      GL_NORMAL_ARRAY */
/*      GL_COLOR_ARRAY */
/*      GL_INDEX_ARRAY */
/*      GL_TEXTURE_COORD_ARRAY */
/*      GL_EDGE_FLAG_ARRAY */

/* ClipPlaneName */
local const GL_CLIP_PLANE0 =                    0x3000;
local const GL_CLIP_PLANE1 =                    0x3001;
local const GL_CLIP_PLANE2 =                    0x3002;
local const GL_CLIP_PLANE3 =                    0x3003;
local const GL_CLIP_PLANE4 =                    0x3004;
local const GL_CLIP_PLANE5 =                    0x3005;

/* ColorMaterialFace */
/*      GL_FRONT */
/*      GL_BACK */
/*      GL_FRONT_AND_BACK */

/* ColorMaterialParameter */
/*      GL_AMBIENT */
/*      GL_DIFFUSE */
/*      GL_SPECULAR */
/*      GL_EMISSION */
/*      GL_AMBIENT_AND_DIFFUSE */

/* ColorPointerType */
/*      GL_BYTE */
/*      GL_UNSIGNED_BYTE */
/*      GL_SHORT */
/*      GL_UNSIGNED_SHORT */
/*      GL_INT */
/*      GL_UNSIGNED_INT */
/*      GL_FLOAT */
/*      GL_DOUBLE */

/* CullFaceMode */
/*      GL_FRONT */
/*      GL_BACK */
/*      GL_FRONT_AND_BACK */

/* DataType */
local const GL_BYTE =                           0x1400;
local const GL_UNSIGNED_BYTE =                  0x1401;
local const GL_SHORT =                          0x1402;
local const GL_UNSIGNED_SHORT =                 0x1403;
local const GL_INT =                            0x1404;
local const GL_UNSIGNED_INT =                   0x1405;
local const GL_FLOAT =                          0x1406;
local const GL_2_BYTES =                        0x1407;
local const GL_3_BYTES =                        0x1408;
local const GL_4_BYTES =                        0x1409;
local const GL_DOUBLE =                         0x140A;

/* DepthFunction */
/*      GL_NEVER */
/*      GL_LESS */
/*      GL_EQUAL */
/*      GL_LEQUAL */
/*      GL_GREATER */
/*      GL_NOTEQUAL */
/*      GL_GEQUAL */
/*      GL_ALWAYS */

/* DrawBufferMode */
local const GL_NONE =                           0;
local const GL_FRONT_LEFT =                     0x0400;
local const GL_FRONT_RIGHT =                    0x0401;
local const GL_BACK_LEFT =                      0x0402;
local const GL_BACK_RIGHT =                     0x0403;
local const GL_FRONT =                          0x0404;
local const GL_BACK =                           0x0405;
local const GL_LEFT =                           0x0406;
local const GL_RIGHT =                          0x0407;
local const GL_FRONT_AND_BACK =                 0x0408;
local const GL_AUX0 =                           0x0409;
local const GL_AUX1 =                           0x040A;
local const GL_AUX2 =                           0x040B;
local const GL_AUX3 =                           0x040C;

/* Enable */
/*      GL_FOG */
/*      GL_LIGHTING */
/*      GL_TEXTURE_1D */
/*      GL_TEXTURE_2D */
/*      GL_LINE_STIPPLE */
/*      GL_POLYGON_STIPPLE */
/*      GL_CULL_FACE */
/*      GL_ALPHA_TEST */
/*      GL_BLEND */
/*      GL_INDEX_LOGIC_OP */
/*      GL_COLOR_LOGIC_OP */
/*      GL_DITHER */
/*      GL_STENCIL_TEST */
/*      GL_DEPTH_TEST */
/*      GL_CLIP_PLANE0 */
/*      GL_CLIP_PLANE1 */
/*      GL_CLIP_PLANE2 */
/*      GL_CLIP_PLANE3 */
/*      GL_CLIP_PLANE4 */
/*      GL_CLIP_PLANE5 */
/*      GL_LIGHT0 */
/*      GL_LIGHT1 */
/*      GL_LIGHT2 */
/*      GL_LIGHT3 */
/*      GL_LIGHT4 */
/*      GL_LIGHT5 */
/*      GL_LIGHT6 */
/*      GL_LIGHT7 */
/*      GL_TEXTURE_GEN_S */
/*      GL_TEXTURE_GEN_T */
/*      GL_TEXTURE_GEN_R */
/*      GL_TEXTURE_GEN_Q */
/*      GL_MAP1_VERTEX_3 */
/*      GL_MAP1_VERTEX_4 */
/*      GL_MAP1_COLOR_4 */
/*      GL_MAP1_INDEX */
/*      GL_MAP1_NORMAL */
/*      GL_MAP1_TEXTURE_COORD_1 */
/*      GL_MAP1_TEXTURE_COORD_2 */
/*      GL_MAP1_TEXTURE_COORD_3 */
/*      GL_MAP1_TEXTURE_COORD_4 */
/*      GL_MAP2_VERTEX_3 */
/*      GL_MAP2_VERTEX_4 */
/*      GL_MAP2_COLOR_4 */
/*      GL_MAP2_INDEX */
/*      GL_MAP2_NORMAL */
/*      GL_MAP2_TEXTURE_COORD_1 */
/*      GL_MAP2_TEXTURE_COORD_2 */
/*      GL_MAP2_TEXTURE_COORD_3 */
/*      GL_MAP2_TEXTURE_COORD_4 */
/*      GL_POINT_SMOOTH */
/*      GL_LINE_SMOOTH */
/*      GL_POLYGON_SMOOTH */
/*      GL_SCISSOR_TEST */
/*      GL_COLOR_MATERIAL */
/*      GL_NORMALIZE */
/*      GL_AUTO_NORMAL */
/*      GL_VERTEX_ARRAY */
/*      GL_NORMAL_ARRAY */
/*      GL_COLOR_ARRAY */
/*      GL_INDEX_ARRAY */
/*      GL_TEXTURE_COORD_ARRAY */
/*      GL_EDGE_FLAG_ARRAY */
/*      GL_POLYGON_OFFSET_POINT */
/*      GL_POLYGON_OFFSET_LINE */
/*      GL_POLYGON_OFFSET_FILL */

/* ErrorCode */
local const GL_NO_ERROR =                       0;
local const GL_INVALID_ENUM =                   0x0500;
local const GL_INVALID_VALUE =                  0x0501;
local const GL_INVALID_OPERATION =              0x0502;
local const GL_STACK_OVERFLOW =                 0x0503;
local const GL_STACK_UNDERFLOW =                0x0504;
local const GL_OUT_OF_MEMORY =                  0x0505;

/* FeedBackMode */
local const GL_2D =                             0x0600;
local const GL_3D =                             0x0601;
local const GL_3D_COLOR =                       0x0602;
local const GL_3D_COLOR_TEXTURE =               0x0603;
local const GL_4D_COLOR_TEXTURE =               0x0604;

/* FeedBackToken */
local const GL_PASS_THROUGH_TOKEN =             0x0700;
local const GL_POINT_TOKEN =                    0x0701;
local const GL_LINE_TOKEN =                     0x0702;
local const GL_POLYGON_TOKEN =                  0x0703;
local const GL_BITMAP_TOKEN =                   0x0704;
local const GL_DRAW_PIXEL_TOKEN =               0x0705;
local const GL_COPY_PIXEL_TOKEN =               0x0706;
local const GL_LINE_RESET_TOKEN =               0x0707;

/* FogMode */
/*      GL_LINEAR */
local const GL_EXP =                            0x0800;
local const GL_EXP2 =                           0x0801;


/* FogParameter */
/*      GL_FOG_COLOR */
/*      GL_FOG_DENSITY */
/*      GL_FOG_END */
/*      GL_FOG_INDEX */
/*      GL_FOG_MODE */
/*      GL_FOG_START */

/* FrontFaceDirection */
local const GL_CW =                             0x0900;
local const GL_CCW =                            0x0901;

/* GetMapTarget */
local const GL_COEFF =                          0x0A00;
local const GL_ORDER =                          0x0A01;
local const GL_DOMAIN =                         0x0A02;

/* GetPixelMap */
/*      GL_PIXEL_MAP_I_TO_I */
/*      GL_PIXEL_MAP_S_TO_S */
/*      GL_PIXEL_MAP_I_TO_R */
/*      GL_PIXEL_MAP_I_TO_G */
/*      GL_PIXEL_MAP_I_TO_B */
/*      GL_PIXEL_MAP_I_TO_A */
/*      GL_PIXEL_MAP_R_TO_R */
/*      GL_PIXEL_MAP_G_TO_G */
/*      GL_PIXEL_MAP_B_TO_B */
/*      GL_PIXEL_MAP_A_TO_A */

/* GetPointerTarget */
/*      GL_VERTEX_ARRAY_POINTER */
/*      GL_NORMAL_ARRAY_POINTER */
/*      GL_COLOR_ARRAY_POINTER */
/*      GL_INDEX_ARRAY_POINTER */
/*      GL_TEXTURE_COORD_ARRAY_POINTER */
/*      GL_EDGE_FLAG_ARRAY_POINTER */

/* GetTarget */
local const GL_CURRENT_COLOR =                  0x0B00;
local const GL_CURRENT_INDEX =                  0x0B01;
local const GL_CURRENT_NORMAL =                 0x0B02;
local const GL_CURRENT_TEXTURE_COORDS =         0x0B03;
local const GL_CURRENT_RASTER_COLOR =           0x0B04;
local const GL_CURRENT_RASTER_INDEX =           0x0B05;
local const GL_CURRENT_RASTER_TEXTURE_COORDS =  0x0B06;
local const GL_CURRENT_RASTER_POSITION =        0x0B07;
local const GL_CURRENT_RASTER_POSITION_VALID =  0x0B08;
local const GL_CURRENT_RASTER_DISTANCE =        0x0B09;
local const GL_POINT_SMOOTH =                   0x0B10;
local const GL_POINT_SIZE =                     0x0B11;
local const GL_POINT_SIZE_RANGE =               0x0B12;
local const GL_POINT_SIZE_GRANULARITY =         0x0B13;
local const GL_LINE_SMOOTH =                    0x0B20;
local const GL_LINE_WIDTH =                     0x0B21;
local const GL_LINE_WIDTH_RANGE =               0x0B22;
local const GL_LINE_WIDTH_GRANULARITY =         0x0B23;
local const GL_LINE_STIPPLE =                   0x0B24;
local const GL_LINE_STIPPLE_PATTERN =           0x0B25;
local const GL_LINE_STIPPLE_REPEAT =            0x0B26;
local const GL_LIST_MODE =                      0x0B30;
local const GL_MAX_LIST_NESTING =               0x0B31;
local const GL_LIST_BASE =                      0x0B32;
local const GL_LIST_INDEX =                     0x0B33;
local const GL_POLYGON_MODE =                   0x0B40;
local const GL_POLYGON_SMOOTH =                 0x0B41;
local const GL_POLYGON_STIPPLE =                0x0B42;
local const GL_EDGE_FLAG =                      0x0B43;
local const GL_CULL_FACE =                      0x0B44;
local const GL_CULL_FACE_MODE =                 0x0B45;
local const GL_FRONT_FACE =                     0x0B46;
local const GL_LIGHTING =                       0x0B50;
local const GL_LIGHT_MODEL_LOCAL_VIEWER =       0x0B51;
local const GL_LIGHT_MODEL_TWO_SIDE =           0x0B52;
local const GL_LIGHT_MODEL_AMBIENT =            0x0B53;
local const GL_SHADE_MODEL =                    0x0B54;
local const GL_COLOR_MATERIAL_FACE =            0x0B55;
local const GL_COLOR_MATERIAL_PARAMETER =       0x0B56;
local const GL_COLOR_MATERIAL =                 0x0B57;
local const GL_FOG =                            0x0B60;
local const GL_FOG_INDEX =                      0x0B61;
local const GL_FOG_DENSITY =                    0x0B62;
local const GL_FOG_START =                      0x0B63;
local const GL_FOG_END =                        0x0B64;
local const GL_FOG_MODE =                       0x0B65;
local const GL_FOG_COLOR =                      0x0B66;
local const GL_DEPTH_RANGE =                    0x0B70;
local const GL_DEPTH_TEST =                     0x0B71;
local const GL_DEPTH_WRITEMASK =                0x0B72;
local const GL_DEPTH_CLEAR_VALUE =              0x0B73;
local const GL_DEPTH_FUNC =                     0x0B74;
local const GL_ACCUM_CLEAR_VALUE =              0x0B80;
local const GL_STENCIL_TEST =                   0x0B90;
local const GL_STENCIL_CLEAR_VALUE =            0x0B91;
local const GL_STENCIL_FUNC =                   0x0B92;
local const GL_STENCIL_VALUE_MASK =             0x0B93;
local const GL_STENCIL_FAIL =                   0x0B94;
local const GL_STENCIL_PASS_DEPTH_FAIL =        0x0B95;
local const GL_STENCIL_PASS_DEPTH_PASS =        0x0B96;
local const GL_STENCIL_REF =                    0x0B97;
local const GL_STENCIL_WRITEMASK =              0x0B98;
local const GL_MATRIX_MODE =                    0x0BA0;
local const GL_NORMALIZE =                      0x0BA1;
local const GL_VIEWPORT =                       0x0BA2;
local const GL_MODELVIEW_STACK_DEPTH =          0x0BA3;
local const GL_PROJECTION_STACK_DEPTH =         0x0BA4;
local const GL_TEXTURE_STACK_DEPTH =            0x0BA5;
local const GL_MODELVIEW_MATRIX =               0x0BA6;
local const GL_PROJECTION_MATRIX =              0x0BA7;
local const GL_TEXTURE_MATRIX =                 0x0BA8;
local const GL_ATTRIB_STACK_DEPTH =             0x0BB0;
local const GL_CLIENT_ATTRIB_STACK_DEPTH =      0x0BB1;
local const GL_ALPHA_TEST =                     0x0BC0;
local const GL_ALPHA_TEST_FUNC =                0x0BC1;
local const GL_ALPHA_TEST_REF =                 0x0BC2;
local const GL_DITHER =                         0x0BD0;
local const GL_BLEND_DST =                      0x0BE0;
local const GL_BLEND_SRC =                      0x0BE1;
local const GL_BLEND =                          0x0BE2;
local const GL_LOGIC_OP_MODE =                  0x0BF0;
local const GL_INDEX_LOGIC_OP =                 0x0BF1;
local const GL_COLOR_LOGIC_OP =                 0x0BF2;
local const GL_AUX_BUFFERS =                    0x0C00;
local const GL_DRAW_BUFFER =                    0x0C01;
local const GL_READ_BUFFER =                    0x0C02;
local const GL_SCISSOR_BOX =                    0x0C10;
local const GL_SCISSOR_TEST =                   0x0C11;
local const GL_INDEX_CLEAR_VALUE =              0x0C20;
local const GL_INDEX_WRITEMASK =                0x0C21;
local const GL_COLOR_CLEAR_VALUE =              0x0C22;
local const GL_COLOR_WRITEMASK =                0x0C23;
local const GL_INDEX_MODE =                     0x0C30;
local const GL_RGBA_MODE =                      0x0C31;
local const GL_DOUBLEBUFFER =                   0x0C32;
local const GL_STEREO =                         0x0C33;
local const GL_RENDER_MODE =                    0x0C40;
local const GL_PERSPECTIVE_CORRECTION_HINT =    0x0C50;
local const GL_POINT_SMOOTH_HINT =              0x0C51;
local const GL_LINE_SMOOTH_HINT =               0x0C52;
local const GL_POLYGON_SMOOTH_HINT =            0x0C53;
local const GL_FOG_HINT =                       0x0C54;
local const GL_TEXTURE_GEN_S =                  0x0C60;
local const GL_TEXTURE_GEN_T =                  0x0C61;
local const GL_TEXTURE_GEN_R =                  0x0C62;
local const GL_TEXTURE_GEN_Q =                  0x0C63;
local const GL_PIXEL_MAP_I_TO_I =               0x0C70;
local const GL_PIXEL_MAP_S_TO_S =               0x0C71;
local const GL_PIXEL_MAP_I_TO_R =               0x0C72;
local const GL_PIXEL_MAP_I_TO_G =               0x0C73;
local const GL_PIXEL_MAP_I_TO_B =               0x0C74;
local const GL_PIXEL_MAP_I_TO_A =               0x0C75;
local const GL_PIXEL_MAP_R_TO_R =               0x0C76;
local const GL_PIXEL_MAP_G_TO_G =               0x0C77;
local const GL_PIXEL_MAP_B_TO_B =               0x0C78;
local const GL_PIXEL_MAP_A_TO_A =               0x0C79;
local const GL_PIXEL_MAP_I_TO_I_SIZE =          0x0CB0;
local const GL_PIXEL_MAP_S_TO_S_SIZE =          0x0CB1;
local const GL_PIXEL_MAP_I_TO_R_SIZE =          0x0CB2;
local const GL_PIXEL_MAP_I_TO_G_SIZE =          0x0CB3;
local const GL_PIXEL_MAP_I_TO_B_SIZE =          0x0CB4;
local const GL_PIXEL_MAP_I_TO_A_SIZE =          0x0CB5;
local const GL_PIXEL_MAP_R_TO_R_SIZE =          0x0CB6;
local const GL_PIXEL_MAP_G_TO_G_SIZE =          0x0CB7;
local const GL_PIXEL_MAP_B_TO_B_SIZE =          0x0CB8;
local const GL_PIXEL_MAP_A_TO_A_SIZE =          0x0CB9;
local const GL_UNPACK_SWAP_BYTES =              0x0CF0;
local const GL_UNPACK_LSB_FIRST =               0x0CF1;
local const GL_UNPACK_ROW_LENGTH =              0x0CF2;
local const GL_UNPACK_SKIP_ROWS =               0x0CF3;
local const GL_UNPACK_SKIP_PIXELS =             0x0CF4;
local const GL_UNPACK_ALIGNMENT =               0x0CF5;
local const GL_PACK_SWAP_BYTES =                0x0D00;
local const GL_PACK_LSB_FIRST =                 0x0D01;
local const GL_PACK_ROW_LENGTH =                0x0D02;
local const GL_PACK_SKIP_ROWS =                 0x0D03;
local const GL_PACK_SKIP_PIXELS =               0x0D04;
local const GL_PACK_ALIGNMENT =                 0x0D05;
local const GL_MAP_COLOR =                      0x0D10;
local const GL_MAP_STENCIL =                    0x0D11;
local const GL_INDEX_SHIFT =                    0x0D12;
local const GL_INDEX_OFFSET =                   0x0D13;
local const GL_RED_SCALE =                      0x0D14;
local const GL_RED_BIAS =                       0x0D15;
local const GL_ZOOM_X =                         0x0D16;
local const GL_ZOOM_Y =                         0x0D17;
local const GL_GREEN_SCALE =                    0x0D18;
local const GL_GREEN_BIAS =                     0x0D19;
local const GL_BLUE_SCALE =                     0x0D1A;
local const GL_BLUE_BIAS =                      0x0D1B;
local const GL_ALPHA_SCALE =                    0x0D1C;
local const GL_ALPHA_BIAS =                     0x0D1D;
local const GL_DEPTH_SCALE =                    0x0D1E;
local const GL_DEPTH_BIAS =                     0x0D1F;
local const GL_MAX_EVAL_ORDER =                 0x0D30;
local const GL_MAX_LIGHTS =                     0x0D31;
local const GL_MAX_CLIP_PLANES =                0x0D32;
local const GL_MAX_TEXTURE_SIZE =               0x0D33;
local const GL_MAX_PIXEL_MAP_TABLE =            0x0D34;
local const GL_MAX_ATTRIB_STACK_DEPTH =         0x0D35;
local const GL_MAX_MODELVIEW_STACK_DEPTH =      0x0D36;
local const GL_MAX_NAME_STACK_DEPTH =           0x0D37;
local const GL_MAX_PROJECTION_STACK_DEPTH =     0x0D38;
local const GL_MAX_TEXTURE_STACK_DEPTH =        0x0D39;
local const GL_MAX_VIEWPORT_DIMS =              0x0D3A;
local const GL_MAX_CLIENT_ATTRIB_STACK_DEPTH =  0x0D3B;
local const GL_SUBPIXEL_BITS =                  0x0D50;
local const GL_INDEX_BITS =                     0x0D51;
local const GL_RED_BITS =                       0x0D52;
local const GL_GREEN_BITS =                     0x0D53;
local const GL_BLUE_BITS =                      0x0D54;
local const GL_ALPHA_BITS =                     0x0D55;
local const GL_DEPTH_BITS =                     0x0D56;
local const GL_STENCIL_BITS =                   0x0D57;
local const GL_ACCUM_RED_BITS =                 0x0D58;
local const GL_ACCUM_GREEN_BITS =               0x0D59;
local const GL_ACCUM_BLUE_BITS =                0x0D5A;
local const GL_ACCUM_ALPHA_BITS =               0x0D5B;
local const GL_NAME_STACK_DEPTH =               0x0D70;
local const GL_AUTO_NORMAL =                    0x0D80;
local const GL_MAP1_COLOR_4 =                   0x0D90;
local const GL_MAP1_INDEX =                     0x0D91;
local const GL_MAP1_NORMAL =                    0x0D92;
local const GL_MAP1_TEXTURE_COORD_1 =           0x0D93;
local const GL_MAP1_TEXTURE_COORD_2 =           0x0D94;
local const GL_MAP1_TEXTURE_COORD_3 =           0x0D95;
local const GL_MAP1_TEXTURE_COORD_4 =           0x0D96;
local const GL_MAP1_VERTEX_3 =                  0x0D97;
local const GL_MAP1_VERTEX_4 =                  0x0D98;
local const GL_MAP2_COLOR_4 =                   0x0DB0;
local const GL_MAP2_INDEX =                     0x0DB1;
local const GL_MAP2_NORMAL =                    0x0DB2;
local const GL_MAP2_TEXTURE_COORD_1 =           0x0DB3;
local const GL_MAP2_TEXTURE_COORD_2 =           0x0DB4;
local const GL_MAP2_TEXTURE_COORD_3 =           0x0DB5;
local const GL_MAP2_TEXTURE_COORD_4 =           0x0DB6;
local const GL_MAP2_VERTEX_3 =                  0x0DB7;
local const GL_MAP2_VERTEX_4 =                  0x0DB8;
local const GL_MAP1_GRID_DOMAIN =               0x0DD0;
local const GL_MAP1_GRID_SEGMENTS =             0x0DD1;
local const GL_MAP2_GRID_DOMAIN =               0x0DD2;
local const GL_MAP2_GRID_SEGMENTS =             0x0DD3;
local const GL_TEXTURE_1D =                     0x0DE0;
local const GL_TEXTURE_2D =                     0x0DE1;
local const GL_FEEDBACK_BUFFER_POINTER =        0x0DF0;
local const GL_FEEDBACK_BUFFER_SIZE =           0x0DF1;
local const GL_FEEDBACK_BUFFER_TYPE =           0x0DF2;
local const GL_SELECTION_BUFFER_POINTER =       0x0DF3;
local const GL_SELECTION_BUFFER_SIZE =          0x0DF4;
/*      GL_TEXTURE_BINDING_1D */
/*      GL_TEXTURE_BINDING_2D */
/*      GL_VERTEX_ARRAY */
/*      GL_NORMAL_ARRAY */
/*      GL_COLOR_ARRAY */
/*      GL_INDEX_ARRAY */
/*      GL_TEXTURE_COORD_ARRAY */
/*      GL_EDGE_FLAG_ARRAY */
/*      GL_VERTEX_ARRAY_SIZE */
/*      GL_VERTEX_ARRAY_TYPE */
/*      GL_VERTEX_ARRAY_STRIDE */
/*      GL_NORMAL_ARRAY_TYPE */
/*      GL_NORMAL_ARRAY_STRIDE */
/*      GL_COLOR_ARRAY_SIZE */
/*      GL_COLOR_ARRAY_TYPE */
/*      GL_COLOR_ARRAY_STRIDE */
/*      GL_INDEX_ARRAY_TYPE */
/*      GL_INDEX_ARRAY_STRIDE */
/*      GL_TEXTURE_COORD_ARRAY_SIZE */
/*      GL_TEXTURE_COORD_ARRAY_TYPE */
/*      GL_TEXTURE_COORD_ARRAY_STRIDE */
/*      GL_EDGE_FLAG_ARRAY_STRIDE */
/*      GL_POLYGON_OFFSET_FACTOR */
/*      GL_POLYGON_OFFSET_UNITS */

/* GetTextureParameter */
/*      GL_TEXTURE_MAG_FILTER */
/*      GL_TEXTURE_MIN_FILTER */
/*      GL_TEXTURE_WRAP_S */
/*      GL_TEXTURE_WRAP_T */
local const GL_TEXTURE_WIDTH =                  0x1000;
local const GL_TEXTURE_HEIGHT =                 0x1001;
local const GL_TEXTURE_INTERNAL_FORMAT =        0x1003;
local const GL_TEXTURE_BORDER_COLOR =           0x1004;
local const GL_TEXTURE_BORDER =                 0x1005;
/*      GL_TEXTURE_RED_SIZE */
/*      GL_TEXTURE_GREEN_SIZE */
/*      GL_TEXTURE_BLUE_SIZE */
/*      GL_TEXTURE_ALPHA_SIZE */
/*      GL_TEXTURE_LUMINANCE_SIZE */
/*      GL_TEXTURE_INTENSITY_SIZE */
/*      GL_TEXTURE_PRIORITY */
/*      GL_TEXTURE_RESIDENT */

/* HintMode */
local const GL_DONT_CARE =                      0x1100;
local const GL_FASTEST =                        0x1101;
local const GL_NICEST =                         0x1102;

/* HintTarget */
/*      GL_PERSPECTIVE_CORRECTION_HINT */
/*      GL_POINT_SMOOTH_HINT */
/*      GL_LINE_SMOOTH_HINT */
/*      GL_POLYGON_SMOOTH_HINT */
/*      GL_FOG_HINT */
/*      GL_PHONG_HINT */

/* IndexPointerType */
/*      GL_SHORT */
/*      GL_INT */
/*      GL_FLOAT */
/*      GL_DOUBLE */

/* LightModelParameter */
/*      GL_LIGHT_MODEL_AMBIENT */
/*      GL_LIGHT_MODEL_LOCAL_VIEWER */
/*      GL_LIGHT_MODEL_TWO_SIDE */

/* LightName */
local const GL_LIGHT0 =                         0x4000;
local const GL_LIGHT1 =                         0x4001;
local const GL_LIGHT2 =                         0x4002;
local const GL_LIGHT3 =                         0x4003;
local const GL_LIGHT4 =                         0x4004;
local const GL_LIGHT5 =                         0x4005;
local const GL_LIGHT6 =                         0x4006;
local const GL_LIGHT7 =                         0x4007;

/* LightParameter */
local const GL_AMBIENT =                        0x1200;
local const GL_DIFFUSE =                        0x1201;
local const GL_SPECULAR =                       0x1202;
local const GL_POSITION =                       0x1203;
local const GL_SPOT_DIRECTION =                 0x1204;
local const GL_SPOT_EXPONENT =                  0x1205;
local const GL_SPOT_CUTOFF =                    0x1206;
local const GL_CONSTANT_ATTENUATION =           0x1207;
local const GL_LINEAR_ATTENUATION =             0x1208;
local const GL_QUADRATIC_ATTENUATION =          0x1209;

/* InterleavedArrays */
/*      GL_V2F */
/*      GL_V3F */
/*      GL_C4UB_V2F */
/*      GL_C4UB_V3F */
/*      GL_C3F_V3F */
/*      GL_N3F_V3F */
/*      GL_C4F_N3F_V3F */
/*      GL_T2F_V3F */
/*      GL_T4F_V4F */
/*      GL_T2F_C4UB_V3F */
/*      GL_T2F_C3F_V3F */
/*      GL_T2F_N3F_V3F */
/*      GL_T2F_C4F_N3F_V3F */
/*      GL_T4F_C4F_N3F_V4F */

/* ListMode */
local const GL_COMPILE =                        0x1300;
local const GL_COMPILE_AND_EXECUTE =            0x1301;

/* ListNameType */
/*      GL_BYTE */
/*      GL_UNSIGNED_BYTE */
/*      GL_SHORT */
/*      GL_UNSIGNED_SHORT */
/*      GL_INT */
/*      GL_UNSIGNED_INT */
/*      GL_FLOAT */
/*      GL_2_BYTES */
/*      GL_3_BYTES */
/*      GL_4_BYTES */

/* LogicOp */
local const GL_CLEAR =                          0x1500;
local const GL_AND =                            0x1501;
local const GL_AND_REVERSE =                    0x1502;
local const GL_COPY =                           0x1503;
local const GL_AND_INVERTED =                   0x1504;
local const GL_NOOP =                           0x1505;
local const GL_XOR =                            0x1506;
local const GL_OR =                             0x1507;
local const GL_NOR =                            0x1508;
local const GL_EQUIV =                          0x1509;
local const GL_INVERT =                         0x150A;
local const GL_OR_REVERSE =                     0x150B;
local const GL_COPY_INVERTED =                  0x150C;
local const GL_OR_INVERTED =                    0x150D;
local const GL_NAND =                           0x150E;
local const GL_SET =                            0x150F;

/* MapTarget */
/*      GL_MAP1_COLOR_4 */
/*      GL_MAP1_INDEX */
/*      GL_MAP1_NORMAL */
/*      GL_MAP1_TEXTURE_COORD_1 */
/*      GL_MAP1_TEXTURE_COORD_2 */
/*      GL_MAP1_TEXTURE_COORD_3 */
/*      GL_MAP1_TEXTURE_COORD_4 */
/*      GL_MAP1_VERTEX_3 */
/*      GL_MAP1_VERTEX_4 */
/*      GL_MAP2_COLOR_4 */
/*      GL_MAP2_INDEX */
/*      GL_MAP2_NORMAL */
/*      GL_MAP2_TEXTURE_COORD_1 */
/*      GL_MAP2_TEXTURE_COORD_2 */
/*      GL_MAP2_TEXTURE_COORD_3 */
/*      GL_MAP2_TEXTURE_COORD_4 */
/*      GL_MAP2_VERTEX_3 */
/*      GL_MAP2_VERTEX_4 */

/* MaterialFace */
/*      GL_FRONT */
/*      GL_BACK */
/*      GL_FRONT_AND_BACK */

/* MaterialParameter */
local const GL_EMISSION =                       0x1600;
local const GL_SHININESS =                      0x1601;
local const GL_AMBIENT_AND_DIFFUSE =            0x1602;
local const GL_COLOR_INDEXES =                  0x1603;
/*      GL_AMBIENT */
/*      GL_DIFFUSE */
/*      GL_SPECULAR */

/* MatrixMode */
local const GL_MODELVIEW =                      0x1700;
local const GL_PROJECTION =                     0x1701;
local const GL_TEXTURE =                        0x1702;

/* MeshMode1 */
/*      GL_POINT */
/*      GL_LINE */

/* MeshMode2 */
/*      GL_POINT */
/*      GL_LINE */
/*      GL_FILL */

/* NormalPointerType */
/*      GL_BYTE */
/*      GL_SHORT */
/*      GL_INT */
/*      GL_FLOAT */
/*      GL_DOUBLE */

/* PixelCopyType */
local const GL_COLOR =                          0x1800;
local const GL_DEPTH =                          0x1801;
local const GL_STENCIL =                        0x1802;

/* PixelFormat */
local const GL_COLOR_INDEX =                    0x1900;
local const GL_STENCIL_INDEX =                  0x1901;
local const GL_DEPTH_COMPONENT =                0x1902;
local const GL_RED =                            0x1903;
local const GL_GREEN =                          0x1904;
local const GL_BLUE =                           0x1905;
local const GL_ALPHA =                          0x1906;
local const GL_RGB =                            0x1907;
local const GL_RGBA =                           0x1908;
local const GL_LUMINANCE =                      0x1909;
local const GL_LUMINANCE_ALPHA =                0x190A;

/* PixelMap */
/*      GL_PIXEL_MAP_I_TO_I */
/*      GL_PIXEL_MAP_S_TO_S */
/*      GL_PIXEL_MAP_I_TO_R */
/*      GL_PIXEL_MAP_I_TO_G */
/*      GL_PIXEL_MAP_I_TO_B */
/*      GL_PIXEL_MAP_I_TO_A */
/*      GL_PIXEL_MAP_R_TO_R */
/*      GL_PIXEL_MAP_G_TO_G */
/*      GL_PIXEL_MAP_B_TO_B */
/*      GL_PIXEL_MAP_A_TO_A */

/* PixelStore */
/*      GL_UNPACK_SWAP_BYTES */
/*      GL_UNPACK_LSB_FIRST */
/*      GL_UNPACK_ROW_LENGTH */
/*      GL_UNPACK_SKIP_ROWS */
/*      GL_UNPACK_SKIP_PIXELS */
/*      GL_UNPACK_ALIGNMENT */
/*      GL_PACK_SWAP_BYTES */
/*      GL_PACK_LSB_FIRST */
/*      GL_PACK_ROW_LENGTH */
/*      GL_PACK_SKIP_ROWS */
/*      GL_PACK_SKIP_PIXELS */
/*      GL_PACK_ALIGNMENT */

/* PixelTransfer */
/*      GL_MAP_COLOR */
/*      GL_MAP_STENCIL */
/*      GL_INDEX_SHIFT */
/*      GL_INDEX_OFFSET */
/*      GL_RED_SCALE */
/*      GL_RED_BIAS */
/*      GL_GREEN_SCALE */
/*      GL_GREEN_BIAS */
/*      GL_BLUE_SCALE */
/*      GL_BLUE_BIAS */
/*      GL_ALPHA_SCALE */
/*      GL_ALPHA_BIAS */
/*      GL_DEPTH_SCALE */
/*      GL_DEPTH_BIAS */

/* PixelType */
local const GL_BITMAP =                         0x1A00;
/*      GL_BYTE */
/*      GL_UNSIGNED_BYTE */
/*      GL_SHORT */
/*      GL_UNSIGNED_SHORT */
/*      GL_INT */
/*      GL_UNSIGNED_INT */
/*      GL_FLOAT */

/* PolygonMode */
local const GL_POINT =                          0x1B00;
local const GL_LINE =                           0x1B01;
local const GL_FILL =                           0x1B02;

/* ReadBufferMode */
/*      GL_FRONT_LEFT */
/*      GL_FRONT_RIGHT */
/*      GL_BACK_LEFT */
/*      GL_BACK_RIGHT */
/*      GL_FRONT */
/*      GL_BACK */
/*      GL_LEFT */
/*      GL_RIGHT */
/*      GL_AUX0 */
/*      GL_AUX1 */
/*      GL_AUX2 */
/*      GL_AUX3 */

/* RenderingMode */
local const GL_RENDER =                         0x1C00;
local const GL_FEEDBACK =                       0x1C01;
local const GL_SELECT =                         0x1C02;

/* ShadingModel */
local const GL_FLAT =                           0x1D00;
local const GL_SMOOTH =                         0x1D01;


/* StencilFunction */
/*      GL_NEVER */
/*      GL_LESS */
/*      GL_EQUAL */
/*      GL_LEQUAL */
/*      GL_GREATER */
/*      GL_NOTEQUAL */
/*      GL_GEQUAL */
/*      GL_ALWAYS */

/* StencilOp */
/*      GL_ZERO */
local const GL_KEEP =                           0x1E00;
local const GL_REPLACE =                        0x1E01;
local const GL_INCR =                           0x1E02;
local const GL_DECR =                           0x1E03;
/*      GL_INVERT */

/* StringName */
local const GL_VENDOR =                         0x1F00;
local const GL_RENDERER =                       0x1F01;
local const GL_VERSION =                        0x1F02;
local const GL_EXTENSIONS =                     0x1F03;

/* TextureCoordName */
local const GL_S =                              0x2000;
local const GL_T =                              0x2001;
local const GL_R =                              0x2002;
local const GL_Q =                              0x2003;

/* TexCoordPointerType */
/*      GL_SHORT */
/*      GL_INT */
/*      GL_FLOAT */
/*      GL_DOUBLE */

/* TextureEnvMode */
local const GL_MODULATE =                       0x2100;
local const GL_DECAL =                          0x2101;
/*      GL_BLEND */
/*      GL_REPLACE */

/* TextureEnvParameter */
local const GL_TEXTURE_ENV_MODE =               0x2200;
local const GL_TEXTURE_ENV_COLOR =              0x2201;

/* TextureEnvTarget */
local const GL_TEXTURE_ENV =                    0x2300;

/* TextureGenMode */
local const GL_EYE_LINEAR =                     0x2400;
local const GL_OBJECT_LINEAR =                  0x2401;
local const GL_SPHERE_MAP =                     0x2402;

/* TextureGenParameter */
local const GL_TEXTURE_GEN_MODE =               0x2500;
local const GL_OBJECT_PLANE =                   0x2501;
local const GL_EYE_PLANE =                      0x2502;

/* TextureMagFilter */
local const GL_NEAREST =                        0x2600;
local const GL_LINEAR =                         0x2601;

/* TextureMinFilter */
/*      GL_NEAREST */
/*      GL_LINEAR */
local const GL_NEAREST_MIPMAP_NEAREST =         0x2700;
local const GL_LINEAR_MIPMAP_NEAREST =          0x2701;
local const GL_NEAREST_MIPMAP_LINEAR =          0x2702;
local const GL_LINEAR_MIPMAP_LINEAR =           0x2703;

/* TextureParameterName */
local const GL_TEXTURE_MAG_FILTER =             0x2800;
local const GL_TEXTURE_MIN_FILTER =             0x2801;
local const GL_TEXTURE_WRAP_S =                 0x2802;
local const GL_TEXTURE_WRAP_T =                 0x2803;
/*      GL_TEXTURE_BORDER_COLOR */
/*      GL_TEXTURE_PRIORITY */

/* TextureTarget */
/*      GL_TEXTURE_1D */
/*      GL_TEXTURE_2D */
/*      GL_PROXY_TEXTURE_1D */
/*      GL_PROXY_TEXTURE_2D */

/* TextureWrapMode */
local const GL_CLAMP =                          0x2900;
local const GL_REPEAT =                         0x2901;

/* VertexPointerType */
/*      GL_SHORT */
/*      GL_INT */
/*      GL_FLOAT */
/*      GL_DOUBLE */

/* ClientAttribMask */
local const GL_CLIENT_PIXEL_STORE_BIT =         0x00000001;
local const GL_CLIENT_VERTEX_ARRAY_BIT =        0x00000002;
local const GL_CLIENT_ALL_ATTRIB_BITS =         0xffffffff;

/* polygon_offset */
local const GL_POLYGON_OFFSET_FACTOR =          0x8038;
local const GL_POLYGON_OFFSET_UNITS =           0x2A00;
local const GL_POLYGON_OFFSET_POINT =           0x2A01;
local const GL_POLYGON_OFFSET_LINE =            0x2A02;
local const GL_POLYGON_OFFSET_FILL =            0x8037;

/* texture */
local const GL_ALPHA4 =                         0x803B;
local const GL_ALPHA8 =                         0x803C;
local const GL_ALPHA12 =                        0x803D;
local const GL_ALPHA16 =                        0x803E;
local const GL_LUMINANCE4 =                     0x803F;
local const GL_LUMINANCE8 =                     0x8040;
local const GL_LUMINANCE12 =                    0x8041;
local const GL_LUMINANCE16 =                    0x8042;
local const GL_LUMINANCE4_ALPHA4 =              0x8043;
local const GL_LUMINANCE6_ALPHA2 =              0x8044;
local const GL_LUMINANCE8_ALPHA8 =              0x8045;
local const GL_LUMINANCE12_ALPHA4 =             0x8046;
local const GL_LUMINANCE12_ALPHA12 =            0x8047;
local const GL_LUMINANCE16_ALPHA16 =            0x8048;
local const GL_INTENSITY =                      0x8049;
local const GL_INTENSITY4 =                     0x804A;
local const GL_INTENSITY8 =                     0x804B;
local const GL_INTENSITY12 =                    0x804C;
local const GL_INTENSITY16 =                    0x804D;
local const GL_R3_G3_B2 =                       0x2A10;
local const GL_RGB4 =                           0x804F;
local const GL_RGB5 =                           0x8050;
local const GL_RGB8 =                           0x8051;
local const GL_RGB10 =                          0x8052;
local const GL_RGB12 =                          0x8053;
local const GL_RGB16 =                          0x8054;
local const GL_RGBA2 =                          0x8055;
local const GL_RGBA4 =                          0x8056;
local const GL_RGB5_A1 =                        0x8057;
local const GL_RGBA8 =                          0x8058;
local const GL_RGB10_A2 =                       0x8059;
local const GL_RGBA12 =                         0x805A;
local const GL_RGBA16 =                         0x805B;
local const GL_TEXTURE_RED_SIZE =               0x805C;
local const GL_TEXTURE_GREEN_SIZE =             0x805D;
local const GL_TEXTURE_BLUE_SIZE =              0x805E;
local const GL_TEXTURE_ALPHA_SIZE =             0x805F;
local const GL_TEXTURE_LUMINANCE_SIZE =         0x8060;
local const GL_TEXTURE_INTENSITY_SIZE =         0x8061;
local const GL_PROXY_TEXTURE_1D =               0x8063;
local const GL_PROXY_TEXTURE_2D =               0x8064;

/* texture_object */
local const GL_TEXTURE_PRIORITY =               0x8066;
local const GL_TEXTURE_RESIDENT =               0x8067;
local const GL_TEXTURE_BINDING_1D =             0x8068;
local const GL_TEXTURE_BINDING_2D =             0x8069;

/* vertex_array */
local const GL_VERTEX_ARRAY =                   0x8074;
local const GL_NORMAL_ARRAY =                   0x8075;
local const GL_COLOR_ARRAY =                    0x8076;
local const GL_INDEX_ARRAY =                    0x8077;
local const GL_TEXTURE_COORD_ARRAY =            0x8078;
local const GL_EDGE_FLAG_ARRAY =                0x8079;
local const GL_VERTEX_ARRAY_SIZE =              0x807A;
local const GL_VERTEX_ARRAY_TYPE =              0x807B;
local const GL_VERTEX_ARRAY_STRIDE =            0x807C;
local const GL_NORMAL_ARRAY_TYPE =              0x807E;
local const GL_NORMAL_ARRAY_STRIDE =            0x807F;
local const GL_COLOR_ARRAY_SIZE =               0x8081;
local const GL_COLOR_ARRAY_TYPE =               0x8082;
local const GL_COLOR_ARRAY_STRIDE =             0x8083;
local const GL_INDEX_ARRAY_TYPE =               0x8085;
local const GL_INDEX_ARRAY_STRIDE =             0x8086;
local const GL_TEXTURE_COORD_ARRAY_SIZE =       0x8088;
local const GL_TEXTURE_COORD_ARRAY_TYPE =       0x8089;
local const GL_TEXTURE_COORD_ARRAY_STRIDE =     0x808A;
local const GL_EDGE_FLAG_ARRAY_STRIDE =         0x808C;
local const GL_VERTEX_ARRAY_POINTER =           0x808E;
local const GL_NORMAL_ARRAY_POINTER =           0x808F;
local const GL_COLOR_ARRAY_POINTER =            0x8090;
local const GL_INDEX_ARRAY_POINTER =            0x8091;
local const GL_TEXTURE_COORD_ARRAY_POINTER =    0x8092;
local const GL_EDGE_FLAG_ARRAY_POINTER =        0x8093;
local const GL_V2F =                            0x2A20;
local const GL_V3F =                            0x2A21;
local const GL_C4UB_V2F =                       0x2A22;
local const GL_C4UB_V3F =                       0x2A23;
local const GL_C3F_V3F =                        0x2A24;
local const GL_N3F_V3F =                        0x2A25;
local const GL_C4F_N3F_V3F =                    0x2A26;
local const GL_T2F_V3F =                        0x2A27;
local const GL_T4F_V4F =                        0x2A28;
local const GL_T2F_C4UB_V3F =                   0x2A29;
local const GL_T2F_C3F_V3F =                    0x2A2A;
local const GL_T2F_N3F_V3F =                    0x2A2B;
local const GL_T2F_C4F_N3F_V3F =                0x2A2C;
local const GL_T4F_C4F_N3F_V4F =                0x2A2D;

/* Extensions */
local const GL_EXT_vertex_array =               1;
local const GL_EXT_bgra =                       1;
local const GL_EXT_paletted_texture =           1;
local const GL_WIN_swap_hint =                  1;
local const GL_WIN_draw_range_elements =        1;
// #define GL_WIN_phong_shading              1
// #define GL_WIN_specular_fog               1

/* EXT_vertex_array */
local const GL_VERTEX_ARRAY_EXT =               0x8074;
local const GL_NORMAL_ARRAY_EXT =               0x8075;
local const GL_COLOR_ARRAY_EXT =                0x8076;
local const GL_INDEX_ARRAY_EXT =                0x8077;
local const GL_TEXTURE_COORD_ARRAY_EXT =        0x8078;
local const GL_EDGE_FLAG_ARRAY_EXT =            0x8079;
local const GL_VERTEX_ARRAY_SIZE_EXT =          0x807A;
local const GL_VERTEX_ARRAY_TYPE_EXT =          0x807B;
local const GL_VERTEX_ARRAY_STRIDE_EXT =        0x807C;
local const GL_VERTEX_ARRAY_COUNT_EXT =         0x807D;
local const GL_NORMAL_ARRAY_TYPE_EXT =          0x807E;
local const GL_NORMAL_ARRAY_STRIDE_EXT =        0x807F;
local const GL_NORMAL_ARRAY_COUNT_EXT =         0x8080;
local const GL_COLOR_ARRAY_SIZE_EXT =           0x8081;
local const GL_COLOR_ARRAY_TYPE_EXT =           0x8082;
local const GL_COLOR_ARRAY_STRIDE_EXT =         0x8083;
local const GL_COLOR_ARRAY_COUNT_EXT =          0x8084;
local const GL_INDEX_ARRAY_TYPE_EXT =           0x8085;
local const GL_INDEX_ARRAY_STRIDE_EXT =         0x8086;
local const GL_INDEX_ARRAY_COUNT_EXT =          0x8087;
local const GL_TEXTURE_COORD_ARRAY_SIZE_EXT =   0x8088;
local const GL_TEXTURE_COORD_ARRAY_TYPE_EXT =   0x8089;
local const GL_TEXTURE_COORD_ARRAY_STRIDE_EXT = 0x808A;
local const GL_TEXTURE_COORD_ARRAY_COUNT_EXT =  0x808B;
local const GL_EDGE_FLAG_ARRAY_STRIDE_EXT =     0x808C;
local const GL_EDGE_FLAG_ARRAY_COUNT_EXT =      0x808D;
local const GL_VERTEX_ARRAY_POINTER_EXT =       0x808E;
local const GL_NORMAL_ARRAY_POINTER_EXT =       0x808F;
local const GL_COLOR_ARRAY_POINTER_EXT =        0x8090;
local const GL_INDEX_ARRAY_POINTER_EXT =        0x8091;
local const GL_TEXTURE_COORD_ARRAY_POINTER_EXT = 0x8092;
local const GL_EDGE_FLAG_ARRAY_POINTER_EXT =    0x8093;
local const GL_DOUBLE_EXT =                     GL_DOUBLE;

/* EXT_bgra */
local const GL_BGR_EXT =                        0x80E0;
local const GL_BGRA_EXT =                       0x80E1;

/* EXT_paletted_texture */

/* These must match the GL_COLOR_TABLE_*_SGI enumerants */
local const GL_COLOR_TABLE_FORMAT_EXT =         0x80D8;
local const GL_COLOR_TABLE_WIDTH_EXT =          0x80D9;
local const GL_COLOR_TABLE_RED_SIZE_EXT =       0x80DA;
local const GL_COLOR_TABLE_GREEN_SIZE_EXT =     0x80DB;
local const GL_COLOR_TABLE_BLUE_SIZE_EXT =      0x80DC;
local const GL_COLOR_TABLE_ALPHA_SIZE_EXT =     0x80DD;
local const GL_COLOR_TABLE_LUMINANCE_SIZE_EXT = 0x80DE;
local const GL_COLOR_TABLE_INTENSITY_SIZE_EXT = 0x80DF;

local const GL_COLOR_INDEX1_EXT =               0x80E2;
local const GL_COLOR_INDEX2_EXT =               0x80E3;
local const GL_COLOR_INDEX4_EXT =               0x80E4;
local const GL_COLOR_INDEX8_EXT =               0x80E5;
local const GL_COLOR_INDEX12_EXT =              0x80E6;
local const GL_COLOR_INDEX16_EXT =              0x80E7;

/* WIN_draw_range_elements */
local const GL_MAX_ELEMENTS_VERTICES_WIN =      0x80E8;
local const GL_MAX_ELEMENTS_INDICES_WIN =       0x80E9;

/* WIN_phong_shading */
local const GL_PHONG_WIN =                      0x80EA ;
local const GL_PHONG_HINT_WIN =                 0x80EB ;

/* WIN_specular_fog */
local const GL_FOG_SPECULAR_TEXTURE_WIN =       0x80EC;

/* For compatibility with OpenGL v1.0 */
local const GL_LOGIC_OP = GL_INDEX_LOGIC_OP;
local const GL_TEXTURE_COMPONENTS = GL_TEXTURE_INTERNAL_FORMAT;

/*************************************************************/

#define __GLAPI    extern [[__stdcall__]] "Opengl32.dll":
__GLAPI void glAccum(GLenum op, GLfloat value);
__GLAPI void glAlphaFunc(GLenum func, GLclampf ref);
__GLAPI GLboolean glAreTexturesResident(GLsizei n, local const GLuint *textures, GLboolean *residences);
__GLAPI void glArrayElement(GLint i);
__GLAPI void glBegin(GLenum mode);
__GLAPI void glBindTexture(GLenum target, GLuint texture);
__GLAPI void glBitmap(GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, local const GLubyte *bitmap);
__GLAPI void glBlendFunc(GLenum sfactor, GLenum dfactor);
__GLAPI void glCallList(GLuint list_);
__GLAPI void glCallLists(GLsizei n, GLenum type_, local const GLvoid *lists);
__GLAPI void glClear(GLbitfield mask);
__GLAPI void glClearAccum(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
__GLAPI void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
__GLAPI void glClearDepth(GLclampd depth);
__GLAPI void glClearIndex(GLfloat c);
__GLAPI void glClearStencil(GLint s);
__GLAPI void glClipPlane(GLenum plane, local const GLdouble *equation);
__GLAPI void glColor3b(GLbyte red, GLbyte green, GLbyte blue);
__GLAPI void glColor3bv(local const GLbyte *v);
__GLAPI void glColor3d(GLdouble red, GLdouble green, GLdouble blue);
__GLAPI void glColor3dv(local const GLdouble *v);
__GLAPI void glColor3f(GLfloat red, GLfloat green, GLfloat blue);
__GLAPI void glColor3fv(local const GLfloat *v);
__GLAPI void glColor3i(GLint red, GLint green, GLint blue);
__GLAPI void glColor3iv(local const GLint *v);
__GLAPI void glColor3s(GLshort red, GLshort green, GLshort blue);
__GLAPI void glColor3sv(local const GLshort *v);
__GLAPI void glColor3ub(GLubyte red, GLubyte green, GLubyte blue);
__GLAPI void glColor3ubv(local const GLubyte *v);
__GLAPI void glColor3ui(GLuint red, GLuint green, GLuint blue);
__GLAPI void glColor3uiv(local const GLuint *v);
__GLAPI void glColor3us(GLushort red, GLushort green, GLushort blue);
__GLAPI void glColor3usv(local const GLushort *v);
__GLAPI void glColor4b(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
__GLAPI void glColor4bv(local const GLbyte *v);
__GLAPI void glColor4d(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
__GLAPI void glColor4dv(local const GLdouble *v);
__GLAPI void glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
__GLAPI void glColor4fv(local const GLfloat *v);
__GLAPI void glColor4i(GLint red, GLint green, GLint blue, GLint alpha);
__GLAPI void glColor4iv(local const GLint *v);
__GLAPI void glColor4s(GLshort red, GLshort green, GLshort blue, GLshort alpha);
__GLAPI void glColor4sv(local const GLshort *v);
__GLAPI void glColor4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
__GLAPI void glColor4ubv(local const GLubyte *v);
__GLAPI void glColor4ui(GLuint red, GLuint green, GLuint blue, GLuint alpha);
__GLAPI void glColor4uiv(local const GLuint *v);
__GLAPI void glColor4us(GLushort red, GLushort green, GLushort blue, GLushort alpha);
__GLAPI void glColor4usv(local const GLushort *v);
__GLAPI void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
__GLAPI void glColorMaterial(GLenum face, GLenum mode);
__GLAPI void glColorPointer(GLint size, GLenum type_, GLsizei stride, local const GLvoid *pointer);
__GLAPI void glCopyPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type_);
__GLAPI void glCopyTexImage1D(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
__GLAPI void glCopyTexImage2D(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
__GLAPI void glCopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
__GLAPI void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
__GLAPI void glCullFace(GLenum mode);
__GLAPI void glDeleteLists(GLuint list_, GLsizei range);
__GLAPI void glDeleteTextures(GLsizei n, local const GLuint *textures);
__GLAPI void glDepthFunc(GLenum func);
__GLAPI void glDepthMask(GLboolean flag);
__GLAPI void glDepthRange(GLclampd zNear, GLclampd zFar);
__GLAPI void glDisable(GLenum cap);
__GLAPI void glDisableClientState(GLenum array);
__GLAPI void glDrawArrays(GLenum mode, GLint first, GLsizei count);
__GLAPI void glDrawBuffer(GLenum mode);
__GLAPI void glDrawElements(GLenum mode, GLsizei count, GLenum type_, local const GLvoid *indices);
__GLAPI void glDrawPixels(GLsizei width, GLsizei height, GLenum format, GLenum type_, local const GLvoid *pixels);
__GLAPI void glEdgeFlag(GLboolean flag);
__GLAPI void glEdgeFlagPointer(GLsizei stride, local const GLvoid *pointer);
__GLAPI void glEdgeFlagv(local const GLboolean *flag);
__GLAPI void glEnable(GLenum cap);
__GLAPI void glEnableClientState(GLenum array);
__GLAPI void glEnd(void);
__GLAPI void glEndList(void);
__GLAPI void glEvalCoord1d(GLdouble u);
__GLAPI void glEvalCoord1dv(local const GLdouble *u);
__GLAPI void glEvalCoord1f(GLfloat u);
__GLAPI void glEvalCoord1fv(local const GLfloat *u);
__GLAPI void glEvalCoord2d(GLdouble u, GLdouble v);
__GLAPI void glEvalCoord2dv(local const GLdouble *u);
__GLAPI void glEvalCoord2f(GLfloat u, GLfloat v);
__GLAPI void glEvalCoord2fv(local const GLfloat *u);
__GLAPI void glEvalMesh1(GLenum mode, GLint i1, GLint i2);
__GLAPI void glEvalMesh2(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
__GLAPI void glEvalPoint1(GLint i);
__GLAPI void glEvalPoint2(GLint i, GLint j);
__GLAPI void glFeedbackBuffer(GLsizei size, GLenum type_, GLfloat *buffer);
__GLAPI void glFinish(void);
__GLAPI void glFlush(void);
__GLAPI void glFogf(GLenum pname, GLfloat param);
__GLAPI void glFogfv(GLenum pname, local const GLfloat *params);
__GLAPI void glFogi(GLenum pname, GLint param);
__GLAPI void glFogiv(GLenum pname, local const GLint *params);
__GLAPI void glFrontFace(GLenum mode);
__GLAPI void glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
__GLAPI GLuint glGenLists(GLsizei range);
__GLAPI void glGenTextures(GLsizei n, GLuint *textures);
__GLAPI void glGetBooleanv(GLenum pname, GLboolean *params);
__GLAPI void glGetClipPlane(GLenum plane, GLdouble *equation);
__GLAPI void glGetDoublev(GLenum pname, GLdouble *params);
__GLAPI GLenum glGetError(void);
__GLAPI void glGetFloatv(GLenum pname, GLfloat *params);
__GLAPI void glGetIntegerv(GLenum pname, GLint *params);
__GLAPI void glGetLightfv(GLenum light, GLenum pname, GLfloat *params);
__GLAPI void glGetLightiv(GLenum light, GLenum pname, GLint *params);
__GLAPI void glGetMapdv(GLenum target, GLenum query, GLdouble *v);
__GLAPI void glGetMapfv(GLenum target, GLenum query, GLfloat *v);
__GLAPI void glGetMapiv(GLenum target, GLenum query, GLint *v);
__GLAPI void glGetMaterialfv(GLenum face, GLenum pname, GLfloat *params);
__GLAPI void glGetMaterialiv(GLenum face, GLenum pname, GLint *params);
__GLAPI void glGetPixelMapfv(GLenum map, GLfloat *values);
__GLAPI void glGetPixelMapuiv(GLenum map, GLuint *values);
__GLAPI void glGetPixelMapusv(GLenum map, GLushort *values);
__GLAPI void glGetPointerv(GLenum pname, GLvoid **params);
__GLAPI void glGetPolygonStipple(GLubyte *mask);
__GLAPI string glGetString(GLenum name);
__GLAPI void glGetTexEnvfv(GLenum target, GLenum pname, GLfloat *params);
__GLAPI void glGetTexEnviv(GLenum target, GLenum pname, GLint *params);
__GLAPI void glGetTexGendv(GLenum coord, GLenum pname, GLdouble *params);
__GLAPI void glGetTexGenfv(GLenum coord, GLenum pname, GLfloat *params);
__GLAPI void glGetTexGeniv(GLenum coord, GLenum pname, GLint *params);
__GLAPI void glGetTexImage(GLenum target, GLint level, GLenum format, GLenum type_, GLvoid *pixels);
__GLAPI void glGetTexLevelParameterfv(GLenum target, GLint level, GLenum pname, GLfloat *params);
__GLAPI void glGetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint *params);
__GLAPI void glGetTexParameterfv(GLenum target, GLenum pname, GLfloat *params);
__GLAPI void glGetTexParameteriv(GLenum target, GLenum pname, GLint *params);
__GLAPI void glHint(GLenum target, GLenum mode);
__GLAPI void glIndexMask(GLuint mask);
__GLAPI void glIndexPointer(GLenum type_, GLsizei stride, local const GLvoid *pointer);
__GLAPI void glIndexd(GLdouble c);
__GLAPI void glIndexdv(local const GLdouble *c);
__GLAPI void glIndexf(GLfloat c);
__GLAPI void glIndexfv(local const GLfloat *c);
__GLAPI void glIndexi(GLint c);
__GLAPI void glIndexiv(local const GLint *c);
__GLAPI void glIndexs(GLshort c);
__GLAPI void glIndexsv(local const GLshort *c);
__GLAPI void glIndexub(GLubyte c);
__GLAPI void glIndexubv(local const GLubyte *c);
__GLAPI void glInitNames(void);
__GLAPI void glInterleavedArrays(GLenum format, GLsizei stride, local const GLvoid *pointer);
__GLAPI GLboolean glIsEnabled(GLenum cap);
__GLAPI GLboolean glIsList(GLuint list_);
__GLAPI GLboolean glIsTexture(GLuint texture);
__GLAPI void glLightModelf(GLenum pname, GLfloat param);
__GLAPI void glLightModelfv(GLenum pname, local const GLfloat *params);
__GLAPI void glLightModeli(GLenum pname, GLint param);
__GLAPI void glLightModeliv(GLenum pname, local const GLint *params);
__GLAPI void glLightf(GLenum light, GLenum pname, GLfloat param);
__GLAPI void glLightfv(GLenum light, GLenum pname, local const GLfloat *params);
__GLAPI void glLighti(GLenum light, GLenum pname, GLint param);
__GLAPI void glLightiv(GLenum light, GLenum pname, local const GLint *params);
__GLAPI void glLineStipple(GLint factor, GLushort pattern);
__GLAPI void glLineWidth(GLfloat width);
__GLAPI void glListBase(GLuint base);
__GLAPI void glLoadIdentity(void);
__GLAPI void glLoadMatrixd(local const GLdouble *m);
__GLAPI void glLoadMatrixf(local const GLfloat *m);
__GLAPI void glLoadName(GLuint name);
__GLAPI void glLogicOp(GLenum opcode);
__GLAPI void glMap1d(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, local const GLdouble *points);
__GLAPI void glMap1f(GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, local const GLfloat *points);
__GLAPI void glMap2d(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, local const GLdouble *points);
__GLAPI void glMap2f(GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, local const GLfloat *points);
__GLAPI void glMapGrid1d(GLint un, GLdouble u1, GLdouble u2);
__GLAPI void glMapGrid1f(GLint un, GLfloat u1, GLfloat u2);
__GLAPI void glMapGrid2d(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
__GLAPI void glMapGrid2f(GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
__GLAPI void glMaterialf(GLenum face, GLenum pname, GLfloat param);
__GLAPI void glMaterialfv(GLenum face, GLenum pname, local const GLfloat *params);
__GLAPI void glMateriali(GLenum face, GLenum pname, GLint param);
__GLAPI void glMaterialiv(GLenum face, GLenum pname, local const GLint *params);
__GLAPI void glMatrixMode(GLenum mode);
__GLAPI void glMultMatrixd(local const GLdouble *m);
__GLAPI void glMultMatrixf(local const GLfloat *m);
__GLAPI void glNewList(GLuint list_, GLenum mode);
__GLAPI void glNormal3b(GLbyte nx, GLbyte ny, GLbyte nz);
__GLAPI void glNormal3bv(local const GLbyte *v);
__GLAPI void glNormal3d(GLdouble nx, GLdouble ny, GLdouble nz);
__GLAPI void glNormal3dv(local const GLdouble *v);
__GLAPI void glNormal3f(GLfloat nx, GLfloat ny, GLfloat nz);
__GLAPI void glNormal3fv(local const GLfloat *v);
__GLAPI void glNormal3i(GLint nx, GLint ny, GLint nz);
__GLAPI void glNormal3iv(local const GLint *v);
__GLAPI void glNormal3s(GLshort nx, GLshort ny, GLshort nz);
__GLAPI void glNormal3sv(local const GLshort *v);
__GLAPI void glNormalPointer(GLenum type_, GLsizei stride, local const GLvoid *pointer);
__GLAPI void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
__GLAPI void glPassThrough(GLfloat token);
__GLAPI void glPixelMapfv(GLenum map, GLsizei mapsize, local const GLfloat *values);
__GLAPI void glPixelMapuiv(GLenum map, GLsizei mapsize, local const GLuint *values);
__GLAPI void glPixelMapusv(GLenum map, GLsizei mapsize, local const GLushort *values);
__GLAPI void glPixelStoref(GLenum pname, GLfloat param);
__GLAPI void glPixelStorei(GLenum pname, GLint param);
__GLAPI void glPixelTransferf(GLenum pname, GLfloat param);
__GLAPI void glPixelTransferi(GLenum pname, GLint param);
__GLAPI void glPixelZoom(GLfloat xfactor, GLfloat yfactor);
__GLAPI void glPointSize(GLfloat size);
__GLAPI void glPolygonMode(GLenum face, GLenum mode);
__GLAPI void glPolygonOffset(GLfloat factor, GLfloat units);
__GLAPI void glPolygonStipple(local const GLubyte *mask);
__GLAPI void glPopAttrib(void);
__GLAPI void glPopClientAttrib(void);
__GLAPI void glPopMatrix(void);
__GLAPI void glPopName(void);
__GLAPI void glPrioritizeTextures(GLsizei n, local const GLuint *textures, local const GLclampf *priorities);
__GLAPI void glPushAttrib(GLbitfield mask);
__GLAPI void glPushClientAttrib(GLbitfield mask);
__GLAPI void glPushMatrix(void);
__GLAPI void glPushName(GLuint name);
__GLAPI void glRasterPos2d(GLdouble x, GLdouble y);
__GLAPI void glRasterPos2dv(local const GLdouble *v);
__GLAPI void glRasterPos2f(GLfloat x, GLfloat y);
__GLAPI void glRasterPos2fv(local const GLfloat *v);
__GLAPI void glRasterPos2i(GLint x, GLint y);
__GLAPI void glRasterPos2iv(local const GLint *v);
__GLAPI void glRasterPos2s(GLshort x, GLshort y);
__GLAPI void glRasterPos2sv(local const GLshort *v);
__GLAPI void glRasterPos3d(GLdouble x, GLdouble y, GLdouble z);
__GLAPI void glRasterPos3dv(local const GLdouble *v);
__GLAPI void glRasterPos3f(GLfloat x, GLfloat y, GLfloat z);
__GLAPI void glRasterPos3fv(local const GLfloat *v);
__GLAPI void glRasterPos3i(GLint x, GLint y, GLint z);
__GLAPI void glRasterPos3iv(local const GLint *v);
__GLAPI void glRasterPos3s(GLshort x, GLshort y, GLshort z);
__GLAPI void glRasterPos3sv(local const GLshort *v);
__GLAPI void glRasterPos4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
__GLAPI void glRasterPos4dv(local const GLdouble *v);
__GLAPI void glRasterPos4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
__GLAPI void glRasterPos4fv(local const GLfloat *v);
__GLAPI void glRasterPos4i(GLint x, GLint y, GLint z, GLint w);
__GLAPI void glRasterPos4iv(local const GLint *v);
__GLAPI void glRasterPos4s(GLshort x, GLshort y, GLshort z, GLshort w);
__GLAPI void glRasterPos4sv(local const GLshort *v);
__GLAPI void glReadBuffer(GLenum mode);
__GLAPI void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type_, GLvoid *pixels);
__GLAPI void glRectd(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
__GLAPI void glRectdv(local const GLdouble *v1, local const GLdouble *v2);
__GLAPI void glRectf(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
__GLAPI void glRectfv(local const GLfloat *v1, local const GLfloat *v2);
__GLAPI void glRecti(GLint x1, GLint y1, GLint x2, GLint y2);
__GLAPI void glRectiv(local const GLint *v1, local const GLint *v2);
__GLAPI void glRects(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
__GLAPI void glRectsv(local const GLshort *v1, local const GLshort *v2);
__GLAPI GLint glRenderMode(GLenum mode);
__GLAPI void glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
__GLAPI void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
__GLAPI void glScaled(GLdouble x, GLdouble y, GLdouble z);
__GLAPI void glScalef(GLfloat x, GLfloat y, GLfloat z);
__GLAPI void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
__GLAPI void glSelectBuffer(GLsizei size, GLuint *buffer);
__GLAPI void glShadeModel(GLenum mode);
__GLAPI void glStencilFunc(GLenum func, GLint ref, GLuint mask);
__GLAPI void glStencilMask(GLuint mask);
__GLAPI void glStencilOp(GLenum fail, GLenum zfail, GLenum zpass);
__GLAPI void glTexCoord1d(GLdouble s);
__GLAPI void glTexCoord1dv(local const GLdouble *v);
__GLAPI void glTexCoord1f(GLfloat s);
__GLAPI void glTexCoord1fv(local const GLfloat *v);
__GLAPI void glTexCoord1i(GLint s);
__GLAPI void glTexCoord1iv(local const GLint *v);
__GLAPI void glTexCoord1s(GLshort s);
__GLAPI void glTexCoord1sv(local const GLshort *v);
__GLAPI void glTexCoord2d(GLdouble s, GLdouble t);
__GLAPI void glTexCoord2dv(local const GLdouble *v);
__GLAPI void glTexCoord2f(GLfloat s, GLfloat t);
__GLAPI void glTexCoord2fv(local const GLfloat *v);
__GLAPI void glTexCoord2i(GLint s, GLint t);
__GLAPI void glTexCoord2iv(local const GLint *v);
__GLAPI void glTexCoord2s(GLshort s, GLshort t);
__GLAPI void glTexCoord2sv(local const GLshort *v);
__GLAPI void glTexCoord3d(GLdouble s, GLdouble t, GLdouble r);
__GLAPI void glTexCoord3dv(local const GLdouble *v);
__GLAPI void glTexCoord3f(GLfloat s, GLfloat t, GLfloat r);
__GLAPI void glTexCoord3fv(local const GLfloat *v);
__GLAPI void glTexCoord3i(GLint s, GLint t, GLint r);
__GLAPI void glTexCoord3iv(local const GLint *v);
__GLAPI void glTexCoord3s(GLshort s, GLshort t, GLshort r);
__GLAPI void glTexCoord3sv(local const GLshort *v);
__GLAPI void glTexCoord4d(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
__GLAPI void glTexCoord4dv(local const GLdouble *v);
__GLAPI void glTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
__GLAPI void glTexCoord4fv(local const GLfloat *v);
__GLAPI void glTexCoord4i(GLint s, GLint t, GLint r, GLint q);
__GLAPI void glTexCoord4iv(local const GLint *v);
__GLAPI void glTexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q);
__GLAPI void glTexCoord4sv(local const GLshort *v);
__GLAPI void glTexCoordPointer(GLint size, GLenum type_, GLsizei stride, local const GLvoid *pointer);
__GLAPI void glTexEnvf(GLenum target, GLenum pname, GLfloat param);
__GLAPI void glTexEnvfv(GLenum target, GLenum pname, local const GLfloat *params);
__GLAPI void glTexEnvi(GLenum target, GLenum pname, GLint param);
__GLAPI void glTexEnviv(GLenum target, GLenum pname, local const GLint *params);
__GLAPI void glTexGend(GLenum coord, GLenum pname, GLdouble param);
__GLAPI void glTexGendv(GLenum coord, GLenum pname, local const GLdouble *params);
__GLAPI void glTexGenf(GLenum coord, GLenum pname, GLfloat param);
__GLAPI void glTexGenfv(GLenum coord, GLenum pname, local const GLfloat *params);
__GLAPI void glTexGeni(GLenum coord, GLenum pname, GLint param);
__GLAPI void glTexGeniv(GLenum coord, GLenum pname, local const GLint *params);
__GLAPI void glTexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type_, local const GLvoid *pixels);
__GLAPI void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type_, local const GLvoid *pixels);
__GLAPI void glTexParameterf(GLenum target, GLenum pname, GLfloat param);
__GLAPI void glTexParameterfv(GLenum target, GLenum pname, local const GLfloat *params);
__GLAPI void glTexParameteri(GLenum target, GLenum pname, GLint param);
__GLAPI void glTexParameteriv(GLenum target, GLenum pname, local const GLint *params);
__GLAPI void glTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type_, local const GLvoid *pixels);
__GLAPI void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type_, local const GLvoid *pixels);
__GLAPI void glTranslated(GLdouble x, GLdouble y, GLdouble z);
__GLAPI void glTranslatef(GLfloat x, GLfloat y, GLfloat z);
__GLAPI void glVertex2d(GLdouble x, GLdouble y);
__GLAPI void glVertex2dv(local const GLdouble *v);
__GLAPI void glVertex2f(GLfloat x, GLfloat y);
__GLAPI void glVertex2fv(local const GLfloat *v);
__GLAPI void glVertex2i(GLint x, GLint y);
__GLAPI void glVertex2iv(local const GLint *v);
__GLAPI void glVertex2s(GLshort x, GLshort y);
__GLAPI void glVertex2sv(local const GLshort *v);
__GLAPI void glVertex3d(GLdouble x, GLdouble y, GLdouble z);
__GLAPI void glVertex3dv(local const GLdouble *v);
__GLAPI void glVertex3f(GLfloat x, GLfloat y, GLfloat z);
__GLAPI void glVertex3fv(local const GLfloat *v);
__GLAPI void glVertex3i(GLint x, GLint y, GLint z);
__GLAPI void glVertex3iv(local const GLint *v);
__GLAPI void glVertex3s(GLshort x, GLshort y, GLshort z);
__GLAPI void glVertex3sv(local const GLshort *v);
__GLAPI void glVertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
__GLAPI void glVertex4dv(local const GLdouble *v);
__GLAPI void glVertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
__GLAPI void glVertex4fv(local const GLfloat *v);
__GLAPI void glVertex4i(GLint x, GLint y, GLint z, GLint w);
__GLAPI void glVertex4iv(local const GLint *v);
__GLAPI void glVertex4s(GLshort x, GLshort y, GLshort z, GLshort w);
__GLAPI void glVertex4sv(local const GLshort *v);
__GLAPI void glVertexPointer(GLint size, GLenum type_, GLsizei stride, local const GLvoid *pointer);
__GLAPI void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
#undef __GLAPI

}

#endif /* !__DEEMON_GUARD_CXX_GL_GL_H */
