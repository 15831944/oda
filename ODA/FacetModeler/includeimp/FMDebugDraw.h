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

#if !defined(DEBUG_DRAW_INCLUDED)
#define DEBUG_DRAW_INCLUDED

//#define DEBUG_DRAW_ENABLED // Drawing must be built
#if !defined(_DEBUG) && defined(DEBUG_DRAW_ENABLED)
#undef DEBUG_DRAW_ENABLED
#endif
#if defined(_DEBUG) && defined(DEBUG_DRAW_ENABLED)
#pragma comment(lib, "TD_DbRoot.lib")
#pragma comment(lib, "TD_Db.lib")
#pragma comment(lib, "TD_Gi.lib")
#pragma comment(lib, "TD_DbIO.lib")
#endif

#if defined(DEBUG_DRAW_ENABLED)
#include "RemoteGeomProxy.h"
#include "CmColor.h"
#include "FMGiTools.h"
//#include "Gi/AECGi.h"
#include <Gi/GiGeometry.h>
#include <Gi/GiCommonDraw.h>
#include "Ge/GeTrMeshSimplification.h"
#include "../Extensions/ExServices/ExHostAppServices.h"
#include "AbstractViewPE.h"
#else
class OdRemoteGeomProxy;
class OdCmEntityColor;
class OdCmColor;
#endif
#include "FMProfile2D.h"
#include "FMContour2D.h"
#include "FMContour3D.h"
//#include "FMDrawBody.h"
#include "FMProfile3D.h"
#include <Modeler/FMMdlBody.h>
#include <Modeler/FMMdlIterators.h>
#include "Modeler/FMMdlSurface.h"
#include <FMGeometry.h>
#include <Int32Array.h>

//#include "StaticRxObject.h"
//#include "DbEntity/AECDbMassElem.h"
#include "Ge/GeExtents2d.h"
#include "Ge/GeLineSeg3d.h"
//#include "DbBlockReference.h"
#include <vector>


using namespace FacetModeler;

// colors are indices to OdCmEntityColor
struct DbgColor
{
  static const OdUInt16 clear = 0;
  static const OdUInt16 red = 1;
  static const OdUInt16 yellow = 2;
  static const OdUInt16 green = 3;
  static const OdUInt16 cyan = 4;
  static const OdUInt16 blue = 5;
  static const OdUInt16 magenta = 6;
  static const OdUInt16 white = 7;
};


template<typename T>
class  TAecAutoColor
{
  T m_uColor;

public:
  TAecAutoColor()
  {
    m_uColor = 0;
  }

  T color()
  {
    T nRet = m_uColor;

    m_uColor++;
    if(m_uColor > 6)
      m_uColor = 0;

    return nRet;
  }

  T peekColor() const { return m_uColor; }
};

typedef TAecAutoColor<OdUInt16> AecAutoColor;

#if defined(DEBUG_DRAW_ENABLED)
// Do not change this initialization of the g_DEBUG_DRAW_ENABLED.
// Set it to 'true' either in debugger or in sources exactly before DEBUG_DRAW() functions calls
static bool g_DEBUG_DRAW_ENABLED = true;

template <class TSegment2d>
inline void _debug_draw( OdGiWorldGeometry& geom, const TSegment2d& rSeg ) {
  if( OdZero( rSeg.bulge() ) || rSeg.type() == FacetModeler::estCoincident ) 
  {
    OdGePoint2d pts[2];
    pts[0] = rSeg.startPt();
    pts[1] = rSeg.endPt();
    OdGiTools::polyline2d( geom, 2, pts );
  }
  else 
  {
    //double dBulge = rSeg.bulge();
    //FacetModeler::SegmentType st = rSeg.type();
    
    OdGeCircArc2d arc2d;
    rSeg.getArcSeg( arc2d );
    OdGiTools::arc2d( geom, arc2d );
  }
}

template <class TSegment2d>
inline void DEBUG_DRAW(const TSegment2d& rSeg, OdInt16 color) {
  if( g_DEBUG_DRAW_ENABLED ) {
    OdRemoteGeomProxy proxy;
    OdRemoteGeomProxyAutoBlock rAuto(proxy);

    proxy.begin();
    proxy.traits().setColor( color );
    _debug_draw( proxy.wGeom(), rSeg );
    proxy.end();
  }
}



inline void DEBUG_DRAW3(OdRemoteGeomProxy& proxy, const FacetModeler::Contour2D& contour, OdInt16 color) 
{
  proxy.traits().setColor( color );

  double dBulge = 0;

  OdUInt32 nSegments = contour.numSegments();
  for ( OdUInt32 i = 0; i < nSegments; ++i )
  {
    contour.getBulgeAt( i, dBulge );
    if( OdZero( dBulge ) ) {
      OdGeLineSeg2d line;
      contour.getLineSegAt( i, line );
      OdGiTools::lineSeg2d( proxy.wGeom(), line, i );
    }
    else if(contour.segmentType(i) == FacetModeler::estArc) {
      OdGeCircArc2d arc2d;
      contour.getArcSegAt( i, arc2d );
      OdGiTools::arc2d( proxy.wGeom(), arc2d, i );
    }
  }
}

inline void DEBUG_DRAW( const FacetModeler::Contour2D& contour, OdInt16 color) {
  if( g_DEBUG_DRAW_ENABLED ) {
    OdRemoteGeomProxy proxy;
    OdRemoteGeomProxyAutoBlock rAuto(proxy);

    proxy.begin();
    DEBUG_DRAW3( proxy, contour, color );
    proxy.end();
  }
}

