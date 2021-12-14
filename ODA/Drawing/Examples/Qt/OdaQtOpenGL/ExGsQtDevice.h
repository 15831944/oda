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
// ExGsQtDevice.h
//

#ifndef ODEXGSQTDEVICE_INCLUDED
#define ODEXGSQTDEVICE_INCLUDED

#include <QGLWidget>
#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
#include <QOpenGLWidget>
#include <QOpenGLContext>
#endif
#include <QPointer>

#include "CommonDeviceProps.h"
#include "OdPlatform.h"

//typedef OdArray<ODCOLORREF, OdMemoryAllocator<ODCOLORREF> > ODGSPALETTE;
#include "Gs/GsBaseVectorizeDevice.h"

/** Description:
  This class contains methods and properties common to Win32 Vectorizer devices.
  Library: Source *code* provided.
  {group:ExRender_Windows_Classes} 
*/
class ExGsQtDevice : public OdCommonDeviceProps
{
public:
  ExGsQtDevice();
  virtual ~ExGsQtDevice();

  ODRX_DECLARE_DYNAMIC_PROPERTY_MAP(ExGsQtDevice);

  /** Description:
    Sets the device context for this Device object.
  */
  QWidget* setQWidget(QWidget* pQWidget)
  {
    if (pQWidget == m_pQWidget)
      return pQWidget;
    QWidget* pWidgetPrev = m_pQWidget;
    m_pQWidget = pQWidget;
    return pWidgetPrev;
  }
  OdIntPtr setQWidget(OdIntPtr ptrQWidget)
  {
    QWidget* pQWidget = qobject_cast<QWidget*>((QObject*) ptrQWidget);
    return (OdIntPtr)(QObject*) setQWidget(pQWidget);
  }
  QWidget* getQWidget() const
  {
    return m_pQWidget;
  }

  QGLWidget* getQGLWidget() const
  {
    return qobject_cast<QGLWidget*>(m_pQWidget);
  }
#if QT_VERSION >= QT_VERSION_CHECK(5,4,0)
  QOpenGLWidget* getQOpenGLWidget() const
  {
    return qobject_cast<QOpenGLWidget*>(m_pQWidget);
  }
#endif
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

protected:
  
  QPointer<QWidget> m_pQWidget; // QGLWidget or QOpenGLWidge since Qt 5.4
  int m_nColorDepth;
  bool m_bColorIndexMode;

protected:
  /** Description:
    Returns true if and only if the vectorization is to be rendered to the screen.
    Note:
    The default implementation of this function always returns true.
  */
  virtual bool renderToScreen() const 
  { 
    return true; 
  }
  /** Description:
    Creates a device context for this Device object.
  */
  virtual void createContext() = 0;
  /** Description:
    Deletes the device context for this Device object.
  */
  virtual void deleteContext() = 0;
  /** Description:
    Creates a logical palette for this Device object.
    Arguments:
    logicalPalette (O) Receives a pointer to the logical palette.
  */
  virtual void createPalette(ODGSPALETTE* logicalPalette);

  /** Description:
    Returns the *color* depth for this Device object.
  */
  virtual int getDeviceColorDepth() const;
};

#endif // ODEXGSQTDEVICE_INCLUDED
