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


#include "OdaCommon.h"
#include "OdRadialDimLargeRecomputor.h"
#include "RxObjectImpl.h"
#include "DbObjectContextData.h"

#include "Ge/GeExtents3d.h"
#include "Ge/GeLineSeg2d.h"
#include "Ge/GeCircArc2d.h"

#include "DbRadialDimensionLarge.h"
#include "OdRecomputorEngine.h"

#include "DbLine.h"
#include "DbPoint.h"
#include "DbArc.h"

#define STL_USING_ALGORITHM
#include "OdaSTL.h"


OdDimRecomputorPtr OdDbRadialDimLargeRecomputePEImpl::createRecomputor()
{
  return OdRxObjectImpl<OdRadialDimLargeRecomputor>::createObject();
}

OdRadialDimLargeRecomputor::OdRadialDimLargeRecomputor()
: m_dJogAngle(0.0)
, m_bExtArcOn(false)
, m_dExtArcStartAngle(0.0)
, m_dExtArcEndAngle(0.0)
{
}

OdRadialDimLargeRecomputor::~OdRadialDimLargeRecomputor()
{
}

void OdRadialDimLargeRecomputor::getDimParams(const OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx)
{
  OdDimRecomputor::getDimParams(pDim, ctx);
  OdDbRadialDimensionLargePtr pDimPtr(pDim);
  m_CenterPt = pDimPtr->center();
  m_ChordPt = ctx ? ((OdDbRadialDimensionLargeObjectContextData*)ctx)->chordPoint() : pDimPtr->chordPoint();
  m_OverrideCenterPt = ctx ? ((OdDbRadialDimensionLargeObjectContextData*)ctx)->overrideCenter() : pDimPtr->overrideCenter();
  m_JogPt = ctx ? ((OdDbRadialDimensionLargeObjectContextData*)ctx)->jogPoint() : pDimPtr->jogPoint();
  m_dJogAngle = pDimPtr->jogAngle();
  m_bExtArcOn = pDimPtr->extArcOn();
  m_dExtArcStartAngle = pDimPtr->extArcStartAngle();
  m_dExtArcEndAngle = pDimPtr->extArcEndAngle();
}

void OdRadialDimLargeRecomputor::setDimParams(OdDbDimension* pDim, OdDbDimensionObjectContextData* ctx) const
{
  OdDimRecomputor::setDimParams(pDim, ctx);
  /* Recompute doesn't change these parameters
  OdDbRadialDimensionLargePtr pDimPtr(pDim);
  pDimPtr->setCenter(m_CenterPt);
  if (!ctx || ctx->isDefaultContextData())
  {
    pDimPtr->setChordPoint(m_ChordPt);
    pDimPtr->setOverrideCenter(m_OverrideCenterPt);
    pDimPtr->setJogPoint(m_JogPt);
  }
  if (ctx)
  {
    ((OdDbRadialDimensionLargeObjectContextData*)ctx)->setChordPoint(m_ChordPt);
    ((OdDbRadialDimensionLargeObjectContextData*)ctx)->setOverrideCenter(m_OverrideCenterPt);
    ((OdDbRadialDimensionLargeObjectContextData*)ctx)->setJogPoint(m_JogPt);
  }
  pDimPtr->setJogAngle(m_dJogAngle);
  */
}

OdUInt8 OdRadialDimLargeRecomputor::getDimTypeFlag()
{
  return DimRadialLarge;
}


///

void OdRadialDimLargeRecomputor::preprocessDimPoints(OdDbDimStyleTableRecord* )
{
  // empty
}