inline void DEBUG_DRAW( const FacetModeler::IContour2D& contour, OdInt16 color) 
{
  if( !g_DEBUG_DRAW_ENABLED ) 
    return;

  FacetModeler::Contour2D c2d ( contour );

  OdRemoteGeomProxy proxy;
  OdRemoteGeomProxyAutoBlock rAuto(proxy);

  proxy.begin();
  DEBUG_DRAW3( proxy, c2d, color );
  proxy.end();
}

inline void DEBUG_DRAW(const FacetModeler::Contour3D& contour, OdInt16 color ) {
  if( g_DEBUG_DRAW_ENABLED ) {
    OdRemoteGeomProxy proxy;
    OdRemoteGeomProxyAutoBlock rAuto(proxy);

    proxy.begin();
    proxy.wGeom().pushModelTransform( contour.transform() );
    DEBUG_DRAW3( proxy, static_cast<const FacetModeler::Contour2D&>( contour ), color );
    proxy.wGeom().popModelTransform();
    proxy.end();
  }
}

inline void DEBUG_DRAW( const FacetModeler::Profile3D& profile, OdInt16 color ) {
  if( g_DEBUG_DRAW_ENABLED ) {
    OdRemoteGeomProxy proxy;
    OdRemoteGeomProxyAutoBlock rAuto(proxy);

    proxy.begin();
    proxy.traits().setColor( color );

    for(OdUInt32 nContour = 0; nContour < profile.size(); nContour++)
    {
      const FacetModeler::Contour2D& c2d = profile[nContour] ;
      for(OdUInt32 nSeg = 0; nSeg < c2d.numSegments(); nSeg++)
      {
        FacetModeler::Segment2D sg;
        c2d.getSegmentAt(nSeg, sg);

        OdGePoint3d pt3d[2];
        profile.to3D(sg.startPt(), pt3d[0]);
        profile.to3D(sg.endPt(), pt3d[1]);

        proxy.wGeom().polyline( 2, pt3d );
#if 1
        OdUInt16 colorOld = proxy.traits().color();
        proxy.traits().setColor(1);
        pt3d[1] = pt3d[0] + (pt3d[1] - pt3d[0]) * 0.1;
        proxy.wGeom().polyline(2, pt3d);
        proxy.traits().setColor(colorOld);
#endif
      }
    }

    proxy.end();
  }
}

inline void DEBUG_DRAW_LINESEG(const OdGePoint2d& pt1, const OdGePoint2d& pt2, OdInt16 color) 
{
  OdRemoteGeomProxy proxy;
  OdRemoteGeomProxyAutoBlock rAuto(proxy);

  proxy.begin();
  proxy.traits().setColor( color );
  OdGePoint2d pts[2];
  pts[0] = pt1;
  pts[1] = pt2;
  OdGiTools::polyline2d( proxy.wGeom(), 2, pts );
  proxy.end();
}

inline void DEBUG_DRAW_LINE(const OdGeLine2d& aLine, OdInt16 color)
{
  if( !g_DEBUG_DRAW_ENABLED )
    return;

  OdRemoteGeomProxy proxy;
  OdRemoteGeomProxyAutoBlock rAuto(proxy);

  const double dMult = 100000.0;
  OdGePoint2d ptS2d = aLine.pointOnLine() - aLine.direction() * dMult;
  OdGePoint2d ptE2d = aLine.pointOnLine() + aLine.direction() * dMult;

  DEBUG_DRAW_LINESEG(ptS2d, ptE2d, color);

  /*OdGePoint3d pt1(OdGePlane::kXYPlane, ptS2d);
  OdGePoint3d pt2(OdGePlane::kXYPlane, ptE2d);

  proxy.begin();
  proxy.traits().setColor( color );
  proxy.wGeom().xline(pt1, pt2);
  proxy.end();*/
}

inline void DEBUG_DRAW(const OdGeLine2d& aLine, const OdGePlane& aPlane, OdInt16 color)
{
  if( !g_DEBUG_DRAW_ENABLED )
    return;

  OdRemoteGeomProxy proxy;
  OdRemoteGeomProxyAutoBlock rAuto(proxy);

  FacetModeler::Contour3D cLineRep( aPlane );

  const double dMult = 100000.0;
  OdGePoint2d ptS2d = aLine.pointOnLine() - aLine.direction() * dMult;
  OdGePoint2d ptE2d = aLine.pointOnLine() + aLine.direction() * dMult;

  cLineRep.appendVertex(ptS2d);
  cLineRep.appendVertex(ptE2d);

  DEBUG_DRAW(cLineRep, color);
};

inline void DEBUG_DRAW(const OdCmEntityColor& color)
{
  if( !g_DEBUG_DRAW_ENABLED ) 
    return;

  OdRemoteGeomProxy proxy;
  OdRemoteGeomProxyAutoBlock rAuto(proxy);

  proxy.begin();
  proxy.traits().setTrueColor( color );
  proxy.traits().setLineWeight(OdDb::kLnWt211);


  static const double fMarkerSize = 100;
  OdGePoint2d pts[7];
  pts[0] = OdGePoint2d(-fMarkerSize, fMarkerSize);
  pts[1] = OdGePoint2d(fMarkerSize, fMarkerSize);
  pts[2] = OdGePoint2d(fMarkerSize, -fMarkerSize);
  pts[3] = OdGePoint2d(-fMarkerSize, -fMarkerSize);
  pts[4] = OdGePoint2d(-fMarkerSize, fMarkerSize);
  pts[5] = OdGePoint2d(0, -fMarkerSize);
  pts[6] = OdGePoint2d(fMarkerSize, fMarkerSize);
  
  OdGiTools::polyline2d( proxy.wGeom(), 7, pts );  
  
  proxy.end();
}

