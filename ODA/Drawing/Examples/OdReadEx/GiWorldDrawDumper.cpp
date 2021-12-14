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
#include "GiWorldDrawDumper.h"
#include "DbDumper.h"
#include "toString.h"

/************************************************************************/
/* GiWorldDrawDumper.cpp: implementation of the GiWorldDraw class.      */
/*                                                                      */
/* The OdGiWorldGeometryDumper and OdGiSubEntityTraitsDumper functions  */
/* are called by pEnt->worldraw() to vectorize *pEnt.                   */ 
/*                                                                      */
/* Since no actual vectorization is being performed, many of the        */
/* methods do nothing but return a default value                        */                                                
/************************************************************************/

/************************************************************************/
/* Implementation of the GiWorldDraw class.                             */
/************************************************************************/
OdGiWorldDrawDumper::OdGiWorldDrawDumper(int indent, OdGiWorldGeometryDumper* geom, OdGiSubEntityTraitsDumper* etd)
{
  if (geom)
    m_WdGeom = geom;
  else
    m_WdGeom = OdRxObjectImpl<OdGiWorldGeometryDumper>::createObject();
  m_WdGeom->m_indent = indent;
  if (etd)
    m_Traits = etd;
  else
    m_Traits = OdRxObjectImpl<OdGiSubEntityTraitsDumper>::createObject();
  m_Traits->m_indent = indent;
}

void OdGiWorldDrawDumper::writeLine(int indent, OdString leftString,
                       OdString rightString, int colWidth )
{
  ::writeLine(indent, leftString, rightString, colWidth);
}
void OdGiSubEntityTraitsDumper::writeLine(int indent, OdString leftString,
                                    OdString rightString, int colWidth )
{
  ::writeLine(indent, leftString, rightString, colWidth);
}
void OdGiWorldGeometryDumper::writeLine(int indent, OdString leftString,
                                    OdString rightString, int colWidth )
{
  ::writeLine(indent, leftString, rightString, colWidth);
}

/************************************************************************/
/* geometry                                                             */
/************************************************************************/
OdGiWorldGeometry& OdGiWorldDrawDumper::geometry() const
{
  return (OdGiWorldGeometry&)*m_WdGeom.get();
}

/************************************************************************/
/* regenType                                                            */
/************************************************************************/
OdGiRegenType OdGiWorldDrawDumper::regenType() const
{
  return kOdGiForExplode;
}

/************************************************************************/
/* regenAbort                                                            */
/************************************************************************/
bool OdGiWorldDrawDumper::regenAbort() const
{
  return false;
}

/************************************************************************/
/* subEntityTraits                                                      */
/************************************************************************/
OdGiSubEntityTraits& OdGiWorldDrawDumper::subEntityTraits() const
{
  return (OdGiSubEntityTraits&)*m_Traits.get();
}

/************************************************************************/
/* rawGeometry                                                          */
/************************************************************************/
OdGiGeometry& OdGiWorldDrawDumper::rawGeometry() const
{
  return (OdGiGeometry&)*m_WdGeom.get();
}

/************************************************************************/
/* isDragging                                                            */
/************************************************************************/
bool OdGiWorldDrawDumper::isDragging() const
{
  return false;
}
  
/************************************************************************/
/* deviation                                                            */
/************************************************************************/
double OdGiWorldDrawDumper::deviation(const OdGiDeviationType, const OdGePoint3d&) const
{
  return 0;
}

/************************************************************************/
/* modelDeviation                                                       */
/************************************************************************/
double OdGiWorldDrawDumper::modelDeviation(const OdGiDeviationType /*type*/, const OdGePoint3d& /*modelPoint*/) const
{
  return 0;
}

/************************************************************************/
/* numberOfIsolines                                                     */
/************************************************************************/
OdUInt32 OdGiWorldDrawDumper::numberOfIsolines() const
{
  OdUInt32 res = 1;
  OdGiContext* pContext = context();
  if (pContext)
  {
    OdDbDatabase* pDataBase = (OdDbDatabase*)pContext->database();
    if (pDataBase)
    {
      res = pDataBase->getISOLINES();
    }
  }
  return res;
}
  
/************************************************************************/
/* setContext                                                           */
/************************************************************************/
void OdGiWorldDrawDumper::setContext(OdGiContext* pUserContext)
{
  pCtx = pUserContext;
}

/************************************************************************/
/* context                                                              */
/************************************************************************/
OdGiContext* OdGiWorldDrawDumper::context() const
{
  return pCtx;
}



/************************************************************************/
/* Implementation of the GiWorldGeometry class.                         */
/* This class defines functions that allow entities to vectorize        */
/* (or dump) themselves                                                 */
/************************************************************************/

/************************************************************************/
/* Set Extents                                                          */
/************************************************************************/
void OdGiWorldGeometryDumper::setExtents(const OdGePoint3d *extents)
{
  writeLine(m_indent,     OD_T("setExtents()"));
  writeLine(m_indent + 1, OD_T("Min Extents"),   toString(extents[0]));
  writeLine(m_indent + 1, OD_T("Max Extents"),   toString(extents[1]));
}

/************************************************************************/
/* Start Attributes Segment                                             */
/************************************************************************/
void OdGiWorldGeometryDumper::startAttributesSegment()
{
  writeLine(m_indent, OD_T("startAttributesSegment()"));
}

/************************************************************************/
/* Push Model Transform                                                 */
/************************************************************************/
void OdGiWorldGeometryDumper::pushModelTransform(const OdGeVector3d& normal)
{
  writeLine(m_indent,     OD_T("pushModelTransform()"));
  writeLine(m_indent + 1, OD_T("normal"),        toString(normal));
}

