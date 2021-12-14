/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2021, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Open Design Alliance software pursuant to a license 
//   agreement with Open Design Alliance.
//   Open Design Alliance Copyright (C) 2002-2021 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////
#ifndef NOGL
#define NOGL

#include "TD_PackPush.h"

#ifndef GL_NO_ERROR
struct nogl 
{
  typedef unsigned int GLenum;
  typedef unsigned char GLboolean;
  typedef unsigned int GLbitfield;
  typedef signed char GLbyte;
  typedef short GLshort;
  typedef int GLint;
  typedef int GLsizei;
  typedef unsigned char GLubyte;
  typedef unsigned short GLushort;
  typedef unsigned int GLuint;
  typedef float GLfloat;
  typedef float GLclampf;
  typedef double GLdouble;
  typedef double GLclampd;
  typedef void GLvoid;
  typedef ptrdiff_t GLsizeiptr;
  typedef ptrdiff_t GLintptr;
};
enum OdTrRndNoGLConstants
{
    GL_TRUE                           = 1
  , GL_FALSE                          = 0

  , GL_NEVER                          = 0x0200
  , GL_LESS                           = 0x0201
  , GL_EQUAL                          = 0x0202
  , GL_LEQUAL                         = 0x0203
  , GL_GREATER                        = 0x0204
  , GL_NOTEQUAL                       = 0x0205
  , GL_GEQUAL                         = 0x0206
  , GL_ALWAYS                         = 0x0207
                                          
  , GL_KEEP                           = 0x1E00
  , GL_REPLACE                        = 0x1E01
  , GL_INCR                           = 0x1E02
  , GL_DECR                           = 0x1E03
  , GL_INCR_WRAP                      = 0x8507
  , GL_DECR_WRAP                      = 0x8508
                                          
  , GL_ZERO                           = 0
  , GL_ONE                            = 1
  , GL_SRC_COLOR                      = 0x0300
  , GL_ONE_MINUS_SRC_COLOR            = 0x0301
  , GL_SRC_ALPHA                      = 0x0302
  , GL_ONE_MINUS_SRC_ALPHA            = 0x0303
  , GL_DST_ALPHA                      = 0x0304
  , GL_ONE_MINUS_DST_ALPHA            = 0x0305
                                          
  , GL_DST_COLOR                      = 0x0306
  , GL_ONE_MINUS_DST_COLOR            = 0x0307
  , GL_SRC_ALPHA_SATURATE             = 0x0308
                                          
//, GL_BLEND_DST_RGB                  = 0x80C8
//, GL_BLEND_SRC_RGB                  = 0x80C9
//, GL_BLEND_DST_ALPHA                = 0x80CA
//, GL_BLEND_SRC_ALPHA                = 0x80CB
  , GL_DEPTH_COMPONENT16              = 0x81A5
  , GL_MIRRORED_REPEAT                = 0x8370
//, GL_INCR_WRAP                      = 0x8507
//, GL_DECR_WRAP                      = 0x8508
//, GL_GENERATE_MIPMAP_HINT           = 0x8192
  , GL_FUNC_ADD                       = 0x8006
  , GL_FUNC_SUBTRACT                  = 0x800A
  , GL_FUNC_REVERSE_SUBTRACT          = 0x800B
  , GL_CONSTANT_COLOR                 = 0x8001
  , GL_ONE_MINUS_CONSTANT_COLOR       = 0x8002
  , GL_CONSTANT_ALPHA                 = 0x8003
  , GL_ONE_MINUS_CONSTANT_ALPHA       = 0x8004
                                          
//, GL_CLEAR                          = 0x1500
//, GL_AND                            = 0x1501
//, GL_AND_REVERSE                    = 0x1502
//, GL_COPY                           = 0x1503
//, GL_AND_INVERTED                   = 0x1504
//, GL_NOOP                           = 0x1505
//, GL_XOR                            = 0x1506
//, GL_OR                             = 0x1507
//, GL_NOR                            = 0x1508
//, GL_EQUIV                          = 0x1509
  , GL_INVERT                         = 0x150A
//, GL_OR_REVERSE                     = 0x150B
//, GL_COPY_INVERTED                  = 0x150C
//, GL_OR_INVERTED                    = 0x150D
//, GL_NAND                           = 0x150E
//, GL_SET                            = 0x150F
                                          
//, GL_NONE                           0
//, GL_FRONT_LEFT                     = 0x0400
//, GL_FRONT_RIGHT                    = 0x0401
//, GL_BACK_LEFT                      = 0x0402
//, GL_BACK_RIGHT                     = 0x0403
  , GL_FRONT                          = 0x0404
  , GL_BACK                           = 0x0405
//, GL_LEFT                           = 0x0406
//, GL_RIGHT                          = 0x0407
  , GL_FRONT_AND_BACK                 = 0x0408
//, GL_AUX0                           = 0x0409
//, GL_AUX1                           = 0x040A
//, GL_AUX2                           = 0x040B
//, GL_AUX3                           = 0x040C
                                          
//, GL_CURRENT_BIT                    = 0x00000001
//, GL_POINT_BIT                      = 0x00000002
//, GL_LINE_BIT                       = 0x00000004
//, GL_POLYGON_BIT                    = 0x00000008
//, GL_POLYGON_STIPPLE_BIT            = 0x00000010
//, GL_PIXEL_MODE_BIT                 = 0x00000020
//, GL_LIGHTING_BIT                   = 0x00000040
//, GL_FOG_BIT                        = 0x00000080
  , GL_DEPTH_BUFFER_BIT               = 0x00000100
  , GL_ACCUM_BUFFER_BIT               = 0x00000200
  , GL_STENCIL_BUFFER_BIT             = 0x00000400
//, GL_VIEWPORT_BIT                   = 0x00000800
//, GL_TRANSFORM_BIT                  = 0x00001000
//, GL_ENABLE_BIT                     = 0x00002000
  , GL_COLOR_BUFFER_BIT               = 0x00004000
//, GL_HINT_BIT                       = 0x00008000
//, GL_EVAL_BIT                       = 0x00010000
//, GL_LIST_BIT                       = 0x00020000
//, GL_TEXTURE_BIT                    = 0x00040000
//, GL_SCISSOR_BIT                    = 0x00080000
//, GL_ALL_ATTRIB_BITS                = 0x000fffff
                                          
//, GL_POLYGON_OFFSET_FACTOR          = 0x8038
//, GL_POLYGON_OFFSET_UNITS           = 0x2A00
//, GL_POLYGON_OFFSET_POINT           = 0x2A01
//, GL_POLYGON_OFFSET_LINE            = 0x2A02
  , GL_POLYGON_OFFSET_FILL            = 0x8037
                                          
