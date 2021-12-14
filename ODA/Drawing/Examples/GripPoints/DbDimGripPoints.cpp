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
/* These classes are implementations of the OdDbGripPointsPE interface  */
/* for their respective OdDbDimension derived entities.                 */
/************************************************************************/
#include "StdAfx.h"
#include "DbDimGripPoints.h"
#include "DbRotatedDimension.h"
#include "DbAlignedDimension.h"
#include "DbRadialDimension.h"
#include "DbDiametricDimension.h"
#include "Db3PointAngularDimension.h"
#include "DbOrdinateDimension.h"
#include "Db2LineAngularDimension.h"
#include "DbRadialDimensionLarge.h"
#include "DbArcDimension.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeMatrix3d.h"
#include "RxObjectImpl.h"
#include "Ge/GeLine2d.h"
#include "Ge/GeLineSeg3d.h"

#define STL_USING_ALGORITHM
#define STL_USING_UTILITY
#include "OdaSTL.h"

OdResult OdDbRotatedDimGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints )const
{
  OdDbRotatedDimensionPtr pDimPtr = ent;
  unsigned int size = gripPoints.size();
  if ( pDimPtr->jogSymbolOn() )
    gripPoints.resize( size + 6 );
  else
    gripPoints.resize( size + 5 );

  OdGePoint3d defPoint1 = pDimPtr->xLine1Point();
  OdGePoint3d defPoint2 = pDimPtr->xLine2Point();
  OdGePoint3d dimLineDefPt = pDimPtr->dimLinePoint();
  OdGePoint3d dimLineText = pDimPtr->textPosition();
  double dRotAngle = pDimPtr->rotation();
  double dOblique = pDimPtr->oblique();

  OdGeMatrix3d world2Plane(OdGeMatrix3d::worldToPlane(pDimPtr->normal()));
  OdGePoint3d ocsDefPt1(defPoint1), ocsDimLinDefPt(dimLineDefPt);

  OdGeVector3d vNorm = pDimPtr->normal();
  bool bNeedTransform = false;
  if ( vNorm != OdGeVector3d::kZAxis )
  {
    bNeedTransform = true;

    ocsDefPt1.transformBy(world2Plane);
    ocsDimLinDefPt.transformBy(world2Plane);
  }
  double savedZCoord = ocsDefPt1.z;
  ocsDefPt1.z = ocsDimLinDefPt.z = 0.0; 

  OdGeVector3d v1 = OdGeVector3d::kXAxis;
  v1.rotateBy(dRotAngle, OdGeVector3d::kZAxis);
  OdGeVector3d v2 = OdGeVector3d::kYAxis;
  if(OdNonZero(dOblique))
  {
    dOblique = dOblique - OdaPI2;
  }
  v2.rotateBy(dRotAngle+dOblique, OdGeVector3d::kZAxis);
  OdGeLine3d line1, line2;
  line1.set(ocsDimLinDefPt, v1);
  line2.set(ocsDefPt1, v2);

  OdGePoint3d extLineEnd;
  line1.intersectWith(line2, extLineEnd);
  extLineEnd.z = savedZCoord;
  if ( bNeedTransform )
    extLineEnd.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));

  gripPoints[size + 0] = defPoint1;
  gripPoints[size + 1] = defPoint2;
  gripPoints[size + 2] = extLineEnd;
  gripPoints[size + 3] = dimLineDefPt;
  gripPoints[size + 4] = dimLineText;
  if ( pDimPtr->jogSymbolOn() )
    gripPoints[size + 5] = pDimPtr->jogSymbolPosition();
  return eOk;
}

OdResult OdDbRotatedDimGripPointsPE::moveGripPoint( OdDbEntity* ent, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch)
{
  if ( indices.empty() ) return eOk;


  OdDbRotatedDimensionPtr pDimPtr = ent;

  const OdGePoint3d *pGripPoint3d; // = &gripPoints[indices[0]];
  OdGePoint3d dimNewPt; // = *pGripPoint3d;
  double savedZCoord = 0;
  OdGeVector3d vMoveTxt;

  OdGeVector3d vNorm = pDimPtr->normal();
  bool bNeedTransform = false;
  if ( vNorm != OdGeVector3d::kZAxis )
    bNeedTransform = true;

  for (int i = 0; i < (int)indices.size(); i++)
  {
    pGripPoint3d = &gripPoints[indices[i]];
    dimNewPt = *pGripPoint3d;
    switch (indices[i])
    {
    case 0:
        pDimPtr->setXLine1Point(dimNewPt);
        break;
    case 1:
        pDimPtr->setXLine2Point(dimNewPt);
        break;
    case 2:
      {
        OdGeMatrix3d world2Plane(OdGeMatrix3d::worldToPlane(pDimPtr->normal()));

        OdGePoint3d ocsDimNewPt(dimNewPt);

        OdGePoint3d dimLinePt = pDimPtr->dimLinePoint();
        OdGePoint3d ocsDimLinePt(dimLinePt);

        OdGePoint3d defPoint1 = pDimPtr->xLine1Point();
        OdGePoint3d ocsDefPt1(defPoint1);
        if ( bNeedTransform )
        {
          ocsDimNewPt.transformBy(world2Plane);
          ocsDimLinePt.transformBy(world2Plane);
          ocsDefPt1.transformBy(world2Plane);
        }
        savedZCoord = ocsDefPt1.z;
        ocsDimNewPt.z = 0.0;
        ocsDimLinePt.z = 0.0;
        ocsDefPt1.z = 0.0;

        double dRotAngle = pDimPtr->rotation();
        double dOblique = pDimPtr->oblique();
        OdGeVector3d v1 = OdGeVector3d::kXAxis;
        v1.rotateBy(dRotAngle, OdGeVector3d::kZAxis);
        OdGeVector3d v2 = OdGeVector3d::kYAxis;

        if(OdNonZero(dOblique))
        {
          dOblique = dOblique - OdaPI2;
        }
        v2.rotateBy(dRotAngle+dOblique, OdGeVector3d::kZAxis);

        OdGeLine3d line1, line2;
        line1.set(ocsDimLinePt, v1);
        line2.set(ocsDefPt1, v2);
        OdGePoint3d extLineEnd;
        line1.intersectWith(line2, extLineEnd);
        vMoveTxt = extLineEnd - ocsDimNewPt;

        line1.set(ocsDimNewPt, v1);
        line2.set(ocsDimLinePt, v2);
        line1.intersectWith(line2, ocsDimNewPt);

        dimNewPt = ocsDimNewPt;
        dimNewPt.z = savedZCoord;
        if ( bNeedTransform )
          dimNewPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));
      }
    case 3:
      if(!pDimPtr->isUsingDefaultTextPosition() && pDimPtr->dimtmove() == 0)
      {
        OdGeMatrix3d world2Plane(OdGeMatrix3d::worldToPlane(pDimPtr->normal()));
        if(indices[0] == 3) 
        {
          OdGePoint3d ocsDimLinePt(pDimPtr->dimLinePoint());
          OdGePoint3d ocsDimNewPt = dimNewPt;
          if ( bNeedTransform )
          {
            ocsDimLinePt.transformBy(world2Plane);
            ocsDimNewPt.transformBy(world2Plane);
          }
          ocsDimLinePt.z = 0.0;
          ocsDimNewPt.z = 0.0;

          vMoveTxt = ocsDimLinePt - ocsDimNewPt;
        }

        OdGePoint3d dimTextPt = gripPoints[4];
        //pDimPtr->textPosition();
        OdGePoint3d ocsDimTextPt = dimTextPt;
        if ( bNeedTransform )
          ocsDimTextPt.transformBy(world2Plane);
        savedZCoord = ocsDimTextPt.z;
        ocsDimTextPt.z = 0.0;

        OdGePoint3d ocsDimNewTextPt = ocsDimTextPt - vMoveTxt;
        OdGePoint3d dimNewTextPt = ocsDimNewTextPt;
        dimNewTextPt.z = savedZCoord;
        if ( bNeedTransform )
          dimNewTextPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));

        pDimPtr->setTextPosition(dimNewTextPt);

      } else
          pDimPtr->setDimLinePoint(dimNewPt);
      break;

    case 4:
      pDimPtr->setTextPosition(dimNewPt);
      if (indices.size() == 1 || !bStretch)
        pDimPtr->useSetTextPosition();
      break;
  
    case 5:
      if(!pDimPtr->isUsingDefaultTextPosition() && pDimPtr->dimtmove() == 0)
      {
        OdGeMatrix3d world2Plane(OdGeMatrix3d::worldToPlane(pDimPtr->normal()));

        OdGePoint3d ocsDimJogPt(pDimPtr->jogSymbolPosition());
        OdGePoint3d ocsDimNewPt = dimNewPt;
      
        OdGePoint3d p1 = pDimPtr->xLine2Point();
        OdGePoint3d p2 = pDimPtr->dimLinePoint();
        if ( bNeedTransform )
        {
          ocsDimJogPt.transformBy(world2Plane);
          ocsDimNewPt.transformBy(world2Plane);
          p1.transformBy(world2Plane);
          p2.transformBy(world2Plane);
        }
        ocsDimJogPt.z = 0.0;
        ocsDimNewPt.z = 0.0;
        p1.z = p2.z = 0.0;

        OdGeVector3d v = p2 - p1;
        vMoveTxt = ocsDimJogPt - ocsDimNewPt;
        if ( v.length() )
          v.normalize();
        v *= vMoveTxt.length()*cos(v.angleTo(vMoveTxt));

        OdGePoint3d dimTextPt = gripPoints[4];
        OdGePoint3d ocsDimTextPt = dimTextPt;
        if ( bNeedTransform )
          ocsDimTextPt.transformBy(world2Plane);
        savedZCoord = ocsDimTextPt.z;
        ocsDimTextPt.z = 0.0;

        OdGePoint3d ocsDimNewTextPt = ocsDimTextPt - v;
        OdGePoint3d dimNewTextPt = ocsDimNewTextPt;

        dimNewTextPt.z = savedZCoord;
        if ( bNeedTransform )
          dimNewTextPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));
        pDimPtr->setTextPosition(dimNewTextPt);
      } else
      {
        pDimPtr->setDimLinePoint(dimNewPt);
      }
      pDimPtr->setJogSymbolPosition(dimNewPt);
      break;

    default:
       break;
    }
  }
  return eOk;
}

