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
#include "DgRecomputeAssocPtsPEImpl.h"
#include "DgDatabase.h"
#include "DgElement.h"
#include "DgReferenceAttach.h"
#include "DgLine.h"
#include "DgLineString.h"
#include "DgShape.h"
#include "DgPointString.h"
#include "DgCurve.h"
#include "DgEllipse.h"
#include "DgArc.h"
#include "DgBSplineCurve.h"
#include "DgBSplineSurface.h"
#include "DgMultiline.h"
#include "DgCellHeader.h"
#include "DgText.h"
#include "DgTextNode.h"
#include "DgSharedCellReference.h"
#include "Ge/GeLine3d.h"
#include "Ge/GeLine2d.h"
#include "Ge/GePlane.h"
#include "DgBSplineSurface.h"
#include "DgMesh.h"
#include "DynamicLinker.h"
#include "DgElementIntersectionPE.h"

//----------------------------------------------------------
//
// OdDgRecomputeDimPtsPEImpl
//
//----------------------------------------------------------

OdResult getXRefTransformation(OdDgReferenceAttachmentHeaderPtr& pXRef, OdGeMatrix3d& matRet)
{
  OdDgModelPtr pModel = pXRef->getReferencedModel();

  if( pModel.isNull() )
    return eInvalidXrefObjectId;

  OdGeMatrix3d retVal = OdGeMatrix3d::translation(-(pXRef->getReferenceOrigin() + pModel->getInsertionBase().asVector()).asVector()
        * pModel->getMeasuresConversion(OdDgModel::kWuUnitOfResolution, pModel->getWorkingUnit()));

  retVal = OdGeMatrix3d::scaling(pXRef->getEntireScale()) * retVal;
  OdGeMatrix3d matRotate = pXRef->getTransformation();

  if( matRotate.isSingular() )
  {
    OdGeVector3d vrXAxis = matRotate.getCsXAxis();
    OdGeVector3d vrYAxis = matRotate.getCsYAxis();
    OdGeVector3d vrZAxis = matRotate.getCsYAxis();

    if (vrZAxis.isZeroLength())
    {
      if (!vrXAxis.isZeroLength() && !vrYAxis.isZeroLength())
        vrZAxis = vrXAxis.crossProduct(vrYAxis);
      else
        vrZAxis = OdGeVector3d::kZAxis;
    }

    if (vrXAxis.isZeroLength())
    {
      if (!vrYAxis.isZeroLength())
        vrXAxis = vrYAxis.crossProduct(vrZAxis);
      else
        vrXAxis = OdGeVector3d::kXAxis;
    }

    if (vrYAxis.isZeroLength())
      vrYAxis = vrZAxis.crossProduct(vrXAxis);

    matRotate.setCoordSystem(OdGePoint3d::kOrigin, vrXAxis, vrYAxis, vrZAxis);
  }

  retVal = matRotate * retVal;
  retVal = OdGeMatrix3d::translation(pXRef->getMasterOrigin().asVector()) * retVal;

  matRet = retVal;

  return eOk;
}

//================================================================================

OdDgElementPtr getAssociatedElement(OdDgDatabase* pDb, OdUInt64 uXRefId, OdUInt64 uElementId)
{
  OdDgElementPtr pRet;

  if( uXRefId != 0 )
  {
    OdDgElementId idXRef = pDb->getElementId(OdDbHandle(uXRefId));

    if( !idXRef.isNull() )
    {
      OdDgElementPtr pXRefElm = idXRef.openObject(OdDg::kForRead);

      if( !pXRefElm.isNull() && pXRefElm->isKindOf(OdDgReferenceAttachmentHeader::desc()) )
      {
        OdDgReferenceAttachmentHeaderPtr pXRef = pXRefElm;

        OdDgDatabasePtr pXRefDatabase = pXRef->getReferencedDatabase();

        if (!pXRefDatabase.isNull() && (uElementId != 0))
        {
          OdDgElementId idElm = pXRefDatabase->getElementId(OdDbHandle(uElementId));

          if( !idElm.isNull() )
            pRet = idElm.openObject(OdDg::kForRead);

          OdGeMatrix3d matTransform;

          if( getXRefTransformation(pXRef, matTransform) == eOk )
          {
            if( matTransform != OdGeMatrix3d::kIdentity )
            {
              pRet = pRet->clone();
              pRet->transformBy(matTransform);
            }
          }
          else
            pRet = NULL;
        }
      }
    }
  }
  else if( uElementId != 0 )
  {
    OdDgElementId idElm = pDb->getElementId(OdDbHandle(uElementId));

    if (!idElm.isNull())
      pRet = idElm.openObject(OdDg::kForRead);
  }

  return pRet;
}

//================================================================================

OdResult getLinearOrProjectionAssocPoint( const OdDgElementPtr& pElm,
                                            OdUInt32 uIndex,
                                              OdUInt32 nElmVertices,
                                                double dFraction,
                                                  OdGePoint3d& ptRet )
{
  OdGePoint3dArray arrPts;
  OdUInt32 nVertices = 0;

  if (pElm->isKindOf(OdDgLine2d::desc()))
  {
    OdDgLine2dPtr pLine2d = pElm;

    OdGePoint2d ptStart = pLine2d->getStartPoint();
    OdGePoint2d ptEnd = pLine2d->getEndPoint();

    arrPts.push_back(OdGePoint3d(ptStart.x, ptStart.y, 0.0));
    arrPts.push_back(OdGePoint3d(ptEnd.x, ptEnd.y, 0.0));
  }
  else if (pElm->isKindOf(OdDgMultiVertex2d::desc()) && !pElm->isKindOf(OdDgCurve2d::desc()))
  {
    OdDgMultiVertex2dPtr pMultiVertex2d = pElm;

    for (OdUInt32 i = 0; i < pMultiVertex2d->getVerticesCount(); i++)
    {
      OdGePoint2d ptVertex = pMultiVertex2d->getVertexAt(i);
      arrPts.push_back(OdGePoint3d(ptVertex.x, ptVertex.y, 0.0));
    }
  }
  else if (pElm->isKindOf(OdDgBSplineCurve2d::desc()))
  {
    OdDgBSplineCurve2dPtr pSpline2d = pElm;

    if (pSpline2d->hasFitData())
    {
      OdGePoint2dArray arrPts2d;
      OdUInt32 uOrder;
      bool bTangents;
      OdGeVector2d vrStartTg, vrEndTg;

      if (pSpline2d->getFitData(arrPts2d, uOrder, bTangents, vrStartTg, vrEndTg) == eOk)
      {
        for (OdUInt32 i = 0; i < arrPts2d.size(); i++)
          arrPts.push_back(OdGePoint3d(arrPts2d[i].x, arrPts2d[i].y, 0.0));

        nVertices = pSpline2d->numControlPoints();
      }
    }

    if (arrPts.isEmpty())
    {
      OdGePoint2dArray arrPts2d;
      OdUInt32 uOrder;
      bool bRational, bClosed;
      OdGeKnotVector vrKnots;
      OdGeDoubleArray arrWeights;

      if (pSpline2d->getNurbsData(uOrder, bRational, bClosed, arrPts2d, vrKnots, arrWeights) == eOk)
      {
        for (OdUInt32 i = 0; i < arrPts2d.size(); i++)
          arrPts.push_back(OdGePoint3d(arrPts2d[i].x, arrPts2d[i].y, 0.0));
      }
    }
  }
  else if (pElm->isKindOf(OdDgLine3d::desc()))
  {
    OdDgLine3dPtr pLine3d = pElm;

    arrPts.push_back(pLine3d->getStartPoint());
    arrPts.push_back(pLine3d->getEndPoint());
  }
  else if (pElm->isKindOf(OdDgMultiVertex3d::desc()) && !pElm->isKindOf(OdDgCurve3d::desc()))
  {
    OdDgMultiVertex3dPtr pMultiVertex3d = pElm;

    for (OdUInt32 i = 0; i < pMultiVertex3d->getVerticesCount(); i++)
      arrPts.push_back(pMultiVertex3d->getVertexAt(i));
  }
  else if (pElm->isKindOf(OdDgBSplineCurve3d::desc()))
  {
    OdDgBSplineCurve3dPtr pSpline3d = pElm;

    if (pSpline3d->hasFitData())
    {
      OdGePoint3dArray arrPts3d;
      OdUInt32 uOrder;
      bool bTangents;
      OdGeVector3d vrStartTg, vrEndTg;

      if (pSpline3d->getFitData(arrPts3d, uOrder, bTangents, vrStartTg, vrEndTg) == eOk)
      {
        arrPts = arrPts3d;
        nVertices = pSpline3d->numControlPoints();
      }
    }

    if (arrPts.isEmpty())
    {
      OdGePoint3dArray arrPts3d;
      OdUInt32 uOrder;
      bool bRational, bClosed;
      OdGeKnotVector vrKnots;
      OdGeDoubleArray arrWeights;

      if (pSpline3d->getNurbsData(uOrder, bRational, bClosed, arrPts3d, vrKnots, arrWeights) == eOk)
        arrPts = arrPts3d;
    }
  }
  else
    return eNotThatKindOfClass;

  if (nVertices == 0)
    nVertices = arrPts.size();

  if (nVertices != nElmVertices || (arrPts.size() <= uIndex))
    return eInvalidInput;

  OdGePoint3d ptBase = arrPts[uIndex];

  if( OdZero(dFraction) || (uIndex == arrPts.size() - 1))
    ptRet = ptBase;
  else
  {
    OdGeVector3d vrOffset = arrPts[uIndex + 1] - ptBase;
    ptRet = ptBase + vrOffset * dFraction;
  }

  return eOk;
}

