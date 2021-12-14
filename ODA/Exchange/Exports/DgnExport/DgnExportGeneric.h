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

#ifndef _DGN_EXPORT_GENERIC_INCLUDED_ 
#define _DGN_EXPORT_GENERIC_INCLUDED_

#include "DgnExportPE.h"
#include <RxDispatchImpl.h>
#include <RxVariantValue.h>
#include "Gi/GiBaseVectorizer.h"
#include "OdList.h"
#include "StaticRxObject.h"
#include "GiContextForDbDatabase.h"
#include "Gi/GiGeometrySimplifier.h"

namespace TD_DGN_EXPORT 
{

//------------------------------------------------------------------------------

  class OdExportDbObjectThroughDraw :
    public OdGiBaseVectorizer,
    public OdGiContextForDbDatabase,
    public OdGiGeometrySimplifier
  {
  public:
    OdExportDbObjectThroughDraw();
    OdResult explode(const OdDbEntity* pEnt, OdDgDatabase* pDb, OdDbDatabase* db, OdRxObjectPtrArray& entitySet);
    virtual void worldDrawEntity(const OdGiDrawable* pEnt);
    OdDgDatabase *getDgDatabase() { return m_pDgDb.get(); }

    void prepareToDraw(const OdGiDrawable* pDrawable);
    void restoreAfterDraw();
    void setCurrTraitsTo(OdDgElement* pElm);

    // OdGiBaseVectorizer overrides
  //
    OdGiRegenType regenType() const;
    virtual bool doDraw(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable);
    virtual void draw1(const OdGiDrawable* pDrawable, bool bCheckExplodeBeforeTransform);

    virtual void polyline(OdInt32 nbPoints,
      const OdGePoint3d* pVertexList,
      const OdGeVector3d* pNormal = NULL,
      OdGsMarker lBaseSubEntMarker = -1);
    virtual void polygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList);
    virtual void pline(const OdGiPolyline& lwBuf, OdUInt32 fromIndex = 0, OdUInt32 numSegs = 0);
    virtual void xline(const OdGePoint3d&, const OdGePoint3d&);
    virtual void ray(const OdGePoint3d&, const OdGePoint3d&);
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

    // OdGiGeometrySimplifier overrides
    //
    virtual void circleProc(
      const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d* pExtrusion = 0);

    virtual void circleProc(
      const OdGePoint3d&, const OdGePoint3d&, const OdGePoint3d&, const OdGeVector3d* pExtrusion = 0);

    virtual void circularArcProc(
      const OdGePoint3d& center,
      double radius,
      const OdGeVector3d& normal,
      const OdGeVector3d& startVector,
      double sweepAngle,
      OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);

    virtual void circularArcProc(
      const OdGePoint3d& start,
      const OdGePoint3d& point,
      const OdGePoint3d& end,
      OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);

    virtual void meshProc(OdInt32 rows,
      OdInt32 columns,
      const OdGePoint3d* pVertexList,
      const OdGiEdgeData* pEdgeData = NULL,
      const OdGiFaceData* pFaceData = NULL,
      const OdGiVertexData* pVertexData = NULL);

    virtual void shellProc(OdInt32 nbVertex,
      const OdGePoint3d* pVertexList,
      OdInt32 faceListSize,
      const OdInt32* pFaceList,
      const OdGiEdgeData* pEdgeData = NULL,
      const OdGiFaceData* pFaceData = NULL,
      const OdGiVertexData* pVertexData = NULL);

    virtual void rasterImageProc(const OdGePoint3d& origin,
      const OdGeVector3d& u,
      const OdGeVector3d& v,
      const OdGiRasterImage* pImage,
      const OdGePoint2d* uvBoundary,
      OdUInt32 numBoundPts,
      bool transparency = false,
      double brightness = 50.0,
      double contrast = 50.0,
      double fade = 0.0);

    virtual void polylineOut(OdInt32 nbPoints, const OdGePoint3d* pVertexList);
    virtual void facetOut(const OdInt32* p3Vertices, const OdInt32* pEdgeIndices, const OdGeVector3d* pNormal);

