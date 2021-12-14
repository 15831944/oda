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
#include "DgElementOverrule.h"
#include "Ed/EdCommandStack.h"
#include "StaticRxObject.h"
#include "DgCommandContext.h"

#define OVERRULE_SAMPLE_APPNAME L"ExDgnOverruleAppname"

class ExDgnSampleDrawableOverrule : public OdGiDrawableOverrule
{
public:
  virtual bool isApplicable(const OdRxObject* pOverruledSubject) const;
  virtual OdUInt32 setAttributes (const OdGiDrawable* pSubject, OdGiDrawableTraits * traits);
  virtual bool worldDraw (const OdGiDrawable* pSubject, OdGiWorldDraw * wd);
};

class ExDgnSampleTransformOverrule : public OdDgTransformOverrule
{
public:
  virtual bool isApplicable(const OdRxObject* pOverruledSubject) const;
  virtual OdResult explode(const OdDgElement* pSubject, OdRxObjectPtrArray& entitySet);
};

class ExDgnSampleDgElementOverrule : public OdDgElementOverrule
{
public:
  virtual bool isApplicable(const OdRxObject* pOverruledSubject) const;
  virtual OdDgElementPtr deepClone(const OdDgElement* pSubject, OdDgIdMapping& idMap);
  virtual OdDgElementPtr wblockClone(const OdDgElement* pSubject, OdDgIdMapping& idMap);
};

class ExDgnSampleGeometryOverrule : public OdDgGeometryOverrule
{
public:
  virtual bool isApplicable(const OdRxObject* pOverruledSubject) const;
  virtual OdResult getGeomExtents(const OdDgElement* pSubject, OdGeExtents3d& extents);
};

class ExDgnSampleOverruleCommand : public OdEdCommand
{ 
public:
  const OdString groupName() const;
  const OdString globalName() const;
  void execute(OdEdCommandContext* pCmdCtx);
};

class ExDgnOverrulingModule : public OdRxModule
{
  OdStaticRxObject<ExDgnSampleOverruleCommand>    m_SampleOverruleCommand;
  OdStaticRxObject<ExDgnSampleDrawableOverrule>   m_SampleDrawableOverrule;
  OdStaticRxObject<ExDgnSampleTransformOverrule>  m_SampleTransformOverrule;
  OdStaticRxObject<ExDgnSampleDgElementOverrule>  m_SampleDbObjectOverrule;
  OdStaticRxObject<ExDgnSampleGeometryOverrule>   m_SampleGeometryOverrule;
  void initApp();
  void uninitApp();
};
