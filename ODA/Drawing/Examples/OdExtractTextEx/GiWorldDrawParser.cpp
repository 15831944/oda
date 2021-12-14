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
#include "ExProtocolExtension.h"
#include "GiWorldDrawParser.h"
#include "toString.h"

/************************************************************************/
/* GiWorldDrawParser.cpp: implementation of the GiWorldDraw class.      */
/*                                                                      */
/* The OdGiWorldGeometryParser and OdGiSubEntityTraitsParser functions  */
/* are called by pEnt->worldraw() to vectorize *pEnt.                   */ 
/*                                                                      */
/* Since no actual vectorization is being performed, many of the        */
/* methods do nothing but return a default value                        */                                                
/************************************************************************/

/************************************************************************/
/* Implementation of the GiWorldDraw class.                             */
/************************************************************************/
OdGiWorldDrawParser::OdGiWorldDrawParser()
{
    m_WdGeom = OdRxObjectImpl<OdGiWorldGeometryParser>::createObject();
    m_Traits = OdRxObjectImpl<OdGiSubEntityTraitsParser>::createObject();
}

void OdGiWorldDrawParser::setIndent(int indent)
{
  m_WdGeom->m_indent = indent;
}

/************************************************************************/
/* geometry                                                             */
/************************************************************************/
OdGiWorldGeometry& OdGiWorldDrawParser::geometry() const
{
  return (OdGiWorldGeometry&)*m_WdGeom.get();
}

/************************************************************************/
/* regenType                                                            */
/************************************************************************/
OdGiRegenType OdGiWorldDrawParser::regenType() const
{
  return kOdGiForExplode;
}

/************************************************************************/
/* regenAbort                                                            */
/************************************************************************/
bool OdGiWorldDrawParser::regenAbort() const
{
  return false;
}

/************************************************************************/
/* subEntityTraits                                                      */
/************************************************************************/
OdGiSubEntityTraits& OdGiWorldDrawParser::subEntityTraits() const
{
  return (OdGiSubEntityTraits&)*m_Traits.get();
}

/************************************************************************/
/* rawGeometry                                                          */
/************************************************************************/
OdGiGeometry& OdGiWorldDrawParser::rawGeometry() const
{
  return (OdGiGeometry&)*m_WdGeom.get();
}

/************************************************************************/
/* isDragging                                                            */
/************************************************************************/
bool OdGiWorldDrawParser::isDragging() const
{
  return false;
}
  
/************************************************************************/
/* deviation                                                            */
/************************************************************************/
double OdGiWorldDrawParser::deviation(const OdGiDeviationType, const OdGePoint3d&) const
{
  return 0;
}

/************************************************************************/
/* modelDeviation                                                       */
/************************************************************************/
double OdGiWorldDrawParser::modelDeviation(const OdGiDeviationType /*type*/, const OdGePoint3d& /*modelPoint*/) const
{
  return 0;
}

/************************************************************************/
/* numberOfIsolines                                                     */
/************************************************************************/
OdUInt32 OdGiWorldDrawParser::numberOfIsolines() const
{
  return 1;
}
  
/************************************************************************/
/* setContext                                                           */
/************************************************************************/
void OdGiWorldDrawParser::setContext(OdGiContext* pUserContext)
{
  pCtx = pUserContext;
}

/************************************************************************/
/* context                                                              */
/************************************************************************/
OdGiContext* OdGiWorldDrawParser::context() const
{
  return pCtx;
}



/************************************************************************/
/* Implementation of the GiWorldGeometry class.                         */
/* This class defines functions that allow entities to vectorize        */
/* (or parse) themselves                                                 */
/************************************************************************/

/************************************************************************/
/* Set Extents                                                          */
/************************************************************************/
void OdGiWorldGeometryParser::setExtents(const OdGePoint3d *extents)
{
}

/************************************************************************/
/* Start Attributes Segment                                             */
/************************************************************************/
void OdGiWorldGeometryParser::startAttributesSegment()
{
}

/************************************************************************/
/* Push Model Transform                                                 */
/************************************************************************/
void OdGiWorldGeometryParser::pushModelTransform(const OdGeVector3d& normal)
{
}


/************************************************************************/
/* Push Model Transform                                                 */
/************************************************************************/
void OdGiWorldGeometryParser::pushModelTransform(const OdGeMatrix3d& xfm)
{
}

