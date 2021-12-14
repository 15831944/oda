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
// Base local context interface

#ifndef ODTRRNDLOCALCONTEXT
#define ODTRRNDLOCALCONTEXT

#include "TD_PackPush.h"

#include "../TrVisRenderClient.h"
#include "nogl/TrRndNoGLExtensionsRegistry.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
#define GLProcedure0(t,a) virtual t no##a() const{ return (t)0; }
#define GLProcedure(t,a,b) virtual t no##a(b) const{ return (t)0; }
#define GLFunction(t,a,b) virtual t no##a(b) const {}
#define GLFunction2(t,a,b,c) virtual t no##a(b,c) const {}
#define GLFunction3(t,a,b,c,d) virtual t no##a(b,c,d) const {}
#define GLFunction4(t,a,b,c,d,e) virtual t no##a(b,c,d,e) const {}
#define GLFunction5(t,a,b,c,d,e,f) virtual t no##a(b,c,d,e,f) const {}
#define GLFunction6(t,a,b,c,d,e,f,g) virtual t no##a(b,c,d,e,f,g) const {}
#define GLFunction7(t,a,b,c,d,e,f,g,h) virtual t no##a(b,c,d,e,f,g,h) const {}
#define GLFunction9(t,a,b,c,d,e,f,g,h,i,j) virtual t no##a(b,c,d,e,f,g,h,i,j) const {}


struct GLFunctions : public nogl
{
  virtual ~GLFunctions(){};
  virtual void glesSafeDrawArrays(GLenum mode, GLint first, GLsizei count) const
  {
    noglDrawArrays(mode, first, count);
  };
  virtual void* GetDrawBuffersExt(OdTrRndNoGLExtensionsRegistry &extReg, GLsizei n, const GLenum *bufs, bool bNativeES){return NULL;};
#include "nogl/TrRndNoGLContextHolderFunctionsList.inl"
};

class OdTrRndNoGLShaderState;
class OdTrRndLocalContext : public OdRxObject, public GLFunctions
{
    virtual void CreateExtensionRegistry() const{};
  protected:
    //mutable OdRxObjectPtr m_pExtensionsRegistry;
    mutable OdTrRndNoGLExtensionsRegistry *m_pExtensionsRegistry;
  public:
    OdTrRndLocalContext() : m_pExtensionsRegistry(NULL){ }
    virtual ~OdTrRndLocalContext()
    {
      if (m_pExtensionsRegistry)
        delete m_pExtensionsRegistry;
      m_pExtensionsRegistry = NULL;
    }

    virtual void createContext(OdTrVisRenderClient *pDevice) = 0;
    virtual void updateContext(OdTrVisRenderClient * /*pDevice*/) { }
    virtual void destroyContext() = 0;

    virtual bool isContextCreated() const = 0;

    virtual void makeCurrentContext() = 0;

    virtual bool pushCurrentContext() { return false; }
    virtual bool popCurrentContext() { return false; }

    virtual void presentContext() = 0;

    virtual bool isExtensionBasedEmulation() const { return false; }
    virtual bool isExtensionSupported(const char * /*pExtensionName*/) { return false; }
    virtual void *acquireExtensionFunctionPtr(const char * /*pFunctionName*/) { return NULL; }

    // Search OpenGL extension in cross-platform way using our own extensions registry
    OdTrRndNoGLExtensionsRegistry &extensionsRegistry() const
    {
      if (m_pExtensionsRegistry == NULL)
        CreateExtensionRegistry();
      return *m_pExtensionsRegistry;
    }
    virtual OdTrRndNoGLShaderState* CreateShaderState() const {return NULL;};
    virtual void* GetNoneDrawBuffer(){return NULL;};
    virtual void* GetNoneReadBuffer(){return NULL;};
    virtual void* GetFrameDrawBuffer(){return NULL;};
    virtual void* GetFrameReadBuffer(){return NULL;};
    virtual void DeleteDrawBuffer(void*){};
    virtual void DeleteReadBuffer(void*){};
};

typedef OdSmartPtr<OdTrRndLocalContext> OdTrRndLocalContextPtr;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrRndLocalContextStub : public OdTrRndLocalContext
{
  public:
    OdTrRndLocalContextStub() : OdTrRndLocalContext() { }
    ~OdTrRndLocalContextStub() { }

    virtual void createContext(OdTrVisRenderClient * /*pDevice*/) { }
    virtual void destroyContext() { }

    virtual bool isContextCreated() const { return true; }

    virtual void makeCurrentContext() { }

    virtual void presentContext() { }
};

#include "TD_PackPop.h"

#endif // ODTRRNDLOCALCONTEXT
