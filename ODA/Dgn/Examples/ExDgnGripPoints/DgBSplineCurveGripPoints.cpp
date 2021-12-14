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
#include "DgBSplineCurveGripPoints.h"
#include "DgBSplineCurve.h"
#include "RxObjectImpl.h"
//#include "Ge/GeBSplineCurve3d.h"

OdResult OdDgBSplineCurveGripPointsPE::getGripPoints(const OdDgElement* pEnt, OdGePoint3dArray& gripPoints)const
{
	unsigned int size = gripPoints.size();
	//gripPoints.resize(size + 2);

	OdGeExtents3d extents;

	if (pEnt->isKindOf(OdDgBSplineCurve2d::desc()))
	{
		OdDgBSplineCurve2dPtr pBSplineCurve2d = pEnt;

    if( pBSplineCurve2d->hasFitData() )
    {
      OdUInt32 uPolesCount = pBSplineCurve2d->numFitPoints();
      gripPoints.resize(size + uPolesCount + 2);

      for (OdUInt32 i = 0; i < uPolesCount; i++)
      {
        OdGePoint2d ptVert;
        pBSplineCurve2d->getFitPointAt(i, ptVert);
        gripPoints[size + i] = OdGePoint3d(ptVert.x, ptVert.y, 0);
      }

      OdGeVector2d vrStartTangent, vrEndTangent;
      pBSplineCurve2d->getFitTangents( vrStartTangent, vrEndTangent );

      OdGeVector3d vrStart3d( vrStartTangent.x, vrStartTangent.y, 0.0 );
      OdGeVector3d vrEnd3d( vrEndTangent.x, vrEndTangent.y, 0.0 );

      gripPoints[size + uPolesCount]     = gripPoints[size] + vrStart3d;
      gripPoints[size + uPolesCount + 1] = gripPoints[size + uPolesCount - 1] - vrEnd3d;
    }
    else
    {
      OdUInt32 uPolesCount = pBSplineCurve2d->numControlPoints();
      gripPoints.resize(size + uPolesCount);

      for (OdUInt32 i = 0; i < uPolesCount; i++)
      {
        OdGePoint2d ptVert;
        pBSplineCurve2d->getControlPointAt(i, ptVert);
        gripPoints[size + i] = OdGePoint3d(ptVert.x, ptVert.y, 0);
      }
    }
	}
	else
	{
		OdDgBSplineCurve3dPtr pBSplineCurve3d = pEnt;

    if( pBSplineCurve3d->hasFitData() )
    {
      OdUInt32 uPolesCount = pBSplineCurve3d->numFitPoints();
      gripPoints.resize(size + uPolesCount + 2);

      for (OdUInt32 i = 0; i < uPolesCount; i++)
      {
        OdGePoint3d ptVert;
        pBSplineCurve3d->getFitPointAt(i, ptVert);
        gripPoints[size + i] = ptVert;
      }

      OdGeVector3d vrStartTangent, vrEndTangent;
      pBSplineCurve3d->getFitTangents( vrStartTangent, vrEndTangent );

      gripPoints[size + uPolesCount]     = gripPoints[size] + vrStartTangent;
      gripPoints[size + uPolesCount + 1] = gripPoints[size + uPolesCount - 1] - vrEndTangent;
    }
    else
    {
      OdUInt32 uPolesCount = pBSplineCurve3d->numControlPoints();
      gripPoints.resize(size + uPolesCount);

      for (OdUInt32 i = 0; i < uPolesCount; i++)
      {
        OdGePoint3d ptVert;
        pBSplineCurve3d->getControlPointAt(i, ptVert);
        gripPoints[size + i] = ptVert;
      }
    }
	}
	return eOk;
}

