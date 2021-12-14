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
#include "DgEllipseGripPoints.h"
#include "DgEllipse.h"
#include "RxObjectImpl.h"
#include "Ge/GeEllipArc2d.h"
#include "Ge/GePlane.h"

OdResult OdDgEllipseGripPointsPE::getGripPoints(const OdDgElement* pEnt, OdGePoint3dArray& gripPoints)const
{
	unsigned int size = gripPoints.size();
	gripPoints.resize(size + 3);

	if (pEnt->isKindOf(OdDgEllipse2d::desc()))
	{
		OdDgEllipse2dPtr pEllipse2d = pEnt;

		OdGePoint2d ptOrigin = pEllipse2d->getOrigin();
		OdGePoint2d ptTop, ptLeft;

		double primaryR = pEllipse2d->getPrimaryAxis();
		double secondaryR = pEllipse2d->getSecondaryAxis();

		double dRotation = pEllipse2d->getRotationAngle();

		ptTop = pEllipse2d->getEllipArc().evalPoint(OdaPI2);
    ptLeft = pEllipse2d->getEllipArc().evalPoint(0);

		gripPoints[size] = OdGePoint3d(ptOrigin.x, ptOrigin.y, 0);
		gripPoints[size + 1] = OdGePoint3d(ptLeft.x, ptLeft.y, 0);
    gripPoints[size + 2] = OdGePoint3d(ptTop.x, ptTop.y, 0);
	}
	else
	{
		OdDgEllipse3dPtr pEllipse3d = pEnt;

		OdGePoint3d ptOrigin;
		pEllipse3d->getOrigin(ptOrigin);

		OdGePoint3d ptTop, ptLeft;

		double primaryR = pEllipse3d->getPrimaryAxis();
		double secondaryR = pEllipse3d->getSecondaryAxis();

		ptTop = pEllipse3d->getEllipArc().evalPoint(OdaPI2);
		ptLeft = pEllipse3d->getEllipArc().evalPoint(0);

		gripPoints[size]     = ptOrigin;
		gripPoints[size + 1] = ptLeft;
		gripPoints[size + 2] = ptTop;
	}
	return eOk;
}

//--------------------------------------------------------------------------------------------------------------

void moveEllipse2dGripPointsAt(OdDgEllipse2d* pEllipse2d, const OdIntArray& indices, const OdGeVector3d& offset)
{
  for (OdUInt32 i = 0; i < indices.length(); i++)
  {
    double dPrimaryAxis   = pEllipse2d->getPrimaryAxis();
    double dSecondaryAxis = pEllipse2d->getSecondaryAxis();
    OdGePoint2d ptOrigin  = pEllipse2d->getOrigin();

    OdUInt32 index = indices[i];

    switch (index)
    {
      case 0: //origin
      {
        ptOrigin.x += offset.x;
        ptOrigin.y += offset.y;
        pEllipse2d->setOrigin(ptOrigin);
      }  break;

      case 1: // major radius
      {
        OdGePoint2d ptMajorRadius = pEllipse2d->getEllipArc().evalPoint(0);
        ptMajorRadius.x += offset.x;
        ptMajorRadius.y += offset.y;

        double dNewPrimaryAxis = ptMajorRadius.distanceTo(ptOrigin);

        OdGeVector2d vrNewPrimaryAxis = ptMajorRadius - ptOrigin;

        pEllipse2d->setRotationAngle(vrNewPrimaryAxis.angle());
        pEllipse2d->setPrimaryAxis(dNewPrimaryAxis);
        pEllipse2d->setSecondaryAxis(dSecondaryAxis * dNewPrimaryAxis / dPrimaryAxis);
      }  break;
      case 2: //minor radius
      {
        OdGePoint2d ptMinorRadius = pEllipse2d->getEllipArc().evalPoint(OdaPI2);
        ptMinorRadius.x += offset.x;
        ptMinorRadius.y += offset.y;

        double dNewSecondaryAxis = ptMinorRadius.distanceTo(ptOrigin);

        OdGeVector2d vrNewSecondaryAxis = ptMinorRadius - ptOrigin;

        pEllipse2d->setRotationAngle(vrNewSecondaryAxis.angle() - OdaPI2);
        pEllipse2d->setPrimaryAxis(dPrimaryAxis * dNewSecondaryAxis / dSecondaryAxis);
        pEllipse2d->setSecondaryAxis(dNewSecondaryAxis);
      }  break;
    }
  }
}

//--------------------------------------------------------------------------------------------------------------

