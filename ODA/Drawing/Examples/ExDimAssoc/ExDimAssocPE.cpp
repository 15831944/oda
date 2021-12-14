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
#include "ExDimAssocPE.h"
#include "DbDimAssoc.h"
#include "ExDimAssoc.h"
#include "DbCircle.h"
#include "DbArc.h"
#include "DbCurve.h"
#include "DbPolyline.h"
#include "DbRotatedDimension.h"
#include "DbAlignedDimension.h"
#include "DbRadialDimension.h"
#include "DbDiametricDimension.h"
#include "DbArcDimension.h"
#include "Db2LineAngularDimension.h"
#include "Db3PointAngularDimension.h"
#include "DbRadialDimensionLarge.h"
#include "DbOrdinateDimension.h"
#include "DbLeader.h"
#include "DbMLeader.h"
 
OdDbViewportPtr ExDimAssocWatcherPE::GetViewPort( OdExDimAssoc* pThisEx)
{
	OdDbViewportPtr vViewPort;
	OdDbDatabase* pDb = pThisEx->database();
	OdDbEntityPtr pEnt = pThisEx->dimObjId().openObject(OdDb::kForRead);
	OdDbDimensionPtr pDim = OdDbDimension::cast(pEnt);

	if (!pDim.isNull() && pDim->ownerId() == pDb->getPaperSpaceId())
	{
		for (int i = 0; i < OdDbDimAssoc::kMaxPointRefs; ++i)
		{
			if (!pThisEx->pointRef(i).isNull())
			{
				vViewPort = OdDbViewport::cast(pThisEx->pointRef(i)->mainEntity().objectIds().first().openObject(OdDb::kForRead));
				break;
			}
		}
	}
	
	return vViewPort;
}

void ExDimAssocWatcherPE::openedForModify(OdDbObject* pDA, const OdDbObject* pAssocObj)
{
  OdExDimAssoc* pThisEx = OdExDimAssoc::cast( pDA );

  pThisEx->TransactionReactor.setTransactionLevel( pThisEx, pAssocObj );

  OdDbDatabase* pDb = pThisEx->database();
  OdDbEntityPtr pEnt = pThisEx->dimObjId().openObject( OdDb::kForRead );
  OdDbEntityPtr pDim = OdDbEntity::cast( pEnt );
  
  if (!pDim.isNull() && !pDim->isKindOf(OdDbDimension::desc()) && !pDim->isKindOf(OdDbMLeader::desc()))
    pDim = NULL;

  //save ViewPort state
  OdDbViewportPtr vViewPort;
  
  if ( !pDim.isNull() && pDim->ownerId() == pDb->getPaperSpaceId() )
  {
	  vViewPort = GetViewPort(pThisEx);
    //for ( int i = 0; i < OdDbDimAssoc::kMaxPointRefs; ++i )
    //{
    //  if ( !pThisEx->pointRef( i ).isNull( ) )
    //  {
    //    vViewPort = OdDbViewport::cast( pThisEx->pointRef( i )->mainEntity( ).objectIds( ).first( ).openObject( OdDb::kForRead ) );
    //    break;
    //  }
    //}

    if ( !vViewPort.isNull() )
      pThisEx->ViewPortScale = vViewPort->customScale( );
    else
      pThisEx->ViewPortScale = 0;
  }
  //save pointRef state
  for ( int i = 0; i < OdDbDimAssoc::kMaxPointRefs; ++i )
  {
    if ( !pThisEx->pointRef( i ).isNull( ) )
    {
      pThisEx->ptPresentPrv[i] = true;

	    ODA_ASSERT_ONCE(!pDim.isNull() || !vViewPort.isNull());

	    if (!pDim.isNull() || !vViewPort.isNull())///TODO
	    {
		    OdResult res = pThisEx->pointRef(i)->evalPoint(pThisEx->ptValsPrv[i]);
		    if (res != eOk)
			    break;
	    }
    }
    else
    {
      pThisEx->ptPresentPrv[i] = false;
    }
  }
  //save Entity radius
  OdDbDimAssocWatcherPEPtr pDimAssocWatcher = OdDbDimAssocWatcherPE::cast( this );
  if ( !pDimAssocWatcher.isNull() )
  {
    ExDimAssocWatcherPE *pWatcher = (ExDimAssocWatcherPE *)pDimAssocWatcher.get();
    if ( !pDimAssocWatcher.isNull( ) && !pThisEx->pointRef( 1 ).isNull( ) )
      pWatcher->GetEntityRadius( pThisEx->pointRef( 1 )->mainEntity( ), pThisEx->EntityRadius );
  }
}

void ExDimAssocWatcherPE::copied(OdDbObject* pDA, const OdDbObject* pAssocObj, const OdDbObject* pNewObj)
{
  OdDbDimensionPtr pDim = OdDbDimension::cast( pNewObj);

  if ( ! pDim.isNull() )
  {
    OdExDimAssoc* pThisEx = OdExDimAssoc::cast(pDA);
    
    bool AllPointRefIsNull = true;     
    for (int i = 0; i < OdDbDimAssoc::kMaxPointRefs; i++ )
    {
      if (!pThisEx->pointRef(i).isNull())
      {
        AllPointRefIsNull = false;
        break;
      }
    }

    if (AllPointRefIsNull)
    {   
      OdDbObjectIdArray reactors = pDim->getPersistentReactors();
      int reactorCount = reactors.size();
      for (int i = 0; i < reactorCount; i++)
        pDim->removePersistentReactor( reactors[i] );//TODO
      
      pThisEx->removeAssociativity();
    }
  }
}

void ExDimAssocWatcherPE::erased(OdDbObject* pDA, const OdDbObject* pAssocObj, bool erasing)
{  
  OdDbDimAssoc* pDimAssoc = OdDbDimAssoc::cast(pDA);

  for (int i = 0; i < OdDbDimAssoc::kMaxPointRefs; i++ )
  {
    if (pDimAssoc->pointRef(i).isNull())
      continue;

    if ( pDimAssoc->pointRef(i)->mainEntity().objectIds().contains( pAssocObj->objectId() )
      || pDimAssoc->pointRef(i)->intersectEntity().objectIds().contains( pAssocObj->objectId() ) )
    {
      pDimAssoc->removePointRef( i );
    }
  }
}


