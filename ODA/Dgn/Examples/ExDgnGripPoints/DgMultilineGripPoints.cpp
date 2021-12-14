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
#include "DgMultilineGripPoints.h"
#include "DgMultiline.h"
#include "RxObjectImpl.h"
#include "Ge/GeExtents3d.h"
//#include "Ge/GeMultiline.h"

OdResult OdDgMultilineGripPointsPE::getGripPoints(const OdDgElement* pEnt, OdGePoint3dArray& gripPoints)const
{
	unsigned int size = gripPoints.size();

	if (pEnt->isKindOf(OdDgMultiline::desc()))
	{
		OdDgMultilinePtr pMultiline = pEnt;

		OdUInt32 uPointsCount = pMultiline->getPointsCount();
		gripPoints.resize(size + uPointsCount);

		for (OdUInt32 i = 0; i < uPointsCount; i++)
		{
			OdDgMultilinePoint multiLinePoint;
			pMultiline->getPoint(i, multiLinePoint);
			OdGePoint3d point3d;
			multiLinePoint.getPoint(point3d);
			gripPoints[size + i] = OdGePoint3d(point3d.x, point3d.y, point3d.z);
		}
	}

	return eOk;
}

OdResult OdDgMultilineGripPointsPE::moveGripPointsAt(OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
	unsigned size = indices.size();

	if (size == 0)
		return eOk;

	if (pEnt->isKindOf(OdDgMultiline::desc()))
	{
		OdDgMultilinePtr pMultiline = pEnt;

		OdGePoint3dArray gripPoints;
		OdResult res = getGripPoints(pEnt, gripPoints);

		for (OdUInt32 i = 0; i < indices.length(); i++)
		{

			OdUInt32 index = indices[i];

			OdArray<OdDgMultilinePoint> multiLinePointArray;

			for (OdUInt32 j = 0; j < pMultiline->getPointsCount(); j++)
			{
				OdDgMultilinePoint multiLinePoint;
				pMultiline->getPoint(j, multiLinePoint);
				multiLinePointArray.append(multiLinePoint);
			}

			OdDgMultilinePoint multiLinePoint = multiLinePointArray[index]; //getting modified point
			OdGePoint3d point3d;
			multiLinePoint.getPoint(point3d);

			point3d.x += offset.x;
			point3d.y += offset.y;
			point3d.z += offset.z;

			multiLinePointArray[index].setPoint(point3d); //setting modified point back to array

			pMultiline->removeAllPoints();

			for (OdUInt32 j = 0; j < multiLinePointArray.length(); j++)
			{
				pMultiline->addPoint(multiLinePointArray[j]);	//setting modified points back from array
			}
		}
	}

	return eOk;
}

OdResult OdDgMultilineGripPointsPE::getStretchPoints(const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints) const
{
	OdResult res = getGripPoints(pEnt, stretchPoints);

	if (res == eOk)
	{
		stretchPoints.resize(stretchPoints.size() - 1);
	}

	return res;
}


OdResult OdDgMultilineGripPointsPE::moveStretchPointsAt(OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
	return moveGripPointsAt(pEnt, indices, offset);
}


OdResult OdDgMultilineGripPointsPE::getOsnapPoints(const OdDgElement* pEnt,
	OdDgElement::OsnapMode osnapMode,
	OdGsMarker gsSelectionMark,
	const OdGePoint3d& pickPoint,
	const OdGePoint3d& lastPoint,
	const OdGeMatrix3d& xWorldToEye,
	OdGePoint3dArray& snapPoints) const
{
	OdGePoint3d ptMin, ptMax, ptCenter;

	if (pEnt->isKindOf(OdDgMultiline::desc()))
	{
		OdDgMultilinePtr pMultiline = pEnt;

		OdGeExtents3d extents;

		pMultiline->getGeomExtents(extents);

		ptMin = extents.minPoint();
		ptMax = extents.maxPoint();
		ptCenter = extents.center();

	}


	switch (osnapMode)
	{
	case OdDgElement::kOsModeEnd:
	{
		snapPoints.append(ptMax);
	}
	break;
	case OdDgElement::kOsModeStart:
	{
		snapPoints.append(ptMin);
	}
	break;
	case OdDgElement::kOsModeCen:
	{
		snapPoints.append(ptCenter);
	}
	break;


	default:
	{

	}
	break;
	}

	return eOk;
}