/************************************************************************/
/* Pop Model Transform                                                  */
/************************************************************************/
void OdGiWorldGeometryParser::popModelTransform()
{
}

/************************************************************************/
/* Body                                                               */
/************************************************************************/
void OdGiWorldGeometryParser::body(const OdString& body, void **pCacheData)
{
}

/************************************************************************/
/* Circle                                                               */
/************************************************************************/
void OdGiWorldGeometryParser::circle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal)
{
}
  
/************************************************************************/
/* Circle                                                               */
/************************************************************************/
void OdGiWorldGeometryParser::circle(const OdGePoint3d& firstPoint, 
    const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint)
{
}

/************************************************************************/
/* Circular Arc                                                         */
/************************************************************************/
void OdGiWorldGeometryParser::circularArc(
    const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType)
{
}

/************************************************************************/
/* Circular Arc                                                         */
/************************************************************************/
void OdGiWorldGeometryParser::circularArc(
    const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType)
{
}

/************************************************************************/
/* Elliptical Arc                                                        */
/************************************************************************/
void OdGiWorldGeometryParser::ellipArc(
    const OdGeEllipArc3d& ellipArc,
    const OdGePoint3d* endPointsOverrides,
    OdGiArcType arcType)
{
}

/************************************************************************/
/* draw                                                                 */
/************************************************************************/
void OdGiWorldGeometryParser:: draw(
    const OdGiDrawable* pDrawable) 
{
  OdDbEntityPtr pEnt = OdDbEntity::cast(pDrawable);
  if (!pEnt.isNull())
  {
    OdSmartPtr<OdDbEntity_Parser> pEntParser = pEnt;
    pEntParser->extractText(pEnt, m_indent);
  }
}

/************************************************************************/
/* Elliptical Arc                                                        */
/************************************************************************/
void OdGiWorldGeometryParser::ellipArc(
    const OdGePoint3d& center,
    const OdGeVector3d& majorRadius,
    const OdGeVector3d& minorRadius,
    const OdGePoint3d& startPoint,
    const OdGePoint3d& endPoint,
    OdGiArcType arcType) 
{
}

/************************************************************************/
/* Polyline                                                             */
/************************************************************************/
void OdGiWorldGeometryParser::polyline(
    OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal,
    OdGsMarker lBaseSubEntMarker)
{
}

/************************************************************************/
/* Polygon                                                              */
/************************************************************************/
void OdGiWorldGeometryParser::polygon(
    OdInt32 numVertices, 
    const OdGePoint3d* vertexList)
{
}

/************************************************************************/
/* LW Polyline                                                          */
/************************************************************************/
void OdGiWorldGeometryParser::pline(
    const OdGiPolyline& polyline, 
    OdUInt32 fromIndex, 
    OdUInt32 numSegs)
{
}

/************************************************************************/
/* WorldLine                                                            */
/************************************************************************/
void OdGiWorldGeometryParser:: worldLine(const OdGePoint3d points[2])
{
}

/************************************************************************/
/* BGRAImage                                                            */
/************************************************************************/
void OdGiWorldGeometryParser::image(
    const OdGiImageBGRA32& img,
    const OdGePoint3d& origin,
    const OdGeVector3d& uVec,
    const OdGeVector3d& vVec,
    OdGiRasterImage::TransparencyMode trpMode)
{
}

/************************************************************************/
/* Edges                                                                */
/************************************************************************/
void OdGiWorldGeometryParser::edge(
  const OdGiEdge2dArray& edges)
{
}

/************************************************************************/
/* Mesh                                                                 */
/************************************************************************/
void OdGiWorldGeometryParser::mesh(
    OdInt32 numRows,
    OdInt32 numColumns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData)
{
}

/************************************************************************/
/* Shell                                                                */
/************************************************************************/
void OdGiWorldGeometryParser::shell(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData)
{
}

/************************************************************************/
/* Text                                                                 */
/************************************************************************/
void OdGiWorldGeometryParser::text(
    const OdGePoint3d& position,
    const OdGeVector3d& normal, 
    const OdGeVector3d& direction,
    double height, 
    double width, 
    double oblique, 
    const OdString& msg)
{
  writeLine(m_indent + 1, L"Text string", msg);
}

/************************************************************************/
/* Text                                                                 */
/************************************************************************/
void OdGiWorldGeometryParser::text(
    const OdGePoint3d& position,
    const OdGeVector3d& normal, 
    const OdGeVector3d& direction,
    const OdChar* msg, 
    OdInt32 length, 
    bool raw, 
    const OdGiTextStyle* pTextStyle)
{
  // msg may not be null-terminated.
  OdString buffer(msg, length);
  writeLine(m_indent + 1, L"Text string", buffer);
}

