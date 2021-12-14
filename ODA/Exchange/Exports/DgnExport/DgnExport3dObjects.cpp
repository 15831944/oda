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
#include "DgnExport3dObjects.h"
#include "DgnExportCurve.h"
#include "DgnExportCommon.h"
#include "DgnExportContext.h"
#include <DbPolyFaceMesh.h>
#include <DgCellHeader.h>
#include <DgComplexShape.h>
#include <DgComplexString.h>
#include <DbFaceRecord.h>
#include <Db3dSolid.h>
#include <DbSurface.h>
#include <DbBody.h>
#include <DbNurbSurface.h>
#include <DgBRepEntityPE.h>
#include <DbLayerTableRecord.h>
#include <DgBSplineSurface.h>
#include <MemoryStream.h>
#include <Br/BrBrep.h>
#include <Br/BrBrepFaceTraverser.h>
#include <Br/BrFaceLoopTraverser.h>
#include <Br/BrBrepEdgeTraverser.h>
#include <Br/BrLoopEdgeTraverser.h>

namespace TD_DGN_EXPORT {

  //===================================================================================================

  void setGraphicsElementProperties(OdDgCellHeader3d* pCell3d, OdDbEntity* pDbBaseEntity)
  {
    OdDgElementIteratorPtr pIter = pCell3d->createIterator();

    if (!pIter.isNull())
    {
      for (; !pIter->done(); pIter->step())
      {
        OdDgElementPtr pElm = pIter->item().openObject(OdDg::kForWrite);

        if (pElm->isKindOf(OdDgGraphicsElement::desc()))
        {
          OdDgGraphicsElementPtr pGrElm = pElm;
          copyElementProperties(pGrElm, pDbBaseEntity, pCell3d->database());
        }
      }
    }
  }

  //===================================================================================================

  bool export3dElement(OdDbEntity* pDbEntity, OdDgElement* owner, OdDbEntity* pDbBaseEntity)
  {
    bool bRet = false;

    OdStreamBufPtr pStreamBuf = OdMemoryStream::createNew();
    bool bCreateSurface = false;

    if( pDbEntity->isKindOf(OdDb3dSolid::desc()) )
    {
      OdDb3dSolid* pDbSolid = (OdDb3dSolid*)(pDbEntity);

      if (pDbSolid->acisOut(pStreamBuf, kAf_Binary_700) == eOk)
        bRet = true;
    }
    else if (pDbEntity->isKindOf(OdDbSurface::desc()))
    {
      OdDbSurface* pDbSurface = (OdDbSurface*)(pDbEntity);
      bCreateSurface = true;

      if (pDbSurface->acisOut(pStreamBuf, kAf_Binary_700) == eOk)
        bRet = true;
    }
    else if (pDbEntity->isKindOf(OdDbBody::desc()))
    {
      OdDbBody* pDbBody = (OdDbBody*)(pDbEntity);
      bCreateSurface = true;

      if (pDbBody->acisOut(pStreamBuf, kAf_Binary_700) == eOk)
        bRet = true;
    }

    if( bRet )
    {
      pStreamBuf->rewind();

      OdDgCellHeader3dPtr pCellHeader = OdDgCellHeader3d::createObject();
      pCellHeader->setDatabaseDefaults(owner->database());
      OdDgBRepEntityPEPtr pBrepPE = pCellHeader;

      if (!pBrepPE.isNull() && !pCellHeader.isNull() )
      {
        addElementToDatabase(pCellHeader, owner);
        copyElementProperties(pCellHeader, pDbBaseEntity, owner->database());
        appendOdDgElementToOwner(owner, pCellHeader);

        double dBrepScale = 1.0;

        OdDgElementId idModel = getOwnerModelId(pCellHeader);

        if (!idModel.isNull())
        {
          OdDgModelPtr pModel = idModel.openObject(OdDg::kForRead);
          OdDgModel::StorageUnitDescription storageUnits;
          pModel->getStorageUnit(storageUnits);
          dBrepScale = OdDgnExportContext::getMasterToUORsScale() / storageUnits.m_uorPerStorageUnit;
        }
        else
        {
          idModel = pCellHeader->database()->getDefaultModelId();

          if( !idModel.isNull() )
          {
            OdDgModelPtr pModel = idModel.openObject(OdDg::kForRead);
            OdDgModel::StorageUnitDescription storageUnits;
            pModel->getStorageUnit(storageUnits);
            dBrepScale = 1.0 / storageUnits.m_uorPerStorageUnit;
          }
          else
            dBrepScale = 0.0001;
        }

        OdDgnExportContext::addDgnElement(pDbBaseEntity->objectId(), pCellHeader->elementId());
        pBrepPE->fillSmartSolid(*pCellHeader.get(), *pStreamBuf.get(), true, dBrepScale, bCreateSurface);
        setGraphicsElementProperties(pCellHeader, pDbBaseEntity);
      }
      else
        bRet = false;
    }

    return bRet;
  }

  //===================================================================================================

