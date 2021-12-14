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

#include "StdAfx.h"
#include "DbCurve.h"
#include "DbEntity.h"
#include "DbJoinEntityPE.h"
#include "DbIdMapping.h"
#include "DbLine.h"
#include "../../Extensions/IdViewObjectEnablers/IdBlockReference.h"
#include "DbDictionary.h"
#include "DbLayout.h"

/////////////////////////////////////////////////////////////////////////
// Base class for Transform commands tracker
/////////////////////////////////////////////////////////////////////////
class TransformTracker : public OdStaticRxObject<OdEdPointTracker>
{
  OdArray<OdDbEntityPtr>  m_ents;   // Copy of selection set entities
  OdDbLinePtr             m_pLine;  // Rubber band line
  OdGeMatrix3d            m_xForm;  // Last transformation

protected:
  OdGePoint3d             m_ptBase;

  // Individual for various Transform commands
  virtual OdGeMatrix3d getTransform(const OdGePoint3d& value) = 0;

public:
  TransformTracker(OdGePoint3d ptBase, OdDbSelectionSet* pSSet)
    : m_ptBase(ptBase)
  {
    m_pLine = OdDbLine::createObject();
    m_pLine->setDatabaseDefaults((OdDbDatabase*)pSSet->database());
    m_pLine->setColorIndex(3);
    m_pLine->setStartPoint(ptBase);

    OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();

    while (!pIter->done())
    {
      OdDbObjectId objId  = pIter->objectId();
      OdDbEntityPtr pEnt  = objId.openObject(OdDb::kForWrite);

      if (!pEnt.isNull())
      {
        OdDbEntityPtr pSubEnt;
        if (pIter->subentCount() == 0)
        {
          pSubEnt = pEnt->clone();
          m_ents.push_back(pSubEnt);
        }
        else
        {
          OdDbFullSubentPath pathSubent;
          OdDbFullSubentPathArray arrPaths;

          for (unsigned int i = 0; i < pIter->subentCount(); i++)
          {
            pIter->getSubentity(i,pathSubent);
            pSubEnt = pEnt->subentPtr(pathSubent);
            if (!pSubEnt.isNull())
              m_ents.push_back(pSubEnt);
          }
        }
        pSubEnt->dragStatus(OdDb::kDragStart);
      }
      pIter->next();
    }
  }

  virtual void setValue(const OdGePoint3d& value)
  {
    m_pLine->setEndPoint(value);
    if (value == m_ptBase)
      return; // skip useless GE errors
    OdGeMatrix3d matNewTransform = getTransform(value);
    // Compensate previous transform
    OdGeMatrix3d xTrans = m_xForm.inverse();
    xTrans.preMultBy(matNewTransform);
    // Remember last transform
    m_xForm = matNewTransform;
    for ( int i = m_ents.size() - 1; i >= 0; --i)
    {
      m_ents[i]->transformBy(xTrans);
    }
  }

  virtual int addDrawables(OdGsView* pView)
  {
    pView->add(m_pLine, 0);
    for ( int i = m_ents.size() - 1; i >= 0; --i)
    {
      pView->add(m_ents[i], 0);
    }
    return 1;
  }

  virtual void removeDrawables(OdGsView* pView)
  {
    for ( int i = m_ents.size() - 1; i >= 0; --i)
    {
      pView->erase(m_ents[i]);
    }
    pView->erase(m_pLine);
  }

};

///////////////////////////////////////////////////////////////////////////////////