void moveEllipse3dGripPointsAt(OdDgEllipse3d* pEllipse3d, const OdIntArray& indices, const OdGeVector3d& offset)
{
  for (OdUInt32 i = 0; i < indices.length(); i++)
  {
    double dPrimaryAxis   = pEllipse3d->getPrimaryAxis();
    double dSecondaryAxis = pEllipse3d->getSecondaryAxis();
    OdGePoint3d ptOrigin  = pEllipse3d->origin();

    OdUInt32 index = indices[i];

    switch (index)
    {
      case 0: //origin
      {
        ptOrigin += offset;
        pEllipse3d->setOrigin(ptOrigin);
      }  break;

      case 1: // major radius
      {
        OdGeMatrix3d matRotation = pEllipse3d->getRotation().getMatrix();
        OdGeVector3d vrNormal = OdGeVector3d::kZAxis;
        vrNormal = vrNormal.transformBy( matRotation );

        OdGePoint3d ptMajorRadius = pEllipse3d->getEllipArc().evalPoint(0);
        ptMajorRadius += offset;

        OdGePlane ellipsePlane( ptOrigin, vrNormal );
        ptMajorRadius = ptMajorRadius.project( ellipsePlane, vrNormal );

        double dNewPrimaryAxis = ptMajorRadius.distanceTo(ptOrigin);

        OdGeVector3d vrNewPrimaryAxis   = ptMajorRadius - ptOrigin;
        vrNewPrimaryAxis.normalizeGetLength();

        OdGeVector3d vrNewSecondaryAxis = vrNewPrimaryAxis;
        vrNewSecondaryAxis = vrNewSecondaryAxis.rotateBy(OdaPI2, vrNormal);
        vrNewSecondaryAxis.normalizeGetLength();

        matRotation.setCoordSystem( OdGePoint3d::kOrigin, vrNewPrimaryAxis, vrNewSecondaryAxis, vrNormal );

        OdGeQuaternion quatRotation;
        quatRotation.set( matRotation );

        pEllipse3d->setRotation(quatRotation);
        pEllipse3d->setPrimaryAxis(dNewPrimaryAxis);
        pEllipse3d->setSecondaryAxis(dSecondaryAxis * dNewPrimaryAxis / dPrimaryAxis);
      }  break;
      case 2: //minor radius
      {
        OdGeMatrix3d matRotation = pEllipse3d->getRotation().getMatrix();
        OdGeVector3d vrNormal = OdGeVector3d::kZAxis;
        vrNormal = vrNormal.transformBy( matRotation );

        OdGePoint3d ptMinorRadius = pEllipse3d->getEllipArc().evalPoint(OdaPI2);
        ptMinorRadius += offset;

        OdGePlane ellipsePlane( ptOrigin, vrNormal );
        ptMinorRadius = ptMinorRadius.project( ellipsePlane, vrNormal );

        double dNewSecondaryAxis = ptMinorRadius.distanceTo(ptOrigin);

        OdGeVector3d vrNewSecondaryAxis = ptMinorRadius - ptOrigin;
        vrNewSecondaryAxis.normalizeGetLength();

        OdGeVector3d vrNewPrimaryAxis = vrNewSecondaryAxis;
        vrNewPrimaryAxis = vrNewPrimaryAxis.rotateBy(-OdaPI2, vrNormal);
        vrNewPrimaryAxis.normalizeGetLength();

        matRotation.setCoordSystem( OdGePoint3d::kOrigin, vrNewPrimaryAxis, vrNewSecondaryAxis, vrNormal );

        OdGeQuaternion quatRotation;
        quatRotation.set( matRotation );

        pEllipse3d->setRotation(quatRotation);
        pEllipse3d->setPrimaryAxis(dPrimaryAxis * dNewSecondaryAxis / dSecondaryAxis);
        pEllipse3d->setSecondaryAxis(dNewSecondaryAxis);
      }  break;
    }
  }
}

//--------------------------------------------------------------------------------------------------------------

OdResult OdDgEllipseGripPointsPE::moveGripPointsAt(OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
	unsigned size = indices.size();

	if (size == 0)
		return eOk;

	if (pEnt->isKindOf(OdDgEllipse2d::desc()))
	{
		OdDgEllipse2dPtr pEllipse2d = pEnt;
    moveEllipse2dGripPointsAt( pEllipse2d, indices, offset );
	}
	else
	{
		OdDgEllipse3dPtr pEllipse3d = pEnt;
    moveEllipse3dGripPointsAt( pEllipse3d, indices, offset );
	}

	return eOk;
}

OdResult OdDgEllipseGripPointsPE::getStretchPoints(const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints) const
{
	OdResult res = getGripPoints(pEnt, stretchPoints);

	if (res == eOk)
	{
		stretchPoints.resize(stretchPoints.size() - 1);
	}

	return res;
}

OdResult OdDgEllipseGripPointsPE::moveStretchPointsAt(OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
	return moveGripPointsAt(pEnt, indices, offset);
}

OdResult OdDgEllipseGripPointsPE::getOsnapPoints(const OdDgElement* pEnt,
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

	case OdDgElement::kOsModeMid:
	{
		snapPoints.append(gripPoints[0]); //origin
	}
	break;

	case OdDgElement::kOsModeCen:
	{
		snapPoints.append(gripPoints[0]); //origin
	}
	break;

	default:
	{

	}
	break;
	}

	return eOk;
}