inline void DEBUG_DRAW1(const OdCmColor& color)
{
  DEBUG_DRAW(color.entityColor());
};

inline void DEBUG_DRAW_POLYLINE(const OdGePoint3d* ptPoly, OdUInt32 nPts, OdInt16 color) {
  if( g_DEBUG_DRAW_ENABLED ) {
    OdRemoteGeomProxy proxy;
    OdRemoteGeomProxyAutoBlock rAuto(proxy);

    proxy.begin();
    proxy.traits().setColor( color );
    proxy.wGeom().polyline(nPts, ptPoly);
    proxy.end();
  }
}

inline void DEBUG_DRAW_POLYLINE(const OdGePoint2d* ptPoly2d, OdUInt32 nPts, OdInt16 color) {
  if (g_DEBUG_DRAW_ENABLED) {

    OdGePoint3dArray ptPoly;
    ptPoly.resize(nPts);
    for (OdUInt32 i = 0; i != nPts; i++)
    {
      ptPoly[i] = OdGePoint3d(ptPoly2d[i].x, ptPoly2d[i].y, 0);
    }

    DEBUG_DRAW_POLYLINE(ptPoly.asArrayPtr(), nPts, color);
  }
}

inline void DEBUG_DRAW_TRIANGLE(const OdGePoint3d* ptTriangle, OdInt16 color) 
{
  if( !g_DEBUG_DRAW_ENABLED ) 
    return;

  OdRemoteGeomProxy proxy;
  OdRemoteGeomProxyAutoBlock rAuto(proxy);

  OdInt32Array faceList;
  faceList.push_back(3);
  faceList.push_back(0);
  faceList.push_back(1);
  faceList.push_back(2);

  proxy.begin();
  proxy.traits().setColor( color );
  proxy.wGeom().shell(3, ptTriangle, 4, faceList.asArrayPtr());
  proxy.end();
}

inline void DEBUG_DRAW( const FacetModeler::Profile2D& profile, OdInt16 color ) 
{
  if( g_DEBUG_DRAW_ENABLED ) {
    OdRemoteGeomProxy proxy;
    OdRemoteGeomProxyAutoBlock rAuto(proxy);

    proxy.begin();
    proxy.traits().setColor( color );

    OdUInt32 i, iSize = profile.size();
    for( i = 0; i <iSize; i++ )
    {
      OdUInt32 j, jSize = profile[ i ].numSegments();
      for( j = 0; j < jSize; j++ )
      {
        FacetModeler::Segment2D seg;
        profile[ i ].getSegmentAt( j, seg );
        _debug_draw( proxy.wGeom(), seg );
      }
    }

    proxy.end();
  }
}

inline void DEBUG_DRAW( const std::vector<FacetModeler::Profile2D>& rProfiles, OdInt16 color ) 
{
  if( !g_DEBUG_DRAW_ENABLED )
    return;

  for(size_t idx = 0; idx < rProfiles.size(); idx++)
  {
    DEBUG_DRAW(rProfiles[idx], color);
  }  
}

inline void DEBUG_DRAW2(const OdGePoint2d& pt1, const OdGePoint2d& pt2, OdInt16 color)
{
  if (g_DEBUG_DRAW_ENABLED) { // set this flag in debugger to enable geometry sending to remote viever
    OdRemoteGeomProxy proxy;
    OdRemoteGeomProxyAutoBlock rAuto(proxy);

    proxy.begin(); // initialize proxy for remote OdGiWorldDraw object
    proxy.traits().setColor(color);// use remote OdGiSubEntityTraits
    OdGePoint3d pts[] = { OdGePoint3d(pt1.x, pt1.y, 0), OdGePoint3d(pt2.x, pt2.y, 0) };
    proxy.wGeom().polyline(2, pts);
    proxy.end();// send geometry to remote viewer
  }
}

inline void DEBUG_DRAW3(const OdGePoint3d& pt1, const OdGePoint3d& pt2, OdInt16 color) 
{
  if( g_DEBUG_DRAW_ENABLED ) { // set this flag in debugger to enable geometry sending to remote viever
    OdRemoteGeomProxy proxy;
    OdRemoteGeomProxyAutoBlock rAuto(proxy);

    proxy.begin(); // initialize proxy for remote OdGiWorldDraw object
    proxy.traits().setColor( color );// use remote OdGiSubEntityTraits
    OdGePoint3d pts[] = {pt1, pt2};
    proxy.wGeom().polyline( 2, pts );
    proxy.end();// send geometry to remote viewer
  }
}

inline void DEBUG_DRAW(const OdGeLine3d& line, OdInt16 color)
{
  if(!g_DEBUG_DRAW_ENABLED)
    return;

  OdRemoteGeomProxy proxy;
  OdRemoteGeomProxyAutoBlock rAuto(proxy);

  const double dLineLength = 10000;

  OdGePoint3d pt1, pt2;
  pt1 = line.pointOnLine() + line.direction() * dLineLength;
  pt2 = line.pointOnLine() - line.direction() * dLineLength;


  proxy.begin(); // initialize proxy for remote OdGiWorldDraw object
  proxy.traits().setColor( color );// use remote OdGiSubEntityTraits
  OdGePoint3d pts[] = {pt1, pt2};
  proxy.wGeom().polyline( 2, pts );
  proxy.end();// send geometry to remote viewer
}

