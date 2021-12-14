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

#include "RxModule.h"
#include "DbEntityOverrule.h"
#include "StaticRxObject.h"

class OdDbCmAppData : public OdRxObject
{
public:
  OdDbCmAppData() : nGripIndex(-1), vOrient(0,0,0), vNormal(0, 0, 1), bCenterStretchMode(true){}
  ODRX_DECLARE_MEMBERS(OdDbCmAppData);
  int nGripIndex;                   // grip index (0-3 extension grips, 3-7 overshoot grips, 8 - center grip)
  OdGeVector3d vOrient;             // orientation of grips
  OdGeVector3d vNormal;             // normal to CenterMark plane
  bool bCenterStretchMode;          // center grip mode: true - stretch, false - extension lenght
};

class CenterMarkGripsOverrule : public OdDbGripOverrule
{
public:
  virtual bool isApplicable(const OdRxObject* pOverruledSubject) const ODRX_OVERRIDE;

  virtual OdResult getGripPoints( const OdDbEntity* pSubject, OdGePoint3dArray& gripPoints );

  virtual OdResult getGripPoints(const OdDbEntity* pSubject, 
    OdDbGripDataPtrArray& gripsData,
    const double curViewUnitSize,
    const int gripSize,
    const OdGeVector3d& curViewDir,
    const int bitFlags );

  virtual OdResult moveGripPointsAt(OdDbEntity* pSubject, 
    const OdIntArray& indices,
    const OdGeVector3d& offset );

  virtual OdResult moveGripPointsAt(OdDbEntity* pSubject,
    const OdDbVoidPtrArray& grips,
    const OdGeVector3d& offset,
    int bitFlags );
/*
  virtual OdResult getStretchPoints(const OdDbEntity* pSubject,
    OdGePoint3dArray& stretchPoints);

  virtual OdResult moveStretchPointsAt(OdDbEntity* pSubject,
    const OdIntArray & indices,
    const OdGeVector3d& offset);

   virtual void gripStatus(OdDbEntity* pSubject, const OdDb::GripStat status);
*/
};


class CenterObjectsGripsModule : public OdRxModule
{
  OdStaticRxObject<CenterMarkGripsOverrule> m_CenterMarkGripsOverrule;
  void initApp();
  void uninitApp();

  OdRxModulePtr m_pConstraintsModule;
};