//================================================================================

OdResult getLinearAssocPoint(OdDgDatabase* pDb, 
                               const OdDgLinearAssociationPtr& pDimAssoc, 
                                 OdGePoint3d& ptRet )
{
  OdDgElementPtr pElm = getAssociatedElement(pDb, pDimAssoc->getRefAttachmentId(), pDimAssoc->getElementId());

  if( pElm.isNull() )
    return eNullObjectPointer;

  return getLinearOrProjectionAssocPoint(pElm, pDimAssoc->getVertexNumber(), pDimAssoc->getNVertices(), pDimAssoc->getNumerator()*1.0 / pDimAssoc->getDivisor(), ptRet);
}

//================================================================================

OdResult getProjectionAssocPoint(OdDgDatabase* pDb,
                                   const OdDgProjectionAssociationPtr& pDimAssoc,
                                     OdGePoint3d& ptRet)
{
  OdDgElementPtr pElm = getAssociatedElement(pDb, pDimAssoc->getRefAttachmentId(), pDimAssoc->getElementId());

  if (pElm.isNull())
    return eNullObjectPointer;

  return getLinearOrProjectionAssocPoint(pElm, pDimAssoc->getVertexNumber(), pDimAssoc->getNVertices(), pDimAssoc->getFraction(), ptRet);
}

//================================================================================

OdResult getArcAssocPoint(OdDgDatabase* pDb,
                            const OdDgArcAssociationPtr& pDimAssoc,
                              OdGePoint3d& ptRet)
{
  OdDgElementPtr pElm = getAssociatedElement(pDb, pDimAssoc->getRefAttachmentId(), pDimAssoc->getElementId());

  if (pElm.isNull())
    return eNullObjectPointer;

  if( pElm->isKindOf(OdDgEllipse2d::desc()) )
  {
    OdDgEllipse2dPtr pEllipse2d = pElm;

    switch( pDimAssoc->getKeyPoint() )
    {
      case OdDgArcAssociation::kCenterPoint:
      {
        OdGePoint2d ptRet2d = pEllipse2d->getOrigin();
        ptRet.set(ptRet2d.x, ptRet2d.y, 0.0);
      } break;

      case OdDgArcAssociation::kStartPoint:
      case OdDgArcAssociation::kEndPoint:
      {
        OdGePoint2d ptRet2d;
        pEllipse2d->getStartPoint(ptRet2d);
        ptRet.set(ptRet2d.x, ptRet2d.y, 0.0);
      } break;

      case OdDgArcAssociation::kAnglePoint:
      {
        OdGePoint2d ptRet2d;
        pEllipse2d->getPointAtParam(pDimAssoc->getAngle(), ptRet2d);
        ptRet.set(ptRet2d.x, ptRet2d.y, 0.0);
      } break;
    }
  }
  else if (pElm->isKindOf(OdDgEllipse3d::desc()))
  {
    OdDgEllipse3dPtr pEllipse3d = pElm;

    switch (pDimAssoc->getKeyPoint())
    {
      case OdDgArcAssociation::kCenterPoint:
        pEllipse3d->getOrigin(ptRet);
      break;

      case OdDgArcAssociation::kStartPoint:
      case OdDgArcAssociation::kEndPoint:
        pEllipse3d->getStartPoint(ptRet);
      break;

      case OdDgArcAssociation::kAnglePoint:
        pEllipse3d->getPointAtParam(pDimAssoc->getAngle(), ptRet);
      break;
    }
  }
  else if (pElm->isKindOf(OdDgArc2d::desc()))
  {
    OdDgArc2dPtr pArc2d = pElm;

    switch (pDimAssoc->getKeyPoint())
    {
      case OdDgArcAssociation::kCenterPoint:
      {
        OdGePoint2d ptRet2d = pArc2d->getOrigin();
        ptRet.set(ptRet2d.x, ptRet2d.y, 0.0);
      } break;

      case OdDgArcAssociation::kStartPoint:
      {
        OdGePoint2d ptRet2d;
        pArc2d->getStartPoint(ptRet2d);
        ptRet.set(ptRet2d.x, ptRet2d.y, 0.0);
      } break;

      case OdDgArcAssociation::kEndPoint:
      {
        OdGePoint2d ptRet2d;
        pArc2d->getEndPoint(ptRet2d);
        ptRet.set(ptRet2d.x, ptRet2d.y, 0.0);
      } break;

      case OdDgArcAssociation::kAnglePoint:
      {
        OdGePoint2d ptRet2d;
        pArc2d->getPointAtParam(pDimAssoc->getAngle(), ptRet2d);
        ptRet.set(ptRet2d.x, ptRet2d.y, 0.0);
      } break;
    }
  }
  else if (pElm->isKindOf(OdDgArc3d::desc()))
  {
    OdDgArc3dPtr pArc3d = pElm;

    switch (pDimAssoc->getKeyPoint())
    {
      case OdDgArcAssociation::kCenterPoint:
        pArc3d->getOrigin(ptRet);
        break;

      case OdDgArcAssociation::kStartPoint:
        pArc3d->getStartPoint(ptRet);
        break;

      case OdDgArcAssociation::kEndPoint:
        pArc3d->getEndPoint(ptRet);
        break;

      case OdDgArcAssociation::kAnglePoint:
        pArc3d->getPointAtParam(pDimAssoc->getAngle(), ptRet);
        break;
    }
  }
  else
    return eNotThatKindOfClass;

  return eOk;
}

