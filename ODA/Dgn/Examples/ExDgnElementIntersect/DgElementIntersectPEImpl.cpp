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
#include "DgElementIntersectPEImpl.h"
#include "GiDrawObjectsForIntersect.h"
#include "DgGiContext.h"

//=====================================================================================================

OdResult OdDgElementIntersectBasePE::intersectWith( const OdDgGraphicsElement* pThisElm,
                                                      const OdDgGraphicsElement* pElm, 
                                                        OdDg::OdDgIntersectType intType,
                                                          OdGePoint3dArray& points
                                                  )
{
  OdResult retVal = eOk;

  OdGiContextForDgDatabasePtr pCtx = OdGiContextForDgDatabase::createObject();
  pCtx->setDatabase(pThisElm->database());

  OdGiAuxDataForDgDatabasePtr pAuxData = OdGiAuxDataForDgDatabase::createObject();
  pAuxData->setFastCurveFlag(false);
  pAuxData->setHideTextsFlag(false);
  pAuxData->setHighQualityFontsFlag(false);
  pAuxData->setShowLineWeightsFlag(false);
  pAuxData->setShowPatternsFlag(true);
  pAuxData->setShowTextNodesFlag(false);
  pAuxData->setShowDataFieldsFlag(false);
  pAuxData->setShowGridFlag(false);
  pAuxData->setShowLevelSymbologyFlag(false);
  pAuxData->setShowPointsFlag(false);
  pAuxData->setShowConstructionFlag(true);
  pAuxData->setShowDimensionsFlag(false);
  pAuxData->setFastCellsFlag(false);
  pAuxData->setShowFillsFlag(false);
  pAuxData->setRasterTextFlag(false);
  pAuxData->setShowAxisTriadFlag(false);
  pAuxData->setOrientationDisplayFlag(false);
  pAuxData->setViewRenderedFlag(false);
  pAuxData->setShowBackgroundFlag(false);
  pAuxData->setShowBoundaryFlag(false);
  pAuxData->setFastBoundaryClipFlag(false);
  pAuxData->setDepthCueFlag(false);
  pAuxData->setInhibitDynamicsFlag(false);
  pAuxData->setShowShadowsFlag(false);
  pAuxData->setShowTextureMapsFlag(false);
  pAuxData->setShowHazeFlag(false);
  pAuxData->setTransparencyFlag(false);
  pAuxData->setIgnoreLineStylesFlag(true);
  pAuxData->setAcceleratedFlag(false);
  pAuxData->setPatternDynamicsFlag(false);
  pAuxData->setHiddenLineFlag(false);
  pAuxData->setShowTagsFlag(true);
  pAuxData->setDisplayEdgesFlag(false);
  pAuxData->setDisplayHiddenEdgesFlag(false);
  pAuxData->setOverrideBackgroundFlag(false);
  pAuxData->setShowClipFrontFlag(false);
  pAuxData->setShowClipBackFlag(false);
  pAuxData->setShowClipVolumeFlag(false);
  pAuxData->setUseDisplaySetFlag(false);
  pAuxData->setShowConstructionOnlyFlag(false);
  pAuxData->setDisplayMode(OdDgView::kWireFrame);

  OdStaticRxObject<OdGiDrawObjectsForIntersect> drawObject;
  drawObject.setContext( pCtx );
  drawObject.setAuxData(pAuxData);
  retVal = drawObject.intersect(pThisElm, pElm, (OdGiDrawObjectsForIntersect::OdGiIntersectType)intType, points);

  return retVal;
}

//=====================================================================================================

OdResult OdDgElementIntersectBasePE::intersectWith( const OdDgGraphicsElement* pThisElm,
                                                      const OdDgGraphicsElement* pElm, 
                                                        OdDg::OdDgIntersectType intType,
                                                          const OdGePlane& projPlane, 
                                                            OdGePoint3dArray& points
                                                  )
{
  return eNotImplementedYet;
}

//=====================================================================================================
