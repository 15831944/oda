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

#ifndef ODEXGSGENERICDEVICE_INCLUDED
#define ODEXGSGENERICDEVICE_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Gs/GsBaseVectorizeDevice.h"
#include "RxDispatchImpl.h"
#include "RxVariantValue.h"

class ExGsGenericDevice : public OdRxDispatchImpl<>
{
public:
  ExGsGenericDevice();

  virtual int getDeviceColorDepth() const;
  virtual ~ExGsGenericDevice();
  
  ODRX_DECLARE_DYNAMIC_PROPERTY_MAP(ExGsGenericDevice);

  void setDoubleBufferEnabled(bool b) { m_bDoubleBufferEnabled = b; }
  bool doubleBufferEnabled() const { return m_bDoubleBufferEnabled; }

  bool plotStyleMode() const { return m_bPlotStyleMode; }
  void setPlotStyleMode(bool b) { m_bPlotStyleMode = b; }

protected:
  bool      m_bDoubleBufferEnabled;
  int       m_nColorDepth;
  bool      m_bColorIndexMode;
  bool      m_bPlotStyleMode;

protected:
  virtual bool renderToScreen() const { return true; }
  virtual void createContext() = 0;
  virtual void deleteContext();
  virtual void createPalette(ODGSPALETTE *logicalPalette);
};


#endif // ODEXGSGENERICDEVICE_INCLUDED