void OdRadialDimLargeRecomputor::formatDimStrWithLimsAndTols(OdString& measurement, bool isLims, bool isTols,
                                                             double dMeasurement, OdInt16 dimLunit,
                                                             double dimTm, double dimTp, OdInt16 dimTzin,
                                                             double dimTfac, OdInt16 dimTdec, double dimRnd,
                                                             OdChar dimDsep, OdInt16 dimFrac, double dimTxt,
                                                             OdInt16 dimZin, OdInt16 dimDec, OdString dimPost,
                                                             bool bPrimaryUnits, bool dimAlt)
{
  OdDimRecomputor::formatDimStrWithLimsAndTols(measurement, isLims, isTols, dMeasurement, dimLunit,
    dimTm, dimTp, dimTzin, dimTfac, dimTdec, dimRnd, dimDsep, dimFrac, dimTxt, dimZin, dimDec, dimPost, bPrimaryUnits, dimAlt);

  int numQuotesPost = dimPost.find(OD_T("<>"));  // dimpost - primary units 
  int numQuotesApost = dimPost.find(OD_T("[]")); // dimapost - alternate units
  if ( ( bPrimaryUnits && numQuotesPost == -1) || ( !bPrimaryUnits && numQuotesApost == -1 ) )
  {
    if(getDimTypeFlag() == DimDiametric)
      measurement = OD_T("\\U+2205") + measurement;
    else
      measurement = OD_T("R") + measurement;
  }
}

// Updates measurement value
void OdRadialDimLargeRecomputor::updateMeasurement(OdDbDimStyleTableRecord* /*pDimVars*/)
{
  m_dMeasurement = m_ChordPt.distanceTo(m_CenterPt);
  m_dMeasurement *= m_dimlfac;
}

#define ADD_LINE(a, b) \
  pLine = OdDbLine::createObject(); \
  pLine->setColor(color); \
  pLine->setLineWeight(lineweight); \
  pLine->setStartPoint(a); \
  pLine->setEndPoint(b); \
  pLine->setLinetype(dimLinetype); \
  pLine->setTransparency(transparency); \
  ents.push_back(pLine);

#define ADD_POINT(a) \
  point = OdDbPoint::createObject(); \
  point->setPosition(a); \
  point->setColor(OdCmEntityColor::kByBlock); \
  point->setLayer(defPointsId); \
  point->setTransparency(transparency); \
  ents.push_back(point);

#define ADD_ARC(sAngle, eAngle) \
  pDimArc = OdDbArc::createObject(); \
  pDimArc->setLineWeight(lineweight); \
  pDimArc->setColor(color); \
  pDimArc->setCenter(centerPoint); \
  pDimArc->setRadius(radius); \
  pDimArc->setStartAngle((sAngle > Oda2PI) ? sAngle - Oda2PI : sAngle ); \
  pDimArc->setEndAngle((eAngle > Oda2PI) ? eAngle - Oda2PI : eAngle ); \
  pDimArc->setLinetype(dimLinetype); \
  pDimArc->setLinetypeScale(linetypeScale); \
  pDimArc->setTransparency(transparency); \
  ents.push_back(pDimArc);

class OdRadialLargeRecomputorEngine : public OdRecomputorEngine
{
public:
  OdGePoint3d m_centerPt;
  OdGePoint3d m_chordPt;
  OdGePoint3d m_overrideCenterPt;
  OdGePoint3d m_jogPt;

  OdGePoint3d m_endDimLinePt;

  OdGePoint3d m_dimLinePt1;
  OdGePoint3d m_dimLinePt2;

  double  m_jogAngle;

  bool m_bTextIntersectDimLine;
  OdGePoint3d  m_textStartPt;
  OdGePoint3d  m_textEndPt;

  OdGeVector3d m_dirArrow;

  double m_dStartExtAng;
  double m_dEndExtAng;

  OdInt32 m_numTextIntersection;

