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
#include "DgArcGripPoints.h"
#include "DgArc.h"
#include "RxObjectImpl.h"
#include "Ge/GeLine2d.h"
#include "Ge/GeLine3d.h"
#include "Ge/GePlane.h"
#include "Ge/GeInterval.h"

//-----------------------------------------------------------------------------------------------------

void getArc2dGripPoints(const OdDgArc2d* pEnt, OdGePoint3dArray& gripPoints)
{
  unsigned int size = gripPoints.size();
  gripPoints.resize(size + 3);

  OdGeEllipArc2d ellipArc2d = pEnt->getEllipArc();

  OdGePoint2d ptTop, ptStart, ptEnd;
  OdGeInterval arcInterval;

  ellipArc2d.getInterval(arcInterval);

  ptTop   = ellipArc2d.evalPoint((arcInterval.lowerBound() + arcInterval.upperBound())/2.0);
  ptStart = ellipArc2d.startPoint();
  ptEnd   = ellipArc2d.endPoint();

  gripPoints[size] = OdGePoint3d(ptTop.x, ptTop.y, 0);
  gripPoints[size + 1] = OdGePoint3d(ptStart.x, ptStart.y, 0);
  gripPoints[size + 2] = OdGePoint3d(ptEnd.x, ptEnd.y, 0);
}

//-----------------------------------------------------------------------------------------------------

void getArc3dGripPoints(const OdDgArc3d* pEnt, OdGePoint3dArray& gripPoints)
{
  unsigned int size = gripPoints.size();
  gripPoints.resize(size + 3);

  OdGeEllipArc3d ellipArc3d = pEnt->getEllipArc();

  OdGePoint3d ptTop, ptStart, ptEnd;
  OdGeInterval arcInterval;

  ellipArc3d.getInterval(arcInterval);

  ptTop   = ellipArc3d.evalPoint((arcInterval.lowerBound() + arcInterval.upperBound())/2.0);
  ptStart = ellipArc3d.startPoint();
  ptEnd   = ellipArc3d.endPoint();

  gripPoints[size]     = ptTop;
  gripPoints[size + 1] = ptStart;
  gripPoints[size + 2] = ptEnd;
}

//-----------------------------------------------------------------------------------------------------

OdResult OdDgArcGripPointsPE::getGripPoints(const OdDgElement* pEnt, OdGePoint3dArray& gripPoints)const
{
	if (pEnt->isKindOf(OdDgArc2d::desc()))
	{
		OdDgArc2dPtr pArc2d = pEnt;
    getArc2dGripPoints( pArc2d, gripPoints );
	}
	else
	{
		OdDgArc3dPtr pArc3d = pEnt;
    getArc3dGripPoints( pArc3d, gripPoints );
	}
	return eOk;
}

//-----------------------------------------------------------------------------------------------------

void move2dArcRadiusCtrlPoint( OdDgArc2d* pArc2d,const OdGeVector3d& offset )
{
  OdGeEllipArc2d fullEllipArc2d = pArc2d->getEllipArc();

  OdGeInterval arcInterval;
  fullEllipArc2d.getInterval(arcInterval);

  OdGePoint2d ptRadiusCtrl = fullEllipArc2d.evalPoint((arcInterval.lowerBound() + arcInterval.upperBound())/2.0);

  ptRadiusCtrl.x += offset.x;
  ptRadiusCtrl.y += offset.y;

  fullEllipArc2d.setAngles(0, Oda2PI);

  OdGePoint2d ptOrigin       = pArc2d->getOrigin();
  double      dPrimaryAxis   = pArc2d->getPrimaryAxis();
  double      dSecondaryAxis = pArc2d->getSecondaryAxis();

  OdGePoint2d ptIsect;

  double dIsectParam = fullEllipArc2d.paramOf(ptRadiusCtrl);

  ptIsect = fullEllipArc2d.evalPoint( dIsectParam );

  double dOldLength = ptIsect.distanceTo( ptOrigin );
  double dNewLength = ptRadiusCtrl.distanceTo( ptOrigin );

  pArc2d->setPrimaryAxis(dPrimaryAxis * dNewLength / dOldLength);
  pArc2d->setSecondaryAxis(dSecondaryAxis * dNewLength / dOldLength);
}