  , GL_TEXTURE0                       = 0x84C0
//, GL_TEXTURE1                       = 0x84C1
//, GL_TEXTURE2                       = 0x84C2
//, GL_TEXTURE3                       = 0x84C3
//, GL_TEXTURE4                       = 0x84C4
//, GL_TEXTURE5                       = 0x84C5
//, GL_TEXTURE6                       = 0x84C6
//, GL_TEXTURE7                       = 0x84C7
//, GL_TEXTURE8                       = 0x84C8
//, GL_TEXTURE9                       = 0x84C9
//, GL_TEXTURE10                      = 0x84CA
//, GL_TEXTURE11                      = 0x84CB
//, GL_TEXTURE12                      = 0x84CC
//, GL_TEXTURE13                      = 0x84CD
//, GL_TEXTURE14                      = 0x84CE
//, GL_TEXTURE15                      = 0x84CF
//, GL_TEXTURE16                      = 0x84D0
//, GL_TEXTURE17                      = 0x84D1
//, GL_TEXTURE18                      = 0x84D2
//, GL_TEXTURE19                      = 0x84D3
//, GL_TEXTURE20                      = 0x84D4
//, GL_TEXTURE21                      = 0x84D5
//, GL_TEXTURE22                      = 0x84D6
//, GL_TEXTURE23                      = 0x84D7
//, GL_TEXTURE24                      = 0x84D8
//, GL_TEXTURE25                      = 0x84D9
//, GL_TEXTURE26                      = 0x84DA
//, GL_TEXTURE27                      = 0x84DB
//, GL_TEXTURE28                      = 0x84DC
//, GL_TEXTURE29                      = 0x84DD
//, GL_TEXTURE30                      = 0x84DE
//, GL_TEXTURE31                      = 0x84DF
//, GL_ACTIVE_TEXTURE                 = 0x84E0
//, GL_SAMPLE_ALPHA_TO_COVERAGE       = 0x809E
//, GL_SAMPLE_COVERAGE                = 0x80A0
//, GL_SAMPLE_BUFFERS                 = 0x80A8
//, GL_SAMPLES                        = 0x80A9
//, GL_SAMPLE_COVERAGE_VALUE          = 0x80AA
//, GL_SAMPLE_COVERAGE_INVERT         = 0x80AB
  , GL_TEXTURE_CUBE_MAP               = 0x8513
//, GL_TEXTURE_BINDING_CUBE_MAP       = 0x8514
  , GL_TEXTURE_CUBE_MAP_POSITIVE_X    = 0x8515
//, GL_TEXTURE_CUBE_MAP_NEGATIVE_X    = 0x8516
//, GL_TEXTURE_CUBE_MAP_POSITIVE_Y    = 0x8517
//, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    = 0x8518
//, GL_TEXTURE_CUBE_MAP_POSITIVE_Z    = 0x8519
  , GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    = 0x851A
//, GL_MAX_CUBE_MAP_TEXTURE_SIZE      = 0x851C
//, GL_NUM_COMPRESSED_TEXTURE_FORMATS = 0x86A2
//, GL_COMPRESSED_TEXTURE_FORMATS     = 0x86A3
                                          
//, GL_CURRENT_COLOR                  = 0x0B00
//, GL_CURRENT_INDEX                  = 0x0B01
//, GL_CURRENT_NORMAL                 = 0x0B02
//, GL_CURRENT_TEXTURE_COORDS         = 0x0B03
//, GL_CURRENT_RASTER_COLOR           = 0x0B04
//, GL_CURRENT_RASTER_INDEX           = 0x0B05
//, GL_CURRENT_RASTER_TEXTURE_COORDS  = 0x0B06
//, GL_CURRENT_RASTER_POSITION        = 0x0B07
//, GL_CURRENT_RASTER_POSITION_VALID  = 0x0B08
//, GL_CURRENT_RASTER_DISTANCE        = 0x0B09
//, GL_POINT_SMOOTH                   = 0x0B10
//, GL_POINT_SIZE                     = 0x0B11
//, GL_POINT_SIZE_RANGE               = 0x0B12
//, GL_POINT_SIZE_GRANULARITY         = 0x0B13
//, GL_LINE_SMOOTH                    = 0x0B20
//, GL_LINE_WIDTH                     = 0x0B21
//, GL_LINE_WIDTH_RANGE               = 0x0B22
//, GL_LINE_WIDTH_GRANULARITY         = 0x0B23
//, GL_LINE_STIPPLE                   = 0x0B24
//, GL_LINE_STIPPLE_PATTERN           = 0x0B25
//, GL_LINE_STIPPLE_REPEAT            = 0x0B26
//, GL_LIST_MODE                      = 0x0B30
//, GL_MAX_LIST_NESTING               = 0x0B31
//, GL_LIST_BASE                      = 0x0B32
//, GL_LIST_INDEX                     = 0x0B33
//, GL_POLYGON_MODE                   = 0x0B40
//, GL_POLYGON_SMOOTH                 = 0x0B41
//, GL_POLYGON_STIPPLE                = 0x0B42
//, GL_EDGE_FLAG                      = 0x0B43
  , GL_CULL_FACE                      = 0x0B44
//, GL_CULL_FACE_MODE                 = 0x0B45
//, GL_FRONT_FACE                     = 0x0B46
//, GL_LIGHTING                       = 0x0B50
//, GL_LIGHT_MODEL_LOCAL_VIEWER       = 0x0B51
//, GL_LIGHT_MODEL_TWO_SIDE           = 0x0B52
//, GL_LIGHT_MODEL_AMBIENT            = 0x0B53
//, GL_SHADE_MODEL                    = 0x0B54
//, GL_COLOR_MATERIAL_FACE            = 0x0B55
//, GL_COLOR_MATERIAL_PARAMETER       = 0x0B56
//, GL_COLOR_MATERIAL                 = 0x0B57
//, GL_FOG                            = 0x0B60
//, GL_FOG_INDEX                      = 0x0B61
//, GL_FOG_DENSITY                    = 0x0B62
//, GL_FOG_START                      = 0x0B63
//, GL_FOG_END                        = 0x0B64
//, GL_FOG_MODE                       = 0x0B65
//, GL_FOG_COLOR                      = 0x0B66
//, GL_DEPTH_RANGE                    = 0x0B70
  , GL_DEPTH_TEST                     = 0x0B71
//, GL_DEPTH_WRITEMASK                = 0x0B72
//, GL_DEPTH_CLEAR_VALUE              = 0x0B73
//, GL_DEPTH_FUNC                     = 0x0B74
//, GL_ACCUM_CLEAR_VALUE              = 0x0B80
  , GL_STENCIL_TEST                   = 0x0B90
//, GL_STENCIL_CLEAR_VALUE            = 0x0B91
//, GL_STENCIL_FUNC                   = 0x0B92
//, GL_STENCIL_VALUE_MASK             = 0x0B93
//, GL_STENCIL_FAIL                   = 0x0B94
//, GL_STENCIL_PASS_DEPTH_FAIL        = 0x0B95
//, GL_STENCIL_PASS_DEPTH_PASS        = 0x0B96
//, GL_STENCIL_REF                    = 0x0B97
//, GL_STENCIL_WRITEMASK              = 0x0B98
//, GL_MATRIX_MODE                    = 0x0BA0
//, GL_NORMALIZE                      = 0x0BA1
//, GL_VIEWPORT                       = 0x0BA2
//, GL_MODELVIEW_STACK_DEPTH          = 0x0BA3
//, GL_PROJECTION_STACK_DEPTH         = 0x0BA4
//, GL_TEXTURE_STACK_DEPTH            = 0x0BA5
//, GL_MODELVIEW_MATRIX               = 0x0BA6
//, GL_PROJECTION_MATRIX              = 0x0BA7
//, GL_TEXTURE_MATRIX                 = 0x0BA8
//, GL_ATTRIB_STACK_DEPTH             = 0x0BB0
//, GL_CLIENT_ATTRIB_STACK_DEPTH      = 0x0BB1
//, GL_ALPHA_TEST                     = 0x0BC0
//, GL_ALPHA_TEST_FUNC                = 0x0BC1
//, GL_ALPHA_TEST_REF                 = 0x0BC2
//, GL_DITHER                         = 0x0BD0
//, GL_BLEND_DST                      = 0x0BE0
//, GL_BLEND_SRC                      = 0x0BE1
  , GL_BLEND                          = 0x0BE2
//, GL_LOGIC_OP_MODE                  = 0x0BF0
//, GL_INDEX_LOGIC_OP                 = 0x0BF1
//, GL_COLOR_LOGIC_OP                 = 0x0BF2
//, GL_AUX_BUFFERS                    = 0x0C00
//, GL_DRAW_BUFFER                    = 0x0C01
//, GL_READ_BUFFER                    = 0x0C02
//, GL_SCISSOR_BOX                    = 0x0C10
  , GL_SCISSOR_TEST                   = 0x0C11
//, GL_INDEX_CLEAR_VALUE              = 0x0C20
//, GL_INDEX_WRITEMASK                = 0x0C21
//, GL_COLOR_CLEAR_VALUE              = 0x0C22
//, GL_COLOR_WRITEMASK                = 0x0C23
//, GL_INDEX_MODE                     = 0x0C30
//, GL_RGBA_MODE                      = 0x0C31
//, GL_DOUBLEBUFFER                   = 0x0C32
//, GL_STEREO                         = 0x0C33
//, GL_RENDER_MODE                    = 0x0C40
//, GL_PERSPECTIVE_CORRECTION_HINT    = 0x0C50
//, GL_POINT_SMOOTH_HINT              = 0x0C51
//, GL_LINE_SMOOTH_HINT               = 0x0C52
//, GL_POLYGON_SMOOTH_HINT            = 0x0C53
//, GL_FOG_HINT                       = 0x0C54
//, GL_TEXTURE_GEN_S                  = 0x0C60
//, GL_TEXTURE_GEN_T                  = 0x0C61
//, GL_TEXTURE_GEN_R                  = 0x0C62
//, GL_TEXTURE_GEN_Q                  = 0x0C63
//, GL_PIXEL_MAP_I_TO_I               = 0x0C70
//, GL_PIXEL_MAP_S_TO_S               = 0x0C71
//, GL_PIXEL_MAP_I_TO_R               = 0x0C72
//, GL_PIXEL_MAP_I_TO_G               = 0x0C73
//, GL_PIXEL_MAP_I_TO_B               = 0x0C74
//, GL_PIXEL_MAP_I_TO_A               = 0x0C75
//, GL_PIXEL_MAP_R_TO_R               = 0x0C76
//, GL_PIXEL_MAP_G_TO_G               = 0x0C77
//, GL_PIXEL_MAP_B_TO_B               = 0x0C78
//, GL_PIXEL_MAP_A_TO_A               = 0x0C79
//, GL_PIXEL_MAP_I_TO_I_SIZE          = 0x0CB0
//, GL_PIXEL_MAP_S_TO_S_SIZE          = 0x0CB1
//, GL_PIXEL_MAP_I_TO_R_SIZE          = 0x0CB2
//, GL_PIXEL_MAP_I_TO_G_SIZE          = 0x0CB3
//, GL_PIXEL_MAP_I_TO_B_SIZE          = 0x0CB4
//, GL_PIXEL_MAP_I_TO_A_SIZE          = 0x0CB5
//, GL_PIXEL_MAP_R_TO_R_SIZE          = 0x0CB6
//, GL_PIXEL_MAP_G_TO_G_SIZE          = 0x0CB7
//, GL_PIXEL_MAP_B_TO_B_SIZE          = 0x0CB8
//, GL_PIXEL_MAP_A_TO_A_SIZE          = 0x0CB9
//, GL_UNPACK_SWAP_BYTES              = 0x0CF0
//, GL_UNPACK_LSB_FIRST               = 0x0CF1
//, GL_UNPACK_ROW_LENGTH              = 0x0CF2
//, GL_UNPACK_SKIP_ROWS               = 0x0CF3
//, GL_UNPACK_SKIP_PIXELS             = 0x0CF4
  , GL_UNPACK_ALIGNMENT               = 0x0CF5
//, GL_PACK_SWAP_BYTES                = 0x0D00
//, GL_PACK_LSB_FIRST                 = 0x0D01
//, GL_PACK_ROW_LENGTH                = 0x0D02
//, GL_PACK_SKIP_ROWS                 = 0x0D03
//, GL_PACK_SKIP_PIXELS               = 0x0D04
  , GL_PACK_ALIGNMENT                 = 0x0D05
//, GL_MAP_COLOR                      = 0x0D10
//, GL_MAP_STENCIL                    = 0x0D11
//, GL_INDEX_SHIFT                    = 0x0D12
//, GL_INDEX_OFFSET                   = 0x0D13
//, GL_RED_SCALE                      = 0x0D14
//, GL_RED_BIAS                       = 0x0D15
//, GL_ZOOM_X                         = 0x0D16
//, GL_ZOOM_Y                         = 0x0D17
//, GL_GREEN_SCALE                    = 0x0D18
//, GL_GREEN_BIAS                     = 0x0D19
//, GL_BLUE_SCALE                     = 0x0D1A
//, GL_BLUE_BIAS                      = 0x0D1B
//, GL_ALPHA_SCALE                    = 0x0D1C
//, GL_ALPHA_BIAS                     = 0x0D1D
//, GL_DEPTH_SCALE                    = 0x0D1E
//, GL_DEPTH_BIAS                     = 0x0D1F
//, GL_MAX_EVAL_ORDER                 = 0x0D30
//, GL_MAX_LIGHTS                     = 0x0D31
//, GL_MAX_CLIP_PLANES                = 0x0D32
  , GL_MAX_TEXTURE_SIZE               = 0x0D33
//, GL_MAX_PIXEL_MAP_TABLE            = 0x0D34
//, GL_MAX_ATTRIB_STACK_DEPTH         = 0x0D35
//, GL_MAX_MODELVIEW_STACK_DEPTH      = 0x0D36
//, GL_MAX_NAME_STACK_DEPTH           = 0x0D37
//, GL_MAX_PROJECTION_STACK_DEPTH     = 0x0D38
//, GL_MAX_TEXTURE_STACK_DEPTH        = 0x0D39
//, GL_MAX_VIEWPORT_DIMS              = 0x0D3A
//, GL_MAX_CLIENT_ATTRIB_STACK_DEPTH  = 0x0D3B
//, GL_SUBPIXEL_BITS                  = 0x0D50
//, GL_INDEX_BITS                     = 0x0D51
//, GL_RED_BITS                       = 0x0D52
//, GL_GREEN_BITS                     = 0x0D53
//, GL_BLUE_BITS                      = 0x0D54
//, GL_ALPHA_BITS                     = 0x0D55
//, GL_DEPTH_BITS                     = 0x0D56
//, GL_STENCIL_BITS                   = 0x0D57
//, GL_ACCUM_RED_BITS                 = 0x0D58
//, GL_ACCUM_GREEN_BITS               = 0x0D59
//, GL_ACCUM_BLUE_BITS                = 0x0D5A
//, GL_ACCUM_ALPHA_BITS               = 0x0D5B
//, GL_NAME_STACK_DEPTH               = 0x0D70
//, GL_AUTO_NORMAL                    = 0x0D80
//, GL_MAP1_COLOR_4                   = 0x0D90
//, GL_MAP1_INDEX                     = 0x0D91
//, GL_MAP1_NORMAL                    = 0x0D92
//, GL_MAP1_TEXTURE_COORD_1           = 0x0D93
//, GL_MAP1_TEXTURE_COORD_2           = 0x0D94
//, GL_MAP1_TEXTURE_COORD_3           = 0x0D95
//, GL_MAP1_TEXTURE_COORD_4           = 0x0D96
//, GL_MAP1_VERTEX_3                  = 0x0D97
//, GL_MAP1_VERTEX_4                  = 0x0D98
//, GL_MAP2_COLOR_4                   = 0x0DB0
//, GL_MAP2_INDEX                     = 0x0DB1
//, GL_MAP2_NORMAL                    = 0x0DB2
//, GL_MAP2_TEXTURE_COORD_1           = 0x0DB3
//, GL_MAP2_TEXTURE_COORD_2           = 0x0DB4
//, GL_MAP2_TEXTURE_COORD_3           = 0x0DB5
//, GL_MAP2_TEXTURE_COORD_4           = 0x0DB6
//, GL_MAP2_VERTEX_3                  = 0x0DB7
//, GL_MAP2_VERTEX_4                  = 0x0DB8
//, GL_MAP1_GRID_DOMAIN               = 0x0DD0
//, GL_MAP1_GRID_SEGMENTS             = 0x0DD1
//, GL_MAP2_GRID_DOMAIN               = 0x0DD2
//, GL_MAP2_GRID_SEGMENTS             = 0x0DD3
//, GL_TEXTURE_1D                     = 0x0DE0
  , GL_TEXTURE_2D                     = 0x0DE1
//, GL_FEEDBACK_BUFFER_POINTER        = 0x0DF0
//, GL_FEEDBACK_BUFFER_SIZE           = 0x0DF1
//, GL_FEEDBACK_BUFFER_TYPE           = 0x0DF2
//, GL_SELECTION_BUFFER_POINTER       = 0x0DF3
//, GL_SELECTION_BUFFER_SIZE          = 0x0DF4
                                          
