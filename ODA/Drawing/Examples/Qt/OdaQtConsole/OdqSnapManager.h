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
// OdqSnapManager.h
//

#ifndef ODA_QT_SNAP_MANAGER_H_
#define ODA_QT_SNAP_MANAGER_H_

#include "OSnapManager.h"

/////////////////////////////////////////////////////////////////////////////// 

typedef OdSmartPtr<class OdqSnapManager> OdqSnapManagerPtr;

class OdqSnapManager : public OdBaseSnapManager
{
  ODRX_DECLARE_MEMBERS(OdqSnapManager);

protected:
  OdRxObject* m_pRxDb; 
  const OdGePoint3d* m_pBasePt;

  // Construction
  OdqSnapManager();
  virtual ~OdqSnapManager();

  void init(OdRxObject* pRxDb);
public:
  static OdqSnapManagerPtr createObject(OdRxObject* pRxDb);
  static bool isSupported(OdRxObject* pRxDb);

  virtual unsigned snapModes() const;
  virtual OdCmEntityColor snapTrueColor() const;

  const OdGePoint3d* basePtRef() const;
  void setBasePtRef(const OdGePoint3d* pBasePt);
};

#endif // ODA_QT_SNAP_MANAGER_H_