OdResult OdDbAlignedDimGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints )const
{
  OdDbAlignedDimensionPtr pDimPtr = ent;
  unsigned int size = gripPoints.size();
  if ( pDimPtr->jogSymbolOn() )
    gripPoints.resize( size + 6 );
  else
    gripPoints.resize( size + 5 );

  OdGePoint3d defPoint1 = pDimPtr->xLine1Point();
  OdGePoint3d defPoint2 = pDimPtr->xLine2Point();
  OdGePoint3d dimLineDefPt = pDimPtr->dimLinePoint();
  OdGePoint3d dimLineText = pDimPtr->textPosition();

  double dOblique = pDimPtr->oblique();

  OdGeMatrix3d world2Plane(OdGeMatrix3d::worldToPlane(pDimPtr->normal()));
  OdGePoint3d ocsDefPt1(defPoint1), ocsDefPt2(defPoint2), ocsDimLinDefPt(dimLineDefPt);

  OdGeVector3d vNorm = pDimPtr->normal();
  bool bNeedTransform = false;
  if ( vNorm != OdGeVector3d::kZAxis )
  {
    bNeedTransform = true;

    ocsDefPt1.transformBy(world2Plane);
    ocsDefPt2.transformBy(world2Plane);
    ocsDimLinDefPt.transformBy(world2Plane);
  }
  double savedZCoord = ocsDefPt1.z;
  ocsDefPt1.z = ocsDefPt1.z = ocsDimLinDefPt.z = 0.0; 

  OdGeVector3d v1 = OdGeVector3d::kXAxis;
  OdGeVector3d v2 = OdGeVector3d::kYAxis;
  if(OdNonZero(dOblique))
  {
      dOblique = Oda2PI-dOblique;
  }
  else
  {
      dOblique = OdaPI2;
  }
  v2 = ocsDimLinDefPt - ocsDefPt2;
  if(OdZero(v2.length()))
  {
    v2 = OdGeVector3d::kYAxis;
    OdGeVector3d dlv =  defPoint2 - defPoint1;
    if ( OdNonZero(dlv.length() ))
      v2 = dlv.perpVector();
  }
  v1 = v2;
  v1.rotateBy(dOblique, OdGeVector3d::kZAxis);

  OdGeLine3d line1, line2;
  line1.set(ocsDimLinDefPt, v1);
  line2.set(ocsDefPt1,  v2 );
  OdGePoint3d extLineEnd;
  line1.intersectWith(line2, extLineEnd);

  extLineEnd.z = savedZCoord;
  if ( bNeedTransform ) 
    extLineEnd.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));

  gripPoints[size + 0] = defPoint1;
  gripPoints[size + 1] = defPoint2;
  gripPoints[size + 2] = extLineEnd;
  gripPoints[size + 3] = dimLineDefPt;
  gripPoints[size + 4] = dimLineText;
  if ( pDimPtr->jogSymbolOn() )
    gripPoints[size + 5] = pDimPtr->jogSymbolPosition();

  return eOk;
}


