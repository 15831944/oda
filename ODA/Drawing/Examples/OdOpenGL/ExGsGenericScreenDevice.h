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

#ifndef ODEXSCREENDEVICE_INCLUDED
#define ODEXSCREENDEVICE_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RxObjectImpl.h"
#include "Gs/Gs.h"
template <class T>
class ExGsGenericScreenDevice : public T
{
public:
  ExGsGenericScreenDevice()
  {
  }
  virtual ~ExGsGenericScreenDevice()
  {
    deleteContext();
  }
  static OdGsDevicePtr createObject()
  {
    return OdRxObjectImpl<ExGsGenericScreenDevice<T>, OdGsDevice>::createObject();
  }
protected:

  void createContext()
  {
    this->m_nColorDepth = T::getDeviceColorDepth();
    T::createContext();
  }

  void deleteContext()
  {
  }

  void createPalette()
  {
    ExGsGenericDevice::createPalette(&this->m_logPalette);
  }

  bool renderToScreen() const
  {
    return true;
  }
};

#endif //ODEXSCREENDEVICE_INCLUDED