inline void DEBUG_DRAW3(const OdGeVector3d& v1, const OdGeVector3d& v2, OdInt16 color) {
  DEBUG_DRAW3( (const OdGePoint3d&)v1, (const OdGePoint3d&)v2, color );
}

static void bodyGenMesh(GeMesh::OdGeTrMesh& mesh, const FacetModeler::Body& body)
{
  FaceIterator itF(&body);
  while (!itF.done())
  {
    Face* pFace = itF.get();
    const OdGePlane& plane = pFace->plane();
    FacetModeler::Profile2D profile;
    pFace->projection(plane, profile);
    OdGePoint2dArray aVxFace;
    std::vector<OdInt32> aFaceIndex;
    for (unsigned int i = 0; i < profile.size(); ++i)
    {
      const FacetModeler::Contour2D& contour = profile[i];
      const int nVt = contour.numVerts();
      const bool bHole = !contour.isCCW();
      aFaceIndex.push_back(bHole ? -nVt : nVt);
      for (int j = 0; j < nVt; ++j)
      {
        const int iVx = aVxFace.logicalLength();
        aVxFace.resize(iVx + 1);
        OdGePoint2d pt;
        contour.getVertexAt(j, &aVxFace[iVx]);
        aFaceIndex.push_back(iVx);
      }
    }
    OdArray<OdInt32> aTrFace;
    GeMesh::triangulateProfile(aVxFace, aFaceIndex, aTrFace);

    const int nVxMesh = mesh.m_aVx.logicalLength();
    const int nVxFace = aVxFace.logicalLength();
    mesh.m_aVx.resize(nVxMesh + nVxFace);
    for (int i = 0; i < nVxFace; ++i)
    {
      mesh.m_aVx[nVxMesh + i] = plane.evalPoint(aVxFace[i]);
    }
    const int nTr = aTrFace.logicalLength() / 4;
    for (int i = 0; i < nTr; ++i)
    {
      ODA_ASSERT(aTrFace[i * 4] == 3);
      GeMesh::OdGeTr tr;
      tr.tr[0] = nVxMesh + aTrFace[i * 4 + 1];
      tr.tr[1] = nVxMesh + aTrFace[i * 4 + 2];
      tr.tr[2] = nVxMesh + aTrFace[i * 4 + 3];
      mesh.m_aTr.push_back(tr);
    }
    itF.next();
  }
}

static inline void DebugDrawBody(OdRemoteGeomProxy& proxy, const FacetModeler::Body& rBody, bool wired = true)
{
  if (rBody.isNull())
    return;

  if (wired)
  {
    FacetModeler::EdgeBodyIterator itEdges(&rBody);
    while (!itEdges.done())
    {
      FacetModeler::Edge* pEdge = itEdges.get();
      if (pEdge != NULL)
      {
        FacetModeler::Edge* pPairEdge = pEdge->next();
        if (pPairEdge != NULL)
        {
          OdGePoint3d ptVtx[2];
          ptVtx[0] = pEdge->vertex()->point();
          ptVtx[1] = pPairEdge->vertex()->point();

          proxy.wGeom().polyline(2, ptVtx);
        }
      }

      itEdges.next();
    }
  }
  else{
    GeMesh::OdGeTrMesh mesh;
    bodyGenMesh(mesh, rBody);

    OdGePoint3dArray vertices;
    OdInt32Array faces;
    for (unsigned int i = 0; i < mesh.m_aTr.length(); i++)
    {
      faces.push_back(3);
      faces.push_back(mesh.m_aTr[i].tr[0]);
      faces.push_back(mesh.m_aTr[i].tr[1]);
      faces.push_back(mesh.m_aTr[i].tr[2]);
    }
    proxy.wGeom().shell(mesh.m_aVx.length(), mesh.m_aVx.asArrayPtr(), faces.length(), faces.asArrayPtr());
  }
}

inline void DEBUG_DRAW(const FacetModeler::Body& body, OdInt16 color, bool wired = true )
{
  if( !g_DEBUG_DRAW_ENABLED )
    return;

  OdRemoteGeomProxy proxy;
  OdRemoteGeomProxyAutoBlock rAuto(proxy);

  proxy.begin(); // initialize proxy for remote OdGiWorldDraw object

  proxy.traits().setColor( color );// use remote OdGiSubEntityTraits
  DebugDrawBody( proxy, body, wired);
  
  proxy.end();// send geometry to remote viewer
}



inline void DEBUG_DRAW( const OdGePoint3d& pt, double dSize, OdInt16 color ) 
{
    if( !g_DEBUG_DRAW_ENABLED )
        return;

    OdRemoteGeomProxy proxy;
    OdRemoteGeomProxyAutoBlock rAuto(proxy);

    proxy.begin(); // initialize proxy for remote OdGiWorldDraw object

    const double dSizes = dSize;
    OdGePoint3d ptOrigin = pt - OdGeVector3d(dSizes/2, dSizes/2, dSizes/2);

    proxy.traits().setColor( color );// use remote OdGiSubEntityTraits
    DebugDrawBody( proxy, FacetModeler::Body::box( ptOrigin, OdGeVector3d(dSizes, dSizes, dSizes) ) );

    proxy.end();// send geometry to remote viewer
}