OdResult OdDbAlignedDimGripPointsPE::moveGripPoint( OdDbEntity* ent, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch)
{
  if ( indices.empty() ) return eOk;

  OdDbAlignedDimensionPtr pDimPtr = ent;

  const OdGePoint3d *pGripPoint3d = &gripPoints[indices[0]];
  OdGePoint3d dimNewPt = *pGripPoint3d;
  double savedZCoord = 0;
  OdGeVector3d vMoveTxt;

  OdGeVector3d vNorm = pDimPtr->normal();
  bool bNeedTransform = false;
  if ( vNorm != OdGeVector3d::kZAxis )
  {
    bNeedTransform = true;
  }
  for (int i = 0; i < (int)indices.size(); i++)
  {
    pGripPoint3d = &gripPoints[indices[i]];
    dimNewPt = *pGripPoint3d;
    switch (indices[i])
    {
    case 0:
        pDimPtr->setXLine1Point(dimNewPt);
        break;
    case 1:
        pDimPtr->setXLine2Point(dimNewPt);
        break;
    case 2:
        {
            OdGeMatrix3d world2Plane(OdGeMatrix3d::worldToPlane(pDimPtr->normal()));
            OdGePoint3d ocsDimNewPt(dimNewPt);

            OdGePoint3d xLinePt1 = pDimPtr->xLine1Point();
            OdGePoint3d xLinePt2 = pDimPtr->xLine2Point();
            OdGePoint3d dimLinePt = pDimPtr->dimLinePoint();

            OdGePoint3d ocsXLinePt1(xLinePt1);
            OdGePoint3d ocsXLinePt2(xLinePt2);
            OdGePoint3d ocsDimLinePt(dimLinePt);
            if ( bNeedTransform )
            {
              ocsDimNewPt.transformBy(world2Plane);
              ocsXLinePt1.transformBy(world2Plane);
              ocsXLinePt2.transformBy(world2Plane);
              ocsDimLinePt.transformBy(world2Plane);
            }
            double savedZCoord = ocsXLinePt1.z;
            ocsDimNewPt.z = 0.0;
            ocsXLinePt1.z = ocsXLinePt2.z = ocsDimLinePt.z = 0.0;

            double dOblique = pDimPtr->oblique();
            OdGeVector3d v1 = OdGeVector3d::kXAxis;
            OdGeVector3d v2 = OdGeVector3d::kYAxis;

            if(!OdNonZero(dOblique))
            {
                dOblique = OdaPI2;
            }
            v1 = v2 = ocsXLinePt2 - ocsXLinePt1;
            v2.rotateBy(dOblique, OdGeVector3d::kZAxis);

            OdGeLine3d line1, line2;
            line1.set(ocsDimLinePt, v1);
            line2.set(ocsXLinePt1, v2);
            OdGePoint3d extLineEnd;
            line1.intersectWith(line2, extLineEnd);
            vMoveTxt = extLineEnd - ocsDimNewPt;


            line1.set(ocsDimNewPt, v1);
            line2.set(ocsDimLinePt, v2);
            line1.intersectWith(line2, ocsDimLinePt);
            dimNewPt = ocsDimLinePt;
            dimNewPt.z = savedZCoord;
            if ( bNeedTransform )
              dimNewPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));
        }
    case 3:
        if(!pDimPtr->isUsingDefaultTextPosition() && pDimPtr->dimtmove() == 0)
        {

            OdGeMatrix3d world2Plane(OdGeMatrix3d::worldToPlane(pDimPtr->normal()));
            if(indices[0] == 3) 
            {
                OdGePoint3d ocsDimLinePt(pDimPtr->dimLinePoint());
                OdGePoint3d ocsDimNewPt = dimNewPt;
                if ( bNeedTransform )
                {
                  ocsDimLinePt.transformBy(world2Plane);
                  ocsDimLinePt.transformBy(world2Plane);
                }
                ocsDimNewPt.z = 0.0;
                ocsDimNewPt.z = 0.0;

                vMoveTxt = ocsDimLinePt - ocsDimNewPt;
            }

            OdGePoint3d dimTextPt = gripPoints[4];
            //pDimPtr->textPosition();
            OdGePoint3d ocsDimTextPt = dimTextPt;
            if ( bNeedTransform )
              ocsDimTextPt.transformBy(world2Plane);
            double savedZCoord = ocsDimTextPt.z;
            ocsDimTextPt.z = 0.0;
            OdGePoint3d ocsDimNewTextPt = ocsDimTextPt - vMoveTxt;
            OdGePoint3d dimNewTextPt = ocsDimNewTextPt;

            dimNewTextPt.z = savedZCoord;
            if ( bNeedTransform )
              dimNewTextPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));

            pDimPtr->setTextPosition(dimNewTextPt);

        }
        else
            pDimPtr->setDimLinePoint(dimNewPt);
        break;
    case 4:
        pDimPtr->setTextPosition(dimNewPt);
        if (indices.size() == 1 || !bStretch)
          pDimPtr->useSetTextPosition();
        break;

    case 5:
      if(!pDimPtr->isUsingDefaultTextPosition() && pDimPtr->dimtmove() == 0)
      {
        OdGeMatrix3d world2Plane(OdGeMatrix3d::worldToPlane(pDimPtr->normal()));

        OdGePoint3d ocsDimJogPt(pDimPtr->jogSymbolPosition());
        OdGePoint3d ocsDimNewPt = dimNewPt;

        OdGePoint3d p1 = pDimPtr->xLine2Point();
        OdGePoint3d p2 = pDimPtr->dimLinePoint();
        if ( bNeedTransform )
        {
          ocsDimJogPt.transformBy(world2Plane);
          ocsDimNewPt.transformBy(world2Plane);
          p1.transformBy(world2Plane);
          p2.transformBy(world2Plane);
        }
        ocsDimJogPt.z = 0.0;
        ocsDimNewPt.z = 0.0;
        p1.z = p2.z = 0.0;

        OdGeVector3d v = p2 - p1;
        vMoveTxt = ocsDimJogPt - ocsDimNewPt;
        if ( v.length() )
          v.normalize();
        v *= vMoveTxt.length()*cos(v.angleTo(vMoveTxt));

        OdGePoint3d dimTextPt = gripPoints[4];
        OdGePoint3d ocsDimTextPt = dimTextPt;
        if ( bNeedTransform )
          ocsDimTextPt.transformBy(world2Plane);
        savedZCoord = ocsDimTextPt.z;
        ocsDimTextPt.z = 0.0;

        OdGePoint3d ocsDimNewTextPt = ocsDimTextPt - v;
        OdGePoint3d dimNewTextPt = ocsDimNewTextPt;

        dimNewTextPt.z = savedZCoord;
        if ( bNeedTransform )
          dimNewTextPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));
        pDimPtr->setTextPosition(dimNewTextPt);
      } else
      {
        pDimPtr->setDimLinePoint(dimNewPt);
      }
      pDimPtr->setJogSymbolPosition(dimNewPt);
      break;

    default:
        break;
    }
  }
  return eOk;

}


OdResult OdDbRadialDimGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints )const
{
  unsigned int size = gripPoints.size();
  gripPoints.resize( size + 3 );
  OdDbRadialDimensionPtr pDimPtr = ent;

  gripPoints[size + 0] = pDimPtr->center();
  gripPoints[size + 1] = pDimPtr->chordPoint();
  gripPoints[size + 2] = pDimPtr->textPosition();

  return eOk;
}

OdResult OdDbRadialDimGripPointsPE::moveGripPoint( OdDbEntity* ent, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch)
{
  if ( indices.empty() ) return eOk;

  OdDbRadialDimensionPtr pDimPtr = ent;

  const OdGePoint3d *pGripPoint3d = &gripPoints[indices[0]];
  OdGePoint3d dimNewPt = *pGripPoint3d;

  OdGeVector3d vNorm = pDimPtr->normal();
  bool bNeedTransform = false;
  if ( vNorm != OdGeVector3d::kZAxis )
  {
    bNeedTransform = true;
  }

  switch( indices[0] )
  {
  case 0:
    {
      // radius is constant
      // chord point is calculated using center and text points
      // text point may be changed in recompute dimension block

      OdGePoint3d dimCenterPt = pDimPtr->center();
      OdGePoint3d dimChordPt = pDimPtr->chordPoint();
      double radius = dimCenterPt.distanceTo(dimChordPt);

      OdGePoint3d textPt = pDimPtr->textPosition();
      OdGeVector3d v = textPt - dimNewPt;
      v.normalize();

      dimChordPt = dimNewPt + v*radius; 
      pDimPtr->setChordPoint(dimChordPt);
      pDimPtr->setCenter(dimNewPt);
      break;
    }
  case 1:
      {
        // radius is constant, center point is constant
        // text points is calculated and may be changed in recompute dimension block
        OdGePoint3d dimCenterPt = pDimPtr->center();
        OdGePoint3d dimChordPt = pDimPtr->chordPoint();
        // double dist = dimChordPt.distanceTo(pDimPtr->textPosition());
        double dist = dimCenterPt.distanceTo(pDimPtr->textPosition());

        OdGeMatrix3d world2Plane(OdGeMatrix3d::worldToPlane(pDimPtr->normal()));
        OdGePoint3d ocsDimCenterPt(dimCenterPt), ocsDimChordPt(dimChordPt); 

        OdGeVector3d vX, vY;
        OdGeLine3d line1, line2;
        OdGePoint3d ocsDimNewPt(dimNewPt);

        if ( bNeedTransform )
        {
          ocsDimCenterPt.transformBy(world2Plane);
          ocsDimChordPt.transformBy(world2Plane);
          ocsDimNewPt.transformBy(world2Plane);
        }
        double savedZCoord = ocsDimCenterPt.z;
        ocsDimCenterPt.z = ocsDimChordPt.z = 0.0; 
        ocsDimNewPt.z = 0.0;

        vX = ocsDimCenterPt - ocsDimNewPt;
        vY = ocsDimCenterPt - ocsDimChordPt;

        double angle = vY.angleTo(vX, OdGeVector3d::kZAxis);
        // if( !angle ) return eOk;
        vY.rotateBy(Oda2PI-(OdaPI - angle)/2, OdGeVector3d::kZAxis);
        line1.set( ocsDimChordPt, vY );
        line2.set( ocsDimCenterPt, vX);
        line1.intersectWith(line2, ocsDimChordPt);
        dimNewPt = ocsDimChordPt;

        dimNewPt.z = savedZCoord;
        if ( bNeedTransform )
          dimNewPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));

        OdGeVector3d v = dimNewPt - dimCenterPt;
        v.normalize();
        OdGePoint3d textPt = dimCenterPt + v*dist;

        pDimPtr->setTextPosition(textPt);

        pDimPtr->setChordPoint(dimNewPt);
        break;
      }
  case 2:
    {
      // radius is constant, center point is constant
      // chord point is calculated using center and text points
      OdGePoint3d dimCenterPt = pDimPtr->center();
      OdGePoint3d dimChordPt = pDimPtr->chordPoint();
      double radius = dimCenterPt.distanceTo(dimChordPt);

      OdGePoint3d textPt = dimNewPt;
      OdGeVector3d v = textPt - dimCenterPt;
      v.normalize();
      dimChordPt = dimCenterPt + v*radius; 

      pDimPtr->setChordPoint(dimChordPt);
      pDimPtr->setTextPosition(dimNewPt);
      //pDimPtr->useSetTextPosition();
      break;
    }
  default:
      break;
  }

  return eOk;
}