  , GL_NO_ERROR                       = 0
//, GL_INVALID_ENUM                   = 0x0500
//, GL_INVALID_VALUE                  = 0x0501
//, GL_INVALID_OPERATION              = 0x0502
//, GL_STACK_OVERFLOW                 = 0x0503
//, GL_STACK_UNDERFLOW                = 0x0504
//, GL_OUT_OF_MEMORY                  = 0x0505
                                          
  , GL_BYTE                           = 0x1400
  , GL_UNSIGNED_BYTE                  = 0x1401
  , GL_SHORT                          = 0x1402
  , GL_UNSIGNED_SHORT                 = 0x1403
  , GL_INT                            = 0x1404
  , GL_UNSIGNED_INT                   = 0x1405
  , GL_FLOAT                          = 0x1406
//, GL_2_BYTES                        = 0x1407
//, GL_3_BYTES                        = 0x1408
//, GL_4_BYTES                        = 0x1409
//, GL_DOUBLE                         = 0x140A
                                          
//, GL_COLOR_INDEX                    = 0x1900
//, GL_STENCIL_INDEX                  = 0x1901
//, GL_DEPTH_COMPONENT                = 0x1902
  , GL_RED                            = 0x1903
  , GL_GREEN                          = 0x1904
  , GL_BLUE                           = 0x1905
  , GL_ALPHA                          = 0x1906
  , GL_RGB                            = 0x1907
  , GL_RGBA                           = 0x1908
  , GL_LUMINANCE                      = 0x1909
//, GL_LUMINANCE_ALPHA                = 0x190A
                                          