    virtual void pushClipBoundary(OdGiClipBoundary* pBoundary);
    virtual void pushClipBoundary(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pClipInfo);
    virtual void popClipBoundary();

    TD_USING(OdGiBaseVectorizer::text);
    TD_USING(OdGiGeometrySimplifier::polylineOut);

  public:

    void getEntityArray(OdRxObjectPtrArray& entitySet) const;

  protected:
    ODRX_USING_HEAP_OPERATORS(OdGiBaseVectorizer);
    virtual void addElement(OdDgElementPtr pEnt, bool needTransform = false);

    virtual void addTransformedCopy(OdDgElementPtr pEnt);

  private:
    OdDgElementPtr makeCircle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal);
    OdDgElementPtr makeCircle3d(const OdGePoint3d& center, double radius, const OdGeVector3d& normal);
    OdDgElementPtr makeCircle2d(const OdGePoint3d& center, double radius, const OdGeVector3d& normal);

    OdDgElementPtr makePolyline(OdInt32 nbPoints, const OdGePoint3d* pVertexList, const OdGeVector3d* pNormal);
    OdDgElementPtr makePolyline2d(OdInt32 nbPoints, const OdGePoint3d* pVertexList, const OdGeVector3d* pNormal);
    OdDgElementPtr makePolyline3d(OdInt32 nbPoints, const OdGePoint3d* pVertexList, const OdGeVector3d* pNormal);

    OdDgElementPtr makeArc2d(const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d& startVector,
      double sweepAngle, OdGiArcType arcType);
    OdDgElementPtr makeArc3d(const OdGePoint3d& center, double radius, const OdGeVector3d& normal, const OdGeVector3d& startVector,
      double sweepAngle, OdGiArcType arcType);

    OdDgElementPtr makePolygonOrGroupedHole(OdInt32 nbPoints, const OdGePoint3d* pVertexList, OdInt32 faceListSize, const OdInt32* pFaceList);
    OdDgElementPtr makeGroupedHole2d(const OdArray<OdGePoint3dArray>& arrFaces, const OdArray<bool>& arrHoleStatus);
    OdDgElementPtr makeGroupedHole3d(const OdArray<OdGePoint3dArray>& arrFaces, const OdArray<bool>& arrHoleStatus);

    OdDgElementPtr makePolygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList);
    OdDgElementPtr makePolygon2d(OdInt32 nbPoints, const OdGePoint3d* pVertexList);
    OdDgElementPtr makePolygon3d(OdInt32 nbPoints, const OdGePoint3d* pVertexList);

    OdDgElementPtr makeText2d(const OdGePoint3d& position, const OdGeVector3d& normal, const OdGeVector3d& direction,
      double height, double width, double oblique, const OdString& msg);
    OdDgElementPtr makeText3d(const OdGePoint3d& position, const OdGeVector3d& normal, const OdGeVector3d& direction,
      double height, double width, double oblique, const OdString& msg);

    OdDgElementPtr makeText2d(const OdGePoint3d& position, const OdGeVector3d& normal, const OdGeVector3d& direction,
      bool raw, const OdGiTextStyle* pTextStyle, OdString& msg);
    OdDgElementPtr makeText3d(const OdGePoint3d& position, const OdGeVector3d& normal, const OdGeVector3d& direction,
      bool raw, const OdGiTextStyle* pTextStyle, OdString& msg);

    const OdGiEdgeData*    m_pEdgeData;
    const OdGiVertexData*  m_pVertexData;
    OdDgMeshFaceLoopsPtr   m_pMesh;
    std::set<OdUInt64>     m_contourPairStartEnd;
    bool                   m_bClipEnabled;
    bool                   m_bAllowToCloneElements;

    TD_USING(OdGiGeometrySimplifier::triangleOut);
  private:
    OdDgDatabasePtr       m_pDgDb;
    OdGiRegenType         m_uRegenType;
    OdGiRegenType         m_uStoredRegenType;
    bool                  m_b3dElement;
    double                m_dLSTransform;
    OdList<OdDgGraphicsElementPtr> m_entityList;
  };

  //------------------------------------------------------------------------------
}
#endif // _DGN_EXPORT_GENERIC_INCLUDED_