  OdRadialLargeRecomputorEngine() : OdRecomputorEngine() 
    , m_jogAngle(OdaPI2)
    , m_dStartExtAng(0.0)
    , m_dEndExtAng(0.0)
    , m_numTextIntersection(0)
  {
    m_dirArrow = OdGeVector3d::kXAxis;
    m_bTextIntersectDimLine = false;
  }
  void transformBy(const OdGeMatrix3d& matr)
  {
    m_centerPt.transformBy(matr);
    m_chordPt.transformBy(matr);
    m_overrideCenterPt.transformBy(matr);
    m_jogPt.transformBy(matr);
    m_centerPt.z = m_chordPt.z = m_overrideCenterPt.z = m_jogPt.z = 0.0;
  }
  void setTextParams(OdUInt16 tad, double gap);
  void setAszParams(double asz)
  {
    m_asz1 = asz;
  }
  void intersectTextBoxWithDimLine();
  void calcDimLines();
  void calcTextDirection();
  void adjustTextByVert(OdInt16 dimMove);
  bool calcExtensionArc(double startAngle, double endAngle, double dimExe, double dimExo);
  void calcTextHomePosition(OdInt16 dimMove);
};

#ifdef _DEBUG
#define DEBUG_BUILD_LARGE_DIM
#endif