  , GL_TEXTURE_MAG_FILTER             = 0x2800
  , GL_TEXTURE_MIN_FILTER             = 0x2801
  , GL_TEXTURE_WRAP_S                 = 0x2802
  , GL_TEXTURE_WRAP_T                 = 0x2803
                                          
  , GL_NEAREST                        = 0x2600
  , GL_LINEAR                         = 0x2601
                                          
//, GL_UNSIGNED_SHORT_4_4_4_4         = 0x8033
//, GL_UNSIGNED_SHORT_5_5_5_1         = 0x8034
//, GL_UNSIGNED_SHORT_5_6_5           = 0x8363
  , GL_CLAMP_TO_EDGE                  = 0x812F
//, GL_ALIASED_LINE_WIDTH_RANGE       = 0x846E
  , GL_ALIASED_POINT_SIZE_RANGE       = 0x846D
                                          
//, GL_FIXED                          = 0x140C
//, GL_IMPLEMENTATION_COLOR_READ_TYPE = 0x8B9A
//, GL_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B
//, GL_LOW_FLOAT                      = 0x8DF0
//, GL_MEDIUM_FLOAT                   = 0x8DF1
//, GL_HIGH_FLOAT                     = 0x8DF2
//, GL_LOW_INT                        = 0x8DF3
//, GL_MEDIUM_INT                     = 0x8DF4
//, GL_HIGH_INT                       = 0x8DF5
//, GL_SHADER_COMPILER                = 0x8DFA
//, GL_SHADER_BINARY_FORMATS          = 0x8DF8
//, GL_NUM_SHADER_BINARY_FORMATS      = 0x8DF9
  , GL_MAX_VERTEX_UNIFORM_VECTORS     = 0x8DFB
//, GL_MAX_VARYING_VECTORS            = 0x8DFC
  , GL_MAX_FRAGMENT_UNIFORM_VECTORS   = 0x8DFD
//, GL_RGB565                         = 0x8D62
                                          
//, GL_INVALID_FRAMEBUFFER_OPERATION  = 0x0506
//, GL_MAX_RENDERBUFFER_SIZE          = 0x84E8
//, GL_FRAMEBUFFER_BINDING            = 0x8CA6
//, GL_RENDERBUFFER_BINDING           = 0x8CA7
//, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
//, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
//, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
//, GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
  , GL_FRAMEBUFFER_COMPLETE           = 0x8CD5
//, GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
//, GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
//, GL_FRAMEBUFFER_UNSUPPORTED        = 0x8CDD
  , GL_COLOR_ATTACHMENT0              = 0x8CE0
  , GL_DEPTH_ATTACHMENT               = 0x8D00
  , GL_STENCIL_ATTACHMENT             = 0x8D20
  , GL_FRAMEBUFFER                    = 0x8D40
  , GL_RENDERBUFFER                   = 0x8D41
//, GL_RENDERBUFFER_WIDTH             = 0x8D42
//, GL_RENDERBUFFER_HEIGHT            = 0x8D43
//, GL_RENDERBUFFER_INTERNAL_FORMAT   = 0x8D44
//, GL_STENCIL_INDEX8                 = 0x8D48
//, GL_RENDERBUFFER_RED_SIZE          = 0x8D50
//, GL_RENDERBUFFER_GREEN_SIZE        = 0x8D51
//, GL_RENDERBUFFER_BLUE_SIZE         = 0x8D52
//, GL_RENDERBUFFER_ALPHA_SIZE        = 0x8D53
//, GL_RENDERBUFFER_DEPTH_SIZE        = 0x8D54
//, GL_RENDERBUFFER_STENCIL_SIZE      = 0x8D55
                                          
//, GL_BLEND_EQUATION_RGB             = 0x8009
//, GL_VERTEX_ATTRIB_ARRAY_ENABLED    = 0x8622
//, GL_VERTEX_ATTRIB_ARRAY_SIZE       = 0x8623
//, GL_VERTEX_ATTRIB_ARRAY_STRIDE     = 0x8624
//, GL_VERTEX_ATTRIB_ARRAY_TYPE       = 0x8625
//, GL_CURRENT_VERTEX_ATTRIB          = 0x8626
//, GL_VERTEX_ATTRIB_ARRAY_POINTER    = 0x8645
//, GL_STENCIL_BACK_FUNC              = 0x8800
//, GL_STENCIL_BACK_FAIL              = 0x8801
//, GL_STENCIL_BACK_PASS_DEPTH_FAIL   = 0x8802
//, GL_STENCIL_BACK_PASS_DEPTH_PASS   = 0x8803
//, GL_BLEND_EQUATION_ALPHA           = 0x883D
//, GL_MAX_VERTEX_ATTRIBS             = 0x8869
//, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED = 0x886A
  , GL_MAX_TEXTURE_IMAGE_UNITS        = 0x8872
//, GL_FRAGMENT_SHADER                = 0x8B30
//, GL_VERTEX_SHADER                  = 0x8B31
  , GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS = 0x8B4C
  , GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS=0x8B4D
//, GL_SHADER_TYPE                    = 0x8B4F
//, GL_FLOAT_VEC2                     = 0x8B50
//, GL_FLOAT_VEC3                     = 0x8B51
//, GL_FLOAT_VEC4                     = 0x8B52
//, GL_INT_VEC2                       = 0x8B53
//, GL_INT_VEC3                       = 0x8B54
//, GL_INT_VEC4                       = 0x8B55
//, GL_BOOL                           = 0x8B56
//, GL_BOOL_VEC2                      = 0x8B57
//, GL_BOOL_VEC3                      = 0x8B58
//, GL_BOOL_VEC4                      = 0x8B59
//, GL_FLOAT_MAT2                     = 0x8B5A
//, GL_FLOAT_MAT3                     = 0x8B5B
//, GL_FLOAT_MAT4                     = 0x8B5C
//, GL_SAMPLER_2D                     = 0x8B5E
//, GL_SAMPLER_CUBE                   = 0x8B60
//, GL_DELETE_STATUS                  = 0x8B80
//, GL_COMPILE_STATUS                 = 0x8B81
//, GL_LINK_STATUS                    = 0x8B82
//, GL_VALIDATE_STATUS                = 0x8B83
//, GL_INFO_LOG_LENGTH                = 0x8B84
//, GL_ATTACHED_SHADERS               = 0x8B85
//, GL_ACTIVE_UNIFORMS                = 0x8B86
//, GL_ACTIVE_UNIFORM_MAX_LENGTH      = 0x8B87
//, GL_SHADER_SOURCE_LENGTH           = 0x8B88
//, GL_ACTIVE_ATTRIBUTES              = 0x8B89
//, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH    = 0x8B8A
//, GL_SHADING_LANGUAGE_VERSION       = 0x8B8C
//, GL_CURRENT_PROGRAM                = 0x8B8D
//, GL_STENCIL_BACK_REF               = 0x8CA3
//, GL_STENCIL_BACK_VALUE_MASK        = 0x8CA4
//, GL_STENCIL_BACK_WRITEMASK         = 0x8CA5
                                          
//, GL_DONT_CARE                      = 0x1100
//, GL_FASTEST                        = 0x1101
  , GL_NICEST                         = 0x1102
                                          
