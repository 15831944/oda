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

#ifndef _DGN_EXPORTCOMMON_INCLUDED_ 
#define _DGN_EXPORTCOMMON_INCLUDED_

#define STL_USING_MAP
#include <OdaSTL.h>
#include <DgDatabase.h>
#include <DbEntity.h>
#include <DgGraphicsElement.h>
#include <OdFont.h>
#include <DbPolyline.h>
#include <TextDefs.h>
#include <Ge/GeCircArc3d.h>
#include <DgDimension.h>
#include <DgModel.h>

class OdDbBlockTableRecord;

/** \details
  <group OdExport_Classes> 
*/
namespace TD_DGN_EXPORT 
{
  enum OdDgnExportDimArrowheadType
  {
    acArrowDefault = 0,
    acArrowClosedBlank = 1,
    acArrowClosed = 2,
    acArrowDot = 3,
    acArrowArchTick = 4,
    acArrowOblique = 5,
    acArrowOpen = 6,
    acArrowOrigin = 7,
    acArrowOrigin2 = 8,
    acArrowOpen90 = 9,
    acArrowOpen30 = 10,
    acArrowDotSmall = 11,
    acArrowDotBlank = 12,
    acArrowSmall = 13,
    acArrowBoxBlank = 14,
    acArrowBoxFilled = 15,
    acArrowDatumBlank = 16,
    acArrowDatumFilled = 17,
    acArrowIntegral = 18,
    acArrowNone = 19,
    acArrowUserDefined = 20
  };

  OdUInt32 getDgnFontEntryId(OdFont* pFont, OdDgDatabase* pDgnDb);
  OdUInt32 getDgnFontEntryId(const OdString& strFontName, OdDgDatabase* pDgnDb, bool bShxFont);
  OdUInt32 odDgnExportLineweightMapping(OdDb::LineWeight mappingVal);
  OdUInt32 getDgnLineStyleEntryId(const OdDbObjectId& idDbLineStyle);
  void     setDgnLineStyleScale( OdDgGraphicsElement* pDgnElm, double dLineStyleScale );
  void     setDgnLineStyleModifiers(OdDgGraphicsElement* pDgElm, const OdDbEntity* pDbEnt);
  OdUInt32 getDgnColorIndex(const OdCmColor& entityColor, OdDgDatabase* pDgnDb);
  OdUInt32 getDgnEntityColorIndex(const OdCmEntityColor& entityColor, OdDgDatabase* pDgnDb);
  ODCOLORREF OdCmColorToRGB(const OdCmColor& entityColor);
  ODCOLORREF OdCmColorToRGBLayerBlock(const OdCmColor& entityColor, const OdDbObjectId& idLayer, 
    OdDgDatabase* pDgDb);
  ODCOLORREF OdCmEntityColorToRGB(const OdCmEntityColor& entityColor, const OdDbObjectId& idLayer);
  bool     getDgnVisibility(const OdDbEntity* pDbEnt);
  void     setDgnMaterial(OdDgGraphicsElement* pDgElm, const OdDbDatabase* pDwgDb, const OdDbObjectId& idMaterial, const OdDbObjectId& idLayer);
  bool     isContinuousLineStyle(const OdDbObjectId& idDbLineStyle, const OdDbObjectId& idLayer, bool bLineWidthIsConstant);
  bool     isNonModelOwner(const OdDgElement* pElm);
  void     addElementToDatabase(OdDgElement* pElm, const OdDgElement* pOwner);

  bool     isPlanar(const OdGePoint3dArray& arrPts, double dTolerance = 1e-10);
  bool     isPlanar(const OdGePoint3d* pPts, OdUInt32 nPts, double dTolerance = 1e-10);
  bool     isXRef(const OdDbBlockTableRecord* pDbBlock);

  OdDgElementId getOwnerModelId(const OdDgElement* pElm);

  OdUInt32 getDgnTextStyleIndex(const OdDbObjectId& idDbTextStyle);

  OdDgDimTextFormat::Accuracy convertAccuracyToDgnFormat(OdUInt8  uDimUnitFormat, OdUInt8  uDimPresission, double dRoundValue = 1e-10);
  OdDgnExportDimArrowheadType getArrowheadDwgType(OdDbObjectId objId);
  double getDimDefaultArrowHeight( OdDgnExportDimArrowheadType arrowType, double dArrowSize);
  OdDgDimension::TerminatorArrowHeadType getTerminatorArrowType(OdDgnExportDimArrowheadType arrowType);
  void fillUnitDescription(double dUnitScale, OdDgModel::UnitDescription& descUnit);

  OdDg::TextJustification getDgTextJustification(OdDb::TextHorzMode horMode, OdDb::TextVertMode verMode);

  OdDgGraphicsElementPtr createDgLine(const OdGePoint3d& ptStart, const OdGePoint3d& ptEnd, OdDgDatabase* pDb);
  OdDgGraphicsElementPtr createDgCurveFromGeCurve(OdGeCurve3d* pGeCurve, OdGeVector3d& vrNormal, OdDgDatabase* pDb, bool bCloseNurbs = false);
  OdDgGraphicsElementPtr createDgLineString(const OdGePoint3dArray& arrVertices, const OdDbEntityPtr& pDbEnt, OdDgDatabase* pDb);
  OdDgGraphicsElementPtr createDgShape(const OdGePoint3dArray& arrVertices, const OdDbEntityPtr& pDbEnt, OdDgDatabase* pDb);
  OdDgGraphicsElementPtr createDgArcFromGeCircleArc3d( const OdGeCircArc3d& geArc, bool bCreateEllipse, OdDgDatabase* pDb);

  bool isClosedCurve(OdDgGraphicsElement* pDgElm);
  void getDgnCurveStartEndPoint(const OdDgGraphicsElementPtr& pDgCurve, OdGePoint3d& ptStart, OdGePoint3d& ptEnd);
  void reverseDgnCurve(OdDgGraphicsElementPtr& pDgCurve);
  OdGePoint3dArray getShapePtsFromSetOfDgnCurves(const OdArray<OdDgGraphicsElementPtr>& arrDgnCurves);

  void separateContours(const OdArray<OdGePoint2dArray>& arrGeContours, const OdArray<bool>& arrOutermostLoopStatus, OdUInt32Array& arrContourLevels);

  void copyElementProperties( OdDgGraphicsElement* pDgElm, const OdDbEntity* pDbEnt, OdDgDatabase* pDgnDb, bool bSetLineStyleModifiers = true );
  void appendOdDgElementToOwner( OdDgElement* pOwner, const OdDgElement* pDgElm);
}
#endif // _DGN_EXPORTCOMMON_INCLUDED_