inline void DEBUG_DRAW( const FacetModeler::Edge& pEdge, OdInt16 color ) 
{
  if( !g_DEBUG_DRAW_ENABLED )
    return;

  OdRemoteGeomProxy proxy;
  OdRemoteGeomProxyAutoBlock rAuto(proxy);

  proxy.begin(); // initialize proxy for remote OdGiWorldDraw object
  proxy.traits().setColor( color );// use remote OdGiSubEntityTraits
  OdGePoint3d pts[] = {pEdge.startPoint(), pEdge.endPoint()};
  proxy.wGeom().polyline( 2, pts );
  proxy.end();// send geometry to remote viewer

  DEBUG_DRAW(pEdge.endPoint(), 0.01, color);
}

inline void DebugDrawFaceEdges(const FacetModeler::Face* pFace, OdInt16 color)
{
  if( !g_DEBUG_DRAW_ENABLED )
    return;

  EdgeFaceIterator it(pFace);
  while(!it.done())
  {
    DEBUG_DRAW(*it.get(), color);

    OdGePoint3d pts = it.get()->startPoint();
    OdGePoint3d pte = it.get()->endPoint();

    double d = pts.distanceTo(pte);

    it.next();
  }

}


inline void DEBUG_DRAW( const OdGePoint3d& pt, OdInt16 color ) 
{
    DEBUG_DRAW(pt, 12.0, color);
}

inline void DEBUG_DRAW3(const OdGePoint3d& pt, const OdGeVector3d& v, OdInt16 color) 
{
  if( g_DEBUG_DRAW_ENABLED )
  { // set this flag in debugger to enable geometry sending to remote viever
    OdRemoteGeomProxy proxy;
    OdRemoteGeomProxyAutoBlock rAuto(proxy);

    const double dLength = v.length();
    const double dVecLength = dLength * 0.9;
    OdGePoint3d ptEnd1 = pt + (dVecLength * v);
    OdGePoint3d ptEnd2 = pt + v;

    OdGePoint3d pts1[] = {pt, ptEnd1};
    DEBUG_DRAW_POLYLINE(pts1, 2, color);

    OdGePoint3d pts2[] = {ptEnd1, ptEnd2};
    DEBUG_DRAW_POLYLINE(pts2, 2, color+1);
  }
}


inline void DEBUG_DRAW( const OdGeLineSeg3d& aSeg,  OdInt16 color )
{
  if( g_DEBUG_DRAW_ENABLED ) { // set this flag in debugger to enable geometry sending to remote viever
    OdRemoteGeomProxy proxy;
    OdRemoteGeomProxyAutoBlock rAuto(proxy);

    proxy.begin(); // initialize proxy for remote OdGiWorldDraw object
    proxy.traits().setColor( color );// use remote OdGiSubEntityTraits
    OdGePoint3d pts[] = {aSeg.startPoint(), aSeg.endPoint()};
    proxy.wGeom().polyline( 2, pts );
    proxy.end();// send geometry to remote viewer
  }
};


inline void DEBUG_DRAW_ARROW(const OdGePoint3d& ptStart, const OdGeVector3d& vec, OdInt16 edgeBaseColor)
{
  OdGePoint3d ptEnd = ptStart + vec;
  OdGePoint3d ptArrowBase = ptEnd - (ptEnd - ptStart) / 20.0;

  OdInt16 edgeColor = edgeBaseColor;
  OdInt16 arrowColor = OdCmEntityColor::kACIRed;
  DEBUG_DRAW3(ptStart, ptEnd, edgeColor);
  DEBUG_DRAW3(ptArrowBase, ptEnd, arrowColor);
};


inline void DEBUG_DRAW_FACE( const FacetModeler::Face* pFace, OdInt16 color ) {
  if( g_DEBUG_DRAW_ENABLED ) { // set this flag in debugger to enable geometry sending to remote viever
    OdRemoteGeomProxy proxy;
    OdRemoteGeomProxyAutoBlock rAuto(proxy);

    FacetModeler::Profile3D profile;
    pFace->profile( profile );
    DEBUG_DRAW( profile, color );
  }
}


inline void DEBUG_DRAW_AXIS4(const OdGePoint3d& ptOrigin, const OdGeVector3d& vAxis, const double fLen, OdInt16 color)
{
  OdRemoteGeomProxy proxy;
  OdRemoteGeomProxyAutoBlock rAuto(proxy);

  OdGePoint3d v1 = ptOrigin + vAxis * (0.9 * fLen);
  OdGePoint3d v2 = v1 + vAxis * (0.1 * fLen);
  DEBUG_DRAW3( ptOrigin, v1, color );
  DEBUG_DRAW3( v1, v2, OdCmEntityColor::kACIRed );
}

inline void DEBUG_DRAW_AXIS(const OdGePoint3d& ptOrigin, const OdGeVector3d& vAxis, const double fLen) {
  DEBUG_DRAW_AXIS4(ptOrigin, vAxis, fLen, OdCmEntityColor::kACIGreen);
}