/************************************************************************/
/* Xline                                                                */
/************************************************************************/
void OdGiWorldGeometryParser::xline(
    const OdGePoint3d& firstPoint, 
    const OdGePoint3d& secondPoint)
{
}

/************************************************************************/
/* Ray                                                                 */
/************************************************************************/
void OdGiWorldGeometryParser::ray(
    const OdGePoint3d& basePoint, 
    const OdGePoint3d& throughPoint)
{
}

/************************************************************************/
/* LW Polyline                                                          */
/************************************************************************/
void OdGiWorldGeometryParser::pline(
    const OdDbPolyline& polyline, 
    OdUInt32 fromIndex, 
    OdUInt32 numSegs)
{
}
/************************************************************************/
/* Nurbs curve                                                          */
/************************************************************************/
void OdGiWorldGeometryParser::nurbs(const OdGeNurbCurve3d& nurbsCurve)
{
}

/************************************************************************/
/* Clip Boundary                                                        */
/************************************************************************/
void OdGiWorldGeometryParser::pushClipBoundary(OdGiClipBoundary* pBoundary)
{
}

void OdGiWorldGeometryParser::popClipBoundary()
{
}

/************************************************************************/
/* OdGiSubEntityTraitsParser                                            */
/*                                                                      */
/* An OdGiSubEntityTraits instance is available during calls to         */
/* worldDraw and viewportDraw, so that drawable classes can control     */
/* attributes during the vectorization process. Graphical attribute     */
/* settings are used for all graphical primitives until they are        */
/*                                                                      */
/* Calls to members of OdGiSubEntityTraitsParser actually do nothing    */
/* and return a default value.                                          */
/************************************************************************/

/************************************************************************/
/* setColor                                                             */
/************************************************************************/
void OdGiSubEntityTraitsParser::setColor(OdUInt16 colorIndex)
{
}

/************************************************************************/
/* setTrueColor                                                         */
/************************************************************************/
void OdGiSubEntityTraitsParser::setTrueColor(const OdCmEntityColor& trueColor)
{
}

/************************************************************************/
/* setLayer                                                             */
/************************************************************************/
void OdGiSubEntityTraitsParser::setLayer(OdDbStub* layerId)
{
}
/************************************************************************/
/* setLineType                                                          */
/************************************************************************/
void OdGiSubEntityTraitsParser::setLineType(OdDbStub* linetypeId)
{
}

/************************************************************************/
/* setSelectionMarker                                                   */
/************************************************************************/
void OdGiSubEntityTraitsParser::setSelectionMarker(OdGsMarker markerId)
{
}

/************************************************************************/
/* setFillType                                                          */
/************************************************************************/
void OdGiSubEntityTraitsParser::setFillType(OdGiFillType fillType)
{
}

/************************************************************************/
/* setLineWeight                                                        */
/************************************************************************/
void OdGiSubEntityTraitsParser::setLineWeight(OdDb::LineWeight lineWeight)
{
}

/************************************************************************/
/* setLineTypeScale                                                     */
/************************************************************************/
void OdGiSubEntityTraitsParser::setLineTypeScale(double lineTypeScale)
{
}

/************************************************************************/
/* setThickness                                                         */
/************************************************************************/
void OdGiSubEntityTraitsParser::setThickness(double thickness)
{
}

/************************************************************************/
/* setPlotStyleName                                                     */
/************************************************************************/
void OdGiSubEntityTraitsParser::setPlotStyleName(OdDb::PlotStyleNameType plotStyleNameType, OdDbStub* plotStyleNameId )
{
}

/************************************************************************/
/* setMaterial                                                          */
/************************************************************************/
void OdGiSubEntityTraitsParser::setMaterial(OdDbStub* materialId)
{
}

/************************************************************************/
/* setMapper                                                            */
/************************************************************************/
void OdGiSubEntityTraitsParser::setMapper(const OdGiMapper* pMapper)
{
}

/************************************************************************/
/* setVisualStyle                                                       */
/************************************************************************/
void OdGiSubEntityTraitsParser::setVisualStyle(const OdDbStub* visualStyleId)
{
}

