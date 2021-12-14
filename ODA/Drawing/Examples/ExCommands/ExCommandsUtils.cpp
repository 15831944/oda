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

// ExCommandsUtils.cpp: implementation of the ExCommands utility functions.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "AbstractViewPE.h"
#include "DbViewport.h"

OdDbObjectId ExCommandsUtils::activeBlockTableRecord(const OdDbDatabase* pDb)
{
  OdDbObjectId idSpace = pDb->getActiveLayoutBTRId();
  if(idSpace == pDb->getPaperSpaceId())
  {
    OdDbViewportPtr pVp = pDb->activeViewportId().safeOpenObject();
    if(pVp->number()>1)
      idSpace = pDb->getModelSpaceId();
  }
  return idSpace;
}

OdGeVector3d ExCommandsUtils::ucsNormal(const OdDbDatabase * pDb)
{
  OdGeVector3d vNormal(OdGeVector3d::kZAxis);
  OdDbObjectId idSpace = activeBlockTableRecord(pDb);
  if (idSpace == pDb->getModelSpaceId())
  {
    OdDbObjectId idVp = pDb->activeViewportId();
    OdDbObjectPtr pVp(idVp.openObject());
    OdAbstractViewPEPtr pAVp(pVp);
    if (!pAVp.isNull())
    {
      OdGePoint3d ptOrigin;
      OdGeVector3d vX, vY;
      pAVp->getUcs(pVp, ptOrigin, vX, vY);
      vNormal = vX.crossProduct(vY).normalize();
    }
  }
  return vNormal;
}

////////////////////////////////////////////////////////////////////////////
// Iterator returning entities from current space
// except presenting in Selection Set

void OdDbInvertedSelectionSetIterator::init(OdDbSelectionSet* pSSet)
{
  m_pSSet = pSSet;
  OdDbSelectionSetIteratorPtr pSSIter = pSSet->newIterator();

  OdDbObjectPtr pObj = pSSIter->objectId().openObject();
  if (!pObj.isNull())
  {
    OdDbBlockTableRecordPtr pBlk = OdDbBlockTableRecord::cast(pObj->ownerId().openObject());
    if (!pBlk.isNull())
    {
      m_pBlkIter = pBlk->newIterator();
      checkPos();
    }
  }
}

OdDbObjectId OdDbInvertedSelectionSetIterator::objectId() const
{
  OdDbObjectId idRet;
  if (!m_pBlkIter.isNull())
    idRet = m_pBlkIter->objectId();
  return idRet;
}

OdDbSelectionMethodPtr OdDbInvertedSelectionSetIterator::method() const
{
  return OdDbSelectionMethodPtr();
}

bool OdDbInvertedSelectionSetIterator::done() const
{
  return m_pBlkIter.isNull() || m_pBlkIter->done();
}

bool OdDbInvertedSelectionSetIterator::next()
{
  m_pBlkIter->step();
  checkPos();
  return m_pBlkIter->done();
}

OdUInt32 OdDbInvertedSelectionSetIterator::subentCount() {return 0;}

bool OdDbInvertedSelectionSetIterator::getSubentity(OdUInt32 i, OdDbBaseFullSubentPath& path)
{
  return false;
}

OdDbStub* OdDbInvertedSelectionSetIterator::id() const
{
  return NULL;
}

void OdDbInvertedSelectionSetIterator::checkPos()
{
  while (!m_pBlkIter->done())
  {
    OdDbObjectId idObj = m_pBlkIter->objectId();
    if (!m_pSSet->isMember(idObj))
      break;
    m_pBlkIter->step();
  }
}
