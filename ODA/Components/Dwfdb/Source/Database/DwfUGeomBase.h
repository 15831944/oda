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

#ifndef _DWFUGEOMBASE_INCLUDED_
#define _DWFUGEOMBASE_INCLUDED_

#include "DwfUBase.h"
#include "DwfUUserData.h"

namespace DWFCore
{
  class DWFString;
  class DWFInputStream;
}
class WT_File;

class DwfUGeomBase : public DwfUBase
{
  OdString m_Password;
  OdString m_LayoutName;
  DwfUUserDataImpl m_ud;
protected:
  virtual class DwfUUserData* getUserData();

  virtual DWFCore::DWFString requestedResourceType();

  virtual OdResult proceed6stream(DWFCore::DWFInputStream& stream);
  virtual OdResult proceedXstream(DWFCore::DWFInputStream& stream, WT_File& );
  virtual OdResult proceed55stream(DWFCore::DWFInputStream& stream);

  virtual OdResult loadDwfXStream(DWFCore::DWFInputStream& stream);
  virtual OdResult loadDwf6Stream(DWFCore::DWFInputStream& stream);
  virtual OdResult loadDwf55Stream(DWFCore::DWFInputStream& stream);

public:
  DwfUGeomBase(OdString password, OdString layoutName);

  virtual ~DwfUGeomBase() {}
};

#endif // _DWFUGEOMBASE_INCLUDED_