/************************************************************************/
/* setTransparency                                                      */
/************************************************************************/
void OdGiSubEntityTraitsParser::setTransparency(const OdCmTransparency &transparency)
{
}

/************************************************************************/
/* setDrawFlags                                                         */
/************************************************************************/
void OdGiSubEntityTraitsParser::setDrawFlags(OdUInt32 drawFlags)
{
}

/************************************************************************/
/* setSelectionGeom                                                     */
/************************************************************************/
void OdGiSubEntityTraitsParser::setSelectionGeom(bool bSelectionFlag)
{
}

/************************************************************************/
/* setShadowFlags                                                       */
/************************************************************************/
void OdGiSubEntityTraitsParser::setShadowFlags(OdGiSubEntityTraits::ShadowFlags shadowFlags)
{
}

/************************************************************************/
/* setSectionable                                                       */
/************************************************************************/
void OdGiSubEntityTraitsParser::setSectionable(bool bSectionableFlag)
{
}

/************************************************************************/
/* setSelectionFlags                                                    */
/************************************************************************/
void OdGiSubEntityTraitsParser::setSelectionFlags(OdGiSubEntityTraits::SelectionFlags selectionFlags)
{
}

/************************************************************************/
/* color stub                                                           */
/************************************************************************/
OdUInt16 OdGiSubEntityTraitsParser::color() const
{
  return 0;
}

/************************************************************************/
/* layer stub                                                           */
/************************************************************************/
OdDbStub* OdGiSubEntityTraitsParser::layer() const
{
  return NULL;
}

/************************************************************************/
/* lineType stub                                                        */
/************************************************************************/
OdDbStub* OdGiSubEntityTraitsParser::lineType() const
{
  return NULL;
}

/************************************************************************/
/* fillType stub                                                        */
/************************************************************************/
OdGiFillType OdGiSubEntityTraitsParser::fillType() const
{
  return OdGiFillType();
}

/************************************************************************/
/* lineWeight stub                                                      */
/************************************************************************/
OdDb::LineWeight OdGiSubEntityTraitsParser::lineWeight() const
{
  return OdDb::LineWeight(0);
}

/************************************************************************/
/* lineTypeScale stub                                                   */
/************************************************************************/
double OdGiSubEntityTraitsParser::lineTypeScale() const
{
  return 0;
}

/************************************************************************/
/* thickness stub                                                       */
/************************************************************************/
double OdGiSubEntityTraitsParser::thickness() const
{
  return 0;
}

/************************************************************************/
/* plotStyleNameType stub                                               */
/************************************************************************/
OdDb::PlotStyleNameType OdGiSubEntityTraitsParser::plotStyleNameType() const 
{
  return OdDb::kPlotStyleNameByLayer;
}

/************************************************************************/
/* plotStyleNameId stub                                                 */
/************************************************************************/
OdDbStub* OdGiSubEntityTraitsParser::plotStyleNameId() const 
{
  return 0;
}

/************************************************************************/
/* material stub                                                        */
/************************************************************************/
OdDbStub* OdGiSubEntityTraitsParser::material() const
{
  return 0;
}

/************************************************************************/
/* mapper stub                                                          */
/************************************************************************/
const OdGiMapper* OdGiSubEntityTraitsParser::mapper() const
{
  return 0;
}

/************************************************************************/
/* visualStyle stub                                                     */
/************************************************************************/
OdDbStub* OdGiSubEntityTraitsParser::visualStyle() const
{
  return 0;
}

/************************************************************************/
/* drawFlags stub                                                       */
/************************************************************************/
OdUInt32 OdGiSubEntityTraitsParser::drawFlags() const
{
  return 0;
}

/************************************************************************/
/* selectionGeom stub                                                   */
/************************************************************************/
bool OdGiSubEntityTraitsParser::selectionGeom() const
{
  return false;
}

/************************************************************************/
/* shadowFlags stub                                                     */
/************************************************************************/
OdGiSubEntityTraits::ShadowFlags OdGiSubEntityTraitsParser::shadowFlags() const
{
  return kShadowsCastAndReceive;
}

/************************************************************************/
/* sectionable stub                                                     */
/************************************************************************/
bool OdGiSubEntityTraitsParser::sectionable() const
{
  return false;
}

/************************************************************************/
/* selectionFlags stub                                                  */
/************************************************************************/
OdGiSubEntityTraits::SelectionFlags OdGiSubEntityTraitsParser::selectionFlags() const
{
  return kNoSelectionFlags;
}
