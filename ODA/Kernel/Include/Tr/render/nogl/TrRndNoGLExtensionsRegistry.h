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

#ifndef ODTRRNDNOGLEXTENSIONSREGISTRY
#define ODTRRNDNOGLEXTENSIONSREGISTRY

#include "NoGL.h"

class OdTrRndLocalContext;

class OdTrRndNoGLExtensionsRegistry : public nogl
{
  public:
    enum ExtensionName
    {
      kExtensionNonPow2 = 0,
      kExtensionNonPow2ES,
      kExtensionBGRA,
      kExtensionBGR,
      kExtensionTextureMirroredRepeat,
      kExtensionTextureEdgeClamp,
      kExtensionTextureBorderClamp,
      kExtensionDepthTexture,
      kExtensionDepthTextureCubeMap,
      kExtensionDepth24,
      kExtensionDepth32,
      kExtensionPackedDepthStencil,
      kExtensionStencil8,
      kExtensionTextureHalfFloat,
      kExtensionTextureFloat,
      kExtensionTextureRG,
      kExtensionTextureNorm16,
      kExtensionTextureSNorm,
      kExtensionTextureSNorm16,
      kExtensionFloatBufferClamp,
      kExtensionFragDepth,
      kExtensionDerivatives,
      kExtensionDrawBuffers,
      kExtensionElementIndexUInt,
      kExtensionGeometryShader,
      kExtensionES2Compatibility,

      kNumExtensions
    };
    enum FunctionName
    {
      kPfnDrawBuffers = 0,
      kPfnProgramParameteri,
      kPfnFramebufferTexture,
      kPfnClampColor,

      kNumFunctions
    };
  protected:
    OdUInt32 m_extensionChecked[kNumExtensions / 32 + 1];
    OdUInt32 m_extensionAvailable[kNumExtensions / 32 + 1];
    OdTrRndLocalContext *m_pLocalContext;
    void *m_pFunctionPtr[kNumFunctions];
    friend class OdTrRndLocalContext;
  public:
    //OdTrGL2ExtensionsRegistry(OdTrRndLocalContext *pLocalContext);
    virtual ~OdTrRndNoGLExtensionsRegistry(){};

    virtual bool isExtensionSupported(ExtensionName nExt) = 0;

    virtual bool isFunctionAvailable(FunctionName nFunc) = 0;

    // Extension function callers
    // kExtensionDrawBuffers
    virtual void glDrawBuffers(GLsizei n, const GLenum *bufs) = 0;
    // kExtensionGeometryShader
    virtual void glProgramParameteri(GLuint program, GLenum pname, GLint value) = 0;
    virtual void glFramebufferTexture(GLenum target, GLenum attachment, GLuint texture, GLint level) = 0;
    // kExtensionFloatBufferClamp
    virtual void glClampColor(GLenum target, GLenum clamp) = 0;
};

#endif // ODTRRNDNOGLEXTENSIONSREGISTRY
