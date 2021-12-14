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

/************************************************************************/
/* This class is an implementation of the OdDbGripPointsPE class for    */
/* OdDbRasterImage entities.                                            */
/************************************************************************/

#include "StdAfx.h"
#include "DbRasterImageGripPoints.h"
#include "DbRasterImage.h"
#include "DbWipeout.h"
#include "RxObjectImpl.h"
#include "Ge/GeLine3d.h"

OdResult OdDbRasterImageGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints )const
{
  OdDbRasterImagePtr pImg = ent;
  OdGeExtents3d exts;
  OdResult res = pImg->getGeomExtents(exts);
  if (eOk == res)
  {
    unsigned int size = gripPoints.size();
    if (!pImg->isClipped() || !pImg->isSetDisplayOpt(OdDbRasterImage::kClip))
    {
      gripPoints.resize( size + 5 );
      gripPoints[size] = exts.minPoint() + (exts.maxPoint() - exts.minPoint()) / 2.;

      OdGePoint3d origin;
      OdGeVector3d u, v;
      pImg->getOrientation( origin, u, v );

      gripPoints[size + 1] = origin;
      gripPoints[size + 2] = origin + v;
      gripPoints[size + 3] = origin + u + v;
      gripPoints[size + 4] = origin + u;
    }
    else
    {
      OdGePoint3dArray clipPoints;
      pImg->getVertices(clipPoints);
      if (clipPoints.last() == clipPoints.first())
        clipPoints.removeLast();
      gripPoints.append(clipPoints);
    }
  }
  return res;
}

struct Coeff
{
  int m_iUOrg;
  int m_iVOrg;
  int m_iUCf;
  int m_iVCf;
};

OdResult OdDbRasterImageGripPointsPE::moveGripPointsAt( OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset )
{
  unsigned size = indices.size();
  if ( size == 0 )
    return eOk;

  OdDbRasterImagePtr pImg = pEnt;
  if (!pImg->isClipped() || !pImg->isSetDisplayOpt(OdDbRasterImage::kClip))
  {
    if (indices[0] == 0)
    {
      return pEnt->transformBy(OdGeMatrix3d::translation(offset));
    }
    else
    {
      OdGePoint3d origin;
      OdGeVector3d u, v;
      pImg->getOrientation(origin, u, v);

      OdGeVector3d xNorm = u, yNorm = v;
      double xNLen = xNorm.normalizeGetLength();
      double yNLen = yNorm.normalizeGetLength();

      //Default bottom - left corner
      Coeff cfs = { 1, 1, 1, 1 };

      double dX = xNorm.dotProduct(offset);
      double dY = yNorm.dotProduct(offset);
      double dKoeffXY = xNLen / yNLen;

      switch (indices[0])
      {
      case 2:
        //top - left corner
        cfs.m_iVOrg = 0;
        cfs.m_iVCf = -1;
        break;
      case 3:
        //top - right corner
        cfs.m_iVOrg = 0;
        cfs.m_iVCf = -1;
        cfs.m_iUOrg = 0;
        cfs.m_iUCf = -1;
        break;
      case 4:
        //bottom - right corner
        cfs.m_iUOrg = 0;
        cfs.m_iUCf = -1;
        break;
      }
  	  dY = cfs.m_iVCf * dY * dKoeffXY;
	    dX = cfs.m_iUCf * dX;
	    if (dX > xNLen)
		    dX = 2 * xNLen - dX;

	    if (dY > yNLen)
		    dY = 2 * yNLen - dY;

	    // For Ortho mode, either dX or dY is always 0.
	    double dMov =  fabs(dX) > fabs(dY) ? dX : dY; 

      OdGeVector3d vecU = xNorm * dMov;
      OdGeVector3d vecV = (yNorm * dMov * yNLen) / xNLen;

      OdGePoint3d pOrg = origin + cfs.m_iUOrg * vecU + cfs.m_iVOrg * vecV;
      if (!(u - vecU).isZeroLength() && !(v - vecV).isZeroLength())
        pImg->setOrientation(pOrg, u - vecU, v - vecV);
    }
  }
  else
  {
    OdGePoint3dArray clipPoints;
    pImg->getVertices(clipPoints);
    for (unsigned i = 0; i < indices.size(); ++i)
    {
      if (indices[i] >= 0 && (unsigned)indices[i] < clipPoints.size())
        clipPoints[indices[i]] += offset;
    }
    clipPoints.last() = clipPoints.first();
    OdGePoint2dArray outBry;
    outBry.resize(clipPoints.size());
    OdGeMatrix3d w2p = pImg->getPixelToModelTransform().invert();
    OdGeVector2d isize = pImg->imageSize();
    bool isWipeout = pEnt->isKindOf(OdDbWipeout::desc());
    for (OdUInt32 setPt = 0; setPt < clipPoints.size(); setPt++)
    {
      outBry[setPt] = clipPoints[setPt].transformBy(w2p).convert2d();
      if (!isWipeout)
      {
        if (outBry[setPt].x < 0.0)
          outBry[setPt].x = 0.0;
        else if (outBry[setPt].x > isize.x)
          outBry[setPt].x = isize.x;
        if (outBry[setPt].y < 0.0)
          outBry[setPt].y = 0.0;
        else if (outBry[setPt].y > isize.y)
          outBry[setPt].y = isize.y;
      }
    }
    pImg->setClipBoundary(outBry);
  }
  return eOk;
}

OdResult OdDbRasterImageGripPointsPE::getStretchPoints(const OdDbEntity* ent, OdGePoint3dArray& stretchPoints) const
{
  return getGripPoints(ent, stretchPoints);
}

OdResult OdDbRasterImageGripPointsPE::moveStretchPointsAt(OdDbEntity* ent, const OdIntArray& indices, const OdGeVector3d& offset)
{
  return moveGripPointsAt( ent, indices, offset );
}

OdResult OdDbRasterImageGripPointsPE::getOsnapPoints( const OdDbEntity* ent, OdDb::OsnapMode osnapMode, OdGsMarker gsSelectionMark,
                                                      const OdGePoint3d& pickPoint, const OdGePoint3d& lastPoint, const OdGeMatrix3d& viewXform,
                                                      OdGePoint3dArray& snapPoints ) const
{
  switch (osnapMode)
  {
    case OdDb::kOsModeEnd:
    case OdDb::kOsModeCen:
      {
        OdGePoint3dArray gripPoints;
        getGripPoints( ent, gripPoints );
        if (gripPoints.size() > 0)
        {
          if (osnapMode == OdDb::kOsModeEnd)
            gripPoints.erase(gripPoints.begin());
          else
            gripPoints.erase(gripPoints.begin() + 1, gripPoints.end());
          snapPoints.append(gripPoints);
        }
      }
    break;
    default:
    break;
  }
  return eOk;
}
