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
#include "DgShapeGripPoints.h"
#include "DgShape.h"
#include "RxObjectImpl.h"
#include "Ge/GeExtents3d.h"

OdResult OdDgShapeGripPointsPE::getGripPoints(const OdDgElement* pEnt, OdGePoint3dArray& gripPoints)const
{
	unsigned int size = gripPoints.size();

	if (pEnt->isKindOf(OdDgShape2d::desc()))
	{
		OdDgShape2dPtr pShape2d = pEnt;

		OdUInt32 uVerticesCount = pShape2d->getVerticesCount();
		gripPoints.resize(size + uVerticesCount);

		for (OdUInt32 i = 0; i < uVerticesCount; i++)
		{
			OdGePoint2d ptVert = pShape2d->getVertexAt(i);
			gripPoints[size + i] = OdGePoint3d(ptVert.x, ptVert.y, 0);
		}
	}
	else
	{
		OdDgShape3dPtr pShape3d = pEnt;

		OdUInt32 uVerticesCount = pShape3d->getVerticesCount();
		gripPoints.resize(size + uVerticesCount);
		for (OdUInt32 i = 0; i < uVerticesCount; i++)
		{
			OdGePoint3d ptVert = pShape3d->getVertexAt(i);
			gripPoints[size + i] = ptVert;
		}
	}

	return eOk;
}

OdResult OdDgShapeGripPointsPE::moveGripPointsAt(OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
	unsigned size = indices.size();

	if (size == 0)
		return eOk;

	OdGePoint3dArray gripPoints;
	OdResult res = getGripPoints(pEnt, gripPoints);

	if (pEnt->isKindOf(OdDgShape2d::desc()))
	{
		OdDgShape2dPtr pShape2d = pEnt;

		for (OdUInt32 i = 0; i < indices.length(); i++)
		{
			OdUInt32 index = indices[i];

				OdGePoint2d ptVert = pShape2d->getVertexAt(index);
				ptVert.x += offset.x;
				ptVert.y += offset.y;
				pShape2d->setVertexAt(index, ptVert);
		}
	}
	else
	{
		OdDgShape3dPtr pShape3d = pEnt;

		for (OdUInt32 i = 0; i < indices.length(); i++)
		{
			OdUInt32 index = indices[i];

				OdGePoint3d ptVert = pShape3d->getVertexAt(index);
				ptVert.x += offset.x;
				ptVert.y += offset.y;
				pShape3d->setVertexAt(index, ptVert);
		}
	}

	return eOk;
}

OdResult OdDgShapeGripPointsPE::getStretchPoints(const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints) const
{
	OdResult res = getGripPoints(pEnt, stretchPoints);

	if (res == eOk)
	{
		stretchPoints.resize(stretchPoints.size() - 1);
	}

	return res;
}

OdResult OdDgShapeGripPointsPE::moveStretchPointsAt(OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
	return moveGripPointsAt(pEnt, indices, offset);
}

OdResult OdDgShapeGripPointsPE::getOsnapPoints(const OdDgElement* pEnt,
	OdDgElement::OsnapMode osnapMode,
	OdGsMarker gsSelectionMark,
	const OdGePoint3d& pickPoint,
	const OdGePoint3d& lastPoint,
	const OdGeMatrix3d& xWorldToEye,
	OdGePoint3dArray& snapPoints) const
{
	OdArray<OdGePoint3d> point3dArray;

	if (pEnt->isKindOf(OdDgShape2d::desc()))
	{
		OdDgShape2dPtr pShape2d = pEnt;

		OdUInt32 uVerticesCount = pShape2d->getVerticesCount();

		for (OdUInt32 i = 0; i < uVerticesCount; i++)
		{
			OdGePoint2d ptVert = pShape2d->getVertexAt(i);
			point3dArray.append(OdGePoint3d(ptVert.x, ptVert.y, 0));
		}
	}
	else
	{
		OdDgShape3dPtr pShape3d = pEnt;

		OdUInt32 uVerticesCount = pShape3d->getVerticesCount();

		for (OdUInt32 i = 0; i < uVerticesCount; i++)
		{
			OdGePoint3d ptVert = pShape3d->getVertexAt(i);
			point3dArray.append(ptVert);
		}
	}

	switch (osnapMode)
	{
	case OdDgElement::kOsModeMid:
	{
		double x(0), y(0), z(0);
		OdUInt32 l = point3dArray.length();
		for (OdUInt32 i = 0; i < l; i++)
		{
			x += point3dArray[i].x;
			y += point3dArray[i].y;
			z += point3dArray[i].z;
		}
		snapPoints.append(OdGePoint3d(x / l, y / l, z / l));
	}
	break;

	default:
	{

	}
	break;
	}

	return eOk;
}