OdResult OdDbDiametricDimGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints )const
{
  unsigned int size = gripPoints.size();
  gripPoints.resize( size + 3 );
  OdDbDiametricDimensionPtr pDimPtr = ent;

  gripPoints[size + 0] = pDimPtr->chordPoint();
  gripPoints[size + 1] = pDimPtr->farChordPoint();
  gripPoints[size + 2] = pDimPtr->textPosition();

  return eOk;
}

OdResult OdDbDiametricDimGripPointsPE::moveGripPoint( OdDbEntity* ent, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch)
{
  if ( indices.empty() ) return eOk;

  // unsigned int size = gripPoints.size();
  OdDbDiametricDimensionPtr pDimPtr = ent;

  OdGePoint3d dimChordPt = pDimPtr->chordPoint();
  OdGePoint3d dimFarChordPt = pDimPtr->farChordPoint();
  OdGePoint3d dimTextPt = pDimPtr->textPosition();
  OdGePoint3d dimMidPt = dimFarChordPt + (dimChordPt - dimFarChordPt )/2;

  OdGeMatrix3d world2Plane(OdGeMatrix3d::worldToPlane(pDimPtr->normal()));
  OdGePoint3d ocsDimChordPt(dimChordPt), ocsDimFarChordPt(dimFarChordPt), 
      ocsDimTextPt(dimTextPt), ocsDimMidPt(dimMidPt);

  OdGeVector3d vNorm = pDimPtr->normal();
  bool bNeedTransform = false;
  if ( vNorm != OdGeVector3d::kZAxis )
  {
    bNeedTransform = true;

    ocsDimChordPt.transformBy(world2Plane);
    ocsDimFarChordPt.transformBy(world2Plane);
    ocsDimTextPt.transformBy(world2Plane);
    ocsDimMidPt.transformBy(world2Plane);
  }
  double savedZCoord = ocsDimChordPt.z;
  ocsDimChordPt.z = ocsDimFarChordPt.z = ocsDimTextPt.z = ocsDimMidPt.z = 0.0; 

  OdGeVector3d vLen = ocsDimFarChordPt - ocsDimChordPt;
  OdGeVector3d vX, vY;
  const OdGePoint3d *pGripPoint3d = 0;
  OdGeLine3d line1, line2, lineText;
  pGripPoint3d = &gripPoints[indices[0]];
  OdGePoint3d ocsDimNewPt = *pGripPoint3d;
  OdGePoint3d dimNewPt = ocsDimNewPt;
  if ( bNeedTransform )
    ocsDimNewPt.transformBy(world2Plane);
  ocsDimNewPt.z = 0.0;

  vX = ocsDimMidPt - ocsDimNewPt;
  vY = ocsDimMidPt - ocsDimChordPt;

  double angle = vY.angleTo(vX, OdGeVector3d::kZAxis);
  if( indices[0] == 1 ) 
     angle += OdaPI;

  vLen.rotateBy(angle, OdGeVector3d::kZAxis);

  if( indices[0] == 1 ) 
  {
    double newLen = dimMidPt.distanceTo(ocsDimNewPt);
    vLen.normalize();
    vLen *= newLen;
  } else
  {
    vLen *= 0.5;
  }

  ocsDimChordPt = dimMidPt - vLen;
  ocsDimFarChordPt = dimMidPt + vLen;
  if ( OdNonZero(angle) )
    ocsDimTextPt.rotateBy(angle, OdGeVector3d::kZAxis, dimMidPt);
  if( indices[0] == 2 )
  {
    ocsDimTextPt = ocsDimNewPt;
    pDimPtr->useSetTextPosition();
    if ( ocsDimTextPt.distanceTo(ocsDimFarChordPt) <  ocsDimTextPt.distanceTo(ocsDimChordPt) )
    {
      std::swap(ocsDimFarChordPt, ocsDimChordPt);
    }
  }

  ocsDimFarChordPt.z = savedZCoord;
  ocsDimChordPt.z = savedZCoord;
  ocsDimTextPt.z = savedZCoord;
  if ( bNeedTransform )
  {
    ocsDimFarChordPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));
    ocsDimChordPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));
    ocsDimTextPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));
  }
  pDimPtr->setFarChordPoint(ocsDimFarChordPt);
  pDimPtr->setChordPoint(ocsDimChordPt);
  pDimPtr->setTextPosition(ocsDimTextPt);

  return eOk;
}


OdResult OdDb3PointAngularDimGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints )const
{
  unsigned int size = gripPoints.size();
  gripPoints.resize( size + 5 );
  OdDb3PointAngularDimensionPtr pDimPtr = ent;

  gripPoints[size + 0] = pDimPtr->xLine1Point();
  gripPoints[size + 1] = pDimPtr->xLine2Point();
  gripPoints[size + 2] = pDimPtr->centerPoint();
  gripPoints[size + 3] = pDimPtr->arcPoint();
  gripPoints[size + 4] = pDimPtr->textPosition();

  return eOk;

}


