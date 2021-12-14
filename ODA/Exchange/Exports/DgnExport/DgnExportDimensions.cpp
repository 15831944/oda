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

#include <OdaCommon.h>
#include "DgnExportDimensions.h"
#include "DgnExportCommon.h"
#include "DgnExportContext.h"
#include "DgnExportText.h"
#include "DgnExportImpl.h"
#include <DgDimension.h>
#include <Db2LineAngularDimension.h>
#include <Db3PointAngularDimension.h>
#include <DbArcDimension.h>
#include <DbRadialDimension.h>
#include <DbDiametricDimension.h>
#include <DbOrdinateDimension.h>
#include <DbAlignedDimension.h>
#include <DbRadialDimensionLarge.h>
#include <DbRotatedDimension.h>
#include <DbDimension.h>
#include <DbBlockTableRecord.h>
#include <Ge/GeLine3d.h>
#include <Ge/GeLineSeg3d.h>
#include <DbMText.h>
#include <DbLeader.h>
#include <DbMLeader.h>
#include <DgCellHeader.h>
#include <DgLine.h>
#include <DbAnnotativeObjectPE.h>
#include <Gi/GiAnnoScaleSet.h>
#include <DbObjectContextData.h>
#include <DbViewport.h>
#include <DbObjectContextInterface.h>
#include <DbObjectContextPE.h>
#include <DbSolid.h>
#include <DbLine.h>
#include <DbPolyline.h>
#include <Db2dPolyline.h>
#include <Db3dPolyline.h>
#include <DbBlockReference.h>
#include <DbCircle.h>
#include <DbTextStyleTableRecord.h>
#include <DbDimAssoc.h>
#include <DbDictionary.h>
#include <DgMultiVertex.h>
#include <DgArc.h>
#include <DgEllipse.h>
#include <DgBSplineCurve.h>
#include <DgMultiline.h>
#include <DgComplexCurve.h>
#include <DgMesh.h>
#include <DgBSplineSurface.h>
#include <Ge/GePointOnCurve3d.h>
#include <DgText.h>
#include <DgTextNode.h>
#include <DgTagElement.h>
#include <DgSharedCellReference.h>
#include <DgElementIntersectionPE.h>

