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
#include "DgConeGripPoints.h"
#include "DgCone.h"
#include "RxObjectImpl.h"
//#include "Ge/GeCone3d.h"

OdResult OdDgConeGripPointsPE::getGripPoints(const OdDgElement* pEnt, OdGePoint3dArray& gripPoints)const
{
	unsigned int size = gripPoints.size();
	gripPoints.resize(size + 4);

	if (pEnt->isKindOf(OdDgCone::desc()))
	{
		OdDgConePtr pCone3d = pEnt;

		OdGePoint3d ptCenter1 = pCone3d->getCenter1(),
			        ptCenter2 = pCone3d->getCenter2();
		OdGePoint3d ptRadius1 = ptCenter1,
			        ptRadius2 = ptCenter2;
		ptRadius1.x += pCone3d->getRadius1();
		ptRadius2.x += pCone3d->getRadius2();

		gripPoints[size    ] = ptCenter1;
		gripPoints[size + 1] = ptCenter2;
		gripPoints[size + 2] = ptRadius1;
		gripPoints[size + 3] = ptRadius2;
	}
	return eOk;
}

OdResult OdDgConeGripPointsPE::moveGripPointsAt(OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
	unsigned size = indices.size();

	if (size == 0)
		return eOk;

	if (pEnt->isKindOf(OdDgCone::desc()))
	{
		OdDgConePtr pCone3d = pEnt;

		for (OdUInt32 i = 0; i < indices.length(); i++)
		{
			OdUInt32 index = indices[i];

			switch (index) {
			case 0: //Center1
			{
				OdGePoint3d ptCenter1 = pCone3d->getCenter1();
				ptCenter1 += offset;
				pCone3d->setCenter1(ptCenter1);
			} break;

			case 1: //Center2
			{
				OdGePoint3d ptCenter2 = pCone3d->getCenter2();
				ptCenter2 += offset;
				pCone3d->setCenter2(ptCenter2);
			} break;

			case 2: //Radius point 1
			{
				OdGePoint3d ptRadius1 = pCone3d->getCenter1();
				ptRadius1.x += pCone3d->getRadius1();
				ptRadius1 += offset;
				double dRadius0 = pCone3d->getRadius1(); 
				double dRadius = ptRadius1.distanceTo(pCone3d->getCenter1());
				pCone3d->setRadius1(dRadius);
			} break;

			case 3: //Radius point 2
			{
				OdGePoint3d ptRadius2 = pCone3d->getCenter2();
				ptRadius2.x += pCone3d->getRadius2();
				ptRadius2 += offset;
				double dRadius = ptRadius2.distanceTo(pCone3d->getCenter2());
				pCone3d->setRadius2(dRadius);
			} break;
			}
		}
	}

	return eOk;
}

OdResult OdDgConeGripPointsPE::getStretchPoints(const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints) const
{
	OdResult res = getGripPoints(pEnt, stretchPoints);

	if (res == eOk)
	{
		stretchPoints.resize(stretchPoints.size() - 1);
	}

	return res;
}

OdResult OdDgConeGripPointsPE::moveStretchPointsAt(OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
	return moveGripPointsAt(pEnt, indices, offset);
}

OdResult OdDgConeGripPointsPE::getOsnapPoints(const OdDgElement* pEnt,
	OdDgElement::OsnapMode osnapMode,
	OdGsMarker gsSelectionMark,
	const OdGePoint3d& pickPoint,
	const OdGePoint3d& lastPoint,
	const OdGeMatrix3d& xWorldToEye,
	OdGePoint3dArray& snapPoints) const
{
	OdGePoint3dArray gripPoints;
	OdResult res = getGripPoints(pEnt, gripPoints);
	if (res != eOk || gripPoints.size() < 5)
		return res;

	switch (osnapMode)
	{
	case OdDgElement::kOsModeCen:
	case OdDgElement::kOsModeMid:
	{
		OdGePoint3d ptMidPoint,
		ptCenter1 = gripPoints[0], 
		ptCenter2 =	gripPoints[1];

		ptMidPoint.x = ( ptCenter1.x + ptCenter2.x ) /2;
		ptMidPoint.y = (ptCenter1.y + ptCenter2.y) / 2;
		ptMidPoint.z = (ptCenter1.z + ptCenter2.z) / 2;

		snapPoints.append(ptMidPoint);
	} break;

	case OdDgElement::kOsModeStart:
		snapPoints.append(gripPoints[0]); //center1
		break;

	case OdDgElement::kOsModeEnd:
		snapPoints.append(gripPoints[1]); //center2
		break;

	default:
	break;
	}
	return eOk;
}