/************************************************************************/
/* Dump Transformation Matrix                                           */
/************************************************************************/
void OdGiWorldGeometryDumper::dumpXfmMatrix(int indent, const OdString label, const OdGeMatrix3d& xfm)
{
  for (int i = 0; i < 4; i++)
  {
    OdString leftString;
    if (i) 
      leftString = label;
    OdString rightString(OD_T("["));
    for (int j = 0; j < 4 ; j++)
    {
      if (j)
      {
        rightString += OD_T(" ");
      }
      rightString += toString(xfm[i][j]);
    }
    rightString += OD_T("]");
    writeLine(indent,  leftString, rightString);
  }
}
/************************************************************************/
/* Push Model Transform                                                 */
/************************************************************************/
void OdGiWorldGeometryDumper::pushModelTransform(const OdGeMatrix3d& xfm)
{
  writeLine(m_indent,     OD_T("pushModelTransform()"));
  dumpXfmMatrix(m_indent + 1, OD_T("xfm"), xfm);
}

/************************************************************************/
/* Pop Model Transform                                                  */
/************************************************************************/
void OdGiWorldGeometryDumper::popModelTransform()
{
  writeLine(m_indent,     OD_T("popModelTransform()"));
}

/************************************************************************/
/* Body                                                               */
/************************************************************************/
void OdGiWorldGeometryDumper::body(const OdString& body, void **pCacheData)
{
  writeLine(m_indent,     OD_T("body()"));
  writeLine(m_indent + 1, OD_T("body"),          toString(body));
}

/************************************************************************/
/* Circle                                                               */
/************************************************************************/
void OdGiWorldGeometryDumper::circle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal)
{
  writeLine(m_indent,     OD_T("circle()"));
  writeLine(m_indent + 1, OD_T("center"),          toString(center));
  writeLine(m_indent + 1, OD_T("radius"),          toString(radius));
  writeLine(m_indent + 1, OD_T("normal"),          toString(normal));
}
  
/************************************************************************/
/* Circle                                                               */
/************************************************************************/
void OdGiWorldGeometryDumper::circle(const OdGePoint3d& firstPoint, 
    const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint)
{
  writeLine(m_indent,     OD_T("circle()"));
  writeLine(m_indent + 1, OD_T("firstPoint"),       toString(firstPoint));
  writeLine(m_indent + 1, OD_T("secondPoint"),      toString(secondPoint));
  writeLine(m_indent + 1, OD_T("thirdPoint"),       toString(thirdPoint));
}

/************************************************************************/
/* Circular Arc                                                         */
/************************************************************************/
void OdGiWorldGeometryDumper::circularArc(
    const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType)
{
  writeLine(m_indent,     OD_T("circularArc()"));
  writeLine(m_indent + 1, OD_T("center"),          toString(center));
  writeLine(m_indent + 1, OD_T("radius"),          toString(radius));
  writeLine(m_indent + 1, OD_T("normal"),          toString(normal));
  writeLine(m_indent + 1, OD_T("startVector"),     toString(startVector));
  writeLine(m_indent + 1, OD_T("sweepAngle"),      toString(sweepAngle));
  writeLine(m_indent + 1, OD_T("arcType"),         toString(arcType));
}

/************************************************************************/
/* Circular Arc                                                         */
/************************************************************************/
void OdGiWorldGeometryDumper::circularArc(
    const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType)
{
  writeLine(m_indent,     OD_T("circularArc()"));
  writeLine(m_indent + 1, OD_T("firstPoint"),       toString(firstPoint));
  writeLine(m_indent + 1, OD_T("secondPoint"),      toString(secondPoint));
  writeLine(m_indent + 1, OD_T("thirdPoint"),       toString(thirdPoint));
}

/************************************************************************/
/* Elliptical Arc                                                        */
/************************************************************************/
void OdGiWorldGeometryDumper::ellipArc(
    const OdGeEllipArc3d& ellipArc,
    const OdGePoint3d* endPointsOverrides,
    OdGiArcType arcType)
{
  writeLine(m_indent, OD_T("ellipArc()"));
  writeLine(m_indent, OD_T("Center"),                     toString(ellipArc.center()));
  writeLine(m_indent, OD_T("Major Axis"),                 toString(ellipArc.majorAxis()));
  writeLine(m_indent, OD_T("Minor Axis"),                 toString(ellipArc.minorAxis()));
  writeLine(m_indent, OD_T("Major Radius"),               toString(ellipArc.majorRadius()));
  writeLine(m_indent, OD_T("Minor Radius"),               toString(ellipArc.minorRadius()));
  writeLine(m_indent, OD_T("Start Angle"),                toDegreeString(ellipArc.startAng()));
  writeLine(m_indent, OD_T("End Angle"),                  toDegreeString(ellipArc.endAng()));
  writeLine(m_indent, OD_T("Normal"),                     toString(ellipArc.normal()));
  if (endPointsOverrides)
  {
    writeLine(m_indent + 1, OD_T("Start Point Override"),  toString(endPointsOverrides[0]));
    writeLine(m_indent + 1, OD_T("End Point Override"),  toString(endPointsOverrides[0]));
  }
  else
  {
    OdGePoint3d startPoint;
    if (ellipArc.hasStartPoint(startPoint))
    {
      writeLine(m_indent + 1, OD_T("Start Point"),  toString(startPoint));
    }
    OdGePoint3d endPoint;
    if (ellipArc.hasEndPoint(endPoint))
    {
      writeLine(m_indent + 1, OD_T("End Point"),  toString(endPoint));
    }
  }
  
}
/************************************************************************/
/* draw                                                                 */
/************************************************************************/
void OdGiWorldGeometryDumper:: draw(
    const OdGiDrawable* pDrawable) 
{
  writeLine(m_indent, OD_T("draw()"), toString(pDrawable->isA()));
}

/************************************************************************/
/* Elliptical Arc                                                        */
/************************************************************************/
void OdGiWorldGeometryDumper::ellipArc(
    const OdGePoint3d& center,
    const OdGeVector3d& majorRadius,
    const OdGeVector3d& minorRadius,
    const OdGePoint3d& startPoint,
    const OdGePoint3d& endPoint,
    OdGiArcType arcType) 
{
  writeLine(m_indent,     OD_T("ellipArc()"));
  writeLine(m_indent + 1, OD_T("arcType"),      toString(arcType));
  writeLine(m_indent + 1, OD_T("center"),       toString(center));
  writeLine(m_indent + 1, OD_T("majorRadius"),  toString(majorRadius));
  writeLine(m_indent + 1, OD_T("minorRadius"),  toString(minorRadius));
  writeLine(m_indent + 1, OD_T("startPoint"),   toString(startPoint));
  writeLine(m_indent + 1, OD_T("endPoint"),     toString(endPoint));
}

