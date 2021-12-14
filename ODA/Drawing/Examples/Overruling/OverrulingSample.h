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
#include "DbObjectOverrule.h"
#include "Ed/EdCommandStack.h"
#include "StaticRxObject.h"

#define OVERRULE_SAMPLE_APPNAME L"OverruleSampleAppname"

class SampleDrawableOverrule : public OdGiDrawableOverrule
{
public:
  virtual bool isApplicable(const OdRxObject* pOverruledSubject) const ODRX_OVERRIDE;
  virtual OdUInt32 setAttributes (const OdGiDrawable* pSubject, OdGiDrawableTraits * traits) ODRX_OVERRIDE;
  virtual bool worldDraw (const OdGiDrawable* pSubject, OdGiWorldDraw * wd) ODRX_OVERRIDE;
};

class SampleTransformOverrule : public OdDbTransformOverrule
{
public:
  virtual bool isApplicable(const OdRxObject* pOverruledSubject) const ODRX_OVERRIDE;
  virtual OdResult explode(const OdDbEntity* pSubject, OdRxObjectPtrArray& entitySet) ODRX_OVERRIDE;
};

class SampleDbObjectOverrule : public OdDbObjectOverrule
{
public:
  virtual bool isApplicable(const OdRxObject* pOverruledSubject) const ODRX_OVERRIDE;
  virtual OdDbObjectPtr deepClone(const OdDbObject* pSubject, OdDbIdMapping& idMap, OdDbObject*, bool bPrimary = true) ODRX_OVERRIDE;
  virtual OdDbObjectPtr wblockClone(const OdDbObject* pSubject, OdDbIdMapping& idMap, OdDbObject*, bool bPrimary = true) ODRX_OVERRIDE;
};

class SampleGeometryOverrule : public OdDbGeometryOverrule
{
public:
  virtual bool isApplicable(const OdRxObject* pOverruledSubject) const ODRX_OVERRIDE;
  virtual OdResult getGeomExtents(const OdDbEntity* pSubject, OdGeExtents3d& extents) ODRX_OVERRIDE;
};

class SampleOverruleCommand : public OdEdCommand
{ 
public:
  const OdString groupName() const;
  const OdString globalName() const;
  void execute(OdEdCommandContext* pCmdCtx);
};

class OverrulingSampleModule : public OdRxModule
{
  OdStaticRxObject<SampleOverruleCommand> m_SampleOverruleCommand;
  OdStaticRxObject<SampleDrawableOverrule> m_SampleDrawableOverrule;
  OdStaticRxObject<SampleTransformOverrule> m_SampleTransformOverrule;
  OdStaticRxObject<SampleDbObjectOverrule> m_SampleDbObjectOverrule;
  OdStaticRxObject<SampleGeometryOverrule> m_SampleGeometryOverrule;
  void initApp();
  void uninitApp();
};
