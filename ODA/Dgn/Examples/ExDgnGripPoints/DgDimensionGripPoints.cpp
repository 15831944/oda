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
#include "DgDimensionGripPoints.h"
#include "DgDimension.h"
#include "RxObjectImpl.h"

OdResult OdDgDimensionGripPointsPE::getGripPoints(const OdDgElement* pEnt, OdGePoint3dArray& gripPoints)const
{
	unsigned int size = gripPoints.size();
	gripPoints.resize(size + 3);

	if (pEnt->isKindOf(OdDgDimension::desc()))
	{
		OdDgDimensionPtr pDimension = pEnt;

		OdUInt32 uPointsCount = pDimension->getPointsCount();
		gripPoints.resize(size + uPointsCount);

		for (OdUInt32 i = 0; i < uPointsCount; i++)
		{
			OdDgDimPoint dimPoint = pDimension->getPoint(i);
			OdGePoint3d point3d;
			dimPoint.getPoint(point3d);
			gripPoints[size + i] = point3d;
		}

	}

	return eOk;
}

OdResult OdDgDimensionGripPointsPE::moveGripPointsAt(OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
	unsigned size = indices.size();

	if (size == 0)
		return eOk;

	if (pEnt->isKindOf(OdDgDimension::desc()))
	{
		OdDgDimensionPtr pDimension = pEnt;

		for (OdUInt32 i = 0; i < indices.length(); i++)
		{
			OdUInt32 index = indices[i];

			OdDgDimPoint dimPoint = pDimension->getPoint(index);
			OdGePoint3d ptVert = dimPoint.getPoint();
			ptVert.x += offset.x;
			ptVert.y += offset.y;
			dimPoint.setPoint(ptVert);
      OdDgAssocPointRootPtr pNullAssoc;
      dimPoint.setDimAssoc(pNullAssoc);
			pDimension->setPoint(index, dimPoint);
		}
	}
	return eOk;
}


OdResult OdDgDimensionGripPointsPE::getStretchPoints(const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints) const
{
	OdResult res = getGripPoints(pEnt, stretchPoints);

	if (res == eOk)
	{
		stretchPoints.resize(stretchPoints.size() - 1);
	}

	return res;
}


OdResult OdDgDimensionGripPointsPE::moveStretchPointsAt(OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
	return moveGripPointsAt(pEnt, indices, offset);
}


OdResult OdDgDimensionGripPointsPE::getOsnapPoints(const OdDgElement* pEnt,
	OdDgElement::OsnapMode osnapMode,
	OdGsMarker gsSelectionMark,
	const OdGePoint3d& pickPoint,
	const OdGePoint3d& lastPoint,
	const OdGeMatrix3d& xWorldToEye,
	OdGePoint3dArray& snapPoints) const
{

	OdGePoint3d ptMin, ptMax, ptCenter;

	if (pEnt->isKindOf(OdDgDimension::desc()))
	{
		OdDgDimensionPtr pDimension = pEnt;
		OdGeExtents3d extents;

		pDimension->getGeomExtents(extents);

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