  void OdDb3dSolidExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
  {
    if (!export3dElement(pDbEntity, owner, pDbEntity))
      OdDgnExportPE::subExportElement(pDbEntity, owner);
  }

  //===================================================================================================

  void OdDb3dSurfaceExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
  {
    if( !export3dElement(pDbEntity, owner, pDbEntity) )
      OdDgnExportPE::subExportElement(pDbEntity, owner);
  }

  //===================================================================================================

  void OdDbBodyExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
  {
    if (!export3dElement(pDbEntity, owner, pDbEntity))
      OdDgnExportPE::subExportElement(pDbEntity, owner);
  }

  //===================================================================================================

  bool connectLoopPts(OdGePoint2dArray& arrLoopPts, OdGePoint2dArray& arrCurvePts, bool bAllowToReverseLoopPts)
  {
    OdGePoint2d ptLoopStart  = arrLoopPts.first();
    OdGePoint2d ptLoopEnd    = arrLoopPts.last();
    OdGePoint2d ptCurveStart = arrCurvePts.first();
    OdGePoint2d ptCurveEnd   = arrCurvePts.last();

    bool bRet = false;

    if( bAllowToReverseLoopPts )
    {
      if (ptCurveStart.isEqualTo(ptLoopEnd))
      {
        arrLoopPts.removeLast();
        arrLoopPts.append(arrCurvePts);
        bRet = true;
      }
      else if (ptCurveEnd.isEqualTo(ptLoopEnd))
      {
        arrCurvePts.reverse();
        arrLoopPts.removeLast();
        arrLoopPts.append(arrCurvePts);
        bRet = true;
      }
      else if (ptCurveStart.isEqualTo(ptLoopStart))
      {
        arrLoopPts.reverse();
        arrLoopPts.removeLast();
        arrLoopPts.append(arrCurvePts);
        bRet = true;
      }
      else if (ptCurveEnd.isEqualTo(ptLoopStart))
      {
        arrLoopPts.reverse();
        arrCurvePts.reverse();
        arrLoopPts.removeLast();
        arrLoopPts.append(arrCurvePts);
        bRet = true;
      }
    }
    else
    {
      if( ptCurveStart.isEqualTo(ptLoopEnd) )
      {
        arrLoopPts.removeLast();
        arrLoopPts.append(arrCurvePts);
        bRet = true;
      }
      else if (ptCurveEnd.isEqualTo(ptLoopEnd))
      {
        arrCurvePts.reverse();
        arrLoopPts.removeLast();
        arrLoopPts.append(arrCurvePts);
        bRet = true;
      }
    }

    return bRet;
  }

  //===================================================================================================

  bool isMainContour(const OdGePoint2dArray& arrPts)
  {
    bool bRet = false;

    if( arrPts.size() == 5 )
    {
      bRet = true;

      for( OdUInt32 i = 0; i < arrPts.size(); i++ )
      {
        if((!OdZero(arrPts[i].x) && !OdZero(arrPts[i].x - 1)) || (!OdZero(arrPts[i].y) && !OdZero(arrPts[i].y - 1)))
        {
          bRet = false;
          break;
        }
      }
    }

    return bRet;
  }

  //===================================================================================================

  bool getTrimmingLoops(OdDbNurbSurface* pDbNurbSurface,
                          const OdGeKnotVector& uKnots,
                            const OdGeKnotVector& vKnots,
                              OdArray<OdGePoint2dArray>& arrBoundaries,
                                bool& bHole)
  {
    OdBrBrep brSurface;
    pDbNurbSurface->brep(brSurface);

    double dXRange = uKnots.endParam() - uKnots.startParam();
    double dYRange = vKnots.endParam() - vKnots.startParam();
    double dDeviation = (dXRange > dYRange) ? dYRange / 100.0 : dXRange / 100.0;

    OdBrBrepFaceTraverser brFaceTraverser;
    OdBrErrorStatus err = brFaceTraverser.setBrep(brSurface);

    if( err != odbrOK )
      return false;

    for (; !brFaceTraverser.done(); brFaceTraverser.next())
    {
      // calc bbox
      OdBrFace brFace = brFaceTraverser.getFace();

      OdBrFaceLoopTraverser brLoopTraverser;

      err = brLoopTraverser.setFace(brFace);

      if (err != odbrOK)
        continue;

      for (; !brLoopTraverser.done(); brLoopTraverser.next())
      {
        OdBrLoop brLoop = brLoopTraverser.getLoop();

        OdBrLoopEdgeTraverser brLoopEdgeTraverser;

        if (brLoopEdgeTraverser.setLoop(brLoop) != odbrOK)
          continue;

        OdGePoint2dArray arrLoopPts;
        bool bAllowToReverseLoopPts = true;

        while (!brLoopEdgeTraverser.done())
        {
          OdGeCurve2d* pCurve2d = brLoopEdgeTraverser.getParamCurve();

          if( !pCurve2d )
            return false;

          OdGeInterval curveInterval;
          pCurve2d->getInterval(curveInterval);

          OdGePoint2dArray arrCurvePts;

          pCurve2d->getSamplePoints(curveInterval.lowerBound(), curveInterval.upperBound(), dDeviation, arrCurvePts);

          if( arrCurvePts.size() < 2 )
            continue;

          if( arrLoopPts.isEmpty() )
            arrLoopPts = arrCurvePts;
          else
          {
            if( !connectLoopPts(arrLoopPts, arrCurvePts, bAllowToReverseLoopPts) )
              return false;

            bAllowToReverseLoopPts = false;
          }

          delete pCurve2d;

          brLoopEdgeTraverser.next();
        }

        if( !arrLoopPts.isEmpty() )
        {
          if( !arrLoopPts.first().isEqualTo(arrLoopPts.last()) )
            arrLoopPts.push_back(arrLoopPts.first());

          for(OdUInt32 k = 0; k < arrLoopPts.size(); k++)
          {
            arrLoopPts[k].x -= uKnots.startParam();
            arrLoopPts[k].x /= dXRange;
            arrLoopPts[k].y -= vKnots.startParam();
            arrLoopPts[k].y /= dYRange;
          }

          arrBoundaries.push_back(arrLoopPts);
        }
      }
    }

    bHole = false;
    bool bHasMainContour = false;

    for (OdUInt32 l = 0; l < arrBoundaries.size(); l++)
    {
      if (isMainContour(arrBoundaries[l]))
      {
        arrBoundaries.removeAt(l);

        if( !arrBoundaries.isEmpty() )
          bHole = true;

        break;
      }
    }

    return true;
  }

