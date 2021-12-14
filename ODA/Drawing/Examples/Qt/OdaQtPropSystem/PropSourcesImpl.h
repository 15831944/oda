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
// PropSourcesImpl.h
//

#ifndef PROP_SOURCES_IMPL_H_
#define PROP_SOURCES_IMPL_H_

#include "OdaCommon.h"
#include "SSet.h"

#include "PropServices.h"

//------------------------------------------------------------------------

class ObjectPropSourceImpl : public OdPropSource
{
protected:
  OdSelectionSetPtr m_pIds;
  OdEdCommandContext* m_pCtx;
  int m_flagsToUse;

public:
  ObjectPropSourceImpl(OdEdCommandContext* pCtx, 
                       int flagsToUseSources = 0, // 0 to get via PROPSRC variable
                       OdSelectionSet* pIds = NULL);
  void setIds(OdSelectionSet* pIds);
  int flagsToUsePropSources();
  virtual OdSelectionSet* ids();
  virtual OdSelectionSetPtr idsByCurrentFilter();

  virtual bool updateXml(OdString& sXml);
  virtual bool setPropertyValue(const OdString& csPath,
                                const OdString& sValue,
                                Action& action);
  virtual bool getPropertyPossibleValues(const OdString& sPath,
                                         OdStringArray& values,
                                         OdStringArray& iconPaths);

  // without transaction
  bool setPropertyValueDirect(const OdString& csPath,
                              const OdString& csValue,
                              Action& action);
private:
  OdStringArray collectClassNames(OdDbStub* id);

  bool substituteValues(OdDbStub* id, OdString& sXmlEntity); // for $(...)
};

//------------------------------------------------------------------------

class ToolPropSourceImpl : public OdPropSource
{
  OdEdCommandContext* m_pCtx;

  OdString m_sDlgKey; 
  class EntityPropSourceImpl* m_pEntitySource;
  
  mutable OdStringArray m_asFilters; 
  mutable OdString m_sCurrFilter;
  mutable int m_numSelEntitiesOfCurrFileter;
  mutable OdSelectionSetPtr m_pFilteredSSet;

public:
  ToolPropSourceImpl(OdEdCommandContext* pCtx);
  // is used for setMergeFilter
  void setEntitySource(const OdString& sDlgKey, EntityPropSourceImpl* pEntitySource);

  virtual bool updateXml(OdString& sXml);
  virtual bool setPropertyValue(const OdString& csPath,
                                const OdString& sValue,
                                Action& action);
  virtual bool getPropertyPossibleValues(const OdString& sPath,
                                         OdStringArray& values,
                                         OdStringArray& iconPaths);

  static OdString entityClassName(OdEdCommandContext* pCtx, OdDbStub* id, bool bCutPrefix = true);
  static OdSelectionSetPtr createSelectionSet(OdRxObject* pRxDatabase);

  const OdStringArray& collectFilters(bool* pbChanged = NULL) const;
  OdString getCurrentFilter(bool bAsMergeFilter = false) const;
  OdSelectionSetPtr selectionSetByCurrentFilter(bool* pbChanged = NULL) const;
};

//------------------------------------------------------------------------

class EntityPropSourceImpl : public ObjectPropSourceImpl
{
  //OdEdCommandContext* m_pCtx;
  ToolPropSourceImpl* m_pToolSource;
public:
  EntityPropSourceImpl(OdEdCommandContext* pCtx,
                       int flagsToUseSources = 0, // 0 to get via PROPSRC variable
                       ToolPropSourceImpl* pToolSource = NULL);
  virtual OdSelectionSet* ids();
  virtual OdSelectionSetPtr idsByCurrentFilter();
};

//------------------------------------------------------------------------

#endif // PROP_SOURCES_IMPL_H_
