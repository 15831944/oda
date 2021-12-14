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
#include "DbEntityGripPoints.h"
#include "DbBlockReference.h"
#include "DbAttribute.h"
#include "DbDictionary.h"
#include "Gi/GiClipBoundary.h"
#include "DbSpatialFilter.h"
#include "DbBlockReferenceGripPoints.h"
#include "Gi/GiViewportDraw.h"

ODRX_CONS_DEFINE_MEMBERS(OdDbBlockGripAppData, OdRxObject, RXIMPL_CONSTR);

static OdDbSpatialFilterPtr getBlockReferenceSpatialFilter(OdDbBlockReference *pBR, OdDb::OpenMode openMode)
{
  OdDbDictionaryPtr pDict = OdDbDictionary::cast(pBR->extensionDictionary().openObject());
  if (pDict.isNull())
    return OdDbSpatialFilterPtr();
  OdDbDictionaryPtr pFDict = OdDbDictionary::cast(pDict->getAt(OD_T("ACAD_FILTER"), OdDb::kForRead));
  if (pFDict.isNull())
    return OdDbSpatialFilterPtr();
  return OdDbSpatialFilter::cast(pFDict->getAt(OD_T("SPATIAL"), openMode));
}

void OdDbBlockGripOpStatus(OdDbGripData* pThis, OdDbStub*, OdDbGripOperations::GripStatus status)
{
  if (pThis->appDataOdRxClass() == OdDbBlockGripAppData::desc() && (status == OdDbGripOperations::kGripEnd || status == OdDbGripOperations::kGripAbort))
  {
    ((OdDbBlockGripAppData*)pThis->appData())->release();
    pThis->setAppData(0);
  }
}

static double getGripSize(OdGiViewportDraw* pWd, const OdGePoint3d& eyePt, int gripSize)
{
  OdGePoint2d ptDim;
  OdGePoint3d wcsPt(eyePt); wcsPt.transformBy(pWd->viewport().getEyeToWorldTransform());
  pWd->viewport().getNumPixelsInUnitSquare(wcsPt, ptDim);
  OdGeVector3d v(gripSize / ptDim.x, 0, 0);
  v.transformBy(pWd->viewport().getWorldToEyeTransform());
  return v.length();
}

static void drawFlipArrow(OdGiViewportDraw* pWd, const OdGePoint3d& p, int gripSize, const OdGeVector3d& orient)
{
  OdGeVector3d v(orient);
  v.normalize();
  double dGripSize = getGripSize(pWd, p, gripSize);
  v *= dGripSize;
  OdGeVector3d n(v);
  n.rotateBy(OdaPI2, OdGeVector3d::kZAxis);
  n *= 0.75;
  OdGePoint3d pp[7];
  pp[0] = p + n * 0.5;
  pp[1] = p + n;
  pp[3] = p - n;
  pp[4] = p - n * 0.5;
  pp[2] = p + v;
  pp[5] = p - v - n * 0.5;;
  pp[6] = p - v + n * 0.5;;
  pWd->geometry().polygonEye(7, pp);
}

static void OdDbBlockGripViewportDraw(OdDbGripData* pThis, OdGiViewportDraw* pWd, OdDbStub* entId,
  OdDbGripOperations::DrawType type, OdGePoint3d* imageGripPoint, int gripSize)
{
  ODA_ASSERT(pThis->appDataOdRxClass() == OdDbBlockGripAppData::desc());
  OdGePoint3d p = imageGripPoint == 0 ? pThis->gripPoint() : *imageGripPoint;
  p.transformBy(pWd->viewport().getWorldToEyeTransform());
  switch (type)
  {
  case OdDbGripOperations::kWarmGrip:
    pWd->subEntityTraits().setColor(OdCmEntityColor::kACICyan);
    break;
  case OdDbGripOperations::kHoverGrip:
    pWd->subEntityTraits().setColor(OdCmEntityColor::kACIRed);
    break;
  case OdDbGripOperations::kHotGrip:
    pWd->subEntityTraits().setColor(OdCmEntityColor::kACIMagenta);
    break;
  case OdDbGripOperations::kDragImageGrip:
    pWd->subEntityTraits().setColor(OdCmEntityColor::kACIBlue);
    break;
  }
  pWd->subEntityTraits().setFillType(kOdGiFillAlways);
  OdDbBlockGripAppData* appdata = (OdDbBlockGripAppData*)pThis->appData();
  if (appdata)
    drawFlipArrow(pWd, p, gripSize, appdata->m_vClipInvertOrientation);
}

