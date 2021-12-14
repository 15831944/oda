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
#ifndef _DbDynBlockReferenceGripPoints_h_Included_
#define _DbDynBlockReferenceGripPoints_h_Included_

#include "DbGripPoints.h"
#include "DynamicBlocks/DbBlockGrips.h"
#include "DbEntityOverrule.h"

class OdDbSBAppData : public OdRxObject
{
public:
  OdDbSBAppData() : m_pContext(0), m_nAttributeIndex(-1){}
  ODRX_DECLARE_MEMBERS(OdDbSBAppData);
  OdDbBlockGripPtr m_pGrip;
  OdDbBlockParameterPtr m_pParameter;
  OdString m_sPropertyName;
  OdStringArray m_sValues;
  OdDbBlockRepresentationContext* m_pContext;
  int m_nAttributeIndex;
};

class OdDbDynBlockReferenceGripPointsOverrule : public OdDbGripOverrule
{
public:
  virtual bool isApplicable(const OdRxObject* pOverruledSubject) const ODRX_OVERRIDE;

  virtual OdResult getGripPoints(
    const OdDbEntity* pEntity,
    OdDbGripDataPtrArray& grips,
    const double curViewUnitSize,
    const int gripSize,
    const OdGeVector3d& curViewDir,
    const int bitFlags ) ODRX_OVERRIDE;

  virtual OdResult moveGripPointsAt(
    OdDbEntity* pEntity,
    const OdDbVoidPtrArray& grips,
    const OdGeVector3d& offset,
    int bitFlags) ODRX_OVERRIDE;
};

#endif // _DbDynBlockReferenceGripPoints_h_Included_
