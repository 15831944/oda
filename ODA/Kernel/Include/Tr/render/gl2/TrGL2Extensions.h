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
// GLES2 extensions

#ifndef ODTRGL2EXTENSIONS
#define ODTRGL2EXTENSIONS

#include "../nogl/TrRndNoGLExtensionsRegistry.h"

// BGR/BGRA textures (kExtensionBGR/kExtensionBGRA)
#ifndef GL_BGR_EXT
#define GL_BGR_EXT                        0x80E0
#endif // GL_BGR_EXT
#ifndef GL_BGRA_EXT
#define GL_BGRA_EXT                       0x80E1
#endif // GL_BGRA_EXT

// ClampToEdge wrap mode (kExtensionTextureEdgeClamp)
#ifndef GL_CLAMP
#define GL_CLAMP                          0x2900
#endif // GL_CLAMP

// ClampToBorder wrap mode (kExtensionTextureBorderClamp)
#ifndef GL_CLAMP_TO_BORDER
#define GL_CLAMP_TO_BORDER                0x812D
#endif // GL_CLAMP_TO_BORDER
#ifndef GL_TEXTURE_BORDER_COLOR
#define GL_TEXTURE_BORDER_COLOR           0x1004
#endif // GL_TEXTURE_BORDER_COLOR

// Depth textures (kExtensionDepthTexture)
#ifndef GL_DEPTH_COMPONENT
#define GL_DEPTH_COMPONENT                0x1902
#endif // GL_DEPTH_COMPONENT
#ifndef GL_DEPTH_COMPONENT32
#define GL_DEPTH_COMPONENT32              0x81A7
#endif // GL_DEPTH_COMPONENT32

// 24-bit depth buffer format (kExtensionDepth24)
#ifndef GL_DEPTH_COMPONENT24
#define GL_DEPTH_COMPONENT24              0x81A6
#endif // GL_DEPTH_COMPONENT24

// Packed depth-stencil format (kExtensionPackedDepthStencil)
#ifndef GL_DEPTH_STENCIL
#define GL_DEPTH_STENCIL                  0x84F9
#endif // GL_DEPTH_STENCIL
#ifndef GL_UNSIGNED_INT_24_8
#define GL_UNSIGNED_INT_24_8              0x84FA
#endif // GL_UNSIGNED_INT_24_8
#ifndef GL_DEPTH24_STENCIL8
#define GL_DEPTH24_STENCIL8               0x88F0
#endif // GL_DEPTH24_STENCIL8
// WebGL case
#ifndef GL_DEPTH_STENCIL_ATTACHMENT
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#endif // GL_DEPTH_STENCIL_ATTACHMENT

// GL_STENCIL_INDEX8 render buffer storage (kExtensionStencil8)
#ifndef GL_STENCIL_INDEX
#define GL_STENCIL_INDEX                  0x1901
#endif // GL_STENCIL_INDEX
#ifndef GL_STENCIL_INDEX8
#define GL_STENCIL_INDEX8                 0x8D48
#endif // GL_STENCIL_INDEX8

// Half-float textures support (kExtensionTextureHalfFloat)
#ifndef GL_RGBA16F
#define GL_RGBA16F                        0x881A
#endif // GL_RGBA16F
//#ifndef GL_ALPHA16F
//#define GL_ALPHA16F                       0x881C
//#endif // GL_ALPHA16F
// This definitions is only for desktop OpenGL. Native OpenGL ES should use GL_RGBA instead and GL_HALF_FLOAT will be accepted as texture type.
#ifndef GL_HALF_FLOAT
#define GL_HALF_FLOAT                     0x8D61
#endif // GL_HALF_FLOAT

// Floating point textures support (kExtensionTextureFloat)
#ifndef GL_RGBA32F
#define GL_RGBA32F                        0x8814
#endif // GL_RGBA32F
//#ifndef GL_ALPHA32F
//#define GL_ALPHA32F                       0x8816
//#endif // GL_ALPHA32F
// This definitions is only for desktop OpenGL. Native OpenGL ES should use GL_RGBA instead and GL_FLOAT will be accepted as texture type.

// R/RG half-float textures support (kExtensionTextureHalfFloat + kExtensionTextureRG)
#ifndef GL_R16F
#define GL_R16F                           0x822D
#endif // GL_R16F
#ifndef GL_RG16F
#define GL_RG16F                          0x822F
#endif // GL_RG16F

// R/RG floating point textures support (kExtensionTextureFloat + kExtensionTextureRG)
#ifndef GL_R32F
#define GL_R32F                           0x822E
#endif // GL_R32F
#ifndef GL_RG32F
#define GL_RG32F                          0x8230
#endif // GL_RG32F