void OdRadialDimLargeRecomputor::buildDimension(OdArray<OdDbEntityPtr>& ents, OdDbDimStyleTableRecord* pDimVars)
{
  OdDb::LineWeight lineweight = OdDb::LineWeight(getDimlwd(pDimVars));
  OdCmColor color = getDimclrd(pDimVars);
  OdDbObjectId dimLinetype = m_DimLinetypeId;
  OdCmTransparency transparency = m_transparency;

  OdRadialLargeRecomputorEngine engine;
  engine.m_centerPt = m_CenterPt;
  engine.m_chordPt = m_ChordPt;
  engine.m_overrideCenterPt = m_OverrideCenterPt;
  engine.m_jogPt = m_JogPt;

  OdGeMatrix3d mWorld2Plane(OdGeMatrix3d::worldToPlane(m_vNormal));

  engine.transformBy(mWorld2Plane);
  getUCS(engine.m_xAxis, engine.m_yAxis);

  engine.m_jogAngle = m_dJogAngle;

  OdUInt16 tad = getDimtad(pDimVars);
  engine.setTextParams(tad, getDimgap(pDimVars));

  OdDbMTextPtr pText;
  createTextEntityForDimGraphic(pText, pDimVars);
  engine.m_textPosition = m_ocsDimTextMidPt;
  engine.m_textPosition.z = 0.0;

  OdDbMTextPtr pAddedText;
  pAddedText = engine.createXText(pText, getDimtmove(pDimVars), m_bUserDefinedTextPosition, m_dTextRot, true);

  engine.calcTextExtents(pText, false, false);

  OdDbMTextPtr pLabel, pRate;
  if ( m_bInspection )
  {
    engine.setInspection( (m_nInspectionFrame & OdDbDimension::kShapeLabel) != 0, 
      (m_nInspectionFrame & OdDbDimension::kShapeRate) != 0, 
      (m_nInspectionFrame & (OdDbDimension::kShapeRound | OdDbDimension::kShapeAngular ) ) != 0 );
    if ( pText.get() )
    {
      createInspectionEntitisForDimGraphic(pLabel, pRate, pDimVars);
      engine.calcInspectionExtents(pLabel, pRate, true);
    }
  }

  // Arrow blocks
  OdDbObjectId dimBlk;
  if(getDimsah(pDimVars))
  {
    dimBlk = getDimblk1(pDimVars);
  }
  else
  {
    dimBlk = getDimblk(pDimVars);
  }
  double asz = 0.0, asz1 = 0.0;
  asz = asz1 = getDimasz(pDimVars);
  if(toExtend(dimBlk))
  {
    asz1 = 0.0;
  }
  engine.setAszParams(asz1);
  engine.calcDimLines();

  if(pText.get())
  {
    if (OdNonZero(m_dTextRot) || getDimtih(pDimVars) )
    {
      engine.m_dTextRot = m_dTextRot;
      if ( getDimtih(pDimVars) )
        engine.m_bTextHor = true;
      else
        engine.m_bUseRotate = true;
    }

    if( !m_bUserDefinedTextPosition )
    {
      engine.calcTextDirection();
      engine.calcTextHomePosition(getDimtmove(pDimVars));
    } else
    {
      engine.calcTextDirection();
      if (getDimtmove(pDimVars) != 2 )
        engine.adjustTextByVert(getDimtmove(pDimVars));
    }

    pText->setLocation(engine.m_textPosition);
    if (OdNonZero(m_dTextRot) )
      pText->setRotation(m_dTextRot);
    else if ( getDimtih(pDimVars) )
      pText->setRotation(0.0);
    else
      pText->setDirection(engine.m_textDirection);
    engine.m_dTextRot = pText->rotation();
  }

  engine.intersectTextBoxWithDimLine();

  OdDbLinePtr pLine;
  if ( m_bArrowFirstIsFlipped && asz > 0.0 && !getDimsd2(pDimVars) )
  {
    engine.m_dirArrow *= -1;
    OdGePoint3d p1, p2;
    p1 = engine.m_chordPt + engine.m_dirArrow * asz;
    p2 = p1 + engine.m_dirArrow * asz;
    ADD_LINE(p1, p2);
    engine.m_endDimLinePt = engine.m_chordPt;
  }

  if (!getDimsd2(pDimVars))
  {
    ADD_LINE(engine.m_overrideCenterPt, engine.m_dimLinePt1);
    setDimLineIndex(ents.last(), DIM_LINE_INDEX);
    ADD_LINE(engine.m_dimLinePt2, engine.m_dimLinePt1);
    setDimLineIndex(ents.last(), DIM_LINE_INDEX);

    if (engine.m_bTextIntersectDimLine)
    {
      if (engine.m_numTextIntersection == 2)
      {
        ADD_LINE(engine.m_dimLinePt2, engine.m_textStartPt);
        setDimLineIndex(ents.last(), DIM_LINE_INDEX);
        ADD_LINE(engine.m_textEndPt, engine.m_endDimLinePt);
      }
      else
      {
        ADD_LINE(engine.m_textStartPt, engine.m_textEndPt);
      }
      setDimLineIndex(ents.last(), DIM_LINE_INDEX);
    }
    else
    {
      ADD_LINE(engine.m_dimLinePt2, engine.m_endDimLinePt);
      setDimLineIndex(ents.last(), DIM_LINE_INDEX);
    }
  }

  buildArrow2(engine.m_chordPt, engine.m_dirArrow, ents, pDimVars);

  // Make Inspection
  if ( m_bInspection )
  {
    OdGePoint3d pos = engine.m_textPosition;
    makeInspectionFrame(pDimVars, &engine, ents);
    if ( !pText.isNull() )
    {
      pText->setLocation(engine.m_textPosition);
      pText->setDirection(engine.m_textDirection);
      ents.push_back(pText);
    }

    if ( !pLabel.isNull() )
    {
      pLabel->setLocation(engine.m_labelPosition);
      pLabel->setDirection(engine.m_textDirection);
      ents.push_back(pLabel);
    }

    if ( !pRate.isNull() )
    {
      pRate->setLocation(engine.m_ratePosition);
      pRate->setDirection(engine.m_textDirection);
      ents.push_back(pRate);
    }
    engine.m_textPosition = pos; // restore for calculate arcsym posision
  } else
  {
    if(pText.get())
    {
      ents.push_back(pText);
      buildBoundaryBox(pDimVars, pText, ents);
    }
  }

  if ( m_bExtArcOn )
  {
    if ( engine.calcExtensionArc(m_dExtArcStartAngle, m_dExtArcEndAngle, getDimexe(pDimVars), getDimexo(pDimVars)) )
    {
      const OdGePoint3d centerPoint = engine.m_centerPt;
      const double radius = engine.m_chordPt.distanceTo(engine.m_centerPt);
      OdDbArcPtr pDimArc;

      color = getDimclre(pDimVars);
      lineweight = OdDb::LineWeight(getDimlwe(pDimVars));
      const double linetypeScale = m_dLinetypeScale;

      ADD_ARC(engine.m_dStartExtAng, engine.m_dEndExtAng);
    }
  }
  if ( !pText.isNull() )
  {
    pText->setAttachmentMovingLocation((OdDbMText::AttachmentPoint)m_AttachPt);
  } 

  OdDbPointPtr point;
  OdDbObjectId defPointsId = getLayerDefpointsId();
  ADD_POINT(engine.m_chordPt);
  ADD_POINT(engine.m_overrideCenterPt);
  ADD_POINT(engine.m_jogPt);
}