/************************************************************************/
/* Polypoint                                                            */
/************************************************************************/
void OdGiWorldGeometryDumper::polypoint(
    OdInt32 numPoints,
    const OdGePoint3d* vertexList,
    const OdCmEntityColor* pColors,
    const OdCmTransparency* pTransparency,
    const OdGeVector3d* pNormals,
    const OdGsMarker* pSubEntMarkers,
    OdInt32 nPointSize)
{
  writeLine(m_indent,     OD_T("polypoint()"));
  for (int i = 0; i < numPoints; i++)
    writeLine(m_indent + 1, toString(OD_T("vertexList[%d]"), i), toString(vertexList[i]));
  if (pColors)
  {
    for (int i = 0; i < numPoints; i++)
      writeLine(m_indent + 1, toString(OD_T("pColors[%d]"), i), toString(pColors[i]));
  }
  if (pColors)
  {
    for (int i = 0; i < numPoints; i++)
      writeLine(m_indent + 1, toString(OD_T("pTransparency[%d]"), i), toString(pTransparency[i]));
  }
  if (pNormals)
  {
    for (int i = 0; i < numPoints; i++)
      writeLine(m_indent + 1, toString(OD_T("pNormals[%d]"), i), toString(pNormals[i]));
  }
  if (pSubEntMarkers)
  {
    for (int i = 0; i < numPoints; i++)
      writeLine(m_indent + 1, toString(OD_T("pSubEntMarkers[%d]"), i), toString((int)pSubEntMarkers[i]));
  }
  writeLine(m_indent + 1, OD_T("nPointSize"), toString((int)nPointSize));
}

/************************************************************************/
/* PolyPolygon                                                          */
/************************************************************************/
void OdGiWorldGeometryDumper::polyPolygon(
  OdUInt32 numIndices,
  const OdUInt32 *pNumPositions,
  const OdGePoint3d *pPositions,
  const OdUInt32 *pNumPoints,
  const OdGePoint3d *pPoints,
  const OdCmEntityColor *pOutlineColors,
  const OdUInt32 *pOutlinePsLinetypes,
  const OdCmEntityColor *pFillColors,
  const OdCmTransparency *pFillTransparencies)
{
  writeLine(m_indent, OD_T("polyPolygon()"));
  OdUInt32 numPositions = 0, numPoints = 0;
  for (OdUInt32 i = 0; i < numIndices; i++)
    writeLine(m_indent + 1, toString(OD_T("pNumPositions[%d]"), (int)i), toString((int)pNumPositions[i])), numPositions += pNumPositions[i];
  if (pPositions)
  {
    for (OdUInt32 i = 0; i < numPositions; i++)
      writeLine(m_indent + 1, toString(OD_T("pPositions[%d]"), (int)i), toString(pPositions[i]));
  }
  if (pNumPoints)
  {
    for (OdUInt32 i = 0; i < numIndices; i++)
      writeLine(m_indent + 1, toString(OD_T("pNumPoints[%d]"), (int)i), toString((int)pNumPoints[i])), numPoints += pNumPoints[i];
  }
  if (pPoints)
  {
    for (OdUInt32 i = 0; i < numPoints; i++)
      writeLine(m_indent + 1, toString(OD_T("pPoints[%d]"), (int)i), toString(pPoints[i]));
  }
  if (pOutlineColors)
  {
    for (OdUInt32 i = 0; i < numIndices; i++)
      writeLine(m_indent + 1, toString(OD_T("pOutlineColors[%d]"), (int)i), toString(pOutlineColors[i]));
  }
  if (pOutlinePsLinetypes)
  {
    for (OdUInt32 i = 0; i < numIndices; i++)
      writeLine(m_indent + 1, toString(OD_T("pOutlinePsLinetypes[%d]"), (int)i), toString((int)pOutlinePsLinetypes[i]));
  }
  if (pFillColors)
  {
    for (OdUInt32 i = 0; i < numIndices; i++)
      writeLine(m_indent + 1, toString(OD_T("pFillColors[%d]"), (int)i), toString(pFillColors[i]));
  }
  if (pFillTransparencies)
  {
    for (OdUInt32 i = 0; i < numIndices; i++)
      writeLine(m_indent + 1, toString(OD_T("pFillTransparencies[%d]"), (int)i), toString(pFillTransparencies[i]));
  }
}

/************************************************************************/
/* RowOfDots                                                            */
/************************************************************************/
void OdGiWorldGeometryDumper::rowOfDots(
    OdInt32 numPoints,
    const OdGePoint3d& startPoint,
    const OdGeVector3d& dirToNextPoint)
{
  writeLine(m_indent,     OD_T("rowOfDots()"));
  writeLine(m_indent + 1, OD_T("numPoints"),  toString((int)numPoints));
  writeLine(m_indent + 1, OD_T("startPoint"), toString(startPoint));
  writeLine(m_indent + 1, OD_T("dirToNext"),  toString(dirToNextPoint));
}

/************************************************************************/
/* Polyline                                                             */
/************************************************************************/
void OdGiWorldGeometryDumper::polyline(
    OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal,
    OdGsMarker lBaseSubEntMarker)
{
  writeLine(m_indent,     OD_T("polyline()"));
  if (pNormal)
  {
    writeLine(m_indent + 1, OD_T("normal"),            toString(*pNormal));
  }
  writeLine(m_indent + 1, OD_T("lBaseSubEntMarker"), toString((int)lBaseSubEntMarker));
  for (int i = 0; i < numVertices; i++)
  {
    writeLine(m_indent + 1, toString(OD_T("vertexList[%d]"), i), toString(vertexList[i]));
  }
}

/************************************************************************/
/* Polygon                                                              */
/************************************************************************/
void OdGiWorldGeometryDumper::polygon(
    OdInt32 numVertices, 
    const OdGePoint3d* vertexList)
{
  writeLine(m_indent,     OD_T("polygon()"));
  for (int i = 0; i < numVertices; i++)
  {
    writeLine(m_indent + 1, toString(OD_T("vertexList[%d]"), i), toString(vertexList[i]));
  }
}