OdResult OdDgBSplineCurveGripPointsPE::moveGripPointsAt(OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
	unsigned size = indices.size();

	if (size == 0)
		return eOk;

	OdGePoint3dArray gripPoints;
	OdResult res = getGripPoints(pEnt, gripPoints);

	if (pEnt->isKindOf(OdDgBSplineCurve2d::desc()))
	{
		OdDgBSplineCurve2dPtr pBSplineCurve2d = pEnt;

		for (OdUInt32 i = 0; i < indices.length(); i++)
		{
      OdUInt32 index = indices[i];

      if( pBSplineCurve2d->hasFitData() )
      {
        if( index < pBSplineCurve2d->numFitPoints() )
        {
          OdGePoint2d ptVert;
          pBSplineCurve2d->getFitPointAt(index, ptVert);
          ptVert.x += offset.x;
          ptVert.y += offset.y;
          pBSplineCurve2d->setFitPointAt(index, ptVert);
        }
        else
        {
          OdGeVector2d vrStartTangent, vrEndTangent;
          pBSplineCurve2d->getFitTangents( vrStartTangent, vrEndTangent );

          OdGePoint2d ptStartFit;
          OdGePoint2d ptEndFit;
          pBSplineCurve2d->getFitPointAt(pBSplineCurve2d->numFitPoints() - 1, ptEndFit);

          OdGePoint2d ptStartTangent = ptStartFit + vrStartTangent;
          OdGePoint2d ptEndTangent   = ptEndFit   - vrEndTangent;

          if( index == pBSplineCurve2d->numFitPoints() )
          {
            ptStartTangent.x += offset.x;
            ptStartTangent.y += offset.y;
          }
          else
          {
            ptEndTangent.x += offset.x;
            ptEndTangent.y += offset.y;
          }

          vrStartTangent = ptStartTangent - ptStartFit;
          vrEndTangent   = ptEndFit - ptEndTangent;
          pBSplineCurve2d->setFitTangents( vrStartTangent, vrEndTangent );
        }
      }
      else
      {
        OdGePoint2d ptVert;
        pBSplineCurve2d->getControlPointAt(index, ptVert);
        ptVert.x += offset.x;
        ptVert.y += offset.y;
        pBSplineCurve2d->setControlPointAt(index, ptVert);
      }
		}
	}
	else
	{
		OdDgBSplineCurve3dPtr pBSplineCurve3d = pEnt;

		for (OdUInt32 i = 0; i < indices.length(); i++)
		{
			OdUInt32 index = indices[i];

      if( pBSplineCurve3d->hasFitData() )
      {
        if( index < pBSplineCurve3d->numFitPoints() )
        {
           OdGePoint3d ptVert;
           pBSplineCurve3d->getFitPointAt(index, ptVert);
           ptVert += offset;
           pBSplineCurve3d->setFitPointAt(index, ptVert);
        }
        else
        {
          OdGeVector3d vrStartTangent, vrEndTangent;
          pBSplineCurve3d->getFitTangents( vrStartTangent, vrEndTangent );

          OdGePoint3d ptStartFit;
          OdGePoint3d ptEndFit;
          pBSplineCurve3d->getFitPointAt(pBSplineCurve3d->numFitPoints() - 1, ptEndFit);

          OdGePoint3d ptStartTangent = ptStartFit + vrStartTangent;
          OdGePoint3d ptEndTangent   = ptEndFit   - vrEndTangent;

          if( index == pBSplineCurve3d->numFitPoints() )
          {
            ptStartTangent += offset;
          }
          else
          {
            ptEndTangent += offset;
          }

          vrStartTangent = ptStartTangent - ptStartFit;
          vrEndTangent   = ptEndFit - ptEndTangent;
          pBSplineCurve3d->setFitTangents( vrStartTangent, vrEndTangent );
        }
      }
      else
      {
        OdGePoint3d ptVert;
        pBSplineCurve3d->getControlPointAt(index, ptVert);
        ptVert += offset;
        pBSplineCurve3d->setControlPointAt(index, ptVert);
      }
		}
	}

	return eOk;
}

OdResult OdDgBSplineCurveGripPointsPE::getStretchPoints(const OdDgElement* pEnt, OdGePoint3dArray& stretchPoints) const
{
	OdResult res = getGripPoints(pEnt, stretchPoints);

	if (res == eOk)
	{
		stretchPoints.resize(stretchPoints.size() - 1);
	}

	return res;
}

OdResult OdDgBSplineCurveGripPointsPE::moveStretchPointsAt(OdDgElement* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
	return moveGripPointsAt(pEnt, indices, offset);
}

OdResult OdDgBSplineCurveGripPointsPE::getOsnapPoints(const OdDgElement* pEnt,
	OdDgElement::OsnapMode osnapMode,
	OdGsMarker gsSelectionMark,
	const OdGePoint3d& pickPoint,
	const OdGePoint3d& lastPoint,
	const OdGeMatrix3d& xWorldToEye,
	OdGePoint3dArray& snapPoints) const
{
	OdArray<OdGePoint3d> point3dArray;

  if (pEnt->isKindOf(OdDgBSplineCurve2d::desc()))
  {
    OdDgBSplineCurve2dPtr pBSplineCurve2d = pEnt;

    if( pBSplineCurve2d->hasFitData() )
    {
      OdUInt32 uPolesCount = pBSplineCurve2d->numFitPoints();

      for (OdUInt32 i = 0; i < uPolesCount; i++)
      {
        OdGePoint2d ptVert;
        pBSplineCurve2d->getFitPointAt(i, ptVert);
        point3dArray.push_back(OdGePoint3d(ptVert.x, ptVert.y, 0));
      }
    }
    else
    {
      OdUInt32 uPolesCount = pBSplineCurve2d->numControlPoints();

      for (OdUInt32 i = 0; i < uPolesCount; i++)
      {
        OdGePoint2d ptVert;
        pBSplineCurve2d->getControlPointAt(i, ptVert);
        point3dArray.push_back(OdGePoint3d(ptVert.x, ptVert.y, 0));
      }
    }
  }
  else
  {
    OdDgBSplineCurve3dPtr pBSplineCurve3d = pEnt;

    if( pBSplineCurve3d->hasFitData() )
    {
      OdUInt32 uPolesCount = pBSplineCurve3d->numFitPoints();

      for (OdUInt32 i = 0; i < uPolesCount; i++)
      {
        OdGePoint3d ptVert;
        pBSplineCurve3d->getFitPointAt(i, ptVert);
        point3dArray.push_back(ptVert);
      }
    }
    else
    {
      OdUInt32 uPolesCount = pBSplineCurve3d->numControlPoints();

      for (OdUInt32 i = 0; i < uPolesCount; i++)
      {
        OdGePoint3d ptVert;
        pBSplineCurve3d->getControlPointAt(i, ptVert);
        point3dArray.push_back(ptVert);
      }
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