void OdRadialLargeRecomputorEngine::setTextParams(OdUInt16 tad, double gap)
{
  m_tad = tad; 
  m_isBox = ( gap < 0.0 ) ? true : false;
  m_gap = ::fabs(gap);
}

void OdRadialLargeRecomputorEngine::intersectTextBoxWithDimLine()
{
  if ( !m_bNeedCalcTextIntersection )
  {
    m_bTextIntersectDimLine = false;
    return;
  }
  const OdGeMatrix2d matr = OdGeMatrix2d::rotation(m_dTextRot, OdGePoint2d(m_textPosition.x, m_textPosition.y));
  OdGePoint2d p1, p2;
  OdGeLineSeg2d line1, line2;
  OdGePoint3d minPt;
  OdGePoint3d maxPt;

  const double extX2 = textWidth()/2.0;
  const double extY2 = textHeight()/2.0;

  minPt.x = m_textPosition.x - extX2;
  maxPt.x = m_textPosition.x + extX2;

  minPt.y = m_textPosition.y - extY2;
  maxPt.y = m_textPosition.y + extY2;

  OdGePoint3dArray intersections;

  line1.set(OdGePoint2d(m_dimLinePt2.x,m_dimLinePt2.y), OdGePoint2d(m_chordPt.x,m_chordPt.y));
#ifdef DEBUG_BUILD_LARGE_DIM
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", line1.startPoint().x, line1.startPoint().y);
  ODA_TRACE2("%f,%f,0.0\n\n", line1.endPoint().x, line1.endPoint().y);
#endif

  line2.set(OdGePoint2d(minPt.x, minPt.y), OdGePoint2d(minPt.x, maxPt.y));
  line2.transformBy(matr);
#ifdef DEBUG_BUILD_LARGE_DIM
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", line2.startPoint().x, line2.startPoint().y);
  ODA_TRACE2("%f,%f,0.0\n\n", line2.endPoint().x, line2.endPoint().y);
#endif
  if ( line1.intersectWith(line2, p1) )
  {
    intersections.push_back(OdGePoint3d(p1.x, p1.y, 0.0));
  }

  line2.set(OdGePoint2d(minPt.x, maxPt.y), OdGePoint2d(maxPt.x, maxPt.y));
  line2.transformBy(matr);
#ifdef DEBUG_BUILD_LARGE_DIM
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", line2.startPoint().x, line2.startPoint().y);
  ODA_TRACE2("%f,%f,0.0\n\n", line2.endPoint().x, line2.endPoint().y);
#endif
  if ( line1.intersectWith(line2, p1) )
  {
    intersections.push_back(OdGePoint3d(p1.x, p1.y, 0.0));
  }

  line2.set(OdGePoint2d(maxPt.x, maxPt.y), OdGePoint2d(maxPt.x, minPt.y));
  line2.transformBy(matr);
#ifdef DEBUG_BUILD_LARGE_DIM
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", line2.startPoint().x, line2.startPoint().y);
  ODA_TRACE2("%f,%f,0.0\n\n", line2.endPoint().x, line2.endPoint().y);
#endif
  if ( line1.intersectWith(line2, p1) )
  {
    intersections.push_back(OdGePoint3d(p1.x, p1.y, 0.0));
  }

  line2.set(OdGePoint2d(maxPt.x, minPt.y), OdGePoint2d(minPt.x, minPt.y));
  line2.transformBy(matr);
#ifdef DEBUG_BUILD_LARGE_DIM
  ODA_TRACE0("line\n");
  ODA_TRACE2("%f,%f,0.0\n", line2.startPoint().x, line2.startPoint().y);
  ODA_TRACE2("%f,%f,0.0\n\n", line2.endPoint().x, line2.endPoint().y);
#endif
  if ( line1.intersectWith(line2, p1) )
  {
    intersections.push_back(OdGePoint3d(p1.x, p1.y, 0.0));
  }

  if ( intersections.size() == 2 )
  {
    double dist1 = intersections[0].distanceTo(m_chordPt);
    double dist2 = intersections[1].distanceTo(m_chordPt);
    if ( dist1 > dist2 )
    {
      m_textStartPt = intersections[0];
      m_textEndPt = intersections[1];
    } else
    {
      m_textStartPt = intersections[1];
      m_textEndPt = intersections[0];
    }
    m_bTextIntersectDimLine = true;
    m_numTextIntersection = 2;
  } else if ( intersections.size() == 1 )
  {
    m_bTextIntersectDimLine = true;
    m_numTextIntersection = 1;
    if (intersections[0].distanceTo(m_chordPt) < intersections[0].distanceTo(m_dimLinePt2))
    {
      m_textStartPt = m_dimLinePt2;
      m_textEndPt = intersections[0];
    } else
    {
      m_textStartPt = m_endDimLinePt;
      m_textEndPt = intersections[0];
    }
  } else
  {
    m_bTextIntersectDimLine = false;
  }
}