//================================================================================

OdResult getBSplineAssocPoint(OdDgDatabase* pDb,
                                const OdDgBSplineCurveAssociationPtr& pDimAssoc,
                                  OdGePoint3d& ptRet)
{
  OdDgElementPtr pElm = getAssociatedElement(pDb, pDimAssoc->getRefAttachmentId(), pDimAssoc->getElementId());

  if (pElm.isNull())
    return eNullObjectPointer;

  OdResult retVal = eOk;

  if (pElm->isKindOf(OdDgBSplineCurve2d::desc()))
  {
    OdDgBSplineCurve2dPtr pCurve2d = pElm;

    OdGePoint2d ptRet2d;
    retVal = pCurve2d->getPointAtParam(pDimAssoc->getParam(), ptRet2d);

    if (retVal == eOk)
      ptRet.set(ptRet2d.x, ptRet2d.y, 0.0);
  }
  else if (pElm->isKindOf(OdDgBSplineCurve3d::desc()))
  {
    OdDgBSplineCurve3dPtr pCurve3d = pElm;
    retVal = pCurve3d->getPointAtParam(pDimAssoc->getParam(), ptRet);
  }
  else if (pElm->isKindOf(OdDgCurve2d::desc()))
  {
    OdDgCurve2dPtr pCurve2d = pElm;

    OdGePoint2d ptRet2d;
    retVal = pCurve2d->getPointAtParam(pDimAssoc->getParam(), ptRet2d);

    if (retVal == eOk)
      ptRet.set(ptRet2d.x, ptRet2d.y, 0.0);
  }
  else if (pElm->isKindOf(OdDgCurve3d::desc()))
  {
    OdDgCurve3dPtr pCurve3d = pElm;
    retVal = pCurve3d->getPointAtParam(pDimAssoc->getParam(), ptRet);
  }
  else
    return eNotThatKindOfClass;

  return retVal;
}

//================================================================================

OdResult getMLineProfilePoint(const OdDgMultilinePtr& pMLine, 
                                   const OdGeVector3d& vrNormal,
                                     OdUInt32 uVertexNumber, 
                                       double dOffset,
                                         OdGePoint3d& ptRet)
{
  OdResult retVal = eOk;

  if( OdZero(dOffset) )
  {
    OdDgMultilinePoint ptMLPoint;
    pMLine->getPoint(uVertexNumber, ptMLPoint);
    ptMLPoint.getPoint(ptRet);
  }
  else if( uVertexNumber == 0 )
  {
    OdDgMultilinePoint ptMLSeg0;
    OdDgMultilinePoint ptMLSeg1;

    pMLine->getPoint(0, ptMLSeg0);
    pMLine->getPoint(1, ptMLSeg1);

    OdGePoint3d ptSegStart, ptSegEnd;

    ptMLSeg0.getPoint(ptSegStart);
    ptMLSeg1.getPoint(ptSegEnd);

    OdGeVector3d vrStartEnd = ptSegEnd - ptSegStart;
    OdGeVector3d vrOffset = vrStartEnd;
    double dStartAngle = pMLine->getOriginCapAngle();
    vrOffset.rotateBy(dStartAngle, vrNormal);
    vrOffset.normalize();

    if( !vrStartEnd.isZeroLength() && !OdEqual(dStartAngle, OdaPI))
      ptRet = ptSegStart + vrOffset * dOffset / sin(dStartAngle);
    else
      retVal = eDegenerateGeometry;
  }
  else if (uVertexNumber == pMLine->getPointsCount() - 1)
  {
    OdDgMultilinePoint ptMLSeg0;
    OdDgMultilinePoint ptMLSeg1;

    pMLine->getPoint(pMLine->getPointsCount() - 2, ptMLSeg0);
    pMLine->getPoint(pMLine->getPointsCount() - 1, ptMLSeg1);

    OdGePoint3d ptSegStart, ptSegEnd;

    ptMLSeg0.getPoint(ptSegStart);
    ptMLSeg1.getPoint(ptSegEnd);

    OdGeVector3d vrStartEnd = ptSegEnd - ptSegStart;
    OdGeVector3d vrOffset = vrStartEnd;

    double dEndAngle = pMLine->getOriginCapAngle();

    vrOffset.rotateBy(dEndAngle, vrNormal);
    vrOffset.normalize();

    if (!vrStartEnd.isZeroLength() && !OdEqual(dEndAngle, OdaPI) )
      ptRet = ptSegEnd + vrOffset * dOffset / sin(dEndAngle);
    else
      retVal = eDegenerateGeometry;
  }
  else
  {
    OdDgMultilinePoint ptMLSeg0;
    OdDgMultilinePoint ptMLSeg1;
    OdDgMultilinePoint ptMLSeg2;

    pMLine->getPoint(uVertexNumber - 1, ptMLSeg0);
    pMLine->getPoint(uVertexNumber, ptMLSeg1);
    pMLine->getPoint(uVertexNumber + 1, ptMLSeg2);

    OdGePoint3d ptSeg0, ptSeg1, ptSeg2;

    ptMLSeg0.getPoint(ptSeg0);
    ptMLSeg1.getPoint(ptSeg1);
    ptMLSeg2.getPoint(ptSeg2);

    OdGeVector3d vrSeg1 = ptSeg1 - ptSeg0;
    OdGeVector3d vrOffset1 = vrSeg1;
    OdGeVector3d vrSeg2 = ptSeg2 - ptSeg1;
    OdGeVector3d vrOffset2 = vrSeg2;

    if (!vrOffset1.isZeroLength() && !vrOffset2.isZeroLength())
    {
      vrOffset1.normalize();
      vrOffset2.normalize();

      OdGeVector3d averagedDirection = (vrOffset1 + vrOffset2) / 2.;

      if( averagedDirection.length() < .001 )
        averagedDirection = vrOffset1.crossProduct(vrNormal);

      averagedDirection.normalize();

      OdGeVector3d vrOffset = vrNormal.crossProduct(averagedDirection).normal();
      OdGeVector3d auxiliaryVector = vrNormal.crossProduct(vrOffset1);
      double half = auxiliaryVector.dotProduct(vrOffset);

      if( half < .01 )
        half = .01;

      vrOffset *= 1. / half;

      ptRet = ptSeg1 + vrOffset * dOffset;
    }
    else
      retVal = eDegenerateGeometry;
  }

  return retVal;
}

//================================================================================