void _Move_func(OdEdCommandContext* pCmdCtx)
{
  class MoveTracker : public TransformTracker
  {
  public:
    MoveTracker(OdGePoint3d ptBase, OdDbSelectionSet* pSSet)
      : TransformTracker(ptBase, pSSet)
    {}

    virtual OdGeMatrix3d getTransform(const OdGePoint3d& value)
    {
      OdGeMatrix3d mRet;
      mRet.setTranslation(value - m_ptBase);
      return mRet;
    }
  };

  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSSet = pIO->select(L"Select objects:", 
                                          OdEd::kSelAllowObjects | 
                                          OdEd::kSelAllowSubents |
                                          OdEd::kSelLeaveHighlighted);

  if (pSSet->numEntities())
  {
    OdGePoint3d ptBase   = pIO->getPoint(L"Specify base point:");
    MoveTracker tracker(ptBase, pSSet);
    OdGePoint3d ptOffset = pIO->getPoint(L"Specify second point:", OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand,
                                         0, OdString::kEmpty, &tracker);

    OdGeMatrix3d matOffset = tracker.getTransform(ptOffset);
    OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();

    OdDbDatabasePtr pDb = pDbCmdCtx->database();
    pDb->startTransaction();
    while (!pIter->done())
    {
      OdDbObjectId objId  = pIter->objectId();
      OdDbEntityPtr pEnt  = objId.openObject(OdDb::kForWrite);

      if (!pEnt.isNull())
      {
        if (pIter->subentCount())
        {
          OdDbFullSubentPath pathSubent;
          OdDbFullSubentPathArray arrPaths;

          for (unsigned int i = 0; i < pIter->subentCount(); i++)
          {
            pIter->getSubentity(i,pathSubent);
            arrPaths.push_back(pathSubent);
          }

          pEnt->transformSubentPathsBy(arrPaths, matOffset);
        }
        else
          pEnt->transformBy(matOffset);
      }

      pIter->next();
    }
    pDb->endTransaction();
  }
}


////////////////////////////////////////////////////////////////////////////////////

void _Scale_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSSet = pIO->select(L"Select objects:", 
                                          OdEd::kSelAllowObjects | 
                                          OdEd::kSelAllowSubents |
                                          OdEd::kSelLeaveHighlighted);

  if (pSSet->numEntities())
  {
    OdGePoint3d ptBase = pIO->getPoint(L"Specify base point:");
    double dScale = pIO->getReal(L"Specify scale factor",OdEd::kInpNonZero, 1.0);

    OdGeMatrix3d matOffset;
    matOffset.setToScaling(dScale, ptBase);
    OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();

    OdDbDatabasePtr pDb = pDbCmdCtx->database();
    pDb->startTransaction();
    while (!pIter->done())
    {
      OdDbObjectId objId = pIter->objectId();
      OdDbEntityPtr pEnt = objId.openObject(OdDb::kForWrite);

      if (!pEnt.isNull())
      {
        if (pIter->subentCount())
        {
          OdDbFullSubentPath pathSubent;
          OdDbFullSubentPathArray arrPaths;

          for (unsigned int i = 0; i < pIter->subentCount(); i++)
          {
            pIter->getSubentity(i,pathSubent);
            arrPaths.push_back(pathSubent);
          }

          pEnt->transformSubentPathsBy(arrPaths, matOffset);
        }
        else
          pEnt->transformBy(matOffset);
      }

      pIter->next();
    }
    pDb->endTransaction();
  }
}


/////////////////////////////////////////////////////////////////////////////////

void _Rotate_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSSet = pIO->select(L"Select objects:", 
                                          OdEd::kSelAllowObjects | 
                                          OdEd::kSelAllowSubents |
                                          OdEd::kSelLeaveHighlighted);
  if (pSSet->numEntities())
  {
    OdGePoint3d ptBase = pIO->getPoint(L"Specify base point:");
    double dAngle = pIO->getAngle(L"Specify rotation angle", OdEd::kGanNoZero | OdEd::kGanFromLastPoint | OdEd::kGptRubberBand, OdaPI);

    OdGeMatrix3d matOffset;
    matOffset.setToRotation(dAngle, pDb->getUCSXDIR().crossProduct(pDb->getUCSYDIR()), ptBase);
    OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();

    pDb->startTransaction();
    while (!pIter->done())
    {
      OdDbObjectId objId = pIter->objectId();
      OdDbEntityPtr pEnt = objId.openObject(OdDb::kForWrite);

      if (!pEnt.isNull())
      {
        if (pIter->subentCount())
        {
          OdDbFullSubentPath pathSubent;
          OdDbFullSubentPathArray arrPaths;

          for (OdUInt32 i = 0; i < pIter->subentCount(); i++)
          {
            pIter->getSubentity(i, pathSubent);
            arrPaths.push_back(pathSubent);
          }

          pEnt->transformSubentPathsBy(arrPaths, matOffset);
        }
        else
          pEnt->transformBy(matOffset);
      }
      pIter->next();
    }
    pDb->endTransaction();
  }
}