/************************************************************************/
/* LW Polyline                                                          */
/************************************************************************/
void OdGiWorldGeometryDumper::pline(
    const OdGiPolyline& polyline, 
    OdUInt32 fromIndex, 
    OdUInt32 numSegs)
{
  writeLine(m_indent,     OD_T("pline()"));
  writeLine(m_indent + 1, OD_T("fromIndex"),                  toString((int)fromIndex));
  writeLine(m_indent + 1, OD_T("numSegs"),                    toString((int)numSegs));
  writeLine(m_indent + 1, OD_T("Closed"),                     toString(polyline.isClosed()));
  writeLine(m_indent + 1, OD_T("Has Width"),                  toString(polyline.hasWidth()));
  if (!polyline.hasWidth())
  {
    writeLine(m_indent + 1, OD_T("ConstantWidth"),            toString(polyline.getConstantWidth()));
  }
  writeLine(m_indent + 1, OD_T("Elevation"),                  toString(polyline.elevation()));
  writeLine(m_indent + 1, OD_T("Normal"),                     toString(polyline.normal()));
  writeLine(m_indent + 1, OD_T("Thickness"),                  toString(polyline.thickness()));

  /**********************************************************************/
  /* Dump vertices                                                      */
  /**********************************************************************/
  if (!numSegs)
  {
    numSegs = (int) polyline.numVerts();
  }
  for (int i = (int) fromIndex; i < (int) fromIndex + (int) numSegs; i++)
  { 
    writeLine(m_indent + 1, toString(OD_T("Vertex %d"),i));
    writeLine(m_indent + 2, OD_T("Segment Type"),             toString(polyline.segType(i)));
    OdGePoint2d pt;
    polyline.getPointAt(i, pt);
    writeLine(m_indent + 2, OD_T("Point"), toString(pt));
    if (polyline.hasWidth())
    {     
      double startWidth;
      double endWidth;
      polyline.getWidthsAt(i, startWidth, endWidth);
      writeLine(m_indent + 2, OD_T("Start Width"),            toString(startWidth));
      writeLine(m_indent + 2, OD_T("End Width"),              toString(endWidth));
    }
    writeLine(m_indent + 2, OD_T("Bulge"),                    toString(polyline.getBulgeAt(i)));
    if (polyline.segType(i) == OdGiPolyline::kArc) 
    {
      writeLine(m_indent + 2, OD_T("Bulge Angle"),            toDegreeString(4 * atan(polyline.getBulgeAt(i))));
    }
  }
}

/************************************************************************/
/* WorldLine                                                            */
/************************************************************************/
void OdGiWorldGeometryDumper:: worldLine(const OdGePoint3d points[2])
{
  writeLine(m_indent,     OD_T("worldLine()"));
  writeLine(m_indent + 1, OD_T("points[0]"),           toString(points[0]));
  writeLine(m_indent + 1, OD_T("points[1]"),           toString(points[1]));
}

/************************************************************************/
/* BGRAImage                                                            */
/************************************************************************/
void OdGiWorldGeometryDumper::image(
    const OdGiImageBGRA32& /*img*/,
    const OdGePoint3d& origin,
    const OdGeVector3d& uVec,
    const OdGeVector3d& vVec,
    OdGiRasterImage::TransparencyMode /*trpMode*/)
{
  writeLine(m_indent,     OD_T("image()"));
  writeLine(m_indent + 1, OD_T("origin"), toString(origin));
  writeLine(m_indent + 1, OD_T("uVec"),   toString(uVec));
  writeLine(m_indent + 1, OD_T("vVec"),   toString(vVec));
}

/************************************************************************/
/* Edges                                                                */
/************************************************************************/
void OdGiWorldGeometryDumper::edge(
  const OdGiEdge2dArray& edges)
{
  writeLine(m_indent, OD_T("edge()"));
  writeLine(m_indent + 1, OD_T("numCurves"), toString((int)edges.size()));
}

/************************************************************************/
/* Dump Additional Vertex Data                                          */
/************************************************************************/
void OdGiWorldGeometryDumper::dumpVertexData(
    const OdGiVertexData* pVertexData, 
    const int numVertices)
{
  if (!pVertexData)
  {
    return;
  }
  writeLine(m_indent + 1, OD_T("Additional Vertex Data"));
  writeLine(m_indent + 2, OD_T("Orientation"), toString(pVertexData->orientationFlag()));
  for (int i = 0; i < numVertices; i++)
  {
     writeLine(m_indent + 2, toString(OD_T("Vertex %d"), i));
     if (pVertexData->normals())
     {
        writeLine(m_indent + 3, OD_T("Normal"), toString(pVertexData->normals()[i]));
     }
     if (pVertexData->trueColors())
     {
        writeLine(m_indent + 3, OD_T("TrueColor"), toString(pVertexData->trueColors()[i]));
     }
  }
}