OdResult OdDb3PointAngularDimGripPointsPE::moveGripPoint( OdDbEntity* ent, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch)
{
  if ( indices.empty() ) return eOk;

  // unsigned int size = gripPoints.size();
  OdDb3PointAngularDimensionPtr pDimPtr = ent;

  OdGePoint3d dimLine1Pt = pDimPtr->xLine1Point();
  OdGePoint3d dimLine2Pt = pDimPtr->xLine2Point();
  OdGePoint3d dimCenterPt = pDimPtr->centerPoint();
  OdGePoint3d dimArcPt = pDimPtr->arcPoint();
  OdGePoint3d dimTextPt = pDimPtr->textPosition();
  OdGePoint3d dimArcNewPt = dimArcPt;

  OdGeMatrix3d world2Plane(OdGeMatrix3d::worldToPlane(pDimPtr->normal()));
  OdGePoint3d ocsDimLine1Pt(dimLine1Pt), ocsDimLine2Pt(dimLine2Pt), 
      ocsDimCenterPt(dimCenterPt), ocsDimArcPt(dimArcPt), ocsDimTextPt(dimTextPt),
      ocsDimArcNewPt(dimArcNewPt);

  OdGeVector3d vNorm = pDimPtr->normal();
  bool bNeedTransform = false;
  if ( vNorm != OdGeVector3d::kZAxis )
  {
    bNeedTransform = true;

    ocsDimLine1Pt.transformBy(world2Plane);
    ocsDimLine2Pt.transformBy(world2Plane);
    ocsDimCenterPt.transformBy(world2Plane);
    ocsDimArcPt.transformBy(world2Plane);
    ocsDimTextPt.transformBy(world2Plane);
    ocsDimTextPt.transformBy(world2Plane);
    ocsDimArcNewPt.transformBy(world2Plane);
  }
  double savedZCoord = ocsDimLine1Pt.z;
  ocsDimLine1Pt.z = ocsDimLine2Pt.z = ocsDimCenterPt.z = ocsDimArcPt.z = ocsDimTextPt.z = ocsDimArcNewPt.z = 0.0; 

  OdGeVector3d vX1, vX2, vArc, vTxt;
  const OdGePoint3d *pGripPoint3d = 0;
  OdGeLine3d line1, line2, lineText;
  pGripPoint3d = &gripPoints[indices[0]];
  OdGePoint3d ocsDimNewPt = *pGripPoint3d;
  OdGePoint3d dimNewPt = ocsDimNewPt;
  if ( bNeedTransform )
    ocsDimNewPt.transformBy(world2Plane);
  ocsDimNewPt.z = 0.0;

  for (int i = 0; i < (int)indices.size(); i++)
  {
    pGripPoint3d = &gripPoints[indices[i]];
    dimNewPt = *pGripPoint3d;
    ocsDimNewPt = dimNewPt;
    if (indices[i] < 4 && !pDimPtr->isUsingDefaultTextPosition())
    {
      pDimPtr->useDefaultTextPosition();
    }
    switch (indices[i])
    {
    case 0:
            pDimPtr->setXLine1Point(dimNewPt); 
            break;

    case 1:
            pDimPtr->setXLine2Point(dimNewPt);
            break;

        case 2:
            pDimPtr->setCenterPoint(dimNewPt);
            break;

        case 4:
        {
            OdGeVector3d v4 = ocsDimCenterPt - ocsDimArcNewPt;
            ocsDimArcNewPt = ocsDimCenterPt - v4;
            ocsDimTextPt = ocsDimNewPt;

            ocsDimTextPt.z = savedZCoord;
            if ( bNeedTransform )
              ocsDimTextPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));
            if (indices.size() == 1 || !bStretch)
              pDimPtr->useSetTextPosition();
            pDimPtr->setTextPosition(ocsDimTextPt);
            break;
        }
        case 3:
        {
            pDimPtr->setArcPoint(dimNewPt);
            break;
          /*
           if(indices[0] == 3) ocsDimArcNewPt = ocsDimNewPt;
           vX1 = ocsDimCenterPt - ocsDimLine1Pt;
           vX2 = ocsDimCenterPt - ocsDimLine2Pt;
           vArc = ocsDimCenterPt - ocsDimArcNewPt;

           vTxt = vX2;
           double angle1 = vX1.angleTo(vX2);
           double angle2 = vX1.angleTo(vArc);
           double angle3 = vX2.angleTo(vArc);

           if( angle2 <= angle1 && angle3 <= angle1 )
           {
               vTxt.rotateBy( angle1/2, OdGeVector3d::kZAxis);
               vX2.rotateBy( angle1/3, OdGeVector3d::kZAxis);
           }
           else
           {
               vTxt.rotateBy( angle1/2 + OdaPI, OdGeVector3d::kZAxis);
               vX2.rotateBy( Oda2PI/3, OdGeVector3d::kZAxis);
           }

           OdGeVector3d vY = vX2;
           angle1 = vArc.angleTo(vX2, OdGeVector3d::kZAxis);
           vY.rotateBy((OdaPI-angle1)/2, OdGeVector3d::kZAxis );

           line1.set(ocsDimArcNewPt, vY);
           line2.set(ocsDimCenterPt,  vX2 );
           line1.intersectWith(line2, ocsDimArcPt);

           ocsDimArcPt.z = savedZCoord;
           if ( bNeedTransform )
             ocsDimArcPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));
           pDimPtr->setArcPoint(ocsDimArcPt);
            break;
          */  
        }
        default:
            break;
    }
  }
  //pDimPtr->recordGraphicsModified(false);
  return eOk;
}

OdResult OdDbOrdinateDimGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints )const
{
  unsigned int size = gripPoints.size();
  gripPoints.resize( size + 4 );
  OdDbOrdinateDimensionPtr pDimPtr = ent;

  gripPoints[size + 0] = pDimPtr->definingPoint();
  gripPoints[size + 1] = pDimPtr->leaderEndPoint();
  gripPoints[size + 2] = pDimPtr->origin();
  gripPoints[size + 3] = pDimPtr->textPosition();

  return eOk;

}

OdResult OdDbOrdinateDimGripPointsPE::moveGripPoint( OdDbEntity* ent, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch)
{
  if ( indices.empty() ) return eOk;

  OdDbOrdinateDimensionPtr pDimPtr = ent;

  OdGePoint3d dimDefiningPt = pDimPtr->definingPoint();
  OdGePoint3d dimLeaderEndPt = pDimPtr->leaderEndPoint();
  OdGePoint3d dimOriginPt = pDimPtr->origin();
  OdGePoint3d dimTextPt = pDimPtr->textPosition();

  OdGeMatrix3d world2Plane(OdGeMatrix3d::worldToPlane(pDimPtr->normal()));
  OdGePoint3d ocsDimDefiningPt(dimDefiningPt), ocsDimLeaderEndPt(dimLeaderEndPt), 
      ocsDimTextPt(dimTextPt);

  OdGeVector3d vNorm = pDimPtr->normal();
  bool bNeedTransform = false;
  if ( vNorm != OdGeVector3d::kZAxis )
  {
    bNeedTransform = true;

    ocsDimDefiningPt.transformBy(world2Plane);
    ocsDimLeaderEndPt.transformBy(world2Plane);
    ocsDimTextPt.transformBy(world2Plane);
  }
  double savedZCoord = ocsDimDefiningPt.z;
  ocsDimDefiningPt.z = ocsDimLeaderEndPt.z = ocsDimTextPt.z = 0.0; 

  const OdGePoint3d *pGripPoint3d = 0;
  pGripPoint3d = &gripPoints[indices[0]];
  OdGePoint3d ocsDimNewPt = *pGripPoint3d;
  OdGePoint3d dimNewPt = ocsDimNewPt;
  if ( bNeedTransform )
    ocsDimNewPt.transformBy(world2Plane);
  ocsDimNewPt.z = 0.0;

  OdGeVector3d v1 = ocsDimTextPt - ocsDimLeaderEndPt;

  for (int i = 0; i < (int)indices.size(); i++)
  {
    pGripPoint3d = &gripPoints[indices[i]];
    dimNewPt = *pGripPoint3d;
    ocsDimNewPt = dimNewPt;
    switch (indices[i])
    {
    case 0:
        pDimPtr->setDefiningPoint(dimNewPt);
        break;

    case 1:
        v1.normalize();
        v1 *= OdGeVector3d(ocsDimTextPt - ocsDimLeaderEndPt).length();
        ocsDimTextPt = ocsDimNewPt + v1;

        ocsDimTextPt.z = savedZCoord;
        ocsDimNewPt.z = savedZCoord;
        if ( bNeedTransform )
        {
          ocsDimTextPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));
          ocsDimNewPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));
        }

        if ( !pDimPtr->isUsingDefaultTextPosition() && pDimPtr->dimtmove() == 2 )
          pDimPtr->setLeaderEndPoint(ocsDimNewPt);
        else
          pDimPtr->setTextPosition(ocsDimTextPt);
        break;

    case 2:
        pDimPtr->setOrigin(dimNewPt);
        break;

    case 3:
        v1.normalize();
        v1 *= OdGeVector3d(ocsDimTextPt - ocsDimLeaderEndPt).length();
        ocsDimLeaderEndPt = ocsDimNewPt - v1;
        ocsDimNewPt.z = savedZCoord;
        ocsDimLeaderEndPt.z = savedZCoord;
        if ( bNeedTransform )
        {
          ocsDimNewPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));
          ocsDimLeaderEndPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));
        }
        pDimPtr->setTextPosition(ocsDimNewPt);
        //pDimPtr->useSetTextPosition();
        if ( pDimPtr->isUsingDefaultTextPosition() && pDimPtr->dimtmove() != 2 )
          pDimPtr->setLeaderEndPoint(ocsDimLeaderEndPt);
        break;
    }
  }
  return eOk;
}


OdResult OdDb2LineAngularDimGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints )const
{

  unsigned int size = gripPoints.size();
  gripPoints.resize( size + 6 );

  OdDb2LineAngularDimensionPtr pDimPtr = ent;

  gripPoints[size + 0] = pDimPtr->xLine1Start();
  gripPoints[size + 1] = pDimPtr->xLine1End();
  gripPoints[size + 2] = pDimPtr->xLine2Start();
  gripPoints[size + 3] = pDimPtr->xLine2End();
  gripPoints[size + 4] = pDimPtr->arcPoint();
  gripPoints[size + 5] = pDimPtr->textPosition();

  return eOk;
}