OdResult OdDbBlockHotGrip(OdDbGripData* pThis, OdDbStub* entId, int status)
{
  if (status & OdDbGripOperations::kSharedGrip)
    return eOk;
  OdDbBlockGripAppData* appdata = (OdDbBlockGripAppData*)pThis->appData();
  if (!appdata->m_bClipInvertGrip)
    return eOk;
  appdata->m_vClipInvertOrientation.negate();
  OdDbBlockReferencePtr pBtr = OdDbObjectId(entId).safeOpenObject(OdDb::kForWrite);
  OdDbSpatialFilterPtr pSpatialFilter = ::getBlockReferenceSpatialFilter(pBtr, OdDb::kForWrite);
  pSpatialFilter->setFilterInverted(!pSpatialFilter->isFilterInverted());
  pBtr->assertWriteEnabled();
  return eOk;
}

OdResult OdDbBlockReferenceGripPointsPE::getGripPoints(
  const OdDbEntity* pEntity,
  OdDbGripDataPtrArray& grips,
  const double curViewUnitSize,
  const int gripSize,
  const OdGeVector3d& curViewDir,
  const int bitFlags) const
{
  OdDbBlockReferencePtr pBtr(pEntity);
  
  { // Origin
    OdDbGripDataPtr originGrip(new OdDbGripData());
    originGrip->setGripPoint(pBtr->position());
    originGrip->setGripOpStatFunc(OdDbBlockGripOpStatus);
    originGrip->setAppDataOdRxClass(OdDbBlockGripAppData::desc());
    OdSmartPtr<OdDbBlockGripAppData> originAppData(OdRxObjectImpl<OdDbBlockGripAppData>::createObject());
    originGrip->setAppData(originAppData.detach());
    grips.append(originGrip);
  }

  { // XCLIP boundary
    OdDbSpatialFilterPtr pSpatialFilter = ::getBlockReferenceSpatialFilter(pBtr, OdDb::kForRead);
    OdGePoint2dArray clipPoints; OdGeMatrix3d xFullXForm;
    if (!pSpatialFilter.isNull() && pSpatialFilter->isEnabled())
    {
      pSpatialFilter->boundary(clipPoints);
      OdGeMatrix3d xClipSpace = pSpatialFilter->getClipSpaceToWCSMatrix(xClipSpace); xClipSpace.invert();
      OdGeMatrix3d xClipInvBlock = pSpatialFilter->getOriginalInverseBlockXform(xClipInvBlock);
      const OdGeMatrix3d xBlockXForm = pBtr->blockTransform();
      xFullXForm = xBlockXForm * xClipInvBlock * xClipSpace;
      for (OdUInt32 nClipPoint = 0; nClipPoint < clipPoints.size(); nClipPoint++)
      {
        OdGePoint3d clipPoint(clipPoints[nClipPoint].x, clipPoints[nClipPoint].y, 0.0);
        clipPoint.transformBy(xFullXForm);
        OdDbGripDataPtr aGrip(new OdDbGripData());
        aGrip->setGripPoint(clipPoint);
        aGrip->setGripOpStatFunc(OdDbBlockGripOpStatus);
        aGrip->setAppDataOdRxClass(OdDbBlockGripAppData::desc());
        OdSmartPtr<OdDbBlockGripAppData> appData(OdRxObjectImpl<OdDbBlockGripAppData>::createObject());
        appData->m_nClipGripGripIndex = nClipPoint;
        aGrip->setAppData(appData.detach());
        grips.append(aGrip);
      }
      // XCLIP inversion
      if (clipPoints.size() > 1)
      {
        OdGePoint3d p1(clipPoints[0].x, clipPoints[0].y, 0.0);
        OdGePoint3d p2(clipPoints[1].x, clipPoints[1].y, 0.0);
        p1.transformBy(xFullXForm);
        p2.transformBy(xFullXForm);
        OdGeVector3d v(p2 - p1);
        OdGePoint3d midPoint = p1 + v / 2;
        OdDbGripDataPtr aGrip(new OdDbGripData());
        aGrip->setGripPoint(midPoint);
        aGrip->setGripOpStatFunc(OdDbBlockGripOpStatus);
        aGrip->setViewportDraw(OdDbBlockGripViewportDraw);
        aGrip->setHotGripFunc(OdDbBlockHotGrip);
        aGrip->setAppDataOdRxClass(OdDbBlockGripAppData::desc());
        OdSmartPtr<OdDbBlockGripAppData> appData(OdRxObjectImpl<OdDbBlockGripAppData>::createObject());
        appData->m_bClipInvertGrip = true;
        OdGeError flag = OdGe::kOk;
        v.normalize(OdGeContext::gTol, flag);
        if (flag == OdGe::kOk)
        {
          v.rotateBy(OdaPI2, curViewDir);
          if (pSpatialFilter->isFilterInverted())
            v.negate();
          appData->m_vClipInvertOrientation = v;
        }
        aGrip->setAppData(appData.detach());
        grips.append(aGrip);
      }
    }
    // Attributes
    int attributeIndex = 0;
    for (OdDbObjectIteratorPtr i = pBtr->attributeIterator(); !i->done(); i->step())
    {
      OdDbAttributePtr attr(i->entity());
      if (attr->lockPositionInBlock())
        continue;
      OdDbGripDataPtr aGrip(new OdDbGripData());
      if (attr->isMTextAttribute())
      {
        aGrip->setGripPoint(attr->getMTextAttribute()->location());
      }
      else
      {
        if (attr->horizontalMode() == OdDb::kTextLeft && attr->verticalMode() == OdDb::kTextBase)
          aGrip->setGripPoint(attr->position());
        else
          aGrip->setGripPoint(attr->alignmentPoint());
      }
      aGrip->setGripOpStatFunc(OdDbBlockGripOpStatus);
      aGrip->setAppDataOdRxClass(OdDbBlockGripAppData::desc());
      OdSmartPtr<OdDbBlockGripAppData> appData(OdRxObjectImpl<OdDbBlockGripAppData>::createObject());
      appData->m_nAttributeIndex = attributeIndex++;
      aGrip->setAppData(appData.detach());
      grips.append(aGrip);
    }
  }
  return eOk;
}