OdResult getMultilineAssocPoint(OdDgDatabase* pDb,
                                  const OdDgMultilineAssociationPtr& pDimAssoc,
                                     OdGePoint3d& ptRet)
{
  OdDgElementPtr pElm = getAssociatedElement(pDb, pDimAssoc->getRefAttachmentId(), pDimAssoc->getElementId());

  if (pElm.isNull())
    return eNullObjectPointer;

  if (pElm->isKindOf(OdDgMultiline::desc()))
  {
    OdDgMultilinePtr pMLine = pElm;

    if ((pDimAssoc->getLineNumber() >= pMLine->getProfilesCount()) ||
      (pDimAssoc->getVertexNumber() >= pMLine->getPointsCount())
      )
    {
      return eInvalidInput;
    }

    OdGeVector3d vrNormal = pMLine->getZVector();

    OdDgMultilineProfile lineData;
    pMLine->getProfile(pDimAssoc->getLineNumber(), lineData);

    if( pDimAssoc->getJointFlag() )
    {
      return getMLineProfilePoint(pMLine, vrNormal, pDimAssoc->getVertexNumber(), lineData.getDistance(), ptRet);
    }
    else
    {
      if( pDimAssoc->getProjectFlag() )
      {
        OdDgMultilinePoint ptMLStart;
        OdDgMultilinePoint ptMLEnd;

        if (pDimAssoc->getVertexNumber() < pMLine->getPointsCount() - 1)
        {
          pMLine->getPoint(pDimAssoc->getVertexNumber(), ptMLStart);
          pMLine->getPoint(pDimAssoc->getVertexNumber() + 1, ptMLEnd);
        }
        else if (pDimAssoc->getVertexNumber() == pMLine->getPointsCount() - 1)
        {
          pMLine->getPoint(pDimAssoc->getVertexNumber(), ptMLStart);
          pMLine->getPoint(pDimAssoc->getVertexNumber() - 1, ptMLEnd);
        }

        OdGePoint3d ptStart, ptEnd;

        ptMLStart.getPoint(ptStart);
        ptMLEnd.getPoint(ptEnd);

        OdGeVector3d vrStartEnd = ptEnd - ptStart;
        OdGeVector3d vrOffset = vrStartEnd;
        vrOffset.rotateBy(OdaPI2, vrNormal);
        vrOffset.normalize();

        if (!vrStartEnd.isZeroLength())
          ptRet = ptStart + vrStartEnd * pDimAssoc->getOffset() + vrOffset * lineData.getDistance();
        else
          return eInvalidInput;
      }
      else
      {
        OdGePoint3d ptStart, ptEnd;

        if( pDimAssoc->getVertexNumber() == pMLine->getPointsCount() - 1 )
        {
          OdResult retVal = getMLineProfilePoint(pMLine, vrNormal, pDimAssoc->getVertexNumber(), lineData.getDistance(), ptStart);

          if (retVal == eOk)
            ptRet = ptStart;

          return retVal;
        }
        else
        {
          OdResult retVal1 = getMLineProfilePoint(pMLine, vrNormal, pDimAssoc->getVertexNumber(), lineData.getDistance(), ptStart);
          OdResult retVal2 = getMLineProfilePoint(pMLine, vrNormal, pDimAssoc->getVertexNumber() + 1, lineData.getDistance(), ptEnd);

          if ((retVal1 == eOk) && (retVal2 == eOk))
          {
            OdGeVector3d vrStartEnd = ptEnd - ptStart;
            ptRet = ptStart + vrStartEnd * pDimAssoc->getOffset();
            return eOk;
          }
          else
            return eDegenerateGeometry;
        }
      }
    }
  }
  else
    return eNotThatKindOfClass;

  return eOk;
}

//================================================================================

OdResult getOriginAssocPoint(OdDgDatabase* pDb,
                               const OdDgOriginAssociationPtr& pDimAssoc,
                                 OdGePoint3d& ptRet)
{
  OdDgElementPtr pElm = getAssociatedElement(pDb, pDimAssoc->getRefAttachmentId(), pDimAssoc->getElementId());

  if (pElm.isNull())
    return eNullObjectPointer;

  if (pElm->isKindOf(OdDgCellHeader2d::desc()))
  {
    OdDgCellHeader2dPtr pCell = pElm;
    OdGePoint2d ptRet2d = pCell->getOrigin();
    ptRet.set(ptRet2d.x, ptRet2d.y, 0.0);
  }
  else if (pElm->isKindOf(OdDgCellHeader3d::desc()))
  {
    OdDgCellHeader3dPtr pCell = pElm;
    ptRet = pCell->getOrigin();
  }
  else if (pElm->isKindOf(OdDgSharedCellReference::desc()))
  {
    OdDgSharedCellReferencePtr pCell = pElm;
    ptRet = pCell->getOrigin();
  }
  else if (pElm->isKindOf(OdDgTextNode2d::desc()))
  {
    OdDgTextNode2dPtr pTextNode = pElm;
    OdGePoint2d ptRet2d = pTextNode->getOrigin();
    ptRet.set(ptRet2d.x, ptRet2d.y, 0.0);
  }
  else if (pElm->isKindOf(OdDgTextNode3d::desc()))
  {
    OdDgTextNode3dPtr pTextNode = pElm;
    ptRet = pTextNode->getOrigin();
  }
  else if (pElm->isKindOf(OdDgText2d::desc()))
  {
    OdDgText2dPtr pText = pElm;

    if (pDimAssoc->getTextOriginOption() == OdDgOriginAssociation::kInsertionPoint)
    {
      OdGePoint2d ptRet2d = pText->getPosition();
      ptRet.set(ptRet2d.x, ptRet2d.y, 0.0);
    }
    else
    {
      OdGePoint2d ptRet2d = pText->getOrigin();
      ptRet.set(ptRet2d.x, ptRet2d.y, 0.0);
    }
  }
  else if (pElm->isKindOf(OdDgText3d::desc()))
  {
    OdDgText3dPtr pText = pElm;

    if (pDimAssoc->getTextOriginOption() == OdDgOriginAssociation::kInsertionPoint)
      ptRet = pText->getPosition();
    else
      ptRet = pText->getOrigin();
  }
  else
    return eNotThatKindOfClass;

  return eOk;
}

//================================================================================

OdResult getBSplineSurfaceAssocPoint(OdDgDatabase* pDb,
                                       const OdDgBSplineSurfaceAssociationPtr& pDimAssoc,
                                         OdGePoint3d& ptRet)
{
  OdDgElementPtr pElm = getAssociatedElement(pDb, pDimAssoc->getRefAttachmentId(), pDimAssoc->getElementId());

  if (pElm.isNull())
    return eNullObjectPointer;

  if (pElm->isKindOf(OdDgBSplineSurface::desc()))
  {
    OdDgBSplineSurfacePtr pSpline = pElm;
    return pSpline->evaluate(pDimAssoc->getUParam(), pDimAssoc->getVParam(), ptRet);
  }
  else
    return eNotThatKindOfClass;

  return eOk;
}

//================================================================================