//-----------------------------------------------------------------------------------------------------

void move2dArcStartPoint( OdDgArc2d* pArc2d,const OdGeVector3d& offset )
{
  OdGeEllipArc2d ellipArc2d = pArc2d->getEllipArc();
  OdGePoint2d ptStart = ellipArc2d.startPoint(); 
  ptStart.x += offset.x;
  ptStart.y += offset.y;

  ellipArc2d.setAngles(0, Oda2PI);

  double dEndAngle = pArc2d->getStartAngle() + pArc2d->getSweepAngle();

  double dStartAngle = ellipArc2d.paramOf(ptStart);

  if( dEndAngle < dStartAngle )
  {
    dEndAngle += Oda2PI;
  }

  double dSweepAngle = dEndAngle - dStartAngle;

  if( dSweepAngle > Oda2PI )
  {
    dSweepAngle -= Oda2PI;
  }

  pArc2d->setStartAngle( dStartAngle );
  pArc2d->setSweepAngle( dSweepAngle );
}

//-----------------------------------------------------------------------------------------------------

void move2dArcEndPoint( OdDgArc2d* pArc2d,const OdGeVector3d& offset )
{
  OdGeEllipArc2d ellipArc2d = pArc2d->getEllipArc();

  OdGePoint2d ptSweep = ellipArc2d.endPoint();

  ptSweep.x += offset.x;
  ptSweep.y += offset.y;

  ellipArc2d.setAngles(0, Oda2PI);

  double dEndAngle = ellipArc2d.paramOf(ptSweep);

  if( dEndAngle < pArc2d->getStartAngle() )
  {
    dEndAngle += Oda2PI;
  }

  double dSweepAngle = dEndAngle - pArc2d->getStartAngle();

  pArc2d->setSweepAngle( dSweepAngle );
}

//-----------------------------------------------------------------------------------------------------

void moveArc2dGripPointsAt(OdDgArc2d* pArc2d, const OdIntArray& indices, const OdGeVector3d& offset)
{
  for (OdUInt32 i = 0; i < indices.length(); i++)
  {
    OdUInt32 index = indices[i];

    switch (index)
    {
      case 0: // radius ctrl point
      {
        move2dArcRadiusCtrlPoint( pArc2d, offset );
      } break;

      case 1: //start 
      {
        move2dArcStartPoint( pArc2d, offset );
      } break;

      case 2: //end 
      {
        move2dArcEndPoint( pArc2d, offset );
      } break;
    }
  }
}

//-----------------------------------------------------------------------------------------------------

void move3dArcRadiusCtrlPoint( OdDgArc3d* pArc3d, const OdGeVector3d& offset )
{
  OdGeEllipArc3d fullEllipArc3d = pArc3d->getEllipArc();

  OdGeInterval arcInterval;
  fullEllipArc3d.getInterval(arcInterval);

  OdGePoint3d ptRadiusCtrl = fullEllipArc3d.evalPoint((arcInterval.lowerBound() + arcInterval.upperBound())/2.0);
  ptRadiusCtrl += offset;

  OdGePlane ellipsePlane( pArc3d->getOrigin(), fullEllipArc3d.normal() );

  ptRadiusCtrl = ptRadiusCtrl.project( ellipsePlane, fullEllipArc3d.normal() );

  fullEllipArc3d.setAngles(0, Oda2PI);

  OdGePoint3d ptOrigin       = pArc3d->getOrigin();
  double      dPrimaryAxis   = pArc3d->getPrimaryAxis();
  double      dSecondaryAxis = pArc3d->getSecondaryAxis();

  double dIsectParam = fullEllipArc3d.paramOf(ptRadiusCtrl);
  OdGePoint3d ptIsect = fullEllipArc3d.evalPoint(dIsectParam);

  double dOldLength = ptIsect.distanceTo( ptOrigin );
  double dNewLength = ptRadiusCtrl.distanceTo( ptOrigin );

  pArc3d->setPrimaryAxis(dPrimaryAxis * dNewLength / dOldLength);
  pArc3d->setSecondaryAxis(dSecondaryAxis * dNewLength / dOldLength);
}

//-----------------------------------------------------------------------------------------------------

