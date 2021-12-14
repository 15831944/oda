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

#ifndef _SiShapePlane_h_Included_
#define _SiShapePlane_h_Included_

#include "Ge/GePlane.h"
#include "Ge/GeExtents3d.h"
#include "Ge/GeBoundBlock3d.h"
#include "Si/SiSpatialIndex.h"

#include "TD_PackPush.h"

/** \details
    This class represents plane implementation for OdSiShape interface.

    Corresponding C++ library: SpatialIndex
    <group OdSi_Classes>
*/
struct OdSiShapePlane : public OdSiShape {
  OdGePoint3d m_pointOnPlane;
  OdGeVector3d m_planeNormal;

  OdSiShapePlane( const OdGePoint3d& pointOnPlane, const OdGeVector3d& planeNormal ) {
    m_pointOnPlane = pointOnPlane;
    m_planeNormal = planeNormal;
  }
  OdSiShapePlane( const OdGePlane& plane ) {
    m_pointOnPlane = plane.pointOnPlane();
    m_planeNormal = plane.normal();
  }
  inline double signedDistanceTo( const OdGePoint3d& pt ) const {
    return m_planeNormal.dotProduct( pt - m_pointOnPlane );
  }
  bool contains( const OdGeExtents3d& , bool , const OdGeTol&  ) const {
    return false;
  }
  bool intersects( const OdGeExtents3d& extents, bool planar, const OdGeTol& tol ) const {
    const OdGePoint3d& minPt = extents.minPoint();
    double e = tol.equalPoint();
    if( OdLess( signedDistanceTo( minPt ), 0.0, e ) ) {
      const OdGePoint3d& maxPt = extents.maxPoint();
      if( !OdLess( signedDistanceTo( maxPt ), 0.0, e ) ) {
        return true;
      }
      OdGePoint3d pt( minPt.x, minPt.y, maxPt.z );
      if( !OdLess( signedDistanceTo( pt ), 0.0, e ) ) {
        return true;
      }
      pt.set( minPt.x, maxPt.y, minPt.z );
      if( !OdLess( signedDistanceTo( pt ), 0.0, e ) ) {
        return true;
      }
      pt.set( maxPt.x, minPt.y, minPt.z );
      if( !OdLess( signedDistanceTo( pt ), 0.0, e ) ) {
        return true;
      }
      pt.set( maxPt.x, maxPt.y, minPt.z );
      if( !OdLess( signedDistanceTo( pt ), 0.0, e ) ) {
        return true;
      }
      pt.set( maxPt.x, minPt.y, maxPt.z );
      if( !OdLess( signedDistanceTo( pt ), 0.0, e ) ) {
        return true;
      }
      pt.set( minPt.x, maxPt.y, maxPt.z );
      if( !OdLess( signedDistanceTo( pt ), 0.0, e ) ) {
        return true;
      }
      return false;
    }
    else if( OdGreater( signedDistanceTo( minPt ), 0.0, e ) ) {
      const OdGePoint3d& maxPt = extents.maxPoint();
      if( !OdGreater( signedDistanceTo( maxPt ), 0.0, e ) ) {
        return true;
      }
      OdGePoint3d pt( minPt.x, minPt.y, maxPt.z );
      if( !OdGreater( signedDistanceTo( pt ), 0.0, e ) ) {
        return true;
      }
      pt.set( minPt.x, maxPt.y, minPt.z );
      if( !OdGreater( signedDistanceTo( pt ), 0.0, e ) ) {
        return true;
      }
      pt.set( maxPt.x, minPt.y, minPt.z );
      if( !OdGreater( signedDistanceTo( pt ), 0.0, e ) ) {
        return true;
      }
      pt.set( maxPt.x, maxPt.y, minPt.z );
      if( !OdGreater( signedDistanceTo( pt ), 0.0, e ) ) {
        return true;
      }
      pt.set( maxPt.x, minPt.y, maxPt.z );
      if( !OdGreater( signedDistanceTo( pt ), 0.0, e ) ) {
        return true;
      }
      pt.set( minPt.x, maxPt.y, maxPt.z );
      if( !OdGreater( signedDistanceTo( pt ), 0.0, e ) ) {
        return true;
      }
      return false;
    }
    return true; // point on plane
  }
  virtual OdSiShape* clone() const { return new OdSiShapePlane(m_pointOnPlane, m_planeNormal); }
  virtual void transform(const OdGeMatrix3d& tf)
  {
    m_pointOnPlane.transformBy(tf);
    m_planeNormal.transformBy(tf);
    m_planeNormal.normalize();
  }
};