void ExDimAssocWatcherPE::modified(OdDbObject *pDimAssoc, const OdDbObject* pModObj)
{
  OdDbDimAssoc* pThis   = OdDbDimAssoc::cast(pDimAssoc);
  OdExDimAssoc* pThisEx = OdExDimAssoc::cast(pDimAssoc);

  OdGePoint3d ptVals[OdDbDimAssoc::kMaxPointRefs];
  int ptPresent[OdDbDimAssoc::kMaxPointRefs];

  if ( OdDbDimension::cast(pModObj).isNull() 
    && OdDbLeader::cast(pModObj).isNull()
    && OdDbMLeader::cast(pModObj).isNull() )
  {      
    OdDbEntityPtr pEnt    = pThisEx->dimObjId().openObject(OdDb::kForWrite);

    for (int i = 0; i < OdDbDimAssoc::kMaxPointRefs; ++i)
    {
      OdDbOsnapPointRef* pPoint = pThisEx->pointRef(i);
      if (pPoint)
      {
        ptPresent[i] = true; 
        OdResult res = pPoint->evalPoint(ptVals[i]);
        if (res != eOk)
          return;
      }
      else
      {
        ptPresent[i] = false; 
      }
    }

    double Dimlflac;  
    if (pThisEx->ViewPortScaleModified(Dimlflac))
    {
      OdDbDimensionPtr pDim = OdDbDimension::cast(pEnt); 
      pDim->setDimlfac( Dimlflac );

      UpdateDimension(pEnt, pThis, ptVals);
    }

    if ( pThisEx->PointRefPositionModified(ptPresent, ptVals) )
    {
      UpdateDimension(pEnt, pThis, ptVals);
    }

    if ( pThisEx->EntityRadiusModified() )
    {
      UpdateDimension(pEnt, pThis, ptVals);
    }
  } 
  
}

void ExDimAssocWatcherPE::CheckForAssociativity(OdDbObject *pDimAssoc)
{
  OdExDimAssoc* pThisEx = OdExDimAssoc::cast(pDimAssoc);

  CheckPointRefAssociativity( pThisEx, pDimAssoc );
}

OdResult ExDimAssocWatcherPE::GetEntityRadius(const OdDbFullSubentPath &m_MainEntity, double &radius)
{
  radius = 0.0;

  if (m_MainEntity.objectIds().length() > 0)// Usual case. See OdDbXrefFullSubentPath::dxfOut
  {
    OdDbEntityPtr pMainEnt = OdDbEntity::cast(m_MainEntity.objectIds().last().openObject(OdDb::kForRead));
    if (pMainEnt.isNull())
      return eWasErased;

    OdDbCirclePtr circle = OdDbCircle::cast(pMainEnt);
    OdDbArcPtr arc = OdDbArc::cast(pMainEnt);
    OdDbVertexPtr pVertexEnt = OdDbVertex::cast(pMainEnt);

    if (!circle.isNull())
    {
      radius = circle->radius();
    }
    else if (!arc.isNull())
    {
      radius = arc->radius();
    }
    else if (m_MainEntity.subentId().index() || !pVertexEnt.isNull())
    {
      pMainEnt = pMainEnt->subentPtr(m_MainEntity);

      arc = OdDbArc::cast(pMainEnt);
      circle = OdDbCircle::cast(pMainEnt);

      if (!arc.isNull())
      {
        radius = arc->radius();
      }
      if (!circle.isNull())
      {
        radius = circle->radius();
      }
    }
    else
    {
      return eInvalidInput;
    }
  }

  return eOk;
}