namespace TD_DGN_EXPORT {

//---------------------------------------------------------------------------------------------------

double getEntityAnnotativeScale(const OdDbEntity* pEnt)
{
  double dScale = 1.0;

  OdDbAnnotativeObjectPEPtr pAnno = OdDbAnnotativeObjectPE::cast(pEnt);
  bool isAnno = pAnno.get() && pAnno->annotative(pEnt);

  OdGiAnnoScaleSet res;
  OdDbBaseDatabasePE* pDbPE = OdDbBaseDatabasePE::cast(pEnt->database()).get();
  if (pDbPE && !pDbPE->getAnnoScaleSet(pEnt->objectId(), res))
    isAnno = false;
  if (isAnno)
  {
    OdDbDatabase* pDb = pEnt->database();
    ODA_ASSERT_ONCE_X(MLEADER, pDb);
    OdDbAnnotationScalePtr pScale;
    OdDbAnnotScaleObjectContextDataPtr pScaleCtx;
    if (pDb->getActiveLayoutBTRId() == pDb->getModelSpaceId())
      pScale = pDb->getCANNOSCALE();
    else
    {
      OdDbBlockTableRecordPtr pBtr = OdDbBlockTableRecord::cast(pDb->getActiveLayoutBTRId().safeOpenObject());
      OdDbLayoutPtr pLayout = OdDbLayout::cast(pBtr->getLayoutId().safeOpenObject(OdDb::kForRead));
      OdDbViewportPtr pVpt = OdDbViewport::cast(pLayout->activeViewportId().safeOpenObject());
      pScale = pVpt->annotationScale();
    }

    OdDbObjectContextInterfacePtr op(pEnt);
    if (!op->hasContext(pEnt, *pScale))
    {
      OdDbObjectContextPEPtr pCtxPE = op; // = OdDbObjectContextInterface::cast(pMLeader);
      OdDbObjectContextDataPtr pCtxDef = pCtxPE->getDefaultContextData(pEnt, L"ACDB_ANNOTATIONSCALES");
      pScaleCtx = OdDbAnnotScaleObjectContextData::cast(pCtxDef);
      ODA_ASSERT_ONCE_X(MLEADER, pScaleCtx.get());
      if (pScaleCtx.get())
        pScale = NULL;
    }

    OdResult res = eInvalidContext;
    if (pScale.get())
      res = pScale->getScale(dScale);
    else if (pScaleCtx.get())
      res = pScaleCtx->getScale(dScale);
    ODA_ASSERT_ONCE_X(MLEADER, res == eOk && !OdZero(dScale));
    if (res == eOk && !OdZero(dScale))
      dScale = 1.0 / dScale;
    else
      dScale = 1.0;
  }
  return dScale;
}

//---------------------------------------------------------------------------------------------------

double getMLeaderScale(const OdDbMLeader* pMLeader)
{
  ODA_ASSERT_ONCE(pMLeader);
  double dScale = pMLeader->scale();

  if (dScale < 1e-8) // is zero for annotative (ObjectArx)
    dScale = getEntityAnnotativeScale(pMLeader);

  return dScale;
}

//---------------------------------------------------------------------------------------------------

double getDimnsionScale(const OdDbDimension* pDim)
{
  ODA_ASSERT_ONCE(pDim);
  double dScale = pDim->dimscale();

  if (dScale < 1e-8) // is zero for annotative (ObjectArx)
    dScale = getEntityAnnotativeScale(pDim);

  return dScale;
}

//---------------------------------------------------------------------------------------------------

OdGePoint3dArray calculateDimensionTextBox(const OdDbDimension* pDbDim)
{
  OdGePoint3dArray retVal;

  bool bInitDirections = false;
  OdGeLine3d   lineX;
  OdGeLine3d   lineY;
  OdGeVector3d vrXDir = OdGeVector3d::kXAxis;
  OdGeVector3d vrYDir = OdGeVector3d::kYAxis;
  OdGePoint3d  ptBoxOrigin = OdGePoint3d::kOrigin;

  double dMinX = 0;
  double dMaxX = 0;
  double dMinY = 0;
  double dMaxY = 0;

  OdDbObjectId idBlock = pDbDim->dimBlockId();

  if( !idBlock.isNull() )
  {
    OdDbBlockTableRecordPtr pBlock = idBlock.openObject(OdDb::kForRead);

    OdDbObjectIteratorPtr pIter = pBlock->newIterator();

    for(; !pIter->done(); pIter->step())
    {
      OdDbObjectPtr pObj = pIter->objectId().openObject(OdDb::kForRead);

      if( !pObj.isNull() && pObj->isKindOf(OdDbMText::desc()))
      {
        OdGePoint3dArray arrMTextBox;
        OdDbMTextPtr pMText = pObj;
        pMText->getActualBoundingPoints(arrMTextBox);

        if( !bInitDirections && !arrMTextBox.isEmpty() )
        {
          ptBoxOrigin = arrMTextBox[2];
          vrXDir = arrMTextBox[1] - arrMTextBox[0];
          vrYDir = arrMTextBox[0] - arrMTextBox[2];

          if (!vrXDir.isZeroLength())
            vrXDir.normalize();
          else
            vrXDir = OdGeVector3d::kXAxis;

          if (!vrYDir.isZeroLength())
            vrYDir.normalize();
          else
            vrYDir = OdGeVector3d::kYAxis;

          lineX.set(ptBoxOrigin, vrXDir);
          lineY.set(ptBoxOrigin, vrYDir);

          bInitDirections = true;
        }

        for( OdUInt32 i = 0; i < arrMTextBox.size(); i++ )
        {
          OdGePoint3d ptXProj = lineX.closestPointTo(arrMTextBox[i]);
          OdGePoint3d ptYProj = lineY.closestPointTo(arrMTextBox[i]);

          OdGeVector3d vrXProj = ptXProj - ptBoxOrigin;
          OdGeVector3d vrYProj = ptYProj - ptBoxOrigin;

          double dCurXDistance = 0.0;

          if (!vrXProj.isZeroLength() && !vrXProj.isCodirectionalTo(vrXDir))
            dCurXDistance = -vrXProj.length();
          else
            dCurXDistance = vrXProj.length();

          double dCurYDistance = 0.0;

          if (!vrYProj.isZeroLength() && !vrYProj.isCodirectionalTo(vrYDir))
            dCurYDistance = -vrYProj.length();
          else
            dCurYDistance = vrYProj.length();

          if (dMinX > dCurXDistance)
            dMinX = dCurXDistance;
          else if (dMaxX < dCurXDistance)
            dMaxX = dCurXDistance;

          if( dMinY > dCurYDistance )
            dMinY = dCurYDistance;
          else if (dMaxY < dCurYDistance)
            dMaxY = dCurYDistance;
        }
      }
    }

    if( !bInitDirections )
    {
      pIter = pBlock->newIterator();

      for (; !pIter->done(); pIter->step())
      {
        OdDbObjectPtr pObj = pIter->objectId().openObject(OdDb::kForRead);

        if (!pObj.isNull() && pObj->isKindOf(OdDbText::desc()))
        {
          OdGePoint3dArray arrMTextBox;
          OdDbTextPtr pText = pObj;
          pText->getBoundingPoints(arrMTextBox);

          if (!bInitDirections && !arrMTextBox.isEmpty())
          {
            ptBoxOrigin = arrMTextBox[2];
            vrXDir = arrMTextBox[1] - arrMTextBox[0];
            vrYDir = arrMTextBox[0] - arrMTextBox[2];

            if (!vrXDir.isZeroLength())
              vrXDir.normalize();
            else
              vrXDir = OdGeVector3d::kXAxis;

            if (!vrYDir.isZeroLength())
              vrYDir.normalize();
            else
              vrYDir = OdGeVector3d::kYAxis;

            lineX.set(ptBoxOrigin, vrXDir);
            lineY.set(ptBoxOrigin, vrYDir);

            bInitDirections = true;
          }

          for (OdUInt32 i = 0; i < arrMTextBox.size(); i++)
          {
            OdGePoint3d ptXProj = lineX.closestPointTo(arrMTextBox[i]);
            OdGePoint3d ptYProj = lineY.closestPointTo(arrMTextBox[i]);

            OdGeVector3d vrXProj = ptXProj - ptBoxOrigin;
            OdGeVector3d vrYProj = ptYProj - ptBoxOrigin;

            double dCurXDistance = 0.0;

            if (!vrXProj.isZeroLength() && !vrXProj.isCodirectionalTo(vrXDir))
              dCurXDistance = -vrXProj.length();
            else
              dCurXDistance = vrXProj.length();

            double dCurYDistance = 0.0;

            if (!vrYProj.isZeroLength() && !vrYProj.isCodirectionalTo(vrYDir))
              dCurYDistance = -vrYProj.length();
            else
              dCurYDistance = vrYProj.length();

            if (dMinX > dCurXDistance)
              dMinX = dCurXDistance;
            else if (dMaxX < dCurXDistance)
              dMaxX = dCurXDistance;

            if (dMinY > dCurYDistance)
              dMinY = dCurYDistance;
            else if (dMaxY < dCurYDistance)
              dMaxY = dCurYDistance;
          }
        }
      }
    }
  }

  if( bInitDirections )
  {
    OdGePoint3d ptMin = ptBoxOrigin + vrXDir * dMinX + vrYDir * dMinY;

    retVal.push_back(ptMin);
    retVal.push_back(ptMin + vrXDir * (dMaxX - dMinX));
    retVal.push_back(ptMin + vrXDir * (dMaxX - dMinX) + vrYDir * (dMaxY - dMinY));
    retVal.push_back(ptMin + vrYDir * (dMaxY - dMinY));

    OdGeMatrix3d matBlockTransform = pDbDim->dimBlockTransform();

    for (OdUInt32 j = 0; j < 4; j++)
      retVal[j].transformBy(matBlockTransform);
  }

  return retVal;
}

//---------------------------------------------------------------------------------------------------

OdGePoint3dArray getDimensionTextBox(const OdDbDimension* pDbDim)
{
  OdGePoint3dArray retVal = calculateDimensionTextBox(pDbDim);

  if( retVal.isEmpty() )
  {
    OdDbDimension* pDbVarDim = const_cast<OdDbDimension*>(pDbDim);
    pDbVarDim->upgradeOpen();
    pDbVarDim->recomputeDimBlock();
    pDbVarDim->downgradeOpen();
    retVal = calculateDimensionTextBox(pDbDim);
  }

  if( !retVal.isEmpty() && !OdZero(pDbDim->textRotation()) )
  {
    OdGeMatrix3d matRotationBack = OdGeMatrix3d::rotation(pDbDim->textRotation(), pDbDim->normal(), pDbDim->textPosition());
    matRotationBack = matRotationBack.invert();

    for( OdUInt32 i = 0; i < retVal.size(); i++ )
      retVal[i].transformBy(matRotationBack);
  }

  return retVal;
}

//---------------------------------------------------------------------------------------------------

bool calculateAngularDimTextOffsets(const OdDbDimension* pDbDim,
                                      const OdGePoint3d& ptDimStart,
                                        const OdGePoint3d& ptDimEnd,
                                          const OdGePoint3d& ptDimOrigin,
                                            const OdGeVector3d& vrDimDir,
                                              bool bHorizontalFlag,
                                                double dTextMargin,
                                                  double dTextHeight,
                                                    OdGePoint2d& ptTextOffset
)
{
  bool bRet = false;

  // Calculate dimension text box

  OdGePoint3dArray arrTextBox = getDimensionTextBox(pDbDim);

  if (arrTextBox.isEmpty())
    return false;

  double dRadius = ptDimOrigin.distanceTo(ptDimStart);

  double dTextBoxWidth = 0;
  double dTextBoxHeight = dTextHeight;
  OdGeVector3d vrTextDir = vrDimDir;

  OdGePoint3d ptBottomLeft;

  dTextBoxWidth = arrTextBox[0].distanceTo(arrTextBox[1]);
  dTextBoxHeight = arrTextBox[0].distanceTo(arrTextBox[3]);
  vrTextDir = arrTextBox[1] - arrTextBox[0];

  if (!vrTextDir.isZeroLength())
    vrTextDir.normalize();
  else
    vrTextDir = vrDimDir;

  OdGeVector3d vrTextDirY = arrTextBox[3] - arrTextBox[0];

  if (!vrTextDirY.isZeroLength())
    vrTextDirY.normalize();
  else
  {
    vrTextDirY = vrDimDir;
    vrTextDirY.rotateBy(OdaPI2, pDbDim->normal());
  }

  double dXOffset = 0;
  double dYOffset = 0;

  // calculate text offset and extents for manual text without no leader

  if( pDbDim->dimtmove() == 0 )
  {
    OdGePoint3d ptTextBoxMid = arrTextBox[0] + vrTextDir * dTextBoxWidth / 2.0 + vrTextDirY * dTextBoxHeight / 2.0;

    OdGeVector3d vrOriginMid = ptTextBoxMid - ptDimOrigin;

    if (vrOriginMid.isZeroLength())
      return false;

    vrOriginMid.normalize();

    OdGeVector3d vrStart = ptDimStart - ptDimOrigin;

    double dTextAngle = vrStart.angleTo(vrOriginMid, pDbDim->normal());

    dXOffset = dRadius * dTextAngle;

    if( !bHorizontalFlag && pDbDim->dimgap() > 0)
      dXOffset -= dTextMargin;

    bRet = true;
  }
  else
  {
    double dOffset = 0;

    if (pDbDim->dimgap() > 0)
      dOffset += pDbDim->dimgap();

    OdGePoint3d ptTextLeft  = arrTextBox[0] - vrTextDir * dOffset + vrTextDirY * dTextBoxHeight / 2.0;
    OdGePoint3d ptTextRight = arrTextBox[1] + vrTextDir * dOffset + vrTextDirY * dTextBoxHeight / 2.0;

    OdGeVector3d vrStart = ptDimStart - ptDimOrigin;
    OdGeVector3d vrEnd   = ptDimEnd - ptDimOrigin;

    double dAngleMid = vrStart.angleTo(vrEnd, pDbDim->normal())/2.0;

    OdGeVector3d vrMid = vrStart;
    vrMid = vrMid.rotateBy(dAngleMid, pDbDim->normal());
    vrMid.normalize();

    OdGePoint3d ptMid = ptDimOrigin + vrMid * dRadius;

    OdGeVector3d vrRight = ptTextRight - ptMid;

    OdGeVector3d vrTextNormal = vrTextDir.crossProduct(vrTextDirY);

    double dRightAngle = vrTextDirY.angleTo(vrRight, vrTextNormal);

    OdGePoint3d ptBase = ptTextLeft;

    if ((dRightAngle > 0) && (dRightAngle < OdaPI))
      ptBase = ptTextRight;

    OdGeVector3d vrBaseDir = ptBase - ptDimOrigin;

    vrBaseDir.normalize();

    OdGePoint3d ptRadius = ptDimOrigin + vrBaseDir * dRadius;

    OdGeVector3d vrYOffsetDir = ptBase - ptRadius;

    if (vrBaseDir.angleTo(vrYOffsetDir) < OdaPI2)
      dYOffset = vrYOffsetDir.length();
    else
      dYOffset = -vrYOffsetDir.length();

    dXOffset = vrStart.angleTo(vrBaseDir, pDbDim->normal())*dRadius;

    bRet = true;
  }

  ptTextOffset.x = dXOffset;
  ptTextOffset.y = dYOffset;

  return bRet;
}

//---------------------------------------------------------------------------------------------------

bool calculateLinearDimTextOffsets(const OdDbDimension* pDbDim,
                                     const OdGePoint3d& ptDimStart,
                                       const OdGePoint3d& ptDimEnd,
                                         const OdGeVector3d& vrDimDir,
                                           bool bHorizontalFlag,
                                             double dTextMargin,
                                               double dTextHeight,
                                                 OdGePoint2d& ptTextOffset
                                         )
{
  // Calculate dimension text box

  OdGePoint3dArray arrTextBox = getDimensionTextBox(pDbDim);

  if( arrTextBox.isEmpty() )
    return false;

  double dTextBoxWidth = 0;
  double dTextBoxHeight = dTextHeight;
  OdGeVector3d vrTextDir = vrDimDir;
  OdGeVector3d vrStartEndDir = ptDimEnd - ptDimStart;

  OdGePoint3d ptBottomLeft;

  dTextBoxWidth = arrTextBox[0].distanceTo(arrTextBox[1]);
  dTextBoxHeight = arrTextBox[0].distanceTo(arrTextBox[3]);
  vrTextDir = arrTextBox[1] - arrTextBox[0];

  if (!vrTextDir.isZeroLength())
    vrTextDir.normalize();
  else
    vrTextDir = vrDimDir;

  OdGeVector3d vrTextDirY = arrTextBox[3] - arrTextBox[0];

  if (!vrTextDirY.isZeroLength())
    vrTextDirY.normalize();
  else
  {
    vrTextDirY = vrDimDir;
    vrTextDirY.rotateBy(OdaPI2, pDbDim->normal());
  }

  // Correct text box for horizontal text

  if (bHorizontalFlag && (pDbDim->dimtmove() == 0) && (pDbDim->dimtad() != 2) && (pDbDim->dimtad() != 4))
  {
    OdGePoint3d ptMidStart, ptMidEnd;
    ptMidStart = arrTextBox[0] + (arrTextBox[3] - arrTextBox[0]) / 2.0;
    ptMidEnd = arrTextBox[1] + (arrTextBox[2] - arrTextBox[1]) / 2.0;

    OdGeLine3d midLine(ptMidStart, ptMidEnd);
    OdGeLine3d dimLine(ptDimStart, ptDimEnd);

    OdGePoint3d ptLinesIsect;

    if (dimLine.intersectWith(midLine, ptLinesIsect))
    {
      OdGeVector3d vrOffset = ptLinesIsect - vrTextDir * dTextBoxWidth / 2.0 - vrTextDirY * dTextBoxHeight / 2.0 - arrTextBox[0];

      for (OdUInt32 k = 0; k < arrTextBox.size(); k++)
        arrTextBox[k] += vrOffset;
    }
  }

  double dXOffset = 0;
  double dYOffset = 0;

  // calculate text offset and extents for manual text without no leader

  if (pDbDim->dimtmove() == 0)
  {
    if( bHorizontalFlag ) // Horizontal dimensions
    {
      if( OdEqual(vrStartEndDir.angleTo(vrTextDir), OdaPI2) )
      {
        OdGePoint3d ptBasePoint1 = arrTextBox[3] + vrTextDirY * dTextMargin;
        OdGePoint3d ptBasePoint2 = arrTextBox[0] - vrTextDirY * dTextMargin;

        OdGeLine3d dimLine(ptDimStart, vrDimDir);
        OdGeLine3d dimText1(ptBasePoint1, vrTextDir);
        OdGeLine3d dimText2(ptBasePoint2, vrTextDir);

        OdGePoint3d ptProj1;
        OdGePoint3d ptProj2;

        if (!dimLine.intersectWith(dimText1, ptProj1))
          ptProj1 = dimLine.closestPointTo(ptBasePoint1);

        if (!dimLine.intersectWith(dimText2, ptProj2))
          ptProj2 = dimLine.closestPointTo(ptBasePoint2);

        OdGeVector3d vrProjX1 = ptProj1 - ptDimStart;
        OdGeVector3d vrProjX2 = ptProj2 - ptDimStart;

        if (!vrProjX1.isZeroLength() && !vrProjX1.isCodirectionalTo(vrStartEndDir))
        {
          if (!vrProjX2.isZeroLength() && !vrProjX2.isCodirectionalTo(vrStartEndDir))
          {
            if (vrProjX1.length() < vrProjX2.length())
              dXOffset = -vrProjX2.length();
            else
              dXOffset = -vrProjX1.length();
          }
          else
            dXOffset = -vrProjX1.length();
        }
        else if (!vrProjX2.isZeroLength() && !vrProjX2.isCodirectionalTo(vrStartEndDir))
        {
          dXOffset = -vrProjX2.length();
        }
        else if (vrProjX1.length() < vrProjX2.length())
        {
          dXOffset = vrProjX1.length();
        }
        else
        {
          dXOffset = vrProjX2.length();
        }
      }
      else
      {
        arrTextBox[0] = arrTextBox[0] - vrTextDir * dTextMargin - vrTextDirY * dTextMargin;
        arrTextBox[1] = arrTextBox[1] + vrTextDir * dTextMargin - vrTextDirY * dTextMargin;
        arrTextBox[2] = arrTextBox[2] + vrTextDir * dTextMargin + vrTextDirY * dTextMargin;
        arrTextBox[3] = arrTextBox[3] - vrTextDir * dTextMargin + vrTextDirY * dTextMargin;

        OdGePoint3d ptBoxMid = arrTextBox[0] + vrTextDir * arrTextBox[0].distanceTo(arrTextBox[1]) / 2.0 + vrTextDirY * arrTextBox[0].distanceTo(arrTextBox[3]) / 2.0;

        OdGeLine3d dimLine(ptBoxMid, vrDimDir);
        OdGeLine3d boxYLine(arrTextBox[0], vrTextDirY);

        bool bYIsect = false;

        OdGePoint3d ptIsect;

        if( dimLine.intersectWith(boxYLine, ptIsect) )
        {
          OdGeLineSeg3d boxYSeg(arrTextBox[0], arrTextBox[3]);

          if (boxYSeg.isOn(ptIsect))
            bYIsect = true;
        }

        if (bYIsect)
        {
          OdGePoint3d ptBasePoint1 = arrTextBox[1];
          OdGePoint3d ptBasePoint2 = arrTextBox[0];

          OdGeLine3d dimLine(ptDimStart, vrDimDir);
          OdGeLine3d dimText1(ptBasePoint1, vrTextDirY);
          OdGeLine3d dimText2(ptBasePoint2, vrTextDirY);

          OdGePoint3d ptProj1;
          OdGePoint3d ptProj2;

          if (!dimLine.intersectWith(dimText1, ptProj1))
            ptProj1 = dimLine.closestPointTo(ptBasePoint1);

          if (!dimLine.intersectWith(dimText2, ptProj2))
            ptProj2 = dimLine.closestPointTo(ptBasePoint2);

          OdGeVector3d vrProjX1 = ptProj1 - ptDimStart;
          OdGeVector3d vrProjX2 = ptProj2 - ptDimStart;

          if( !vrProjX1.isZeroLength() && !vrProjX1.isCodirectionalTo(vrStartEndDir) )
          {
            if( !vrProjX2.isZeroLength() && !vrProjX2.isCodirectionalTo(vrStartEndDir) )
            {
              if (vrProjX1.length() < vrProjX2.length())
                dXOffset = -vrProjX2.length();
              else
                dXOffset = -vrProjX1.length();
            }
            else
              dXOffset = -vrProjX1.length();
          }
          else if (!vrProjX2.isZeroLength() && !vrProjX2.isCodirectionalTo(vrStartEndDir))
          {
            dXOffset = -vrProjX2.length();
          }
          else if (vrProjX1.length() < vrProjX2.length())
          {
            dXOffset = vrProjX1.length();
          }
          else
          {
            dXOffset = vrProjX2.length();
          }
        }
        else
        {
          OdGePoint3d ptBasePoint1 = arrTextBox[3];
          OdGePoint3d ptBasePoint2 = arrTextBox[0];

          OdGeLine3d dimLine(ptDimStart, vrDimDir);
          OdGeLine3d dimText1(ptBasePoint1, vrTextDir);
          OdGeLine3d dimText2(ptBasePoint2, vrTextDir);

          OdGePoint3d ptProj1;
          OdGePoint3d ptProj2;

          if (!dimLine.intersectWith(dimText1, ptProj1))
            ptProj1 = dimLine.closestPointTo(ptBasePoint1);

          if (!dimLine.intersectWith(dimText2, ptProj2))
            ptProj2 = dimLine.closestPointTo(ptBasePoint2);

          OdGeVector3d vrProjX1 = ptProj1 - ptDimStart;
          OdGeVector3d vrProjX2 = ptProj1 - ptDimStart;

          if (!vrProjX1.isZeroLength() && !vrProjX1.isCodirectionalTo(vrStartEndDir))
          {
            if (!vrProjX2.isZeroLength() && !vrProjX2.isCodirectionalTo(vrStartEndDir))
            {
              if (vrProjX1.length() < vrProjX2.length())
                dXOffset = -vrProjX2.length();
              else
                dXOffset = -vrProjX1.length();
            }
            else
              dXOffset = -vrProjX1.length();
          }
          else if (!vrProjX2.isZeroLength() && !vrProjX2.isCodirectionalTo(vrStartEndDir))
          {
            dXOffset = -vrProjX2.length();
          }
          else if (vrProjX1.length() < vrProjX2.length())
          {
            dXOffset = vrProjX1.length();
          }
          else
          {
            dXOffset = vrProjX2.length();
          }
        }
      }
    }
    else // Aligned dimensions.
    {
      if (!vrStartEndDir.isZeroLength())
        vrStartEndDir.normalize();

      OdGePoint3d ptBasePoint;

      if (vrStartEndDir.angleTo(vrTextDir) > OdaPI2)
        ptBasePoint = arrTextBox[1] + vrTextDir * dTextMargin;
      else
        ptBasePoint = arrTextBox[0] - vrTextDir * dTextMargin;

      OdGeLine3d dimLine(ptDimStart, vrStartEndDir);
      OdGeLine3d dimText(ptBasePoint, vrTextDirY);

      OdGePoint3d ptProj;

      if (!dimLine.intersectWith(dimText, ptProj))
        ptProj = dimLine.closestPointTo(ptBasePoint);

      OdGeVector3d vrProjX = ptProj - ptDimStart;

      if (vrProjX.isZeroLength() || vrProjX.isCodirectionalTo(vrStartEndDir))
        dXOffset = vrProjX.length();
      else
        dXOffset = -vrProjX.length();
    }
  }
  else // Calculate offsets for dimension with leader
  {
    // Calculate x and y offsets.

    OdGeLine3d lineDim(ptDimStart, vrDimDir);
    OdGeVector3d vrDimYDir = vrDimDir;
    vrDimYDir.rotateBy(OdaPI2, pDbDim->normal());

    if( bHorizontalFlag )
    {
      if (vrDimDir.isParallelTo(vrTextDir))
      {
        // Calculate Y offset

        OdGeLine3d dimLine(ptDimStart, vrDimDir);

        OdGePoint3d ptLeftTextMid = arrTextBox[0];

        if (pDbDim->dimalt())
          ptLeftTextMid += vrTextDirY * dTextBoxHeight / 2.0;

        OdGePoint3d ptLeftTextProj = lineDim.closestPointTo(ptLeftTextMid);

        OdGeVector3d vrYOffsetDir = ptLeftTextMid - ptLeftTextProj;

        if (vrYOffsetDir.angleTo(vrDimYDir) < OdaPI2)
          dYOffset = vrYOffsetDir.length();
        else
          dYOffset = -vrYOffsetDir.length();

        // Calculate X offset

        OdGeVector3d vrStartEnd = ptDimEnd - ptDimStart;
        double dMidLength = vrStartEnd.length() / 2.0;

        if (!vrStartEnd.isZeroLength())
          vrStartEnd.normalize();

        ptLeftTextProj -= vrStartEnd * dTextMargin;

        OdGeVector3d vrProjStart = ptLeftTextProj - ptDimStart;

        if (vrProjStart.isZeroLength() || vrProjStart.isCodirectionalTo(vrStartEnd))
          dXOffset = vrProjStart.length();
        else
          dXOffset = -vrProjStart.length();

        if (vrStartEnd.angleTo(vrTextDir) > OdaPI2)
          dXOffset -= dTextBoxWidth;

        if (dXOffset < dMidLength)
          dXOffset += dTextBoxWidth + 2.0 * dTextMargin;
      }
      else
      {
        OdGePoint3d ptLeftTextMid = arrTextBox[0] + vrTextDirY * dTextBoxHeight / 2.0;
        OdGePoint3d ptRightTextMid = arrTextBox[1] + vrTextDirY * dTextBoxHeight / 2.0;
        ptLeftTextMid -= vrTextDir * dTextMargin;
        ptRightTextMid += vrTextDir * dTextMargin;

        OdGeLineSeg3d segDim(ptDimStart, ptDimEnd);
        OdGePoint3d ptSegMid = segDim.midPoint();

        OdGeVector3d vrRight = ptRightTextMid - ptSegMid;

        OdGeVector3d vrTextNormal = vrTextDir.crossProduct(vrTextDirY);

        double dRightAngle = vrTextDirY.angleTo(vrRight, vrTextNormal);

        OdGePoint3d ptBase = ptLeftTextMid;

        if ((dRightAngle > 0) && (dRightAngle < OdaPI))
          ptBase = ptRightTextMid;

        OdGePoint3d ptBaseProj = lineDim.closestPointTo(ptBase);

        OdGeVector3d vrYOffsetDir = ptBase - ptBaseProj;

        if (vrYOffsetDir.angleTo(vrDimYDir) < OdaPI2)
          dYOffset = vrYOffsetDir.length();
        else
          dYOffset = -vrYOffsetDir.length();

        OdGeVector3d vrXOffsetDir = ptBaseProj - ptDimStart;

        OdGeVector3d vrStartEnd = ptDimEnd - ptDimStart;

        if (!vrStartEnd.isZeroLength())
          vrStartEnd.normalize();

        if (vrXOffsetDir.angleTo(vrStartEnd) < OdaPI2)
          dXOffset = vrXOffsetDir.length();
        else
          dXOffset = -vrXOffsetDir.length();
      }
    }
    else
    {
      OdGeLineSeg3d segDim(ptDimStart, ptDimEnd);
      OdGePoint3d ptSegMid = segDim.midPoint();
      OdGePoint3d ptLeftTextMid = arrTextBox[0] + vrTextDirY * dTextBoxHeight / 2.0;
      OdGePoint3d ptLeftTextProj = lineDim.closestPointTo(ptLeftTextMid);

      OdGeVector3d vrYOffsetDir = ptLeftTextMid - ptLeftTextProj;

      if (vrYOffsetDir.angleTo(vrDimYDir) < OdaPI2)
        dYOffset = vrYOffsetDir.length();
      else
        dYOffset = -vrYOffsetDir.length();

      OdGeVector3d vrStartEnd = ptDimEnd - ptDimStart;

      if (!vrStartEnd.isZeroLength())
        vrStartEnd.normalize();

      ptLeftTextProj -= vrStartEnd * dTextMargin;

      if (vrTextDir.angleTo(vrStartEnd) > OdaPI2)
        ptLeftTextProj -= vrStartEnd * dTextBoxWidth;

      OdGeVector3d vrXOffsetDir = ptLeftTextProj - ptDimStart;

      if (vrXOffsetDir.angleTo(vrStartEnd) < OdaPI2)
        dXOffset = vrXOffsetDir.length();
      else
        dXOffset = -vrXOffsetDir.length();

      double dMidLength = ptDimStart.distanceTo(ptSegMid);
      double dActualTextWidth = dTextBoxWidth + 2 * dTextMargin;

      if (dMidLength > dXOffset + dActualTextWidth)
        dXOffset += dActualTextWidth;
    }
  }

  ptTextOffset.x = dXOffset;
  ptTextOffset.y = dYOffset;

  return true;
}

//---------------------------------------------------------------------------------------------------

bool getDimArrowBlock(const OdDbObjectId& idDbArrow,
                        OdDgnExportDimArrowheadType arrowType,
                          OdDgDatabase* pDb,
                            OdDgDimTool::TerminatorType& uTermType,
                              OdDgElementId& idDgTerm)
{
  bool bRet = false;

  switch( arrowType )
  {
    case acArrowDefault:
    case acArrowUserDefined:
    case acArrowOrigin2:
    case acArrowArchTick:
    case acArrowBoxBlank:
    case acArrowBoxFilled:
    case acArrowDatumBlank:
    case acArrowDatumFilled:
    case acArrowIntegral:
    {
      uTermType = OdDgDimTool::kTtArrow;
      bRet = true;
    } break;

    case acArrowClosedBlank:
    case acArrowClosed:
    case acArrowOpen:
    case acArrowOpen90:
    case acArrowOpen30:
    {
      uTermType = OdDgDimTool::kTtArrow;
    } break;

    case acArrowDot:
    case acArrowDotBlank:
    case acArrowOrigin:
    {
      uTermType = OdDgDimTool::kTtCircle;
    } break;

    case acArrowOblique:
    {
      uTermType = OdDgDimTool::kTtStroke;
    } break;

    case acArrowNone:
    {
      uTermType = OdDgDimTool::kTtNone;
    } break;

    case acArrowDotSmall:
    case acArrowSmall:
    {
      uTermType = OdDgDimTool::kTtFilledCircle;
    } break;
  }

  if( bRet )
  {
    idDgTerm = OdDgnExportContext::getElementId(idDbArrow);

    if( idDgTerm.isNull() && !idDbArrow.isNull() )
    {
      OdDbBlockTableRecordPtr pDbBlock = idDbArrow.openObject(OdDb::kForRead);

      if( !pDbBlock.isNull() )
      {
        OdDgSharedCellDefinitionTablePtr pDgBlockTable = pDb->getSharedCellDefinitionTable(OdDg::kForWrite);

        OdDgSharedCellDefinitionPtr pDgBlockDef = OdDgSharedCellDefinition::createObject();
        pDgBlockDef->setName(pDbBlock->getName());
        pDgBlockDef->setDescription(pDbBlock->comments());
        pDgBlockDef->setOrigin(pDbBlock->origin());
        pDgBlockDef->set3dFormatFlag(OdDgnExportContext::is3d());
        pDgBlockTable->add(pDgBlockDef);

        double dOldScaleFactor = OdDgnExportContext::getLineStyleScaleCorrectionFactor();
        OdDgnExportContext::setLineStyleScaleCorrectionFactor(1.0 / OdDgnExportContext::getMasterToUORsScale());

        OdDgnExportContext::addProcessingBlockId(pDbBlock->id());
        DgnExporter::copyBlock(pDbBlock, pDgBlockDef);
        OdDgnExportContext::removeProcessingBlockId(pDbBlock->id());

        OdDgnExportContext::setLineStyleScaleCorrectionFactor(dOldScaleFactor);

        OdDgnExportContext::addDgnElement(pDbBlock->objectId(), pDgBlockDef->elementId());

        pDgBlockDef->transformBy(OdGeMatrix3d::scaling(OdDgnExportContext::getMasterToUORsScale()));

        idDgTerm = pDgBlockDef->elementId();
      }

      if( idDgTerm.isNull())
        bRet = false;
    }
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

OdDbDimAssocPtr getDimAssoc(const OdDbDimension* pDbDim)
{
  OdDbDimAssocPtr pRet;

  OdDbObjectId idObject = pDbDim->extensionDictionary();

  if( !idObject.isNull() )
  {
    OdDbDictionaryPtr pDict = idObject.openObject(OdDb::kForRead);

    if (!pDict.isNull())
    {
      OdDbObjectId idDimAssoc = pDict->getAt(L"ACAD_DIMASSOC");

      if( !idDimAssoc.isNull() )
        pRet = idDimAssoc.openObject(OdDb::kForRead);
    }
  }

  return pRet;
}

//---------------------------------------------------------------------------------------------------

OdDgTextNode2dPtr createTextOverride(const OdDbDimension* pDbDim, 
                                       OdDgDatabase* pDb, 
                                         const OdString& strTextOverride,
                                           double dTextHeight )
{
  OdDgTextNode2dPtr pRet;

  OdDbMTextPtr pMText = OdDbMText::createObject();
  pMText->setPropertiesFrom(pDbDim);

  OdCmColor dimTextColor = pDbDim->dimclrt();

  if( dimTextColor.colorMethod() != OdCmEntityColor::kByBlock )
    pMText->setColor(dimTextColor);

  pMText->setContents(strTextOverride);
  pMText->setHorizontalMode(OdDb::kTextCenter);
  pMText->setVerticalMode(OdDb::kTextVertMid);
  pMText->setTextStyle(pDbDim->dimtxsty());
  pMText->setTextHeight(dTextHeight*OdDgnExportContext::getMasterToUORsScale());

  bool bOld3d = OdDgnExportContext::is3d();

  OdDgnExportContext::set3d( false );

  OdDgGraphicsElementPtr pDgMText = convertMText(pMText, pDb);

  OdDgnExportContext::set3d(bOld3d);

  if( pDgMText->isKindOf(OdDgTextNode2d::desc()) )
  {
    pRet = pDgMText;
    pRet->recalculateTextNode();
  }

  return pRet;
}

//---------------------------------------------------------------------------------------------------

void applyDimensionSettings( const OdDbDimension* pDbDim, 
                               OdDgDimension* pDgDim, 
                                 OdDgDatabase* pDb,
                                   const OdDgElementId& idModel,
                                     const OdGePoint3d& ptDimLineStart,
                                       const OdGePoint3d& ptDimLineEnd,
                                         const OdGePoint3d& ptArcOrigin,
                                           const OdGeVector3d& vrDimDir,
                                             OdUInt32 uFirstPointIndex,
                                               OdUInt32 uSecondPointIndex,
                                                 bool bAllowToUseManualTextPos,
                                                   bool bAngularDim,
                                                     bool bShowAngleValue = false)
{
  // ========================== Prepare structures of dgn dimension =========

  OdDgDimensionExportPEPtr pDgDimPE = OdDgDimensionExportPEPtr(OdRxObjectPtr(pDgDim));

  OdDgDimGeometry     dimGeom;
  OdDgDimTextInfo     textInfo;
  OdDgDimTextFormat   textFormat;
  OdArray<OdDgDimPoint> arrDimPts;
  OdUInt32 i;

  for(i = 0; i < pDgDim->getPointsCount(); i++)
    arrDimPts.push_back(pDgDim->getPoint(i));

  // ============================= Angular Dim ==============================

  textFormat.setAngleMeasureFlag(bShowAngleValue);

  if( bShowAngleValue )
  {
    OdUInt32 uAccur = pDbDim->dimadec();

    if (uAccur < 8)
      textFormat.setPrimaryAccuracy((OdDgDimTextFormat::Accuracy)uAccur);
    else
      textFormat.setPrimaryAccuracy((OdDgDimTextFormat::Accuracy)7);

    textFormat.setAngleFormatFlag(false);
    textFormat.setRadiansFlag(false);
    pDgDim->setCentesimalFlag(false);
    textFormat.setHideAngleSecondsFlag(true);
    textFormat.setAngleMode(OdDgDimTextFormat::kAngle_D);

    switch (pDbDim->dimaunit())
    {
      case 0: textFormat.setAngleFormatFlag(false); break;
      case 1:
      {
        textFormat.setAngleFormatFlag(true);
        textFormat.setRadiansFlag(false);
        pDgDim->setCentesimalFlag(false);
        textFormat.setHideAngleSecondsFlag(false);

        textFormat.setAngleMode(OdDgDimTextFormat::kAngle_DMS);
      } break;

      case 2: 
      {
        textFormat.setRadiansFlag(false);
        pDgDim->setCentesimalFlag(true);
        textFormat.setAngleMode(OdDgDimTextFormat::kAngle_C);
      } break;

      case 3: 
      {
        textFormat.setRadiansFlag(true);
        pDgDim->setCentesimalFlag(false);
        textFormat.setAngleMode(OdDgDimTextFormat::kAngle_Radian);
      } break;
    }

    pDgDim->setTrailingZerosFlag(pDbDim->dimazin() < 2);
    pDgDim->setLeadingZerosFlag(pDbDim->dimazin() == 2 || pDbDim->dimazin() == 0);
  }

  // ========================== Symbology settings ==========================

  // 1. Native dimension symbology.

  OdUInt32 uElementLineStyle  = getDgnLineStyleEntryId(pDbDim->linetypeId());
  OdUInt32 uElementLineWeight = odDgnExportLineweightMapping(pDbDim->lineWeight());
  OdUInt32 uElementColorIndex = getDgnColorIndex(pDbDim->color(), pDb);

  // 2. Dimension lines and terminators symbology

  OdUInt32 uDimLineLineStyle  = uElementLineStyle;
  OdUInt32 uDimLineLineWeight = uElementLineWeight;
  OdUInt32 uDimLineColorIndex = uElementColorIndex;

  OdCmColor         dimColor      = pDbDim->dimclrd();
  OdDb::LineWeight  dimLineWeight = pDbDim->dimlwd();
  OdDbObjectId      dimLineTypeId = pDbDim->dimltype();

  if(dimColor.colorMethod() != OdCmEntityColor::kByBlock)
    uDimLineColorIndex = getDgnColorIndex(dimColor, pDb);

  if(dimLineWeight != OdDb::kLnWtByBlock)
    uDimLineLineWeight = odDgnExportLineweightMapping(dimLineWeight);

  OdUInt32 uDgLineTypeEntryId = getDgnLineStyleEntryId(dimLineTypeId);

  if( uDgLineTypeEntryId != OdDg::kLineStyleByCell )
    uDimLineLineStyle = uDgLineTypeEntryId;

  // 3. Extension lines symbology.

  OdUInt32 uExtLineStyle = uElementLineStyle;
  OdUInt32 uExtLineWeight = uElementLineWeight;
  OdUInt32 uExtColorIndex = uElementColorIndex;

  OdCmColor         extColor      = pDbDim->dimclre();
  OdDb::LineWeight  extLineWeight = pDbDim->dimlwe();
  OdDbObjectId      extLineTypeId = pDbDim->dimltex1();

  if (extColor.colorMethod() != OdCmEntityColor::kByBlock)
    uExtColorIndex = getDgnColorIndex(extColor, pDb);

  if (extLineWeight != OdDb::kLnWtByBlock)
    uExtLineWeight = odDgnExportLineweightMapping(extLineWeight);

  uDgLineTypeEntryId = getDgnLineStyleEntryId(extLineTypeId);

  if (uDgLineTypeEntryId != OdDg::kLineStyleByCell)
    uExtLineStyle = uDgLineTypeEntryId;

  // 4. Text color

  OdUInt32 uTextColorIndex = uElementColorIndex;

  OdCmColor dimTextColor = pDbDim->dimclrt();

  if( dimTextColor.colorMethod() != OdCmEntityColor::kByBlock )
    uTextColorIndex = getDgnColorIndex(dimTextColor, pDb);

  // 5. Apply dimension color to dgn element

  if( uElementColorIndex != uDimLineColorIndex )
    pDgDim->setColorIndex(uDimLineColorIndex);

  if( uElementLineStyle != uDimLineLineStyle )
    pDgDim->setLineStyleEntryId(uDimLineLineStyle);

  if( uElementLineWeight != uDimLineLineWeight )
    pDgDim->setLineWeight(uDimLineLineWeight);

  bool bUseAltSymb = false;

  if( uDimLineColorIndex != uExtColorIndex )
  {
    bUseAltSymb = true;
    pDgDim->setAltColorIndex(uExtColorIndex);
  }
  else
    pDgDim->setAltColorIndex(0);

  if (uDimLineLineStyle != uExtLineStyle)
  {
    bUseAltSymb = true;
    pDgDim->setAltLineStyleEntryId(uExtLineStyle);
  }
  else
    pDgDim->setAltLineStyleEntryId(0);

  if( uDimLineLineWeight != uExtLineWeight)
  {
    bUseAltSymb = true;
    pDgDim->setAltLineWeight(uExtLineStyle);
  }
  else
    pDgDim->setAltLineWeight(0);

  if( uDimLineColorIndex != uTextColorIndex )
  {
    textInfo.setColorIndex(uTextColorIndex);
    textInfo.setUseColorFlag(true);
  }
  else
    textInfo.setUseColorFlag(false);

  // ========================== Center mark settings ==========================

  double dScale = getDimnsionScale(pDbDim);

  dimGeom.setCenterSize(pDbDim->dimcen()*dScale);
  pDgDimPE->setCenterMarkFlag(pDgDim, !OdZero(pDbDim->dimcen()));

  // ========================== Extension line settings =======================

  // 1. Extension line extend and offset

  dimGeom.setWitnessLineOffset(pDbDim->dimexo()*dScale);
  dimGeom.setWitnessLineExtend(pDbDim->dimexe()*dScale);

  // 2. Extension line visibility and alt symbology usage.

  if (arrDimPts.size() > uFirstPointIndex)
  {
    arrDimPts[uFirstPointIndex].setNoWitnessLineFlag(pDbDim->dimse1());
    arrDimPts[uFirstPointIndex].setUseAltSymbologyFlag(bUseAltSymb);
  }

  if( arrDimPts.size() > uSecondPointIndex )
  {
    arrDimPts[uSecondPointIndex].setNoWitnessLineFlag(pDbDim->dimse2());
    arrDimPts[uSecondPointIndex].setUseAltSymbologyFlag(bUseAltSymb);
  }

  // ========================== Text settings =================================

  // 1. Text height

  double dTextHeight = pDbDim->dimtxt()*dScale;

  if (!OdZero(dTextHeight))
  {
    textInfo.setHeight(dTextHeight);
    textInfo.setWidth(dTextHeight);
  }

  // 2. Text style

  bool bVerticalDim = false;

  OdDbObjectId idDbTextStyle = pDbDim->dimtxsty();
  OdDgElementId idDgDimTextStyle = OdDgnExportContext::getElementId(idDbTextStyle);

  if (!idDgDimTextStyle.isNull())
  {
    OdDbTextStyleTableRecordPtr pDbTextStyle = idDbTextStyle.openObject(OdDb::kForRead);

    OdDgElementPtr pTextStyleElm = idDgDimTextStyle.openObject(OdDg::kForRead);

    if (!pTextStyleElm.isNull() && pTextStyleElm->isKindOf(OdDgTextStyleTableRecord::desc()))
    {
      OdDgTextStyleTableRecordPtr pTextStyle = pTextStyleElm;
      pDgDim->setTextStyleEntryId(pTextStyle->getEntryId());
      textInfo.setFontEntryId(pTextStyle->getFontEntryId() ); // Use font from text style
      bVerticalDim = pTextStyle->getTextDirection() & OdDg::kVertical ? true : false;

      if (!OdZero(pDbTextStyle->textSize()))
      {
        if (pDbDim->dimscale() > 1e-8)
        {
          dTextHeight = pTextStyle->getTextHeight() / OdDgnExportContext::getMasterToUORsScale();
          textInfo.setHeight(dTextHeight);
        }
      }

      textInfo.setWidth(dTextHeight * pDbTextStyle->xScale());
    }
  }

  if (OdZero(dTextHeight))
    dTextHeight = dScale;

  // 3. Text underline.

  OdString strPrefixSuffixMaster = pDbDim->dimpost();

  if( strPrefixSuffixMaster.find(L"\\L") != -1 )
    pDgDim->setUnderlineTextFlag(true);
  else
    pDgDim->setUnderlineTextFlag(false);

  // 4. Horizontal text.

  OdGePoint3d ptTextPosition = pDbDim->textPosition();

  if( bVerticalDim )
  {
    pDgDimPE->setTextType(pDgDim, OdDgDimTool::kVertical);
  }
  else
  {
    pDgDimPE->setTextType(pDgDim, OdDgDimTool::kStandard);

    bool bInlineText = true;

    if( bAngularDim )
    {
      OdGeVector3d vrStart = ptDimLineStart - ptArcOrigin;
      OdGeVector3d vrEnd   = ptDimLineEnd   - ptArcOrigin;
      OdGeVector3d vrText  = ptTextPosition - ptArcOrigin;

      double dStartEnd  = vrStart.angleTo(vrEnd, pDbDim->normal());
      double dStartText = vrStart.angleTo(vrText, pDbDim->normal());

      if( dStartEnd < dStartText )
        bInlineText = false;
    }
    else
    {
      OdGeLineSeg3d segStartEnd(ptDimLineStart, ptDimLineEnd);
      OdGeLine3d    lineStartEnd(ptDimLineStart, ptDimLineEnd);
      OdGePoint3d ptTextProj = lineStartEnd.closestPointTo(ptTextPosition);

      bInlineText = segStartEnd.isOn(ptTextProj);
    }

    if(bAngularDim && (pDbDim->dimtmove() == 1 ) )
      pDgDim->setHorizontalFlag(true);
    else if( bInlineText )
      pDgDim->setHorizontalFlag(pDbDim->dimtih());
    else
      pDgDim->setHorizontalFlag(pDbDim->dimtoh());
  }

  // 5. Text offset , lift and text box flag

  dimGeom.setTextMargin(fabs(pDbDim->dimgap()*dScale));

  if( pDbDim->dimalt() || pDbDim->dimgap() >= 0 )
    dimGeom.setTextLift((pDbDim->dimtvp() + fabs(pDbDim->dimgap()))*dScale);
  else
    dimGeom.setTextLift((pDbDim->dimtvp() + fabs(pDbDim->dimgap()*2.0))*dScale);

  pDgDim->setInlineTextLift(0.0);
  pDgDim->setBoxTextFlag(pDbDim->dimgap() < 0);

  // 6. Text alignments

  switch (pDbDim->dimtad())
  {
    case 1:
    case 3: pDgDim->setTextLocation(OdDgDimTextInfo::kTextAbove); break;
    case 2: pDgDim->setTextLocation(OdDgDimTextInfo::kTextOutside); break;
    case 4: pDgDim->setTextLocation(OdDgDimTextInfo::kTextTopLeft); break;
    default: pDgDim->setTextLocation(OdDgDimTextInfo::kTextInline); break;
  }

  if( pDbDim->isUsingDefaultTextPosition() )
  {
    if (arrDimPts.size() > uSecondPointIndex)
    {
      switch (pDbDim->dimjust())
      {
        case 1:
        case 3:  arrDimPts[uSecondPointIndex].setJustification(OdDgDimTextInfo::kLeftText); break;
        case 2:
        case 4:  arrDimPts[uSecondPointIndex].setJustification(OdDgDimTextInfo::kRightText); break;
        default: arrDimPts[uSecondPointIndex].setJustification(OdDgDimTextInfo::kCenterLeftText); break;
      }      
    }
  }
  else
  {
    if( arrDimPts.size() > uSecondPointIndex )
    {
      OdGePoint2d ptTextOffset;

      bool bManualText = bAllowToUseManualTextPos;
      
      if (bManualText)
      {
        if (bAngularDim)
        {
          bManualText = calculateAngularDimTextOffsets(pDbDim, ptDimLineStart, ptDimLineEnd, ptArcOrigin,
            vrDimDir, pDgDim->getHorizontalFlag(), dimGeom.getTextMargin(), dTextHeight, ptTextOffset);
        }
        else
        {
          bManualText = calculateLinearDimTextOffsets(pDbDim, ptDimLineStart, ptDimLineEnd,
            vrDimDir, pDgDim->getHorizontalFlag(), dimGeom.getTextMargin(), dTextHeight, ptTextOffset);
        }
      }

      if( bManualText )
      {
        arrDimPts[uSecondPointIndex].setJustification(OdDgDimTextInfo::kManualText);
        arrDimPts[uSecondPointIndex].setOffsetToDimLine(ptTextOffset.x);
        arrDimPts[uSecondPointIndex].setOffsetY(ptTextOffset.y);

        if( pDbDim->dimtmove() != 0 )
        {
          OdDgDimOptionOffsetPtr pDimLeaderOption = OdDgDimOption::createObject(OdDgDimOption::kOffset);
          pDimLeaderOption->setTerminator(OdDgDimTool::kTtNone);
          pDimLeaderOption->setAlignment(OdDgDimOptionOffset::kAutoAlignment);
          pDimLeaderOption->setElbowFlag(true);

          if (pDbDim->dimtmove() == 1)
            pDimLeaderOption->setChainType(OdDgDimOptionOffset::kLine);
          else
            pDimLeaderOption->setChainType(OdDgDimOptionOffset::kNone);

          pDgDim->setOption(pDimLeaderOption);
        }
      }
      else
      {
        arrDimPts[uSecondPointIndex].setJustification(OdDgDimTextInfo::kCenterLeftText);
      }
    }
  }

  // ========================== Dimension primary units =================================

  bool     bUseSecondaryUnits = pDbDim->dimalt() && !bShowAngleValue;
  pDgDim->setDualFlag(bUseSecondaryUnits);
  textFormat.setDualFormat(bUseSecondaryUnits);

  OdUInt16 uCommaSym = pDbDim->dimdsep();
  textFormat.setDecimalCommaFlag(uCommaSym == L',');

  // Dimension Primary units

  OdUInt32 uDimZinValue = pDbDim->dimzin();
  double   dPrimaryUnitsScaleFactor = fabs(pDbDim->dimlfac());
  double   dSubUnitsScaleFactor = fabs(pDbDim->dimmzf());
           strPrefixSuffixMaster = pDbDim->dimpost();
  OdString strPrefixSuffixSub = pDbDim->dimmzs();
  OdUInt8  uDimPresission = pDbDim->dimdec();
  OdUInt8  uDimUnitFormat = pDbDim->dimlunit();

  // 1. leading and trailing zeros

  if( !bShowAngleValue )
  {
    pDgDim->setTrailingZerosFlag(uDimZinValue < 8);
    pDgDim->setLeadingZerosFlag(uDimZinValue & 0x00000004 ? true : false);
  }

  // 2. Prefix and suffix for units and subunits

  OdString strMainPrefix;
  OdString strMainSuffix;
  OdString strSecondaryPrefix;
  OdString strSecondarySuffix;

  int iPrefixEnd = strPrefixSuffixMaster.find(L"<>");

  if (iPrefixEnd != -1)
  {
    if (iPrefixEnd != 0)
      strMainPrefix = strPrefixSuffixMaster.left(iPrefixEnd);

    if (iPrefixEnd != strPrefixSuffixMaster.getLength() - 2)
      strPrefixSuffixMaster = strPrefixSuffixMaster.right(strPrefixSuffixMaster.getLength() - iPrefixEnd - 2);
    else
      strPrefixSuffixMaster.empty();
  }

  strMainSuffix = strPrefixSuffixMaster;

  if (strMainSuffix.right(2) == L"\\L")
  {
    if (strMainSuffix.getLength() > 2)
      strMainSuffix = strMainSuffix.left(strMainSuffix.getLength() - 2);
    else
      strMainSuffix.empty();
  }

  if (strMainSuffix.right(2) == L"\\X")
  {
    if (strMainSuffix.getLength() > 2)
      strMainSuffix = strMainSuffix.left(strMainSuffix.getLength() - 2);
    else
      strMainSuffix.empty();
  }

  // 3. Scale factor of units and subunits

  OdDgModel::UnitDescription baseMasterUnits;
  OdDgModel::UnitDescription baseSubUnits;

  baseMasterUnits.m_numerator = 1.0;
  baseMasterUnits.m_denominator = 1.0;
  baseSubUnits.m_numerator = 1.0;
  baseSubUnits.m_denominator = 1.0;

  OdDgModelPtr pDimensionModel;

  if( !idModel.isNull() )
    pDimensionModel = idModel.openObject(OdDg::kForRead);

  double dBaseSubUnitsScaleFactor = 1.0;

  if (!pDimensionModel.isNull())
  {
    pDimensionModel->getMasterUnit(baseMasterUnits);
    pDimensionModel->getSubUnit(baseSubUnits);
    dBaseSubUnitsScaleFactor = baseMasterUnits.m_denominator / baseMasterUnits.m_numerator /
      baseSubUnits.m_denominator * baseSubUnits.m_numerator;
  }

  // Use units that differ of master units.

  if( (uDimUnitFormat == 3) || (uDimUnitFormat == 4) )
  {
    textFormat.setLocalFileUnitsFlag(false);
    textFormat.setPrimarySubUnitsFlag(true);
    textFormat.setPrimaryLabelsFlag(true);
    textFormat.setPrimaryDelimiterFlag(true);

    OdDgDimOptionPrimaryUnitsPtr pPrimaryUnits = OdDgDimOption::createObject(OdDgDimOption::kPrimaryUnits);

    OdDgModel::UnitDescription masterUnits;
    OdDgModel::UnitDescription subUnits;

    double dMasterNum = baseMasterUnits.m_numerator * dPrimaryUnitsScaleFactor / baseMasterUnits.m_denominator / 12.0;
    double dSubNum = dMasterNum * 12.0;

    fillUnitDescription(dMasterNum, masterUnits);
    fillUnitDescription(dSubNum, subUnits);

    masterUnits.m_name = L"\'";
    subUnits.m_name = L"\"";

    pPrimaryUnits->setMasterUnit(masterUnits);
    pPrimaryUnits->setSubUnit(subUnits);

    pDgDim->setOption(pPrimaryUnits);
  }
  else
  {
    if (!OdEqual(dPrimaryUnitsScaleFactor, 1.0) || !OdEqual(dSubUnitsScaleFactor, dBaseSubUnitsScaleFactor))
    {
      textFormat.setLocalFileUnitsFlag(false);

      OdDgDimOptionPrimaryUnitsPtr pPrimaryUnits = OdDgDimOption::createObject(OdDgDimOption::kPrimaryUnits);

      OdDgModel::UnitDescription masterUnits;
      OdDgModel::UnitDescription subUnits;

      double dMasterNum = baseMasterUnits.m_numerator * dPrimaryUnitsScaleFactor / baseMasterUnits.m_denominator;
      double dSubNum = dMasterNum * dSubUnitsScaleFactor;

      fillUnitDescription(dMasterNum, masterUnits);
      fillUnitDescription(dSubNum, subUnits);

      pPrimaryUnits->setMasterUnit(masterUnits);
      pPrimaryUnits->setSubUnit(subUnits);

      pDgDim->setOption(pPrimaryUnits);
    }
    else
      textFormat.setLocalFileUnitsFlag(true);

    textFormat.setPrimarySubUnitsFlag(false);
    textFormat.setPrimaryLabelsFlag(false);
  }

  if( ((strMainPrefix.getLength() == 1) || (strMainSuffix.getLength() == 1)) && !bShowAngleValue )
  {
    OdDimOptionPrefixSuffixPtr pPrimaryPrefixSuffix = OdDgDimOption::createObject(OdDgDimOption::kPrefixSuffix);

    if (bUseSecondaryUnits)
    {
      if (strMainPrefix.getLength() == 1)
      {
        pPrimaryPrefixSuffix->setUpperPrefix(strMainPrefix.getAt(0));
        strMainPrefix.empty();
      }

      if (strMainSuffix.getLength() == 1)
      {
        pPrimaryPrefixSuffix->setUpperSuffix(strMainSuffix.getAt(0));
        strMainSuffix.empty();
      }
    }
    else
    {
      if (strMainPrefix.getLength() == 1)
      {
        pPrimaryPrefixSuffix->setMainPrefix(strMainPrefix.getAt(0));
        strMainPrefix.empty();
      }

      if (strMainSuffix.getLength() == 1)
      {
        pPrimaryPrefixSuffix->setMainSuffix(strMainSuffix.getAt(0));
        strMainSuffix.empty();
      }
    }

    pDgDim->setOption(pPrimaryPrefixSuffix);
  }

  // 4. Accuracy

  if( !bShowAngleValue )
    textFormat.setPrimaryAccuracy(convertAccuracyToDgnFormat(uDimUnitFormat, uDimPresission, pDbDim->dimrnd()));

  // ========================== Dimension secondary units =================================

  if (bUseSecondaryUnits)
  {
    double dSecondaryUnitsScaleFactor = fabs(pDbDim->dimaltf());
    double dSecondarySubUnitsScaleFactor = fabs(pDbDim->dimaltmzf());
    uDimPresission = pDbDim->dimaltd();
    uDimUnitFormat = pDbDim->dimaltu();
    uDimZinValue = pDbDim->dimaltz();
    strPrefixSuffixMaster = pDbDim->dimapost();
    strPrefixSuffixSub = pDbDim->dimaltmzs();

    // 1. leading and trailing zeros

    pDgDim->setTrailingZeros2Flag(uDimZinValue < 8);
    pDgDim->setLeadingZero2Flag(uDimZinValue & 0x00000004 ? true : false);

    // 2. Prefix and suffix for units and subunits

    int iPrefixEnd = strPrefixSuffixMaster.find(L"[]");

    if (iPrefixEnd != -1)
    {
      if (iPrefixEnd != 0)
        strSecondaryPrefix = strPrefixSuffixMaster.left(iPrefixEnd);

      if (iPrefixEnd != strPrefixSuffixMaster.getLength() - 2)
        strPrefixSuffixMaster = strPrefixSuffixMaster.right(strPrefixSuffixMaster.getLength() - iPrefixEnd - 2);
      else
        strPrefixSuffixMaster.empty();
    }

    strSecondarySuffix = strPrefixSuffixMaster;

    if (strSecondarySuffix.right(2) == L"\\L" || strSecondarySuffix.right(2) == L"\\X")
    {
      if (strSecondarySuffix.getLength() > 2)
        strSecondarySuffix = strSecondarySuffix.left(strSecondarySuffix.getLength() - 2);
      else
        strSecondarySuffix.empty();
    }

    // 3. Scale factor of units and subunits

    if ((uDimUnitFormat == 3) || (uDimUnitFormat == 4))
    {
      textFormat.setSecondarySubUnitsFlag(true);
      textFormat.setSecondaryLabelsFlag(true);
      textFormat.setSecondaryDelimiterFlag(true);

      OdDgDimOptionSecondaryUnitsPtr pSecondaryUnits = OdDgDimOption::createObject(OdDgDimOption::kSecondaryUnits);

      OdDgModel::UnitDescription masterAltUnits;
      OdDgModel::UnitDescription subAltUnits;

      double dMasterNum = baseMasterUnits.m_numerator * dSecondaryUnitsScaleFactor / baseMasterUnits.m_denominator/12.0;
      double dSubNum = dMasterNum * 12.0;

      fillUnitDescription(dMasterNum, masterAltUnits);
      fillUnitDescription(dSubNum, subAltUnits);

      masterAltUnits.m_name = L"\'";
      subAltUnits.m_name = L"\"";

      pSecondaryUnits->setMasterUnit(masterAltUnits);
      pSecondaryUnits->setSubUnit(subAltUnits);

      pDgDim->setOption(pSecondaryUnits);
    }
    else
    {
      OdDgDimOptionSecondaryUnitsPtr pSecondaryUnits = OdDgDimOption::createObject(OdDgDimOption::kSecondaryUnits);

      OdDgModel::UnitDescription masterAltUnits;
      OdDgModel::UnitDescription subAltUnits;

      double dMasterNum = baseMasterUnits.m_numerator * dSecondaryUnitsScaleFactor / baseMasterUnits.m_denominator;
      double dSubNum = dMasterNum * dSecondarySubUnitsScaleFactor;

      fillUnitDescription(dMasterNum, masterAltUnits);
      fillUnitDescription(dSubNum, subAltUnits);

      pSecondaryUnits->setMasterUnit(masterAltUnits);
      pSecondaryUnits->setSubUnit(subAltUnits);

      pDgDim->setOption(pSecondaryUnits);

      textFormat.setSecondarySubUnitsFlag(false);
      textFormat.setSecondaryLabelsFlag(false);
    }

    if ((strSecondaryPrefix.getLength() == 1) || (strSecondarySuffix.getLength() == 1))
    {
      OdDimOptionPrefixSuffixPtr pPrefixSuffix = pDgDim->getOption(OdDgDimOption::kPrefixSuffix);

      if(pPrefixSuffix.isNull() )
        pPrefixSuffix = OdDgDimOption::createObject(OdDgDimOption::kPrefixSuffix);

      if (bUseSecondaryUnits)
      {
        if (strSecondaryPrefix.getLength() == 1)
        {
          pPrefixSuffix->setLowerPrefix(strSecondaryPrefix.getAt(0));
          strSecondaryPrefix.empty();
        }

        if (strSecondarySuffix.getLength() == 1)
        {
          pPrefixSuffix->setUpperSuffix(strSecondarySuffix.getAt(0));
          strSecondarySuffix.empty();
        }
      }

      pDgDim->setOption(pPrefixSuffix);
    }

    // 4. Accuracy

    textFormat.setSecondaryAccuracy(convertAccuracyToDgnFormat(uDimUnitFormat, uDimPresission, pDbDim->dimaltrnd()));
  }

  // =================== Dimension Stacked fraction settings =================================

  pDgDim->setStackFractFlag(true);
  textInfo.setStackedFractionType(OdDgDimTextInfo::kFractionFromText);

  if (pDbDim->dimfrac() == 0)
    textInfo.setStackedFractionType(OdDgDimTextInfo::kFractionHorizontal);
  else if (pDbDim->dimfrac() == 1)
    textInfo.setStackedFractionType(OdDgDimTextInfo::kFractionDiagonal);

  textInfo.setStackFractAlignment(OdDgDimTextInfo::kFractAlignmentCenter);

  // =========================== Dimension Tolerances =======================================

  bool bDisplayTolerance = pDbDim->dimtol();
  pDgDim->setToleranceFlag(bDisplayTolerance);

  if (bDisplayTolerance)
  {
    pDgDim->setTolmodeFlag(pDbDim->dimlim());

    OdDgDimOptionTolerancePtr pTolerance = OdDgDimOption::createObject(OdDgDimOption::kTolerance);
    pTolerance->setToleranceTextHeight(pDbDim->dimtfac()*dTextHeight);
    pTolerance->setToleranceTextWidth(pDbDim->dimtfac()*textInfo.getWidth());
    pTolerance->setShowSignForZeroFlag(false);
    pTolerance->setStackAlign(pDbDim->dimtolj());

    double dLowTol = pDbDim->dimtm();
    double dHighTol = pDbDim->dimtp();

    pTolerance->setToleranceLower(dLowTol);
    pTolerance->setToleranceUpper(dHighTol);
    pTolerance->setStackEqualFlag(OdEqual(dLowTol, dHighTol));

    pDgDim->setPrimaryToleranceAccuracy(convertAccuracyToDgnFormat(2, pDbDim->dimtdec(), pDbDim->dimrnd()));
    pDgDim->setSecondaryToleranceAccuracy(convertAccuracyToDgnFormat(2, pDbDim->dimalttd(), pDbDim->dimrnd()));
  }

  // =========================== Dimension fit options =======================================

  pDgDim->setJoinerFlag(pDbDim->dimtofl());

  if (pDbDim->dimtix())
  {
    pDgDim->setFitOptions(OdDgDimTextInfo::kTextInside);
  }
  else
  {
    switch (pDbDim->dimatfit())
    {
      case 0: pDgDim->setFitOptions(OdDgDimTextInfo::kBothMoves); break;
      case 1: pDgDim->setFitOptions(OdDgDimTextInfo::kTermMovesFirst); break;
      case 2: pDgDim->setFitOptions(OdDgDimTextInfo::kTextMovesFirst); break;
      case 3: pDgDim->setFitOptions(OdDgDimTextInfo::kSmallestMoves); break;
    }
  }

  // =========================== Dimension arrow heads =======================================

  dimGeom.setMargin(pDbDim->dimasz()*dScale);
  dimGeom.setUseMargin(true);

  OdDbObjectId  idArrowStart, idArrowEnd;

  if (pDbDim->dimsah())
  {
    idArrowStart = pDbDim->dimblk1();
    idArrowEnd = pDbDim->dimblk2();
  }
  else
  {
    idArrowStart = pDbDim->dimblk();
    idArrowEnd = pDbDim->dimblk();
  }

  double        dArrowSize   = pDbDim->dimasz() * dScale;
  double        dArrowHeight  = pDbDim->dimtsz() * dScale;

  OdDgnExportDimArrowheadType arrowTypeStart = getArrowheadDwgType(idArrowStart);
  OdDgnExportDimArrowheadType arrowTypeEnd = getArrowheadDwgType(idArrowEnd);

  OdDgDimTool::TerminatorType leftTerm = OdDgDimTool::kTtArrow;
  OdDgDimTool::TerminatorType rightTerm = OdDgDimTool::kTtArrow;

  OdDgElementId idDgLeftTerm, idDgRightTerm;

  bool bLeftArrowIsBlock  = getDimArrowBlock(idArrowStart, arrowTypeStart, pDb, leftTerm, idDgLeftTerm);
  bool bRightArrowIsBlock = getDimArrowBlock(idArrowEnd, arrowTypeEnd, pDb, rightTerm, idDgRightTerm);

  OdDgnExportDimArrowheadType arrowTypeTerm = arrowTypeStart;

  if( bLeftArrowIsBlock )
  {
    arrowTypeTerm = arrowTypeEnd;

    if (OdZero(dArrowHeight))
      dArrowHeight = getDimDefaultArrowHeight(arrowTypeStart, dArrowSize);

    OdDgDimOptionTerminatorsPtr pTerminators = OdDgDimOption::createObject(OdDgDimOption::kTerminators);

    if( bRightArrowIsBlock )
    {
      rightTerm = OdDgDimTool::kTtCircle;

      pTerminators->setArrowTermStyle(OdDgDimOptionTerminators::kTermCell);
      pTerminators->setArrowCellID(idDgLeftTerm.getHandle());

      pTerminators->setOriginTermStyle(OdDgDimOptionTerminators::kTermCell);
      pTerminators->setOriginCellID(idDgRightTerm.getHandle());
    }
    else
    {
      if( rightTerm == OdDgDimTool::kTtArrow )
      {
        leftTerm = OdDgDimTool::kTtCircle;

        pTerminators->setOriginTermStyle(OdDgDimOptionTerminators::kTermCell);
        pTerminators->setOriginCellID(idDgLeftTerm.getHandle());
      }
      else
      {
        pTerminators->setArrowTermStyle(OdDgDimOptionTerminators::kTermCell);
        pTerminators->setArrowCellID(idDgLeftTerm.getHandle());
      }
    }

    pDgDim->setOption(pTerminators);
  }
  else if( bRightArrowIsBlock )
  {
    if (OdZero(dArrowHeight))
      dArrowHeight = getDimDefaultArrowHeight(arrowTypeStart, dArrowSize);

    OdDgDimOptionTerminatorsPtr pTerminators = OdDgDimOption::createObject(OdDgDimOption::kTerminators);

    if (leftTerm == OdDgDimTool::kTtArrow)
    {
      rightTerm = OdDgDimTool::kTtCircle;

      pTerminators->setOriginTermStyle(OdDgDimOptionTerminators::kTermCell);
      pTerminators->setOriginCellID(idDgRightTerm.getHandle());
    }
    else
    {
      pTerminators->setArrowTermStyle(OdDgDimOptionTerminators::kTermCell);
      pTerminators->setArrowCellID(idDgRightTerm.getHandle());
    }

    pDgDim->setOption(pTerminators);
  }
  else if (OdZero(dArrowHeight))
    dArrowHeight = dArrowSize / 2.0;

  OdDgDimension::TerminatorArrowHeadType aType = getTerminatorArrowType(arrowTypeTerm);
  pDgDim->setArrowHead(aType);

  dimGeom.setTerminatorHeight(dArrowHeight);
  dimGeom.setTerminatorWidth(dArrowSize);

  pDgDimPE->setLeftTerminator(pDgDim, leftTerm);
  pDgDimPE->setRightTerminator(pDgDim, rightTerm);

  // ========================== Dimension prefix, suffix and text overrides =======

  OdDgTextNode2dPtr pMText;

  OdString strTextOverride = pDbDim->dimensionText();

  if( !strTextOverride.isEmpty() && strTextOverride.find(L'\\') != -1 )
  {
    strTextOverride.replace(L"<>", L"*");

    pMText = createTextOverride(pDbDim, pDb, strTextOverride, dTextHeight);

    if( !pMText.isNull() )
      strTextOverride = OdString::kEmpty;
  }

  bool bSetPrefixSuffix = true;

  if( !strTextOverride.isEmpty() && strTextOverride.find(L"<>") == -1 )
    bSetPrefixSuffix = false;
  else
  {
    OdString strPrefixOverride;
    OdString strSuffixOverride;

    int iPrefixOEnd = strTextOverride.find(L"<>");

    if (iPrefixOEnd != -1)
    {
      if (iPrefixOEnd != 0)
        strPrefixOverride = strTextOverride.left(iPrefixOEnd);

      if (iPrefixOEnd != strTextOverride.getLength() - 2)
        strTextOverride = strTextOverride.right(strTextOverride.getLength() - iPrefixOEnd - 2);
      else
        strTextOverride.empty();
    }

    strSuffixOverride = strTextOverride;

    strMainPrefix = strPrefixOverride + strMainPrefix;

    if( bUseSecondaryUnits )
      strSecondarySuffix += strSuffixOverride;
    else
      strMainSuffix += strSuffixOverride;
  }

  if (arrDimPts.size() > uSecondPointIndex)
  {
    if (bSetPrefixSuffix)
    {
      if (!strMainPrefix.isEmpty() || !strMainSuffix.isEmpty())
      {
        arrDimPts[uSecondPointIndex].setPrimaryTextFlag(true);
        arrDimPts[uSecondPointIndex].setPrimaryText(strMainPrefix + L"*" + strMainSuffix);
      }

      if (!strSecondaryPrefix.isEmpty() || !strSecondarySuffix.isEmpty())
      {
        arrDimPts[uSecondPointIndex].setSecondaryTextFlag(true);
        arrDimPts[uSecondPointIndex].setSecondaryText(strSecondaryPrefix + L"*" + strSecondarySuffix);
      }
    }
    else
    {
      arrDimPts[uSecondPointIndex].setPrimaryTextFlag(true);
      arrDimPts[uSecondPointIndex].setPrimaryText(strTextOverride);
    }
  }
  else
    pMText = NULL;

  // ================= Apply dimension structures to dimension ====================

  pDgDim->setDimGeometry(dimGeom);
  pDgDim->setDimTextInfo(textInfo);
  pDgDim->setDimTextFormat(textFormat);

  for (i = 0; i < arrDimPts.size(); i++)
    pDgDim->setPoint(i, arrDimPts[i]);

  // Convert dimension text override to MText and add it as dimension multiline text

  if (!pMText.isNull())
  {
    OdDgDimensionExportPEPtr pDgDimPE = OdDgDimensionExportPEPtr(OdRxObjectPtr(pDgDim));
    pDgDimPE->setMultilineText(pDgDim, uSecondPointIndex, pMText);
  }
}

//---------------------------------------------------------------------------------------------------

bool getRadialDimensionTextPosition(OdDbDimension* pDbDim,
                                      const OdGePoint3d& ptCenter,
                                        const OdGePoint3d& ptChord,
                                          const OdGeVector3d& vrDimStartEnd,
                                            bool bInlineText,
                                              bool bHorizontal,
                                                OdGePoint3d& ptRealTextPos,
                                                  OdGePoint3d& ptAddTextPos,
                                                    bool& bAddPoint )
{
  bool bRet = true;

  OdGePoint3dArray arrTextBox = getDimensionTextBox(pDbDim);

  if( !arrTextBox.isEmpty() )
  {
    bool bManualText = pDbDim->dimtmove() != 0;

    if( bHorizontal )
    {
      OdGeVector3d vrTextDir = arrTextBox[1] - arrTextBox[0];
      OdGeVector3d vrTextYDir = arrTextBox[3] - arrTextBox[0];

      if (!vrTextYDir.isZeroLength())
        vrTextYDir.normalize();

      if (!vrTextDir.isZeroLength())
        vrTextDir.normalize();

      OdGeLine3d lineTextY(ptChord, vrTextYDir);

      OdGePoint3d ptLineBase;

      if (bInlineText)
        ptLineBase = arrTextBox[0] + vrTextYDir * arrTextBox[0].distanceTo(arrTextBox[3]) / 2.0;
      else if (pDbDim->dimgap() < 0)
        ptLineBase = arrTextBox[0] - vrTextYDir * (pDbDim->dimtvp() + 2 * fabs(pDbDim->dimgap()));
      else
        ptLineBase = arrTextBox[0] - vrTextYDir * (pDbDim->dimtvp() + fabs(pDbDim->dimgap()));

      OdGeLine3d lineBase(ptLineBase, vrTextDir);

      OdGePoint3d ptIsect;

      double dInlineLeader = fabs(pDbDim->dimasz())*2.0;

      if (pDbDim->dimgap() < 0)
        dInlineLeader += fabs(pDbDim->dimgap());

      if( lineBase.intersectWith(lineTextY, ptIsect) )
      {
        OdGeLine3d lineStart(arrTextBox[0] - vrTextDir * dInlineLeader, vrTextYDir);
        OdGeLine3d lineEnd(arrTextBox[1] + vrTextDir * dInlineLeader, vrTextYDir);

        OdGePoint3d ptIsectStart, ptIsectEnd;

        if (lineBase.intersectWith(lineStart, ptIsectStart) && lineBase.intersectWith(lineEnd, ptIsectEnd))
        {
          double dStart = ptIsect.distanceTo(ptIsectStart);
          double dEnd = ptIsect.distanceTo(ptIsectEnd);

          if( dStart > dEnd )
            ptRealTextPos = ptIsectEnd;
          else
            ptRealTextPos = ptIsectStart;
        }
        else
          bRet = false;
      }
      else
        bRet = false;
    }
    else
    {
      OdGeLine3d lineDimDir(ptCenter, vrDimStartEnd);
      OdGeVector3d vrTextDir = arrTextBox[1] - arrTextBox[0];
      OdGeVector3d vrTextYDir = arrTextBox[3] - arrTextBox[0];

      if (!vrTextYDir.isZeroLength())
        vrTextYDir.normalize();

      if (!vrTextDir.isZeroLength())
        vrTextDir.normalize();

      double dOffset = fabs(pDbDim->dimgap());

      if (pDbDim->dimgap() < 0)
        dOffset = 0;

      OdGeLine3d lineBoxStart(arrTextBox[0] - vrTextDir * dOffset, vrTextYDir);
      OdGeLine3d lineBoxEnd(arrTextBox[1] + vrTextDir * dOffset, vrTextYDir);

      OdGePoint3d ptIsectStart, ptIsectEnd;

      if (lineDimDir.intersectWith(lineBoxStart, ptIsectStart) && lineDimDir.intersectWith(lineBoxEnd, ptIsectEnd))
      {
        double dRadius = ptCenter.distanceTo(ptChord);
        double dStart = ptCenter.distanceTo(ptIsectStart);
        double dEnd = ptCenter.distanceTo(ptIsectEnd);

        if (dStart < dRadius || dEnd < dRadius)
        {
          if (bInlineText)
            ptRealTextPos = (dStart > dEnd) ? ptIsectStart : ptIsectEnd;
          else
            ptRealTextPos = (dStart > dEnd) ? ptIsectEnd : ptIsectStart;
        }
        else
        {
          if (bInlineText)
            ptRealTextPos = (dStart > dEnd) ? ptIsectEnd : ptIsectStart;
          else
            ptRealTextPos = (dStart > dEnd) ? ptIsectStart : ptIsectEnd;
        }
      }
      else
        bRet = false;

      if( bManualText && bRet )
      {
        OdGePoint3d ptLineBase;

        if (bInlineText)
          ptLineBase = arrTextBox[0] + vrTextYDir * arrTextBox[0].distanceTo(arrTextBox[3]) / 2.0;
        else if(pDbDim->dimgap() < 0)
          ptLineBase = arrTextBox[0] - vrTextYDir * (pDbDim->dimtvp() + 2*fabs(pDbDim->dimgap()));
        else
          ptLineBase = arrTextBox[0] - vrTextYDir * (pDbDim->dimtvp() + fabs(pDbDim->dimgap()));

        OdGeLine3d lineBase(ptLineBase, vrTextDir);

        OdGePoint3d ptIsect = lineBase.closestPointTo(ptRealTextPos);

        OdGeVector3d vrHalfDir = (ptChord - ptRealTextPos)/2.0;

        ptRealTextPos = ptIsect;
        ptAddTextPos  = ptIsect + vrHalfDir;
        bAddPoint     = true;
      }
    }
  }
  else
    bRet = false;

  return bRet;
}

//---------------------------------------------------------------------------------------------------

bool isTextOutside(const OdGePoint3d& ptStart, const OdGePoint3d& ptEnd, const OdGePoint3d& ptTextPos)
{
  OdGeLine3d    lineStartEnd(ptStart, ptEnd);
  OdGeLineSeg3d segStartEnd(ptStart, ptEnd);

  OdGePoint3d ptProj = lineStartEnd.closestPointTo(ptTextPos);

  bool bRet = !segStartEnd.isOn(ptProj);

  return bRet;
}

//---------------------------------------------------------------------------------------------------

void connectDimensionBlock(const OdDbDimension* pDbDim, OdDgDimension* pDgDim)
{
  if (!pDbDim || !pDgDim || OdDgnExportContext::getRecomputeDimensions())
    return;

  OdDbObjectId  idBlock = pDbDim->dimBlockId();
  OdDgElementId idDgBlock = OdDgnExportContext::getElementId(idBlock);

  if( idDgBlock.isNull() && !idBlock.isNull() )
  {
    OdDbBlockTableRecordPtr pDbBlock = idBlock.openObject(OdDb::kForRead);

    OdDgSharedCellDefinitionTablePtr pDgBlockTable = pDgDim->database()->getSharedCellDefinitionTable(OdDg::kForWrite);

    OdDgSharedCellDefinitionPtr pDgBlockDef = OdDgSharedCellDefinition::createObject();
    pDgBlockDef->setName(pDbBlock->getName());
    pDgBlockDef->setDescription(pDbBlock->comments());
    pDgBlockDef->setOrigin(pDbBlock->origin());
    pDgBlockDef->set3dFormatFlag(OdDgnExportContext::is3d());
    pDgBlockTable->add(pDgBlockDef);

    double dOldScaleFactor = OdDgnExportContext::getLineStyleScaleCorrectionFactor();
    OdDgnExportContext::setLineStyleScaleCorrectionFactor(1.0 / OdDgnExportContext::getMasterToUORsScale());

    OdDgnExportContext::addProcessingBlockId(pDbBlock->id());
    DgnExporter::copyBlock(pDbBlock, pDgBlockDef);
    OdDgnExportContext::removeProcessingBlockId(pDbBlock->id());

    OdDgnExportContext::setLineStyleScaleCorrectionFactor(dOldScaleFactor);

    OdDgnExportContext::addDgnElement(pDbBlock->objectId(), pDgBlockDef->elementId());

    pDgBlockDef->transformBy(OdGeMatrix3d::scaling(OdDgnExportContext::getMasterToUORsScale()));

    idDgBlock = pDgBlockDef->elementId();
  }

  if (idBlock.isNull())
    return;

  OdDgDimOptionProxyCellPtr pProxyCellOption = OdDgDimOption::createObject(OdDgDimOption::kProxyCell);
  pDgDim->setOption(pProxyCellOption);

  OdGeMatrix3d matTransform = pDbDim->dimBlockTransform();
  OdGeVector3d vrTranslate = matTransform.translation();
  vrTranslate *= OdDgnExportContext::getMasterToUORsScale();
  matTransform.setTranslation(vrTranslate);

  pProxyCellOption->setDrawDimensionAsCell(pDgDim, idDgBlock, matTransform);
}

//---------------------------------------------------------------------------------------------------

void exportRadialDimension(OdDbDimension* pDbDim, 
                             OdDgElement* owner, 
                               const OdGePoint3d& ptCenter, 
                                 const OdGePoint3d& ptChord)
{
  OdDgDimRadiusExtendedPtr pDgDim = OdDgDimRadiusExtended::createObject();
  pDgDim->setDatabaseDefaults(owner->database());
  copyElementProperties(pDgDim, pDbDim, owner->database(), false);

  OdDgDimensionExportPEPtr pDgDimPE = OdDgDimensionExportPEPtr(OdRxObjectPtr(pDgDim));

  OdGePoint3d ptTextPos = pDbDim->textPosition();
  OdGeVector3d vrNormal = pDbDim->normal();

  // calculate dimension rotation

  OdGeMatrix3d matRotation = OdGeMatrix3d::planeToWorld(vrNormal);

  OdGeVector3d vrDimStartEnd = ptChord - ptCenter;

  if (!vrDimStartEnd.isZeroLength())
  {
    vrDimStartEnd.normalize();

    OdGeVector3d vrXAxis = OdGeVector3d::kXAxis;
    vrXAxis.transformBy(matRotation);
    double dAngle = vrXAxis.angleTo(vrDimStartEnd, vrNormal);
    matRotation = OdGeMatrix3d::rotation(dAngle, vrNormal) * matRotation;
    matRotation.setTranslation(OdGeVector3d::kIdentity);

    pDgDim->set3dFlag(true);
    OdGeQuaternion quatRot;
    quatRot.set(matRotation);
    pDgDim->setRotation(quatRot);
  }

  // calculate center, chord and text position

  OdDgDimPoint ptDimCenter;
  OdDgDimPoint ptDimChord;
  OdDgDimPoint ptDimDir;
  OdDgDimPoint ptDimText;

  ptDimCenter.setPoint(ptCenter);
  ptDimCenter.setRelative(false);

  ptDimChord.setPoint(ptChord);
  ptDimChord.setRelative(false);

  ptDimText.setPoint(ptTextPos);
  ptDimText.setRelative(false);

  pDgDim->setCrossCenterFlag(false);

  if (pDbDim->dimtmove() == 0)
  {
    if (pDbDim->dimtofl() && isTextOutside(ptCenter, ptChord, ptTextPos))
      pDgDim->setLeader(OdDgDimTool::kRadiusExt1);
    else
      pDgDim->setLeader(OdDgDimTool::kRadius);
  }
  else
    pDgDim->setLeader(OdDgDimTool::kDiameter);

  pDgDim->addPoint(ptDimCenter);
  pDgDim->addPoint(ptDimChord);

  if (pDbDim->dimtmove() != 0)
    pDgDim->addPoint(ptDimChord);

  pDgDim->addPoint(ptDimText);

  // Apply dimension settings

  applyDimensionSettings(pDbDim, pDgDim, owner->database(), getOwnerModelId(owner), ptCenter, ptChord, ptChord, vrDimStartEnd, 0, pDgDim->getPointsCount() - 1, false, false);

  bool bInlineText = (pDbDim->dimtad() == 0) || pDbDim->dimalt();
  bool bHorizontal = pDgDim->getHorizontalFlag();

  if (bHorizontal && !bInlineText)
  {
    OdDgDimGeometry dimGeom;
    pDgDim->getDimGeometry(dimGeom);
    dimGeom.setMargin(0.0);
    pDgDim->setDimGeometry(dimGeom);
  }

  // Calculate text position

  OdGePoint3d ptRealTextPos;
  OdGePoint3d ptAddTextPos;
  bool        bUseAddPos = false;

  if (getRadialDimensionTextPosition(pDbDim, ptCenter, ptChord, vrDimStartEnd, bInlineText, bHorizontal, ptRealTextPos, ptAddTextPos, bUseAddPos))
  {
    if (bUseAddPos)
    {
      ptDimText = pDgDim->getPoint(pDgDim->getPointsCount() - 1);
      ptDimText.setPoint(ptRealTextPos);

      OdDgDimPoint ptAdd;

      ptAdd.setPoint(ptAddTextPos);
      ptAdd.setRelative(false);

      pDgDim->setPoint(pDgDim->getPointsCount() - 1, ptAdd);
      pDgDim->addPoint(ptDimText);
    }
    else
    {
      ptDimText = pDgDim->getPoint(pDgDim->getPointsCount() - 1);
      ptDimText.setPoint(ptRealTextPos);
      pDgDim->setPoint(pDgDim->getPointsCount() - 1, ptDimText);
    }
  }

  // Set correct justification for all points

  for (OdUInt32 i = 0; i < pDgDim->getPointsCount(); i++)
  {
    OdDgDimPoint ptCur = pDgDim->getPoint(i);
    ptCur.setJustification(OdDgDimTextInfo::kCenterLeftText);
    pDgDim->setPoint(i, ptCur);
  }

  if (!pDbDim->dimpost().isEmpty())
    pDgDimPE->setPrefix(pDgDim, OdDgDimTool::kCsNone);

  // Save association data

  // Set annotation data

  addElementToDatabase(pDgDim, owner);
  appendOdDgElementToOwner(owner, pDgDim);
  OdDgnExportContext::addDgnElement(pDbDim->objectId(), pDgDim->elementId());

  // Get associative points

  bool bSetBlock = true;

  OdDbDimAssocPtr pDimAssoc = getDimAssoc(pDbDim);

  if (!pDimAssoc.isNull())
  {
    OdDbOsnapPointRefPtr ptAssoc = pDimAssoc->pointRef(1);

    if (!ptAssoc.isNull())
    {
      OdDgnExportDimAssocData dimAssocData;
      OdDgnExportDimAssoc dgAssoc;
      dgAssoc.m_uPointIndex = 0;
      dgAssoc.m_pPoint = ptAssoc;
      dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      dimAssocData.m_bDiamAssoc = true;
      dimAssocData.m_b2LinesAssoc = false;
      dimAssocData.m_idDgDimension = pDgDim->elementId();
      dimAssocData.m_idDbDimension = pDbDim->objectId();
      OdDgnExportContext::addDimensionAssoc(dimAssocData);
      bSetBlock = false;
    }
  }

  // Set dimension block

  if( bSetBlock )
    connectDimensionBlock(pDbDim, pDgDim);

}

//---------------------------------------------------------------------------------------------------

void OdDbRotatedDimensionExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbRotatedDimension* pDbDim = (OdDbRotatedDimension*)(pDbEntity);

  if( !pDbDim || !owner )
    return;

  OdDgDimSizeArrowPtr pDgDim = OdDgDimSizeArrow::createObject();
  pDgDim->setDatabaseDefaults(owner->database());
  copyElementProperties(pDgDim, pDbDim, owner->database(), false);

  OdDgDimensionExportPEPtr pDgDimPE = OdDgDimensionExportPEPtr(OdRxObjectPtr(pDgDim));

  OdGePoint3d ptStart   = pDbDim->xLine1Point();
  OdGePoint3d ptEnd     = pDbDim->xLine2Point();
  OdGePoint3d ptLine    = pDbDim->dimLinePoint();
  OdGeVector3d vrNormal = pDbDim->normal();

  // calculate dimension rotation

  OdGeMatrix3d matRotation = OdGeMatrix3d::planeToWorld(vrNormal);
  matRotation = OdGeMatrix3d::rotation(pDbDim->rotation(), vrNormal) * matRotation;
  matRotation.setTranslation(OdGeVector3d::kIdentity);

  pDgDim->set3dFlag(true);
  OdGeQuaternion quatRot;
  quatRot.set(matRotation);
  pDgDim->setRotation(quatRot);

  // calculate position, offsets and oblique of dimension points

  OdGeVector3d vrDimDir = OdGeVector3d::kXAxis;
  vrDimDir.transformBy(matRotation);

  OdGeVector3d vrExtDir = vrDimDir;

  double dOblique = pDbDim->oblique();

  if (OdEqual(dOblique, OdaPI2) || OdEqual(dOblique, 3 * OdaPI2 / 2.0) || OdEqual(dOblique, -OdaPI2))
    dOblique = 0.0;

  vrExtDir.rotateBy(OdaPI2 + dOblique, vrNormal);

  OdGeLine3d lineExt1(ptStart, vrExtDir);
  OdGeLine3d lineExt2(ptEnd, vrExtDir);
  OdGeLine3d lineDim(ptLine, vrDimDir);

  OdGePoint3d ptDimStart, ptDimEnd;

  if( !lineExt1.intersectWith(lineDim, ptDimStart) )
    ptDimStart = ptStart;

  if( !lineExt2.intersectWith(lineDim, ptDimEnd) )
    ptDimEnd = ptEnd;

  bool bNegativeExt1 = false;
  bool bNegativeExt2 = false;

  OdGeVector3d vrExt1Sign = ptDimStart - ptStart;
  OdGeVector3d vrExt2Sign = ptDimEnd - ptEnd;

  if( !vrExt1Sign.isZeroLength() && !vrExtDir.isZeroLength() )
    bNegativeExt1 = !vrExt1Sign.isCodirectionalTo(vrExtDir);

  if (!vrExt2Sign.isZeroLength() && !vrExtDir.isZeroLength())
    bNegativeExt2 = !vrExt2Sign.isCodirectionalTo(vrExtDir);

  OdDgDimPoint ptExt1;
  OdDgDimPoint ptExt2;

  ptExt1.setPoint(ptStart);
  ptExt1.setRelative(false);

  if( bNegativeExt1 )
    ptExt1.setOffsetToDimLine(-ptDimStart.distanceTo(ptStart));
  else
    ptExt1.setOffsetToDimLine(ptDimStart.distanceTo(ptStart));

  ptExt2.setPoint(ptEnd);
  ptExt2.setRelative(false);

  if( bNegativeExt2 )
    ptExt2.setOffsetToDimLine(-ptDimEnd.distanceTo(ptEnd));
  else
    ptExt2.setOffsetToDimLine(ptDimEnd.distanceTo(ptEnd));

  pDgDim->addPoint(ptExt1);
  pDgDim->addPoint(ptExt2);

  if( !OdZero(dOblique) )
    pDgDimPE->setWitnessLineSlantAngle(pDgDim, dOblique);

  // To do: Jog symbol

  // Apply dimension settings

  applyDimensionSettings(pDbDim, pDgDim, owner->database(), getOwnerModelId(owner), ptDimStart, ptDimEnd, ptDimEnd, vrDimDir, 0, 1, true, false);

  // Save association data

  // Set annotation data

  addElementToDatabase(pDgDim, owner);
  appendOdDgElementToOwner(owner, pDgDim);
  OdDgnExportContext::addDgnElement(pDbDim->objectId(), pDgDim->elementId());

  // Get associative points

  bool bSetBlock = true;

  OdDbDimAssocPtr pDimAssoc = getDimAssoc(pDbDim);

  if( !pDimAssoc.isNull() )
  {
    OdDbOsnapPointRefPtr ptAssoc1 = pDimAssoc->pointRef(0);
    OdDbOsnapPointRefPtr ptAssoc2 = pDimAssoc->pointRef(1);

    OdGePoint3d ptXLine1 = pDbDim->xLine1Point();
    OdGePoint3d ptXLine2 = pDbDim->xLine2Point();

    OdDgnExportDimAssocData dimAssocData;
    OdDgnExportDimAssoc dgAssoc;

    if( ptStart.isEqualTo(ptXLine1) )
    {
      if( !ptAssoc1.isNull() )
      { 
        dgAssoc.m_uPointIndex = 0;
        dgAssoc.m_pPoint = ptAssoc1;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }
    else if (ptStart.isEqualTo(ptXLine2))
    {
      if (!ptAssoc2.isNull())
      {
        dgAssoc.m_uPointIndex = 0;
        dgAssoc.m_pPoint = ptAssoc2;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }

    if (ptEnd.isEqualTo(ptXLine2))
    {
      if (!ptAssoc2.isNull())
      {
        dgAssoc.m_uPointIndex = 1;
        dgAssoc.m_pPoint = ptAssoc2;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }
    else if (ptEnd.isEqualTo(ptXLine1))
    {
      if (!ptAssoc1.isNull())
      {
        dgAssoc.m_uPointIndex = 1;
        dgAssoc.m_pPoint = ptAssoc1;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }

    if( !dimAssocData.m_arrAccocPts.isEmpty() )
    {
      dimAssocData.m_idDgDimension = pDgDim->elementId();
      dimAssocData.m_idDbDimension = pDbDim->objectId();
      dimAssocData.m_bDiamAssoc    = false;
      dimAssocData.m_b2LinesAssoc = false;
      OdDgnExportContext::addDimensionAssoc(dimAssocData);
      bSetBlock = false;
    }
  }

  // Set dimension block

  if( bSetBlock )
    connectDimensionBlock(pDbDim, pDgDim);

}

//---------------------------------------------------------------------------------------------------

void OdDb2LineAngularDimensionExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDb2LineAngularDimension* pDbDim = (OdDb2LineAngularDimension*)(pDbEntity);

  if (!pDbDim || !owner)
    return;

  OdGePoint3d ptXLine1Start = pDbDim->xLine1Start();
  OdGePoint3d ptXLine1End   = pDbDim->xLine1End();
  OdGePoint3d ptXLine2Start = pDbDim->xLine2Start();
  OdGePoint3d ptXLine2End   = pDbDim->xLine2End();
  OdGePoint3d ptArc = pDbDim->arcPoint();

  OdGeLine3d xLine1(ptXLine1Start, ptXLine1End);
  OdGeLine3d xLine2(ptXLine2Start, ptXLine2End);

  OdGePoint3d ptOrigin;

  if( !xLine1.intersectWith(xLine2, ptOrigin) )
    return;

  double dRadius = ptOrigin.distanceTo(ptArc);

  OdGeVector3d vrOriginArc = ptArc - ptOrigin;
  OdGeVector3d vrLine1 = ptXLine1End - ptXLine1Start;
  OdGeVector3d vrLine2 = ptXLine2End - ptXLine2Start;

  if( vrOriginArc.isZeroLength() || vrLine1.isZeroLength() || vrLine2.isZeroLength() )
    return;

  vrOriginArc.normalize();
  vrLine1.normalize();
  vrLine2.normalize();

  if( vrOriginArc.angleTo(vrLine1) > OdaPI2 )
    vrLine1 = -vrLine1;

  if( vrOriginArc.angleTo(vrLine2) > OdaPI2 )
    vrLine2 = -vrLine2;

  OdGePoint3d ptDimLineStart = ptOrigin + vrLine1 * dRadius;
  OdGePoint3d ptDimLineEnd   = ptOrigin + vrLine2 * dRadius;

  double dLine1Start  = ptDimLineStart.distanceTo(ptXLine1Start);
  double dLine1End    = ptDimLineStart.distanceTo(ptXLine1End);
  double dLine2Start  = ptDimLineEnd.distanceTo(ptXLine2Start);
  double dLine2End    = ptDimLineEnd.distanceTo(ptXLine2End);

  double dStartExtLength = 0;
  double dEndExtLength = 0;
  OdGePoint3d ptExtStart, ptExtEnd;

  if( dLine1Start <= dLine1End )
  {
    ptExtStart = ptXLine1Start;
    dStartExtLength = dLine1Start;
  }
  else
  {
    ptExtStart = ptXLine1End;
    dStartExtLength = dLine1End;
  }

  if (dLine2Start <= dLine2End)
  {
    ptExtEnd = ptXLine2Start;
    dEndExtLength = dLine2Start;
  }
  else
  {
    ptExtEnd = ptXLine2End;
    dEndExtLength = dLine2End;
  }

  OdGeVector3d vrStartCheck = ptExtStart - ptOrigin;
  OdGeVector3d vrEndCheck   = ptExtEnd   - ptOrigin;

  if( (!vrStartCheck.isCodirectionalTo(vrLine1) && ((dStartExtLength > dRadius) || vrStartCheck.length() > dRadius))
      || vrStartCheck.isZeroLength() 
    )
  {
    ptExtStart = ptOrigin + vrLine1 * dRadius/20.0;
  }

  if ((!vrEndCheck.isCodirectionalTo(vrLine2) && ((dEndExtLength > dRadius) || vrEndCheck.length() > dRadius))
      || vrEndCheck.isZeroLength() 
     )
  {
    ptExtEnd = ptOrigin + vrLine2 * dRadius / 20.0;
  }

  OdGeLineSeg3d segXLine1(ptXLine1Start, ptXLine1End);
  OdGeLineSeg3d segXLine2(ptXLine2Start, ptXLine2End);

  bool bShowExt1 = !segXLine1.isOn(ptOrigin) || !segXLine1.isOn(ptDimLineStart);
  bool bShowExt2 = !segXLine2.isOn(ptOrigin) || !segXLine2.isOn(ptDimLineEnd);

  OdGeVector3d vrNormal = pDbDim->normal();

  OdGeVector3d vrDimStart = ptDimLineStart - ptOrigin;
  OdGeVector3d vrDimEnd = ptDimLineEnd - ptOrigin;

  if( vrDimStart.angleTo(vrDimEnd, vrNormal) > OdaPI )
  {
    OdGePoint3d ptTmp = ptDimLineStart;
    ptDimLineStart = ptDimLineEnd;
    ptDimLineEnd = ptTmp;
    ptTmp = ptExtStart;
    ptExtStart = ptExtEnd;
    ptExtEnd = ptTmp;
    bool bTmp = bShowExt1;
    bShowExt1 = bShowExt2;
    bShowExt2 = bTmp;
  }

  OdGeVector3d vrDimDir = ptDimLineStart - ptOrigin;

  if (!vrDimDir.isZeroLength())
    vrDimDir.normalize();

  OdDgDimAngleLinesPtr pDgDim = OdDgDimAngleLines::createObject();
  pDgDim->setDatabaseDefaults(owner->database());
  copyElementProperties(pDgDim, pDbDim, owner->database(), false);

  // calculate dimension rotation

  OdGeMatrix3d matRotation = OdGeMatrix3d::planeToWorld(vrNormal);
  OdGeVector3d vrXAxis = OdGeVector3d::kXAxis;
  vrXAxis.transformBy(matRotation);

  double dAngle = vrXAxis.angleTo(vrDimDir, vrNormal);

  matRotation = OdGeMatrix3d::rotation(dAngle, vrNormal) * matRotation;
  matRotation.setTranslation(OdGeVector3d::kIdentity);

  pDgDim->set3dFlag(true);
  OdGeQuaternion quatRot;
  quatRot.set(matRotation);
  pDgDim->setRotation(quatRot);

  OdDgDimPoint ptDimOrigin;
  OdDgDimPoint ptDimStart;
  OdDgDimPoint ptDimEnd;

  ptDimOrigin.setPoint(ptOrigin);

  ptDimStart.setPoint(ptExtStart);

  OdGeVector3d vrExtLine1Dir = ptDimLineStart - ptExtStart;

  if( vrExtLine1Dir.angleTo(vrDimDir) < OdaPI2 )
    ptDimStart.setOffsetToDimLine(vrExtLine1Dir.length());
  else
    ptDimStart.setOffsetToDimLine(-vrExtLine1Dir.length());

  ptDimEnd.setPoint(ptExtEnd);

  pDgDim->addPoint(ptDimOrigin);
  pDgDim->addPoint(ptDimStart);
  pDgDim->addPoint(ptDimEnd);

  // calculate position, offsets and oblique of dimension points

  applyDimensionSettings(pDbDim, pDgDim, owner->database(), getOwnerModelId(owner), ptDimLineStart, ptDimLineEnd, ptOrigin, vrDimDir, 1, 2, true, true, true);

  if( !bShowExt1 )
  {
    ptDimStart = pDgDim->getPoint(1);
    ptDimStart.setNoWitnessLineFlag(true);
    pDgDim->setPoint(1, ptDimStart);
  }

  if( !bShowExt2 )
  {
    ptDimEnd = pDgDim->getPoint(2);
    ptDimEnd.setNoWitnessLineFlag(true);
    pDgDim->setPoint(2, ptDimEnd);
  }

  // Set annotation data

  addElementToDatabase(pDgDim, owner);
  appendOdDgElementToOwner(owner, pDgDim);
  OdDgnExportContext::addDgnElement(pDbDim->objectId(), pDgDim->elementId());

  // Get associative points

  bool bSetBlock = true;

  OdDbDimAssocPtr pDimAssoc = getDimAssoc(pDbDim);

  if (!pDimAssoc.isNull())
  {
    OdDbOsnapPointRefPtr ptAssoc1 = pDimAssoc->pointRef(0);
    OdDbOsnapPointRefPtr ptAssoc2 = pDimAssoc->pointRef(1);
    OdDbOsnapPointRefPtr ptAssoc3 = pDimAssoc->pointRef(2);
    OdDbOsnapPointRefPtr ptAssoc4 = pDimAssoc->pointRef(3);

    OdDgnExportDimAssocData dimAssocData;
    OdDgnExportDimAssoc dgAssoc;

    if (ptExtStart.isEqualTo(ptXLine1Start))
    {
      if (!ptAssoc1.isNull())
      {
        dgAssoc.m_uPointIndex = 1;
        dgAssoc.m_pPoint = ptAssoc1;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }
    else if (ptExtStart.isEqualTo(ptXLine1End))
    {
      if (!ptAssoc2.isNull())
      {
        dgAssoc.m_uPointIndex = 1;
        dgAssoc.m_pPoint = ptAssoc2;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }
    else if (ptExtStart.isEqualTo(ptXLine2Start))
    {
      if (!ptAssoc3.isNull())
      {
        dgAssoc.m_uPointIndex = 1;
        dgAssoc.m_pPoint = ptAssoc3;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }
    else if (ptExtStart.isEqualTo(ptXLine2End))
    {
      if (!ptAssoc4.isNull())
      {
        dgAssoc.m_uPointIndex = 1;
        dgAssoc.m_pPoint = ptAssoc4;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }

    if (ptExtEnd.isEqualTo(ptXLine2Start))
    {
      if (!ptAssoc3.isNull())
      {
        dgAssoc.m_uPointIndex = 2;
        dgAssoc.m_pPoint = ptAssoc3;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }
    else if (ptExtEnd.isEqualTo(ptXLine2End))
    {
      if (!ptAssoc4.isNull())
      {
        dgAssoc.m_uPointIndex = 2;
        dgAssoc.m_pPoint = ptAssoc4;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }
    else if (ptExtEnd.isEqualTo(ptXLine1Start))
    {
      if (!ptAssoc1.isNull())
      {
        dgAssoc.m_uPointIndex = 2;
        dgAssoc.m_pPoint = ptAssoc1;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }
    else if (ptExtEnd.isEqualTo(ptXLine1End))
    {
      if (!ptAssoc2.isNull())
      {
        dgAssoc.m_uPointIndex = 2;
        dgAssoc.m_pPoint = ptAssoc2;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }

    if (dimAssocData.m_arrAccocPts.size() > 1)
    {
      dimAssocData.m_idDgDimension = pDgDim->elementId();
      dimAssocData.m_idDbDimension = pDbDim->objectId();
      dimAssocData.m_bDiamAssoc    = false;
      dimAssocData.m_b2LinesAssoc  = true;
      OdDgnExportContext::addDimensionAssoc(dimAssocData);
      bSetBlock = false;
    }
  }

  // Set dimension block

  if( bSetBlock )
    connectDimensionBlock(pDbDim, pDgDim);

}

//---------------------------------------------------------------------------------------------------

void exportAngularDimension(const OdDbDimension* pDbDim, 
                              OdDgElement* owner,
                                OdGePoint3d& ptXLine1,
                                  OdGePoint3d& ptXLine2,
                                    OdGePoint3d& ptArc,
                                      OdGePoint3d& ptOrigin,
                                        bool bShowLength,
                                          bool bShowArcSymbol )
{
  double dRadius = ptOrigin.distanceTo(ptArc);

  OdGeVector3d vrOriginArc = ptArc - ptOrigin;
  OdGeVector3d vrLine1 = ptXLine1 - ptOrigin;
  OdGeVector3d vrLine2 = ptXLine2 - ptOrigin;

  if (vrLine1.angleTo(vrLine2, pDbDim->normal()) < vrLine1.angleTo(vrOriginArc, pDbDim->normal()))
  {
    OdGePoint3d ptTmp = ptXLine1;
    ptXLine1 = ptXLine2;
    ptXLine2 = ptTmp;
    OdGeVector3d vrTmp = vrLine1;
    vrLine1 = vrLine2;
    vrLine2 = vrTmp;
  }

  if (vrOriginArc.isZeroLength() || vrLine1.isZeroLength() || vrLine2.isZeroLength())
    return;

  vrOriginArc.normalize();
  vrLine1.normalize();
  vrLine2.normalize();

  OdGePoint3d ptDimLineStart = ptOrigin + vrLine1 * dRadius;
  OdGePoint3d ptDimLineEnd = ptOrigin + vrLine2 * dRadius;

  OdGePoint3d ptExtStart = ptXLine1;
  OdGePoint3d ptExtEnd = ptXLine2;

  OdGeVector3d vrNormal = pDbDim->normal();
  OdGeVector3d vrDimDir = ptDimLineStart - ptOrigin;

  if (!vrDimDir.isZeroLength())
    vrDimDir.normalize();

  OdDgDimAngleLinesPtr pDgDim = OdDgDimAngleLines::createObject();
  pDgDim->setDatabaseDefaults(owner->database());
  copyElementProperties(pDgDim, pDbDim, owner->database(), false);

  // calculate dimension rotation

  OdGeMatrix3d matRotation = OdGeMatrix3d::planeToWorld(vrNormal);
  OdGeVector3d vrXAxis = OdGeVector3d::kXAxis;
  vrXAxis.transformBy(matRotation);

  double dAngle = vrXAxis.angleTo(vrDimDir, vrNormal);

  matRotation = OdGeMatrix3d::rotation(dAngle, vrNormal) * matRotation;
  matRotation.setTranslation(OdGeVector3d::kIdentity);

  pDgDim->set3dFlag(true);
  OdGeQuaternion quatRot;
  quatRot.set(matRotation);
  pDgDim->setRotation(quatRot);

  OdDgDimPoint ptDimOrigin;
  OdDgDimPoint ptDimStart;
  OdDgDimPoint ptDimEnd;

  ptDimOrigin.setPoint(ptOrigin);

  ptDimStart.setPoint(ptExtStart);

  OdGeVector3d vrExtLine1Dir = ptDimLineStart - ptExtStart;

  if (vrExtLine1Dir.angleTo(vrDimDir) < OdaPI2)
    ptDimStart.setOffsetToDimLine(vrExtLine1Dir.length());
  else
    ptDimStart.setOffsetToDimLine(-vrExtLine1Dir.length());

  ptDimEnd.setPoint(ptExtEnd);

  pDgDim->addPoint(ptDimOrigin);
  pDgDim->addPoint(ptDimStart);
  pDgDim->addPoint(ptDimEnd);

  // calculate position, offsets and oblique of dimension points

  applyDimensionSettings(pDbDim, pDgDim, owner->database(), getOwnerModelId(owner), ptDimLineStart, ptDimLineEnd, ptOrigin, vrDimDir, 1, 2, true, true, !bShowLength);

  if( bShowArcSymbol )
  {
    OdDgDimensionExportPEPtr pDgDimPE = OdDgDimensionExportPEPtr(OdRxObjectPtr(pDgDim));
    pDgDimPE->setArcSymbolFlag(pDgDim, true);
  }

  // Save association data

  // Set annotation data

  addElementToDatabase(pDgDim, owner);
  appendOdDgElementToOwner(owner, pDgDim);
  OdDgnExportContext::addDgnElement(pDbDim->objectId(), pDgDim->elementId());

  // Get associative points

  bool bSetBlock = true;

  OdDbDimAssocPtr pDimAssoc = getDimAssoc(pDbDim);

  if (!pDimAssoc.isNull())
  {
    OdDbOsnapPointRefPtr ptAssoc1 = pDimAssoc->pointRef(0);
    OdDbOsnapPointRefPtr ptAssoc2 = pDimAssoc->pointRef(1);
    OdDbOsnapPointRefPtr ptAssoc3 = pDimAssoc->pointRef(2);

    OdDgnExportDimAssocData dimAssocData;
    OdDgnExportDimAssoc dgAssoc;

    if( !ptAssoc3.isNull() )
    {
      dgAssoc.m_uPointIndex = 0;
      dgAssoc.m_pPoint = ptAssoc3;
      dimAssocData.m_arrAccocPts.push_back(dgAssoc);
    }

    if (ptExtStart.isEqualTo(ptXLine1))
    {
      if (!ptAssoc1.isNull())
      {
        dgAssoc.m_uPointIndex = 1;
        dgAssoc.m_pPoint = ptAssoc1;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }
    else if (ptExtStart.isEqualTo(ptXLine2))
    {
      if (!ptAssoc2.isNull())
      {
        dgAssoc.m_uPointIndex = 1;
        dgAssoc.m_pPoint = ptAssoc2;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }

    if (ptExtEnd.isEqualTo(ptXLine2))
    {
      if (!ptAssoc2.isNull())
      {
        dgAssoc.m_uPointIndex = 2;
        dgAssoc.m_pPoint = ptAssoc2;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }
    else if (ptExtEnd.isEqualTo(ptXLine1))
    {
      if (!ptAssoc1.isNull())
      {
        dgAssoc.m_uPointIndex = 2;
        dgAssoc.m_pPoint = ptAssoc1;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }

    if (dimAssocData.m_arrAccocPts.size() > 2)
    {
      dimAssocData.m_idDgDimension = pDgDim->elementId();
      dimAssocData.m_idDbDimension = pDbDim->objectId();
      dimAssocData.m_bDiamAssoc    = false;
      dimAssocData.m_b2LinesAssoc  = false;
      OdDgnExportContext::addDimensionAssoc(dimAssocData);
      bSetBlock = false;
    }
  }

  // Set dimension block

  if( bSetBlock )
    connectDimensionBlock(pDbDim, pDgDim);

}

//---------------------------------------------------------------------------------------------------

void OdDb3PointAngularDimensionExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDb3PointAngularDimension* pDbDim = (OdDb3PointAngularDimension*)(pDbEntity);

  if (!pDbDim || !owner)
    return;

  OdGePoint3d ptXLine1 = pDbDim->xLine1Point();
  OdGePoint3d ptXLine2 = pDbDim->xLine2Point();
  OdGePoint3d ptArc    = pDbDim->arcPoint();
  OdGePoint3d ptOrigin = pDbDim->centerPoint();

  exportAngularDimension(pDbDim, owner, ptXLine1, ptXLine2, ptArc, ptOrigin, false, false);
}

//---------------------------------------------------------------------------------------------------

void OdDbArcDimensionExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbArcDimension* pDbDim = (OdDbArcDimension*)(pDbEntity);

  if (!pDbDim || !owner)
    return;

  OdGePoint3d ptXLine1 = pDbDim->xLine1Point();
  OdGePoint3d ptXLine2 = pDbDim->xLine2Point();
  OdGePoint3d ptArc = pDbDim->arcPoint();
  OdGePoint3d ptOrigin = pDbDim->centerPoint();

  exportAngularDimension(pDbDim, owner, ptXLine1, ptXLine2, ptArc, ptOrigin, true, pDbDim->arcSymbolType() != 2);
}

//---------------------------------------------------------------------------------------------------

void OdDbDiametricDimensionExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbDiametricDimension* pDbDim = (OdDbDiametricDimension*)(pDbEntity);

  if (!pDbDim || !owner)
    return;

  OdDgDimDiameterExtendedPtr pDgDim = OdDgDimDiameterExtended::createObject();
  pDgDim->setDatabaseDefaults(owner->database());
  copyElementProperties(pDgDim, pDbDim, owner->database(), false);

  OdDgDimensionExportPEPtr pDgDimPE = OdDgDimensionExportPEPtr(OdRxObjectPtr(pDgDim));

  OdGePoint3d ptCenter = pDbDim->farChordPoint();
  OdGePoint3d ptChord = pDbDim->chordPoint();

  OdGeVector3d vrDimStartEnd = ptChord - ptCenter;

  ptCenter = ptChord - vrDimStartEnd / 2.0;

  OdGePoint3d ptTextPos = pDbDim->textPosition();
  OdGeVector3d vrNormal = pDbDim->normal();

  // calculate dimension rotation

  OdGeMatrix3d matRotation = OdGeMatrix3d::planeToWorld(vrNormal);

  if (!vrDimStartEnd.isZeroLength())
  {
    vrDimStartEnd.normalize();

    OdGeVector3d vrXAxis = OdGeVector3d::kXAxis;
    vrXAxis.transformBy(matRotation);
    double dAngle = vrXAxis.angleTo(vrDimStartEnd, vrNormal);
    matRotation = OdGeMatrix3d::rotation(dAngle, vrNormal) * matRotation;
    matRotation.setTranslation(OdGeVector3d::kIdentity);

    pDgDim->set3dFlag(true);
    OdGeQuaternion quatRot;
    quatRot.set(matRotation);
    pDgDim->setRotation(quatRot);
  }

  // calculate center, chord and text position

  OdDgDimPoint ptDimCenter;
  OdDgDimPoint ptDimChord;
  OdDgDimPoint ptDimText;

  ptDimCenter.setPoint(ptCenter);
  ptDimCenter.setRelative(false);

  ptDimChord.setPoint(ptChord);
  ptDimChord.setRelative(false);

  ptDimText.setPoint(ptTextPos);
  ptDimText.setRelative(false);

  pDgDim->setCrossCenterFlag(false);

  if (pDbDim->dimtmove() == 0)
  {
    if (pDbDim->dimtofl() && isTextOutside(ptCenter, ptChord, ptTextPos))
      pDgDim->setLeader(OdDgDimTool::kRadiusExt2);
    else
      pDgDim->setLeader(OdDgDimTool::kRadius);
  }
  else
    pDgDim->setLeader(OdDgDimTool::kDiameter);

  pDgDim->addPoint(ptDimCenter);
  pDgDim->addPoint(ptDimChord);

  if (pDbDim->dimtmove() != 0)
    pDgDim->addPoint(ptDimChord);

  pDgDim->addPoint(ptDimText);

  // Apply dimension settings

  applyDimensionSettings(pDbDim, pDgDim, owner->database(), getOwnerModelId(owner), ptCenter, ptChord, ptChord, vrDimStartEnd, 0, pDgDim->getPointsCount() - 1, false, false);

  bool bInlineText = (pDbDim->dimtad() == 0) || pDbDim->dimalt();
  bool bHorizontal = pDgDim->getHorizontalFlag();

  if (bHorizontal && !bInlineText)
  {
    OdDgDimGeometry dimGeom;
    pDgDim->getDimGeometry(dimGeom);
    dimGeom.setMargin(0.0);
    pDgDim->setDimGeometry(dimGeom);
  }

  // Calculate text position

  OdGePoint3d ptRealTextPos;
  OdGePoint3d ptAddTextPos;
  bool        bUseAddPos = false;

  if (getRadialDimensionTextPosition(pDbDim, pDbDim->farChordPoint(), ptChord, vrDimStartEnd, bInlineText, bHorizontal, ptRealTextPos, ptAddTextPos, bUseAddPos))
  {
    if (bUseAddPos)
    {
      ptDimText = pDgDim->getPoint(pDgDim->getPointsCount() - 1);
      ptDimText.setPoint(ptRealTextPos);

      OdDgDimPoint ptAdd;

      ptAdd.setPoint(ptAddTextPos);
      ptAdd.setRelative(false);

      pDgDim->setPoint(pDgDim->getPointsCount() - 1, ptAdd);
      pDgDim->addPoint(ptDimText);
    }
    else
    {
      ptDimText = pDgDim->getPoint(pDgDim->getPointsCount() - 1);
      ptDimText.setPoint(ptRealTextPos);
      pDgDim->setPoint(pDgDim->getPointsCount() - 1, ptDimText);
    }
  }

  // Set correct justification for all points

  for (OdUInt32 i = 0; i < pDgDim->getPointsCount(); i++)
  {
    OdDgDimPoint ptCur = pDgDim->getPoint(i);
    ptCur.setJustification(OdDgDimTextInfo::kCenterLeftText);
    pDgDim->setPoint(i, ptCur);
  }

  if (!pDbDim->dimpost().isEmpty())
    pDgDimPE->setPrefix(pDgDim, OdDgDimTool::kCsNone);

  // Save association data

  // Set annotation data

  addElementToDatabase(pDgDim, owner);
  appendOdDgElementToOwner(owner, pDgDim);
  OdDgnExportContext::addDgnElement(pDbDim->objectId(), pDgDim->elementId());

  // Get associative points

  bool bSetBlock = true;

  OdDbDimAssocPtr pDimAssoc = getDimAssoc(pDbDim);

  if (!pDimAssoc.isNull())
  {
    OdDbOsnapPointRefPtr ptAssoc = pDimAssoc->pointRef(1);

    if( !ptAssoc.isNull() )
    {
      OdDgnExportDimAssocData dimAssocData;
      OdDgnExportDimAssoc dgAssoc;
      dgAssoc.m_uPointIndex = 0;
      dgAssoc.m_pPoint = ptAssoc;
      dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      dimAssocData.m_bDiamAssoc = true;
      dimAssocData.m_b2LinesAssoc = false;
      dimAssocData.m_idDgDimension = pDgDim->elementId();
      dimAssocData.m_idDbDimension = pDbDim->objectId();
      OdDgnExportContext::addDimensionAssoc(dimAssocData);
      bSetBlock = false;
    }
  }

  // Set dimension block

  if( bSetBlock )
    connectDimensionBlock(pDbDim, pDgDim);

}

//---------------------------------------------------------------------------------------------------

void OdDbRadialDimensionExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbRadialDimension* pDbDim = (OdDbRadialDimension*)(pDbEntity);

  if (!pDbDim || !owner)
    return;

  exportRadialDimension(pDbDim, owner, pDbDim->center(), pDbDim->chordPoint());
}

//---------------------------------------------------------------------------------------------------

void OdDbOrdinateDimensionExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbOrdinateDimension* pDbDim = (OdDbOrdinateDimension*)(pDbEntity);

  if (!pDbDim || !owner)
    return;

  bool bXAxis             = pDbDim->isUsingXAxis();
  OdGePoint3d ptOrigin    = pDbDim->origin();
  OdGePoint3d ptDefining  = pDbDim->definingPoint();
  OdGePoint3d ptText      = pDbDim->leaderEndPoint();

  OdDgDimOrdinatePtr pDgDim = OdDgDimOrdinate::createObject();
  pDgDim->setDatabaseDefaults(owner->database());
  copyElementProperties(pDgDim, pDbDim, owner->database(), false);

  OdDgDimPoint ptDimOrigin;
  OdDgDimPoint ptDimOrdinate;

  ptDimOrigin.setPoint(ptOrigin);
  ptDimOrigin.setRelative(false);

  ptDimOrdinate.setPoint(ptDefining);
  ptDimOrdinate.setRelative(false);

  pDgDim->setFreeLocationOfTxtFlag(true);
  pDgDim->addPoint(ptDimOrigin);
  pDgDim->addPoint(ptDimOrdinate);

  OdGeVector3d vrNormal = pDbDim->normal();

  // calculate dimension rotation

  OdGeMatrix3d matRotation = OdGeMatrix3d::planeToWorld(vrNormal);

  double dAngle = 0;
  OdGeVector3d vrDimDir = OdGeVector3d::kXAxis;

  if (bXAxis)
  {
    vrDimDir = OdGeVector3d::kYAxis;
    dAngle = OdaPI2;
  }

  vrDimDir.transformBy(matRotation);

  OdGeVector3d vrYDimDir = vrDimDir;
  vrYDimDir.rotateBy(OdaPI2, vrNormal);

  matRotation = OdGeMatrix3d::rotation(dAngle, vrNormal) * matRotation;
  matRotation.setTranslation(OdGeVector3d::kIdentity);

  pDgDim->set3dFlag(true);
  OdGeQuaternion quatRot;
  quatRot.set(matRotation);
  pDgDim->setRotation(quatRot);

  // Apply dimension settings

  applyDimensionSettings(pDbDim, pDgDim, owner->database(), getOwnerModelId(owner), ptOrigin, ptDefining, ptDefining, vrDimDir, 0, 1, false, false);

  // Set leaders

  ptDimOrigin = pDgDim->getPoint(0);
  ptDimOrigin.setNoWitnessLineFlag(true);
  ptDimOrigin.setJustification(OdDgDimTextInfo::kManualText);
  pDgDim->setPoint(0, ptDimOrigin);

  ptDimOrdinate = pDgDim->getPoint(1);
  ptDimOrdinate.setNoWitnessLineFlag(false);
  ptDimOrdinate.setJustification(OdDgDimTextInfo::kManualText);

  OdGeLine3d lineDimDir(ptDefining, vrDimDir);

  OdGePoint3d ptBase = lineDimDir.closestPointTo(ptText);

  OdGeVector3d vrBaseX = ptBase - ptDefining;
  OdGeVector3d vrBaseY = ptText - ptBase;

  double dXOffset = 0;
  double dYOffset = 0;

  if (vrBaseX.angleTo(vrDimDir) < OdaPI2)
    dXOffset = vrBaseX.length();
  else
    dXOffset = -vrBaseX.length();

  if (vrBaseY.angleTo(vrYDimDir) < OdaPI2)
    dYOffset = vrBaseY.length();
  else
    dYOffset = -vrBaseY.length();

  ptDimOrdinate.setOffsetToDimLine(dXOffset);
  ptDimOrdinate.setOffsetY(dYOffset);
  pDgDim->setPoint(1, ptDimOrdinate);

  OdDgDimGeometry dimGeom;
  pDgDim->getDimGeometry(dimGeom);
  dimGeom.setMargin(pDbDim->dimasz()*2.0);
  pDgDim->setDimGeometry(dimGeom);

  OdDgDimensionExportPEPtr pDgDimPE = OdDgDimensionExportPEPtr(OdRxObjectPtr(pDgDim));
  pDgDimPE->setLeftTerminator(pDgDim, OdDgDimTool::kTtNone);
  pDgDimPE->setRightTerminator(pDgDim, OdDgDimTool::kTtNone);

  // Save association data

  // Set annotation data

  addElementToDatabase(pDgDim, owner);
  appendOdDgElementToOwner(owner, pDgDim);
  OdDgnExportContext::addDgnElement(pDbDim->objectId(), pDgDim->elementId());

  // Get associative points

  bool bSetBlock = true;

  OdDbDimAssocPtr pDimAssoc = getDimAssoc(pDbDim);

  if (!pDimAssoc.isNull())
  {
    OdDbOsnapPointRefPtr ptAssoc1 = pDimAssoc->pointRef(0);
    OdDbOsnapPointRefPtr ptAssoc2 = pDimAssoc->pointRef(1);

    OdDgnExportDimAssocData dimAssocData;
    OdDgnExportDimAssoc dgAssoc;

    if (!ptAssoc1.isNull())
    {
      dgAssoc.m_uPointIndex = 0;
      dgAssoc.m_pPoint = ptAssoc1;
      dimAssocData.m_arrAccocPts.push_back(dgAssoc);
    }

    if (!ptAssoc2.isNull())
    {
      dgAssoc.m_uPointIndex = 1;
      dgAssoc.m_pPoint = ptAssoc2;
      dimAssocData.m_arrAccocPts.push_back(dgAssoc);
    }

    if (!dimAssocData.m_arrAccocPts.isEmpty())
    {
      dimAssocData.m_idDgDimension = pDgDim->elementId();
      dimAssocData.m_idDbDimension = pDbDim->objectId();
      dimAssocData.m_bDiamAssoc = false;
      dimAssocData.m_b2LinesAssoc = false;
      OdDgnExportContext::addDimensionAssoc(dimAssocData);
      bSetBlock = false;
    }
  }

  // Set dimension block

  if( bSetBlock )
    connectDimensionBlock(pDbDim, pDgDim);

}

//---------------------------------------------------------------------------------------------------

void OdDbAlignedDimensionExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbAlignedDimension* pDbDim = (OdDbAlignedDimension*)(pDbEntity);

  if (!pDbDim || !owner)
    return;

  OdDgDimSizeArrowPtr pDgDim = OdDgDimSizeArrow::createObject();
  pDgDim->setDatabaseDefaults(owner->database());
  copyElementProperties(pDgDim, pDbDim, owner->database(), false);

  OdDgDimensionExportPEPtr pDgDimPE = OdDgDimensionExportPEPtr(OdRxObjectPtr(pDgDim));

  OdGePoint3d ptStart = pDbDim->xLine1Point();
  OdGePoint3d ptEnd = pDbDim->xLine2Point();
  OdGePoint3d ptLine = pDbDim->dimLinePoint();
  OdGeVector3d vrNormal = pDbDim->normal();

  // calculate dimension rotation

  OdGeMatrix3d matRotation = OdGeMatrix3d::planeToWorld(vrNormal);

  OdGeVector3d vrDimStartEnd = ptEnd - ptStart;

  if (!vrDimStartEnd.isZeroLength())
  {
    vrDimStartEnd.normalize();

    OdGeVector3d vrXAxis = OdGeVector3d::kXAxis;
    vrXAxis.transformBy(matRotation);
    double dAngle = vrXAxis.angleTo(vrDimStartEnd, vrNormal);
    matRotation = OdGeMatrix3d::rotation(dAngle, vrNormal) * matRotation;
    matRotation.setTranslation(OdGeVector3d::kIdentity);

    pDgDim->set3dFlag(true);
    OdGeQuaternion quatRot;
    quatRot.set(matRotation);
    pDgDim->setRotation(quatRot);
  }

  // calculate position, offsets and oblique of dimension points

  OdGeVector3d vrDimDir = OdGeVector3d::kXAxis;
  vrDimDir.transformBy(matRotation);

  OdGeVector3d vrExtDir = vrDimDir;

  double dOblique = pDbDim->oblique();

  if (OdEqual(dOblique, OdaPI2) || OdEqual(dOblique, 3 * OdaPI2 / 2.0) || OdEqual(dOblique, -OdaPI2))
    dOblique = 0.0;

  vrExtDir.rotateBy(OdaPI2 + dOblique, vrNormal);

  OdGeLine3d lineExt1(ptStart, vrExtDir);
  OdGeLine3d lineExt2(ptEnd, vrExtDir);
  OdGeLine3d lineDim(ptLine, vrDimDir);

  OdGePoint3d ptDimStart, ptDimEnd;

  if (!lineExt1.intersectWith(lineDim, ptDimStart))
    ptDimStart = ptStart;

  if (!lineExt2.intersectWith(lineDim, ptDimEnd))
    ptDimEnd = ptEnd;

  bool bNegativeExt1 = false;
  bool bNegativeExt2 = false;

  OdGeVector3d vrExt1Sign = ptDimStart - ptStart;
  OdGeVector3d vrExt2Sign = ptDimEnd - ptEnd;

  if (!vrExt1Sign.isZeroLength() && !vrExtDir.isZeroLength())
    bNegativeExt1 = !vrExt1Sign.isCodirectionalTo(vrExtDir);

  if (!vrExt2Sign.isZeroLength() && !vrExtDir.isZeroLength())
    bNegativeExt2 = !vrExt2Sign.isCodirectionalTo(vrExtDir);

  OdDgDimPoint ptExt1;
  OdDgDimPoint ptExt2;

  ptExt1.setPoint(ptStart);
  ptExt1.setRelative(false);

  if (bNegativeExt1)
    ptExt1.setOffsetToDimLine(-ptDimStart.distanceTo(ptStart));
  else
    ptExt1.setOffsetToDimLine(ptDimStart.distanceTo(ptStart));

  ptExt2.setPoint(ptEnd);
  ptExt2.setRelative(false);

  if (bNegativeExt2)
    ptExt2.setOffsetToDimLine(-ptDimEnd.distanceTo(ptEnd));
  else
    ptExt2.setOffsetToDimLine(ptDimEnd.distanceTo(ptEnd));

  pDgDim->addPoint(ptExt1);
  pDgDim->addPoint(ptExt2);

  if (!OdZero(dOblique))
    pDgDimPE->setWitnessLineSlantAngle(pDgDim, dOblique);

  // Apply dimension settings

  applyDimensionSettings(pDbDim, pDgDim, owner->database(), getOwnerModelId(owner), ptDimStart, ptDimEnd, ptDimEnd, vrDimDir, 0, 1, true, false);

  // Save association data

  // Set annotation data

  addElementToDatabase(pDgDim, owner);
  appendOdDgElementToOwner(owner, pDgDim);
  OdDgnExportContext::addDgnElement(pDbDim->objectId(), pDgDim->elementId());

  // Get associative points

  bool bSetBlock = true;

  OdDbDimAssocPtr pDimAssoc = getDimAssoc(pDbDim);

  if (!pDimAssoc.isNull())
  {
    OdDbOsnapPointRefPtr ptAssoc1 = pDimAssoc->pointRef(0);
    OdDbOsnapPointRefPtr ptAssoc2 = pDimAssoc->pointRef(1);

    OdGePoint3d ptXLine1 = pDbDim->xLine1Point();
    OdGePoint3d ptXLine2 = pDbDim->xLine2Point();

    OdDgnExportDimAssocData dimAssocData;
    OdDgnExportDimAssoc dgAssoc;

    if (ptStart.isEqualTo(ptXLine1))
    {
      if (!ptAssoc1.isNull())
      {
        dgAssoc.m_uPointIndex = 0;
        dgAssoc.m_pPoint = ptAssoc1;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }
    else if (ptStart.isEqualTo(ptXLine2))
    {
      if (!ptAssoc2.isNull())
      {
        dgAssoc.m_uPointIndex = 0;
        dgAssoc.m_pPoint = ptAssoc2;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }

    if (ptEnd.isEqualTo(ptXLine2))
    {
      if (!ptAssoc2.isNull())
      {
        dgAssoc.m_uPointIndex = 1;
        dgAssoc.m_pPoint = ptAssoc2;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }
    else if (ptEnd.isEqualTo(ptXLine1))
    {
      if (!ptAssoc1.isNull())
      {
        dgAssoc.m_uPointIndex = 1;
        dgAssoc.m_pPoint = ptAssoc1;
        dimAssocData.m_arrAccocPts.push_back(dgAssoc);
      }
    }

    if (!dimAssocData.m_arrAccocPts.isEmpty())
    {
      dimAssocData.m_idDgDimension = pDgDim->elementId();
      dimAssocData.m_idDbDimension = pDbDim->objectId();
      dimAssocData.m_bDiamAssoc = false;
      dimAssocData.m_b2LinesAssoc = false;
      OdDgnExportContext::addDimensionAssoc(dimAssocData);
      bSetBlock = false;
    }
  }

  // Set dimension block

  if( bSetBlock )
    connectDimensionBlock(pDbDim, pDgDim);

}

//---------------------------------------------------------------------------------------------------

void OdDbRadialDimensionLargeExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbRadialDimensionLarge* pDbDim = (OdDbRadialDimensionLarge*)(pDbEntity);

  if (!pDbDim || !owner)
    return;

  exportRadialDimension(pDbDim, owner, pDbDim->center(), pDbDim->chordPoint());
}

//---------------------------------------------------------------------------------------------------

void setDimNotePropertiesFromLeader( OdDgDimNote* pDgDim, OdDbLeader* pDbLeader, OdDgDatabase* pDb )
{
  //------------------------------- Symbology ---------------------------------

  OdUInt32 uElementLineWeight = odDgnExportLineweightMapping(pDbLeader->lineWeight());
  OdUInt32 uElementColorIndex = getDgnColorIndex(pDbLeader->color(), pDb);

  OdUInt32 uDimLineLineWeight = uElementLineWeight;
  OdUInt32 uDimLineColorIndex = uElementColorIndex;

  OdCmColor         dimColor = pDbLeader->dimclrd();
  OdDb::LineWeight  dimLineWeight = pDbLeader->dimlwd();

  if (dimColor.colorMethod() != OdCmEntityColor::kByBlock)
    uDimLineColorIndex = getDgnColorIndex(dimColor, pDb);

  if (dimLineWeight != OdDb::kLnWtByBlock)
    uDimLineLineWeight = odDgnExportLineweightMapping(dimLineWeight);

  // 5. Apply dimension color to dgn element

  if (uElementColorIndex != uDimLineColorIndex)
    pDgDim->setColorIndex(uDimLineColorIndex);

  if (uElementLineWeight != uDimLineLineWeight)
    pDgDim->setLineWeight(uDimLineLineWeight);

  //------------------------------- Leader line -------------------------------

  pDgDim->setSplineFit(pDbLeader->isSplined());
  pDgDim->setLeaderLength(pDbLeader->dimasz()*2.0);
  pDgDim->setDimNoteInlineLeaderFlag(pDbLeader->hasHookLine());

  double dScale = pDbLeader->dimscale();

  //------------------------------- Arrow -------------------------------------

  OdDgDimGeometry dimGeom;
  pDgDim->getDimGeometry(dimGeom);

  double        dArrowSize = pDbLeader->dimasz() * dScale;
  double        dArrowHeight = pDbLeader->dimasz() /2.0 * dScale;

  OdDbObjectId  idArrow;
  OdDgnExportDimArrowheadType arrowType;

  if (pDbLeader->hasArrowHead())
  {
    idArrow = pDbLeader->dimldrblk();
    arrowType = getArrowheadDwgType(idArrow);
  }
  else
    arrowType = acArrowNone;

  OdDgDimTool::TerminatorType uTerm = OdDgDimTool::kTtNone;

  OdDgElementId idDgTerm;

  bool bArrowIsBlock = getDimArrowBlock(idArrow, arrowType, pDb, uTerm, idDgTerm);

  OdDgnExportDimArrowheadType arrowTypeTerm = arrowType;

  if( bArrowIsBlock )
  {
    if( OdZero(dArrowHeight) )
      dArrowHeight = getDimDefaultArrowHeight(arrowType, dArrowSize);

    OdDgDimOptionTerminatorsPtr pTerminators = OdDgDimOption::createObject(OdDgDimOption::kTerminators);

    pTerminators->setArrowTermStyle(OdDgDimOptionTerminators::kTermCell);
    pTerminators->setArrowCellID(idDgTerm.getHandle());

    pDgDim->setOption(pTerminators);
  }
  else if (OdZero(dArrowHeight))
    dArrowHeight = dArrowSize / 2.0 * dScale;

  OdDgDimension::TerminatorArrowHeadType aType = getTerminatorArrowType(arrowTypeTerm);
  pDgDim->setArrowHead(aType);

  dimGeom.setTerminatorHeight(dArrowHeight);
  dimGeom.setTerminatorWidth(dArrowSize);
  dimGeom.setTextMargin(fabs(pDbLeader->dimgap()));

  pDgDim->setTerminator((OdDgDimNote::NoteTerminatorType)(uTerm));

  pDgDim->setDimGeometry(dimGeom);

  //------------------------------- Text --------------------------------------

  pDgDim->setLeftMargin(fabs(pDbLeader->dimgap()));
  pDgDim->setLowerMargin(fabs(pDbLeader->dimgap()));

  if (pDbLeader->dimgap() < 0)
    pDgDim->setNoteFrameType(OdDgDimNote::kFrameBox);
  else
    pDgDim->setNoteFrameType(OdDgDimNote::kFrameNone);

  OdDgDimTextInfo textInfo;
  pDgDim->getDimTextInfo(textInfo);

  double dTextHeight = pDbLeader->dimtxt();

  if (!OdZero(dTextHeight))
  {
    textInfo.setHeight(dTextHeight);
    textInfo.setWidth(dTextHeight);
  }

  OdDgElementId idDgDimTextStyle = OdDgnExportContext::getElementId(pDbLeader->dimtxsty());

  if (!idDgDimTextStyle.isNull())
  {
    OdDgElementPtr pTextStyleElm = idDgDimTextStyle.openObject(OdDg::kForRead);

    if (!pTextStyleElm.isNull() && pTextStyleElm->isKindOf(OdDgTextStyleTableRecord::desc()))
    {
      OdDgTextStyleTableRecordPtr pTextStyle = pTextStyleElm;
      pDgDim->setTextStyleEntryId(pTextStyle->getEntryId());
      textInfo.setFontEntryId(pTextStyle->getFontEntryId()); // Use font from text style

      if (!OdZero(pTextStyle->getTextHeight()))
      {
        textInfo.setWidth(pTextStyle->getTextWidth() / pTextStyle->getTextHeight() * textInfo.getHeight());

        if (OdZero(dTextHeight))
          textInfo.setHeight(pTextStyle->getTextHeight() / OdDgnExportContext::getMasterToUORsScale());
      }
    }
  }

  pDgDim->setHorizontalFlag(true);

  pDgDim->setNoteHorizontalAttachmentLeaderDirection(OdDgDimNote::kDirectionAuto);

  OdDgDimNote::NoteAttachmentAlignment attachAlign = OdDgDimNote::kAlignMiddle;

  switch (pDbLeader->dimtad())
  {
    case 1:
    case 3:
    case 2:
    case 4: attachAlign = OdDgDimNote::kAlignBottom; break;
    case 5: attachAlign = OdDgDimNote::kAlignTop; break;
  }

  pDgDim->setNoteLeftAttachmentAlignment(attachAlign);
  pDgDim->setNoteRightAttachmentAlignment(attachAlign);
}

//---------------------------------------------------------------------------------------------------

void addNoteDependency(OdDgGraphicsElement* pCell, OdDgDimNote* pDgDim)
{
  OdDgAssocPointIData assocData;

  OdDgOriginAssociationPtr pOriginRoot = OdDgOriginAssociation::createObject();

  pOriginRoot->setElementId((OdUInt64)(pCell->elementId().getHandle()));
  pOriginRoot->setRefAttachmentId(0);
  pOriginRoot->setTextOriginOption(OdDgOriginAssociation::kInsertionPoint);

  assocData.m_assocPointData = pOriginRoot;
  assocData.m_iParam1 = 1;
  assocData.m_iParam2 = 0;

  OdDgDepLinkageAssocPointIPtr pDimDepLinkage = OdDgDepLinkageAssocPointI::createObject();
  pDimDepLinkage->setCopyOptions(OdDgDependencyLinkage::kCopyEx);
  pDimDepLinkage->setAppId(10000);
  pDimDepLinkage->setAppValue(1);
  pDimDepLinkage->add(assocData);

  pDgDim->addLinkage(pDimDepLinkage->getPrimaryId(), pDimDepLinkage.get(), false);

  OdDgDepLinkageElementIdPtr pDepLinkage = OdDgDepLinkageElementId::createObject();
  pDepLinkage->setCopyOptions(OdDgDependencyLinkage::kCopyEx);
  pDepLinkage->setAppId(10000);
  pDepLinkage->setAppValue(1);
  pDepLinkage->add((OdUInt64)(pDgDim->elementId().getHandle()));

  pCell->addLinkage(pDepLinkage->getPrimaryId(), pDepLinkage.get(), false);

  OdDgNoteLinkagePtr pNoteLinkage = OdDgNoteLinkage::createObject();
  pCell->addLinkage(pNoteLinkage->getPrimaryId(), pNoteLinkage.get(), false);
}

//---------------------------------------------------------------------------------------------------

void OdDbLeaderExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbLeader* pDbLeader = (OdDbLeader*)(pDbEntity);

  if (!pDbLeader || !owner)
    return;

  OdDgDimNotePtr pDgDim = OdDgDimNote::createObject();
  pDgDim->setDatabaseDefaults(owner->database());
  copyElementProperties(pDgDim, pDbLeader, owner->database(), false);

  OdGePoint3d ptNewLast;
  OdGePoint3d ptLast;
  OdGePoint3d ptPreLast;

  for (OdInt32 i = 0; i < pDbLeader->numVertices(); i++)
  {
    OdDgDimPoint ptDim;
    ptDim.setPoint(pDbLeader->vertexAt(i));
    pDgDim->addPoint(ptDim);

    if( i == pDbLeader->numVertices() - 2 )
      ptPreLast = pDbLeader->vertexAt(i);
    else if( i == pDbLeader->numVertices() - 1 )
      ptLast = pDbLeader->vertexAt(i);
  }

  ptNewLast = ptLast;

  OdGeVector3d vrNormal = pDbLeader->normal();

  // calculate dimension rotation

  OdGeMatrix3d matRotation = OdGeMatrix3d::planeToWorld(vrNormal);

  OdGeVector3d vrDimYAxis = OdGeVector3d::kYAxis;
  vrDimYAxis.transformBy(matRotation);

  matRotation.setTranslation(OdGeVector3d::kIdentity);

  pDgDim->set3dFlag(true);
  OdGeQuaternion quatRot;
  quatRot.set(matRotation);
  pDgDim->setRotation(quatRot);

  //

  if( pDbLeader->hasHookLine() )
  {
    OdGeLine3d lineLastY(ptPreLast, vrDimYAxis);
    OdGePoint3d ptLastProj = lineLastY.closestPointTo(ptLast);

    OdGeVector3d vrLast = ptLastProj - ptLast;

    if (!vrLast.isZeroLength())
    {
      vrLast.normalize();
      ptNewLast = ptLast + vrLast * fabs(pDbLeader->dimasz())*2.0;
    }

    OdDgDimPoint ptLastDim = pDgDim->getPoint(pDgDim->getPointsCount() - 1);
    ptLastDim.setPoint(ptNewLast);
    pDgDim->setPoint(pDgDim->getPointsCount() - 1, ptLastDim);
  }

  if( (pDbLeader->annoType() != OdDbLeader::kMText) && !ptLast.isEqualTo(ptNewLast) )
  {
    OdDgDimPoint ptDim;
    ptDim.setPoint(ptLast);
    pDgDim->addPoint(ptDim);
  }

  setDimNotePropertiesFromLeader(pDgDim, pDbLeader, owner->database());

  addElementToDatabase(pDgDim, owner);
  appendOdDgElementToOwner(owner, pDgDim);
  OdDgnExportContext::addDgnElement(pDbLeader->objectId(), pDgDim->elementId());

  // Create note attachment

  if( pDbLeader->annoType() == OdDbLeader::kMText )
  {
    OdDbObjectId idMText = pDbLeader->annotationObjId();

    OdDbEntityPtr pDbEnt = idMText.openObject(OdDb::kForRead);

    if( !pDbEnt.isNull() && pDbEnt->isKindOf(OdDbMText::desc()) )
    {
      if( OdDgnExportContext::is3d() )
      {
        OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();
        pCell->setDatabaseDefaults(owner->database());
        copyElementProperties(pCell, pDbLeader, owner->database(), false);
        pCell->setLevelEntryId(0);

        pCell->setTransformation(matRotation);
        pCell->setOrigin(ptNewLast);

        addElementToDatabase(pCell, owner);
        appendOdDgElementToOwner(owner, pCell);

        OdDgGraphicsElementPtr pDimClone = pDgDim->clone();
        pDimClone->setInvisibleFlag(true);
        pCell->add(pDimClone);

        if( !ptLast.isEqualTo(ptNewLast) )
        {
          OdDgLine3dPtr pLine = OdDgLine3d::createObject();
          pLine->setDatabaseDefaults(owner->database());
          copyElementProperties(pLine, pDbLeader, owner->database(), false);
          pLine->setStartPoint(ptLast);
          pLine->setEndPoint(ptNewLast);
          pLine->setColorIndex(pDgDim->getColorIndex());
          pCell->add(pLine);
        }

        OdDgnExportPE* pExportPE = (OdDgnExportPE*)pDbEnt->queryX(OdDgnExportPE::desc());

        if( pExportPE )
          pExportPE->exportElement(pDbEnt, pCell);

        addNoteDependency(pCell, pDgDim);
        OdDgnExportContext::addObjectToSkipExport(pDbEnt->id());
      }
      else
      {
        OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();
        pCell->setDatabaseDefaults(owner->database());
        copyElementProperties(pCell, pDbLeader, owner->database(), false);
        pCell->setLevelEntryId(0);

        pCell->setOrigin(ptNewLast.convert2d());

        addElementToDatabase(pCell, owner);
        appendOdDgElementToOwner(owner, pCell);

        OdDgGraphicsElementPtr pDimClone = pDgDim->clone();
        pDimClone->setInvisibleFlag(true);
        pCell->add(pDimClone);

        if (!ptLast.isEqualTo(ptNewLast))
        {
          OdDgLine2dPtr pLine = OdDgLine2d::createObject();
          pLine->setDatabaseDefaults(owner->database());
          copyElementProperties(pLine, pDbLeader, owner->database(), false);
          pLine->setStartPoint(ptLast.convert2d());
          pLine->setEndPoint(ptNewLast.convert2d());
          pLine->setColorIndex(pDgDim->getColorIndex());
          pCell->add(pLine);
        }

        OdDgnExportPE* pExportPE = (OdDgnExportPE*)pDbEnt->queryX(OdDgnExportPE::desc());

        if (pExportPE)
          pExportPE->exportElement(pDbEnt, pCell);

        addNoteDependency(pCell, pDgDim);
        OdDgnExportContext::addObjectToSkipExport(pDbEnt->id());
      }
    }
  }
}

//---------------------------------------------------------------------------------------------------

bool isDoglegEnabled(OdDbMLeader* pMLeader,
                       bool* pbConnectedAtDogLeg = NULL, // "Horizontal Landing" in ACAD property
                         bool* pbUseDogLegCenterGrip = NULL, // use DOGLEG_CENTER_GRIP
                           bool* pbSkipForLastVertex = NULL)
{
  if (pbConnectedAtDogLeg)
    *pbConnectedAtDogLeg = true;
  if (pbUseDogLegCenterGrip)
    *pbUseDogLegCenterGrip = false; // = true // old way
  if (pbSkipForLastVertex)
    *pbSkipForLastVertex = true; // last vertex will be moved via DOGLEG_END_GRIP (is a duplicate of last grip point on arrow)

  if (!pMLeader)
    return false;

  bool bEnableDogleg = pMLeader->enableDogleg();
  OdDbMLeaderStyle::TextAttachmentDirection tp = pMLeader->textAttachmentDirection();
  if (tp == OdDbMLeaderStyle::kAttachmentVertical || pMLeader->leaderLineType() == OdDbMLeaderStyle::kSplineLeader)
    bEnableDogleg = false;
  if (!bEnableDogleg)
  {
    switch (pMLeader->contentType())
    {
      case OdDbMLeaderStyle::kBlockContent:
        if (!pMLeader->blockContentId().isNull() && pMLeader->blockConnectionType() == OdDbMLeaderStyle::kConnectBase)
        {
          if (pbConnectedAtDogLeg)
            *pbConnectedAtDogLeg = false;        
        }
      break;

      case OdDbMLeaderStyle::kMTextContent:
        switch (tp)
        {
          case OdDbMLeaderStyle::kAttachmentHorizontal:
            if (bEnableDogleg)
          break;

          case OdDbMLeaderStyle::kAttachmentVertical:
            if (pbConnectedAtDogLeg)
              *pbConnectedAtDogLeg = false;
            if (pbSkipForLastVertex)
              *pbSkipForLastVertex = false;
          break;
        }
      break;
    }
  }

  return bEnableDogleg;
}

//---------------------------------------------------------------------------------------------------

bool getConnectionData( OdDbMLeader* pMLeader, 
                          int leaderIndex,
                            bool bEnableDogleg, 
                              bool bConnectedAtDogLeg,  
                                OdGePoint3d& ptConnect, 
                                  OdGeVector3d& vrDoglegDir, // out
                                    double& dDoglegLength, 
                                      double* pdScale = NULL) // out
{
  vrDoglegDir = OdGeVector3d();
  ptConnect = OdGePoint3d();
  dDoglegLength = 0;
  double dScale = getMLeaderScale(pMLeader);
  if (pdScale)
    *pdScale = dScale;
  OdDbMLeaderStyle::TextAttachmentDirection tp = pMLeader->textAttachmentDirection();

  if (!bEnableDogleg && !bConnectedAtDogLeg) // vertical
  {
    ODA_ASSERT_ONCE_X(MLEADER, pMLeader->contentType() != OdDbMLeaderStyle::kMTextContent
      || tp == OdDbMLeaderStyle::kAttachmentVertical || !bEnableDogleg);
    // there is no reason in connection date for such mleader
    return true;
  }

  ODA_ASSERT_ONCE_X(MLEADER, tp != OdDbMLeaderStyle::kAttachmentVertical);
  if (tp == OdDbMLeaderStyle::kAttachmentVertical)
    return true;

  pMLeader->getDoglegDirection(leaderIndex, vrDoglegDir);
  vrDoglegDir = -vrDoglegDir;
  if (pMLeader->connectionPoint(vrDoglegDir, ptConnect) != eOk) // via text location point
    return false;

  OdGePoint3d ptCnt = ptConnect;
  if (pMLeader->connectionPoint(OdGeVector3d::kIdentity, // unused now
    ptCnt, tp) == eOk && ptCnt != ptConnect) // via current value of connection point
  {
    //ODA_FAIL_ONCE_X(MLEADER);
    ptConnect = ptCnt; // way for broken setDogLegLength_acad.dwg created by ARX subtest setDogLegLength
  }

  dDoglegLength = pMLeader->doglegLength(leaderIndex) * dScale;

  if (pMLeader->contentType() != OdDbMLeaderStyle::kMTextContent || !bEnableDogleg)
    return true;

  OdIntArray leaderLineIndexes;
  pMLeader->getLeaderLineIndexes(leaderIndex, leaderLineIndexes);
  if (leaderLineIndexes.isEmpty())
    return false;

  OdGePoint3d ptConnectOpposite;
  if (pMLeader->getLastVertex(leaderLineIndexes[0], ptConnectOpposite) != eOk)
    return true;
  OdGePoint3d ptConnectAlt = ptConnectOpposite - vrDoglegDir * dDoglegLength;
  if (ptConnect == ptConnectAlt)
    return true;

  // this offset (exists in some files) will disappear after recomputing
  OdGeVector3d vOffs = ptConnectAlt - ptConnect;
  double dProjX = vOffs.dotProduct(vrDoglegDir),
    dProjAbsX = fabs(dProjX),
    dProjY = vOffs.dotProduct(vrDoglegDir.crossProduct(OdGeVector3d::kZAxis)),
    dProjAbsY = fabs(dProjY),
    dLen = vOffs.length();
  if ((dProjAbsX * 100.0) < dLen) // if (OdEqual(ptConnect.x, ptConnectAlt.x))
    ptConnect = ptConnectAlt;    //   ptConnect.y = ptConnectAlt.y; //shrt_10917.dwg 
  else if ((dProjAbsY * 25.0) < dLen) // if (OdEqual(ptConnect.y, ptConnectAlt.y))
    ptConnect += vrDoglegDir * dProjX;  //   ptConnect.x = ptConnectAlt.x; // 581902_shrt_acad_res_10738.dwg
  //else
  //{
  //  ODA_FAIL_X(MLEADER); // TODO
  //}
  return true;
}

//---------------------------------------------------------------------------------------------------

bool getMLeaderLeaders(OdDbMLeader* pMLeader, OdArray<OdGePoint3dArray>& arrLeaders)
{
  bool bConnectedAtDogLeg = false;
  OdIntArray leaderIndexes;

  bool bUseDogLegCenterGrip = true;
  bool bSkipForLastVertex = false;
  bool bEnableDogleg = isDoglegEnabled(pMLeader, &bConnectedAtDogLeg, &bUseDogLegCenterGrip, &bSkipForLastVertex);

  pMLeader->getLeaderIndexes(leaderIndexes);


  for (OdUInt32 i = 0, sz = leaderIndexes.size(); i < sz; i++)
  {
    OdGePoint3d  ptConnect;
    OdGeVector3d vrDoglegDir;
    double dDoglegLength = 0;
    bool bRes = getConnectionData(pMLeader, leaderIndexes[i], bEnableDogleg, bConnectedAtDogLeg,
      ptConnect, vrDoglegDir, dDoglegLength); // out

    if (bRes)
    {
      OdGePoint3dArray arrLeaderDogleg;

      if (bEnableDogleg)
      {
        OdGePoint3d tmpPt2(ptConnect + vrDoglegDir * dDoglegLength);
        arrLeaderDogleg.push_back(tmpPt2);
        arrLeaderDogleg.push_back(ptConnect);
      }
      else if (bConnectedAtDogLeg)
      {
        OdGePoint3d tmpPt2(ptConnect + vrDoglegDir * dDoglegLength);
        arrLeaderDogleg.push_back(tmpPt2);
      }

      OdIntArray leaderLineIndexes;
      pMLeader->getLeaderLineIndexes(leaderIndexes[i], leaderLineIndexes);

      for (OdUInt32 k = 0, lsz = leaderLineIndexes.size(); k < lsz; k++)
      {
        OdGePoint3dArray arrLeaderPts;

        int nVertices = 0;

        if (pMLeader->numVertices(leaderLineIndexes[k], nVertices) == eOk)
        {
          for (int j = 0; j < (bSkipForLastVertex ? (nVertices - 1) : nVertices); j++)
          {
            OdGePoint3d ptVertex;
            pMLeader->getVertex(leaderLineIndexes[k], j, ptVertex);
            arrLeaderPts.push_back(ptVertex);
          }
        }

        if (!arrLeaderDogleg.isEmpty())
          arrLeaderPts.append(arrLeaderDogleg);

        if (!arrLeaderPts.isEmpty())
          arrLeaders.push_back(arrLeaderPts);
      }
    }
  }

  return true;
}

//---------------------------------------------------------------------------------------------------

bool isPointOfLeader(const OdGePoint3d& ptCheck, const OdArray<OdGePoint3dArray>& arrLeaders)
{
  bool bRet = false;

  for( OdUInt32 i = 0; i < arrLeaders.size(); i++ )
  {
    if( ptCheck.isEqualTo(arrLeaders[i].first()) || ptCheck.isEqualTo(arrLeaders[i].last()) )
    {
      bRet = true;
      break;
    }
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

void removeLeadersFromContent( const OdArray<OdGePoint3dArray>& arrLeaders, OdRxObjectPtrArray& arrMLeaderContent)
{
  for( OdUInt32 i = 0; i < arrMLeaderContent.size(); i++ )
  {
    bool bRemoveFromContent = false;

    if (arrMLeaderContent[i]->isKindOf(OdDbSolid::desc()))
    {
      OdDbSolidPtr pSolid = arrMLeaderContent[i];

      for( OdUInt32 j = 0; j < 4; j++ )
      {
        OdGePoint3d ptSolid;
        pSolid->getPointAt(j, ptSolid);

        bRemoveFromContent = isPointOfLeader(ptSolid, arrLeaders);

        if( bRemoveFromContent )
          break;
      }
    }
    else if (arrMLeaderContent[i]->isKindOf(OdDb2dPolyline::desc()) || 
             arrMLeaderContent[i]->isKindOf(OdDb3dPolyline::desc()) ||
             arrMLeaderContent[i]->isKindOf(OdDbPolyline::desc()) ||
             arrMLeaderContent[i]->isKindOf(OdDbLine::desc())
            )
    {
      OdDbCurvePtr pPolyline = arrMLeaderContent[i];

      OdGePoint3d ptLineStart, ptLineEnd;
      
      if( pPolyline->getStartPoint(ptLineStart) == eOk && pPolyline->getEndPoint(ptLineEnd) == eOk )
      {
        bRemoveFromContent = isPointOfLeader(ptLineStart, arrLeaders) || isPointOfLeader(ptLineEnd, arrLeaders);
      }
    }
    else if (arrMLeaderContent[i]->isKindOf(OdDbBlockReference::desc()))
    {
      OdDbBlockReferencePtr pBlock = arrMLeaderContent[i];
      bRemoveFromContent = isPointOfLeader(pBlock->position(), arrLeaders);
    }
    else if (arrMLeaderContent[i]->isKindOf(OdDbCircle::desc()))
    {
      OdDbCirclePtr pCircle = arrMLeaderContent[i];
      bRemoveFromContent = isPointOfLeader(pCircle->center(), arrLeaders);
    }

    if( bRemoveFromContent )
    {
      arrMLeaderContent.removeAt(i);
      i--;
    }
  }
}

//---------------------------------------------------------------------------------------------------

void setDimNotePropertiesFromMLeader( OdDgDimNote* pDgDim, 
                                        OdDbMLeader* pDbLeader, 
                                          OdUInt32 uLeaderLine,
                                            OdDgDatabase* pDb)
{
  //------------------------------- Symbology ---------------------------------

  OdUInt32 uElementLineStyle  = getDgnLineStyleEntryId(pDbLeader->linetypeId());
  OdUInt32 uElementLineWeight = odDgnExportLineweightMapping(pDbLeader->lineWeight());
  OdUInt32 uElementColorIndex = getDgnColorIndex(pDbLeader->color(), pDb);

  OdUInt32 uDimLineLineWeight = uElementLineWeight;
  OdUInt32 uDimLineColorIndex = uElementColorIndex;
  OdUInt32 uDimLineLineStyle  = uElementLineStyle;

  OdCmColor         dimColor      = pDbLeader->leaderLineColor();
  OdDb::LineWeight  dimLineWeight = pDbLeader->leaderLineWeight();
  OdDbObjectId      dimLineTypeId = pDbLeader->leaderLineTypeId();

  if (dimColor.colorMethod() != OdCmEntityColor::kByBlock)
    uDimLineColorIndex = getDgnColorIndex(dimColor, pDb);

  if (dimLineWeight != OdDb::kLnWtByBlock)
    uDimLineLineWeight = odDgnExportLineweightMapping(dimLineWeight);

  OdUInt32 uDgLineTypeEntryId = getDgnLineStyleEntryId(dimLineTypeId);

  if (uDgLineTypeEntryId != OdDg::kLineStyleByCell)
    uDimLineLineStyle = uDgLineTypeEntryId;

  // 5. Apply dimension color to dgn element

  if (uElementColorIndex != uDimLineColorIndex)
    pDgDim->setColorIndex(uDimLineColorIndex);

  if (uElementLineWeight != uDimLineLineWeight)
    pDgDim->setLineWeight(uDimLineLineWeight);

  if (uElementLineStyle != uDimLineLineStyle)
    pDgDim->setLineStyleEntryId(uDimLineLineStyle);

  //------------------------------- Leader line -------------------------------

  pDgDim->setSplineFit(pDbLeader->leaderLineType() == OdDbMLeaderStyle::kSplineLeader);
  pDgDim->setLeaderLength(0.0);
  pDgDim->setDimNoteInlineLeaderFlag(false);
  pDgDim->setInvisibleFlag(pDbLeader->leaderLineType() == OdDbMLeaderStyle::kInVisibleLeader);

  double dScale = getMLeaderScale(pDbLeader);

  //------------------------------- Arrow -------------------------------------

  OdDgDimGeometry dimGeom;
  pDgDim->getDimGeometry(dimGeom);

  double        dArrowSize = pDbLeader->arrowSize() * dScale;
  double        dArrowHeight = pDbLeader->arrowSize() / 2.0 * dScale;

  OdDbObjectId  idArrow;
  OdDgnExportDimArrowheadType arrowType;

  idArrow = pDbLeader->arrowSymbolId();
  arrowType = getArrowheadDwgType(idArrow);

  OdDgDimTool::TerminatorType uTerm = OdDgDimTool::kTtNone;

  OdDgElementId idDgTerm;

  bool bArrowIsBlock = getDimArrowBlock(idArrow, arrowType, pDb, uTerm, idDgTerm);

  OdDgnExportDimArrowheadType arrowTypeTerm = arrowType;

  if (bArrowIsBlock)
  {
    if (OdZero(dArrowHeight))
      dArrowHeight = getDimDefaultArrowHeight(arrowType, dArrowSize);

    OdDgDimOptionTerminatorsPtr pTerminators = OdDgDimOption::createObject(OdDgDimOption::kTerminators);

    pTerminators->setArrowTermStyle(OdDgDimOptionTerminators::kTermCell);
    pTerminators->setArrowCellID(idDgTerm.getHandle());

    pDgDim->setOption(pTerminators);
  }
  else if (OdZero(dArrowHeight))
    dArrowHeight = dArrowSize / 2.0 * dScale;

  OdDgDimension::TerminatorArrowHeadType aType = getTerminatorArrowType(arrowTypeTerm);
  pDgDim->setArrowHead(aType);

  dimGeom.setTerminatorHeight(dArrowHeight);
  dimGeom.setTerminatorWidth(dArrowSize);
  dimGeom.setTextMargin(fabs(pDbLeader->landingGap()));

  pDgDim->setTerminator((OdDgDimNote::NoteTerminatorType)(uTerm));

  pDgDim->setDimGeometry(dimGeom);

  //------------------------------- Text --------------------------------------

  pDgDim->setLeftMargin(fabs(pDbLeader->landingGap()));
  pDgDim->setLowerMargin(fabs(pDbLeader->landingGap()));

  if( pDbLeader->enableFrameText() )
    pDgDim->setNoteFrameType(OdDgDimNote::kFrameBox);
  else
    pDgDim->setNoteFrameType(OdDgDimNote::kFrameNone);

  OdDgDimTextInfo textInfo;
  pDgDim->getDimTextInfo(textInfo);

  double dTextHeight = pDbLeader->textHeight();

  if (!OdZero(dTextHeight))
  {
    textInfo.setHeight(dTextHeight);
    textInfo.setWidth(dTextHeight);
  }

  OdDgElementId idDgDimTextStyle = OdDgnExportContext::getElementId(pDbLeader->textStyleId());

  if (!idDgDimTextStyle.isNull())
  {
    OdDgElementPtr pTextStyleElm = idDgDimTextStyle.openObject(OdDg::kForRead);

    if (!pTextStyleElm.isNull() && pTextStyleElm->isKindOf(OdDgTextStyleTableRecord::desc()))
    {
      OdDgTextStyleTableRecordPtr pTextStyle = pTextStyleElm;
      pDgDim->setTextStyleEntryId(pTextStyle->getEntryId());
      textInfo.setFontEntryId(pTextStyle->getFontEntryId()); // Use font from text style

      if (!OdZero(pTextStyle->getTextHeight()))
      {
        textInfo.setWidth(pTextStyle->getTextWidth() / pTextStyle->getTextHeight() * textInfo.getHeight());

        if (OdZero(dTextHeight))
          textInfo.setHeight(pTextStyle->getTextHeight() / OdDgnExportContext::getMasterToUORsScale());
      }
    }
  }

  pDgDim->setHorizontalFlag(true);

  OdDgDimNote::NoteAttachmentAlignment attachAlignLeft  = OdDgDimNote::kAlignMiddle;
  OdDgDimNote::NoteAttachmentAlignment attachAlignRight = OdDgDimNote::kAlignMiddle;

  switch( pDbLeader->textAttachmentType(OdDbMLeaderStyle::kLeftLeader) )
  {
    case OdDbMLeaderStyle::kAttachmentTopOfTop: attachAlignLeft = OdDgDimNote::kAlignTop; break;
    case OdDbMLeaderStyle::kAttachmentMiddleOfTop: attachAlignLeft = OdDgDimNote::kAlignFirstLine; break;
    case OdDbMLeaderStyle::kAttachmentMiddle: attachAlignLeft = OdDgDimNote::kAlignMiddle; break;
    case OdDbMLeaderStyle::kAttachmentMiddleOfBottom: attachAlignLeft = OdDgDimNote::kAlignBottom; break;
    case OdDbMLeaderStyle::kAttachmentBottomOfBottom: attachAlignLeft = OdDgDimNote::kAlignBottom; break;
    case OdDbMLeaderStyle::kAttachmentBottomLine: attachAlignLeft = OdDgDimNote::kAlignLastLine; break;
    case OdDbMLeaderStyle::kAttachmentBottomOfTopLine: attachAlignLeft = OdDgDimNote::kAlignFirstLine; break;
    case OdDbMLeaderStyle::kAttachmentBottomOfTop: attachAlignLeft = OdDgDimNote::kAlignFirstLine; break;
    case OdDbMLeaderStyle::kAttachmentAllLine: attachAlignLeft = OdDgDimNote::kAlignMiddle; break;
    case OdDbMLeaderStyle::kAttachmentCenter: attachAlignLeft = OdDgDimNote::kAlignMiddle; break;
    case OdDbMLeaderStyle::kAttachmentLinedCenter: attachAlignLeft = OdDgDimNote::kAlignMiddle; break;
  }

  switch (pDbLeader->textAttachmentType(OdDbMLeaderStyle::kRightLeader))
  {
    case OdDbMLeaderStyle::kAttachmentTopOfTop: attachAlignRight = OdDgDimNote::kAlignTop; break;
    case OdDbMLeaderStyle::kAttachmentMiddleOfTop: attachAlignRight = OdDgDimNote::kAlignFirstLine; break;
    case OdDbMLeaderStyle::kAttachmentMiddle: attachAlignRight = OdDgDimNote::kAlignMiddle; break;
    case OdDbMLeaderStyle::kAttachmentMiddleOfBottom: attachAlignRight = OdDgDimNote::kAlignBottom; break;
    case OdDbMLeaderStyle::kAttachmentBottomOfBottom: attachAlignRight = OdDgDimNote::kAlignBottom; break;
    case OdDbMLeaderStyle::kAttachmentBottomLine: attachAlignRight = OdDgDimNote::kAlignLastLine; break;
    case OdDbMLeaderStyle::kAttachmentBottomOfTopLine: attachAlignRight = OdDgDimNote::kAlignFirstLine; break;
    case OdDbMLeaderStyle::kAttachmentBottomOfTop: attachAlignRight = OdDgDimNote::kAlignFirstLine; break;
    case OdDbMLeaderStyle::kAttachmentAllLine: attachAlignRight = OdDgDimNote::kAlignMiddle; break;
    case OdDbMLeaderStyle::kAttachmentCenter: attachAlignRight = OdDgDimNote::kAlignMiddle; break;
    case OdDbMLeaderStyle::kAttachmentLinedCenter: attachAlignRight = OdDgDimNote::kAlignMiddle; break;
  }

  pDgDim->setNoteLeftAttachmentAlignment(attachAlignLeft);
  pDgDim->setNoteRightAttachmentAlignment(attachAlignRight);

  pDgDim->setNoteHorizontalAttachmentLeaderDirection(OdDgDimNote::kDirectionAuto);
}

//---------------------------------------------------------------------------------------------------

void OdDbMLeaderExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
{
  OdDbMLeader* pDbMLeader = (OdDbMLeader*)(pDbEntity);

  if (!pDbMLeader || !owner)
    return;

  OdArray<OdGePoint3dArray> arrLeaders;

  getMLeaderLeaders(pDbMLeader, arrLeaders);

  OdRxObjectPtrArray arrMLeaderContent;
  pDbMLeader->explode(arrMLeaderContent);

  removeLeadersFromContent(arrLeaders, arrMLeaderContent);

  for( OdUInt32 i = 0; i < arrLeaders.size(); i++ )
  {
    OdDgDimNotePtr pDgDim = OdDgDimNote::createObject();
    pDgDim->setDatabaseDefaults(owner->database());
    copyElementProperties(pDgDim, pDbMLeader, owner->database(), false);

    OdGeVector3d vrNormal = pDbMLeader->normal();

    // calculate dimension rotation

    OdGeMatrix3d matRotation = OdGeMatrix3d::planeToWorld(vrNormal);
    matRotation.setTranslation(OdGeVector3d::kIdentity);

    pDgDim->set3dFlag(true);
    OdGeQuaternion quatRot;
    quatRot.set(matRotation);
    pDgDim->setRotation(quatRot);

    for (OdUInt32 j = 0; j < arrLeaders[i].size(); j++)
    {
      OdDgDimPoint ptDim;
      ptDim.setPoint(arrLeaders[i][j]);
      pDgDim->addPoint(ptDim);
    }

    setDimNotePropertiesFromMLeader(pDgDim, pDbMLeader, i, owner->database());

    addElementToDatabase(pDgDim, owner);
    appendOdDgElementToOwner(owner, pDgDim);

    // Create note attachment

    if (i == 0)
    {
      OdDgnExportContext::addDgnElement(pDbMLeader->objectId(), pDgDim->elementId());

      if (arrMLeaderContent.isEmpty())
        continue;

      bool bMTextContent = pDbMLeader->contentType() == OdDbMLeaderStyle::kMTextContent;

      if (OdDgnExportContext::is3d())
      {
        OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::createObject();
        pCell->setDatabaseDefaults(owner->database());
        copyElementProperties(pCell, pDbMLeader, owner->database(), false);
        pCell->setLevelEntryId(0);

        pCell->setTransformation(matRotation);
        pCell->setOrigin(arrLeaders[i].last());

        addElementToDatabase(pCell, owner);
        appendOdDgElementToOwner(owner, pCell);

        if( bMTextContent )
        {
          OdDgGraphicsElementPtr pDimClone = pDgDim->clone();
          pDimClone->setInvisibleFlag(true);
          pCell->add(pDimClone);
        }

        for( OdUInt32 k = 0; k < arrMLeaderContent.size(); k++ )
        {
          OdDgnExportPE* pExportPE = (OdDgnExportPE*)arrMLeaderContent[k]->queryX(OdDgnExportPE::desc());

          if (pExportPE)
            pExportPE->exportElement((OdDbEntity*)arrMLeaderContent[k].get(), pCell);
        }

        if( bMTextContent )
          addNoteDependency(pCell, pDgDim);
      }
      else
      {
        OdDgCellHeader2dPtr pCell = OdDgCellHeader2d::createObject();
        pCell->setDatabaseDefaults(owner->database());
        copyElementProperties(pCell, pDbMLeader, owner->database(), false);
        pCell->setLevelEntryId(0);

        pCell->setOrigin(arrLeaders[i].last().convert2d());

        addElementToDatabase(pCell, owner);
        appendOdDgElementToOwner(owner, pCell);

        if (bMTextContent)
        {
          OdDgGraphicsElementPtr pDimClone = pDgDim->clone();
          pDimClone->setInvisibleFlag(true);
          pCell->add(pDimClone);
        }

        for (OdUInt32 k = 0; k < arrMLeaderContent.size(); k++)
        {
          OdDgnExportPE* pExportPE = (OdDgnExportPE*)arrMLeaderContent[k]->queryX(OdDgnExportPE::desc());

          if (pExportPE)
            pExportPE->exportElement((OdDbEntity*)arrMLeaderContent[k].get(), pCell);
        }

        if( bMTextContent )
          addNoteDependency(pCell, pDgDim);
      }
    }
  }
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getDgDimAssocStartEnd(const OdDgElementPtr& pDgElm, const OdGePoint3d& ptDimPt,
  bool bUseIsectPoint, const OdGePoint3d& ptIsect, bool bFirstIsectSegment);

OdDgAssocPointRootPtr getDgDimAssocCenter(const OdDgElementPtr& pDgElm, const OdGePoint3d& ptDimPt);

//---------------------------------------------------------------------------------------------------

bool getDgAssociatedElement(const OdDbFullSubentPath& dbEntPath, OdDgElementId& idDgAssocBase, OdUInt32& uAssocSegment)
{
  bool bRet = false;

  const OdDbObjectIdArray& objectIdsArr = dbEntPath.objectIds();

  if( !objectIdsArr.isEmpty() )
  {
    OdDbObjectId idDbItem = objectIdsArr.first();

    if( !idDbItem.isNull() )
    {
      OdDbObjectPtr pObject = idDbItem.openObject(OdDb::kForRead);

      if( !pObject.isNull() && !pObject->isKindOf(OdDbBlockReference::desc()) )
      {
        idDgAssocBase = OdDgnExportContext::getElementId(idDbItem);
        uAssocSegment = dbEntPath.subentId().index();

        if( !idDgAssocBase.isNull() )
          bRet = true;
      }
    }
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getKeyPointAssocForLine( const OdDgElementId& idLine, 
                                                 const OdGePoint3d& ptStart, 
                                                   const OdGePoint3d& ptEnd, 
                                                     const OdGePoint3d& ptAssoc)
{
  if (ptAssoc.isEqualTo(ptStart))
  {
    OdDgLinearAssociationPtr pLinearAssoc = OdDgLinearAssociation::createObject();
    pLinearAssoc->setRefAttachmentId(0);
    pLinearAssoc->setElementId(idLine.getHandle());
    pLinearAssoc->setVertexNumber(0);
    pLinearAssoc->setNVertices(2);
    pLinearAssoc->setNumerator(0);
    pLinearAssoc->setDivisor(1);

    return pLinearAssoc;
  }
  else if (ptAssoc.isEqualTo(ptEnd))
  {
    OdDgLinearAssociationPtr pLinearAssoc = OdDgLinearAssociation::createObject();
    pLinearAssoc->setRefAttachmentId(0);
    pLinearAssoc->setElementId(idLine.getHandle());
    pLinearAssoc->setVertexNumber(0);
    pLinearAssoc->setNVertices(2);
    pLinearAssoc->setNumerator(1);
    pLinearAssoc->setDivisor(1);

    return pLinearAssoc;
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getKeyPointAssocForLine2d(const OdDgLine2dPtr& pLine, const OdGePoint3d& ptAssoc)
{
  OdGePoint2d ptStart2d = pLine->getStartPoint();
  OdGePoint2d ptEnd2d = pLine->getStartPoint();

  return getKeyPointAssocForLine(pLine->elementId(), OdGePoint3d(ptStart2d.x, ptStart2d.y, 0.0), OdGePoint3d(ptEnd2d.x, ptEnd2d.y, 0.0), ptAssoc);
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getKeyPointAssocForLine3d(const OdDgLine3dPtr& pLine, const OdGePoint3d& ptAssoc)
{
  return getKeyPointAssocForLine(pLine->elementId(), pLine->getStartPoint(), pLine->getEndPoint(), ptAssoc);
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getPointAssocForArc2d(const OdDgArc2dPtr& pArc, 
                                              const OdGePoint3d& ptAssoc, 
                                                bool bCheckParam = false,
                                                  bool bOriginOnly = false)
{
  OdGePoint2d ptAssoc2d = ptAssoc.convert2d();
  OdGePoint2d ptStart, ptEnd, ptOrigin;
  pArc->getStartPoint(ptStart);
  pArc->getEndPoint(ptEnd);
  ptOrigin = pArc->getOrigin();

  if( !bOriginOnly && ptAssoc2d.isEqualTo(ptStart))
  {
    OdDgArcAssociationPtr pArcAssoc = OdDgArcAssociation::createObject();
    pArcAssoc->setRefAttachmentId(0);
    pArcAssoc->setElementId(pArc->elementId().getHandle());
    pArcAssoc->setAngle(0.0);
    pArcAssoc->setKeyPoint(OdDgArcAssociation::kStartPoint);

    return pArcAssoc;
  }
  else if( !bOriginOnly && ptAssoc2d.isEqualTo(ptEnd))
  {
    OdDgArcAssociationPtr pArcAssoc = OdDgArcAssociation::createObject();
    pArcAssoc->setRefAttachmentId(0);
    pArcAssoc->setElementId(pArc->elementId().getHandle());
    pArcAssoc->setAngle(0.0);
    pArcAssoc->setKeyPoint(OdDgArcAssociation::kEndPoint);

    return pArcAssoc;
  }
  else if (ptAssoc2d.isEqualTo(ptOrigin) && !bCheckParam )
  {
    OdDgArcAssociationPtr pArcAssoc = OdDgArcAssociation::createObject();
    pArcAssoc->setRefAttachmentId(0);
    pArcAssoc->setElementId(pArc->elementId().getHandle());
    pArcAssoc->setAngle(0.0);
    pArcAssoc->setKeyPoint(OdDgArcAssociation::kCenterPoint);

    return pArcAssoc;
  }

  double dParam;

  if( !bOriginOnly && (pArc->getParamAtPoint(ptAssoc2d, dParam) == eOk) )
  {
    OdDgArcAssociationPtr pArcAssoc = OdDgArcAssociation::createObject();
    pArcAssoc->setRefAttachmentId(0);
    pArcAssoc->setElementId(pArc->elementId().getHandle());
    pArcAssoc->setAngle(dParam);
    pArcAssoc->setKeyPoint(OdDgArcAssociation::kAnglePoint);

    return pArcAssoc;
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getPointAssocForArc3d(const OdDgArc3dPtr& pArc, 
                                              const OdGePoint3d& ptAssoc, 
                                                bool bCheckParam = false,
                                                  bool bOriginOnly = false)
{
  OdGePoint3d ptStart, ptEnd, ptOrigin;
  pArc->getStartPoint(ptStart);
  pArc->getEndPoint(ptEnd);
  ptOrigin = pArc->getOrigin();

  if( !bOriginOnly && ptAssoc.isEqualTo(ptStart) )
  {
    OdDgArcAssociationPtr pArcAssoc = OdDgArcAssociation::createObject();
    pArcAssoc->setRefAttachmentId(0);
    pArcAssoc->setElementId(pArc->elementId().getHandle());
    pArcAssoc->setAngle(0.0);
    pArcAssoc->setKeyPoint(OdDgArcAssociation::kStartPoint);

    return pArcAssoc;
  }
  else if( !bOriginOnly && ptAssoc.isEqualTo(ptEnd) )
  {
    OdDgArcAssociationPtr pArcAssoc = OdDgArcAssociation::createObject();
    pArcAssoc->setRefAttachmentId(0);
    pArcAssoc->setElementId(pArc->elementId().getHandle());
    pArcAssoc->setAngle(0.0);
    pArcAssoc->setKeyPoint(OdDgArcAssociation::kEndPoint);

    return pArcAssoc;
  }
  else if (ptAssoc.isEqualTo(ptOrigin) && !bCheckParam)
  {
    OdDgArcAssociationPtr pArcAssoc = OdDgArcAssociation::createObject();
    pArcAssoc->setRefAttachmentId(0);
    pArcAssoc->setElementId(pArc->elementId().getHandle());
    pArcAssoc->setAngle(0.0);
    pArcAssoc->setKeyPoint(OdDgArcAssociation::kCenterPoint);

    return pArcAssoc;
  }

  double dParam;

  if( !bOriginOnly && (pArc->getParamAtPoint(ptAssoc, dParam) == eOk) )
  {
    OdDgArcAssociationPtr pArcAssoc = OdDgArcAssociation::createObject();
    pArcAssoc->setRefAttachmentId(0);
    pArcAssoc->setElementId(pArc->elementId().getHandle());
    pArcAssoc->setAngle(dParam);
    pArcAssoc->setKeyPoint(OdDgArcAssociation::kAnglePoint);

    return pArcAssoc;
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getPointAssocForEllipse2d(const OdDgEllipse2dPtr& pEllipse, 
                                                  const OdGePoint3d& ptAssoc, 
                                                    bool bCheckParam = false,
                                                      bool bOriginOnly = false)
{
  OdGePoint2d ptAssoc2d = ptAssoc.convert2d();
  OdGePoint2d ptStart;
  OdGePoint2d ptOrigin = pEllipse->getOrigin();
  double dParam;

  if(!bOriginOnly && (pEllipse->getStartPoint(ptStart) == eOk) && ptAssoc2d.isEqualTo(ptStart) )
  {
    OdDgArcAssociationPtr pArcAssoc = OdDgArcAssociation::createObject();
    pArcAssoc->setRefAttachmentId(0);
    pArcAssoc->setElementId(pEllipse->elementId().getHandle());
    pArcAssoc->setAngle(0.0);
    pArcAssoc->setKeyPoint(OdDgArcAssociation::kStartPoint);

    return pArcAssoc;
  }
  else if( ptAssoc2d.isEqualTo(ptOrigin) && !bCheckParam )
  {
    OdDgArcAssociationPtr pArcAssoc = OdDgArcAssociation::createObject();
    pArcAssoc->setRefAttachmentId(0);
    pArcAssoc->setElementId(pEllipse->elementId().getHandle());
    pArcAssoc->setAngle(0.0);
    pArcAssoc->setKeyPoint(OdDgArcAssociation::kCenterPoint);

    return pArcAssoc;
  }
  else if( !bOriginOnly && (pEllipse->getParamAtPoint(ptAssoc2d, dParam) == eOk) )
  {
    OdDgArcAssociationPtr pArcAssoc = OdDgArcAssociation::createObject();
    pArcAssoc->setRefAttachmentId(0);
    pArcAssoc->setElementId(pEllipse->elementId().getHandle());
    pArcAssoc->setAngle(dParam);
    pArcAssoc->setKeyPoint(OdDgArcAssociation::kAnglePoint);

    return pArcAssoc;
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getPointAssocForEllipse3d(const OdDgEllipse3dPtr& pEllipse, 
                                                  const OdGePoint3d& ptAssoc, 
                                                    bool bCheckParam = false,
                                                      bool bOriginOnly = false)
{
  OdGePoint3d ptStart;
  OdGePoint3d ptOrigin;
  pEllipse->getOrigin(ptOrigin);
  double dParam;

  if(!bOriginOnly && (pEllipse->getStartPoint(ptStart) == eOk) && ptAssoc.isEqualTo(ptStart) )
  {
    OdDgArcAssociationPtr pArcAssoc = OdDgArcAssociation::createObject();
    pArcAssoc->setRefAttachmentId(0);
    pArcAssoc->setElementId(pEllipse->elementId().getHandle());
    pArcAssoc->setAngle(0.0);
    pArcAssoc->setKeyPoint(OdDgArcAssociation::kStartPoint);

    return pArcAssoc;
  }
  else if (ptAssoc.isEqualTo(ptOrigin) && !bCheckParam)
  {
    OdDgArcAssociationPtr pArcAssoc = OdDgArcAssociation::createObject();
    pArcAssoc->setRefAttachmentId(0);
    pArcAssoc->setElementId(pEllipse->elementId().getHandle());
    pArcAssoc->setAngle(0.0);
    pArcAssoc->setKeyPoint(OdDgArcAssociation::kCenterPoint);

    return pArcAssoc;
  }
  else if( !bOriginOnly && (pEllipse->getParamAtPoint(ptAssoc, dParam) == eOk) )
  {
    OdDgArcAssociationPtr pArcAssoc = OdDgArcAssociation::createObject();
    pArcAssoc->setRefAttachmentId(0);
    pArcAssoc->setElementId(pEllipse->elementId().getHandle());
    pArcAssoc->setAngle(dParam);
    pArcAssoc->setKeyPoint(OdDgArcAssociation::kAnglePoint);

    return pArcAssoc;
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getPointAssocForBSplineCurve2d(const OdDgBSplineCurve2dPtr& pCurve, const OdGePoint3d& ptAssoc, bool bCheckParam = false)
{
  OdGePoint2d ptAssoc2d = ptAssoc.convert2d();
  OdGePoint2d ptCurve;
  double dParam = -1;

  if ((pCurve->getStartPoint(ptCurve) == eOk) && ptAssoc2d.isEqualTo(ptCurve))
  {
    if (pCurve->getStartParam(dParam) != eOk)
      dParam = -1;
  }
  else if ((pCurve->getEndPoint(ptCurve) == eOk) && ptAssoc2d.isEqualTo(ptCurve))
  {
    if (pCurve->getEndParam(dParam) != eOk)
      dParam = -1;
  }
  else if (pCurve->getParamAtPoint(ptAssoc2d, dParam) != eOk)
    dParam = -1;
  
  if( !OdNegative(dParam) )
  {
    if( bCheckParam )
    {
      double dStartParam = 0.0;
      double dEndParam = 1.0;
      pCurve->getStartParam(dStartParam);
      pCurve->getEndParam(dEndParam);

      if( dStartParam > dParam || dEndParam < dParam )
        return OdDgAssocPointRootPtr();
    }

    OdDgBSplineCurveAssociationPtr pCurveAssoc = OdDgBSplineCurveAssociation::createObject();
    pCurveAssoc->setRefAttachmentId(0);
    pCurveAssoc->setElementId(pCurve->elementId().getHandle());
    pCurveAssoc->setParam(dParam);

    return pCurveAssoc;
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getPointAssocForBSplineCurve3d(const OdDgBSplineCurve3dPtr& pCurve, const OdGePoint3d& ptAssoc, bool bCheckParam = false)
{
  OdGePoint3d ptCurve;
  double dParam = -1;
  double dStartParam, dEndParam;

  if(pCurve->getStartParam(dStartParam) != eOk)
    return OdDgAssocPointRootPtr();

  if (pCurve->getEndParam(dEndParam) != eOk)
    return OdDgAssocPointRootPtr();

  if ((pCurve->getStartPoint(ptCurve) == eOk) && ptAssoc.isEqualTo(ptCurve))
    dParam = dStartParam;
  else if ((pCurve->getEndPoint(ptCurve) == eOk) && ptAssoc.isEqualTo(ptCurve))
    dParam = dEndParam;
  else if ( (pCurve->getParamAtPoint(ptAssoc, dParam) != eOk) || OdNegative(dParam - dStartParam) || OdPositive( dParam - dEndParam))
    dParam = -1;

  if (!OdNegative(dParam))
  {
    if (bCheckParam)
    {
      double dStartParam = 0.0;
      double dEndParam = 1.0;
      pCurve->getStartParam(dStartParam);
      pCurve->getEndParam(dEndParam);

      if (dStartParam > dParam || dEndParam < dParam)
        return OdDgAssocPointRootPtr();
    }

    OdDgBSplineCurveAssociationPtr pCurveAssoc = OdDgBSplineCurveAssociation::createObject();
    pCurveAssoc->setRefAttachmentId(0);
    pCurveAssoc->setElementId(pCurve->elementId().getHandle());
    pCurveAssoc->setParam(dParam);

    return pCurveAssoc;
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getKeyPointAssocForMultiVertex( const OdDgElementId& idMultiVertex, 
                                                        const OdGePoint3dArray& arrVertices, 
                                                          const OdGePoint3d& ptAssoc,
                                                            bool bUseIsectPoint,
                                                              const OdGePoint3d& ptIsect)
{
  for (OdUInt32 i = 0; i < arrVertices.size(); i++)
  {
    if( arrVertices[i].isEqualTo(ptAssoc) )
    {
      OdDgLinearAssociationPtr pLinearAssoc = OdDgLinearAssociation::createObject();
      pLinearAssoc->setRefAttachmentId(0);
      pLinearAssoc->setElementId(idMultiVertex.getHandle());

      if( i == 0 )
      {
        pLinearAssoc->setVertexNumber(0);
        pLinearAssoc->setNumerator(0);
        pLinearAssoc->setDivisor(1);
      }
      else
      {
        if( i == arrVertices.size() - 1 )
        {
          pLinearAssoc->setVertexNumber(i - 1);
          pLinearAssoc->setNumerator(1);
          pLinearAssoc->setDivisor(1);
        }
        else
        {
          if( bUseIsectPoint )
          {
            OdGeVector3d vrStart = arrVertices[i] - arrVertices[i - 1];
            OdGeVector3d vrEnd = arrVertices[i + 1] - arrVertices[i];

            OdGeLine3d lineStart(arrVertices[i], vrStart);
            OdGeLine3d lineEnd(arrVertices[i], vrEnd);

            if( lineStart.distanceTo(ptIsect) < lineEnd.distanceTo(ptIsect) )
            {
              pLinearAssoc->setVertexNumber(i - 1);
              pLinearAssoc->setNumerator(1);
              pLinearAssoc->setDivisor(1);
            }
            else
            {
              pLinearAssoc->setVertexNumber(i);
              pLinearAssoc->setNumerator(0);
              pLinearAssoc->setDivisor(1);
            }
          }
          else
          {
            pLinearAssoc->setVertexNumber(i);
            pLinearAssoc->setNumerator(0);
            pLinearAssoc->setDivisor(1);
          }
        }
      }

      pLinearAssoc->setNVertices(arrVertices.size());

      return pLinearAssoc;
    }
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

bool getStartEndPointsOfCurveSegment( const OdDgElementPtr& pAssocElm, 
                                        OdUInt32 uSegmentNumber, 
                                          OdGePoint3d& ptStart, 
                                            OdGePoint3d& ptEnd )
{
  bool bRet = false;

  if( pAssocElm->isKindOf(OdDgLine2d::desc()) )
  {
    OdDgLine2dPtr pLine2d = pAssocElm;
    OdGePoint2d ptStart2d, ptEnd2d;
    ptStart2d = pLine2d->getStartPoint();
    ptEnd2d   = pLine2d->getEndPoint();

    ptStart.set(ptStart2d.x, ptStart2d.y, 0.0);
    ptEnd.set(ptEnd2d.x, ptEnd2d.y, 0.0);
    bRet = true;
  }
  else if (pAssocElm->isKindOf(OdDgLine3d::desc()))
  {
    OdDgLine3dPtr pLine3d = pAssocElm;
    ptStart = pLine3d->getStartPoint();
    ptEnd = pLine3d->getEndPoint();
    bRet = true;
  }
  else if (pAssocElm->isKindOf(OdDgMultiVertex2d::desc()))
  {
    OdDgMultiVertex2dPtr pMVertex2d = pAssocElm;
    OdGePoint2d ptStart2d, ptEnd2d;

    if( uSegmentNumber < pMVertex2d->getVerticesCount() - 1)
    {
      ptStart2d = pMVertex2d->getVertexAt(uSegmentNumber);
      ptEnd2d = pMVertex2d->getVertexAt(uSegmentNumber + 1);
    }
    else
    {
      ptStart2d = pMVertex2d->getVertexAt(uSegmentNumber - 1);
      ptEnd2d = pMVertex2d->getVertexAt(uSegmentNumber);
    }

    ptStart.set(ptStart2d.x, ptStart2d.y, 0.0);
    ptEnd.set(ptEnd2d.x, ptEnd2d.y, 0.0);
    bRet = true;
  }
  else if (pAssocElm->isKindOf(OdDgMultiVertex3d::desc()))
  {
    OdDgMultiVertex3dPtr pMVertex3d = pAssocElm;

    if (uSegmentNumber < pMVertex3d->getVerticesCount() - 1)
    {
      ptStart = pMVertex3d->getVertexAt(uSegmentNumber);
      ptEnd = pMVertex3d->getVertexAt(uSegmentNumber + 1);
    }
    else
    {
      ptStart = pMVertex3d->getVertexAt(uSegmentNumber - 1);
      ptEnd = pMVertex3d->getVertexAt(uSegmentNumber);
    }

    bRet = true;
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr choseKeyPointAssoc(const OdArray<OdDgAssocPointRootPtr>& arrAssocCases, 
                                           bool bUseIsectPoint, 
                                             const OdGePoint3d& ptIsect,
                                               bool bFirstIsectSegment,
                                                 OdDgDatabase* pDb )
{
  if (arrAssocCases.size() == 1)
    return arrAssocCases[0];
  else if (arrAssocCases.size() > 1)
  {
    OdDgAssocPointRootPtr pRet;

    if( bUseIsectPoint )
    {
      for( OdUInt32 i = 0; i < arrAssocCases.size(); i++ )
      {
        if( arrAssocCases[i]->getType() != OdDgAssocPointRoot::kLinearAssociation )
        {
          if( pRet.isNull() )
            pRet = arrAssocCases[i];
        }
        else
        {
          OdDgLinearAssociationPtr pLineAssoc = arrAssocCases[i];
          OdDgElementPtr pAssocElm = pDb->getElementId(OdDbHandle(pLineAssoc->getElementId())).openObject(OdDg::kForRead);

          OdGePoint3d ptStart, ptEnd;

          if (!pAssocElm.isNull() && getStartEndPointsOfCurveSegment(pAssocElm, pLineAssoc->getVertexNumber(), ptStart, ptEnd) )
          {
            OdGeVector3d vrLine = ptEnd - ptStart;

            OdGeLine3d lineIsect(ptStart, vrLine);

            if( lineIsect.isOn(ptIsect) )
            {
              pRet = pLineAssoc;

              if( bFirstIsectSegment )
                break;
            }
          }

          if( pRet.isNull() )
            pRet = arrAssocCases[i];
        }
      }
    }
    else
      pRet = arrAssocCases[0];

    return pRet;
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getKeyPointFromArrayOfCurves(const OdDgElementIdArray& arrItems,
                                                     const OdGePoint3d& ptAssoc,
                                                       bool bUseIsectPoint,
                                                         const OdGePoint3d& ptIsect,
                                                           bool bFirstIsectSegment )
{
  OdArray<OdDgAssocPointRootPtr> arrAssocCases;
  OdDgDatabasePtr pDb;

  for( OdUInt32 i = 0; i < arrItems.size(); i++ )
  {
    OdDgElementPtr pElm = arrItems[i].openObject(OdDg::kForRead);
    pDb = pElm->database();

    OdDgAssocPointRootPtr pAssocCase;

    if( !pElm.isNull() )
      pAssocCase = getDgDimAssocStartEnd(pElm, ptAssoc, bUseIsectPoint, ptIsect, bFirstIsectSegment );

    if( !pAssocCase.isNull() )
      arrAssocCases.push_back(pAssocCase);
  }

  return choseKeyPointAssoc(arrAssocCases, bUseIsectPoint, ptIsect, bFirstIsectSegment, pDb );
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getKeyPointAssocForComplexCurve(const OdDgComplexCurvePtr& pCplxCurve,
                                                        const OdGePoint3d& ptAssoc,
                                                          bool bUseIsectPoint,
                                                            const OdGePoint3d& ptIsect,
                                                              bool bFirstIsectSegment)
{
  OdDgElementIteratorPtr pIter = pCplxCurve->createIterator();

  OdDgElementIdArray arrItems;

  for(; !pIter->done(); pIter->step())
    arrItems.push_back(pIter->item());

  return getKeyPointFromArrayOfCurves(arrItems, ptAssoc, bUseIsectPoint, ptIsect, bFirstIsectSegment );
}

//---------------------------------------------------------------------------------------------------

void addItemToElementIdArray(OdDgElementIdArray& arrItems, const OdDgElementId& idElm)
{
  OdDgElementPtr pItem = idElm.openObject(OdDg::kForRead);

  if (pItem.isNull())
    return;

  if (pItem->isKindOf(OdDgComplexCurve::desc()))
  {
    OdDgComplexCurvePtr pCplxCurve = pItem;
    OdDgElementIteratorPtr pIter = pCplxCurve->createIterator();
    OdDgElementIdArray arrItems;

    for (; !pIter->done(); pIter->step())
      addItemToElementIdArray(arrItems, pIter->item());
  }
  else
    arrItems.push_back(idElm);
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getKeyPointAssocForCellHeader2d(const OdDgCellHeader2dPtr& pCellHeader,
                                                        const OdGePoint3d& ptAssoc,
                                                          bool bUseIsectPoint,
                                                            const OdGePoint3d& ptIsect, 
                                                              bool bFirstIsectSegment)
{
  OdDgElementIteratorPtr pIter = pCellHeader->createIterator();

  OdDgElementIdArray arrItems;

  for (; !pIter->done(); pIter->step())
    addItemToElementIdArray(arrItems, pIter->item());

  return getKeyPointFromArrayOfCurves(arrItems, ptAssoc, bUseIsectPoint, ptIsect, bFirstIsectSegment );
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getKeyPointAssocForCellHeader3d(const OdDgCellHeader3dPtr& pCellHeader,
                                                        const OdGePoint3d& ptAssoc,
                                                          bool bUseIsectPoint,
                                                            const OdGePoint3d& ptIsect,
                                                              bool bFirstIsectSegment )
{
  OdDgElementIteratorPtr pIter = pCellHeader->createIterator();

  OdDgElementIdArray arrItems;

  for (; !pIter->done(); pIter->step())
    addItemToElementIdArray(arrItems, pIter->item());

  return getKeyPointFromArrayOfCurves(arrItems, ptAssoc, bUseIsectPoint, ptIsect, bFirstIsectSegment);
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getPointAssocForBSplineSurface(const OdDgBSplineSurfacePtr& pSurface,
                                                       const OdGePoint3d& ptAssoc)
{
  double dUParam, dVParam;

  if( pSurface->getParameterOfPoint(ptAssoc, dUParam, dVParam) == eOk )
  {
    OdDgBSplineSurfaceAssociationPtr pSurfaceAssoc = OdDgBSplineSurfaceAssociation::createObject();
    pSurfaceAssoc->setRefAttachmentId(0);
    pSurfaceAssoc->setElementId(pSurface->elementId().getHandle());
    pSurfaceAssoc->setUParam(dUParam);
    pSurfaceAssoc->setVParam(dVParam);

    return pSurfaceAssoc;
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getKeyPointAssocForMesh(const OdDgMeshFaceLoopsPtr& pMesh,
                                                const OdGePoint3d& ptAssoc)
{
  for( OdUInt32 i = 0; i < pMesh->getPointsNumber(); i++ )
  {
    OdGePoint3d ptMesh = pMesh->getPoint(i);

    if( ptMesh.isEqualTo(ptAssoc) )
    {
      OdDgMeshVertexAssociationPtr pMeshAssoc = OdDgMeshVertexAssociation::createObject();
      pMeshAssoc->setRefAttachmentId(0);
      pMeshAssoc->setElementId(pMesh->elementId().getHandle());
      pMeshAssoc->setVertexIndex(i);
      pMeshAssoc->setNVertices(pMesh->getPointsNumber());
      return pMeshAssoc;
    }
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdGePoint3dArray arrMLinePts(const OdGePoint3dArray& arrBasePts, 
                               const OdGeVector3d& vrNormal, 
                                 double dOffset,
                                   double dStartAngle,
                                     double dEndAngle )
{
  OdGePoint3dArray arrRetPts;

  for (OdUInt32 i = 0; i < arrBasePts.size(); i++)
  {
    OdGePoint3d ptCur;

    if (i == 0)
    {
      OdGeVector3d vrStartEnd = arrBasePts[1] - arrBasePts[0];

      if (!vrStartEnd.isZeroLength())
        vrStartEnd.normalize();

      vrStartEnd.rotateBy(dStartAngle, vrNormal);
      ptCur = arrBasePts[0] + vrStartEnd * dOffset / sin(dStartAngle);
    }
    else if (i == arrBasePts.size() - 1)
    {
      OdGeVector3d vrStartEnd = arrBasePts[arrBasePts.size() - 1] - arrBasePts[arrBasePts.size() - 2];

      if (!vrStartEnd.isZeroLength())
        vrStartEnd.normalize();

      vrStartEnd.rotateBy(dEndAngle, vrNormal);
      ptCur = arrBasePts.last() + vrStartEnd * dOffset / sin(dEndAngle);
    }
    else
    {
      OdGeVector3d vrSeg1 = arrBasePts[i] - arrBasePts[i - 1];
      OdGeVector3d vrOffset1 = vrSeg1;
      OdGeVector3d vrSeg2 = arrBasePts[i + 1] - arrBasePts[i];
      OdGeVector3d vrOffset2 = vrSeg2;

      if (!vrOffset1.isZeroLength() && !vrOffset2.isZeroLength())
      {
        vrOffset1.normalize();
        vrOffset2.normalize();

        OdGeVector3d averagedDirection = (vrOffset1 + vrOffset2) / 2.;

        if (averagedDirection.length() < .001)
          averagedDirection = vrOffset1.crossProduct(vrNormal);

        averagedDirection.normalize();

        OdGeVector3d vrOffset = vrNormal.crossProduct(averagedDirection).normal();
        OdGeVector3d auxiliaryVector = vrNormal.crossProduct(vrOffset1);
        double half = auxiliaryVector.dotProduct(vrOffset);

        if (half < .01)
          half = .01;

        vrOffset *= 1. / half;

        ptCur = arrBasePts[i] + vrOffset * dOffset;
      }
    }

    arrRetPts.push_back(ptCur);
  }

  return arrRetPts;
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getKeyPointAssocForMultiline(const OdDgMultilinePtr& pMultiline,
                                                     const OdGePoint3d& ptAssoc)
{
  OdUInt32 nPoints   = pMultiline->getPointsCount();
  OdUInt32 nProfiles = pMultiline->getProfilesCount();

  OdGePoint3dArray arrBasePts;

  for( OdUInt32 i = 0; i < nPoints; i++ )
  {
    OdDgMultilinePoint mlPoint;
    pMultiline->getPoint(i, mlPoint);
    OdGePoint3d ptMLine;
    mlPoint.getPoint(ptMLine);
    arrBasePts.push_back(ptMLine);
  }

  OdDgMultilineProfile mlProfile;

  for (OdUInt32 j = 0; j < nProfiles; j++)
  {
    OdDgMultilineProfile mlProfile;
    pMultiline->getProfile(j, mlProfile);

    OdGePoint3dArray arrLinePts = arrMLinePts(arrBasePts, pMultiline->getZVector(), mlProfile.getDistance(),
      pMultiline->getOriginCapAngle(), pMultiline->getEndCapAngle() );

    for( OdUInt32 k = 0; k < arrLinePts.size(); k++ )
    {
      if (arrLinePts[k].isEqualTo(ptAssoc))
      {
        OdDgMultilineAssociationPtr pMLineAssoc = OdDgMultilineAssociation::createObject();
        pMLineAssoc->setRefAttachmentId(0);
        pMLineAssoc->setElementId(pMultiline->elementId().getHandle());
        pMLineAssoc->setJointFlag(true);
        pMLineAssoc->setProjectFlag(false);
        pMLineAssoc->setLineNumber(j);
        pMLineAssoc->setVertexNumber(k);
        pMLineAssoc->setNVertices(nPoints);
        pMLineAssoc->setOffset(0.0);
        return pMLineAssoc;
      }
    }
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getKeyPointAssocForMultiVertex2d(const OdDgMultiVertex2dPtr& pMultiVertex, 
                                                         const OdGePoint3d& ptAssoc, 
                                                           bool bUseIsectPoint,
                                                             const OdGePoint3d& ptIsect)
{
  OdGePoint3dArray arrVertices;

  for (OdUInt32 i = 0; i < pMultiVertex->getVerticesCount(); i++)
  {
    OdGePoint2d ptVertex = pMultiVertex->getVertexAt(i);
    arrVertices.push_back( OdGePoint3d(ptVertex.x, ptVertex.y, 0.0));
  }

  return getKeyPointAssocForMultiVertex(pMultiVertex->elementId(), arrVertices, ptAssoc, bUseIsectPoint, ptIsect);
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getKeyPointAssocForMultiVertex3d(const OdDgMultiVertex3dPtr& pMultiVertex, 
                                                         const OdGePoint3d& ptAssoc, 
                                                           bool bUseIsectPoint, 
                                                             const OdGePoint3d& ptIsect)
{
  OdGePoint3dArray arrVertices;

  for (OdUInt32 i = 0; i < pMultiVertex->getVerticesCount(); i++)
    arrVertices.push_back(pMultiVertex->getVertexAt(i));

  return getKeyPointAssocForMultiVertex(pMultiVertex->elementId(), arrVertices, ptAssoc, bUseIsectPoint, ptIsect);
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getDgDimAssocStartEnd(const OdDgElementPtr& pDgElm,
                                              const OdGePoint3d& ptDimPt,
                                                bool bUseIsectPoint,
                                                  const OdGePoint3d& ptIsect,
                                                    bool bFirstIsectSegment )
{
  OdDgAssocPointRootPtr pDgAssoc;

  if (pDgElm->isKindOf(OdDgLine2d::desc()))
    pDgAssoc = getKeyPointAssocForLine2d(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgLine3d::desc()))
    pDgAssoc = getKeyPointAssocForLine3d(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgMultiVertex2d::desc()))
    pDgAssoc = getKeyPointAssocForMultiVertex2d(pDgElm, ptDimPt, bUseIsectPoint, ptIsect);
  else if (pDgElm->isKindOf(OdDgMultiVertex3d::desc()))
    pDgAssoc = getKeyPointAssocForMultiVertex3d(pDgElm, ptDimPt, bUseIsectPoint, ptIsect);
  else if (pDgElm->isKindOf(OdDgArc2d::desc()))
    pDgAssoc = getPointAssocForArc2d(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgArc3d::desc()))
    pDgAssoc = getPointAssocForArc3d(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgEllipse2d::desc()))
    pDgAssoc = getPointAssocForEllipse2d(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgEllipse3d::desc()))
    pDgAssoc = getPointAssocForEllipse3d(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgBSplineCurve2d::desc()))
    pDgAssoc = getPointAssocForBSplineCurve2d(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgBSplineCurve3d::desc()))
    pDgAssoc = getPointAssocForBSplineCurve3d(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgComplexCurve::desc()))
    pDgAssoc = getKeyPointAssocForComplexCurve(pDgElm, ptDimPt, bUseIsectPoint, ptIsect, bFirstIsectSegment);
  else if (pDgElm->isKindOf(OdDgCellHeader2d::desc()))
    pDgAssoc = getKeyPointAssocForCellHeader2d(pDgElm, ptDimPt, bUseIsectPoint, ptIsect, bFirstIsectSegment);
  else if (pDgElm->isKindOf(OdDgCellHeader3d::desc()))
    pDgAssoc = getKeyPointAssocForCellHeader3d(pDgElm, ptDimPt, bUseIsectPoint, ptIsect, bFirstIsectSegment);
  else if (pDgElm->isKindOf(OdDgMultiline::desc()))
    pDgAssoc = getKeyPointAssocForMultiline(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgMeshFaceLoops::desc()))
    pDgAssoc = getKeyPointAssocForMesh(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgBSplineSurface::desc()))
    pDgAssoc = getPointAssocForBSplineSurface(pDgElm, ptDimPt);

  return pDgAssoc;
}

//---------------------------------------------------------------------------------------------------

void setDgDimAssocStartEnd( OdDgDimension* pDgDim,
                              const OdDbOsnapPointRefPtr& pDbAssocPt, 
                                OdUInt32 uDgDimPointIndex,
                                  bool bUseIsectPoint,
                                    OdGePoint3d& ptIsect )
{
  OdDbFullSubentPath dbEntPath;
  pDbAssocPt->getIdPath(dbEntPath);

  OdDgElementId idDgAssocBase;
  OdUInt32 uAssocSegment = 0;

  OdDgAssocPointRootPtr pDgAssoc;

  if( getDgAssociatedElement(dbEntPath, idDgAssocBase, uAssocSegment) )
  {
    OdDgElementPtr pDgElm = idDgAssocBase.openObject(OdDg::kForRead, true);

    if( pDgElm.isNull() )
      return;

    OdDgDimPoint ptDim = pDgDim->getPoint(uDgDimPointIndex);
    OdGePoint3d  ptDimPt = ptDim.getPoint();

    pDgAssoc = getDgDimAssocStartEnd(pDgElm, ptDimPt, bUseIsectPoint, ptIsect, uDgDimPointIndex == 1);

    if( !pDgAssoc.isNull() )
    {
      ptDim.setAssociativeFlag(true);
      ptDim.setDimAssoc(pDgAssoc);
      pDgDim->setPoint(uDgDimPointIndex, ptDim);
    }
  }
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getNearPointAssocForLine(const OdDgElementId& idLine,
                                                 const OdGePoint3d& ptStart,
                                                   const OdGePoint3d& ptEnd,
                                                     const OdGePoint3d& ptAssoc,
                                                       bool bCheckAssoc,
                                                         bool& bKeyPointAssoc )
{
  OdGeLine3d line(ptStart, ptEnd);

  OdGePoint3d ptOnLine = line.closestPointTo(ptAssoc);

  double dSegmentLength = ptStart.distanceTo(ptEnd);

  if( ptOnLine.distanceTo(ptAssoc) < dSegmentLength / 1000.0 )
  {
    if( bCheckAssoc )
    {
      OdGeLineSeg3d segLine(ptStart, ptEnd);

      if( segLine.isOn(ptOnLine) )
        return OdDgAssocPointRootPtr();
    }

    bKeyPointAssoc = false;

    double dParam = 0;
    
    if( !OdZero(dSegmentLength))
      dParam = ptOnLine.distanceTo(ptStart) / dSegmentLength;

    if (OdZero(dParam) || OdEqual(dParam, 1.0))
    {
      bKeyPointAssoc = true;
      return getKeyPointAssocForLine(idLine, ptStart, ptEnd, ptAssoc);
    }
    else if( OdEqual(dParam, 0.5) || OdEqual(dParam, 0.25) || OdEqual(dParam, 0.75) )
    { 
      OdDgLinearAssociationPtr pLinearAssoc = OdDgLinearAssociation::createObject();
      pLinearAssoc->setRefAttachmentId(0);
      pLinearAssoc->setElementId(idLine.getHandle());
      pLinearAssoc->setVertexNumber(0);
      pLinearAssoc->setNVertices(2);

      OdUInt16 uNum, uDiv;

      if( OdEqual(dParam, 0.5) )
      {
        uNum = 1;
        uDiv = 2;
      }
      else if (OdEqual(dParam, 0.25))
      {
        uNum = 1;
        uDiv = 4;
      }
      else if (OdEqual(dParam, 0.75))
      {
        uNum = 3;
        uDiv = 4;
      }

      pLinearAssoc->setNumerator(uNum);
      pLinearAssoc->setDivisor(uDiv);

      return pLinearAssoc;
    }
    else
    {
      OdDgProjectionAssociationPtr pLinearAssoc = OdDgLinearAssociation::createObject();
      pLinearAssoc->setRefAttachmentId(0);
      pLinearAssoc->setElementId(idLine.getHandle());
      pLinearAssoc->setVertexNumber(0);
      pLinearAssoc->setNVertices(2);
      pLinearAssoc->setFraction(dParam);

      return pLinearAssoc;
    }
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getNearPointAssocForLine2d(const OdDgLine2dPtr& pLine, 
                                                   const OdGePoint3d& ptAssoc,
                                                     bool bCheckAssoc,
                                                       bool& bKeyPointAssoc )
{
  OdGePoint2d ptStart2d = pLine->getStartPoint();
  OdGePoint2d ptEnd2d = pLine->getStartPoint();

  return getNearPointAssocForLine(pLine->elementId(), OdGePoint3d(ptStart2d.x, ptStart2d.y, 0.0), 
    OdGePoint3d(ptEnd2d.x, ptEnd2d.y, 0.0), ptAssoc, bCheckAssoc, bKeyPointAssoc);
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getNearPointAssocForLine3d(const OdDgLine3dPtr& pLine, 
                                                   const OdGePoint3d& ptAssoc,
                                                     bool bCheckAssoc,
                                                       bool& bKeyPointAssoc )
{
  return getNearPointAssocForLine(pLine->elementId(), pLine->getStartPoint(), 
    pLine->getEndPoint(), ptAssoc, bCheckAssoc, bKeyPointAssoc );
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getNearPointAssocForMultiVertex(const OdDgElementId& idMultiVertex,
                                                        const OdGePoint3dArray& arrVertices,
                                                          const OdGePoint3d& ptAssoc,
                                                            bool bUseIsectPoint,
                                                              const OdGePoint3d& ptIsect,
                                                                double dDeviation,
                                                                  bool& bKeyPoint)
{
  for (OdUInt32 i = 0; i < arrVertices.size() - 1; i++)
  {
    OdGeLineSeg3d curSeg(arrVertices[i], arrVertices[i + 1]);

    if( curSeg.isOn(ptAssoc, dDeviation) )
    {
      double dParamLength = ptAssoc.distanceTo(arrVertices[i]);
      double dSegmentLength = arrVertices[i].distanceTo(arrVertices[i + 1]);

      double dParam = 0;

      if (!OdZero(dSegmentLength))
        dParam = dParamLength / dSegmentLength;

      bKeyPoint = false;

      if (OdZero(dParam) || OdEqual(dParam, 1.0))
      {
        bKeyPoint = true;
        return getKeyPointAssocForMultiVertex(idMultiVertex, arrVertices, ptAssoc, bUseIsectPoint, ptIsect);
      }
      else if (OdEqual(dParam, 0.5) || OdEqual(dParam, 0.25) || OdEqual(dParam, 0.75))
      {
        OdDgLinearAssociationPtr pLinearAssoc = OdDgLinearAssociation::createObject();
        pLinearAssoc->setRefAttachmentId(0);
        pLinearAssoc->setElementId(idMultiVertex.getHandle());
        pLinearAssoc->setVertexNumber(i);
        pLinearAssoc->setNVertices(arrVertices.size());

        OdUInt16 uNum, uDiv;

        if (OdEqual(dParam, 0.5))
        {
          uNum = 1;
          uDiv = 2;
        }
        else if (OdEqual(dParam, 0.25))
        {
          uNum = 1;
          uDiv = 4;
        }
        else if (OdEqual(dParam, 0.75))
        {
          uNum = 3;
          uDiv = 4;
        }

        pLinearAssoc->setNumerator(uNum);
        pLinearAssoc->setDivisor(uDiv);

        return pLinearAssoc;
      }
      else
      {
        OdDgProjectionAssociationPtr pLinearAssoc = OdDgLinearAssociation::createObject();
        pLinearAssoc->setRefAttachmentId(0);
        pLinearAssoc->setElementId(idMultiVertex.getHandle());
        pLinearAssoc->setVertexNumber(i);
        pLinearAssoc->setNVertices(arrVertices.size());
        pLinearAssoc->setFraction(dParam);

        return pLinearAssoc;
      }

      break;
    }
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getNearPointAssocForMultiVertex2d(const OdDgMultiVertex2dPtr& pMultiVertex,
                                                          const OdGePoint3d& ptAssoc,
                                                            bool bUseIsectPoint,
                                                              const OdGePoint3d& ptIsect,
                                                                double dDeviation,
                                                                  bool& bKeyPoint )
{
  OdGePoint3dArray arrVertices;
  double dFullLength = 0;

  for (OdUInt32 i = 0; i < pMultiVertex->getVerticesCount(); i++)
  {
    OdGePoint2d ptVertex = pMultiVertex->getVertexAt(i);
    arrVertices.push_back(OdGePoint3d(ptVertex.x, ptVertex.y, 0.0));

    if( i > 0 )
      dFullLength += arrVertices[i - 1].distanceTo(arrVertices[i]);
  }

  if( OdZero(dDeviation) )
    dDeviation = dFullLength / 1000.0;

  return getNearPointAssocForMultiVertex(pMultiVertex->elementId(), arrVertices, ptAssoc, 
    bUseIsectPoint, ptIsect, dDeviation, bKeyPoint );
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getNearPointAssocForMultiVertex3d(const OdDgMultiVertex3dPtr& pMultiVertex,
                                                          const OdGePoint3d& ptAssoc,
                                                            bool bUseIsectPoint,
                                                              const OdGePoint3d& ptIsect,
                                                                double dDeviation,
                                                                  bool& bKeyPoint )
{
  OdGePoint3dArray arrVertices;
  double dFullLength = 0;

  for (OdUInt32 i = 0; i < pMultiVertex->getVerticesCount(); i++)
  {
    arrVertices.push_back(pMultiVertex->getVertexAt(i));

    if (i > 0)
      dFullLength += arrVertices[i - 1].distanceTo(arrVertices[i]);
  }

  if (OdZero(dDeviation))
    dDeviation = dFullLength / 1000.0;

  return getNearPointAssocForMultiVertex(pMultiVertex->elementId(), 
    arrVertices, ptAssoc, bUseIsectPoint, ptIsect, dDeviation, bKeyPoint);
}

//---------------------------------------------------------------------------------------------------

bool getNearPointOfCurve( const OdDgElement* pDgElm, 
                            const OdGePoint3d& ptAssoc, 
                              OdDgAssocPointRootPtr& pDgAssoc, 
                                bool& bKeyPoint )
{
  if (pDgElm->isKindOf(OdDgLine2d::desc()))
    pDgAssoc = getNearPointAssocForLine2d(pDgElm, ptAssoc, true, bKeyPoint);
  else if (pDgElm->isKindOf(OdDgLine3d::desc()))
    pDgAssoc = getNearPointAssocForLine3d(pDgElm, ptAssoc, true, bKeyPoint);
  else if (pDgElm->isKindOf(OdDgMultiVertex2d::desc()))
    pDgAssoc = getNearPointAssocForMultiVertex2d(pDgElm, ptAssoc, false, 
      OdGePoint3d::kOrigin, 1e-8, bKeyPoint);
  else if (pDgElm->isKindOf(OdDgMultiVertex3d::desc()))
    pDgAssoc = getNearPointAssocForMultiVertex3d(pDgElm, ptAssoc, false, 
      OdGePoint3d::kOrigin, 1e-8, bKeyPoint);
  else if (pDgElm->isKindOf(OdDgArc2d::desc()))
  {
    pDgAssoc = getPointAssocForArc2d(pDgElm, ptAssoc, true);

    if( !pDgAssoc.isNull() )
    {
      OdDgArcAssociationPtr pArcAssoc = pDgAssoc;
      bKeyPoint = pArcAssoc->getKeyPoint() != OdDgArcAssociation::kAnglePoint;
    }
  }
  else if (pDgElm->isKindOf(OdDgArc3d::desc()))
  {
    pDgAssoc = getPointAssocForArc3d(pDgElm, ptAssoc, true);

    if (!pDgAssoc.isNull())
    {
      OdDgArcAssociationPtr pArcAssoc = pDgAssoc;
      bKeyPoint = pArcAssoc->getKeyPoint() != OdDgArcAssociation::kAnglePoint;
    }
  }
  else if (pDgElm->isKindOf(OdDgEllipse2d::desc()))
  {
    pDgAssoc = getPointAssocForEllipse2d(pDgElm, ptAssoc, true);

    if (!pDgAssoc.isNull())
    {
      OdDgArcAssociationPtr pArcAssoc = pDgAssoc;
      bKeyPoint = pArcAssoc->getKeyPoint() != OdDgArcAssociation::kAnglePoint;
    }
  }
  else if (pDgElm->isKindOf(OdDgEllipse3d::desc()))
  {
    pDgAssoc = getPointAssocForEllipse3d(pDgElm, ptAssoc, true);

    if (!pDgAssoc.isNull())
    {
      OdDgArcAssociationPtr pArcAssoc = pDgAssoc;
      bKeyPoint = pArcAssoc->getKeyPoint() != OdDgArcAssociation::kAnglePoint;
    }
  }
  else if (pDgElm->isKindOf(OdDgBSplineCurve2d::desc()))
  {
    pDgAssoc = getPointAssocForBSplineCurve2d(pDgElm, ptAssoc, true);

    if (!pDgAssoc.isNull())
    {
      OdDgBSplineCurveAssociationPtr pCurveAssoc = pDgAssoc;
      OdDgBSplineCurve2dPtr pCurve2d = pDgElm;
      double dStartParam, dEndParam;
      pCurve2d->getStartParam(dStartParam);
      pCurve2d->getEndParam(dEndParam);

      if (OdEqual(pCurveAssoc->getParam(), dStartParam) || OdEqual(pCurveAssoc->getParam(), dEndParam))
        bKeyPoint = true;
      else
        bKeyPoint = false;
    }
  }
  else if (pDgElm->isKindOf(OdDgBSplineCurve3d::desc()))
  {
    pDgAssoc = getPointAssocForBSplineCurve3d(pDgElm, ptAssoc, true);

    if (!pDgAssoc.isNull())
    {
      OdDgBSplineCurveAssociationPtr pCurveAssoc = pDgAssoc;
      OdDgBSplineCurve2dPtr pCurve2d = pDgElm;
      double dStartParam, dEndParam;
      pCurve2d->getStartParam(dStartParam);
      pCurve2d->getEndParam(dEndParam);

      if (OdEqual(pCurveAssoc->getParam(), dStartParam) || OdEqual(pCurveAssoc->getParam(), dEndParam))
        bKeyPoint = true;
      else
        bKeyPoint = false;
    }
  }

  return pDgAssoc.isNull();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getNearPointFromArrayOfCurves(const OdDgElementIdArray& arrItems,
                                                      const OdGePoint3d& ptAssoc,
                                                        bool bUseIsectPoint,
                                                          const OdGePoint3d& ptIsect,
                                                            bool bFirstIsectSegment)
{
  OdArray<OdDgAssocPointRootPtr> arrAssocCases;
  OdDgDatabasePtr pDb;

  bool bKeyPoint = false;

  for (OdUInt32 i = 0; i < arrItems.size(); i++)
  {
    OdDgElementPtr pElm = arrItems[i].openObject(OdDg::kForRead);
    pDb = pElm->database();

    OdDgAssocPointRootPtr pRet;

    if( getNearPointOfCurve(pElm, ptAssoc, pRet, bKeyPoint) )
    {
      if (bKeyPoint)
        break;
      else
        return pRet;
    }
  }

  if( bKeyPoint )
    return getKeyPointFromArrayOfCurves(arrItems, ptAssoc, bUseIsectPoint, ptIsect, bFirstIsectSegment);

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getNearPointAssocForComplexCurve(const OdDgComplexCurvePtr& pCplxCurve,
                                                         const OdGePoint3d& ptAssoc,
                                                           bool bUseIsectPoint,
                                                             const OdGePoint3d& ptIsect,
                                                               bool bFirstIsectSegment)
{
  OdDgElementIteratorPtr pIter = pCplxCurve->createIterator();

  OdDgElementIdArray arrItems;

  for (; !pIter->done(); pIter->step())
    arrItems.push_back(pIter->item());

  return getNearPointFromArrayOfCurves(arrItems, ptAssoc, bUseIsectPoint, ptIsect, bFirstIsectSegment);
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getNearPointAssocForCellHeader2d(const OdDgCellHeader2dPtr& pCellHeader,
                                                         const OdGePoint3d& ptAssoc,
                                                           bool bUseIsectPoint,
                                                             const OdGePoint3d& ptIsect,
  bool bFirstIsectSegment)
{
  OdDgElementIteratorPtr pIter = pCellHeader->createIterator();

  OdDgElementIdArray arrItems;

  for (; !pIter->done(); pIter->step())
    addItemToElementIdArray(arrItems, pIter->item());

  return getNearPointFromArrayOfCurves(arrItems, ptAssoc, bUseIsectPoint, ptIsect, bFirstIsectSegment);
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getNearPointAssocForCellHeader3d( const OdDgCellHeader3dPtr& pCellHeader,
                                                          const OdGePoint3d& ptAssoc,
                                                            bool bUseIsectPoint,
                                                              const OdGePoint3d& ptIsect,
                                                                bool bFirstIsectSegment)
{
  OdDgElementIteratorPtr pIter = pCellHeader->createIterator();

  OdDgElementIdArray arrItems;

  for (; !pIter->done(); pIter->step())
    addItemToElementIdArray(arrItems, pIter->item());

  return getNearPointFromArrayOfCurves(arrItems, ptAssoc, bUseIsectPoint, ptIsect, bFirstIsectSegment);
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getNearPointAssocForMultiline(const OdDgMultilinePtr& pMultiline,
                                                      const OdGePoint3d& ptAssoc)
{
  OdUInt32 nPoints = pMultiline->getPointsCount();
  OdUInt32 nProfiles = pMultiline->getProfilesCount();

  if (OdEqual(pMultiline->getOriginCapAngle(), OdaPI) || OdEqual(pMultiline->getEndCapAngle(), OdaPI))
    return OdDgAssocPointRootPtr();

  double dMLineLength = 0;

  OdGePoint3dArray arrBasePts;

  for (OdUInt32 i = 0; i < nPoints; i++)
  {
    OdDgMultilinePoint mlPoint;
    pMultiline->getPoint(i, mlPoint);
    OdGePoint3d ptMLine;
    mlPoint.getPoint(ptMLine);
    arrBasePts.push_back(ptMLine);

    if (i > 0)
      dMLineLength += arrBasePts[i - 1].distanceTo(arrBasePts[i]);
  }

  double dDeviation = dMLineLength / 10000.0;

  for (OdUInt32 j = 0; j < nProfiles; j++)
  {
    OdDgMultilineProfile mlProfile;
    pMultiline->getProfile(j, mlProfile);

    OdGePoint3dArray arrLinePts = arrMLinePts(arrBasePts, pMultiline->getZVector(), mlProfile.getDistance(),
      pMultiline->getOriginCapAngle(), pMultiline->getEndCapAngle() );

    for (OdUInt32 k = 0; k < arrLinePts.size()-1; k++)
    {
      OdGeLineSeg3d lineSeg(arrLinePts[k], arrLinePts[k + 1]);

      if( lineSeg.isOn(ptAssoc, dDeviation) )
      {
        OdGeVector3d vrStartEnd = arrLinePts[k + 1] - arrLinePts[k];
        OdGeVector3d vrProj = vrStartEnd;
        vrProj = vrProj.rotateBy(OdaPI2, pMultiline->getZVector());
        OdGeLine3d line(arrLinePts[k], vrStartEnd);
        OdGePointOnCurve3d ptProj;
        line.getProjClosestPointTo(ptAssoc, vrProj, ptProj);
        OdGePoint3d ptBase = ptProj.point3d();

        OdGeVector3d vrProjStart = ptBase - arrLinePts[k];

        double dOffset = 0;

        if (vrStartEnd.isCodirectionalTo(vrProjStart))
          dOffset = vrProjStart.length();
        else
          dOffset = -vrProjStart.length();

        OdDgMultilineAssociationPtr pMLineAssoc = OdDgMultilineAssociation::createObject();
        pMLineAssoc->setRefAttachmentId(0);
        pMLineAssoc->setElementId(pMultiline->elementId().getHandle());
        pMLineAssoc->setJointFlag(false);
        pMLineAssoc->setProjectFlag(true);
        pMLineAssoc->setLineNumber(j);
        pMLineAssoc->setVertexNumber(k);
        pMLineAssoc->setNVertices(nPoints);
        pMLineAssoc->setOffset(dOffset);

        return pMLineAssoc;
      }
    }
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getNearPointAssocForMesh(const OdDgMeshFaceLoopsPtr& pMesh,
                                                 const OdGePoint3d& ptAssoc)
{
  OdUInt32 uEdgeNumber = 0;

  for( OdUInt32 i = 0; i < pMesh->getFacesNumber(); i++ )
  {
    OdDgMeshFaceLoops::VerticesArray faceData;
    pMesh->getFace(i, faceData);

    for( OdUInt32 j = 0; j < faceData.size(); j++ )
    {
      OdGePoint3d ptStart, ptEnd;
      ptStart = pMesh->getPoint(abs(faceData[j].m_pointIndex));

      if( j == faceData.size() - 1 )
        ptEnd = pMesh->getPoint(abs(faceData[0].m_pointIndex));
      else
        ptEnd = pMesh->getPoint(abs(faceData[j+1].m_pointIndex));

      OdGeLineSeg3d lineSeg(ptStart, ptEnd);

      if (lineSeg.isOn(ptAssoc))
      {
        double dSegLength = ptStart.distanceTo(ptEnd);
        double dParamLength = ptStart.distanceTo(ptAssoc);
        double dParam = 0;

        if (OdZero(dSegLength))
          dParam = dParamLength / dSegLength;

        OdDgMeshEdgeAssociationPtr pMeshAssoc = OdDgMeshEdgeAssociation::createObject();
        pMeshAssoc->setRefAttachmentId(0);
        pMeshAssoc->setElementId(pMesh->elementId().getHandle());
        pMeshAssoc->setEdgeIndex(uEdgeNumber);
        pMeshAssoc->setNEdges(0);
        pMeshAssoc->setUParam(dParam);
        return pMeshAssoc;
      }
      else 
        uEdgeNumber++;
    }
    uEdgeNumber++;
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getDgDimAssocNear(const OdDgElementPtr& pDgElm,
                                          const OdGePoint3d& ptDimPt,
                                            bool bUseIsectPoint,
                                              const OdGePoint3d& ptIsect,
                                                bool bFirstIsectSegment)
{
  OdDgAssocPointRootPtr pDgAssoc;

  bool bKeyPointAssoc = false;

  if (pDgElm->isKindOf(OdDgLine2d::desc()))
    pDgAssoc = getNearPointAssocForLine2d(pDgElm, ptDimPt, false, bKeyPointAssoc);
  else if (pDgElm->isKindOf(OdDgLine3d::desc()))
    pDgAssoc = getNearPointAssocForLine3d(pDgElm, ptDimPt, false, bKeyPointAssoc );
  else if (pDgElm->isKindOf(OdDgMultiVertex2d::desc()))
    pDgAssoc = getNearPointAssocForMultiVertex2d(pDgElm, ptDimPt, 
      bUseIsectPoint, ptIsect, 0.0, bKeyPointAssoc);
  else if (pDgElm->isKindOf(OdDgMultiVertex3d::desc()))
    pDgAssoc = getNearPointAssocForMultiVertex3d(pDgElm, ptDimPt, 
      bUseIsectPoint, ptIsect, 0.0, bKeyPointAssoc);
  else if (pDgElm->isKindOf(OdDgArc2d::desc()))
    pDgAssoc = getPointAssocForArc2d(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgArc3d::desc()))
    pDgAssoc = getPointAssocForArc3d(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgEllipse2d::desc()))
    pDgAssoc = getPointAssocForEllipse2d(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgEllipse3d::desc()))
    pDgAssoc = getPointAssocForEllipse3d(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgBSplineCurve2d::desc()))
    pDgAssoc = getPointAssocForBSplineCurve2d(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgBSplineCurve3d::desc()))
    pDgAssoc = getPointAssocForBSplineCurve3d(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgComplexCurve::desc()))
    pDgAssoc = getNearPointAssocForComplexCurve(pDgElm, ptDimPt, bUseIsectPoint, ptIsect, bFirstIsectSegment);
  else if (pDgElm->isKindOf(OdDgCellHeader2d::desc()))
    pDgAssoc = getNearPointAssocForCellHeader2d(pDgElm, ptDimPt, bUseIsectPoint, ptIsect, bFirstIsectSegment);
  else if (pDgElm->isKindOf(OdDgCellHeader3d::desc()))
    pDgAssoc = getNearPointAssocForCellHeader3d(pDgElm, ptDimPt, bUseIsectPoint, ptIsect, bFirstIsectSegment);
  else if (pDgElm->isKindOf(OdDgMultiline::desc()))
    pDgAssoc = getNearPointAssocForMultiline(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgMeshFaceLoops::desc()))
    pDgAssoc = getNearPointAssocForMesh(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgBSplineSurface::desc()))
    pDgAssoc = getPointAssocForBSplineSurface(pDgElm, ptDimPt);

  return pDgAssoc;
}

//---------------------------------------------------------------------------------------------------

void setDgDimAssocNear(OdDgDimension* pDgDim,
                         const OdDbOsnapPointRefPtr& pDbAssocPt,
                           OdUInt32 uDgDimPointIndex,
                             bool bUseIsectPoint,
                               OdGePoint3d& ptIsect )
{
  OdDbFullSubentPath dbEntPath;
  pDbAssocPt->getIdPath(dbEntPath);

  OdDgElementId idDgAssocBase;
  OdUInt32 uAssocSegment = 0;

  OdDgAssocPointRootPtr pDgAssoc;

  if (getDgAssociatedElement(dbEntPath, idDgAssocBase, uAssocSegment))
  {
    OdDgElementPtr pDgElm = idDgAssocBase.openObject(OdDg::kForRead, true);

    if (pDgElm.isNull())
      return;

    OdDgDimPoint ptDim = pDgDim->getPoint(uDgDimPointIndex);
    OdGePoint3d  ptDimPt = ptDim.getPoint();

    pDgAssoc = getDgDimAssocNear(pDgElm, ptDimPt, bUseIsectPoint, ptIsect, uDgDimPointIndex == 1);

    if (!pDgAssoc.isNull())
    {
      ptDim.setAssociativeFlag(true);
      ptDim.setDimAssoc(pDgAssoc);
      pDgDim->setPoint(uDgDimPointIndex, ptDim);
    }
  }
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getCenterPointFromArrayOfCurves(const OdDgElementIdArray& arrItems, const OdGePoint3d& ptAssoc )
{
  OdArray<OdDgAssocPointRootPtr> arrAssocCases;
  OdDgDatabasePtr pDb;

  bool bKeyPoint = false;

  for (OdUInt32 i = 0; i < arrItems.size(); i++)
  {
    OdDgElementPtr pElm = arrItems[i].openObject(OdDg::kForRead);
    pDb = pElm->database();

    OdDgAssocPointRootPtr pRet = getDgDimAssocCenter(pElm, ptAssoc);

    if (!pRet.isNull())
      return pRet;
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getCenterPointAssocForComplexCurve(const OdDgComplexCurvePtr& pCplxCurve, const OdGePoint3d& ptAssoc)
{
  OdDgElementIteratorPtr pIter = pCplxCurve->createIterator();

  OdDgElementIdArray arrItems;

  for (; !pIter->done(); pIter->step())
    arrItems.push_back(pIter->item());

  return getCenterPointFromArrayOfCurves(arrItems, ptAssoc);
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getCenterPointAssocForCellHeader2d(const OdDgCellHeader2dPtr& pCellHeader, const OdGePoint3d& ptAssoc)
{
  OdDgElementIteratorPtr pIter = pCellHeader->createIterator();

  OdDgElementIdArray arrItems;

  for (; !pIter->done(); pIter->step())
    addItemToElementIdArray(arrItems, pIter->item());

  return getCenterPointFromArrayOfCurves(arrItems, ptAssoc);
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getCenterPointAssocForCellHeader3d(const OdDgCellHeader3dPtr& pCellHeader, const OdGePoint3d& ptAssoc)
{
  OdDgElementIteratorPtr pIter = pCellHeader->createIterator();

  OdDgElementIdArray arrItems;

  for (; !pIter->done(); pIter->step())
    addItemToElementIdArray(arrItems, pIter->item());

  return getCenterPointFromArrayOfCurves(arrItems, ptAssoc);
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getDgDimAssocCenter(const OdDgElementPtr& pDgElm,
                                            const OdGePoint3d& ptDimPt )
{
  OdDgAssocPointRootPtr pDgAssoc;

  bool bKeyPointAssoc = false;

  if (pDgElm->isKindOf(OdDgArc2d::desc()))
    pDgAssoc = getPointAssocForArc2d(pDgElm, ptDimPt, false, true);
  else if (pDgElm->isKindOf(OdDgArc3d::desc()))
    pDgAssoc = getPointAssocForArc3d(pDgElm, ptDimPt, false, true);
  else if (pDgElm->isKindOf(OdDgEllipse2d::desc()))
    pDgAssoc = getPointAssocForEllipse2d(pDgElm, ptDimPt, false, true);
  else if (pDgElm->isKindOf(OdDgEllipse3d::desc()))
    pDgAssoc = getPointAssocForEllipse3d(pDgElm, ptDimPt, false, true);
  else if (pDgElm->isKindOf(OdDgComplexCurve::desc()))
    pDgAssoc = getCenterPointAssocForComplexCurve(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgCellHeader2d::desc()))
    pDgAssoc = getCenterPointAssocForCellHeader2d(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgCellHeader3d::desc()))
    pDgAssoc = getCenterPointAssocForCellHeader3d(pDgElm, ptDimPt);

  return pDgAssoc;
}

//---------------------------------------------------------------------------------------------------

void setDgDimAssocCenter(OdDgDimension* pDgDim,
                         const OdDbOsnapPointRefPtr& pDbAssocPt,
                           OdUInt32 uDgDimPointIndex )
{
  OdDbFullSubentPath dbEntPath;
  pDbAssocPt->getIdPath(dbEntPath);

  OdDgElementId idDgAssocBase;
  OdUInt32 uAssocSegment = 0;

  OdDgAssocPointRootPtr pDgAssoc;

  if (getDgAssociatedElement(dbEntPath, idDgAssocBase, uAssocSegment))
  {
    OdDgElementPtr pDgElm = idDgAssocBase.openObject(OdDg::kForRead, true);

    if (pDgElm.isNull())
      return;

    OdDgDimPoint ptDim = pDgDim->getPoint(uDgDimPointIndex);
    OdGePoint3d  ptDimPt = ptDim.getPoint();

    pDgAssoc = getDgDimAssocCenter(pDgElm, ptDimPt);

    if (!pDgAssoc.isNull())
    {
      ptDim.setAssociativeFlag(true);
      ptDim.setDimAssoc(pDgAssoc);
      pDgDim->setPoint(uDgDimPointIndex, ptDim);
    }
  }
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getPointAssocForText(const OdDgElementPtr& pElm, const OdGePoint3d& ptAssoc)
{
  OdGePoint3d ptOrigin;
  OdGePoint3d ptInsertion;

  bool bInitData = false;

  if (pElm->isKindOf(OdDgText2d::desc()))
  {
    OdDgText2dPtr pText2d = pElm;
    OdGePoint2d   ptOrigin2d   = pText2d->getOrigin();
    OdGePoint2d   ptPosition2d = pText2d->getPosition();
    
    ptOrigin.set(ptOrigin2d.x, ptOrigin2d.y, 0.0);
    ptInsertion.set(ptPosition2d.x, ptPosition2d.y, 0.0);
    bInitData = true;
  }
  else  if (pElm->isKindOf(OdDgText3d::desc()))
  {
    OdDgText3dPtr pText3d = pElm;
    ptOrigin    = pText3d->getOrigin();
    ptInsertion = pText3d->getPosition();
    bInitData = true;
  }
  else  if (pElm->isKindOf(OdDgTagElement::desc()))
  {
    OdDgTagElementPtr pTag = pElm;
    ptOrigin = pTag->getOrigin();
    ptInsertion = ptOrigin;
    ptOrigin = ptAssoc + OdGeVector3d::kXAxis;
    bInitData = true;
  }

  if( bInitData )
  {
    if( ptOrigin.isEqualTo(ptAssoc) )
    {
      OdDgOriginAssociationPtr pOriginAssoc = OdDgOriginAssociation::createObject();
      pOriginAssoc->setRefAttachmentId(0);
      pOriginAssoc->setElementId(pElm->elementId().getHandle());
      pOriginAssoc->setTextOriginOption(OdDgOriginAssociation::kUpperLeftPoint);

      return pOriginAssoc;
    }
    else if (ptInsertion.isEqualTo(ptAssoc))
    {
      OdDgOriginAssociationPtr pOriginAssoc = OdDgOriginAssociation::createObject();
      pOriginAssoc->setRefAttachmentId(0);
      pOriginAssoc->setElementId(pElm->elementId().getHandle());
      pOriginAssoc->setTextOriginOption(OdDgOriginAssociation::kInsertionPoint);

      return pOriginAssoc;
    }
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getPointAssocForInsertion(const OdDgElementPtr& pElm, const OdGePoint3d& ptAssoc)
{
  OdGePoint3d ptOrigin;

  bool bInitData = false;

  if (pElm->isKindOf(OdDgTextNode2d::desc()))
  {
    OdDgTextNode2dPtr pText2d = pElm;
    OdGePoint2d   ptOrigin2d = pText2d->getOrigin();
    ptOrigin.set(ptOrigin2d.x, ptOrigin2d.y, 0.0);
    bInitData = true;
  }
  else if (pElm->isKindOf(OdDgTextNode3d::desc()))
  {
    OdDgTextNode3dPtr pText3d = pElm;
    ptOrigin = pText3d->getOrigin();
    bInitData = true;
  }
  else if (pElm->isKindOf(OdDgSharedCellReference::desc()))
  {
    OdDgSharedCellReferencePtr pCell = pElm;
    ptOrigin = pCell->getOrigin();
    bInitData = true;
  }
  else if (pElm->isKindOf(OdDgCellHeader2d::desc()))
  {
    OdDgCellHeader2dPtr pCell = pElm;
    OdGePoint2d   ptOrigin2d = pCell->getOrigin();
    ptOrigin.set(ptOrigin2d.x, ptOrigin2d.y, 0.0);
    bInitData = true;
  }
  else if (pElm->isKindOf(OdDgCellHeader3d::desc()))
  {
    OdDgCellHeader3dPtr pCell = pElm;
    ptOrigin = pCell->getOrigin();
    bInitData = true;
  }

  if( bInitData && ptOrigin.isEqualTo(ptAssoc) )
  {
    OdDgOriginAssociationPtr pOriginAssoc = OdDgOriginAssociation::createObject();
    pOriginAssoc->setRefAttachmentId(0);
    pOriginAssoc->setElementId(pElm->elementId().getHandle());
    pOriginAssoc->setTextOriginOption(OdDgOriginAssociation::kUpperLeftPoint);

    return pOriginAssoc;
  }

  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getDgDimAssocInsertion(const OdDgElementPtr& pDgElm, const OdGePoint3d& ptDimPt)
{
  OdDgAssocPointRootPtr pDgAssoc;

  if (pDgElm->isKindOf(OdDgText2d::desc()))
    pDgAssoc = getPointAssocForText(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgText3d::desc()))
    pDgAssoc = getPointAssocForText(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgTextNode2d::desc()))
    pDgAssoc = getPointAssocForInsertion(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgTextNode3d::desc()))
    pDgAssoc = getPointAssocForInsertion(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgSharedCellReference::desc()))
    pDgAssoc = getPointAssocForInsertion(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgTagElement::desc()))
    pDgAssoc = getPointAssocForText(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgCellHeader2d::desc()))
    pDgAssoc = getPointAssocForInsertion(pDgElm, ptDimPt);
  else if (pDgElm->isKindOf(OdDgCellHeader3d::desc()))
    pDgAssoc = getPointAssocForInsertion(pDgElm, ptDimPt);

  return pDgAssoc;
}

//---------------------------------------------------------------------------------------------------

void setDgDimAssocInsertion(OdDgDimension* pDgDim,
                              const OdDbOsnapPointRefPtr& pDbAssocPt,
                                OdUInt32 uDgDimPointIndex)
{
  OdDbFullSubentPath dbEntPath;
  pDbAssocPt->getIdPath(dbEntPath);

  OdDgElementId idDgAssocBase;
  OdUInt32 uAssocSegment = 0;

  OdDgAssocPointRootPtr pDgAssoc;

  if (getDgAssociatedElement(dbEntPath, idDgAssocBase, uAssocSegment))
  {
    OdDgElementPtr pDgElm = idDgAssocBase.openObject(OdDg::kForRead, true);

    if (pDgElm.isNull())
      return;

    OdDgDimPoint ptDim = pDgDim->getPoint(uDgDimPointIndex);
    OdGePoint3d  ptDimPt = ptDim.getPoint();

    pDgAssoc = getDgDimAssocInsertion(pDgElm, ptDimPt);

    if (!pDgAssoc.isNull())
    {
      ptDim.setAssociativeFlag(true);
      ptDim.setDimAssoc(pDgAssoc);
      pDgDim->setPoint(uDgDimPointIndex, ptDim);
    }
  }
}

//---------------------------------------------------------------------------------------------------

OdUInt32 getNumberOfSegments(const OdDgElement* pElm)
{
  if( pElm->isKindOf(OdDgMultiVertex2d::desc()) )
  {
    OdDgMultiVertex2dPtr pMVertex = pElm;
    return pMVertex->getVerticesCount() - 1;
  }
  else if (pElm->isKindOf(OdDgMultiVertex3d::desc()))
  {
    OdDgMultiVertex3dPtr pMVertex = pElm;
    return pMVertex->getVerticesCount() - 1;
  }

  return 1;
}

//---------------------------------------------------------------------------------------------------

bool getDgnAssocIsectElement(const OdDgElement* pBaseElement, OdDgElementId& idRet, OdUInt32& uSegment)
{
  OdDgElementIteratorPtr pIter;
  if (pBaseElement->isKindOf(OdDgComplexCurve::desc()))
  {
    OdDgComplexCurvePtr pCplxCurve = pBaseElement;
    pIter = pCplxCurve->createIterator();
  }
  else if (pBaseElement->isKindOf(OdDgCellHeader2d::desc()))
  {
    OdDgCellHeader2dPtr pCell = pBaseElement;
    pIter = pCell->createIterator();
  }
  else if (pBaseElement->isKindOf(OdDgCellHeader3d::desc()))
  {
    OdDgCellHeader3dPtr pCell = pBaseElement;
    pIter = pCell->createIterator();
  }

  if( pIter.isNull() )
  {
    idRet = pBaseElement->elementId();
  }
  else
  {
    for(; !pIter->done(); pIter->step())
    {
      OdDgElementPtr pItem = pIter->item().openObject(OdDg::kForRead);

      if( pItem->isKindOf(OdDgComplexCurve::desc()) ||
          pItem->isKindOf(OdDgCellHeader2d::desc()) || 
          pItem->isKindOf(OdDgCellHeader3d::desc())
        )
      {
        bool bRet = getDgnAssocIsectElement(pItem, idRet, uSegment);

        if (bRet)
          return true;
      }

      OdUInt32 nSegments = getNumberOfSegments(pItem);

      if (uSegment >= nSegments)
        uSegment -= nSegments;
      else
      {
        idRet = pBaseElement->elementId();
        return true;
      }
    }

    return false;
  }

  return true;
}

//---------------------------------------------------------------------------------------------------

OdDgGraphicsElementPtr getGrElmSegment(OdDgGraphicsElementPtr pGrElm, OdUInt16 uSegmentIndex)
{
  OdDgGraphicsElementPtr pRet = pGrElm;

  if (pRet->isKindOf(OdDgMultiVertex2d::desc()))
  {
    OdDgMultiVertex2dPtr pCurve = pRet;

    if (pCurve->getVerticesCount() - 1 > uSegmentIndex)
    {
      OdDgLine2dPtr pLine2d = OdDgLine2d::createObject();
      pLine2d->setStartPoint(pCurve->getVertexAt(uSegmentIndex));
      pLine2d->setEndPoint(pCurve->getVertexAt(uSegmentIndex + 1));
      pLine2d->setDatabaseDefaults(pCurve->database());
      pRet = pLine2d;
    }
  }
  else if (pRet->isKindOf(OdDgMultiVertex3d::desc()))
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

//---------------------------------------------------------------------------------------------------

OdInt32 calculateIntersectNumber( const OdDgElementId& idAssoc1,
                                    OdUInt32 uSegment1,
                                      const OdDgElementId& idAssoc2,
                                        OdUInt32 uSegment2,
                                          const OdGePoint3d& ptAssoc
                                )
{
  OdInt32 iRet = -1;

  OdDgGraphicsElementPtr pGrElm1 = idAssoc1.openObject(OdDg::kForRead);
  OdDgGraphicsElementPtr pGrElm2 = idAssoc2.openObject(OdDg::kForRead);
  pGrElm1 = getGrElmSegment(pGrElm1, uSegment1);
  pGrElm2 = getGrElmSegment(pGrElm2, uSegment2);

  ::odrxDynamicLinker()->loadModule(L"ExDgnElementIntersect.tx");

  OdDgElementIntersectionPEPtr pIsectPE = (OdDgElementIntersectionPEPtr)(OdRxObjectPtr(pGrElm1));

  if (!pIsectPE.isNull())
  {
    OdGePoint3dArray arrIsectPts;

    if (pIsectPE->intersectWith(pGrElm1, pGrElm2, OdDg::kExtendBoth, arrIsectPts) == eOk)
    {
      for( OdUInt32 i = 0; i < arrIsectPts.size(); i++ )
      {
        if( arrIsectPts[i].isEqualTo(ptAssoc, 1e-8) )
        {
          iRet = i;
          break;
        }
      }
    }
  }

  return iRet;
}

//---------------------------------------------------------------------------------------------------

bool isLinearElement(const OdDgElementId& idAssoc)
{
  bool bRet = false;

  OdDgElementPtr pElm = idAssoc.openObject(OdDg::kForRead);

  if ((pElm->getElementType() == OdDgElement::kTypeLine) ||
    (pElm->isKindOf(OdDgMultiVertex2d::desc())) ||
    (pElm->isKindOf(OdDgMultiVertex3d::desc()))
    )
  {
    bRet = true;
  }

  return bRet;
}

//---------------------------------------------------------------------------------------------------

OdUInt8 getNSeg(const OdDgElementId& idAssoc)
{
  OdUInt8 uRet = 0;

  OdDgElementPtr pElm = idAssoc.openObject(OdDg::kForRead);

  if( pElm->getElementType() == OdDgElement::kTypeLine )
  {
    uRet = 2;
  }
  else if(pElm->isKindOf(OdDgMultiVertex2d::desc()))
  {
    OdDgMultiVertex2dPtr pMVertex = pElm;
    uRet = pMVertex->getVerticesCount();
  }
  else if (pElm->isKindOf(OdDgMultiVertex3d::desc()))
  {
    OdDgMultiVertex3dPtr pMVertex = pElm;
    uRet = pMVertex->getVerticesCount();
  }

  return uRet;
}

//---------------------------------------------------------------------------------------------------

OdDgAssocPointRootPtr getDgDimAssocIntersect( const OdDgElementPtr& pElm1,
                                                const OdDgElementPtr& pElm2,
                                                  OdUInt32 uSegment1,
                                                    OdUInt32 uSegment2,
                                                      const OdGePoint3d& ptAssoc
                                            )
{
  OdDgElementId idAssoc1, idAssoc2;

  if (getDgnAssocIsectElement(pElm1, idAssoc1, uSegment1) &&
      getDgnAssocIsectElement(pElm2, idAssoc2, uSegment2)
    )
  {
    if( (idAssoc1 == idAssoc2) && (abs((OdInt32)(uSegment1 - uSegment2)) == 1) )
    {
      OdGePoint3d ptKey;
      OdUInt32 nPoints = 0;
      bool bInitKeyPoint = false;

      OdDgElementPtr pAssocElm = idAssoc1.openObject(OdDg::kForRead);

      if( !pAssocElm.isNull() )
      {
        OdUInt32 uPointNum = uSegment1 > uSegment2 ? uSegment1 : uSegment2;

        if( pAssocElm->isKindOf(OdDgMultiVertex2d::desc()) )
        {
          OdDgMultiVertex2dPtr pMVertex = pAssocElm;
          OdGePoint2d ptKey2d = pMVertex->getVertexAt(uPointNum);
          ptKey.set(ptKey2d.x, ptKey2d.y, 0.0);
          nPoints = pMVertex->getVerticesCount();
          bInitKeyPoint = true;
        }
        else if (pAssocElm->isKindOf(OdDgMultiVertex3d::desc()))
        {
          OdDgMultiVertex3dPtr pMVertex = pAssocElm;
          ptKey = pMVertex->getVertexAt(uPointNum);
          nPoints = pMVertex->getVerticesCount();
          bInitKeyPoint = true;
        }

        if( bInitKeyPoint && ptAssoc.isEqualTo(ptKey) )
        {
          OdDgLinearAssociationPtr pLinearAssoc = OdDgLinearAssociation::createObject();
          pLinearAssoc->setRefAttachmentId(0);
          pLinearAssoc->setElementId(idAssoc1.getHandle());
          pLinearAssoc->setVertexNumber(uPointNum);
          pLinearAssoc->setNVertices(nPoints);
          pLinearAssoc->setNumerator(0);
          pLinearAssoc->setDivisor(1);

          return pLinearAssoc;
        }
      }
    }

    int iIsectNumber = calculateIntersectNumber(idAssoc1, uSegment1, idAssoc2, uSegment2, ptAssoc);

    if( iIsectNumber >= 0 )
    {
      bool bLinearElement1 = isLinearElement(idAssoc1);
      bool bLinearElement2 = isLinearElement(idAssoc2);

      if( bLinearElement1 || bLinearElement2 )
      {
        OdDgIntersect2AssociationPtr pIsectAssoc = OdDgIntersect2Association::createObject();
        pIsectAssoc->setRefAttachmentId(0);
        pIsectAssoc->setElementId(idAssoc1.getHandle());
        pIsectAssoc->setRefAttachment2Id(0);
        pIsectAssoc->setElement2Id(idAssoc2.getHandle());
        pIsectAssoc->setIndex((OdUInt8)(iIsectNumber));
        pIsectAssoc->setSegment1(uSegment1);
        pIsectAssoc->setSegment2(uSegment2);
        pIsectAssoc->setNSeg1(getNSeg(idAssoc1));
        pIsectAssoc->setNSeg2(getNSeg(idAssoc2));

        return pIsectAssoc;
      }
      else
      {
        OdDgIntersectAssociationPtr pIsectAssoc = OdDgIntersectAssociation::createObject();
        pIsectAssoc->setRefAttachmentId(0);
        pIsectAssoc->setElementId(idAssoc1.getHandle());
        pIsectAssoc->setRefAttachment2Id(0);
        pIsectAssoc->setElement2Id(idAssoc2.getHandle());
        pIsectAssoc->setIndex((OdUInt8)(iIsectNumber));

        return pIsectAssoc;
      }
    }
  }
  return OdDgAssocPointRootPtr();
}

//---------------------------------------------------------------------------------------------------

void setDgDimAssocIntersection(OdDgDimension* pDgDim,
                                 const OdDbOsnapPointRefPtr& pDbAssocPt,
                                   OdUInt32 uDgDimPointIndex)
{
  OdDbFullSubentPath dbEntPath1;
  OdDbFullSubentPath dbEntPath2;
  pDbAssocPt->getIdPath(dbEntPath1);
  pDbAssocPt->getIntIdPath(dbEntPath2);

  OdDgElementId idDgAssocBase1, idDgAssocBase2;
  OdUInt32 uAssocSegment1 = 0, uAssocSegment2 = 0;

  OdDgAssocPointRootPtr pDgAssoc;

  if( getDgAssociatedElement(dbEntPath1, idDgAssocBase1, uAssocSegment1) &&
      getDgAssociatedElement(dbEntPath2, idDgAssocBase2, uAssocSegment2)
    )
  {
    OdDgElementPtr pDgElm1 = idDgAssocBase1.openObject(OdDg::kForRead, true);
    OdDgElementPtr pDgElm2 = idDgAssocBase2.openObject(OdDg::kForRead, true);

    if( pDgElm1.isNull() || pDgElm2.isNull() )
      return;

    OdDgDimPoint ptDim = pDgDim->getPoint(uDgDimPointIndex);
    OdGePoint3d  ptDimPt = ptDim.getPoint();

    pDgAssoc = getDgDimAssocIntersect(pDgElm1, pDgElm2, uAssocSegment1, uAssocSegment2, ptDimPt);

    if (!pDgAssoc.isNull())
    {
      ptDim.setAssociativeFlag(true);
      ptDim.setDimAssoc(pDgAssoc);
      pDgDim->setPoint(uDgDimPointIndex, ptDim);
    }
  }
}

//---------------------------------------------------------------------------------------------------

void setDgDimAssoc( OdDgDimension* pDgDim, 
                      const OdDbOsnapPointRefPtr& pDbAssocPt, 
                        OdUInt32 uDgDimPointIndex, 
                          bool bRadialDiamAssoc,
                            bool b2LineAssoc )
{
  if( pDbAssocPt.isNull() )
    return;

  OdGePoint3d ptOriginOfLineDim;

  if (b2LineAssoc)
  {
    OdDgDimPoint ptDim = pDgDim->getPoint(0);
    ptOriginOfLineDim = ptDim.getPoint();
  }

  switch( pDbAssocPt->osnapType() )
  {
    case OdDb::kOsModeStart:
    case OdDb::kOsModeEnd:
    {
      setDgDimAssocStartEnd(pDgDim, pDbAssocPt, uDgDimPointIndex, b2LineAssoc, ptOriginOfLineDim );
    } break;

    case OdDb::kOsModeMid:
    case OdDb::kOsModeNear:
    case OdDb::kOsModeQuad:
    case OdDb::kOsModePerp:
    case OdDb::kOsModeTan:
    case OdDb::kOsModePar:
    {
      setDgDimAssocNear(pDgDim, pDbAssocPt, uDgDimPointIndex, b2LineAssoc, ptOriginOfLineDim);
    } break;

    case OdDb::kOsModeCen:
    {
      setDgDimAssocCenter(pDgDim, pDbAssocPt, uDgDimPointIndex);
    } break;

    case OdDb::kOsModeIns:
    {
      setDgDimAssocInsertion(pDgDim, pDbAssocPt, uDgDimPointIndex);
    } break;

    case OdDb::kOsModeIntersec:
    case OdDb::kOsModeApint:
    {
      setDgDimAssocIntersection(pDgDim, pDbAssocPt, uDgDimPointIndex);
    } break;
  }
}

//---------------------------------------------------------------------------------------------------

void restoreDiamRadiusAssoc(OdDgDimension* pDim)
{
  OdDgDimPoint ptDimOrigin = pDim->getPoint(0);
  OdDgDimPoint ptDimChord = pDim->getPoint(1);

  if( !ptDimOrigin.getDimAssoc().isNull() )
  {
    OdDgAssocPointRootPtr pAssoc = ptDimOrigin.getDimAssoc();

    if (pAssoc->getType() == OdDgAssocPointRoot::kArcAssociation)
    {
      OdDgArcAssociationPtr pArcAssoc = pAssoc;
      OdDgElementPtr pDgElm = pDim->database()->getElementId(OdDbHandle(pArcAssoc->getElementId())).openObject(OdDg::kForRead);

      OdDgAssocPointRootPtr pChordAssoc = getDgDimAssocNear(pDgElm, ptDimChord.getPoint(), false, OdGePoint3d::kOrigin, true);

      if( !pChordAssoc.isNull() )
      {
        ptDimChord.setAssociativeFlag(true);
        ptDimChord.setDimAssoc(pChordAssoc);
        pDim->setPoint(1, ptDimChord);
      }
      else
        pDim->breakAssociation();
    }
    else
      pDim->breakAssociation();
  }
}

//---------------------------------------------------------------------------------------------------

void restoreIntersectAssoc(OdDgDimension* pDim)
{
  OdDgDimPoint ptDimOrigin = pDim->getPoint(0);
  OdDgDimPoint ptDimStart = pDim->getPoint(1);
  OdDgDimPoint ptDimEnd = pDim->getPoint(2);

  OdDgAssocPointRootPtr pDgAssoc;

  if( !ptDimStart.getDimAssoc().isNull() && !ptDimEnd.getDimAssoc().isNull())
  {
    OdDgAssocPointRootPtr pAssoc1 = ptDimStart.getDimAssoc();
    OdDgAssocPointRootPtr pAssoc2 = ptDimEnd.getDimAssoc();

    OdDgElementPtr pDgElm1;
    OdDgElementPtr pDgElm2;

    OdUInt32 uAssocSegment1 = 0;
    OdUInt32 uAssocSegment2 = 0;

    if (!pAssoc1.isNull() && !pAssoc2.isNull() &&
      (pAssoc1->getType() != OdDgAssocPointRoot::kIntersectAssociation) &&
      (pAssoc2->getType() != OdDgAssocPointRoot::kIntersect2Association) &&
      (pAssoc1->getType() != OdDgAssocPointRoot::kIntersectAssociation) &&
      (pAssoc2->getType() != OdDgAssocPointRoot::kIntersect2Association)
      )
    {
      pDgElm1 = pDim->database()->getElementId(OdDbHandle(pAssoc1->getElementId())).openObject(OdDg::kForRead);
      pDgElm2 = pDim->database()->getElementId(OdDbHandle(pAssoc2->getElementId())).openObject(OdDg::kForRead);

      if( pAssoc1->getType() == OdDgAssocPointRoot::kLinearAssociation )
      {
        OdDgLinearAssociationPtr pLinAssoc = pAssoc1;
        uAssocSegment1 = pLinAssoc->getVertexNumber();
      }
      else if( pAssoc1->getType() == OdDgAssocPointRoot::kProjectionAssociation )
      {
        OdDgProjectionAssociationPtr pProjAssoc = pAssoc1;
        uAssocSegment1 = pProjAssoc->getVertexNumber();
      }

      if( pAssoc2->getType() == OdDgAssocPointRoot::kLinearAssociation)
      {
        OdDgLinearAssociationPtr pLinAssoc = pAssoc2;
        uAssocSegment2 = pLinAssoc->getVertexNumber();
      }
      else if (pAssoc2->getType() == OdDgAssocPointRoot::kProjectionAssociation)
      {
        OdDgProjectionAssociationPtr pProjAssoc = pAssoc2;
        uAssocSegment2 = pProjAssoc->getVertexNumber();
      }

      if( !pDgElm1.isNull() && !pDgElm2.isNull() )
        pDgAssoc = getDgDimAssocIntersect(pDgElm1, pDgElm2, uAssocSegment1, uAssocSegment2, ptDimOrigin.getPoint());

      if (!pDgAssoc.isNull())
      {
        ptDimOrigin.setAssociativeFlag(true);
        ptDimOrigin.setDimAssoc(pDgAssoc);
        pDim->setPoint(0, ptDimOrigin);
      }
    }
  }
  
  if( pDgAssoc.isNull() )
    pDim->breakAssociation();
}

//---------------------------------------------------------------------------------------------------

void restoreDimensionAssociation(const OdDgnExportDimAssocData& assocData)
{
  if( assocData.m_idDgDimension.isNull() )
    return;

  OdDgDimensionPtr pDgDim = assocData.m_idDgDimension.openObject(OdDg::kForWrite);

  for( OdUInt32 i = 0; i < assocData.m_arrAccocPts.size(); i++ )
  {
    setDgDimAssoc(pDgDim, assocData.m_arrAccocPts[i].m_pPoint, assocData.m_arrAccocPts[i].m_uPointIndex, 
      assocData.m_bDiamAssoc, assocData.m_b2LinesAssoc );
  }

  if( assocData.m_bDiamAssoc )
    restoreDiamRadiusAssoc(pDgDim);
  else if( assocData.m_b2LinesAssoc )
    restoreIntersectAssoc(pDgDim);

  OdDbDimensionPtr pDbDim = assocData.m_idDbDimension.openObject(OdDb::kForRead);

  connectDimensionBlock(pDbDim, pDgDim);
}

//---------------------------------------------------------------------------------------------------

void restoreDimensionAssociations()
{
  for( OdUInt32 i = 0; i < OdDgnExportContext::getDimensionAssocCount(); i++ )
    restoreDimensionAssociation(OdDgnExportContext::getDimensionAssoc(i));

  OdDgnExportContext::clearAllDimensionAssoc();
}

//---------------------------------------------------------------------------------------------------
}