// R and RG normalized texture and render buffer formats support (kExtensionTextureRG)
#ifndef GL_RED
#define GL_RED                            0x1903
#endif // GL_RED
#ifndef GL_RG
#define GL_RG                             0x8227
#endif // GL_RG
#ifndef GL_R8
#define GL_R8                             0x8229
#endif // GL_R8
#ifndef GL_RG8
#define GL_RG8                            0x822B
#endif // GL_RG8
#ifndef GL_RGB8
#define GL_RGB8                           0x8051
#endif // GL_RGB8
#ifndef GL_ALPHA8
#define GL_ALPHA8                         0x803C
#endif // GL_ALPHA8
#ifndef GL_LUMINANCE8
#define GL_LUMINANCE8                     0x8040
#endif // GL_LUMINANCE8

// R and RG normalized texture and render buffer formats support (kExtensionTextureNorm16)
#ifndef GL_R16
#define GL_R16                            0x822A
#endif // GL_R16
#ifndef GL_RG16
#define GL_RG16                           0x822C
#endif // GL_RG16
#ifndef GL_RGBA16
#define GL_RGBA16                         0x805B
#endif // GL_RGBA16
#ifndef GL_ALPHA16
#define GL_ALPHA16                        0x803E
#endif // GL_ALPHA16

// R and RG signed normalized texture and render buffer formats support (kExtensionTextureSNorm)
#ifndef GL_RED_SNORM
#define GL_RED_SNORM                      0x8F90
#endif // GL_RED_SNORM
#ifndef GL_RG_SNORM
#define GL_RG_SNORM                       0x8F91
#endif // GL_RG_SNORM
#ifndef GL_RGBA_SNORM
#define GL_RGBA_SNORM                     0x8F93
#endif // GL_RGBA_SNORM
#ifndef GL_ALPHA_SNORM
#define GL_ALPHA_SNORM                    0x9010
#endif // GL_ALPHA_SNORM
#ifndef GL_R8_SNORM
#define GL_R8_SNORM                       0x8F94
#endif // GL_R8_SNORM
#ifndef GL_RG8_SNORM
#define GL_RG8_SNORM                      0x8F95
#endif // GL_RG8_SNORM
#ifndef GL_RGBA8_SNORM
#define GL_RGBA8_SNORM                    0x8F97
#endif // GL_RGBA8_SNORM
#ifndef GL_ALPHA8_SNORM
#define GL_ALPHA8_SNORM                   0x9014
#endif // GL_ALPHA8_SNORM

// R and RG signed normalized texture and render buffer formats support (kExtensionTextureSNorm16)
#ifndef GL_R16_SNORM
#define GL_R16_SNORM                      0x8F98
#endif // GL_R16_SNORM
#ifndef GL_RG16_SNORM
#define GL_RG16_SNORM                     0x8F99
#endif // GL_RG16_SNORM
#ifndef GL_RGBA16_SNORM
#define GL_RGBA16_SNORM                   0x8F9B
#endif // GL_RGBA16_SNORM
#ifndef GL_ALPHA16_SNORM
#define GL_ALPHA16_SNORM                  0x9018
#endif // GL_ALPHA16_SNORM

// Clamping color buffer output control (kExtensionFloatBufferClamp)
#ifndef GL_RGBA_FLOAT_MODE
#define GL_RGBA_FLOAT_MODE                0x8820
#endif // GL_RGBA_FLOAT_MODE
#ifndef GL_CLAMP_VERTEX_COLOR
#define GL_CLAMP_VERTEX_COLOR             0x891A
#endif // GL_CLAMP_VERTEX_COLOR
#ifndef GL_CLAMP_FRAGMENT_COLOR
#define GL_CLAMP_FRAGMENT_COLOR           0x891B
#endif // GL_CLAMP_FRAGMENT_COLOR
#ifndef GL_CLAMP_READ_COLOR
#define GL_CLAMP_READ_COLOR               0x891C
#endif // GL_CLAMP_READ_COLOR
#ifndef GL_FIXED_ONLY
#define GL_FIXED_ONLY                     0x891D
#endif // GL_FIXED_ONLY

// Multiple Render Targets support (kExtensionDrawBuffers)
#ifndef GL_DRAW_BUFFER0
#define GL_DRAW_BUFFER0                   0x8825
#endif // GL_DRAW_BUFFER0

