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
#include "DgTextGripPoints.h"
#include "DgText.h"
#include "RxObjectImpl.h"

OdResult OdDgTextGripPointsPE::getGripPoints(const OdDgElement* pEnt, OdGePoint3dArray& gripPoints)const
{
	unsigned int size = gripPoints.size();
	gripPoints.resize(size + 2);

	if (pEnt->isKindOf(OdDgText2d::desc()))
	{
		OdDgText2dPtr pText2d = pEnt;

		OdGePoint2d ptBottomLeft = pText2d->getOrigin();
		OdGePoint2d ptPosition = pText2d->getPosition();

		gripPoints[size] = OdGePoint3d(ptBottomLeft.x, ptBottomLeft.y, 0);
		gripPoints[size + 1] = OdGePoint3d(ptPosition.x, ptPosition.y, 0);
	}
	else
	{
		OdDgText3dPtr pText3d = pEnt;

		OdGePoint3d ptOrigin = pText3d->getOrigin();
		OdGePoint3d ptPosition = pText3d->getPosition();


		gripPoints[size] = ptOrigin;
		gripPoints[size + 1] = ptPosition;
	}

	return eOk;
}

OdResult OdDgTextGripPointsPE::moveGripPointsAt(OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
	unsigned size = indices.size();

	if (size == 0)
		return eOk;

	if (pEnt->isKindOf(OdDgText2d::desc()))
	{
		OdDgText2dPtr pText2d = pEnt;

		for (OdUInt32 i = 0; i < indices.length(); i++)
		{
			OdUInt32 index = indices[i];
			switch (index)
			{
			case 0:
			{
				pText2d->setOrigin(pText2d->getOrigin() + offset.convert2d());
			} break;
			case 1:
			{
				pText2d->setPosition(pText2d->getPosition() + offset.convert2d());
			} break;
			}
		}
	}
	else
	{
		OdDgText3dPtr pText3d = pEnt;

		for (OdUInt32 i = 0; i < indices.length(); i++)
		{
			OdUInt32 index = indices[i];
			switch (index)
			{
			case 0:
			{
				pText3d->setOrigin(pText3d->getOrigin() + offset);
			} break;
			case 1:
			{
				pText3d->setPosition(pText3d->getPosition() + offset);
			} break;
			}
		}
	}

	return eOk;
}

OdResult OdDgTextGripPointsPE::getStretchPoints(const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints) const
{
	OdResult res = getGripPoints(pEnt, stretchPoints);

	if (res == eOk)
	{
		stretchPoints.resize(stretchPoints.size() - 1);
	}

	return res;
}


OdResult OdDgTextGripPointsPE::moveStretchPointsAt(OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
	return moveGripPointsAt(pEnt, indices, offset);
}


OdResult OdDgTextGripPointsPE::getOsnapPoints(const OdDgElement* pEnt,
	OdDgElement::OsnapMode osnapMode,
	OdGsMarker gsSelectionMark,
	const OdGePoint3d& pickPoint,
	const OdGePoint3d& lastPoint,
	const OdGeMatrix3d& xWorldToEye,
	OdGePoint3dArray& snapPoints) const
{
	OdGePoint3d ptPosition;
	OdGePoint3d ptEnd;

	if (pEnt->isKindOf(OdDgText2d::desc()))
	{
		OdDgText2dPtr pText2d = pEnt;

		ptPosition = OdGePoint3d(pText2d->getPosition().x, pText2d->getPosition().y, 0);
	}
	else
	{
		OdDgText3dPtr pText3d = pEnt;

		ptPosition = pText3d->getPosition();
	}

	switch (osnapMode)
	{
	case OdDgElement::kOsModeCen:
	case OdDgElement::kOsModeMid:
	{
		snapPoints.append(ptPosition);
	}
	default:
	{

	}
	break;
	}

	return eOk;
}
