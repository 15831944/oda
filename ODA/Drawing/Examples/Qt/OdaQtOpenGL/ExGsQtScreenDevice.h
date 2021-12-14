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
// ExGsQtScreenDevice.h
//

#ifndef ODEXQTSCREENDEVICE_INCLUDED
#define ODEXQTSCREENDEVICE_INCLUDED

#include "RxObjectImpl.h"
#include "Gs/Gs.h"

#include <QObject>
QT_BEGIN_NAMESPACE
  class QWidget;
QT_END_NAMESPACE

/** {group:ExRender_Windows_Classes} */
template <class T>
class ExGsQtScreenDevice : public T
{
public:
  ExGsQtScreenDevice()
  {
  }
  virtual ~ExGsQtScreenDevice()
  {
    deleteContext();
  }

  /** Description:
    Creates an instance of this Device object.
  */
  static OdGsDevicePtr createObject()
  {
    return OdRxObjectImpl<ExGsQtScreenDevice<T>, OdGsDevice>::createObject();
  }

protected:
  /** Description:
    Creates a context for this Device object.
  */
  void createContext()
  {
    // T:: is needed to fix compiling error on mac xcode
    QWidget* pQWidget = T::getQWidget();
    if(!pQWidget)
      throw OdError(eNotInitializedYet);

    T::m_nColorDepth = T::getDeviceColorDepth();
    T::createContext();
  }

  /** Description:
    Deletes the context for this Device object.
  */
  void deleteContext()
  {
    T::deleteContext();
  }

  /** Description:
    Creates a logical palette for this Device object.
  */ 
  void createPalette()
  {
    T::createPalette(&T::m_logPalette); // ExGsQtDevice::createPalette(&m_logPalette);
  }

  /** Description:
    Returns true if and only if the vectorization is to be rendered to the screen.
    Note:
    The default implementation of this function always returns true.
  */
  bool renderToScreen() const
  {
    return true;
  }
};

#endif // ODEXQTSCREENDEVICE_INCLUDED