OdResult ExDimAssocWatcherPE::CheckPointRefAssociativity( OdExDimAssoc* pThisEx, OdDbObject* pModObjd )
{ 
  OdDbEntityPtr pModObj    = pThisEx->dimObjId().openObject(OdDb::kForRead);

  OdDbRotatedDimensionPtr pRotDim          = OdDbRotatedDimension::cast(pModObj);
  OdDbAlignedDimensionPtr pAlignDim        = OdDbAlignedDimension::cast(pModObj);
  OdDbRadialDimensionPtr pRadDim           = OdDbRadialDimension::cast(pModObj);
  OdDbDiametricDimensionPtr pDiamDim       = OdDbDiametricDimension::cast(pModObj);
  OdDbArcDimensionPtr pArcDim              = OdDbArcDimension::cast(pModObj);
  OdDb2LineAngularDimensionPtr pAngDim     = OdDb2LineAngularDimension::cast(pModObj);
  OdDb3PointAngularDimensionPtr p3pAngDim  = OdDb3PointAngularDimension::cast(pModObj);
  OdDbRadialDimensionLargePtr pRadDimLarge = OdDbRadialDimensionLarge::cast(pModObj);
  OdDbOrdinateDimensionPtr pOrdDim         = OdDbOrdinateDimension::cast(pModObj);
  OdDbLeaderPtr pLeader                    = OdDbLeader::cast(pModObj);
  OdDbMLeaderPtr pMLeader                  = OdDbMLeader::cast(pModObj);

  OdInt32 nFlags = pThisEx->assocFlag();
  OdGePoint3d ptn3d;

  if (!pRotDim.isNull())
  {
    if (nFlags & OdDbDimAssoc::kFirstPointRef)
    {
      ptn3d = pRotDim->xLine1Point();
      if (pThisEx->PointRefPositionModified( 0, ptn3d ) )
      {
        pThisEx->removePointRef( 0 );
      }
    }

    if (nFlags & OdDbDimAssoc::kSecondPointRef)
    {
      ptn3d = pRotDim->xLine2Point();
      if (pThisEx->PointRefPositionModified( 1, ptn3d ) )
      {
        pThisEx->removePointRef( 1 );
      }
    }
  }
  else if (!pAlignDim.isNull())
  {
    if (nFlags & OdDbDimAssoc::kFirstPointRef)
    {
      ptn3d = pAlignDim->xLine1Point();
      if (pThisEx->PointRefPositionModified( 0, ptn3d ) )
      {
        pThisEx->removePointRef( 0 );
      }
    }

    if (nFlags & OdDbDimAssoc::kSecondPointRef)
    {
      ptn3d = pAlignDim->xLine2Point();
      if (pThisEx->PointRefPositionModified( 1, ptn3d ) )
      {
        pThisEx->removePointRef( 1 );
      }
    }
  }
  else if (!pRadDim.isNull())
  {
    if (nFlags & OdDbDimAssoc::kSecondPointRef)
    {
      ptn3d = pRadDim->center();
      if (pThisEx->PointRefPositionModified( 1, ptn3d ) )
      {
        pThisEx->removePointRef( 1 );
      }
    }
  }
  else if (!pDiamDim.isNull())
  {
    if (nFlags & OdDbDimAssoc::kSecondPointRef)
    {
      ptn3d = OdGeLine3d(pDiamDim->farChordPoint(), pDiamDim->chordPoint() ).evalPoint(0.5);
      if (pThisEx->PointRefPositionModified( 1, ptn3d ) )
      {
        pThisEx->removePointRef( 1 );
      }
    }
  }
  else if (!pArcDim.isNull())
  {
    if (nFlags & OdDbDimAssoc::kFirstPointRef)
    {
      ptn3d = pArcDim->xLine1Point();
      if (pThisEx->PointRefPositionModified( 0, ptn3d ) )
      {
        pThisEx->removePointRef( 0 );
      }
    }

    if (nFlags & OdDbDimAssoc::kSecondPointRef)
    {
      ptn3d = pArcDim->xLine2Point();
      if (pThisEx->PointRefPositionModified( 1, ptn3d ) )
      {
        pThisEx->removePointRef( 1 );
      }
    }

    if (nFlags & OdDbDimAssoc::kThirdPointRef)
    {
      ptn3d = pArcDim->centerPoint();
      if (pThisEx->PointRefPositionModified( 2, ptn3d ) )
      {
        pThisEx->removePointRef( 2 );
      }
    }
  }
  else if (!pAngDim.isNull())
  {
    if (nFlags & OdDbDimAssoc::kFirstPointRef)
    {
      ptn3d = pAngDim->xLine1Start();
      if (pThisEx->PointRefPositionModified( 0, ptn3d ) )
      {
        pThisEx->removePointRef( 0 );
      }
    }

    if (nFlags & OdDbDimAssoc::kSecondPointRef)
    {
      ptn3d = pAngDim->xLine1End();
      if (pThisEx->PointRefPositionModified( 1, ptn3d ) )
      {
        pThisEx->removePointRef( 1 );
      }
    }

    if (nFlags & OdDbDimAssoc::kThirdPointRef)
    {
      ptn3d = pAngDim->xLine2Start();
      if (pThisEx->PointRefPositionModified( 2, ptn3d ) )
      {
        pThisEx->removePointRef( 2 );
      }
    }

    if (nFlags & OdDbDimAssoc::kFourthPointRef)
    {
      ptn3d = pAngDim->xLine2End();
      if (pThisEx->PointRefPositionModified( 3, ptn3d ) )
      {
        pThisEx->removePointRef( 3 );
      }
    }
  }
  else if (!p3pAngDim.isNull())
  {
    if (nFlags & OdDbDimAssoc::kFirstPointRef)
    {
      ptn3d = p3pAngDim->xLine1Point();
      if (pThisEx->PointRefPositionModified( 0, ptn3d ) )
      {
        pThisEx->removePointRef( 0 );
      }
    }

    if (nFlags & OdDbDimAssoc::kSecondPointRef)
    {
      ptn3d = p3pAngDim->xLine2Point();
      if (pThisEx->PointRefPositionModified( 1, ptn3d ) )
      {
        pThisEx->removePointRef( 1 );
      }
    }

    if (nFlags & OdDbDimAssoc::kThirdPointRef)
    {
      ptn3d = p3pAngDim->centerPoint();
      if (pThisEx->PointRefPositionModified( 2, ptn3d ) )
      {
        pThisEx->removePointRef( 2 );
      }
    }
  }
  else if (!pRadDimLarge.isNull())
  {
    if (nFlags & OdDbDimAssoc::kSecondPointRef)
    {
      ptn3d = pRadDimLarge->center();
      if (pThisEx->PointRefPositionModified( 1, ptn3d ) )
      {
        pThisEx->removePointRef( 1 );
      }
    }
  }
  else if (!pOrdDim.isNull())
  {
    if (nFlags & OdDbDimAssoc::kSecondPointRef)
    {
      ptn3d = pOrdDim->definingPoint();
      if (pThisEx->PointRefPositionModified( 1, ptn3d ) )
      {
        pThisEx->removePointRef( 1 );
      }
    }
  }
  else if (!pLeader.isNull())
  {
    if (nFlags & OdDbDimAssoc::kFirstPointRef)
    {
      ptn3d = pLeader->vertexAt(0);
      if (pThisEx->PointRefPositionModified( 0, ptn3d ) )
      {
        pThisEx->removePointRef( 1 );
      }
    }
  }
  else if (!pMLeader.isNull())
  {
    OdGePoint3d VertexPoint;

    if (nFlags & OdDbDimAssoc::kFirstPointRef)
    {
      pMLeader->getVertex(0, 0, VertexPoint);
      if (pThisEx->PointRefPositionModified( 0, VertexPoint ) )
      {
        pThisEx->removePointRef( 0 );
      }
    }

    if (nFlags & OdDbDimAssoc::kSecondPointRef)
    {
      pMLeader->getVertex(1, 0, VertexPoint);
      if (pThisEx->PointRefPositionModified( 1, VertexPoint ) )
      {
        pThisEx->removePointRef( 1 );
      }
    }

    if (nFlags & OdDbDimAssoc::kThirdPointRef)
    {
      pMLeader->getVertex(2, 0, VertexPoint);
      if (pThisEx->PointRefPositionModified( 2, VertexPoint ) )
      {
        pThisEx->removePointRef( 2 );
      }
    }

    if (nFlags & OdDbDimAssoc::kFourthPointRef)
    {
      pMLeader->getVertex(3, 0, VertexPoint);
      if (pThisEx->PointRefPositionModified( 3, VertexPoint ) )
      {
        pThisEx->removePointRef( 3 );
      }
    }
  }

  return eOk;
}

OdResult ExDimAssocWatcherPE::UpdateDimension( OdDbEntityPtr &pEnt, OdDbDimAssoc* pThis, OdGePoint3d * ptVals )
{
  // Points are ready    
  OdDbRotatedDimensionPtr pRotDim          = OdDbRotatedDimension::cast(pEnt);
  OdDbAlignedDimensionPtr pAlignDim        = OdDbAlignedDimension::cast(pEnt);
  OdDbRadialDimensionPtr pRadDim           = OdDbRadialDimension::cast(pEnt);
  OdDbDiametricDimensionPtr pDiamDim       = OdDbDiametricDimension::cast(pEnt);
  OdDbArcDimensionPtr pArcDim              = OdDbArcDimension::cast(pEnt);
  OdDb2LineAngularDimensionPtr pAngDim     = OdDb2LineAngularDimension::cast(pEnt);
  OdDb3PointAngularDimensionPtr p3pAngDim  = OdDb3PointAngularDimension::cast(pEnt);
  OdDbRadialDimensionLargePtr pRadDimLarge = OdDbRadialDimensionLarge::cast(pEnt);
  OdDbOrdinateDimensionPtr pOrdDim         = OdDbOrdinateDimension::cast(pEnt);
  OdDbLeaderPtr pLeader                    = OdDbLeader::cast(pEnt);
  OdDbMLeaderPtr pMLeader                  = OdDbMLeader::cast(pEnt);

  if (!pRotDim.isNull())
  {
    updateDimension(pThis, pRotDim, ptVals);
  }
  else if (!pAlignDim.isNull())
  {
    updateDimension(pThis, pAlignDim, ptVals);
  }
  else if (!pRadDim.isNull())
  {
    updateDimension(pThis, pRadDim, ptVals);
  }
  else if (!pDiamDim.isNull())
  {
    updateDimension(pThis, pDiamDim, ptVals);
  }
  else if (!pArcDim.isNull())
  {
    updateDimension(pThis, pArcDim, ptVals);
  }
  else if (!pAngDim.isNull())
  {
    updateDimension(pThis, pAngDim, ptVals);
  }
  else if (!p3pAngDim.isNull())
  {
    updateDimension(pThis, p3pAngDim, ptVals);
  }
  else if (!pRadDimLarge.isNull())
  {
    updateDimension(pThis, pRadDimLarge, ptVals);
  }
  else if (!pOrdDim.isNull())
  {
    updateDimension(pThis, pOrdDim, ptVals);
  }
  else if (!pLeader.isNull())
  {
    updateDimension(pThis, pLeader, ptVals);
  }
  else if (!pMLeader.isNull())
  {
    updateDimension(pThis, pMLeader, ptVals);
  }

  return eOk;
}