/************************************************************************/
/* Dump Additional Face Data                                            */
/************************************************************************/
void OdGiWorldGeometryDumper::dumpFaceData(
    const OdGiFaceData* pFaceData, 
    const int numFaces)
{
  if (!pFaceData)
  {
    return;
  }
  writeLine(m_indent + 1, OD_T("Additional Face Data"));
  for (int i = 0; i < numFaces; i++)
  {
     writeLine(m_indent + 2, toString(OD_T("Face %d"), i));
     if (pFaceData->colors())
     {
        writeLine(m_indent + 3, OD_T("Color"), toString(pFaceData->colors()[i]));
     }
     if (pFaceData->layerIds())
     {
        writeLine(m_indent + 3, OD_T("Layer"), toString((OdDbObjectId)pFaceData->layerIds()[i]));
     }
     if (pFaceData->normals())
     {
        writeLine(m_indent + 3, OD_T("Normal"), toString(pFaceData->normals()[i]));
     }
     if (pFaceData->trueColors())
     {
        writeLine(m_indent + 3, OD_T("TrueColor"), toString(pFaceData->trueColors()[i]));
     }
     if (pFaceData->visibility())
     {
        writeLine(m_indent + 3, OD_T("Visibility"), toOdGiVisibilityString(pFaceData->visibility()[i]));
     }
  }
}
/************************************************************************/
/* Dump Additional Edge Data                                            */
/************************************************************************/
void OdGiWorldGeometryDumper::dumpEdgeData(
    const OdGiEdgeData* pEdgeData, 
    const int numEdges)
{
  if (!pEdgeData)
  {
    return;
  }
  writeLine(m_indent + 1, OD_T("Additional Edge Data"));
  for (int i = 0; i < numEdges; i++)
  {
     writeLine(m_indent + 2, toString(OD_T("Edge %d"), i));
     if (pEdgeData->colors())
     {
        writeLine(m_indent + 3, OD_T("Color"), toString(pEdgeData->colors()[i]));
     }
     if (pEdgeData->layerIds())
     {
        writeLine(m_indent + 3, OD_T("Layer"), toString((OdDbObjectId)pEdgeData->layerIds()[i]));
     }
     if (pEdgeData->trueColors())
     {
        writeLine(m_indent + 3, OD_T("TrueColor"), toString(pEdgeData->trueColors()[i]));
     }
     if (pEdgeData->visibility())
     {
        writeLine(m_indent + 3, OD_T("Visibility"), toOdGiVisibilityString(pEdgeData->visibility()[i]));
     }
  }
}

/************************************************************************/
/* Mesh                                                                 */
/************************************************************************/
void OdGiWorldGeometryDumper::mesh(
    OdInt32 numRows,
    OdInt32 numColumns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData)
{
  writeLine(m_indent,     OD_T("mesh()"));
  writeLine(m_indent + 1, OD_T("numRows"),                    toString((int)numRows));
  writeLine(m_indent + 1, OD_T("numColumns"),                 toString((int)numColumns));
  /**********************************************************************/
  /* Dump vertices                                                      */
  /**********************************************************************/
  for (int i = 0; i < numRows * numColumns; i++)
  {
    writeLine(m_indent + 1, toString(OD_T("Vertex %d"), i), toString(vertexList[i]));
  }
  dumpVertexData(pVertexData, numRows * numColumns);
  dumpFaceData(pFaceData, (numRows - 1) * (numColumns - 1));
  dumpEdgeData(pEdgeData, ((numRows - 1) * numColumns) + (numRows * (numColumns - 1)));
}

/************************************************************************/
/* Shell                                                                */
/************************************************************************/
void OdGiWorldGeometryDumper::shell(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData)
{
  writeLine(m_indent,     OD_T("shell()"));
  writeLine(m_indent + 1, OD_T("numVertices"),                toString((int)numVertices));
  writeLine(m_indent + 1, OD_T("faceListSize"),               toString((int)faceListSize));

  /**********************************************************************/
  /* Dump vertices                                                      */
  /**********************************************************************/
  int i = 0;
  for (; i < numVertices; i++)
  {
    writeLine(m_indent + 1, toString(OD_T("Vertex %d"), i), toString(vertexList[i]));
  }
  
  /**********************************************************************/
  /* Dump faces and holes                                               */
  /**********************************************************************/
  int numEdges = 0;
  int numFaces = 0;
  int numHoles = 0;
  for (i=0; i < faceListSize;)
  {
    int numVerts = faceList[i++];
    bool isHole = numVerts < 0;
    numEdges += abs(numVerts);
    
    OdString face(OD_T("{"));
    for (int j = 0; j < abs(numVerts); j++, i++)
    {
      if (j) 
      {
        face += OD_T(" ");
      }
      face += toString((int)faceList[i]);
    }
    face += OD_T("}");
    if (isHole)
    {
      writeLine(m_indent + 1, toString(OD_T("Hole %d"), numHoles++), face);
    }
    else
    {
      writeLine(m_indent + 1, toString(OD_T("Face %d"), numFaces++), face);
    }
  }
  
  dumpVertexData(pVertexData, numVertices);
  dumpFaceData(pFaceData, numFaces);
  dumpEdgeData(pEdgeData, numEdges);
}

/************************************************************************/
/* Text                                                                 */
/************************************************************************/
void OdGiWorldGeometryDumper::text(
    const OdGePoint3d& position,
    const OdGeVector3d& normal, 
    const OdGeVector3d& direction,
    double height, 
    double width, 
    double oblique, 
    const OdString& msg)
{
  writeLine(m_indent,     OD_T("text()"));
  writeLine(m_indent + 1, OD_T("position"),    toString(position));
  writeLine(m_indent + 1, OD_T("normal"),      toString(normal));
  writeLine(m_indent + 1, OD_T("direction"),   toString(direction));
  writeLine(m_indent + 1, OD_T("height"),      toString(height));
  writeLine(m_indent + 1, OD_T("width"),       toString(width));
  writeLine(m_indent + 1, OD_T("oblique"),     toDegreeString(oblique));
  writeLine(m_indent + 1, OD_T("msg"),         toString(msg));
}

/************************************************************************/
/* Text                                                                 */
/************************************************************************/
void OdGiWorldGeometryDumper::text(
    const OdGePoint3d& position,
    const OdGeVector3d& normal, 
    const OdGeVector3d& direction,
    const OdChar* msg, 
    OdInt32 length, 
    bool raw, 
    const OdGiTextStyle* pTextStyle)
{
  // msg may not be null-terminated.
  OdString buffer;
  for (int i = 0; msg[i] && (i < length) ; i++)
  {
    buffer += msg[i];
  }
  writeLine(m_indent,     OD_T("text()"));
  writeLine(m_indent + 1, OD_T("position"),    toString(position));
  writeLine(m_indent + 1, OD_T("normal"),      toString(normal));
  writeLine(m_indent + 1, OD_T("direction"),   toString(direction));
  writeLine(m_indent + 1, OD_T("msg"),         toString(buffer));
  writeLine(m_indent + 1, OD_T("length"),      toString((int)length));
  writeLine(m_indent + 1, OD_T("raw"),         toString(raw));
  writeLine(m_indent + 1, OD_T("Text Style"));
  OdString typeface;
  bool bold;
  bool italic;
  int charset;
  int pitchAndFamily;
  pTextStyle->font(typeface, bold, italic, charset, pitchAndFamily); 
  writeLine(m_indent + 2, OD_T("Typeface"),               toString(typeface));
  writeLine(m_indent + 2, OD_T("Character Set"),          toString(charset));
  writeLine(m_indent + 2, OD_T("Bold"),                   toString(bold));
  writeLine(m_indent + 2, OD_T("Italic"),                 toString(italic));
  writeLine(m_indent + 2, OD_T("Font Pitch and Family"),  toHexString(pitchAndFamily));
}