OdResult getMeshVertexAssocPoint( OdDgDatabase* pDb,
                                    const OdDgMeshVertexAssociationPtr& pDimAssoc,
                                      OdGePoint3d& ptRet)
{
  OdDgElementPtr pElm = getAssociatedElement(pDb, pDimAssoc->getRefAttachmentId(), pDimAssoc->getElementId());

  if (pElm.isNull())
    return eNullObjectPointer;

  if (pElm->isKindOf(OdDgMeshFaceLoops::desc()))
  {
    OdDgMeshFaceLoopsPtr pMesh = pElm;
    
    if (pMesh->getPointsNumber() > pDimAssoc->getVertexIndex())
      ptRet = pMesh->getPoint(pDimAssoc->getVertexIndex());
    else
      return eInvalidIndex;
  }
  else if (pElm->isKindOf(OdDgMeshPointCloud::desc()))
  {
    OdDgMeshPointCloudPtr pMesh = pElm;

    if (pMesh->getPointsNumber() > pDimAssoc->getVertexIndex())
      ptRet = pMesh->getPoint(pDimAssoc->getVertexIndex());
    else
      return eInvalidIndex;
  } 
  else if (pElm->isKindOf(OdDgMeshTriangleList::desc()))
  {
    OdDgMeshTriangleListPtr pMesh = pElm;

    if( pMesh->getTrianglesNumber() * 3 > pDimAssoc->getVertexIndex() )
    {
      OdDgMeshTriangleList::Triangle curTriangle;
      pMesh->getTriangle(pDimAssoc->getVertexIndex() / 3, curTriangle);
      ptRet = curTriangle.m_points[pDimAssoc->getVertexIndex()%3];
    }
    else
      return eInvalidIndex;
  }
  else if (pElm->isKindOf(OdDgMeshQuadList::desc()))
  {
    OdDgMeshQuadListPtr pMesh = pElm;

    if (pMesh->getQuadsNumber() * 4 > pDimAssoc->getVertexIndex())
    {
      OdDgMeshQuadList::Quad curQuard;
      pMesh->getQuad(pDimAssoc->getVertexIndex() / 4, curQuard);
      ptRet = curQuard.m_points[pDimAssoc->getVertexIndex() % 4];
    }
    else
      return eInvalidIndex;
  }
  else if (pElm->isKindOf(OdDgMeshTriangleGrid::desc()))
  {
    OdDgMeshTriangleGridPtr pMesh = pElm;

    if( pMesh->getColumnsNumber() * pMesh->getRowsNumber() > pDimAssoc->getVertexIndex())
    {
      OdUInt32 iRows = pDimAssoc->getVertexIndex() / pMesh->getColumnsNumber();
      OdUInt32 iColumns = pDimAssoc->getVertexIndex() % pMesh->getColumnsNumber();
      ptRet = pMesh->getPoint(iRows, iColumns);
    }
    else
      return eInvalidIndex;
  }
  else if (pElm->isKindOf(OdDgMeshQuadGrid::desc()))
  {
    OdDgMeshQuadGridPtr pMesh = pElm;

    if (pMesh->getColumnsNumber() * pMesh->getRowsNumber() > pDimAssoc->getVertexIndex())
    {
      OdUInt32 iRows = pDimAssoc->getVertexIndex() / pMesh->getColumnsNumber();
      OdUInt32 iColumns = pDimAssoc->getVertexIndex() % pMesh->getColumnsNumber();
      ptRet = pMesh->getPoint(iRows, iColumns);
    }
    else
      return eInvalidIndex;
  }
  else
    return eNotThatKindOfClass;

  return eOk;
}

//================================================================================

OdResult getMeshEdgeAssocPoint( OdDgDatabase* pDb,
                                  const OdDgMeshEdgeAssociationPtr& pDimAssoc,
                                    OdGePoint3d& ptRet)
{
  OdDgElementPtr pElm = getAssociatedElement(pDb, pDimAssoc->getRefAttachmentId(), pDimAssoc->getElementId());

  if (pElm.isNull())
    return eNullObjectPointer;

  OdGePoint3d ptStart, ptEnd;
  bool bCorrectEdge = false;

  if (pElm->isKindOf(OdDgMeshFaceLoops::desc()))
  {
    OdDgMeshFaceLoopsPtr pMesh = pElm;

    OdUInt32 nEdges = 0;
    OdUInt32 uEdge = pDimAssoc->getEdgeIndex();

    for( OdUInt32 i = 0; i < pMesh->getFacesNumber(); i++ )
    {
      OdDgMeshFaceLoops::VerticesArray faceData;
      pMesh->getFace(i, faceData);

      if (uEdge - nEdges < faceData.size() )
      {
        ptStart = pMesh->getPoint(abs(faceData[uEdge - nEdges].m_pointIndex));

        if(uEdge - nEdges == faceData.size() - 1 )
          ptEnd = pMesh->getPoint(abs(faceData[0].m_pointIndex));
        else
          ptEnd = pMesh->getPoint(abs(faceData[pDimAssoc->getEdgeIndex() - nEdges + 1].m_pointIndex));

        bCorrectEdge = true;
        break;
      }
      else
        nEdges += faceData.size() + 1;
    }
  }
  else if (pElm->isKindOf(OdDgMeshPointCloud::desc()))
  {
    OdDgMeshPointCloudPtr pMesh = pElm;

    if (pMesh->getPointsNumber() > pDimAssoc->getEdgeIndex())
    {
      ptStart = pMesh->getPoint(pDimAssoc->getEdgeIndex());

      if(pDimAssoc->getEdgeIndex() == pMesh->getPointsNumber() - 1)
        ptEnd = pMesh->getPoint(0);
      else
        ptEnd = pMesh->getPoint(pDimAssoc->getEdgeIndex() + 1);

      bCorrectEdge = true;
    }
  }
  else if (pElm->isKindOf(OdDgMeshTriangleList::desc()))
  {
    OdDgMeshTriangleListPtr pMesh = pElm;

    if (pMesh->getTrianglesNumber() * 3 > pDimAssoc->getEdgeIndex())
    {
      OdUInt32 nEdges = 0;

      for( OdUInt32 i = 0; i < pMesh->getTrianglesNumber(); i++ )
      {
        if (pDimAssoc->getEdgeIndex() < nEdges + 3)
        {
          OdDgMeshTriangleList::Triangle curTriangle;
          pMesh->getTriangle(i, curTriangle);

          ptStart = curTriangle.m_points[pDimAssoc->getEdgeIndex() - nEdges];

          if (pDimAssoc->getEdgeIndex() - nEdges == 2)
            ptEnd = curTriangle.m_points[0];
          else
            ptEnd = curTriangle.m_points[pDimAssoc->getEdgeIndex() - nEdges + 1];

          bCorrectEdge = true;
          break;
        }
        else
          nEdges += 3;
      }
    }
  }
  else if (pElm->isKindOf(OdDgMeshQuadList::desc()))
  {
    OdDgMeshQuadListPtr pMesh = pElm;

    if (pMesh->getQuadsNumber() * 4 > pDimAssoc->getEdgeIndex())
    {
      OdUInt32 nEdges = 0;

      for (OdUInt32 i = 0; i < pMesh->getQuadsNumber(); i++)
      {
        if (pDimAssoc->getEdgeIndex() < nEdges + 4)
        {
          OdDgMeshQuadList::Quad curQuad;
          pMesh->getQuad(i, curQuad);

          ptStart = curQuad.m_points[pDimAssoc->getEdgeIndex() - nEdges];

          if (pDimAssoc->getEdgeIndex() - nEdges == 3)
            ptEnd = curQuad.m_points[0];
          else
            ptEnd = curQuad.m_points[pDimAssoc->getEdgeIndex() - nEdges + 1];

          bCorrectEdge = true;
          break;
        }
        else
          nEdges += 4;
      }
    }
  }
  else if (pElm->isKindOf(OdDgMeshTriangleGrid::desc()))
  {
    OdDgMeshTriangleGridPtr pMesh = pElm;

    OdUInt32 xSize = pMesh->getRowsNumber();
    OdUInt32 ySize = pMesh->getColumnsNumber();
    OdUInt32 uEdge = pDimAssoc->getEdgeIndex();

    OdUInt32 nEdges = 0;

    for( OdUInt32 x = 0; x < xSize - 1; x++)
    {
      for ( OdUInt32 y = 0; y < ySize - 1; y++)
      {
        if (uEdge - nEdges < 3)
        {
          if( uEdge - nEdges == 0 )
          {
            ptStart = pMesh->getPoint(x, y);
            ptEnd   = pMesh->getPoint(x, y + 1);
            bCorrectEdge = true;
          }
          else if (uEdge - nEdges == 1)
          {
            ptStart = pMesh->getPoint(x, y + 1);
            ptEnd = pMesh->getPoint(x + 1, y + 1);
            bCorrectEdge = true;
          }
          else
          {
            ptStart = pMesh->getPoint(x + 1, y + 1);
            ptEnd = pMesh->getPoint(x, y);
            bCorrectEdge = true;
          }

          break;
        }
        else
          nEdges += 3;

        if (uEdge - nEdges < 3)
        {
          if (uEdge - nEdges == 0)
          {
            ptStart = pMesh->getPoint(x, y);
            ptEnd = pMesh->getPoint(x + 1, y);
            bCorrectEdge = true;
          }
          else if (uEdge - nEdges == 1)
          {
            ptStart = pMesh->getPoint(x + 1, y);
            ptEnd = pMesh->getPoint(x + 1, y + 1);
            bCorrectEdge = true;
          }
          else
          {
            ptStart = pMesh->getPoint(x + 1, y + 1);
            ptEnd = pMesh->getPoint(x, y);
            bCorrectEdge = true;
          }

          break;
        }
        else
          nEdges += 3;
      }
    }
  }
  else if (pElm->isKindOf(OdDgMeshQuadGrid::desc()))
  {
    OdDgMeshQuadGridPtr pMesh = pElm;

    OdUInt32 xSize = pMesh->getRowsNumber();
    OdUInt32 ySize = pMesh->getColumnsNumber();
    OdUInt32 uEdge = pDimAssoc->getEdgeIndex();

    OdUInt32 nEdges = 0;

    for (OdUInt32 x = 0; x < xSize - 1; x++)
    {
      for (OdUInt32 y = 0; y < ySize - 1; y++)
      {
        if (uEdge - nEdges < 4)
        {
          if (uEdge - nEdges == 0)
          {
            ptStart = pMesh->getPoint(x, y);
            ptEnd = pMesh->getPoint(x, y + 1);
            bCorrectEdge = true;
          }
          else if (uEdge - nEdges == 1)
          {
            ptStart = pMesh->getPoint(x, y + 1);
            ptEnd = pMesh->getPoint(x + 1, y + 1);
            bCorrectEdge = true;
          }
          else if (uEdge - nEdges == 2)
          {
            ptStart = pMesh->getPoint(x + 1, y + 1);
            ptEnd = pMesh->getPoint(x + 1, y);
            bCorrectEdge = true;
          }
          else
          {
            ptStart = pMesh->getPoint(x + 1, y);
            ptEnd = pMesh->getPoint(x, y);
            bCorrectEdge = true;
          }

          break;
        }
        else
          nEdges += 4;
      }
    }
  }
  else
    return eNotThatKindOfClass;

  if (bCorrectEdge)
    ptRet = ptStart + (ptEnd - ptStart) * pDimAssoc->getUParam();
  else
    return eInvalidEdge;

  return eOk;
}