OdResult ExDimAssocWatcherPE::updateDimension(OdDbDimAssoc* pThis, OdDbRotatedDimension* pRotDim, OdGePoint3d* pNewPts, bool /*bUpdate*/)
{
  pThis->assertReadEnabled();
  OdInt32 nFlags = pThis->assocFlag();

  OdGeMatrix3d mWorld2Plane( OdGeMatrix3d::worldToPlane( pRotDim->normal() ) );
  OdGeMatrix3d mPlane2World( OdGeMatrix3d::planeToWorld( pRotDim->normal() ) );
  

  OdGePoint3d xLine1Point = pRotDim->xLine1Point();
  OdGePoint3d xLine2Point = pRotDim->xLine2Point();
  OdGePoint3d DimPoint    = pRotDim->dimLinePoint();
  OdGePoint3d TextPoint   = pRotDim->textPosition();

  xLine1Point.transformBy( mWorld2Plane );
  xLine2Point.transformBy( mWorld2Plane );
  DimPoint.transformBy( mWorld2Plane );
  TextPoint.transformBy( mWorld2Plane );


  OdGePoint3d Line3Point;
  if (pRotDim->isUsingDefaultTextPosition() )
  {    
    Line3Point = DimPoint;     
  }
  else
  {
    Line3Point = TextPoint;
  } 

  int Xpos; //0 - between xLine1Point and xLine2Point,  1 - right(bottom), 2 - left(top)
  int Ypos; // 0 - between xLine1Point and xLine2Point, 1 - top(left), 2 - bottom(right)

  double distX, distY;

  if ( Line3Point.y < xLine1Point.y && Line3Point.y < xLine2Point.y )
  {
    Ypos = 2;
  }
  else if ( Line3Point.y > xLine1Point.y && Line3Point.y > xLine2Point.y )
  {
    Ypos = 1;
  }
  else
  {
    Ypos = 0;
  }

  switch (Ypos)
  {
  case 2:
    distY = odmin( xLine1Point.y, xLine2Point.y ) - Line3Point.y;
    break;
  case 1:
    distY = Line3Point.y - odmax( xLine1Point.y, xLine2Point.y );
    break;
  case 0:
    distY = OdEqual( xLine1Point.y,  xLine2Point.y) ? 0.0 
            : (Line3Point.y - odmin( xLine1Point.y, xLine2Point.y ) ) 
              / fabs(xLine1Point.y - xLine2Point.y);
    break;
  }

  
  if ( Line3Point.x < xLine1Point.x && Line3Point.x < xLine2Point.x )
  {
    Xpos = 2;
  }
  else if ( Line3Point.x > xLine1Point.x && Line3Point.x > xLine2Point.x )
  {
    Xpos = 1;
  }
  else
  {
    Xpos = 0;
  }

  switch (Xpos)
  {
  case 2:
    distX = odmin( xLine1Point.x, xLine2Point.x ) - Line3Point.x;
    break;
  case 1:
    distX = Line3Point.x - odmax( xLine1Point.x, xLine2Point.x );
    break;
  case 0:
    distX = OdEqual(xLine1Point.x ,xLine2Point.x) ? 0.0
            : (Line3Point.x - odmin( xLine1Point.x, xLine2Point.x ) ) 
              / fabs(xLine1Point.x - xLine2Point.x);
    break;
  }

   
  //change dimension
  if (nFlags & OdDbDimAssoc::kFirstPointRef)
  {
    pRotDim->setXLine1Point(pNewPts[0]);
  }

  if (nFlags & OdDbDimAssoc::kSecondPointRef)
  {
    pRotDim->setXLine2Point(pNewPts[1]);
  }



  //calc "dim" point 
  

  xLine1Point = pRotDim->xLine1Point();
  xLine2Point = pRotDim->xLine2Point();
  xLine1Point.transformBy( mWorld2Plane );
  xLine2Point.transformBy( mWorld2Plane );

  OdGePoint3d point;
   
  switch (Ypos)
  {
  case 2:
    point.y = odmin( xLine1Point.y, xLine2Point.y ) - distY; 
    break;

  case 1:
    point.y = odmax( xLine1Point.y, xLine2Point.y ) + distY; 
    break;

  case 0:
    point.y = odmin( xLine1Point.y, xLine2Point.y ) 
            + distY * fabs( xLine1Point.y - xLine2Point.y );
    break;
  }

  switch (Xpos)
  {
  case 2:
    point.x = odmin( xLine1Point.x, xLine2Point.x ) - distX; 
    break;

  case 1:
    point.x = odmax( xLine1Point.x, xLine2Point.x ) + distX; 
    break;

  case 0:
    point.x = odmin( xLine1Point.x, xLine2Point.x ) 
            + distX * fabs( xLine1Point.x - xLine2Point.x );
    break;
  }
 
  point.transformBy( mPlane2World );

  if ( pRotDim->isUsingDefaultTextPosition() ) 
    pRotDim->setDimLinePoint( point );    
  else
    pRotDim->setTextPosition( point );  
      
  return eOk;
}