/************************************************************************/
/* Xline                                                                */
/************************************************************************/
void OdGiWorldGeometryDumper::xline(
    const OdGePoint3d& firstPoint, 
    const OdGePoint3d& secondPoint)
{
  writeLine(m_indent,     OD_T("xline()"));
  writeLine(m_indent + 1, OD_T("firstPoint"),    toString(firstPoint));
  writeLine(m_indent + 1, OD_T("secondPoint"),   toString(secondPoint));
}

/************************************************************************/
/* Ray                                                                 */
/************************************************************************/
void OdGiWorldGeometryDumper::ray(
    const OdGePoint3d& basePoint, 
    const OdGePoint3d& throughPoint)
{
  writeLine(m_indent,     OD_T("ray()"));
  writeLine(m_indent + 1, OD_T("basePoint"),    toString(basePoint));
  writeLine(m_indent + 1, OD_T("throughPoint"),   toString(throughPoint));
}

/************************************************************************/
/* LW Polyline                                                          */
/************************************************************************/
void OdGiWorldGeometryDumper::pline(
    const OdDbPolyline& polyline, 
    OdUInt32 fromIndex, 
    OdUInt32 numSegs)
{
  writeLine(m_indent,     OD_T("pline()"));
  writeLine(m_indent + 1, OD_T("fromIndex"),                  toString((int)fromIndex));
  writeLine(m_indent + 1, OD_T("numSegs"),                    toString((int)numSegs));
  writeLine(m_indent + 1, OD_T("Closed"),                     toString(polyline.isClosed()));
  writeLine(m_indent + 1, OD_T("Has Width"),                  toString(polyline.hasWidth()));
  if (polyline.hasWidth())
  {
    writeLine(m_indent + 1, OD_T("ConstantWidth"),            toString(polyline.getConstantWidth()));
  }
  writeLine(m_indent + 1, OD_T("Elevation"),                  toString(polyline.elevation()));
  writeLine(m_indent + 1, OD_T("Normal"),                     toString(polyline.normal()));
  writeLine(m_indent + 1, OD_T("Thickness"),                  toString(polyline.thickness()));

  /**********************************************************************/
  /* Dump vertices                                                      */
  /**********************************************************************/
  if (!numSegs)
  {
    numSegs = (int) polyline.numVerts();
  }
  for (int i = (int) fromIndex; i < (int) fromIndex + (int) numSegs; i++)
  { 
    writeLine(m_indent + 1, toString(OD_T("Vertex %d"),i));
    writeLine(m_indent + 2, OD_T("Segment Type"),             toString(polyline.segType(i)));
    OdGePoint2d pt;
    polyline.getPointAt(i, pt);
    writeLine(m_indent + 2, OD_T("Point"), toString(pt));
    if (polyline.hasWidth())
    {     
      double startWidth;
      double endWidth;
      polyline.getWidthsAt(i, startWidth, endWidth);
      writeLine(m_indent + 2, OD_T("Start Width"),            toString(startWidth));
      writeLine(m_indent + 2, OD_T("End Width"),              toString(endWidth));
    }
    writeLine(m_indent + 2, OD_T("Bulge"),                    toString(polyline.getBulgeAt(i)));
    if (polyline.segType(i) == OdDbPolyline::kArc) 
    {
      writeLine(m_indent + 2, OD_T("Bulge Angle"),            toDegreeString(4 * atan(polyline.getBulgeAt(i))));
    }
  }
}
/************************************************************************/
/* Nurbs curve                                                          */
/************************************************************************/
void OdGiWorldGeometryDumper::nurbs(const OdGeNurbCurve3d& nurbsCurve)
{
  writeLine(m_indent,     OD_T("nurbs()"));

  int degree;
  bool rational, periodic;
  OdGeKnotVector  knots;
  OdGePoint3dArray controlPoints;
  OdGeDoubleArray weights;

  nurbsCurve.getDefinitionData(degree, rational, periodic, knots, controlPoints, weights);
  writeLine(m_indent + 1, OD_T("Degree"),                   toString(degree));
  writeLine(m_indent + 1, OD_T("Rational"),                 toString(rational));
  writeLine(m_indent + 1, OD_T("Periodic"),                 toString(periodic));

  writeLine(m_indent + 1, OD_T("Number of control points"),        toString((int) controlPoints.size()));
  int i;
  for (i = 0; i < (int) controlPoints.size(); i++)
  {
    writeLine(m_indent + 1, toString(OD_T("Control Point %d"), i),  toString(controlPoints[i]));
  }

  writeLine(m_indent + 1, OD_T("Number of Knots"),                  toString((int) knots.length()));
  for (i = 0; i < (int) knots.length(); i++)
  {
    writeLine(m_indent + 1, toString(OD_T("Knot %d"), i),           toString(knots[i]));
  }

  if (rational) 
  {
    writeLine(m_indent + 1, OD_T("Number of Weights"),              toString((int) weights.size()));
    for (i = 0; i < (int) weights.size(); i++)
    {
      writeLine(m_indent + 1, toString(OD_T("Weight %d"), i),       toString(weights[i]));
    }
  }
}