// Lines anti-aliasing
#ifndef GL_LINE_SMOOTH
#define GL_LINE_SMOOTH                    0x0B20
#endif // GL_LINE_SMOOTH
#ifndef GL_LINE_SMOOTH_HINT
#define GL_LINE_SMOOTH_HINT               0x0C52
#endif // GL_LINE_SMOOTH_HINT
#ifndef GL_SMOOTH_LINE_WIDTH_RANGE
#define GL_SMOOTH_LINE_WIDTH_RANGE        0x0B22
#endif // GL_SMOOTH_LINE_WIDTH_RANGE

// Shading language version
#ifndef GL_SHADING_LANGUAGE_VERSION
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#endif // GL_SHADING_LANGUAGE_VERSION

// Required for some desktop GPU's which invoke compatible OpenGL profiles (not required since OpenGL 3.2)
#ifndef GL_POINT_SPRITE
#define GL_POINT_SPRITE                   0x8861
#endif // GL_POINT_SPRITE
// OpenGL ES 2 GLSL point size (not required by native, but necessary for PC and WebGL)
#ifndef GL_VERTEX_PROGRAM_POINT_SIZE
#define GL_VERTEX_PROGRAM_POINT_SIZE      0x8642
#endif // GL_VERTEX_PROGRAM_POINT_SIZE

// RGBA framebuffers
// Note: don't think that we must check availability for this format. It is defined with GL_EXT_texture extension for desktop OpenGL, but
//       this format will be available anyway if frame buffers supported. It is defined with GL_OES_rgb8_rgba8 extension for OpemGL ES, but
//       it is in any case is one of the native OpenGL formats, so don't think that it will be unavailable anywhere.
#ifndef GL_RGBA8
#define GL_RGBA8                          0x8058
#endif // GL_RGBA8
//

// Geometry shader extension
#ifndef GL_GEOMETRY_SHADER
#define GL_GEOMETRY_SHADER                0x8DD9
#endif // GL_GEOMETRY_SHADER
#ifndef GL_GEOMETRY_VERTICES_OUT
#define GL_GEOMETRY_VERTICES_OUT          0x8DDA
#endif // GL_GEOMETRY_VERTICES_OUT
#ifndef GL_GEOMETRY_INPUT_TYPE
#define GL_GEOMETRY_INPUT_TYPE            0x8DDB
#endif // GL_GEOMETRY_INPUT_TYPE
#ifndef GL_GEOMETRY_OUTPUT_TYPE
#define GL_GEOMETRY_OUTPUT_TYPE           0x8DDC
#endif // GL_GEOMETRY_OUTPUT_TYPE
#ifndef GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS 0x8C29
#endif // GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS
#ifndef GL_LINES_ADJACENCY
#define GL_LINES_ADJACENCY                0xA
#endif // GL_LINES_ADJACENCY
#ifndef GL_LINE_STRIP_ADJACENCY
#define GL_LINE_STRIP_ADJACENCY           0xB
#endif // GL_LINE_STRIP_ADJACENCY
#ifndef GL_TRIANGLES_ADJACENCY
#define GL_TRIANGLES_ADJACENCY            0xC
#endif // GL_TRIANGLES_ADJACENCY
#ifndef GL_TRIANGLE_STRIP_ADJACENCY
#define GL_TRIANGLE_STRIP_ADJACENCY       0xD
#endif // GL_TRIANGLE_STRIP_ADJACENCY

// OpenGL ES2 Compatibility (this definitions must be invoked on desktop if ES2 compatibility doesn't supported)
#ifndef GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#endif // GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
#ifndef GL_MAX_VERTEX_UNIFORM_COMPONENTS
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS  0x8B4A
#endif // GL_MAX_VERTEX_UNIFORM_COMPONENTS
#ifndef GL_POINT_SIZE_RANGE
#define GL_POINT_SIZE_RANGE               0x0B12
#endif // GL_POINT_SIZE_RANGE
#ifndef GL_DOUBLE
#define GL_DOUBLE                         0x140A
#endif // GL_DOUBLE

class OdTrRndLocalContext;

class OdTrGL2ExtensionsRegistry : public OdTrRndNoGLExtensionsRegistry
{
  public:
    OdTrGL2ExtensionsRegistry(OdTrRndLocalContext *pLocalContext);

    bool isExtensionSupported(ExtensionName nExt);

    bool isFunctionAvailable(FunctionName nFunc);

    // Extension function callers
    // kExtensionDrawBuffers
    void glDrawBuffers(GLsizei n, const GLenum *bufs);
    // kExtensionGeometryShader
    void glProgramParameteri(GLuint program, GLenum pname, GLint value);
    void glFramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level);
    // kExtensionFloatBufferClamp
    void glClampColor(GLenum target, GLenum clamp);
};

#endif // ODTRGL2EXTENSIONS
