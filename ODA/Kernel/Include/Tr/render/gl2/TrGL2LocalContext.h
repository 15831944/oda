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
// GLES2 device local context

#ifndef ODTRGL2LOCALCONTEXT
#define ODTRGL2LOCALCONTEXT

#include "TD_PackPush.h"

#include "../TrRndLocalContext.h"
#include "GLES2Include.h"
#include "../TrRenderExport.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
#define GLProcedure0(t,a) virtual t no##a() const;
#define GLProcedure(t,a,b) virtual t no##a(b) const;
#define GLFunction(t,a,b) virtual t no##a(b) const;
#define GLFunction2(t,a,b,c) virtual t no##a(b,c) const;
#define GLFunction3(t,a,b,c,d) virtual t no##a(b,c,d) const;
#define GLFunction4(t,a,b,c,d,e) virtual t no##a(b,c,d,e) const;
#define GLFunction5(t,a,b,c,d,e,f) virtual t no##a(b,c,d,e,f) const;
#define GLFunction6(t,a,b,c,d,e,f,g) virtual t no##a(b,c,d,e,f,g) const;
#define GLFunction7(t,a,b,c,d,e,f,g,h) virtual t no##a(b,c,d,e,f,g,h) const;
#define GLFunction9(t,a,b,c,d,e,f,g,h,i,j) virtual t no##a(b,c,d,e,f,g,h,i,j) const;

class TDRENDER_EXPORT OdTrGL2LocalContext : public OdTrRndLocalContext
{
  virtual void CreateExtensionRegistry() const;
  public:
    OdTrGL2LocalContext() : OdTrRndLocalContext() { }

    virtual OdTrRndNoGLShaderState* CreateShaderState() const;
    virtual void* GetNoneDrawBuffer();
    virtual void* GetNoneReadBuffer();
    virtual void* GetFrameDrawBuffer();
    virtual void* GetFrameReadBuffer();
    virtual void* GetDrawBuffersExt(OdTrRndNoGLExtensionsRegistry &extReg, GLsizei n, const GLenum *bufs, bool bNativeES = false);
    virtual void DeleteDrawBuffer(void *p);
    virtual void DeleteReadBuffer(void *p);
#include "Tr/render/nogl/TrRndNoGLContextHolderFunctionsList.inl"

    // Each platform implementation must implement last one method
    static OdSmartPtr<OdTrGL2LocalContext> createLocalContext(OdTrVisRenderClient *pDevice);
};

typedef OdSmartPtr<OdTrGL2LocalContext> OdTrGL2LocalContextPtr;

#include "TD_PackPop.h"

#endif // ODTRGL2LOCALCONTEXT
