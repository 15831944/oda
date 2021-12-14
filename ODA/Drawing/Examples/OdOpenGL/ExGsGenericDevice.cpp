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

#include "OdaCommon.h"
#include "OdString.h" // else undefined in rxvariantvalue.h (?)
#include "ExGsGenericDevice.h"
#include "ColorMapping.h"

//*******************************************************************************/
//  Constructor()
//*******************************************************************************/
ExGsGenericDevice::ExGsGenericDevice()
  : m_nColorDepth(24)
  , m_bColorIndexMode(false)
  , m_bDoubleBufferEnabled(true)
{
}

ODRX_BEGIN_DYNAMIC_PROPERTY_MAP(ExGsGenericDevice);
ODRX_END_DYNAMIC_PROPERTY_MAP(ExGsGenericDevice);

//*******************************************************************************/
//  Destructor()
//*******************************************************************************/
ExGsGenericDevice::~ExGsGenericDevice()
{
  deleteContext();
}


//*******************************************************************************/
//  deleteContext()
//*******************************************************************************/
void ExGsGenericDevice::deleteContext()
{
}

//*******************************************************************************/
//  getDeviceColorDepth()
//*******************************************************************************/
int ExGsGenericDevice::getDeviceColorDepth() const
{
  return 0;
}


//*******************************************************************************/
//  createPalette()
//*******************************************************************************/
void ExGsGenericDevice::createPalette(ODGSPALETTE *logPalette)
{
  logPalette->resize(256);

  const ODCOLORREF* p = odcmAcadDarkPalette();
  for (int i = 0; i < 256; ++i, ++p)
  {
    logPalette->setAt(i, *p);
  }

}
