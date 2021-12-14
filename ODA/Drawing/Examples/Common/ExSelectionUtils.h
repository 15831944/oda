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
// DbselectionUtils.h: Declarations of the selection utility functions.
//
//////////////////////////////////////////////////////////////////////

#ifndef DBSELECTIONUTILS_INCLUDED
#define DBSELECTIONUTILS_INCLUDED

#include "OdaCommon.h"
#include "DbDatabase.h"
#include "DbViewportTableRecord.h"
#include "DbLayout.h"
#include "DbViewport.h"
#include "DbAttribute.h"
#include "DbBlockReference.h"
#include "Gi/GiPathNode.h"
#include "Gs/GsSelectionReactor.h"
#include "DbBlockTableRecord.h"
#include "Gs/Gs.h"
#include "DbUserIO.h"
#include "DbSubentId.h"
#include "DbPolyline.h"

static OdGsView *_GetGsView_func(OdDbDatabase *pDb)
{
  OdDbBlockTableRecordPtr pId = pDb->getActiveLayoutBTRId().openObject();
  OdDbLayoutPtr pLayout = pId->getLayoutId().openObject();
  OdDbViewportTableRecordPtr pVR = OdDbViewportTableRecord::cast(pLayout->activeViewportId().openObject());
  OdDbViewportPtr pVR2 = OdDbViewport::cast(pLayout->activeViewportId().openObject());
  if (pVR.get())
    return pVR->gsView();
  else if (pVR2.get())
    return pVR2->gsView();
  return NULL;
}

struct _SimplifiedGsPath : public OdGiPathNode
{
  _SimplifiedGsPath *m_pParent;
  OdDbObjectId m_id;
  OdGsMarker m_selMark;
  _SimplifiedGsPath() : m_pParent(NULL), m_selMark(kNullSubentIndex) { }
  ~_SimplifiedGsPath() { if (m_pParent) delete m_pParent; }
  const OdGiPathNode* parent() const { return m_pParent; }
  OdDbStub* persistentDrawableId() const { return m_id; }
  const OdGiDrawable* transientDrawable() const { return NULL; }
  OdGsMarker selectionMarker() const { return m_selMark; }
};

struct _SimplifiedSelectionReactor : public OdGsSelectionReactor
{
  bool m_bSubents;
  _SimplifiedGsPath *m_pPath;
  explicit _SimplifiedSelectionReactor(bool bSubents = false) : m_bSubents(bSubents), m_pPath(NULL) { }
  ~_SimplifiedSelectionReactor() { if (m_pPath) delete m_pPath; }
  bool selected(const OdGiDrawableDesc& pDrawableDesc)
  {
    if (m_pPath)
      return true;
    OdDbObjectIdArray ids;
    const OdGiDrawableDesc *pDD = &pDrawableDesc;
    do
    {
      if (pDD->persistId != NULL)
        ids.push_back(OdDbObjectId(pDD->persistId));
      pDD = pDD->pParent;
    } while (pDD);
    if (ids.empty())
      return false;
    ids.reverse();
    for (OdUInt32 nPath = 0; nPath < ids.size(); nPath++)
    {
      _SimplifiedGsPath *pPath = new _SimplifiedGsPath();
      pPath->m_pParent = m_pPath;
      pPath->m_id = ids[nPath];
      m_pPath = pPath;
    }
    pDrawableDesc.markToBreak(true);
    return true;
  }
  OdUInt32 selected(const OdGiPathNode& pathNode, const OdGiViewport& viewInfo)
  {
    if (!m_bSubents)
      return OdGsSelectionReactor::selected(pathNode, viewInfo);
    if (m_pPath)
      return kBreak;
    OdDbObjectIdArray ids;
    OdGsMarker gsMark = pathNode.selectionMarker();
    const OdGiPathNode *pPN = &pathNode;
    // Attributes is a part of BlockRef's
    if (pPN->persistentDrawableId() && OdDbObjectId(pPN->persistentDrawableId()).safeOpenObject()->isKindOf(OdDbAttribute::desc()) &&
      pPN->parent() && pPN->parent()->persistentDrawableId() && OdDbObjectId(pPN->parent()->persistentDrawableId()).safeOpenObject()->isKindOf(OdDbBlockReference::desc()))
      pPN = pPN->parent();
    //
    do
    {
      if (pPN->persistentDrawableId() != NULL)
        ids.push_back(OdDbObjectId(pPN->persistentDrawableId()));
      pPN = pPN->parent();
    } while (pPN);
    if (ids.empty())
      return kContinue;
    ids.reverse();
    for (OdUInt32 nPath = 0; nPath < ids.size(); nPath++)
    {
      _SimplifiedGsPath *pPath = new _SimplifiedGsPath();
      pPath->m_pParent = m_pPath;
      pPath->m_id = ids[nPath];
      m_pPath = pPath;
    }
    m_pPath->m_selMark = gsMark;
    return kBreak;
  }
};

OdDbFullSubentPath selectedFullSubentPath(OdDbUserIO* pIO, OdDbDatabasePtr pDb, OdString queryStr, OdGePoint3dArray& ptsSelected, OdDbSelectionSetPtr &pSSet, bool isSelectPointExist = false);

#endif