//================================================================================

OdDgGraphicsElementPtr getGrElmSegment(OdDgGraphicsElementPtr pGrElm, OdUInt16 uSegmentIndex)
{
  OdDgGraphicsElementPtr pRet = pGrElm;

  if( pRet->isKindOf(OdDgMultiVertex2d::desc()) )
  {
    OdDgMultiVertex2dPtr pCurve = pRet;

    if( pCurve->getVerticesCount() - 1 > uSegmentIndex )
    {
      OdDgLine2dPtr pLine2d = OdDgLine2d::createObject();
      pLine2d->setStartPoint(pCurve->getVertexAt(uSegmentIndex));
      pLine2d->setEndPoint(pCurve->getVertexAt(uSegmentIndex + 1 ));
      pLine2d->setDatabaseDefaults(pCurve->database());
      pRet = pLine2d;
    }
  }
  else if( pRet->isKindOf(OdDgMultiVertex3d::desc()) )
  {
    OdDgMultiVertex3dPtr pCurve = pRet;

    if (pCurve->getVerticesCount() - 1 > uSegmentIndex)
    {
      OdDgLine3dPtr pLine3d = OdDgLine3d::createObject();
      pLine3d->setStartPoint(pCurve->getVertexAt(uSegmentIndex));
      pLine3d->setEndPoint(pCurve->getVertexAt(uSegmentIndex + 1));
      pLine3d->setDatabaseDefaults(pCurve->database());
      pRet = pLine3d;
    }
  }

  return pRet;
}

//================================================================================

OdResult getIntersect(const OdDgElementPtr& pElm1,
                        const OdDgElementPtr& pElm2,
                          OdInt16 iSegment1,
                            OdUInt16 iSegment2,
                              OdUInt16 uIsectIndex,
                                OdGePoint3d& ptRet
                     )
{
  OdDgGraphicsElementPtr pGrElm1 = pElm1;
  OdDgGraphicsElementPtr pGrElm2 = pElm2;

  if( iSegment1 >= 0 )
    pGrElm1 = getGrElmSegment(pGrElm1, iSegment1);

  if( iSegment2 >= 0 )
    pGrElm2 = getGrElmSegment(pGrElm2, iSegment2);

  ::odrxDynamicLinker()->loadModule(L"ExDgnElementIntersect.tx");

  OdDgElementIntersectionPEPtr pIsectPE = (OdDgElementIntersectionPEPtr)(OdRxObjectPtr(pGrElm1));

  if (!pIsectPE.isNull())
  {
    OdGePoint3dArray arrIsectPts;

    if( pIsectPE->intersectWith(pGrElm1, pGrElm2, OdDg::kExtendBoth, arrIsectPts) == eOk)
    {
      if( arrIsectPts.size() > uIsectIndex )
      {
        ptRet = arrIsectPts[uIsectIndex];
        return eOk;
      }

      return eInvalidIndex;
    }
  }

  return eInvalidInput;
}

//================================================================================

OdResult getIntersectAssocPoint(OdDgDatabase* pDb,
                                  const OdDgIntersectAssociationPtr& pDimAssoc,
                                    OdGePoint3d& ptRet)
{
  OdDgElementPtr pElm1 = getAssociatedElement(pDb, pDimAssoc->getRefAttachmentId(), pDimAssoc->getElementId());

  if (pElm1.isNull())
    return eNullObjectPointer;

  OdDgElementPtr pElm2 = getAssociatedElement(pDb, pDimAssoc->getRefAttachment2Id(), pDimAssoc->getElement2Id());

  if (pElm2.isNull())
    return eNullObjectPointer;

  return getIntersect(pElm1, pElm2, -1, -1, pDimAssoc->getIndex(), ptRet);
}

//================================================================================

OdResult getIntersect2AssocPoint( OdDgDatabase* pDb,
                                    const OdDgIntersect2AssociationPtr& pDimAssoc,
                                      OdGePoint3d& ptRet)
{
  OdDgElementPtr pElm1 = getAssociatedElement(pDb, pDimAssoc->getRefAttachmentId(), pDimAssoc->getElementId());

  if (pElm1.isNull())
    return eNullObjectPointer;

  OdDgElementPtr pElm2 = getAssociatedElement(pDb, pDimAssoc->getRefAttachment2Id(), pDimAssoc->getElement2Id());

  if (pElm2.isNull())
    return eNullObjectPointer;

  return getIntersect(pElm1, pElm2, pDimAssoc->getSegment1(), pDimAssoc->getSegment2(), pDimAssoc->getIndex(), ptRet);
}

//================================================================================