/************************************************************************/
/* Clip Boundary                                                        */
/************************************************************************/
void OdGiWorldGeometryDumper::pushClipBoundary(OdGiClipBoundary* pBoundary)
{
  writeLine(m_indent,     OD_T("pushClipBoundary()"));
  writeLine(m_indent + 1, OD_T("Normal"),                           toString(pBoundary->m_vNormal));
  writeLine(m_indent + 1, OD_T("Origin"),                           toString(pBoundary->m_ptPoint));
  writeLine(m_indent + 1, OD_T("Clip Boundary"));
  for (int i = 0; i < (int) pBoundary->m_Points.size(); i++)
  {
    writeLine(m_indent + 2, toString(OD_T("Point %d"), i),          toString(pBoundary->m_Points[i]));
  }
  dumpXfmMatrix(m_indent + 1, OD_T("Model Space to Clip Space"),          pBoundary->m_xToClipSpace);
  dumpXfmMatrix(m_indent + 1, OD_T("Block Space to Clip Space"),          pBoundary->m_xInverseBlockRefXForm);
  
  writeLine(m_indent + 1, OD_T("Front Clip Distance"),              toString(pBoundary->m_dFrontClipZ));
  writeLine(m_indent + 1, OD_T("Back Clip Distance"),               toString(pBoundary->m_dBackClipZ));
  writeLine(m_indent + 1, OD_T("Front Clip Enabled"),               toString(pBoundary->m_bClippingFront));
  writeLine(m_indent + 1, OD_T("DrawBoundary"),                     toString(pBoundary->m_bDrawBoundary));
  writeLine(m_indent + 1, OD_T("Back Clip Enabled"),                toString(pBoundary->m_bClippingBack));
}

void OdGiWorldGeometryDumper::popClipBoundary()
{
  writeLine(m_indent,     OD_T("popClipBoundary()"));
}

/************************************************************************/
/* OdGiSubEntityTraitsDumper                                            */
/*                                                                      */
/* An OdGiSubEntityTraits instance is available during calls to         */
/* worldDraw and viewportDraw, so that drawable classes can control     */
/* attributes during the vectorization process. Graphical attribute     */
/* settings are used for all graphical primitives until they are        */
/*                                                                      */
/* Calls to members of OdGiSubEntityTraitsDumper dumped and/or return   */
/* a default value, but are otherwise ignored.                          */
/************************************************************************/

/************************************************************************/
/* setColor                                                             */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setColor(OdUInt16 colorIndex)
{
  writeLine(m_indent,     OD_T("setColor()"));
  writeLine(m_indent + 1, OD_T("Color Index"), toString(colorIndex));
}

/************************************************************************/
/* setTrueColor                                                         */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setTrueColor(const OdCmEntityColor& trueColor)
{
  writeLine(m_indent,     OD_T("setTrueColor()"));
  writeLine(m_indent + 1, OD_T("TrueColor"), toString(trueColor));
}

/************************************************************************/
/* setLayer                                                             */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setLayer(OdDbStub* layerId)
{
  writeLine(m_indent,     OD_T("setLayer()"));
  writeLine(m_indent + 1, OD_T("Layer"), toString((OdDbObjectId)layerId));
}
/************************************************************************/
/* setLineType                                                          */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setLineType(OdDbStub* linetypeId)
{
  writeLine(m_indent,     OD_T("setLineType()"));
  writeLine(m_indent + 1, OD_T("Linetype"), toString((OdDbObjectId) linetypeId));
}

/************************************************************************/
/* setSelectionMarker                                                   */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setSelectionMarker(OdGsMarker markerId)
{
  writeLine(m_indent,     OD_T("setSelectionMarker()"));
  writeLine(m_indent + 1, OD_T("markerId"), toString((int)markerId));
}

/************************************************************************/
/* setFillType                                                          */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setFillType(OdGiFillType fillType)
{
  writeLine(m_indent,     OD_T("setFillType()"));
  writeLine(m_indent + 1, OD_T("fillType"), toString(fillType));
}

/************************************************************************/
/* setLineWeight                                                        */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setLineWeight(OdDb::LineWeight lineWeight)
{
  writeLine(m_indent,     OD_T("setLineWeight()"));
  writeLine(m_indent + 1, OD_T("lineWeight"), toString(lineWeight));
}

/************************************************************************/
/* setLineTypeScale                                                     */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setLineTypeScale(double lineTypeScale)
{
  writeLine(m_indent,     OD_T("setLineTypeScale()"));
  writeLine(m_indent + 1, OD_T("lineTypeScale"), toString(lineTypeScale));
}

/************************************************************************/
/* setThickness                                                         */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setThickness(double thickness)
{
  writeLine(m_indent,     OD_T("setThickness()"));
  writeLine(m_indent + 1, OD_T("thickness"), toString(thickness));
}

/************************************************************************/
/* setPlotStyleName                                                     */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setPlotStyleName(OdDb::PlotStyleNameType plotStyleNameType, OdDbStub* plotStyleNameId )
{
  writeLine(m_indent,     OD_T("setPlotStyleName()"));
  writeLine(m_indent + 1, OD_T("plotStyleNameType"), toString(plotStyleNameType));
  if (plotStyleNameType == OdDb::kPlotStyleNameById)
  {
    writeLine(m_indent + 1, OD_T("Plot Style Name"), toString((OdDbObjectId)plotStyleNameId));
  }
}

/************************************************************************/
/* setMaterial                                                          */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setMaterial(OdDbStub* materialId)
{
  writeLine(m_indent,     OD_T("setMaterial()"));
  writeLine(m_indent + 1, OD_T("setMaterial"), toString((OdDbObjectId)materialId));
}

/************************************************************************/
/* setMapper                                                            */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setMapper(const OdGiMapper* pMapper)
{
  writeLine(m_indent,     OD_T("setMapper()"));
  writeLine(m_indent + 1, OD_T("mapperEnabled"), toString(pMapper != 0));
  if (pMapper != 0)
  {
    writeLine(m_indent + 2, OD_T("projection"), toString(pMapper->projection()));
    writeLine(m_indent + 2, OD_T("uTiling"),    toString(pMapper->uTiling()));
    writeLine(m_indent + 2, OD_T("vTiling"),    toString(pMapper->vTiling()));
    writeLine(m_indent + 2, OD_T("autoTransform"), toString(pMapper->autoTransform()));
    const OdGeMatrix3d &transform = pMapper->transform();
    writeLine(m_indent + 2, OD_T("transform"),  toString(OdGeQuaternion(transform[0][0], transform[0][1], transform[0][2], transform[0][3])));
    writeLine(m_indent + 2, OD_T("transform"),  toString(OdGeQuaternion(transform[1][0], transform[1][1], transform[1][2], transform[1][3])));
    writeLine(m_indent + 2, OD_T("transform"),  toString(OdGeQuaternion(transform[2][0], transform[2][1], transform[2][2], transform[2][3])));
    writeLine(m_indent + 2, OD_T("transform"),  toString(OdGeQuaternion(transform[3][0], transform[3][1], transform[3][2], transform[3][3])));
  }
}