void OdRadialLargeRecomputorEngine::calcDimLines()
{

  OdGeVector3d v = m_centerPt - m_chordPt;

  if ( m_centerPt.distanceTo(m_overrideCenterPt) >
    m_centerPt.distanceTo(m_chordPt) )
  {
    const OdGeVector3d perp = v.perpVector();
    OdGeLine2d line1, line2;
    line1.set(OdGePoint2d(m_centerPt.x,m_centerPt.y), OdGeVector2d(perp.x, perp.y));
    line2.set(OdGePoint2d(m_overrideCenterPt.x,m_overrideCenterPt.y), OdGeVector2d(v.x, v.y));
    OdGePoint2d intrsecPt;
    if ( line1.intersectWith(line2, intrsecPt) )
    {
      const OdGePoint2d ovrPt(m_overrideCenterPt.x, m_overrideCenterPt.y);
      if ( intrsecPt.distanceTo( OdGePoint2d(m_overrideCenterPt.x, m_overrideCenterPt.y)) > m_centerPt.distanceTo(m_chordPt))
      {
        const OdGeVector2d vect = intrsecPt - ovrPt;
        if ( OdNonZero(vect.length()) && vect.isCodirectionalTo(OdGeVector2d(v.x, v.y)) )
        {
          v *= -1.0;
        }
      } 
    }
  }

  m_dirArrow = v;
  m_dirArrow.normalize();

  OdGeLine3d line1, line2, line3;
  line1.set(m_chordPt, v);
  line2.set(m_overrideCenterPt, v);

  const OdGeVector3d v1 = m_centerPt - m_overrideCenterPt;
  double jogAng = m_jogAngle;
  if (v.angleTo(v1, OdGeVector3d::kZAxis) > OdaPI )
    jogAng = -jogAng;

  v.rotateBy(jogAng, OdGeVector3d::kZAxis);

  line3.set(m_jogPt, v);

  line1.intersectWith(line3, m_dimLinePt2);
  line2.intersectWith(line3, m_dimLinePt1);

  m_endDimLinePt = m_chordPt + m_dirArrow * m_asz1;

  if ( line1.isOn(m_textPosition) || m_bUseRotate || m_bTextHor )
    m_bNeedCalcTextIntersection = true;
  /*
   else
  m_bNeedCalcTextIntersection = false;
  */
}