/**
	<group !!RECORDS_tkernel_apiref>
*/
struct ODSI_API OdSiHalfSpace : public OdSiShapePlane 
{
  OdSiHalfSpace( const OdGePoint3d& pointOnPlane, const OdGeVector3d& planeNormal ): OdSiShapePlane(pointOnPlane, planeNormal) 
  {
  }
  
  OdSiHalfSpace( const OdGePlane& plane ): OdSiShapePlane(plane){
  }

  void copyFrom(const OdSiHalfSpace& o)
  {
      m_pointOnPlane = o.m_pointOnPlane;
      m_planeNormal = o.m_planeNormal;
  }

  virtual bool contains(const OdGeExtents3d& extents, bool planar, const OdGeTol& tol) const
  {
    double e = tol.equalPoint();
    return OdGreaterOrEqual(signedDistanceTo(extents.minPoint()), 0.0, e) &&
        OdGreaterOrEqual(signedDistanceTo(extents.maxPoint()), 0.0, e) &&
        OdGreaterOrEqual(signedDistanceTo(OdGePoint3d(extents.minPoint().x, extents.minPoint().y, extents.maxPoint().z)), 0.0, e) &&
        OdGreaterOrEqual(signedDistanceTo(OdGePoint3d(extents.minPoint().x, extents.maxPoint().y, extents.minPoint().z)), 0.0, e) &&
        OdGreaterOrEqual(signedDistanceTo(OdGePoint3d(extents.maxPoint().x, extents.minPoint().y, extents.minPoint().z)), 0.0, e) &&
        OdGreaterOrEqual(signedDistanceTo(OdGePoint3d(extents.maxPoint().x, extents.maxPoint().y, extents.minPoint().z)), 0.0, e) &&
        OdGreaterOrEqual(signedDistanceTo(OdGePoint3d(extents.maxPoint().x, extents.minPoint().y, extents.maxPoint().z)), 0.0, e) &&
        OdGreaterOrEqual(signedDistanceTo(OdGePoint3d(extents.minPoint().x, extents.maxPoint().y, extents.maxPoint().z)), 0.0, e);
  }

  virtual bool intersects( const OdGeExtents3d& extents, bool planar, const OdGeTol& tol ) const
  {
    double e = tol.equalPoint();
    return OdGreaterOrEqual(signedDistanceTo(extents.minPoint()), 0.0, e) ||
        OdGreaterOrEqual(signedDistanceTo(extents.maxPoint()), 0.0, e) ||
        OdGreaterOrEqual(signedDistanceTo(OdGePoint3d(extents.minPoint().x, extents.minPoint().y, extents.maxPoint().z)), 0.0, e) ||
        OdGreaterOrEqual(signedDistanceTo(OdGePoint3d(extents.minPoint().x, extents.maxPoint().y, extents.minPoint().z)), 0.0, e) ||
        OdGreaterOrEqual(signedDistanceTo(OdGePoint3d(extents.maxPoint().x, extents.minPoint().y, extents.minPoint().z)), 0.0, e) ||
        OdGreaterOrEqual(signedDistanceTo(OdGePoint3d(extents.maxPoint().x, extents.maxPoint().y, extents.minPoint().z)), 0.0, e) ||
        OdGreaterOrEqual(signedDistanceTo(OdGePoint3d(extents.maxPoint().x, extents.minPoint().y, extents.maxPoint().z)), 0.0, e) ||
        OdGreaterOrEqual(signedDistanceTo(OdGePoint3d(extents.minPoint().x, extents.maxPoint().y, extents.maxPoint().z)), 0.0, e);
  }

  virtual OdSiShape* clone() const { return new OdSiHalfSpace(m_pointOnPlane, m_planeNormal); }

  bool intersects(const OdGeBoundBlock3d& extents, const OdGeTol& tol, bool& bContains) const;
};

#include "TD_PackPop.h"

#endif