////////////////////////////////////////////////////////////////////////

void _Mirror_func(OdEdCommandContext* pCmdCtx)
{
  class MirrorTracker : public TransformTracker
  {
    OdGeVector3d m_vNormal;

  public:
    MirrorTracker(OdGePoint3d ptBase, OdDbSelectionSet* pSSet, OdGeVector3d vNormal)
      : TransformTracker(ptBase, pSSet)
      , m_vNormal(vNormal)
    {}

    virtual OdGeMatrix3d getTransform(const OdGePoint3d& value)
    {
      OdGeVector3d vLine(value - m_ptBase);
      OdGePlane plane(m_ptBase, vLine, m_vNormal);
      return OdGeMatrix3d::mirroring(plane);
    }
  };

  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSSet = pIO->select(L"Select objects:", 
                                          OdEd::kSelAllowObjects
                                          /*| OdEd::kSelLeaveHighlighted*/);
  if (pSSet->numEntities())
  {
    OdGePoint3d ptStart = pIO->getPoint(L"Specify first point of mirror line:");
    MirrorTracker tracker(ptStart, pSSet, ExCommandsUtils::ucsNormal(pDb));
    OdGePoint3d ptEnd = pIO->getPoint(L"Specify second point of mirror line:", OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand,
                                         0, OdString::kEmpty, &tracker);
    while (ptStart == ptEnd)
    {
      ptEnd = pIO->getPoint(L"Specify second point of mirror line:", OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand,
                                         0, OdString::kEmpty, &tracker);
    }
    OdGeMatrix3d xform = tracker.getTransform(ptEnd);
    int nPreserveSource = pIO->getKeyword(L"Erase source objects? [Yes/No]",
                                          L"Yes No", 1);

    pDb->startTransaction();

    OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
    OdDbEntityPtr pEnt;
    if (!nPreserveSource)
    { // Transform in place
      while (!pIter->done())
      {
        pEnt = pIter->objectId().openObject(OdDb::kForWrite);
        if (!pEnt.isNull())
        {
          pEnt->transformBy(xform);
        }
        pIter->next();
      }
    }
    else
    { // Clone selected entities
      OdDbObjectIdArray arrIds;
      arrIds.reserve(pSSet->numEntities());
      while (!pIter->done())
      {
        arrIds.append(pIter->objectId());
        pIter->next();
      }
      OdDbObjectId idOwner = arrIds[0].openObject()->ownerId();
      OdDbIdMappingPtr pIdMap = OdDbIdMapping::createObject();
      pDb->deepCloneObjects(arrIds, idOwner, *pIdMap);

      // Transform new entities
      // arrIds contains source entities' Ids
      // IdMap is used to get cloned entities' Ids
      for (unsigned int i = 0; i < arrIds.size(); ++i)
      {
        OdDbIdPair idPair(arrIds[i]);
        pIdMap->compute(idPair);
        pEnt = idPair.value().openObject(OdDb::kForWrite);
        if (!pEnt.isNull())
        {
          pEnt->transformBy(xform);
        }
      }
    }
    pDb->endTransaction();
  }
}

//////////////////////////////////////////////////////////////////////////

