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
// GLES2 device local context (Qt)

#include <QGLWidget>
#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
#include <QOpenGLWidget>
#include <QOpenGLContext>
#endif
#include <QPointer>

#include "OdaCommon.h"
#include "TrGL2LocalContext.h"
#include "GLES2Include.h"

class OdGLES2LocalContextImpl : public OdTrGL2LocalContext
{
  protected:
    bool m_bContextCreated;
    QPointer<QWidget> m_pQWidget; // QGLWidget or QOpenGLWidget since Qt 5.4 // QPointer<QGLWidget> m_pGLWidget;
    int m_nColorDepth;
    bool m_bColorIndexMode;
    bool m_bDoubleBuffer;
  public:
    OdGLES2LocalContextImpl()
      : OdTrGL2LocalContext()
      , m_nColorDepth(24)
      , m_bColorIndexMode(false)
      , m_bContextCreated(false)
      , m_bDoubleBuffer(false)
    {
    }

#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
    QOpenGLWidget* getQOpenGLWidget() const
    {
      return qobject_cast<QOpenGLWidget*>(m_pQWidget);
    }
#endif
    QGLWidget* getQGLWidget() const
    {
      return qobject_cast<QGLWidget*>(m_pQWidget);
    }
    static bool makeCurrentContext(QWidget* pQWidget, bool checkIsValid = true)
    {
#  if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
      if (QOpenGLWidget* pQOpenGLWidget = qobject_cast<QOpenGLWidget*>(pQWidget))
      {
        if (checkIsValid && !pQOpenGLWidget->isValid())
          return false;
        pQOpenGLWidget->makeCurrent();
      }
      else
#  endif
      if (QGLWidget* pQGLWidget = qobject_cast<QGLWidget*>(pQWidget))
      {
        if (checkIsValid && !pQGLWidget->isValid())
          return false;
        pQGLWidget->makeCurrent();
      }
      else
        return false;
      return true;
    }

    int getDeviceColorDepth() const
    {
      int val = -1;
#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
      if (QOpenGLWidget* pQOpenGLWidget = getQOpenGLWidget())
        val = pQOpenGLWidget->depth();
#endif
      if (val < 0)
      {
        if (QGLWidget* pQGLWidget = getQGLWidget())
          val = pQGLWidget->depth();
      }
      ODA_ASSERT_ONCE(val >= 0 && val <= 32);
      if (val < 0 || val > 24)
        val = 24;
      return val;
    }

    ~OdGLES2LocalContextImpl()
    {
      destroyContext();
      //while (popCurrentContext()) ; // strange with missing pushCurrentContext()
    }

    void createContext(OdTrVisRenderClient *pDevice)
    {
      if (pDevice->hasProperty(OD_T("CreateContext")) &&
          !pDevice->getProperty(OD_T("CreateContext"))->getBool())
      {
        m_bContextCreated = true;
        return;
      }
      if (pDevice->hasProperty(OD_T("QGLWidget")))
      {
        QWidget* ptr = (QWidget*)pDevice->getProperty(OD_T("QGLWidget"))->getIntPtr();
        m_pQWidget = ptr;
        if (m_pQWidget.isNull())
          throw OdError(eNotInitializedYet);
        m_nColorDepth = getDeviceColorDepth();
        createContext();
      }
      // Double buffer
      m_bDoubleBuffer = false;
      if (pDevice->hasProperty(OD_T("DoubleBufferEnabled")))
        m_bDoubleBuffer = pDevice->getProperty(OD_T("DoubleBufferEnabled"))->getBool();

#if !(defined(ANDROID) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED))
      // Run extensions initialization
      GLES2_Extensions_Initialize(pDevice);
#endif

      m_bContextCreated = true;
    }

    bool renderToScreen() const 
    { 
      return true; 
    }

    void createContext()
    {
      if (!OdGLES2LocalContextImpl::makeCurrentContext(m_pQWidget, true))
        return;

      ODA_ASSERT_ONCE(renderToScreen());
      m_bColorIndexMode = true;

     // Create the palette
     // translate logical logical DD palette to system one 

     // createPalette(&m_logPalette);

      m_bContextCreated = true;
    }

    void updateContext(OdTrVisRenderClient *pDevice)
    {
      if (!m_bContextCreated)
        createContext();
      else
        makeCurrentContext();
    }

    void destroyContext()
    {
      if (!OdGLES2LocalContextImpl::makeCurrentContext(m_pQWidget, true))
        return;

      m_bContextCreated = false;
    }

    bool isContextCreated() const
    {
      return m_bContextCreated;
    }

    void makeCurrentContext()
    {
      OdGLES2LocalContextImpl::makeCurrentContext(m_pQWidget, false);
    }

    void presentContext()
    {
      makeCurrentContext();
#    if defined(ANDROID)
#      if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
      if (QOpenGLWidget* pQOpenGLWidget = getQOpenGLWidget())
      {
        // TODO
        QOpenGLContext* pCtx = pQOpenGLWidget->context();
        ODA_ASSERT_ONCE(pCtx);
        pCtx->swapBuffers(pCtx->surface());
      }
      else
#      endif
      if (QGLWidget* pQGLWidget = getQGLWidget())
        pQGLWidget->swapBuffers();
#    endif
    }

#if !(defined(ANDROID) || defined(__IPHONE_OS_VERSION_MIN_REQUIRED))
    bool isExtensionBasedEmulation() const
    {
      return true;
    }

    bool isExtensionSupported(const char *pExtensionName)
    {
      return GLES2_ParseExtension(pExtensionName, NULL);
    }

    void *acquireExtensionFunctionPtr(const char *pFunctionName)
    {
      return GLES2_GetProcAddress(pFunctionName);
    }
#endif
};

OdSmartPtr<OdTrGL2LocalContext> OdTrGL2LocalContext::createLocalContext(OdTrVisRenderClient *pDevice)
{
  OdTrGL2LocalContextPtr pContext = OdRxObjectImpl<OdGLES2LocalContextImpl, OdTrGL2LocalContext>::createObject();
  pContext->createContext(pDevice);
  return pContext;
}