OdResult recomputeDimPoint( OdDgDatabase* pDb,
                              const OdDgAssocPointRootPtr& pDimAssoc,
                                const OdGePoint3d& ptDefault,
                                  OdGePoint3d& ptRet)
{
  ptRet = ptDefault;

  switch( pDimAssoc->getType() )
  {
    case OdDgAssocPointRoot::kLinearAssociation:
    {
      if( getLinearAssocPoint(pDb, pDimAssoc, ptRet) != eOk )
        ptRet = ptDefault;
    } break;

    case OdDgAssocPointRoot::kArcAssociation:
    {
      if (getArcAssocPoint(pDb, pDimAssoc, ptRet) != eOk)
        ptRet = ptDefault;
    } break;

    case OdDgAssocPointRoot::kBSplineCurveAssociation:
    {
      if (getBSplineAssocPoint(pDb, pDimAssoc, ptRet) != eOk)
        ptRet = ptDefault;
    } break;

    case OdDgAssocPointRoot::kMultilineAssociation:
    {
      if (getMultilineAssocPoint(pDb, pDimAssoc, ptRet) != eOk)
        ptRet = ptDefault;
    } break;

    case OdDgAssocPointRoot::kOriginAssociation:
    {
      if (getOriginAssocPoint(pDb, pDimAssoc, ptRet) != eOk)
        ptRet = ptDefault;
    } break;

    case OdDgAssocPointRoot::kBSplineSurfaceAssociation:
    {
      if (getBSplineSurfaceAssocPoint(pDb, pDimAssoc, ptRet) != eOk)
        ptRet = ptDefault;
    } break;

    case OdDgAssocPointRoot::kMeshVertexAssociation:
    {
      if( getMeshVertexAssocPoint(pDb, pDimAssoc, ptRet) != eOk )
        ptRet = ptDefault;
    } break;

    case OdDgAssocPointRoot::kMeshEdgeAssociation:
    {
      if( getMeshEdgeAssocPoint(pDb, pDimAssoc, ptRet) != eOk )
        ptRet = ptDefault;
    } break;

    case OdDgAssocPointRoot::kProjectionAssociation:
    {
      if( getProjectionAssocPoint(pDb, pDimAssoc, ptRet) != eOk)
        ptRet = ptDefault;
    } break;

    case OdDgAssocPointRoot::kIntersectAssociation:
    {
      if( getIntersectAssocPoint(pDb, pDimAssoc, ptRet) != eOk)
        ptRet = ptDefault;
    } break;

    case OdDgAssocPointRoot::kIntersect2Association:
    {
      if( getIntersect2AssocPoint(pDb, pDimAssoc, ptRet) != eOk)
        ptRet = ptDefault;
    } break;
  }
  return eOk;
}

//================================================================================

void updateSecondPointOfRadialDimension(OdDgDimension* obj)
{
  OdDgDimPoint ptDimAssoc = obj->getPoint(0);
  OdDgAssocPointRootPtr ptAssocData = ptDimAssoc.getDimAssoc();

  OdDgDimPoint ptDimText = obj->getPoint(2);
  OdGePoint3d ptText = ptDimText.getPoint();

  OdDgElementPtr pElm = getAssociatedElement(obj->database(), ptAssocData->getRefAttachmentId(), ptAssocData->getElementId());

  if( pElm.isNull() )
    return;

  OdGePoint3d ptRadius;
  bool bCalculateRadius = false;

  if (pElm->isKindOf(OdDgEllipse3d::desc()) || pElm->isKindOf(OdDgArc3d::desc()) || 
      pElm->isKindOf(OdDgEllipse2d::desc()) || pElm->isKindOf(OdDgArc2d::desc()))
  {
    OdGeEllipArc3d arc3d;

    if (pElm->isKindOf(OdDgEllipse3d::desc()))
    {
      OdDgEllipse3dPtr pEllipse3d = pElm;
      arc3d = pEllipse3d->getEllipArc();
      arc3d.setAngles(0, Oda2PI);
    }
    else if (pElm->isKindOf(OdDgEllipse2d::desc()))
    {
      OdGeEllipArc2d arc2d;

      OdDgEllipse2dPtr pEllipse2d = pElm;
      arc2d = pEllipse2d->getEllipArc();
      OdGePoint2d  ptCenter = arc2d.center();
      OdGeVector2d vrMajor = arc2d.majorAxis();
      OdGeVector2d vrMinor = arc2d.minorAxis();
      double dMajR = arc2d.majorRadius();
      double dMinR = arc2d.minorRadius();
      arc3d.set(OdGePoint3d(ptCenter.x, ptCenter.y, 0.0), OdGeVector3d(vrMajor.x, vrMajor.y, 0.0), 
        OdGeVector3d(vrMinor.x, vrMinor.y, 0.0), dMajR, dMinR);
    }
    else if (pElm->isKindOf(OdDgArc2d::desc()))
    {
      OdGeEllipArc2d arc2d;

      OdDgArc2dPtr pArc2d = pElm;
      arc2d = pArc2d->getEllipArc();

      OdGePoint2d  ptCenter = arc2d.center();
      OdGeVector2d vrMajor = arc2d.majorAxis();
      OdGeVector2d vrMinor = arc2d.minorAxis();
      double dMajR = arc2d.majorRadius();
      double dMinR = arc2d.minorRadius();
      arc3d.set(OdGePoint3d(ptCenter.x, ptCenter.y, 0.0), OdGeVector3d(vrMajor.x, vrMajor.y, 0.0),
        OdGeVector3d(vrMinor.x, vrMinor.y, 0.0), dMajR, dMinR);
    }
    else
    {
      OdDgArc3dPtr pArc3d = pElm;
      arc3d = pArc3d->getEllipArc();
      arc3d.setAngles(0, Oda2PI);
    }

    OdGeVector3d vrLine = ptText - arc3d.center();

    if (!vrLine.isZeroLength())
    {
      OdGeLine3d line3d(ptText, vrLine);

      int nIsects = 0;
      OdGePoint3d ptIsect1, ptIsect2;

      if( arc3d.intersectWith(line3d, nIsects, ptIsect1, ptIsect2, 1e-7) )
      {
        if (nIsects == 1)
        {
          ptRadius = ptIsect1;
          bCalculateRadius = true;
        }
        else if (nIsects == 2)
        {
          if (ptIsect1.distanceTo(ptText) < ptIsect2.distanceTo(ptText))
          {
            ptRadius = ptIsect1;
            bCalculateRadius = true;
          }
          else
          {
            ptRadius = ptIsect2;
            bCalculateRadius = true;
          }
        }
      }
    }
  }  
  else
    return;

  if( bCalculateRadius )
  {
    OdDgDimPoint ptDimRadius = obj->getPoint(1);
    ptDimRadius.setPoint(ptRadius);
    obj->setPoint(1, ptDimRadius);
  }
}

//================================================================================

