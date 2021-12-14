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
// GLES2 device local context (Emscripten)

#include "OdaCommon.h"
#include "TrGL2LocalContext.h"
#include "GLES2Include.h"

#include <EGL/egl.h>
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/html5.h>

#include <string>

using namespace emscripten;

class OdGLES2LocalContextImpl : public OdTrGL2LocalContext {
protected:
  static int m_WebGLContext;
  static int m_usedCount;
  bool       m_bContextCreated;
  
public:
  OdGLES2LocalContextImpl()
    : OdTrGL2LocalContext()
    , m_bContextCreated(false)
  {
  }

  ~OdGLES2LocalContextImpl() { 
    -- m_usedCount;
    destroyContext(); 
  }

  void createContext(OdTrVisRenderClient *pDevice) {
    if (m_usedCount <= 0) {
      if (pDevice->hasProperty(OD_T("CreateContext")) && 
         !pDevice->getProperty(OD_T("CreateContext"))->getBool()) 
      {
        m_bContextCreated = true;
        return;
      }

       char* pTarget = (char*) EM_ASM_INT({
        if (! Module['canvas']) {
          console.error("Canvas is not set to module, please use module.canvas = document.querySelector('#canvas');");
          return 0;
        }

        var target = "#canvas";

        if (Module['canvas']['id']) {
          target = "#" + Module['canvas']['id'];
        }

        var lengthBytes = lengthBytesUTF8(target) + 1;
        var stringOnWasmHeap = _malloc(lengthBytes);
        stringToUTF8(target, stringOnWasmHeap, lengthBytes);

        return stringOnWasmHeap;
      }, 0);

      std::string strTarget = std::string(pTarget);
      std::cout << "Render canvas id = " << strTarget << std::endl;

      EmscriptenWebGLContextAttributes attrs;
      emscripten_webgl_init_context_attributes(&attrs);
      attrs.alpha                 = false;
      attrs.depth                 = true;
      attrs.stencil               = true;
      attrs.antialias             = false;
      attrs.preserveDrawingBuffer = true;
      m_WebGLContext  = emscripten_webgl_create_context(pTarget, &attrs);
    } 

    ++ m_usedCount ;
    m_bContextCreated = true;
  }

  void destroyContext() {
    m_bContextCreated = false;
    if (m_usedCount <= 0 && m_WebGLContext > 0) {
      EMSCRIPTEN_RESULT res = emscripten_webgl_destroy_context(m_WebGLContext);
      m_WebGLContext = -1;
      m_usedCount    = 0;
    }
  }

  bool isContextCreated() const { return m_bContextCreated; }

  void makeCurrentContext() {
    if (m_WebGLContext > 0) {
      EMSCRIPTEN_RESULT res = emscripten_webgl_make_context_current(m_WebGLContext);
    }
  }

  void presentContext() { }

  bool isExtensionSupported(const char * pExtensionName) { 
    if (m_WebGLContext > 0) {
      return emscripten_webgl_enable_extension(m_WebGLContext, pExtensionName);
    }
    return false;
	}

  void *acquireExtensionFunctionPtr(const char *pFunctionName)
  {
    return (void*)::eglGetProcAddress(pFunctionName);
  }
};

OdSmartPtr<OdTrGL2LocalContext> OdTrGL2LocalContext::createLocalContext(OdTrVisRenderClient *pDevice)
{
  OdTrGL2LocalContextPtr pContext = OdRxObjectImpl<OdGLES2LocalContextImpl, OdTrGL2LocalContext>::createObject();
  pContext->createContext(pDevice);
  return pContext;
}

int OdGLES2LocalContextImpl::m_WebGLContext = -1;
int OdGLES2LocalContextImpl::m_usedCount    = 0;