OdResult ExDimAssocWatcherPE::updateDimension(OdDbDimAssoc* pThis, OdDbAlignedDimension* pAlignDim, OdGePoint3d* pNewPts, bool /*bUpdate*/)
{
  pThis->assertReadEnabled();
  OdInt32 nFlags = pThis->assocFlag();

  OdGeMatrix3d mWorld2Plane( OdGeMatrix3d::worldToPlane( pAlignDim->normal() ) );
  OdGeMatrix3d mPlane2World( OdGeMatrix3d::planeToWorld( pAlignDim->normal() ) );

  OdGePoint3d xLine1Point = pAlignDim->xLine1Point();
  OdGePoint3d xLine2Point = pAlignDim->xLine2Point();
  OdGePoint3d DimPoint    = pAlignDim->dimLinePoint();
  OdGePoint3d TextPoint   = pAlignDim->textPosition();

  xLine1Point.transformBy( mWorld2Plane );
  xLine2Point.transformBy( mWorld2Plane );
  DimPoint.transformBy( mWorld2Plane );
  TextPoint.transformBy( mWorld2Plane );

  /*
  v12 - vector from xLine1Point to xLine2Point
  v2dim - vector from xLine2Point to dimPoint
  v2text - vector from xLine2Point to TextPoint
  v1text - vector from xLine1Point to TextPoint
  */
  OdGeVector2d v12( OdGePoint2d( xLine2Point.x, xLine2Point.y) - OdGePoint2d( xLine1Point.x, xLine1Point.y ) );
  OdGeVector2d v2dim( OdGePoint2d( DimPoint.x, DimPoint.y) - OdGePoint2d( xLine2Point.x, xLine2Point.y ) );
  OdGeVector2d v2text( OdGePoint2d( TextPoint.x, TextPoint.y) - OdGePoint2d( xLine2Point.x, xLine2Point.y ) );
  OdGeVector2d v1text( OdGePoint2d( TextPoint.x, TextPoint.y) - OdGePoint2d( xLine1Point.x, xLine1Point.y ) );

  bool LeftScrew = ( v12.crossProduct( v2dim ) > 0 ) ? true : false;

  int TextPos; //0 - between xLine1Point and xLine2Point, 1 - "right" from xLine2Point, 2 - "left" from xLine1Point
  if ( v1text.crossProduct( v2dim ) < 0)
  {
    TextPos = 2;
  }
  else if ( v2text.crossProduct( v2dim ) > 0)
  {
    TextPos = 1;
  }
  else
  {
    TextPos = 0;
  }
  
  //dim update
  if (nFlags & OdDbDimAssoc::kFirstPointRef)
  {	  
    pAlignDim->setXLine1Point(pNewPts[0]);
  }
  if (nFlags & OdDbDimAssoc::kSecondPointRef)
  {
    pAlignDim->setXLine2Point(pNewPts[1]);
  }

  xLine1Point = pAlignDim->xLine1Point();
  xLine2Point = pAlignDim->xLine2Point();

  xLine1Point.transformBy( mWorld2Plane );
  xLine2Point.transformBy( mWorld2Plane );

  /*
  v12au - vector from xLine1Point to xLine2Point after update
  v2dimau - vector from xLine2Point to dimPoint after update
  */
  OdGeVector2d v12au( OdGePoint2d( xLine2Point.x, xLine2Point.y) - OdGePoint2d( xLine1Point.x, xLine1Point.y ) );
  OdGeVector2d v2dimau = v12au.perpVector().normalize();

  v2dimau = v2dim.length() * v2dimau;
  if (!LeftScrew)
  {
    v2dimau = v2dimau.negate();
  }

  if ( pAlignDim->isUsingDefaultTextPosition() ) 
  {
    DimPoint.x = xLine2Point.x + v2dimau.x;
    DimPoint.y = xLine2Point.y + v2dimau.y;
    DimPoint.transformBy( mPlane2World );
    
    pAlignDim->setDimLinePoint( DimPoint );    
  }
  else
  {    
    /*
    v2textau - vector from xLine2Point to TextPoint after update
    */ 
    OdGeVector2d v2textau;

    double w, h;

    switch (TextPos)
    {
    case 0:
      h = v2text.length() * sin( v12.angleTo( v2text ) );
      w = v2text.length() * cos( v12.angleTo( v2text ) );
      w *= v12au.length() / v12.length();

      v2textau = w * v12au.normalize();
      v2textau = v2textau + h*v2textau.perpVector().normalize().negate();

      TextPoint.x = xLine2Point.x + v2textau.x;
      TextPoint.y = xLine2Point.y + v2textau.y;

      break;
    case 1:
      v2textau = v2text.rotateBy( v12.angleToCCW(v12au) );
      TextPoint.x = xLine2Point.x + v2textau.x;
      TextPoint.y = xLine2Point.y + v2textau.y;

      break;
    case 2:
      v2textau = v1text.rotateBy( v12.angleToCCW(v12au) );
      TextPoint.x = xLine1Point.x + v2textau.x;
      TextPoint.y = xLine1Point.y + v2textau.y;
      break;
    }
    


    TextPoint.transformBy( mPlane2World );
    pAlignDim->setTextPosition( TextPoint );
  }  
   
  return eOk;
}

OdResult ExDimAssocWatcherPE::updateDimension(OdDbDimAssoc* pThis, OdDbRadialDimension* pRadDim, OdGePoint3d* pNewPts, bool /*bUpdate*/)
{
  pThis->assertReadEnabled();
  OdInt32 nFlags = pThis->assocFlag();

  OdGeMatrix3d mWorld2Plane( OdGeMatrix3d::worldToPlane( pRadDim->normal() ) );
  OdGeMatrix3d mPlane2World( OdGeMatrix3d::planeToWorld( pRadDim->normal() ) );
  
  OdGePoint3d center     = pRadDim->center();
  OdGePoint3d chordPoint = pRadDim->chordPoint();
  OdGePoint3d textPoint  = pRadDim->textPosition();
  
  center.transformBy( mWorld2Plane );
  chordPoint.transformBy( mWorld2Plane );
  textPoint.transformBy( mWorld2Plane );

  OdGeVector2d vCenterChord( OdGePoint2d( chordPoint.x, chordPoint.y) - OdGePoint2d( center.x, center.y) );
  OdGeVector2d vCenterText( OdGePoint2d( textPoint.x, textPoint.y) - OdGePoint2d( center.x, center.y) );
   
  //change dimension
  //if (nFlags & OdDbDimAssoc::kFirstPointRef)
  //{
  //  
  //}
  if (nFlags & OdDbDimAssoc::kSecondPointRef)
  {
    pRadDim->setCenter(pNewPts[1]);
  }
  //if (nFlags & OdDbDimAssoc::kThirdPointRef)
  //{
  //  
  //}
  //if (nFlags & OdDbDimAssoc::kFourthPointRef)
  //{
  //  
  //}

  //calc "dim" point 
  
  center = pRadDim->center();
  center.transformBy( mWorld2Plane );

  double radius;
  GetEntityRadius( pThis->pointRef( 1 )->mainEntity(), radius );
  
  vCenterChord = vCenterChord.normalize() * radius;

  chordPoint.x = center.x + vCenterChord.x;
  chordPoint.y = center.y + vCenterChord.y;

  chordPoint.transformBy( mPlane2World );
  pRadDim->setChordPoint( chordPoint );
 
  if ( !pRadDim->isUsingDefaultTextPosition() )
  {
    textPoint.x = center.x + vCenterText.x;
    textPoint.y = center.y + vCenterText.y;

    textPoint.transformBy( mPlane2World );
    pRadDim->setTextPosition( textPoint );
  }   
      
  return eOk;
}