OdResult OdDbBlockReferenceGripPointsPE::moveGripPointsAt(
  OdDbEntity* pEntity,
  const OdDbVoidPtrArray& grips,
  const OdGeVector3d& offset,
  int bitFlags)
{
  OdDbBlockReferencePtr pBtr(pEntity);
  OdGeMatrix3d mTrans(offset);
  // XCLIP boundary
  OdDbSpatialFilterPtr pSpatialFilter = ::getBlockReferenceSpatialFilter(pBtr, OdDb::kForWrite);
  OdGePoint2dArray clipPoints; OdGeVector3d cbOffset; bool bBoundaryChanged = false;
  if (!pSpatialFilter.isNull() && pSpatialFilter->isEnabled())
  {
    pSpatialFilter->boundary(clipPoints);
    OdGeMatrix3d xClipSpace = pSpatialFilter->getClipSpaceToWCSMatrix(xClipSpace); xClipSpace.invert();
    OdGeMatrix3d xClipInvBlock = pSpatialFilter->getOriginalInverseBlockXform(xClipInvBlock);
    const OdGeMatrix3d xBlockXForm = pBtr->blockTransform();
    const OdGeMatrix3d xFullXForm = (xBlockXForm * xClipInvBlock * xClipSpace).invert();
    cbOffset = offset; cbOffset.transformBy(xFullXForm);
  }
  for (unsigned int k = 0; k < grips.size(); ++k)
  {
    if (grips[k] == 0 || ((OdRxObject*)grips[k])->isA() != OdDbBlockGripAppData::desc())
      continue; // not our grip (maybe overruled)
    OdDbBlockGripAppData* pAppData = (OdDbBlockGripAppData*)grips[k];
    if (pAppData->m_nClipGripGripIndex >= 0)
    { // XCLIP boundary
      clipPoints[pAppData->m_nClipGripGripIndex] += OdGeVector2d(cbOffset.x, cbOffset.y);
      bBoundaryChanged = true;
    }
    else if (pAppData->m_nAttributeIndex >= 0)
    {
      int n = 0;
      for (OdDbObjectIteratorPtr i = pBtr->attributeIterator(); !i->done(); i->step())
      {
        OdDbAttributePtr attr(i->entity(OdDb::kForWrite));
        if (attr->lockPositionInBlock())
          continue;
        if (n == pAppData->m_nAttributeIndex)
        {
          // We need to care about annotation contexts and alignment
          attr->setPosition(attr->position() + offset);
          if (attr->horizontalMode() != OdDb::kTextLeft || attr->verticalMode() != OdDb::kTextBase)
            attr->setAlignmentPoint(attr->alignmentPoint() + offset);
          // attr->transformBy(mTrans); // transformBy() method transforms all annotation contexts but only current is needed to change
        }
        ++n;
      }
    }
    else if (!pAppData->m_bClipInvertGrip)
    {
      pBtr->setPosition(pBtr->position() + offset);
      for (OdDbObjectIteratorPtr i = pBtr->attributeIterator(); !i->done(); i->step())
      {
        OdDbAttributePtr attr(i->entity(OdDb::kForWrite));
        // We need to care about annotation contexts and alignment
        attr->setPosition(attr->position() + offset);
        if ( attr->horizontalMode() != OdDb::kTextLeft || attr->verticalMode() != OdDb::kTextBase)
          attr->setAlignmentPoint(attr->alignmentPoint() + offset);
        // attr->transformBy(mTrans); // transformBy() method transforms all annotation contexts but only current is needed to change
      }
    }
  }
  if (bBoundaryChanged)
  {
    OdGiClipBoundary clipBnd; bool bEnabled, bInverted;
    pSpatialFilter->getDefinition(clipBnd, bEnabled);
    bInverted = pSpatialFilter->isFilterInverted();
    clipBnd.m_Points = clipPoints;
    pSpatialFilter->setDefinition(clipBnd, bEnabled);
    if (bInverted)
      pSpatialFilter->setFilterInverted(bInverted);
    pBtr->assertWriteEnabled(); // Force modification in case if boundary was changed
  }
  return eOk;
}