  //===================================================================================================

  void OdDbNurbSurfaceExportPE::subExportElement(OdDbEntity* pDbEntity, OdDgElement* owner)
  {
    OdDbNurbSurface* pDbNurbSurface = (OdDbNurbSurface*)(pDbEntity);

    int iDegreeU, iDegreeV, nCtrlU, nCtrlV, iRulesU, iRulesV;
    OdGePoint3dArray arrCtrlPts;
    OdGeDoubleArray arrWeights;
    OdGeKnotVector uKnots, vKnots;
    bool bRational;
    bool bClosedInU, bClosedInV;
    bool bAllOk = true;

    if( pDbNurbSurface->get(iDegreeU, iDegreeV, bRational, nCtrlU, nCtrlV, arrCtrlPts, arrWeights, uKnots, vKnots) != eOk )
      bAllOk = false;

    if( pDbNurbSurface->isClosedInU(bClosedInU) != eOk )
      bAllOk = false;

    if( pDbNurbSurface->isClosedInV(bClosedInV) != eOk )
      bAllOk = false;

    if (pDbNurbSurface->getNumberOfSpansInU(iRulesU) != eOk)
      iRulesU = 10;

    if (pDbNurbSurface->getNumberOfSpansInV(iRulesV) != eOk)
      iRulesV = 10;

    OdArray<OdGePoint2dArray> arrBoundaries;
    bool bHole = false;

    bAllOk = getTrimmingLoops(pDbNurbSurface, uKnots, vKnots, arrBoundaries, bHole);

    if( bAllOk )
    {
      if (arrWeights.isEmpty())
        bRational = false;

      uKnots.setRange(0.0, 1.0);
      vKnots.setRange(0.0, 1.0);

      OdGePoint3dArray arrUpdatedCtrlPts;
      OdGeDoubleArray  arrUpdatedWeights;

      for (int j = 0; j < nCtrlV; j++)
      {
        for (int i = 0; i < nCtrlU; i++)
        {
          arrUpdatedCtrlPts.push_back(arrCtrlPts[i * nCtrlV + j]);

          if (bRational)
            arrUpdatedWeights.push_back(arrWeights[i * nCtrlV + j]);
        }
      }

      OdDgBSplineSurfacePtr pDgNurbSurface = OdDgBSplineSurface::createObject();
      pDgNurbSurface->set(iDegreeU + 1, iDegreeV + 1, bRational, bClosedInU, bClosedInV, nCtrlU, nCtrlV, arrUpdatedCtrlPts, arrUpdatedWeights, uKnots, vKnots);
      pDgNurbSurface->setNumberOfSpansInU(iRulesU);
      pDgNurbSurface->setNumberOfSpansInV(iRulesV);

      addElementToDatabase(pDgNurbSurface, owner);
      copyElementProperties(pDgNurbSurface, pDbNurbSurface, owner->database());
      appendOdDgElementToOwner(owner, pDgNurbSurface);
      OdDgnExportContext::addDgnElement(pDbNurbSurface->objectId(), pDgNurbSurface->elementId());

      pDgNurbSurface->setHoleFlag(bHole);

      for (OdUInt32 k = 0; k < arrBoundaries.size(); k++)
        pDgNurbSurface->addBoundary(arrBoundaries[k]);
    }
    else
    {
      if( !export3dElement(pDbEntity, owner, pDbEntity) )
        OdDgnExportPE::subExportElement(pDbEntity, owner);
    }
  }

  //===================================================================================================
}