OdResult OdDb2LineAngularDimGripPointsPE::moveGripPoint( OdDbEntity* ent, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch)
{

  if ( indices.empty() ) return eOk;

 //  unsigned int size = gripPoints.size();
  OdDb2LineAngularDimensionPtr pDimPtr = ent;

  OdGePoint3d dimLine1StartPt = pDimPtr->xLine1Start();
  OdGePoint3d dimLine1EndPt = pDimPtr->xLine1End();
  OdGePoint3d dimLine2StartPt = pDimPtr->xLine2Start();
  OdGePoint3d dimLine2EndPt = pDimPtr->xLine2End();
  OdGePoint3d dimArcPt = pDimPtr->arcPoint();
  OdGePoint3d dimTextPt = pDimPtr->textPosition();
  OdGePoint3d dimCenterPt;
  OdGePoint3d dimArcNewPt = dimArcPt;
  OdGePoint3d dimNewTextPt;

  OdGeMatrix3d world2Plane(OdGeMatrix3d::worldToPlane(pDimPtr->normal()));
  OdGePoint3d ocsDimLine1StartPt(dimLine1StartPt), ocsDimLine1EndPt(dimLine1EndPt), 
      ocsDimLine2StartPt(dimLine2StartPt), ocsDimLine2EndPt(dimLine2EndPt), ocsDimArcPt(dimArcPt),
      ocsDimTextPt(dimTextPt), ocsDimArcNewPt(dimArcNewPt);

  OdGeVector3d vNorm = pDimPtr->normal();
  bool bNeedTransform = false;
  if ( vNorm != OdGeVector3d::kZAxis )
  {
    bNeedTransform = true;

    ocsDimLine1StartPt.transformBy(world2Plane);
    ocsDimLine1EndPt.transformBy(world2Plane);
    ocsDimLine2StartPt.transformBy(world2Plane);
    ocsDimLine2EndPt.transformBy(world2Plane);
    ocsDimArcPt.transformBy(world2Plane);
    ocsDimTextPt.transformBy(world2Plane);
    ocsDimArcNewPt.transformBy(world2Plane);
  }
  double savedZCoord = ocsDimLine1StartPt.z;
  ocsDimLine1StartPt.z = ocsDimLine1EndPt.z = ocsDimLine2StartPt.z = ocsDimLine2EndPt.z = 
      ocsDimArcPt.z = ocsDimTextPt.z = ocsDimArcNewPt.z =0.0; 

  OdGeVector3d vX1, vX2, vArc, vTxt;
  const OdGePoint3d *pGripPoint3d = 0;
  OdGeLine3d line1, line2, lineText;
  pGripPoint3d = &gripPoints[indices[0]];
  OdGePoint3d ocsDimNewPt; // = *pGripPoint3d;
  OdGePoint3d dimNewPt; // = ocsDimNewPt;
  if ( bNeedTransform )
    ocsDimNewPt.transformBy(world2Plane);
  ocsDimNewPt.z = 0.0;

  for (int i = 0; i < (int)indices.size(); i++)
  {
    pGripPoint3d = &gripPoints[indices[i]];
    dimNewPt = *pGripPoint3d;
    ocsDimNewPt = dimNewPt;
    if (indices[i] < 5 && !pDimPtr->isUsingDefaultTextPosition())
    {
      pDimPtr->useDefaultTextPosition();
    }

    switch (indices[i])
    {
    case 0:
        pDimPtr->setXLine1Start(dimNewPt);
        break;

    case 1:
        pDimPtr->setXLine1End(dimNewPt);
        break;

    case 2:
        pDimPtr->setXLine2Start(dimNewPt);
        break;

    case 3:
        pDimPtr->setXLine2End(dimNewPt);
      break;
    case 4: case 5:
    {
        vX1 = ocsDimLine1EndPt - ocsDimLine1StartPt;
        vX2 = ocsDimLine2StartPt - ocsDimLine2EndPt; 
        line1.set(ocsDimLine1EndPt, vX1);
        line2.set(ocsDimLine2StartPt, vX2);
        line1.intersectWith(line2, dimCenterPt);

        double angle1 = vX2.angleTo(vX1);

        if( indices[0] == 4 )
        {
            ocsDimArcNewPt = ocsDimNewPt;
        }

        if( indices[0] == 5 )
        {

          OdGeVector3d vT = dimCenterPt - ocsDimArcNewPt;
          vT.normalize();
          vT *= fabs((OdGeVector3d(dimCenterPt - ocsDimNewPt)).length());
          ocsDimArcNewPt = dimCenterPt - vT;
          ocsDimTextPt = ocsDimNewPt;
        }

        vArc = dimCenterPt - ocsDimArcNewPt;

        double angle2 = vX1.angleTo(vArc);
        double angle3 = vX2.angleTo(vArc);
        OdGeVector3d vXA;

        if( OdEqual((angle3 - angle2), angle1, OdGeContext::gTol.equalPoint()))
        {
           vX2 = ocsDimLine2EndPt - ocsDimLine2StartPt;
        }

        if( OdEqual((angle2 - angle3), angle1, OdGeContext::gTol.equalPoint()))
        {
           vX1 = ocsDimLine1StartPt - ocsDimLine1EndPt;
        }

        if( OdEqual((Oda2PI - (angle3 + angle2)), angle1 , OdGeContext::gTol.equalPoint()))
        {
           vX2 = ocsDimLine2EndPt - ocsDimLine2StartPt;
           vX1 = ocsDimLine1StartPt - ocsDimLine1EndPt;
        }

        angle1 = vX2.angleTo(vX1);
        vXA = !isAngleDirectionBetweenVectors(vArc, vX1) ? vX1 : vX2;

        vTxt = vXA;
        vTxt.rotateBy( vX1.angleTo(vX2)/2, OdGeVector3d::kZAxis);
        vXA.rotateBy( angle1/3, OdGeVector3d::kZAxis);

        OdGeVector3d vY = vXA;
        angle1 = vArc.angleTo(vXA, OdGeVector3d::kZAxis);
        vY.rotateBy((OdaPI-angle1)/2, OdGeVector3d::kZAxis );

        line1.set(ocsDimArcNewPt, vY);
        line2.set(dimCenterPt,  vXA );
        OdGePoint3d intersecPt;
        line1.intersectWith(line2, intersecPt);
        ocsDimArcPt = intersecPt;

        OdGeVector3d vT1 = vTxt;
        vT1.normalize();
        vT1 *= OdGeVector3d(dimCenterPt - ocsDimArcPt).length();
        dimNewTextPt = dimCenterPt - vT1;
        if( indices[0] == 5 )
        {
           dimNewTextPt = ocsDimTextPt;
        }

        ocsDimArcPt.z = savedZCoord;
        dimNewTextPt.z = savedZCoord;
        if ( bNeedTransform)
        {
          ocsDimArcPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));
          dimNewTextPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));
        }
        if ( indices[0] == 4 || pDimPtr->dimtmove() == 0 )
          pDimPtr->setArcPoint(ocsDimArcPt);

        if( indices[0] == 5 )
        {
          if (indices.size() == 1 || !bStretch)
            pDimPtr->useSetTextPosition();
          pDimPtr->setTextPosition(dimNewTextPt);
        }
    }
    default:
        break;
    }
  }
  //pDimPtr->recordGraphicsModified(false);
  return eOk;
}


OdResult OdDbArcDimGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints )const
{

  unsigned int size = gripPoints.size();
  gripPoints.resize( size + 4 );

  OdDbArcDimensionPtr pDimPtr = ent;

  gripPoints[size + 0] = pDimPtr->xLine1Point();
  gripPoints[size + 1] = pDimPtr->xLine2Point();
  gripPoints[size + 2] = pDimPtr->arcPoint();
  gripPoints[size + 3] = pDimPtr->textPosition();

  return eOk;
}

