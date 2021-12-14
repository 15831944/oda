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
#include "OdaCommon.h"
#include "ExDimAssoc.h"
#include "DbDimension.h"
#include "DbViewport.h"
#include "DbDictionary.h"
#include "ExDimAssocPE.h"
#include "DbIdMapping.h"

OdExDimAssoc::TrReactor::TrReactor()       
{
  TransLevel = 0;
}

void OdExDimAssoc::TrReactor::transactionEnded(OdDbDatabase* pDb)
{
  if ( pDb->numActiveTransactions() == TransLevel-1 )  
  {        
    pDb->removeTransactionReactor( this );
    //OdExDimAssoc::CheckForAssociativity( DAId );
    TransLevel = 0;
  }      
}

void OdExDimAssoc::TrReactor::setTransactionLevel(OdExDimAssoc *DimAssoc, const OdDbObject* pObject) 
{ 
  if (pObject->isUndoing())
  {
    TransLevel = -2;
    UndoingObjectId = pObject->objectId();
    return;
  }

  switch (TransLevel)
  {
  case 0:

    TransLevel = DimAssoc->database()->numActiveTransactions();

    if ( TransLevel < 0 )
    {
      ODA_FAIL_ONCE();
    }

    if ( 0 == TransLevel )
    {
      dimId = DimAssoc->dimObjId();
      TransLevel = -1;      
    }
    else
    {
      DAId = DimAssoc->objectId();
      DimAssoc->database()->addTransactionReactor( this );
    }
    break;

  case -2:
  case -1:
    if (DimAssoc->database()->numActiveTransactions())
    {
      TransLevel = DimAssoc->database()->numActiveTransactions();

      if ( TransLevel < 0 )
      {
        ODA_FAIL_ONCE();
      }

      DAId = DimAssoc->objectId();
      DimAssoc->database()->addTransactionReactor( this );

    }
    break;        
  }               
}

bool OdExDimAssoc::TrReactor::CheckAssociativityInObjectClose(OdDbObjectId objectId) 
{
  if ( -1 == TransLevel && objectId == dimId)
  {
    TransLevel = 0;
    return true;
  }

  if (-2 == TransLevel && objectId == UndoingObjectId)
  {
    TransLevel = 0;
  }
  return false;
}



ODDB_PSEUDO_DEFINE_MEMBERS(OdExDimAssoc,
  OdDbDimAssoc,
  OdDbDimAssoc,
  DBOBJECT_CONSTR);

OdExDimAssoc::OdExDimAssoc(void)
{
}


OdExDimAssoc::~OdExDimAssoc(void)
{
}

// void OdExDimAssoc::openedForModify(const OdDbObject* pObject)
// {
//   
// }

bool OdExDimAssoc::ViewPortScaleModified(double &newDimlFac) const
{
  OdDbDatabase* pDb     = database();
  OdDbEntityPtr pEnt    = dimObjId().openObject(OdDb::kForRead);
  OdDbDimensionPtr pDim = OdDbDimension::cast(pEnt);

  if (pDim.isNull())
    return false;

  if ( pDim->ownerId() == pDb->getPaperSpaceId() )
  {
    OdDbViewportPtr vViewPort;
    for (int i = 0; i < OdDbDimAssoc::kMaxPointRefs; ++i)
    {
      if ( !pointRef(i).isNull() )
      {
        vViewPort = OdDbViewport::cast( pointRef(i)->mainEntity().objectIds().first().openObject(OdDb::kForRead) );
        break;
      }
    }

    if ( !vViewPort.isNull() )
    {
      if ( !OdEqual( vViewPort->customScale(), ViewPortScale) )
      {
        newDimlFac = 1/vViewPort->customScale();
        return true;
      }
    }
  }

  return false;
}

bool OdExDimAssoc::PointRefPositionModified(int *ptPresent, OdGePoint3d *ptVals) const
{
  bool PosIsModified = false;
  
  for (int i = 0; i < OdExDimAssoc::kMaxPointRefs; ++i)
  {
    if ( ptPresent[i] != ptPresentPrv[i] )
    {
      PosIsModified = true;      
      break;
    }


    if ( ptPresent[i] )
    {
      if ( ! ptVals[i].isEqualTo( ptValsPrv[i]) )
      {
        PosIsModified = true;        
        break;
      }
    }
  }

  return PosIsModified;
}

bool OdExDimAssoc::PointRefPositionModified(int index, OdGePoint3d &point) const
{
  if (index < 0 || index >= OdExDimAssoc::kMaxPointRefs)
    return false;
   
  return ! ptValsPrv[index].isEqualTo( point ) ;      
}

bool OdExDimAssoc::EntityRadiusModified() const
{
  OdDbDimAssocWatcherPEPtr pDimAssocWatcher = OdDbDimAssocWatcherPE::cast( this );
  if ( !pDimAssocWatcher.isNull() )
  {
    ExDimAssocWatcherPE *pWatcher = (ExDimAssocWatcherPE *)pDimAssocWatcher.get();
    double r = 0.0;
    if ( !pDimAssocWatcher.isNull() && !pointRef( 1 ).isNull() )
      pWatcher->GetEntityRadius( pointRef( 1 )->mainEntity(), r );

    return ! OdEqual( EntityRadius, r );
  }
  return false;
}

void OdExDimAssoc::objectClosed(const OdDbObjectId& objectId)
{
	const OdDbObjectPtr pEnt = objectId.openObject();

	if (!pEnt.isNull() && pEnt->isUndoing())
		return;
	
	if (isOdDbObjectIdsInFlux() || !isReallyClosing() || isUndoing())
		return;

  if ( TransactionReactor.CheckAssociativityInObjectClose( objectId ) )
  {
    OdDbDimAssocWatcherPEPtr pDimAssocWatcher = OdDbDimAssocWatcherPE::cast( this );
    if (pDimAssocWatcher.isNull())
    {
      return;
    }

    ExDimAssocWatcherPE *pWatcher = (ExDimAssocWatcherPE *)pDimAssocWatcher.get();
    pWatcher->CheckForAssociativity( this );
  }
}

void OdExDimAssoc::CheckForAssociativity(OdDbObjectId DimAssocId)
{
  OdExDimAssocPtr pExDimAssoc = OdExDimAssoc::cast( DimAssocId.safeOpenObject(OdDb::kForWrite) );

  if ( pExDimAssoc->isUndoing() )
    return;

  OdDbDimAssocWatcherPEPtr pDimAssocWatcher = OdDbDimAssocWatcherPE::cast( pExDimAssoc );
  if (pDimAssocWatcher.isNull())
  {
    return;
  }

  ExDimAssocWatcherPE *pWatcher = (ExDimAssocWatcherPE *)pDimAssocWatcher.get();
  pWatcher->CheckForAssociativity( pExDimAssoc );
}