void _Join_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSSet = pIO->select(L"Select primary entity:", 
                                          OdEd::kSelSingleEntity | 
                                          OdEd::kSelLeaveHighlighted);

  if (pSSet->numEntities())
  {
    ODA_ASSERT(pSSet->numEntities() == 1);
    OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
    OdDbObjectId objId = pIter->objectId();
    OdDbEntityPtr pPrimary = objId.openObject(OdDb::kForWrite);

    pSSet = pIO->select(L"Select entities to join:", 
      OdEd::kSelAllowObjects | 
      OdEd::kSelLeaveHighlighted);

    OdDbEntityPtrArray aEnt;
    if (pSSet->numEntities())
    {
      pIter = pSSet->newIterator();

      while (!pIter->done())
      {
        OdDbObjectId objId = pIter->objectId();
        OdDbEntityPtr pEnt = objId.openObject(OdDb::kForWrite);

        if (!pEnt.isNull())
        {
          ODA_ASSERT(pIter->subentCount() == 0);
          aEnt.push_back(pEnt);
        }

        pIter->next();
      }

      OdDbJoinEntityPEPtr pJoinPE = OdDbJoinEntityPE::cast(pPrimary);
      if(!pJoinPE.isNull())
      {
        OdGeIntArray aInd;
        OdResult res = pJoinPE->joinEntities(pPrimary, aEnt, aInd);
        if(res == eOk)
        {
          for(unsigned int i = 0; i < aInd.logicalLength(); ++i)
          {
            aEnt[aInd[i]]->erase();
          }
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////
void _Reverse_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbSelectionSetPtr pSSet = pIO->select(L"Select objects:", 
                                          OdEd::kSelAllowObjects | 
                                          OdEd::kSelLeaveHighlighted);

  int nObjects = 0;
  if (pSSet->numEntities())
  {
    OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
    OdDbDatabasePtr pDb = pDbCmdCtx->database();
    pDb->startTransaction();
    bool bOk = true;
    try
    {
      while (!pIter->done())
      {
        OdDbObjectId objId = pIter->objectId();
        OdDbCurvePtr pEnt = OdDbCurve::cast(objId.openObject(OdDb::kForWrite));

        if (!pEnt.isNull())
        {
          if (pEnt->reverseCurve() == eOk)
            ++nObjects;
        }
        pIter->next();
      }
    }
    catch (...)
    {
      bOk = false;
    }

    if (bOk)
    {
      pDb->endTransaction();
      OdString strMessage;
      strMessage.format(L"%d object(s) direction has been reversed.", nObjects);
      pIO->putString(strMessage);
    }
    else
    {
      pDb->abortTransaction();
      pIO->putString(L"Failed.");
    }
  }
}


//////////////////////////////////////////////////////////////////////////
void _ConvertIdBlkRef_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  int nObjects = 0;
  OdUInt32 nKeyword = pIO->getKeyword(L"[All/Selected]:", L"All Selected");
        OdDbDatabasePtr pDb = pDbCmdCtx->database();

  pDb->startTransaction();
  bool bOk = true;
  switch (nKeyword) {
  case 0: // All
    try
    {
      OdDbDictionaryPtr pLayoutDict = pDb->getLayoutDictionaryId().safeOpenObject();
      OdDbDictionaryIteratorPtr pIter = pLayoutDict->newIterator();
      while (!pIter->done())
      {
        OdDbObjectId layoutId = pIter->objectId();
        OdDbLayoutPtr pLayout = layoutId.safeOpenObject();
        OdDbBlockTableRecordPtr pBTR = pLayout->getBlockTableRecordId().safeOpenObject(OdDb::kForWrite);
        if (!pBTR.isNull())
        {
          OdDbObjectIteratorPtr pIter = pBTR->newIterator();
          while (!pIter->done())
          {
            OdSmartPtr<OdIdBlockReference> pId = OdIdBlockReference::cast(pIter->entity());
            if (!pId.isNull())
            {
              if (pId->explodeGeometryToBlock(NULL, NULL) == eOk)
              {
                ++nObjects;
                pId->upgradeOpen();
                pId->erase();
              }
            }
            pIter->step();
          }
        }
        pIter->next();
      }
    }
    catch (...)
    {
       bOk = false;
    }
    break;

  case 1: // Selected
    {
      OdDbSelectionSetPtr pSSet = pIO->select(L"Select objects:", 
                                            OdEd::kSelAllowObjects | 
                                            OdEd::kSelLeaveHighlighted);
      if (pSSet->numEntities())
      {
        OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
        try
        {
          while (!pIter->done())
          {
            OdDbObjectId objId = pIter->objectId();
            OdSmartPtr<OdIdBlockReference> pIdRef = OdIdBlockReference::cast(objId.openObject(OdDb::kForWrite));
            if (!pIdRef.isNull())
            {
              if (pIdRef->explodeGeometryToBlock(NULL, NULL) == eOk)
              {
                ++nObjects;
                pIdRef->erase();
              }
            }
            pIter->next();
          }
        }
        catch (...)
        {
          bOk = false;
        }
      }
    }
  }

  if (bOk)
  {
    pDb->endTransaction();
    OdString strMessage;
    strMessage.format(L"%d object(s) converted.", nObjects);
    pIO->putString(strMessage);
  }
  else
  {
    pDb->abortTransaction();
    pIO->putString(L"Failed.");
  }
}