OdResult OdDbArcDimGripPointsPE::moveGripPoint( OdDbEntity* ent, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch)
{

  if ( indices.empty() ) return eOk;

  // unsigned int size = gripPoints.size();
  OdDbArcDimensionPtr pDimPtr = ent;

  OdGePoint3d dimLine1Pt = pDimPtr->xLine1Point();
  OdGePoint3d dimLine2Pt = pDimPtr->xLine2Point();
  OdGePoint3d dimArcPt = pDimPtr->arcPoint();
  OdGePoint3d dimTextPt = pDimPtr->textPosition();
  OdGePoint3d dimCenterPt = pDimPtr->centerPoint();
  OdGePoint3d dimArcNewPt = dimArcPt;
  OdGePoint3d dimNewTextPt;

  OdGeMatrix3d world2Plane(OdGeMatrix3d::worldToPlane(pDimPtr->normal()));
  OdGePoint3d ocsDimLine1Pt(dimLine1Pt), 
      ocsDimLine2Pt(dimLine2Pt), ocsDimArcPt(dimArcPt),
      ocsDimTextPt(dimTextPt), ocsDimArcNewPt(dimArcNewPt),
      ocsDimCenterPt(dimCenterPt);

  OdGeVector3d vNorm = pDimPtr->normal();
  bool bNeedTransform = false;
  if ( vNorm != OdGeVector3d::kZAxis )
  {
    bNeedTransform = true;

    ocsDimLine1Pt.transformBy(world2Plane);
    ocsDimLine2Pt.transformBy(world2Plane);
    ocsDimArcPt.transformBy(world2Plane);
    ocsDimTextPt.transformBy(world2Plane);
    ocsDimArcNewPt.transformBy(world2Plane);
    ocsDimCenterPt.transformBy(world2Plane);
  }
  double savedZCoord = ocsDimLine1Pt.z;

  ocsDimLine1Pt.z = ocsDimLine2Pt.z = ocsDimArcPt.z = 
      ocsDimTextPt.z = ocsDimArcNewPt.z = ocsDimCenterPt.z = 0.0; 

  OdGeVector3d vX1, vX2, vArc, vTxt;
  const OdGePoint3d *pGripPoint3d = 0;
  OdGeLine3d line1, line2, lineText;
  pGripPoint3d = &gripPoints[indices[0]];
  OdGePoint3d ocsDimNewPt = *pGripPoint3d;
  OdGePoint3d dimNewPt = ocsDimNewPt;
  if  ( bNeedTransform )
    ocsDimNewPt.transformBy(world2Plane);
  ocsDimNewPt.z = 0.0;

  for (int i = 0; i < (int)indices.size(); i++)
  {
    pGripPoint3d = &gripPoints[indices[i]];
    dimNewPt = *pGripPoint3d;
    ocsDimNewPt = dimNewPt;

    if ( indices[i] < 3 && !pDimPtr->isUsingDefaultTextPosition() )
    {
      pDimPtr->useDefaultTextPosition();
    }

    if( indices[i] == 0 )
    {
        pDimPtr->setXLine1Point(*pGripPoint3d);
        // ocsDimLine1Pt = ocsDimNewPt;
        // ocsDimArcNewPt = ocsDimArcPt;
        // return eOk;
        continue;
    }

    if( indices[i] == 1  )
    {
        pDimPtr->setXLine2Point(*pGripPoint3d);
        // ocsDimLine2Pt = ocsDimNewPt;
        // ocsDimArcNewPt = ocsDimArcPt;
        continue;;
    }

    vX1 = ocsDimCenterPt - ocsDimLine1Pt;
    vX2 = ocsDimCenterPt - ocsDimLine2Pt; 

    double angle1 = vX2.angleTo(vX1);

    if( indices[i] == 2 )
    {
        ocsDimArcNewPt = ocsDimNewPt;
    }

    if( indices[i] == 3  )
    {
      if (indices.size() == 1 || !bStretch)
        pDimPtr->useSetTextPosition();
      OdGeVector3d vT = ocsDimCenterPt - ocsDimArcNewPt;
      vT.normalize();
      vT *= fabs((OdGeVector3d(ocsDimCenterPt - ocsDimNewPt)).length());
      ocsDimArcNewPt = ocsDimCenterPt - vT;
      ocsDimTextPt = ocsDimNewPt;
    }

    vArc = dimCenterPt - ocsDimArcNewPt;

    double angle2 = vX1.angleTo(vArc);
    double angle3 = vX2.angleTo(vArc);
    OdGeVector3d vXA;

    if( OdEqual((angle3 - angle2), angle1, OdGeContext::gTol.equalPoint()))
    {
       vX2 = ocsDimLine2Pt - ocsDimCenterPt; 
    }

    if( OdEqual((angle2 - angle3), angle1, OdGeContext::gTol.equalPoint()))
    {
       vX1 = ocsDimLine1Pt - ocsDimCenterPt; 
    }

    if( OdEqual((Oda2PI - (angle3 + angle2)), angle1 , OdGeContext::gTol.equalPoint()))
    {
       vX2 = ocsDimLine2Pt - ocsDimCenterPt;
       vX1 = ocsDimLine1Pt - ocsDimCenterPt;
    }

    angle1 = vX2.angleTo(vX1);
    vXA = !isAngleDirectionBetweenVectors(vArc, vX1) ? vX1 : vX2;

    vTxt = vXA;
    vTxt.rotateBy( vX1.angleTo(vX2)/2, OdGeVector3d::kZAxis);
    vXA.rotateBy( angle1/3, OdGeVector3d::kZAxis);

    OdGeVector3d vY = vXA;
    angle1 = vArc.angleTo(vXA, OdGeVector3d::kZAxis);
    vY.rotateBy((OdaPI-angle1)/2, OdGeVector3d::kZAxis );

    line1.set(ocsDimArcNewPt, vY);
    line2.set(dimCenterPt,  vXA );
    OdGePoint3d intersecPt;
    line1.intersectWith(line2, intersecPt);
    ocsDimArcPt = intersecPt;

    OdGeVector3d vT1 = vTxt;
    vT1.normalize();
    vT1 *= OdGeVector3d(dimCenterPt - ocsDimArcPt).length();
    dimNewTextPt = dimCenterPt - vT1;
    if( indices[i] == 5 )
    {
       dimNewTextPt = ocsDimTextPt;
    }

    ocsDimArcPt.z = savedZCoord;
    dimNewTextPt.z = savedZCoord;
    if ( bNeedTransform )
    {
      ocsDimArcPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));
      dimNewTextPt.transformBy(OdGeMatrix3d::planeToWorld(pDimPtr->normal()));
    }
    if ( indices[i] == 2 || pDimPtr->dimtmove() == 0 )
    {
      pDimPtr->setArcPoint(ocsDimArcPt);
    }
    pDimPtr->setTextPosition(dimNewTextPt);
  }

  //pDimPtr->recordGraphicsModified(false);

  return eOk;
}


OdResult OdDbRadialDimLargeGripPointsPE::getGripPoints( const OdDbEntity* ent, OdGePoint3dArray& gripPoints )const
{

  unsigned int size = gripPoints.size();
  gripPoints.resize( size + 4 );

  OdDbRadialDimensionLargePtr pDimPtr = ent;

  gripPoints[size + 0] = pDimPtr->chordPoint();
  gripPoints[size + 1] = pDimPtr->overrideCenter();
  gripPoints[size + 2] = pDimPtr->jogPoint();
  gripPoints[size + 3] = pDimPtr->textPosition();

  return eOk;
}