  , GL_POINTS                         = 0x0000
  , GL_LINES                          = 0x0001
  , GL_LINE_LOOP                      = 0x0002
  , GL_LINE_STRIP                     = 0x0003
  , GL_TRIANGLES                      = 0x0004
  , GL_TRIANGLE_STRIP                 = 0x0005
  , GL_TRIANGLE_FAN                   = 0x0006
//, GL_QUADS                          = 0x0007
//, GL_QUAD_STRIP                     = 0x0008
//, GL_POLYGON                        = 0x0009
                                          
//, GL_CLAMP                          = 0x2900
  , GL_REPEAT                         = 0x2901
                                          
  , GL_VENDOR                         = 0x1F00
  , GL_RENDERER                       = 0x1F01
  , GL_VERSION                        = 0x1F02
  , GL_EXTENSIONS                     = 0x1F03
                                          
  , GL_BGR_EXT                        = 0x80E0
  , GL_BGRA_EXT                       = 0x80E1
  , GL_CLAMP                          = 0x2900
  , GL_CLAMP_TO_BORDER                = 0x812D
  , GL_TEXTURE_BORDER_COLOR           = 0x1004
  , GL_DEPTH_COMPONENT                = 0x1902
  , GL_DEPTH_COMPONENT32              = 0x81A7
  , GL_DEPTH_COMPONENT24              = 0x81A6
  , GL_DEPTH_STENCIL                  = 0x84F9
  , GL_UNSIGNED_INT_24_8              = 0x84FA
  , GL_DEPTH24_STENCIL8               = 0x88F0
  , GL_DEPTH_STENCIL_ATTACHMENT       = 0x821A
  , GL_STENCIL_INDEX                  = 0x1901
  , GL_STENCIL_INDEX8                 = 0x8D48
  , GL_RGBA16F                        = 0x881A
  , GL_HALF_FLOAT                     = 0x8D61
  , GL_RGBA32F                        = 0x8814
//, GL_ALPHA32F                       = 0x8816
  , GL_R16F                           = 0x822D
  , GL_RG16F                          = 0x822F
  , GL_R32F                           = 0x822E
  , GL_RG32F                          = 0x8230
  , GL_RG                             = 0x8227
  , GL_R8                             = 0x8229
  , GL_RG8                            = 0x822B
  , GL_RGB8                           = 0x8051
  , GL_ALPHA8                         = 0x803C
  , GL_LUMINANCE8                     = 0x8040
  , GL_R16                            = 0x822A
  , GL_RG16                           = 0x822C
  , GL_RGBA16                         = 0x805B
  , GL_ALPHA16                        = 0x803E
  , GL_RED_SNORM                      = 0x8F90
  , GL_RG_SNORM                       = 0x8F91
  , GL_RGBA_SNORM                     = 0x8F93
  , GL_ALPHA_SNORM                    = 0x9010
  , GL_R8_SNORM                       = 0x8F94
  , GL_RG8_SNORM                      = 0x8F95
  , GL_RGBA8_SNORM                    = 0x8F97
  , GL_ALPHA8_SNORM                   = 0x9014
  , GL_R16_SNORM                      = 0x8F98
  , GL_RG16_SNORM                     = 0x8F99
  , GL_RGBA16_SNORM                   = 0x8F9B
  , GL_ALPHA16_SNORM                  = 0x9018
  , GL_RGBA_FLOAT_MODE                = 0x8820
  , GL_CLAMP_VERTEX_COLOR             = 0x891A
  , GL_CLAMP_FRAGMENT_COLOR           = 0x891B
  , GL_CLAMP_READ_COLOR               = 0x891C
  , GL_FIXED_ONLY                     = 0x891D
  , GL_DRAW_BUFFER0                   = 0x8825
  , GL_LINE_SMOOTH                    = 0x0B20
  , GL_LINE_SMOOTH_HINT               = 0x0C52
  , GL_SMOOTH_LINE_WIDTH_RANGE        = 0x0B22
  , GL_SHADING_LANGUAGE_VERSION       = 0x8B8C
  , GL_POINT_SPRITE                   = 0x8861
  , GL_VERTEX_PROGRAM_POINT_SIZE      = 0x8642
  , GL_RGBA8                          = 0x8058
  , GL_GEOMETRY_SHADER                = 0x8DD9
  , GL_GEOMETRY_VERTICES_OUT          = 0x8DDA
  , GL_GEOMETRY_INPUT_TYPE            = 0x8DDB
  , GL_GEOMETRY_OUTPUT_TYPE           = 0x8DDC
  , GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS=0x8C29
  , GL_LINES_ADJACENCY                = 0xA
  , GL_LINE_STRIP_ADJACENCY           = 0xB
  , GL_TRIANGLES_ADJACENCY            = 0xC
  , GL_TRIANGLE_STRIP_ADJACENCY       = 0xD
  , GL_MAX_FRAGMENT_UNIFORM_COMPONENTS= 0x8B49
  , GL_MAX_VERTEX_UNIFORM_COMPONENTS  = 0x8B4A
  , GL_POINT_SIZE_RANGE               = 0x0B12
  , GL_DOUBLE                         = 0x140A

  , GL_MIN                            = 0x8007
  , GL_MAX                            = 0x8008

//, GL_BUFFER_SIZE                    = 0x8764
//, GL_BUFFER_USAGE                   = 0x8765
  , GL_ARRAY_BUFFER                   = 0x8892
  , GL_ELEMENT_ARRAY_BUFFER           = 0x8893
//, GL_ARRAY_BUFFER_BINDING           = 0x8894
//, GL_ELEMENT_ARRAY_BUFFER_BINDING   = 0x8895
//, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING=0x889F
//, GL_STREAM_DRAW                    = 0x88E0
  , GL_STATIC_DRAW                    = 0x88E4
  , GL_DYNAMIC_DRAW                   = 0x88E8
};
#else
struct nogl
{
  typedef ::GLenum GLenum;
  typedef ::GLint GLint;
  typedef ::GLsizei GLsizei;
  typedef ::GLuint GLuint;
  typedef ptrdiff_t GLsizeiptr;
  typedef ptrdiff_t GLintptr;
};
#endif // GL_NO_ERROR

#include "TD_PackPop.h"

#endif // NOGL