void move3dArcStartPoint( OdDgArc3d* pArc3d,const OdGeVector3d& offset )
{
  OdGeEllipArc3d ellipArc3d = pArc3d->getEllipArc();
  OdGePoint3d ptStart = ellipArc3d.startPoint();
  ptStart.x += offset.x;
  ptStart.y += offset.y;
  ptStart.z += offset.z;

  ellipArc3d.setAngles(0, Oda2PI);

  double dEndAngle = pArc3d->getStartAngle() + pArc3d->getSweepAngle();

  double dStartAngle = ellipArc3d.paramOf(ptStart);

  if( dEndAngle < dStartAngle )
  {
    dEndAngle += Oda2PI;
  }

  double dSweepAngle = dEndAngle - dStartAngle;

  if( dSweepAngle > Oda2PI )
  {
    dSweepAngle -= Oda2PI;
  }

  pArc3d->setStartAngle( dStartAngle );
  pArc3d->setSweepAngle( dSweepAngle );
}

//-----------------------------------------------------------------------------------------------------

void move3dArcEndPoint( OdDgArc3d* pArc3d,const OdGeVector3d& offset )
{
  OdGeEllipArc3d ellipArc3d = pArc3d->getEllipArc();
  OdGePoint3d ptEnd = ellipArc3d.endPoint();

  ptEnd += offset;

  ellipArc3d.setAngles(0, Oda2PI);

  double dEndAngle = ellipArc3d.paramOf(ptEnd);

  if( dEndAngle < pArc3d->getStartAngle() )
  {
    dEndAngle += Oda2PI;
  }

  double dSweepAngle = dEndAngle - pArc3d->getStartAngle();

  pArc3d->setSweepAngle( dSweepAngle );
}

//-----------------------------------------------------------------------------------------------------

void moveArc3dGripPointsAt(OdDgArc3d* pArc3d, const OdIntArray& indices, const OdGeVector3d& offset)
{
  for (OdUInt32 i = 0; i < indices.length(); i++)
  {
    OdUInt32 index = indices[i];

    switch (index)
    {
      case 0: // radius ctrl point
      {
        move3dArcRadiusCtrlPoint( pArc3d, offset );
      } break;

      case 1: //start 
      {
        move3dArcStartPoint( pArc3d, offset );
      } break;

      case 2: //end 
      {
        move3dArcEndPoint( pArc3d, offset );
      } break;
    }
  }
}

//-----------------------------------------------------------------------------------------------------

OdResult OdDgArcGripPointsPE::moveGripPointsAt(OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
	unsigned size = indices.size();

	if (size == 0)
		return eOk;

	if (pEnt->isKindOf(OdDgArc2d::desc()))
	{
		OdDgArc2dPtr pArc2d = pEnt;
    moveArc2dGripPointsAt( pArc2d, indices, offset );
	}
	else
	{
		OdDgArc3dPtr pArc3d = pEnt;
    moveArc3dGripPointsAt( pArc3d, indices, offset );
	}

	return eOk;
}

OdResult OdDgArcGripPointsPE::getStretchPoints(const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints) const
{
	OdResult res = getGripPoints(pEnt, stretchPoints);

	if (res == eOk)
	{
		stretchPoints.resize(stretchPoints.size() - 1);
	}

	return res;
}

OdResult OdDgArcGripPointsPE::moveStretchPointsAt(OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
	return moveGripPointsAt(pEnt, indices, offset);
}

OdResult OdDgArcGripPointsPE::getOsnapPoints(const OdDgElement* pEnt,
	OdDgElement::OsnapMode osnapMode,
	OdGsMarker gsSelectionMark,
	const OdGePoint3d& pickPoint,
	const OdGePoint3d& lastPoint,
	const OdGeMatrix3d& xWorldToEye,
	OdGePoint3dArray& snapPoints) const
{
	OdGePoint3dArray gripPoints;
	OdResult res = getGripPoints(pEnt, gripPoints);
	if (res != eOk || gripPoints.size() < 3)
		return res;


	switch (osnapMode)
	{
	  case OdDgElement::kOsModeEnd:
	  {
		  snapPoints.append(gripPoints[0]); //origin
	  } break;

    case OdDgElement::kOsModeStart:
    {
        snapPoints.append(gripPoints[1]); //origin
    } break;

	default:
		break;
	}

	return eOk;
}