OdResult ExDimAssocWatcherPE::updateDimension(OdDbDimAssoc* pThis, OdDbDiametricDimension* pDiamDim, OdGePoint3d* pNewPts, bool /*bUpdate*/)
{
  pThis->assertReadEnabled();
  OdInt32 nFlags = pThis->assocFlag();  
  if (!(nFlags & OdDbDimAssoc::kSecondPointRef))
  {
    return eInvalidInput;
  }

  OdGeMatrix3d mWorld2Plane( OdGeMatrix3d::worldToPlane( pDiamDim->normal() ) );
  OdGeMatrix3d mPlane2World( OdGeMatrix3d::planeToWorld( pDiamDim->normal() ) );

  OdGePoint3d center     = OdGeLine3d(pDiamDim->farChordPoint(), pDiamDim->chordPoint() ).evalPoint(0.5);
  OdGePoint3d chordPoint = pDiamDim->chordPoint();
  OdGePoint3d textPoint  = pDiamDim->textPosition();

  center.transformBy( mWorld2Plane );
  chordPoint.transformBy( mWorld2Plane );
  textPoint.transformBy( mWorld2Plane );
  
  OdGeVector2d vCenterChord( OdGePoint2d( chordPoint.x, chordPoint.y) - OdGePoint2d( center.x, center.y) );
  OdGeVector2d vCenterText( OdGePoint2d( textPoint.x, textPoint.y) - OdGePoint2d( center.x, center.y) );

  //change dimension
  //if (nFlags & OdDbDimAssoc::kFirstPointRef)
  //{
  //  
  //}
  //if (nFlags & OdDbDimAssoc::kSecondPointRef)
  //{
  //  
  //}
  //if (nFlags & OdDbDimAssoc::kThirdPointRef)
  //{
  // 
  //}
  //if (nFlags & OdDbDimAssoc::kFourthPointRef)
  //{
  //  
  //}

  //calc "dim" point 

  double radius;
  GetEntityRadius(pThis->pointRef(1)->mainEntity(), radius);

  vCenterChord = vCenterChord.normalize() * radius; // *2.0;

  if ( !pThis->pointRef(0).isNull() && !pThis->pointRef(1).isNull())
  {
    // two points on 
    OdGePoint3d chordPoint_ = pNewPts[0];
    chordPoint_.transformBy(mPlane2World);
    pDiamDim->setFarChordPoint(chordPoint_);

    chordPoint_ = pNewPts[1];
    chordPoint_.transformBy(mPlane2World);
    pDiamDim->setChordPoint(chordPoint_);

    center = chordPoint_;
    center.x += vCenterChord.x; 
    center.y += vCenterChord.y;

  } else
  {
    center = pNewPts[1];
    center.transformBy(mWorld2Plane);

    OdGePoint3d FarChordPoint;

    FarChordPoint.x = center.x - vCenterChord.x;
    FarChordPoint.y = center.y - vCenterChord.y;
    FarChordPoint.transformBy(mPlane2World);
    pDiamDim->setFarChordPoint(FarChordPoint);

    chordPoint.x = center.x + vCenterChord.x;
    chordPoint.y = center.y + vCenterChord.y;

    chordPoint.transformBy(mPlane2World);
    pDiamDim->setChordPoint(chordPoint);

    center.x += vCenterChord.x / 2.0;
    center.y += vCenterChord.y / 2.0;

  }

  if ( !pDiamDim->isUsingDefaultTextPosition() )
  {
    textPoint.x = center.x + vCenterText.x;
    textPoint.y = center.y + vCenterText.y;

    textPoint.transformBy( mPlane2World );
    pDiamDim->setTextPosition( textPoint );
  }   

  return eOk;
}

OdResult ExDimAssocWatcherPE::updateDimension(OdDbDimAssoc* pThis, OdDbArcDimension* pArcDim, OdGePoint3d* pNewPts, bool /*bUpdate*/)
{
  pThis->assertReadEnabled();
  OdInt32 nFlags = pThis->assocFlag();

  OdGeMatrix3d mWorld2Plane( OdGeMatrix3d::worldToPlane( pArcDim->normal() ) );
  OdGeMatrix3d mPlane2World( OdGeMatrix3d::planeToWorld( pArcDim->normal() ) );

  OdGePoint3d center      = pArcDim->centerPoint();
  OdGePoint3d xLinePoint1 = pArcDim->xLine1Point();
  OdGePoint3d xLinePoint2 = pArcDim->xLine2Point();
  OdGePoint3d arcPoint    = pArcDim->arcPoint();
  OdGePoint3d textPoint   = pArcDim->textPosition();

  center.transformBy( mWorld2Plane );
  xLinePoint1.transformBy( mWorld2Plane );
  xLinePoint2.transformBy( mWorld2Plane );
  arcPoint.transformBy( mWorld2Plane );
  textPoint.transformBy( mWorld2Plane );

  OdGeVector2d vCenterArcpoint = OdGePoint2d( arcPoint.x, arcPoint.y) 
                               - OdGePoint2d( center.x, center.y);

  OdGeVector2d vCenterText = OdGePoint2d( textPoint.x, textPoint.y) 
    - OdGePoint2d( center.x, center.y);
    
  //change dimension
  if (nFlags & OdDbDimAssoc::kFirstPointRef)
  {
  pArcDim->setXLine1Point( pNewPts[0] ); 
  }
  if (nFlags & OdDbDimAssoc::kSecondPointRef)
  {
   pArcDim->setXLine2Point( pNewPts[1] ); 
  }
  if (nFlags & OdDbDimAssoc::kThirdPointRef)
  {
    pArcDim->setCenterPoint( pNewPts[2] );
  }
  //if (nFlags & OdDbDimAssoc::kFourthPointRef)
  //{
  //
  //}

  double radiuas;
  GetEntityRadius( pThis->pointRef( 2 )->mainEntity(), radiuas );

  if ( !pArcDim->isUsingDefaultTextPosition() )
  {
    textPoint = pNewPts[2];
    textPoint.transformBy( mWorld2Plane );

    double r = vCenterText.length() - center.distanceTo(xLinePoint1);
    vCenterText = vCenterText.normalize() * (r + radiuas);

    textPoint.x += vCenterText.x;
    textPoint.y += vCenterText.y;

    textPoint.transformBy( mPlane2World ); 
    pArcDim->setTextPosition( textPoint );   
  }

  arcPoint = pNewPts[2];
  arcPoint.transformBy( mWorld2Plane );   
  
  double r = vCenterArcpoint.length() - center.distanceTo(xLinePoint1);
  
  vCenterArcpoint = vCenterArcpoint.normalize() * (r + radiuas);

  arcPoint.x += vCenterArcpoint.x;
  arcPoint.y += vCenterArcpoint.y;

  arcPoint.transformBy( mPlane2World ); 
  pArcDim->setArcPoint( arcPoint );
   
  return eOk;
}