inline void DEBUG_DRAW_CS(const OdGePoint3d& ptOrigin, const OdGeVector3d& vx, const OdGeVector3d& vy, const OdGeVector3d& vz, const double fLen) 
{
  OdRemoteGeomProxy proxy;
  OdRemoteGeomProxyAutoBlock rAuto(proxy);

  DEBUG_DRAW_AXIS4(ptOrigin, vx, fLen, OdCmEntityColor::kACIRed);
  DEBUG_DRAW_AXIS4(ptOrigin, vy, fLen, OdCmEntityColor::kACIGreen);
  DEBUG_DRAW_AXIS4(ptOrigin, vz, fLen, OdCmEntityColor::kACIBlue);
}

inline void DEBUG_DRAW_PLANE(const OdGePlane& rPlane, const double fWidth, OdInt16 color) 
{
  OdRemoteGeomProxy proxy;
  OdRemoteGeomProxyAutoBlock rAuto(proxy);

  FacetModeler::Contour2D c2dBox;
  c2dBox.appendVertex(OdGePoint2d(-fWidth,-fWidth));
  c2dBox.appendVertex(OdGePoint2d(fWidth,-fWidth));
  c2dBox.appendVertex(OdGePoint2d(fWidth,fWidth));
  c2dBox.appendVertex(OdGePoint2d(-fWidth,fWidth));
  c2dBox.setClosed(true);

  FacetModeler::Profile3D c3dPlane(rPlane);
  c3dPlane.as2d().push_back(c2dBox);
  DEBUG_DRAW( c3dPlane, color );
  DEBUG_DRAW_AXIS( rPlane.pointOnPlane(), rPlane.normal(), fWidth/2 );
}

inline void DEBUG_DRAW(const OdGePoint2d& rPoint, OdInt16 color, double fMarkerSize = 5) 
{
  OdRemoteGeomProxy proxy;
  OdRemoteGeomProxyAutoBlock rAuto(proxy);

  FacetModeler::Contour2D c2dBox;

  c2dBox.appendVertex(rPoint + OdGeVector2d(fMarkerSize, -fMarkerSize));
  c2dBox.appendVertex(rPoint + OdGeVector2d(fMarkerSize, fMarkerSize));
  c2dBox.appendVertex(rPoint + OdGeVector2d(-fMarkerSize, fMarkerSize));
  c2dBox.appendVertex(rPoint + OdGeVector2d(-fMarkerSize, -fMarkerSize));
  c2dBox.setClosed(true);

  DEBUG_DRAW( c2dBox, color );
}

inline void DEBUG_DRAW_FACE_PROFILE(const FacetModeler::Profile2D& faceProfile)
{
  for(OdUInt32 contourIdx = 0; contourIdx < faceProfile.size(); contourIdx++)
  {
    const FacetModeler::Contour2D& currentContour = faceProfile[contourIdx];
    if(currentContour.isCCW())
    {
      DEBUG_DRAW(currentContour, OdCmEntityColor::kACIRed);
    }
    else
    {
      DEBUG_DRAW(currentContour, OdCmEntityColor::kACIGreen);
    }
  }
}

/*inline void TEST_DRAW_PLANE( const OdGePlane& rPlane, const double fWidth) {
  FacetModeler::Contour2D c2dBox;
  c2dBox.appendVertex(OdGePoint2d(-fWidth,-fWidth));
  c2dBox.appendVertex(OdGePoint2d(fWidth,-fWidth));
  c2dBox.appendVertex(OdGePoint2d(fWidth,fWidth));
  c2dBox.appendVertex(OdGePoint2d(-fWidth,fWidth));
  c2dBox.setClosed(true);

  FacetModeler::Profile3D c3dPlane(rPlane);
  c3dPlane.as2d().push_back(c2dBox);
  pGi->Geo().DrawProfile3D(c3dPlane);
}*/

inline void DEBUG_BLOCK_BEGIN()
{
  OdRemoteGeomProxy proxy;
  proxy.sendStringToExecute(g_strGeomBlockBegin);
}

inline void DEBUG_BLOCK_END()
{
  OdRemoteGeomProxy proxy;
  proxy.sendStringToExecute(g_strGeomBlockEnd);
}

inline void DEBUG_DRAW_EXTENTS(const OdGeExtents2d& box, OdUInt16 color)
{
  DEBUG_BLOCK_BEGIN();

  OdGePoint2dArray pts;
  pts.resize(5);

  pts[0] = box.minPoint();
  pts[1] = OdGePoint2d(box.maxPoint().x, box.minPoint().y);
  pts[2] = box.maxPoint();
  pts[3] = OdGePoint2d(box.minPoint().x, box.maxPoint().y);
  pts[4] = pts[0];

  DEBUG_DRAW_POLYLINE(pts.asArrayPtr(), 5, color);
  DEBUG_BLOCK_END();
}

inline void DEBUG_DRAW_ENTITY_EXTENTS_2D(const OdGeMatrix3d& mEntity, const OdGeExtents3d& mExtents, OdUInt16 color)
{
    OdGePoint3d ptOrigin = mEntity.getCsOrigin();

    double dL = mExtents.maxPoint().x - mExtents.minPoint().x;
    double dW = mExtents.maxPoint().y - mExtents.minPoint().y;

    double dPtSize = 1;
    {       
        double dMin = odmin(dL, dW);
        dPtSize = dMin / 10;
    }

    DEBUG_BLOCK_BEGIN();
    DEBUG_DRAW(ptOrigin, dPtSize, 0);
    OdGePoint3d pts[5];

    pts[0] = ptOrigin;
    pts[1] = ptOrigin + mEntity.getCsXAxis() * dL;
    pts[2] = pts[1] + mEntity.getCsYAxis() * dW;
    pts[3] = pts[2] - mEntity.getCsXAxis() * dL;
    pts[4] = pts[3] - mEntity.getCsYAxis() * dW;
    DEBUG_DRAW_POLYLINE(pts, 5, color);
    DEBUG_BLOCK_END();
}

