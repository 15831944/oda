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
//
// ExGsQtVectorizeDevice.cpp
//

#include "OdaCommon.h"
#include "ExGsQtVectorizeDevice.h"
#include "SaveState.h"

OD_OPENGL_DECLARE_COMMON_PROPS(ExGsOpenGLVectorizeDeviceBase, _undefined_, ExGsQtDevice);

//ODRX_DECLARE_PROPERTY2(WGLContext, ExGsQtVectorizeDevice)

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP(ExGsQtVectorizeDevice);
  ODRX_INHERIT_PROPERTIES(ExGsOpenGLVectorizeDeviceBase)
  //ODRX_GENERATE_PROPERTY2(WGLContext, ExGsQtVectorizeDevice)
ODRX_END_DYNAMIC_PROPERTY_MAP(ExGsQtVectorizeDevice);

//ODRX_DEFINE_PROPERTY_METHODS2(WGLContext, ExGsQtVectorizeDevice, ExGsQtVectorizeDevice, getWGLContext, setWGLContext, getInt32); // TODO getInt64

//Implementation of class ExGsQtVectorizeDevice
//*******************************************************************************/
// Constructor
ExGsQtVectorizeDevice::ExGsQtVectorizeDevice()
  : m_bContextCreated(false)
{
}

//*******************************************************************************/
// Destructor
ExGsQtVectorizeDevice::~ExGsQtVectorizeDevice()
{
  deleteContext();
}

//*******************************************************************************/
//  createContext()
void ExGsQtVectorizeDevice::createContext()
{
  if (!ExGsQtDevice::makeCurrentContext(getQWidget(), true))
    return;

  ODA_ASSERT_ONCE(renderToScreen());
#if defined(Q_WS_MAC) || defined(Q_OS_MAC) || defined(__APPLE__)
  m_bDoubleBufferEnabled = false;
#else
  m_bDoubleBufferEnabled = true;
#endif
  m_bColorIndexMode = true;

  // Create the palette
  // translate logical logical DD palette to system one 
  createPalette(&m_logPalette);

  m_bContextCreated = true;

  onContextCreate();
}

//*******************************************************************************/
// deleteContext()
void ExGsQtVectorizeDevice::deleteContext()
{
  if (!ExGsQtDevice::makeCurrentContext(getQWidget(), true))
    return;
  onContextDelete();
}

void ExGsQtVectorizeDevice::preReleaseResource()
{
  ExGsQtDevice::makeCurrentContext(getQWidget(), true);
}

void ExGsQtVectorizeDevice::postReleaseResource() 
{
}

//*******************************************************************************/
// checkExtensionFuncSupport()
//
bool ExGsQtVectorizeDevice::checkExtensionFuncSupport(OpenGLExtension /*extIdx*/) const
{
#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
  if (QOpenGLWidget* pQOpenGLWidget = getQOpenGLWidget())
    return pQOpenGLWidget->context() != NULL;
  else
#endif
  if (QGLWidget* pQGLWidget = getQGLWidget())
    return pQGLWidget->context() != NULL;
  // I have it via print on Tiger // ODA_FAIL_ONCE();
  return false;
}

//*******************************************************************************/
// getExtensionFuncPtrFromListInt()
//
void* ExGsQtVectorizeDevice::getExtensionFuncPtrFromListInt(const char* pFuncName) const
{
#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
  if (QOpenGLWidget* pQOpenGLWidget = getQOpenGLWidget())
  {
    QOpenGLContext* pCtx = pQOpenGLWidget->context();
    ODA_ASSERT_ONCE(pCtx);
    return (void*) pCtx->getProcAddress(pFuncName);
  }
  else
#endif
  if (QGLWidget* pQGLWidget = getQGLWidget())
  {
    const QGLContext* pCtx = pQGLWidget->context();
    ODA_ASSERT_ONCE(pCtx);
    return (void*) pCtx->getProcAddress(QString(pFuncName));
  }
  ODA_FAIL_ONCE();
}

//*******************************************************************************/
//  update()
void ExGsQtVectorizeDevice::update(OdGsDCRect* pRect)
{
  if (!m_bContextCreated)
    createContext();
  else
    ExGsQtDevice::makeCurrentContext(getQWidget(), false);

  m_vectPerfData.m_options = (supportMultithreading()) ? eEnableParallelVectorization : 0;
  if (forcePartialUpdateForMt()) m_vectPerfData.m_options |= eForcePartialUpdateForTest;
  mtDeviceSync().setEnabled(m_vectPerfData.enableParallelVectorization());
  if (m_vectPerfData.enableParallelVectorization() && !gsMultithreadEnabled())
    setMaxMtRegenThreads(getMtRegenThreadsCount(userGiContext()));

  ExGsOpenGLVectorizeDeviceBase::update(pRect);
}

void ExGsQtVectorizeDevice::updateScreen()
{
  if (!ExGsQtDevice::makeCurrentContext(getQWidget(), true))
    return;
  OdGsOpenGLVectorizeDevice::updateScreen();
}

ExGsQtVectorizeView::ExGsQtVectorizeView()
  : ExGsOpenGLVectorizeViewBase()
  , m_bSelecting(false)
{
}

//*******************************************************************************/

void ExGsQtVectorizeView::polylineOut(OdInt32 nPoints, const OdGePoint3d* pPoints)
{
  ExGsOpenGLVectorizeViewBase::polylineOut(nPoints, pPoints);
}

void ExGsQtVectorizeView::select(const OdGePoint2d* pts, int nPoints, OdGsSelectionReactor* pReactor, OdGsView::SelectionMode mode)
{
  OdSaveState<bool> ss(m_bSelecting, true);
  ExGsOpenGLVectorizeViewBase::select(pts, nPoints, pReactor, mode);
}

//// OLE support
//#include "ExOwnerDrawDc.cpp" // Kernel/Extensions/ExRender/ExOwnerDrawDc.cpp

void ExGsQtVectorizeView::ownerDrawDc(const OdGePoint3d& origin,
                                      const OdGeVector3d& u,
                                      const OdGeVector3d& v,
                                      const OdGiSelfGdiDrawable* pDrawable,
                                      bool dcAligned, // = true
                                      bool allowClipping) // = false
{
  ExGsOpenGLVectorizeViewBase::ownerDrawDc(origin, u, v, pDrawable, dcAligned, allowClipping);
  //next was moved into OdGsOpenGLVectorizeView::ownerDrawDc for CORE-11600 (Ole object is not drawn on Mac)
  //::exOwnerDrawDc(origin, u, v, pDrawable, (getRenderMode() == k2DOptimized) && dcAligned, allowClipping, *this,
  //                GETBIT(m_flags, kModelCache), GETBIT(m_flags, kModelCache));
}

