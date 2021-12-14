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
// Metal renderer local context

#ifndef ODTRMTLLOCALCONTEXT
#define ODTRMTLLOCALCONTEXT

#include "TD_PackPush.h"

#include "../TrRndLocalContext.h"

class OdTrMetalShadersRuntime;

class OdTrMetalLocalContext : public OdTrRndLocalContext
{
public:
  OdTrMetalLocalContext() : OdTrRndLocalContext() { }

  // Each platform implementation must implement last one method
  static OdSmartPtr<OdTrMetalLocalContext> createLocalContext(OdTrVisRenderClient *pDevice);
  //\/\delete macros below
#if 0
  virtual void glViewport(GLint x, GLint y, GLsizei width, GLsizei height) {};
  
  virtual void glDrawArrays(GLenum mode, GLint first, GLsizei count) {};
  virtual GLenum glGetError() { return GL_NO_ERROR; };

  virtual void glGenTextures(GLsizei n, GLuint *textures) {};
  virtual void glBindTexture(GLenum target, GLuint texture) {};
  virtual void glTexParameteri(GLenum target, GLenum pname, GLint param) {};
  virtual void glPixelStorei(GLenum pname, GLint param) {};
  virtual void glTexImage2D(GLenum target, GLint level, GLint internalformat, 
                            GLsizei width, GLsizei height, GLint border, 
                            GLenum format, GLenum type, const GLvoid *data) {};
  virtual void glActiveTexture(GLenum texture) {};
  virtual void glTexParameterfv(GLenum target, GLenum pname, const GLfloat *params) {};
  virtual void glDeleteTextures(GLsizei n, const GLuint * textures) {};
  virtual void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid * data) {};

  virtual void glEnable(GLenum  cap) {};
  virtual void glDisable(GLenum  cap) {};
  virtual void glStencilFunc(GLenum func, GLint ref, GLuint mask) {};
  virtual void glStencilMask(GLuint mask) {};
  virtual void glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass) {};
  virtual void glClearStencil(GLint s) {};
  virtual void glClear(GLbitfield mask) {};
  virtual void glDepthFunc(GLenum func) {};
  virtual void glDepthMask(GLboolean flag) {};
  virtual void glDepthRangef(GLfloat nearVal, GLfloat farVal) {};
  virtual void glClearDepthf(GLfloat depth) {};
  virtual void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) {};
  virtual void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {};
  virtual void glBlendFunc(GLenum sfactor, GLenum dfactor) {};
  virtual void glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha) {};
  virtual void glBlendEquation(GLenum mode) {};
  virtual void glBlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha) {};
  virtual void glBlendColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {};
  virtual void glPolygonOffset(GLfloat factor, GLfloat units) {};
  virtual void glCullFace(GLenum mode) {};
  virtual void glScissor(GLint x, GLint y, GLsizei width, GLsizei height) {};

  virtual void glGenFramebuffers(GLsizei n, GLuint *ids) {};
  virtual void glBindFramebuffer(GLenum target, GLuint framebuffer) {};
  virtual void glFramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level) {};
  virtual void glFramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer) {};
  virtual GLenum glCheckFramebufferStatus(GLenum target) { return 0; };
  virtual void glDeleteFramebuffers(GLsizei n, const GLuint * framebuffers) {};
    
  virtual void glGenRenderbuffers(GLsizei n, GLuint * renderbuffers) {};
  virtual void glBindRenderbuffer(GLenum target, GLuint renderbuffer) {};
  virtual void glRenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height) {};
  virtual void glDeleteRenderbuffers(GLsizei n, const GLuint * renderbuffers) {};

  virtual void glLineWidth(GLfloat width){};
#endif
  virtual void SetShadersRuntime(OdTrMetalShadersRuntime *ShadersRuntime){};
  virtual void SetVertexAttribPointerSize(GLuint Size){};
  virtual bool IsAttributeEnabled(GLuint index){return false;};
  virtual void MetalSetAttributeBuffer(GLuint index, int MetalIndex, int Size){};
  virtual void SendDepthRangeDiff(int index, int isVertex){};
};

typedef OdSmartPtr<OdTrMetalLocalContext> OdTrMetalLocalContextPtr;

#include "TD_PackPop.h"

#endif // ODTRMTLLOCALCONTEXT