OdResult OdDbBlockReferenceGripPointsPE::getStretchPoints(const OdDbEntity* pEnt, OdGePoint3dArray& stretchPoints) const
{
  OdDbBlockReferencePtr pBtr(pEnt);
  stretchPoints.append(pBtr->position()); // basic block reference has only one stretch point
  return eOk;
}

OdResult OdDbBlockReferenceGripPointsPE::moveStretchPointsAt(OdDbEntity* pEnt, const OdIntArray& indices, const OdGeVector3d& offset)
{
  OdDbBlockReferencePtr pBtr(pEnt);
  pBtr->transformBy(OdGeMatrix3d::translation(offset));
  return eOk;
}

OdResult OdDbBlockReferenceGripPointsPE::getOsnapPoints(const OdDbEntity* pEnt,
  OdDb::OsnapMode osnapMode,
  OdGsMarker gsSelectionMark,
  const OdGePoint3d& pickPoint,
  const OdGePoint3d& lastPoint,
  const OdGeMatrix3d& xWorldToEye,
  OdGePoint3dArray& snapPoints) const
{
  // Should return:
  // Osnap points for block contents transformed by own transformation.
  // Plus for INS - own position and INS should be called for attributes
  //
  // Note gsSelectionMark also needs to be taken care of
  return eNotImplemented;
}