OdResult ExDimAssocWatcherPE::updateDimension(OdDbDimAssoc* pThis, OdDb2LineAngularDimension* pAngDim, OdGePoint3d* pNewPts, bool /*bUpdate*/)
{
  pThis->assertReadEnabled();
  OdInt32 nFlags = pThis->assocFlag();

  OdGeMatrix3d mWorld2Plane( OdGeMatrix3d::worldToPlane( pAngDim->normal() ) );
  OdGeMatrix3d mPlane2World( OdGeMatrix3d::planeToWorld( pAngDim->normal() ) );

  OdGePoint3d arcPoint    = pAngDim->arcPoint();
  OdGePoint3d xLine1Start = pAngDim->xLine1Start();
  OdGePoint3d xLine1End   = pAngDim->xLine1End();
  OdGePoint3d textPoint   = pAngDim->textPosition();

  arcPoint.transformBy( mWorld2Plane );
  xLine1Start.transformBy( mWorld2Plane );
  xLine1End.transformBy( mWorld2Plane );
  textPoint.transformBy( mWorld2Plane );

  OdGeVector2d v1StartEnd = OdGePoint2d( xLine1End.x, xLine1End.y) 
                          - OdGePoint2d( xLine1Start.x, xLine1Start.y);
  OdGeVector2d v1StartArcpoint = OdGePoint2d( arcPoint.x, arcPoint.y) 
                               - OdGePoint2d( xLine1Start.x, xLine1Start.y);
  OdGeVector2d v1StartText = OdGePoint2d( textPoint.x, textPoint.y) 
                           - OdGePoint2d( xLine1Start.x, xLine1Start.y);
  
  //change dimension
  if (nFlags & OdDbDimAssoc::kFirstPointRef)
  {
    pAngDim->setXLine1Start( pNewPts[0] ); 
  }
  if (nFlags & OdDbDimAssoc::kSecondPointRef)
  {
    pAngDim->setXLine1End( pNewPts[1] );
  }
  if (nFlags & OdDbDimAssoc::kThirdPointRef)
  {
    pAngDim->setXLine2Start( pNewPts[2] ); 
  }
  if (nFlags & OdDbDimAssoc::kFourthPointRef)
  {
    pAngDim->setXLine2End( pNewPts[3] ); 
  }

  xLine1Start = pAngDim->xLine1Start();
  xLine1End = pAngDim->xLine1End();
  xLine1Start.transformBy( mWorld2Plane );
  xLine1End.transformBy( mWorld2Plane );

  OdGeVector2d v1StartEndau = OdGePoint2d( xLine1End.x, xLine1End.y) 
                            - OdGePoint2d( xLine1Start.x, xLine1Start.y);

  v1StartArcpoint = v1StartArcpoint.rotateBy( v1StartEnd.angleToCCW( v1StartEndau ) );

  arcPoint.x = xLine1Start.x + v1StartArcpoint.x;
  arcPoint.y = xLine1Start.y + v1StartArcpoint.y;

  arcPoint.transformBy( mPlane2World );
  pAngDim->setArcPoint( arcPoint );

  if (!pAngDim->isUsingDefaultTextPosition())
  {
    v1StartText = v1StartText.rotateBy( v1StartEnd.angleToCCW( v1StartEndau ) );

    textPoint.x = xLine1Start.x + v1StartText.x;
    textPoint.y = xLine1Start.y + v1StartText.y;

    textPoint.transformBy( mPlane2World );
    pAngDim->setTextPosition( textPoint );
  }

  return eOk;
}

OdResult ExDimAssocWatcherPE::updateDimension(OdDbDimAssoc* pThis, OdDb3PointAngularDimension* pAngDim, OdGePoint3d* pNewPts, bool /*bUpdate*/)
{
  pThis->assertReadEnabled();
  OdInt32 nFlags = pThis->assocFlag();

  OdGeMatrix3d mWorld2Plane( OdGeMatrix3d::worldToPlane( pAngDim->normal() ));
  OdGeMatrix3d mPlane2World( OdGeMatrix3d::planeToWorld( pAngDim->normal() ));

  OdGePoint3d arcPoint    = pAngDim->arcPoint();
  OdGePoint3d xLine1Point = pAngDim->xLine1Point();
  OdGePoint3d xLine2Point = pAngDim->xLine2Point();
  OdGePoint3d center      = pAngDim->centerPoint();
  OdGePoint3d textPoint   = pAngDim->textPosition();

  arcPoint.transformBy( mWorld2Plane );
  xLine1Point.transformBy( mWorld2Plane );
  xLine2Point.transformBy( mWorld2Plane );
  center.transformBy( mWorld2Plane );
  textPoint.transformBy( mWorld2Plane );

  OdGeVector2d v12 = OdGePoint2d( xLine2Point.x, xLine2Point.y) 
    - OdGePoint2d( xLine1Point.x, xLine1Point.y);
  OdGeVector2d v1Arc = OdGePoint2d( arcPoint.x, arcPoint.y) 
    - OdGePoint2d( xLine1Point.x, xLine1Point.y);
  OdGeVector2d v1text = OdGePoint2d( textPoint.x, textPoint.y) 
    - OdGePoint2d( xLine1Point.x, xLine1Point.y); 

  //change dimension
  if (nFlags & OdDbDimAssoc::kFirstPointRef)
  {
    pAngDim->setXLine1Point( pNewPts[0] ); 
  }
  if (nFlags & OdDbDimAssoc::kSecondPointRef)
  {
    pAngDim->setXLine2Point( pNewPts[1] );
  }
  if (nFlags & OdDbDimAssoc::kThirdPointRef)
  {
    pAngDim->setCenterPoint( pNewPts[2] ); 
  }
  //if (nFlags & OdDbDimAssoc::kFourthPointRef)
  //{
  //  
  //}

  xLine1Point = pAngDim->xLine1Point();
  xLine2Point = pAngDim->xLine2Point();
  xLine1Point.transformBy( mWorld2Plane );
  xLine2Point.transformBy( mWorld2Plane );

  OdGeVector2d v12au = OdGePoint2d( xLine2Point.x, xLine2Point.y) 
                     - OdGePoint2d( xLine1Point.x, xLine1Point.y);
  
  v1Arc = v1Arc.rotateBy( v12au.angleToCCW( v12 ) );

  arcPoint.x = xLine1Point.x + v1Arc.x;
  arcPoint.y = xLine1Point.y + v1Arc.y;

  arcPoint.transformBy( mPlane2World );
  pAngDim->setArcPoint( arcPoint );
  
  if (!pAngDim->isUsingDefaultTextPosition())
  {
    v1text = v1text.rotateBy( v12au.angleToCCW( v12 ) );

    textPoint.x = xLine1Point.x + v1text.x;
    textPoint.y = xLine1Point.y + v1text.y;

    textPoint.transformBy( mPlane2World );
    pAngDim->setTextPosition( textPoint );
  }

  return eOk;
}

