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


#include "Gi/Gi.h"
#include "Gi/GiWorldDraw.h"
#include "Gi/GiWorldGeometry.h"
#include "Ge/GeMatrix3d.h"
#include "StaticRxObject.h"
#include "Ge/GeNurbCurve3d.h"
#include "Gi/GiClipBoundary.h"
#include "CmColor.h"
#include "DbDatabase.h"

/************************************************************************/
/* This class defines functions that allow entities to vectorize        */
/* themselves.                                                          */
/************************************************************************/
class OdGiWorldGeometryParser : public OdGiWorldGeometry
{
public:
  int m_indent;
  OdGiWorldGeometryParser(){}

  void setExtents(
    const OdGePoint3d *pNewExtents);

  void startAttributesSegment();

  virtual OdGeMatrix3d getModelToWorldTransform() const 
    { return OdGeMatrix3d(); }
  
  virtual OdGeMatrix3d getWorldToModelTransform() const 
    { return OdGeMatrix3d(); }
  
  virtual void pushModelTransform(
    const OdGeVector3d& normal);
  
  virtual void pushModelTransform(
    const OdGeMatrix3d& xfm);
  
  virtual void popModelTransform();

  virtual void circle(
    const OdGePoint3d& center, 
    double radius, 
    const OdGeVector3d& normal);
  
  virtual void circle(
    const OdGePoint3d& firstPoint, 
    const OdGePoint3d& secondPoint, 
    const OdGePoint3d& thirdPoint);
  
  virtual void circularArc(const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple);
  
  virtual void circularArc(
    const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType);
  
  virtual void polyline(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = NULL,
    OdGsMarker lBaseSubEntMarker = -1);
  
  virtual void polygon(
    OdInt32 numVertices, 
    const OdGePoint3d* pVertexList);
  
  virtual void pline(
    const OdGiPolyline& polyline, 
    OdUInt32 fromIndex = 0, 
    OdUInt32 numSegs = 0);

  virtual void mesh(OdInt32 numRows,
    OdInt32 numColumns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData = NULL,
    const OdGiFaceData* pFaceData = NULL,
    const OdGiVertexData* pVertexData = NULL);
  
  virtual void shell(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = NULL,
    const OdGiFaceData* pFaceData = NULL,
    const OdGiVertexData* pVertexData = NULL);
  
  virtual void text(
    const OdGePoint3d& position,
    const OdGeVector3d& normal, 
    const OdGeVector3d& direction,
    double height, 
    double width, 
    double oblique, 
    const OdString& msg);
  
  virtual void text(
    const OdGePoint3d& position,
    const OdGeVector3d& normal, 
    const OdGeVector3d& direction,
    const OdChar* msg, 
    OdInt32 length, 
    bool raw, 
    const OdGiTextStyle* pTextStyle);
  
  virtual void xline(
    const OdGePoint3d& firstPoint, 
    const OdGePoint3d& secondPoint);
  
  virtual void ray(
    const OdGePoint3d& basePoint, 
    const OdGePoint3d& throughPoint);
  
  virtual void pline(
    const OdDbPolyline& polyline, 
    OdUInt32 fromIndex = 0, 
    OdUInt32 numSegs = 0);

  virtual void nurbs(
    const OdGeNurbCurve3d& nurbsCurve);
  
  virtual void pushClipBoundary(
    OdGiClipBoundary* pBoundary);
    
  virtual void popClipBoundary();
  
  virtual void draw(
    const OdGiDrawable* pDrawable);
    
  virtual void worldLine(
    const OdGePoint3d points[2]);
  
  virtual void image(
    const OdGiImageBGRA32& img,
    const OdGePoint3d& origin,
    const OdGeVector3d& uVec,
    const OdGeVector3d& vVec,
    OdGiRasterImage::TransparencyMode trpMode = OdGiRasterImage::kTransparency8Bit);

  virtual void edge(
    const OdGiEdge2dArray& edges);

  virtual void body(
    const OdString& body, 
    void **pCacheData = 0);

  virtual void ellipArc(
    const OdGeEllipArc3d& ellipArc,
    const OdGePoint3d* endPointsOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple);

  virtual void ellipArc(
    const OdGePoint3d& center,
    const OdGeVector3d& majorRadius,
    const OdGeVector3d& minorRadius,
    const OdGePoint3d& startPoint,
    const OdGePoint3d& endPoint,
    OdGiArcType arcType = kOdGiArcSimple);
};