OdResult OdDbRadialDimLargeGripPointsPE::moveGripPoint( OdDbEntity* ent, const OdGePoint3dArray& gripPoints, const OdIntArray& indices, bool bStretch)
{
  if ( indices.empty() ) return eOk;

  // unsigned int size = gripPoints.size();
  OdDbRadialDimensionLargePtr pDimPtr = ent;


  OdGePoint3d dimChordPt          = pDimPtr->chordPoint();
  OdGePoint3d dimOverrideCenterPt = pDimPtr->overrideCenter();
  OdGePoint3d dimJogPt            = pDimPtr->jogPoint();
  OdGePoint3d dimTextPt           = pDimPtr->textPosition();
  OdGePoint3d dimCenterPt         = pDimPtr->center();
  double jogAngle                 = pDimPtr->jogAngle();

  OdGeVector3d vNorm = pDimPtr->normal();
  bool bNeedTransform = false;
  if ( vNorm != OdGeVector3d::kZAxis )
    bNeedTransform = true;

  OdGeMatrix3d world2Plane(OdGeMatrix3d::worldToPlane(vNorm));
  OdGeMatrix3d plane2World(OdGeMatrix3d::planeToWorld(vNorm));

  OdGePoint3d ocsDimChordPt(dimChordPt), ocsDimOverrideCenterPt(dimOverrideCenterPt), 
      ocsDimJogPt(dimJogPt), ocsDimTextPt(dimTextPt),
      ocsDimCenterPt(dimCenterPt);

  OdGeVector3d vX1, vX2, vArc, vTxt;
  const OdGePoint3d *pGripPoint3d = 0;
  OdGeLine3d line1, line2, lineText;
  pGripPoint3d = &gripPoints[indices[0]];
  OdGePoint3d ocsDimNewPt = *pGripPoint3d;
  OdGePoint3d dimNewPt = ocsDimNewPt;

  if ( bNeedTransform )
  {
    ocsDimChordPt.transformBy(world2Plane);
    ocsDimOverrideCenterPt.transformBy(world2Plane);
    ocsDimJogPt.transformBy(world2Plane);
    ocsDimTextPt.transformBy(world2Plane);
    ocsDimCenterPt.transformBy(world2Plane);

    ocsDimNewPt.transformBy(world2Plane);
  }
  double savedZCoord = ocsDimChordPt.z;

  ocsDimChordPt.z = ocsDimOverrideCenterPt.z = ocsDimJogPt.z = 
      ocsDimTextPt.z = ocsDimCenterPt.z = 0.0; 

  ocsDimNewPt.z = 0.0;


  if( indices[0] == 0 )
  {
    double dimLRadius = fabs((OdGeVector3d(ocsDimCenterPt - ocsDimChordPt)).length());

    OdGeVector3d vR = ocsDimCenterPt - ocsDimChordPt; 

    vR = ocsDimCenterPt - ocsDimNewPt;
    vR.normalize();
    vR *= dimLRadius;
    ocsDimNewPt = ocsDimCenterPt - vR;
    ocsDimChordPt = ocsDimNewPt;
    ocsDimNewPt.z = savedZCoord;
    if ( bNeedTransform )
      ocsDimNewPt.transformBy(plane2World);
    pDimPtr->setChordPoint(ocsDimNewPt);

    // correct text point
    vR.normalize();
    vR *= ocsDimTextPt.distanceTo(ocsDimCenterPt);
    ocsDimTextPt = ocsDimCenterPt - vR;
    ocsDimTextPt.z = savedZCoord;
    if ( bNeedTransform )
      ocsDimTextPt.transformBy(plane2World);

    pDimPtr->setTextPosition(ocsDimTextPt);
  }

  if( indices[0] == 1 )
  {
    ocsDimOverrideCenterPt = ocsDimNewPt;
    if ( bNeedTransform )
      ocsDimNewPt.transformBy(plane2World);
    pDimPtr->setOverrideCenter(ocsDimNewPt);
  }

  if ( indices[0] == 2 )
  {
    OdGeVector3d vR = ocsDimCenterPt - ocsDimChordPt; 
    vX1 = vR;
    vX1.rotateBy(jogAngle, OdGeVector3d::kZAxis);

    line1.set(ocsDimCenterPt, vR);
    line2.set(ocsDimNewPt,  vX1 );
    OdGePoint3d intersecPt;
    line1.intersectWith(line2, intersecPt);

    line1.set(ocsDimOverrideCenterPt,  vR );
    OdGePoint3d intersec2Pt;
    line1.intersectWith(line2, intersec2Pt);

    double dimLen = fabs((OdGeVector3d(intersecPt - intersec2Pt)).length());
    OdGeVector3d vR2 = intersecPt - intersec2Pt;
    vR2.normalize();
    vR2 *= dimLen/2;
    ocsDimJogPt = intersecPt - vR2;
    ocsDimJogPt.z = savedZCoord;

    if ( bNeedTransform )
      ocsDimJogPt.transformBy(plane2World);
    pDimPtr->setJogPoint(ocsDimJogPt);
  }

  if( indices[0] == 3 )
  {
    double dimLRadius = fabs((OdGeVector3d(ocsDimCenterPt - ocsDimChordPt)).length());

    OdGeVector3d vR = ocsDimCenterPt - ocsDimChordPt; 

    vR = ocsDimCenterPt - ocsDimNewPt;
    vR.normalize();
    vR *= dimLRadius;
    ocsDimChordPt = ocsDimCenterPt - vR;
    ocsDimChordPt.z = savedZCoord;
    if ( bNeedTransform )
      ocsDimChordPt.transformBy(plane2World);
    pDimPtr->setChordPoint(ocsDimChordPt);

    ocsDimTextPt = ocsDimNewPt;
    ocsDimTextPt.z = savedZCoord;
    if ( bNeedTransform )
      ocsDimTextPt.transformBy(plane2World);
    pDimPtr->setTextPosition(ocsDimTextPt);

    // correct jog point
    if (indices.size() == 1 || !bStretch)
      pDimPtr->useSetTextPosition();
  }

  if ( indices[0] != 2 )
  {
    // correct jog point
    double dimLen = fabs((OdGeVector3d(ocsDimChordPt - ocsDimOverrideCenterPt)).length());
    OdGeVector3d vR2 = ocsDimChordPt - ocsDimOverrideCenterPt;
    vR2.normalize();
    vR2 *= dimLen/2;
    ocsDimJogPt = ocsDimChordPt - vR2;
    ocsDimJogPt.z = savedZCoord;
    if( bNeedTransform )
      ocsDimJogPt.transformBy(plane2World);
    pDimPtr->setJogPoint(ocsDimJogPt);
  }
  return eOk;
}

const bool OdDbDimGripPointsPE ::isAngleDirectionBetweenVectors( OdGeVector3d v1, OdGeVector3d v2 )
{
  return v1.angleTo( v2, OdGeVector3d::kZAxis ) > OdaPI ? false : true;
}


OdResult OdDbDimGripPointsPE::getStretchPoints( const OdDbEntity* ent, OdGePoint3dArray& stretchPoints ) const
{
  //OdResult res = getGripPoints( ent, stretchPoints );
  //if ( res == eOk ) stretchPoints.resize( stretchPoints.size());
  //return res;
  return getGripPoints( ent, stretchPoints );
}

OdResult OdDbDimGripPointsPE::moveStretchPointsAt( OdDbEntity* ent, const OdIntArray& indices, const OdGeVector3d& offset )
{
  if (indices.empty()) return eOk;
  try
  {
    OdGePoint3dArray gripPoints;
    getGripPoints(ent, gripPoints);
    for (unsigned i = 0; i < indices.size(); ++i)
    {
      if (indices[i] < (int)gripPoints.size())
        gripPoints[indices[i]] += offset;
    }

    moveGripPoint(ent, gripPoints, indices, true);
    OdDbDimensionPtr pDimPtr = ent;
    pDimPtr->recomputeDimBlock();
  }
  catch (const OdError& e)
  {
    return e.code();
  }
  return eOk;
}


OdResult OdDbDimGripPointsPE::getOsnapPoints(const OdDbEntity* /*pEntity*/, 
                                             OdDb::OsnapMode /*osnapMode*/, 
                                             OdGsMarker /*gsSelectionMark*/, 
                                             const OdGePoint3d& /*pickPoint*/,
                                             const OdGePoint3d& /*lastPoint*/, 
                                             const OdGeMatrix3d& /*xWorldToEye*/, 
                                             OdGePoint3dArray& /*snapPoints*/) const
{
  return eOk;
}