OdResult ExDimAssocWatcherPE::updateDimension(OdDbDimAssoc* pThis, OdDbRadialDimensionLarge* pRadDim, OdGePoint3d* pNewPts, bool /*bUpdate*/)
{
  pThis->assertReadEnabled();
  OdInt32 nFlags = pThis->assocFlag();

  OdGeMatrix3d mWorld2Plane( OdGeMatrix3d::worldToPlane( pRadDim->normal() ) );
  OdGeMatrix3d mPlane2World( OdGeMatrix3d::planeToWorld( pRadDim->normal() ) );

  OdGePoint3d center         = pRadDim->center();
  OdGePoint3d chordPoint     = pRadDim->chordPoint();
  OdGePoint3d textPoint      = pRadDim->textPosition();
  OdGePoint3d overrideCenter = pRadDim->overrideCenter();
  OdGePoint3d jogPoint       = pRadDim->jogPoint();

  center.transformBy( mWorld2Plane );
  chordPoint.transformBy( mWorld2Plane );
  textPoint.transformBy( mWorld2Plane );
  overrideCenter.transformBy( mWorld2Plane );
  jogPoint.transformBy( mWorld2Plane );

  OdGeVector2d vCenterChord( OdGePoint2d( chordPoint.x, chordPoint.y) - OdGePoint2d( center.x, center.y) );
  OdGeVector2d vCenterText( OdGePoint2d( textPoint.x, textPoint.y) - OdGePoint2d( center.x, center.y) );
  OdGeVector2d vCenterOvCenter( OdGePoint2d( overrideCenter.x, overrideCenter.y) - OdGePoint2d( center.x, center.y) );
  OdGeVector2d vCenterJog( OdGePoint2d( jogPoint.x, jogPoint.y) - OdGePoint2d( center.x, center.y) );

  //change dimension
  //if (nFlags & OdDbDimAssoc::kFirstPointRef)
  //{
  //  
  //}
  if (nFlags & OdDbDimAssoc::kSecondPointRef)
  {
    pRadDim->setCenter(pNewPts[1]);
  }
  //if (nFlags & OdDbDimAssoc::kThirdPointRef)
  //{
  //  
  //}
  //if (nFlags & OdDbDimAssoc::kFourthPointRef)
  //{
  //  
  //}

  //calc "dim" point 

  center = pRadDim->center();
  center.transformBy( mWorld2Plane );

  double radius;
  GetEntityRadius( pThis->pointRef( 1 )->mainEntity(), radius );

  vCenterChord = vCenterChord.normalize() * radius;

  chordPoint.x = center.x + vCenterChord.x;
  chordPoint.y = center.y + vCenterChord.y;

  chordPoint.transformBy( mPlane2World );
  pRadDim->setChordPoint( chordPoint );

  //override center
  overrideCenter.x = center.x + vCenterOvCenter.x;
  overrideCenter.y = center.y + vCenterOvCenter.y;

  overrideCenter.transformBy( mPlane2World );
  pRadDim->setOverrideCenter( overrideCenter );

  //jog
  jogPoint.x = center.x + vCenterJog.x;
  jogPoint.y = center.y + vCenterJog.y;

  jogPoint.transformBy( mPlane2World );
  pRadDim->setJogPoint( jogPoint );

  if ( !pRadDim->isUsingDefaultTextPosition() )
  {
    textPoint.x = center.x + vCenterText.x;
    textPoint.y = center.y + vCenterText.y;

    textPoint.transformBy( mPlane2World );
    pRadDim->setTextPosition( textPoint );
  }   

  return eOk;
}

OdResult ExDimAssocWatcherPE::updateDimension(OdDbDimAssoc* pThis, OdDbOrdinateDimension* pOrdDim, OdGePoint3d* pNewPts, bool /*bUpdate*/)
{
  pThis->assertReadEnabled();
  OdInt32 nFlags = pThis->assocFlag();

  OdGeMatrix3d mWorld2Plane( OdGeMatrix3d::worldToPlane( pOrdDim->normal() ) );
  OdGeMatrix3d mPlane2World( OdGeMatrix3d::planeToWorld( pOrdDim->normal() ) );


  OdGePoint3d definingPoint  = pOrdDim->definingPoint();
  OdGePoint3d origin         = pOrdDim->origin();  
  OdGePoint3d leaderEndPoint = pOrdDim->leaderEndPoint();
  
  definingPoint.transformBy( mWorld2Plane );
  origin.transformBy( mWorld2Plane );  
  leaderEndPoint.transformBy( mWorld2Plane );

  OdGeVector2d vDefOr( OdGePoint2d( origin.x, origin.y ) - OdGePoint2d( definingPoint.x, definingPoint.y ) );
  OdGeVector2d vDefLeader( OdGePoint2d( leaderEndPoint.x, leaderEndPoint.y ) - OdGePoint2d( definingPoint.x, definingPoint.y ) );

  //change dimension
  //if (nFlags & OdDbDimAssoc::kFirstPointRef)
  //{
  //  
  //}
  if (nFlags & OdDbDimAssoc::kSecondPointRef)
  {
    pOrdDim->setDefiningPoint( pNewPts[1] );
  }
  //if (nFlags & OdDbDimAssoc::kThirdPointRef)
  //{
  //  
  //}
  //if (nFlags & OdDbDimAssoc::kFourthPointRef)
  //{
  // 
  //}

  //calc "dim" point 

  definingPoint = pOrdDim->definingPoint();
  definingPoint.transformBy( mWorld2Plane );

  if (pOrdDim->isUsingXAxis())
    leaderEndPoint.x = definingPoint.x + vDefLeader.x;
  else
    leaderEndPoint.y = definingPoint.y + vDefLeader.y;

  leaderEndPoint.transformBy( mPlane2World );
  pOrdDim->setLeaderEndPoint( leaderEndPoint );

  return eOk;
}

OdResult ExDimAssocWatcherPE::updateDimension(OdDbDimAssoc* pThis, OdDbLeader* pLeader, OdGePoint3d* pNewPts, bool /*bUpdate*/)
{
  pThis->assertReadEnabled();
  OdInt32 nFlags = pThis->assocFlag();

  //change dimension
  if (nFlags & OdDbDimAssoc::kFirstPointRef)
  {
    pLeader->setVertexAt(0, pNewPts[0]);
  }
  //if (nFlags & OdDbDimAssoc::kSecondPointRef)
  //{
  //
  //}
  //if (nFlags & OdDbDimAssoc::kThirdPointRef)
  //{
  //
  //}
  //if (nFlags & OdDbDimAssoc::kFourthPointRef)
  //{
  //
  //}

  return eOk;
}

OdResult ExDimAssocWatcherPE::updateDimension(OdDbDimAssoc* pThis, OdDbMLeader* pLeader, OdGePoint3d* pNewPts, bool /*bUpdate*/)
{
  pThis->assertReadEnabled();
  OdInt32 nFlags = pThis->assocFlag();

  //change dimension
  if (nFlags & OdDbDimAssoc::kFirstPointRef)
  {
    pLeader->setVertex(0, 0, pNewPts[0]);
  }
  if (nFlags & OdDbDimAssoc::kSecondPointRef)
  {
    pLeader->setVertex(1, 0, pNewPts[1]);
  }
  if (nFlags & OdDbDimAssoc::kThirdPointRef)
  {
    pLeader->setVertex(2, 0, pNewPts[2]);
  }
  if (nFlags & OdDbDimAssoc::kFourthPointRef)
  {
    pLeader->setVertex(3, 0, pNewPts[3]);
  }

  return eOk;
}