/*inline void DEBUG_DRAW_ENTITY_EXTENTS_2D(AECDbGeoPtr pEntity, const OdGeExtents3d& mExtents, OdUInt16 color)
{
    DEBUG_DRAW_ENTITY_EXTENTS_2D(pEntity->GetMatrix(), mExtents, color);
}*/

inline void DEBUG_DRAW_CS(const OdGeMatrix3d& m, double dBaseLen, OdUInt16 color)
{
    OdGePoint3d origin;
    OdGeVector3d xAxis;
    OdGeVector3d yAxis; 
    OdGeVector3d zAxis;
    m.getCoordSystem(origin, xAxis, yAxis, zAxis);
  
    DEBUG_BLOCK_BEGIN();
    DEBUG_DRAW(origin, dBaseLen / 10., 0);
    
    OdGePoint3d pts[2];
    pts[0] = origin;

    pts[1] = origin + xAxis * dBaseLen;
    DEBUG_DRAW_POLYLINE(pts, 2, color);

    pts[1] = origin + yAxis * dBaseLen * 3;
    DEBUG_DRAW_POLYLINE(pts, 2, color);

    pts[1] = origin + zAxis * dBaseLen * 5;
    DEBUG_DRAW_POLYLINE(pts, 2, color);

    DEBUG_BLOCK_END();
}

/*inline void DEBUG_DRAW(OdDbBlockReference* pBlock, OdUInt16 color)
{
  DEBUG_BLOCK_BEGIN();
  
  OdRemoteGeomProxy proxy;
  OdRemoteGeomProxyAutoBlock rAuto(proxy);

  proxy.setDatabase(pBlock->database());
  proxy.begin();
  proxy.traits().setColor( color );
  proxy.wGeom().draw( pBlock );
  proxy.end();

  DEBUG_BLOCK_END();
}*/

inline void DEBUG_CONTOUR2D(const FacetModeler::Contour2D& c)
{
  AecAutoColor color;
  bool fIsSI = c.isSelfIntersecting();

  FacetModeler::Segment2D aSeg;
  const int numSegs = c.numSegments();
  for(int s = 0; s < numSegs; s++)
  {
    c.getSegmentAt(s, aSeg);
    DEBUG_DRAW(aSeg, color.peekColor());

    double dBulge = aSeg.bulge();
    FacetModeler::SegmentType sType = aSeg.type();

    OdGePoint2d ptS = aSeg.startPt();
    OdGePoint2d ptE = aSeg.endPt();

    DEBUG_DRAW(ptS, color.peekColor());
    DEBUG_DRAW(ptE, color.peekColor());

    color.color();
  }

  OdGePoint2d pt;
  const int numVerts = c.numVerts();
  for(int i = 0; i < numVerts; i++)
  {
    c.getVertexAt(i, &pt);
    DEBUG_DRAW(pt, color.color());
  }
}

inline void DEBUG_DRAW_EXTENTS(const OdGeExtents3d& box, OdUInt16 color)
{
  DEBUG_BLOCK_BEGIN();

  const OdGePoint3d& minPt = box.minPoint();
  const OdGePoint3d& maxPt = box.maxPoint();

  OdGePoint3d pts[8];

  pts[0] = minPt;
  pts[1] = OdGePoint3d(maxPt.x, minPt.y, minPt.z);
  pts[2] = OdGePoint3d(maxPt.x, maxPt.y, minPt.z);
  pts[3] = OdGePoint3d(minPt.x, maxPt.y, minPt.z);

  pts[4] = OdGePoint3d(minPt.x, minPt.y, maxPt.z);
  pts[5] = OdGePoint3d(maxPt.x, minPt.y, maxPt.z);
  pts[6] = maxPt;
  pts[7] = OdGePoint3d(minPt.x, maxPt.y, maxPt.z);

  OdGePoint3d poly1[6];
  poly1[0] = pts[4];
  poly1[1] = pts[0];
  poly1[2] = pts[1];
  poly1[3] = pts[2];
  poly1[4] = pts[3];
  poly1[5] = pts[7];

  OdGePoint3d poly2[6];
  poly2[0] = pts[1];
  poly2[1] = pts[5];
  poly2[2] = pts[4];
  poly2[3] = pts[7];
  poly2[4] = pts[6];
  poly2[5] = pts[2];

  OdGePoint3d poly3[2];
  poly3[0] = pts[0];
  poly3[1] = pts[3];

  OdGePoint3d poly4[2];
  poly4[0] = pts[5];
  poly4[1] = pts[6];

  DEBUG_DRAW_POLYLINE(poly1, 6, color);
  DEBUG_DRAW_POLYLINE(poly2, 6, color);
  DEBUG_DRAW_POLYLINE(poly3, 2, color);
  DEBUG_DRAW_POLYLINE(poly4, 2, color);

  DEBUG_BLOCK_END();
}


