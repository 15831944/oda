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
#include "ExSelectionUtils.h"
//#include "GiContextForDbDatabase.h"
//#include "Gs/GsViewImpl.h"
//#include "SmartClsIdPtr.h"
OdDbFullSubentPath selectedFullSubentPath(OdDbUserIO* pIO, OdDbDatabasePtr pDb, OdString queryStr, OdGePoint3dArray& ptsSelected, OdDbSelectionSetPtr &pSSet, bool isSelectPointExist)
{
  OdGePoint3d pt;
  OdDbSubentId SentId;

  if (!isSelectPointExist)
  {
    pt = pIO->getPoint(queryStr); // ???
    ptsSelected.append(pt);
  }
  else
    pt = ptsSelected.last();

  pSSet = OdDbSelectionSet::select(pDb->activeViewportId(), 1, &pt, OdDbVisualSelection::kPoint,
          OdDbVisualSelection::kEnableSubents | OdDbVisualSelection::kNestedEntities | OdDbVisualSelection::kIncludeViewport);

  OdDbSelectionSetIteratorPtr pIt = pSSet->newIterator();

  //if (pIt->done())
  //{// the case without SubEnt
  //  pSSet = OdDbSelectionSet::select(pDb->activeViewportId(), 1, &pt, OdDbVisualSelection::kPoint,
  //    OdDbVisualSelection::kIncludeViewport);

  //  pIt = pSSet->newIterator();
  //}

  while (!pIt->done())
  {
    OdDbFullSubentPath entPathSub(OdDb::kNullSubentType, -1);

    for (OdUInt32 nSubent = 0; nSubent < pIt->subentCount(); nSubent++)
    {
      pIt->getSubentity(nSubent, entPathSub);
    }

    if (entPathSub.subentId().type() != OdDb::kFaceSubentType)
    {
      if (entPathSub.subentId().index() == -1)
        return OdDbFullSubentPath(pIt->objectId(), SentId);
      else
        return entPathSub;
    }

    pIt->next();
  }
  
  return OdDbFullSubentPath();

#if 0
  OdGsView *pView = _GetGsView_func(pDb);
  if (pView == NULL)
  {
    if (NULL != pIO)
      pIO->putString(OD_T("No GsView available!"));

    return OdDbFullSubentPath();
  }

  OdDbSelectionSetPtr pSSet;
  OdGePoint3d pt;

  if (isSelectPointExist)
  {
    ODA_ASSERT(!ptsSelected.isEmpty());

    if( ptsSelected.isEmpty())
      return OdDbFullSubentPath();

    pt = ptsSelected.last();
    
    //if (pView == NULL)
    //  pView = OdDbSelectionSet::getSelectedView(pDb->activeViewportId());

    pSSet = OdDbSelectionSet::select(pDb->activeViewportId(), 1, &pt,
      OdDbVisualSelection::kPoint);// , OdDbVisualSelection::kEnableSubents);

    //OdDbSelectionSetIteratorPtr pItSelectionSet = pSSet->newIterator();
    //OdDbEntityPtr pEnt;
    //
    //while (!pItSelectionSet->done())
    //{
    //  pEnt = pItSelectionSet->objectId().safeOpenObject();
    //  pItSelectionSet->next();
    //}

    //if (!pEnt.isNull())
    //{
    //  OdDbFullSubentPath baseSubpath(pEnt->objectId(), OdDb::kFaceSubentType, 1);

    //  int yyy = 0;
    //}
  }
  else
  {
    pSSet = pIO->select(queryStr,
      OdEd::kSelAllowSubentsAlways | OdEd::kSelSinglePass,
      0,
      OdString::kEmpty,
      0,
      &ptsSelected
      );


    if (ptsSelected.isEmpty())
      return OdDbFullSubentPath();

    pt = ptsSelected.last();
  }
  
  OdDbObjectIdArray objArray = pSSet->objectIdArray();

  if (ptsSelected.length() == 0 || objArray.length() < 1 )
    return OdDbFullSubentPath();

  //OdDbObjectPtr ptrObj = OdDbObject::cast(objArray.first().openObject());
  pt.transformBy(pView->worldToDeviceMatrix());
  OdGsDCPoint pts[2];
  OdInt32 pb = pDb->getSysVar(OD_T("PICKBOX"))->getInt16();
  _SimplifiedSelectionReactor selRe(false);

  pts[0].x = (long)pt.x - pb;
  pts[0].y = (long)pt.y - pb;
  pts[1].x = (long)pt.x + pb;
  pts[1].y = (long)pt.y + pb;
  pView->select(pts, 2, &selRe);

  OdDbFullSubentPath entPathSub;
  OdDbObjectId viewportId = pDb->activeViewportId();
  OdDbViewportPtr viewportV = OdDbViewport::cast(viewportId.openObject());
  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
  OdDbObjectIdArray objIdArray;
  OdDbSubentId SentId;

  if (pSSet->numEntities() > 0)
  {
    //OdDbObjectPtr pEnt = pIter->objectId().openObject(OdDb::kForRead);
    //if (!pEnt.isNull())
    if (!pIter->done())
    {
      int count = pIter->subentCount();

      for (int i = 0; i < count; i++)
      {
        if (pIter->getSubentity(i, entPathSub))
        {
          SentId = entPathSub.subentId();
        }
      }
    }
  }

  if (selRe.m_pPath)
  {
    const OdGiPathNode *pPath = selRe.m_pPath;

    while (pPath)
    {
      OdDbObjectId persistentId = pPath->persistentDrawableId();
      OdDbEntityPtr pEntInPath = OdDbEntity::cast(persistentId.openObject());

      if (!pEntInPath.isNull())
      {
        //OdDbPolylinePtr pPoliLine = OdDbPolyline::cast(persistentId.openObject());
        //if (!pPoliLine.isNull())
        //{
        //  SentId.setType(OdDb::kEdgeSubentType);
        //}
        objIdArray.insertAt(0, persistentId);
      }

      pPath = pPath->parent();
    }
  }

  if (!viewportV.isNull())
  {
    objIdArray.insertAt(0, viewportId);
  }

  OdDbFullSubentPath path(objIdArray, SentId);

  return path;
#endif
}