void OdRadialLargeRecomputorEngine::calcTextDirection()
{
  if ( !m_bUseRotate && !m_bTextHor)
  {
    const OdGeVector3d v = m_centerPt-m_chordPt;
    const double d = v.angleTo(m_xAxis, OdGeVector3d::kZAxis);
    //if ( v.x < 0.0 )
    if ( d > OdaPI2 && d < OdaPI + OdaPI2 )
      m_textDirection = -1.0*v;
    else
      m_textDirection = v;
  }
}

void OdRadialLargeRecomputorEngine::adjustTextByVert(OdInt16 dimMove)
{
  if ( !m_bUseRotate && !m_bTextHor )
  {
    if ( m_tad > 0 ) // TODO: for Outside & JIS
    {
      OdGeVector3d v = OdGeVector3d::kZAxis.crossProduct(m_textDirection).normalize();
      double distance = 0.0;
      if ( m_isBox ) 
        distance += m_gap;
      distance += (textHeight(false))/2.0 + m_gap;
      if ( m_tad == 4 )
        v *= -1.0;
      if (!m_bUseXText)
        m_textPosition += v*distance;
      m_bNeedCalcTextIntersection = false;
    }
  } else
  {
    if ( dimMove < 2 && m_tad > 0 && m_bTextHor )
    {
      const OdGeVector3d vect = m_centerPt-m_chordPt;
      OdGeVector3d v = OdGeVector3d::kZAxis.crossProduct(vect).normalize();

      double distance = 0.0;

      const double ang = vect.angleTo(OdGeVector3d::kXAxis);
      distance += textWidth()/2.0*sin(ang) + textHeight()/2.0*cos(ang);

      if ( m_tad == 4 )
        v *= -1.0;
      if (!m_bUseXText)
        m_textPosition += v*distance;
      m_bNeedCalcTextIntersection = false;
    }
  }
}

bool OdRadialLargeRecomputorEngine::calcExtensionArc(double startAngle, double endAngle,
                                                double dimExe, double dimExo)
{
  if ( OdZero(endAngle-startAngle) )
    return false;
  const double radius = m_chordPt.distanceTo(m_centerPt);
  OdGeCircArc2d arc;
  arc.set(OdGePoint2d(m_centerPt.x, m_centerPt.y), radius, startAngle, endAngle);
  const OdGePoint2d point(m_chordPt.x, m_chordPt.y);
  if ( !arc.isOn(point) )
  {
    const OdGePoint2d p1 = arc.startPoint();
    const OdGePoint2d p2 = arc.endPoint();

    double distance = 0.0;
    double dir = 1.0;

    if ( point.distanceTo(p1) < point.distanceTo(p2) )
    { 
      m_dStartExtAng = arc.startAng();
      distance = point.distanceTo(p1);
      dir = -1.0;
    } else
    {
      m_dStartExtAng = arc.endAng();
      distance = point.distanceTo(p2);
    }

    m_dEndExtAng = m_dStartExtAng + dir*chordAngle(distance, radius) + dir*chordAngle(dimExe, radius);

    if ( distance > dimExo )
    {
      m_dStartExtAng += dir*chordAngle(dimExo, radius);;
    }

    if ( m_dStartExtAng > m_dEndExtAng )
    {
      std::swap(m_dStartExtAng, m_dEndExtAng);
    }
    return true;
  }
  return false;
}

void OdRadialLargeRecomputorEngine::calcTextHomePosition(OdInt16 dimMove)
{
  double dist = m_dimLinePt2.distanceTo(m_chordPt) - m_asz1;
  m_textPosition = m_chordPt + m_dirArrow*(dist/2.0 + m_asz1);
  adjustTextByVert(dimMove);
  if ( m_tad == 0 || m_bUseRotate || m_bTextHor )
    m_bNeedCalcTextIntersection = true;
}