inline void DEBUG_DRAW_LOOP(const OdGePoint2dArray& loop, OdUInt16 color)
{
  DEBUG_BLOCK_BEGIN();

  OdGePoint2dArray closedLoop;
  closedLoop.resize(loop.size() + 1);

  for (OdUInt32 i = 0; i != loop.size(); i++)
  {
    closedLoop[i] = loop[i];
  }
  closedLoop[loop.size()] = loop[0];

  DEBUG_DRAW_POLYLINE(closedLoop.asArrayPtr(), closedLoop.size(), color);
  DEBUG_BLOCK_END();
}

#else

template <class TSegment2d>
inline void DEBUG_DRAW(const TSegment2d& rSeg, OdInt16 color) {}

inline void DEBUG_DRAW3(OdRemoteGeomProxy& proxy, const Contour2D& contour, OdInt16 color) {}

inline void DEBUG_DRAW( const Contour2D& contour, OdInt16 color) {}

inline void DEBUG_DRAW(const Contour3D& contour, OdInt16 color ) {}

inline void DEBUG_DRAW( const Profile3D& profile, OdInt16 color ) {}

inline void DEBUG_DRAW_LINESEG(const OdGePoint2d& pt1, const OdGePoint2d& pt2, OdInt16 color) {}

inline void DEBUG_DRAW_LINE(const OdGeLine2d& aLine, OdInt16 color) {}

inline void DEBUG_DRAW(const OdGeLine2d& aLine, const OdGePlane& aPlane, OdInt16 color) {}

inline void DEBUG_DRAW(const OdCmEntityColor& color) {}

inline void DEBUG_DRAW1(const OdCmColor& color) {}

inline void DEBUG_DRAW_POLYLINE(const OdGePoint3d* ptPoly, OdUInt32 nPts, OdInt16 color) {}

inline void DEBUG_DRAW( const Profile2D& profile, OdInt16 color ) {}

inline void DEBUG_DRAW( const std::vector<Profile2D>& rProfiles, OdInt16 color ) {}

inline void DEBUG_DRAW3(const OdGePoint3d& pt1, const OdGePoint3d& pt2, OdInt16 color) {}

inline void DEBUG_DRAW(const OdGeLine3d& line, OdInt16 color) {}

inline void DEBUG_DRAW3(const OdGeVector3d& v1, const OdGeVector3d& v2, OdInt16 color) {}

static inline void DebugDrawBody( OdRemoteGeomProxy& proxy, const Body& rBody ) {}

inline void DEBUG_DRAW( const Body& body, OdInt16 color, bool wired = true ) {}

inline void DEBUG_DRAW( const OdGePoint3d& pt, double dSize, OdInt16 color ) {}

inline void DEBUG_DRAW( const OdGePoint3d& pt, OdInt16 color ) {}

inline void DEBUG_DRAW3(const OdGePoint3d& pt, const OdGeVector3d& v, OdInt16 color) {}


inline void DEBUG_DRAW( const OdGeLineSeg3d& aSeg,  OdInt16 color ) {}

inline void DEBUG_DRAW_FACE( const Face* pFace, OdInt16 color ) {}

inline void DEBUG_DRAW( const Edge& pEdge, OdInt16 color )  {}

inline void DEBUG_DRAW_AXIS4(const OdGePoint3d& ptOrigin, const OdGeVector3d& vAxis, const double fLen, OdInt16 color) {}

inline void DEBUG_DRAW_AXIS(const OdGePoint3d& ptOrigin, const OdGeVector3d& vAxis, const double fLen) {}

inline void DEBUG_DRAW_CS(const OdGePoint3d& ptOrigin, const OdGeVector3d& vx, const OdGeVector3d& vy, const OdGeVector3d& vz, const double fLen)  {}

inline void DEBUG_DRAW_PLANE(const OdGePlane& rPlane, const double fWidth, OdInt16 color) {}

inline void DEBUG_DRAW(const OdGePoint2d& rPoint, OdInt16 color, double fMarkerSize = 5) {}
inline void DEBUG_DRAW(const Segment2D& rSeg, OdInt16 color) {}

inline void DEBUG_DRAW_FACE_PROFILE(const Profile2D& faceProfile) {}

//inline void TEST_DRAW_PLANE( AECGiDraw* pGi, const OdGePlane& rPlane, const double fWidth) {}


inline void DEBUG_DRAW_ENTITY_EXTENTS_2D(const OdGeMatrix3d& mEntity, const OdGeExtents3d& mExtents, OdUInt16 color) {}

//inline void DEBUG_DRAW_ENTITY_EXTENTS_2D(AECDbGeoPtr pEntity, const OdGeExtents3d& mExtents, OdUInt16 color) {}

inline void DEBUG_DRAW_CS(const OdGeMatrix3d& m, double dBaseLen, OdUInt16 color) {}


//inline void DEBUG_DRAW(OdDbBlockReference* pBlock, OdUInt16 color) {}

inline void DEBUG_DRAW_TRIANGLE(const OdGePoint3d* ptTriangle, OdInt16 color) {}

inline void DEBUG_CONTOUR2D(const Contour2D& c) {}

inline void DEBUG_BLOCK_BEGIN(){}
inline void DEBUG_BLOCK_END(){}


inline void DEBUG_DRAW2(const OdGePoint2d& pt1, const OdGePoint2d& pt2, OdInt16 color) {}
inline void DEBUG_DRAW_EXTENTS(const OdGeExtents2d& box, OdUInt16 color) {}
inline void DEBUG_DRAW_LOOP(const OdGePoint2dArray& loop, OdUInt16 color) {}





#endif



#endif //#if !defined(DEBUG_DRAW_INCLUDED)