/************************************************************************/
/* OdGiSubEntityTraitsParser                                            */
/*                                                                      */
/* An OdGiSubEntityTraits instance is available during calls to         */
/* subWorldDraw and subViewportDraw, so that drawable classes can control     */
/* attributes during the vectorization process. Graphical attribute     */
/* settings are used for all graphical primitives until they are        */
/*                                                                      */
/* Calls to members of OdGiSubEntityTraitsDumper dumped and/or return   */
/* a default value, but are otherwise ignored.                          */
/************************************************************************/
class OdGiSubEntityTraitsParser : public OdGiSubEntityTraits
{
public:
  OdGiSubEntityTraitsParser(){}

  void setColor(OdUInt16 colorIndex);
    
  void setTrueColor(const OdCmEntityColor& trueColor);
  
  void setLayer(OdDbStub* layerId);
  
  void setLineType(OdDbStub* linetypeId);
  
  void setSelectionMarker(OdGsMarker markerId);
  
  void setFillType(OdGiFillType fillType);
  
  void setLineWeight(OdDb::LineWeight lineWeight);
  
  void setLineTypeScale(double lineTypeScale = 1.0);
  
  void setThickness(double thickness);
  
  void setPlotStyleName(OdDb::PlotStyleNameType plotStyleNameType, OdDbStub* plotStyleNameId = 0);
  
  void setMaterial(OdDbStub* materialId);

  void setMapper(const OdGiMapper* pMapper);

  void setVisualStyle(const OdDbStub* visualStyleId);

  void setTransparency(const OdCmTransparency &transparency);

  void setDrawFlags(OdUInt32 drawFlags);

  void setSelectionGeom(bool bSelectionFlag);

  void setShadowFlags(ShadowFlags shadowFlags);

  void setSectionable(bool bSectionableFlag);

  void setSelectionFlags(SelectionFlags selectionFlags);

  OdUInt16 color() const;
  
  OdCmEntityColor trueColor() const 
  { return OdCmEntityColor(); }
  
  OdDbStub* layer() const;
  
  OdDbStub* lineType() const;
  
  OdGiFillType fillType() const;
  
  OdDb::LineWeight lineWeight() const;
  
  double lineTypeScale() const;
  
  double thickness() const;
  
  OdDb::PlotStyleNameType plotStyleNameType() const;
  
  OdDbStub* plotStyleNameId() const;
  
  OdDbStub* material() const;

  const OdGiMapper* mapper() const;

  OdDbStub* visualStyle() const;

  OdCmTransparency transparency() const
  { return OdCmTransparency(); }

  OdUInt32 drawFlags() const;

  bool selectionGeom() const;

  ShadowFlags shadowFlags() const;

  bool sectionable() const;

  SelectionFlags selectionFlags() const;
};

class OdGiContextParser : public OdStaticRxObject<OdGiContext>
{
  OdDbDatabase* m_pDb;

public:
  OdGiContextParser() 
  {}
  
  void setDatabase(OdDbDatabase* pDb)
  {
    m_pDb = pDb;
  }

  OdDbBaseDatabase* database() const 
  {
    return m_pDb;
  }
  
  OdGiDrawablePtr openDrawable(OdDbStub*) { return (OdGiDrawable*)0; }
};

class OdGiWorldDrawParser : public OdStaticRxObject<OdGiWorldDraw>
{
protected:
  OdSmartPtr<OdGiWorldGeometryParser> m_WdGeom;
  OdSmartPtr<OdGiSubEntityTraitsParser> m_Traits;
  
  OdGiContext* pCtx;
public:
	OdGiWorldDrawParser();

  void setIndent(int nIndent);

	OdGiWorldGeometry& geometry() const;

  OdGiRegenType regenType() const;

  bool regenAbort() const;

  OdGiSubEntityTraits& subEntityTraits() const;

  OdGiGeometry& rawGeometry() const;

  bool isDragging() const;

  double deviation(const 
    OdGiDeviationType, 
    const OdGePoint3d& pointOnCurve) const;

  double modelDeviation(
    const OdGiDeviationType type, 
    const OdGePoint3d& modelPoint) const;

  OdUInt32 numberOfIsolines() const;

  void setContext(OdGiContext* pUserContext);

  OdGiContext* context() const;
};