/************************************************************************/
/* setVisualStyle                                                       */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setVisualStyle(const OdDbStub* visualStyleId)
{
  writeLine(m_indent,     OD_T("setVisualStyle()"));
  writeLine(m_indent + 1, OD_T("setVisualStyle"), toString((OdDbObjectId)const_cast<OdDbStub*>(visualStyleId)));
}

/************************************************************************/
/* setTransparency                                                      */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setTransparency(const OdCmTransparency &transparency)
{
  writeLine(m_indent,     OD_T("setTransparency()"));
  writeLine(m_indent + 1, OD_T("setVisualStyle"), toString(transparency));
}

/************************************************************************/
/* setDrawFlags                                                         */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setDrawFlags(OdUInt32 drawFlags)
{
  writeLine(m_indent,     OD_T("setDrawFlags()"));
  writeLine(m_indent + 1, OD_T("setDrawFlags"), OdString().format(OD_T("0x%X"), drawFlags));
}

/************************************************************************/
/* setSelectionGeom                                                     */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setSelectionGeom(bool bSelectionFlag)
{
  writeLine(m_indent,     OD_T("setSelectionGeom()"));
  writeLine(m_indent + 1, OD_T("setSelectionGeom"), toString(bSelectionFlag));
}

/************************************************************************/
/* setShadowFlags                                                       */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setShadowFlags(OdGiSubEntityTraits::ShadowFlags shadowFlags)
{
  writeLine(m_indent,     OD_T("setShadowFlags()"));
  writeLine(m_indent + 1, OD_T("setShadowFlags"), toString(shadowFlags));
}

/************************************************************************/
/* setSectionable                                                       */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setSectionable(bool bSectionableFlag)
{
  writeLine(m_indent,     OD_T("setSectionable()"));
  writeLine(m_indent + 1, OD_T("setSectionable"), toString(bSectionableFlag));
}

/************************************************************************/
/* setSelectionFlags                                                    */
/************************************************************************/
void OdGiSubEntityTraitsDumper::setSelectionFlags(OdGiSubEntityTraits::SelectionFlags selectionFlags)
{
  writeLine(m_indent,     OD_T("setSelectionFlags()"));
  writeLine(m_indent + 1, OD_T("setSelectionFlags"), toString(selectionFlags));
}

/************************************************************************/
/* color stub                                                           */
/************************************************************************/
OdUInt16 OdGiSubEntityTraitsDumper::color() const
{
  return 0;
}

/************************************************************************/
/* layer stub                                                           */
/************************************************************************/
OdDbStub* OdGiSubEntityTraitsDumper::layer() const
{
  return NULL;
}

/************************************************************************/
/* lineType stub                                                        */
/************************************************************************/
OdDbStub* OdGiSubEntityTraitsDumper::lineType() const
{
  return NULL;
}

/************************************************************************/
/* fillType stub                                                        */
/************************************************************************/
OdGiFillType OdGiSubEntityTraitsDumper::fillType() const
{
  return OdGiFillType();
}

/************************************************************************/
/* lineWeight stub                                                      */
/************************************************************************/
OdDb::LineWeight OdGiSubEntityTraitsDumper::lineWeight() const
{
  return OdDb::LineWeight(0);
}

/************************************************************************/
/* lineTypeScale stub                                                   */
/************************************************************************/
double OdGiSubEntityTraitsDumper::lineTypeScale() const
{
  return 0;
}

/************************************************************************/
/* thickness stub                                                       */
/************************************************************************/
double OdGiSubEntityTraitsDumper::thickness() const
{
  return 0;
}

/************************************************************************/
/* plotStyleNameType stub                                               */
/************************************************************************/
OdDb::PlotStyleNameType OdGiSubEntityTraitsDumper::plotStyleNameType() const 
{
  return OdDb::kPlotStyleNameByLayer;
}

/************************************************************************/
/* plotStyleNameId stub                                                 */
/************************************************************************/
OdDbStub* OdGiSubEntityTraitsDumper::plotStyleNameId() const 
{
  return 0;
}

/************************************************************************/
/* material stub                                                        */
/************************************************************************/
OdDbStub* OdGiSubEntityTraitsDumper::material() const
{
  return 0;
}

/************************************************************************/
/* mapper stub                                                          */
/************************************************************************/
const OdGiMapper* OdGiSubEntityTraitsDumper::mapper() const
{
  return 0;
}

/************************************************************************/
/* visualStyle stub                                                     */
/************************************************************************/
OdDbStub* OdGiSubEntityTraitsDumper::visualStyle() const
{
  return 0;
}

/************************************************************************/
/* drawFlags stub                                                       */
/************************************************************************/
OdUInt32 OdGiSubEntityTraitsDumper::drawFlags() const
{
  return 0;
}

/************************************************************************/
/* selectionGeom stub                                                   */
/************************************************************************/
bool OdGiSubEntityTraitsDumper::selectionGeom() const
{
  return false;
}

/************************************************************************/
/* shadowFlags stub                                                     */
/************************************************************************/
OdGiSubEntityTraits::ShadowFlags OdGiSubEntityTraitsDumper::shadowFlags() const
{
  return kShadowsCastAndReceive;
}

/************************************************************************/
/* sectionable stub                                                     */
/************************************************************************/
bool OdGiSubEntityTraitsDumper::sectionable() const
{
  return false;
}

/************************************************************************/
/* selectionFlags stub                                                  */
/************************************************************************/
OdGiSubEntityTraits::SelectionFlags OdGiSubEntityTraitsDumper::selectionFlags() const
{
  return kNoSelectionFlags;
}