void updateSecondPointOfParaPerpRadialDimension(OdDgDimension* obj)
{
  OdDgDimPoint ptDimAssoc = obj->getPoint(0);
  OdDgAssocPointRootPtr ptAssocData = ptDimAssoc.getDimAssoc();

  OdDgDimPoint ptDimText = obj->getPoint(2);
  OdGePoint3d ptText = ptDimText.getPoint();

  OdDgElementPtr pElm = getAssociatedElement(obj->database(), ptAssocData->getRefAttachmentId(), ptAssocData->getElementId());

  if (pElm.isNull())
    return;

  OdGePoint3d ptRadius;
  bool bCalculateRadius = false;

  if( pElm->isKindOf(OdDgEllipse3d::desc()) || pElm->isKindOf(OdDgArc3d::desc()) ||
      pElm->isKindOf(OdDgEllipse2d::desc()) || pElm->isKindOf(OdDgArc2d::desc()))
  {
    OdGeEllipArc3d arc3d;

    if (pElm->isKindOf(OdDgEllipse3d::desc()))
    {
      OdDgEllipse3dPtr pEllipse3d = pElm;
      arc3d = pEllipse3d->getEllipArc();
      arc3d.setAngles(0, Oda2PI);
    }
    else if (pElm->isKindOf(OdDgEllipse2d::desc()))
    {
      OdGeEllipArc2d arc2d;

      OdDgEllipse2dPtr pEllipse2d = pElm;
      arc2d = pEllipse2d->getEllipArc();
      OdGePoint2d  ptCenter = arc2d.center();
      OdGeVector2d vrMajor = arc2d.majorAxis();
      OdGeVector2d vrMinor = arc2d.minorAxis();
      double dMajR = arc2d.majorRadius();
      double dMinR = arc2d.minorRadius();
      arc3d.set(OdGePoint3d(ptCenter.x, ptCenter.y, 0.0), OdGeVector3d(vrMajor.x, vrMajor.y, 0.0),
        OdGeVector3d(vrMinor.x, vrMinor.y, 0.0), dMajR, dMinR);
    }
    else if (pElm->isKindOf(OdDgArc2d::desc()))
    {
      OdGeEllipArc2d arc2d;

      OdDgArc2dPtr pArc2d = pElm;
      arc2d = pArc2d->getEllipArc();

      OdGePoint2d  ptCenter = arc2d.center();
      OdGeVector2d vrMajor = arc2d.majorAxis();
      OdGeVector2d vrMinor = arc2d.minorAxis();
      double dMajR = arc2d.majorRadius();
      double dMinR = arc2d.minorRadius();
      arc3d.set(OdGePoint3d(ptCenter.x, ptCenter.y, 0.0), OdGeVector3d(vrMajor.x, vrMajor.y, 0.0),
        OdGeVector3d(vrMinor.x, vrMinor.y, 0.0), dMajR, dMinR);
    }
    else
    {
      OdDgArc3dPtr pArc3d = pElm;
      arc3d = pArc3d->getEllipArc();
      arc3d.setAngles(0, Oda2PI);
    }

    OdDgDimRotation quatView;
    OdDgDimOptionView* pViewOption;
    OdDgDimOptionPtr pOption = obj->getOption(OdDgDimOption::kView);
    pViewOption = dynamic_cast<OdDgDimOptionView*>(pOption.get());

    if (pViewOption != NULL)
    {
      OdGeQuaternion quat = pViewOption->getQuaternion();

      if (obj->Is3D())
        quat.w *= -1;

      quatView.create(quat, obj->Is3D());
    }

    OdGeVector3d vrViewDir = OdGeVector3d::kXAxis;
    vrViewDir = quatView.rotate(vrViewDir);

    OdGePoint3d ptView = arc3d.center() + vrViewDir * arc3d.majorRadius();

    OdGePlane arcPlane(arc3d.center(), arc3d.normal());

    OdGePoint3d ptViewProj = ptView.project(arcPlane, arc3d.normal());

    OdGeVector3d vrViewArc = ptViewProj - arc3d.center();

    if (vrViewArc.isZeroLength())
      vrViewArc = vrViewDir;
    else
      vrViewArc.normalize();

    OdGeVector3d vrDimDir = ptText - arc3d.center();

    if (vrDimDir.isZeroLength())
      vrDimDir = vrViewArc;

    if( obj->isKindOf(OdDgDimDiameterPerpendicular::desc()) )
    {
      ptRadius = arc3d.center() + vrViewArc * arc3d.majorRadius();

      if( vrDimDir.isCodirectionalTo( vrViewDir) )
        ptRadius = arc3d.center() - vrViewArc * arc3d.majorRadius();
      else
        ptRadius = arc3d.center() + vrViewArc * arc3d.majorRadius();
    }
    else
    {
      double dDirAngle = vrDimDir.angleTo(vrViewArc);

      if (dDirAngle > OdaPI2)
        dDirAngle = OdaPI - dDirAngle;

      if (dDirAngle > OdaPI / 4)
      {
        ptRadius = arc3d.center() + vrViewArc * arc3d.majorRadius();
      }
      else
      {
        vrViewArc.rotateBy(OdaPI2, arc3d.normal());
        ptRadius = arc3d.center() + vrViewArc * arc3d.majorRadius();
      }
    }

    bCalculateRadius = true;
  }
  else
    return;

  if( bCalculateRadius )
  {
    OdDgDimPoint ptDimRadius = obj->getPoint(1);
    ptDimRadius.setPoint(ptRadius);
    obj->setPoint(1, ptDimRadius);
  }
}

//================================================================================

OdResult OdDgRecomputeDimPtsPEImpl::recomputeDimensionPoints(OdDgDimension* obj)
{
  OdResult retVal = eOk;

  bool bRadialDimensionAssociationUsed = false;

  for(OdUInt32 i = 0; i < obj->getPointsCount(); i++)
  {
    OdDgDimPoint ptDim = obj->getPoint(i);

    if( !ptDim.getDimAssoc().isNull() )
    {
      OdGePoint3d ptDimPoint = ptDim.getPoint();
      retVal = recomputeDimPoint(obj->database(), ptDim.getDimAssoc(), ptDimPoint, ptDimPoint);

      if (retVal == eOk)
      {
        ptDim.setPoint(ptDimPoint);
        obj->setPoint(i, ptDim);

        if( i == 0 )
          bRadialDimensionAssociationUsed = true;
      }
      else
        break;
    }
  }

  if( !bRadialDimensionAssociationUsed )
    return retVal;

  if( obj->isKindOf(OdDgDimRadius::desc()) || obj->isKindOf(OdDgDimDiameter::desc()) ||
      obj->isKindOf(OdDgDimRadiusExtended::desc()) || obj->isKindOf(OdDgDimDiameterExtended::desc()) )
  {
    if( retVal == eOk)
      updateSecondPointOfRadialDimension(obj);
  }
  else if (obj->isKindOf(OdDgDimDiameterParallel::desc()) || obj->isKindOf(OdDgDimDiameterPerpendicular::desc()))
  {
    if( retVal == eOk )
      updateSecondPointOfParaPerpRadialDimension(obj);
  }

  return retVal;
}

//================================================================================

OdResult OdDgRecomputeMLinePtsPEImpl::recomputeMultilinePoints(OdDgMultiline* obj)
{
  OdResult retVal = eOk;

  for (OdUInt32 i = 0; i < obj->getPointsCount(); i++)
  {
    OdDgMultilinePoint ptMLine;
    obj->getPoint(i, ptMLine);

    if (!ptMLine.getAssocData().isNull())
    {
      OdGePoint3d ptMLinePoint;
      ptMLine.getPoint(ptMLinePoint);
      retVal = recomputeDimPoint(obj->database(), ptMLine.getAssocData(), ptMLinePoint, ptMLinePoint);

      if (retVal == eOk)
      {
        ptMLine.setPoint(ptMLinePoint);
        obj->setPoint(i, ptMLine);
      }
      else
        break;
    }
  }

  return retVal;
}

//================================================================================


