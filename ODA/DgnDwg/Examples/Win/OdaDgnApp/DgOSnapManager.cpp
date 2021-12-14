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

#include "stdafx.h"
#include "DgOSnapManager.h"
#include "Gi/GiPathNode.h"
#include "OdRound.h"
#define STL_USING_LIMITS
#include "OdaSTL.h"
#include "Gi/GiViewportGeometry.h"
#include "Gi/GiViewportDraw.h"

#define snapPtSize 5

DgOSnapManager::DgSubentId::DgSubentId(const OdGiPathNode& giPath)
{
  m_gsMarker = giPath.selectionMarker();
  const OdGiPathNode* pGiPath = &giPath;
  do
  {
    m_path.append(pGiPath->persistentDrawableId());
    pGiPath = pGiPath->parent();
  }
  while(pGiPath);
}

bool DgOSnapManager::DgSubentId::operator == (const DgSubentId& op) const
{
  if(m_gsMarker != op.m_gsMarker)
    return false;
  if(m_path.size()!=op.m_path.size())
    return false;
  OdGePoint3dArray::size_type i;
  for ( i = 0; i < m_path.size(); ++i )
  {
    if(m_path[i]!=op.m_path[i])
      return false;
  }
  return true;
}


DgOSnapManager::DgOSnapManager()
  : m_nSnapModes( 0xFFFFFFFF )
  , m_mode(OdDgElement::OsnapMode(0))
{
}

void DgOSnapManager::reset()
{
  m_centers.clear();
}

void DgOSnapManager::subViewportDraw(OdGiViewportDraw* pVpDraw) const
{
  OdGePoint3d pts[4];
  OdGiViewportGeometry& geom = pVpDraw->geometry();
  OdGiViewport& vp = pVpDraw->viewport();
  OdGeMatrix3d xWorldToEye = vp.getWorldToEyeTransform();
  vp.getNumPixelsInUnitSquare(vp.getCameraTarget() /*OdGePoint3d::kOrigin*/, (OdGePoint2d&)pts[0]);
  double pix = 1. / pts[0].x;
  double s = snapPtSize * pix;

  OdGiSubEntityTraits& traits = pVpDraw->subEntityTraits();
  if(m_mode)
  {
    traits.setColor(2);
    traits.setFillType(kOdGiFillNever);
    traits.setSelectionMarker(kNullSubentIndex);
    pts[0] = xWorldToEye * m_snapPoint;
    vp.doPerspective(pts[0]);

    switch(m_mode)
    {
    case OdDgElement::kOsModeEnd:
      pts[1].set(pts[0].x - s, pts[0].y + s, 0.0);
      pts[2].set(pts[0].x - s, pts[0].y - s, 0.0);
      pts[3].set(pts[0].x + s, pts[0].y - s, 0.0);
      pts[0].set(pts[0].x + s, pts[0].y + s, 0.0);
      geom.polygonEye(4, pts);
      pts[1].set(pts[1].x - pix, pts[1].y + pix, 0.0);
      pts[2].set(pts[2].x - pix, pts[2].y - pix, 0.0);
      pts[3].set(pts[3].x + pix, pts[3].y - pix, 0.0);
      pts[0].set(pts[0].x + pix, pts[0].y + pix, 0.0);
      geom.polygonEye(4, pts);
      break;

    case OdDgElement::kOsModeMid:
      pts[1].set(pts[0].x - s * 1.2, pts[0].y - s * 0.6, 0.0);
      pts[2].set(pts[0].x,           pts[0].y + s * 1.4, 0.0);
      pts[3].set(pts[0].x + s * 1.2, pts[0].y - s * 0.6, 0.0);
      geom.polygonEye(3, pts+1);

      pts[1].set(pts[1].x - pix, pts[1].y - pix, 0.0);
      pts[2].set(pts[2].x,       pts[2].y + pix, 0.0);
      pts[3].set(pts[3].x + pix, pts[3].y - pix, 0.0);
      geom.polygonEye(3, pts+1);
      break;

    case OdDgElement::kOsModeCen:
      {
        OdGiModelTransformSaver mt(geom, vp.getEyeToWorldTransform());
        geom.circle(pts[0], s * 1.4, OdGeVector3d::kZAxis);
      }
      break;

    case OdDgElement::kOsModeQuad:
      pts[1].set(pts[0].x - s,  pts[0].y,     0.0);
      pts[2].set(pts[0].x,      pts[0].y + s, 0.0);
      pts[3].set(pts[0].x + s,  pts[0].y,     0.0);
      pts[0].set(pts[0].x,      pts[0].y - s, 0.0);
      geom.polygonEye(4, pts);
      pts[1].set(pts[1].x - pix,  pts[1].y,       0.0);
      pts[2].set(pts[2].x,        pts[2].y + pix, 0.0);
      pts[3].set(pts[3].x + pix,  pts[3].y,       0.0);
      pts[0].set(pts[0].x,        pts[0].y - pix, 0.0);
      geom.polygonEye(4, pts);
      break;

    case OdDgElement::kOsModePerp:
      pts[1].set(pts[0].x - s,       pts[0].y + s + pix, 0.0);
      pts[2].set(pts[0].x - s,       pts[0].y - s, 0.0);
      pts[3].set(pts[0].x + s + pix, pts[0].y - s, 0.0);
      geom.polylineEye(3, pts+1);
      pts[1].set(pts[1].x - pix, pts[1].y,       0.0);
      pts[2].set(pts[2].x - pix, pts[2].y - pix, 0.0);
      pts[3].set(pts[3].x,       pts[3].y - pix, 0.0);
      geom.polylineEye(3, pts+1);

      pts[1].set(pts[0].x - s,  pts[0].y, 0.0);
      pts[2].set(pts[0].x,      pts[0].y, 0.0);
      pts[3].set(pts[0].x,      pts[0].y - s, 0.0);
      geom.polylineEye(3, pts+1);
      pts[1].set(pts[1].x - pix, pts[1].y + pix, 0.0);
      pts[2].set(pts[2].x + pix, pts[2].y + pix, 0.0);
      pts[3].set(pts[3].x + pix, pts[3].y,       0.0);
      geom.polylineEye(3, pts+1);
      break;

    case OdDgElement::kOsModeTan:
      {
        OdGiModelTransformSaver mt(geom, vp.getEyeToWorldTransform());
        geom.circle(pts[0], s, OdGeVector3d::kZAxis);
      }
      pts[1].set(pts[0].x - s, pts[0].y + s, 0.0);
      pts[2].set(pts[0].x + s, pts[0].y + s, 0.0);
      geom.polylineEye(2, pts+1);
      pts[1].set(pts[1].x, pts[1].y + pix, 0.0);
      pts[2].set(pts[2].x, pts[2].y + pix, 0.0);
      geom.polylineEye(2, pts+1);
      break;

    case OdDgElement::kOsModeNear:
      pts[1].set(pts[0].x - s, pts[0].y + s, 0.0);
      pts[2].set(pts[0].x + s, pts[0].y - s, 0.0);
      pts[3].set(pts[0].x - s, pts[0].y - s, 0.0);
      pts[0].set(pts[0].x + s, pts[0].y + s, 0.0);
      geom.polygonEye(4, pts);
      pts[1].set(pts[1].x - pix, pts[1].y + pix, 0.0);
      pts[2].set(pts[2].x + pix, pts[2].y - pix, 0.0);
      pts[3].set(pts[3].x - pix, pts[3].y - pix, 0.0);
      pts[0].set(pts[0].x + pix, pts[0].y + pix, 0.0);
      geom.polygonEye(4, pts);
      break;

    default:
      pts[1].set(pts[0].x - s, pts[0].y + s, 0.0);
      pts[2].set(pts[0].x + s, pts[0].y - s, 0.0);
      geom.polygonEye(2, pts + 1);
      pts[1].set(pts[0].x - s, pts[0].y - s, 0.0);
      pts[2].set(pts[0].x + s, pts[0].y + s, 0.0);
      geom.polygonEye(2, pts + 1);
      pts[1].set(pts[0].x - s - pix, pts[0].y + s + pix, 0.0);
      pts[2].set(pts[0].x - s - pix, pts[0].y - s - pix, 0.0);
      pts[3].set(pts[0].x + s + pix, pts[0].y - s - pix, 0.0);
      pts[0].set(pts[0].x + s + pix, pts[0].y + s + pix, 0.0);
      geom.polygonEye(4, pts);
      break;
    }
  }
  OdGsMarker gsMarker = kNullSubentIndex;
  if(m_centers.size())
  {
    OdGePoint3dArray::size_type i;
    pVpDraw->subEntityTraits().setColor(7);
    for ( i = 0; i < m_centers.size(); ++i )
    {
      traits.setSelectionMarker(++gsMarker);
      const OdGePoint3d& cntr = xWorldToEye * m_centers[i].m_point;
      pts[0].set(cntr.x, cntr.y + s, 0.0);
      pts[1].set(cntr.x, cntr.y - s, 0.0);
      geom.polygonEye(2, pts);
      pts[0].set(cntr.x + s, cntr.y, 0.0);
      pts[1].set(cntr.x - s, cntr.y, 0.0);
      geom.polygonEye(2, pts);
    }
  }
}

void DgOSnapManager::invalidateViewport(const DgOSnapManager::DgHistEntryArray& centers) const
{
  OdGePoint3d pt;
  OdGeMatrix3d xWorldToScr = m_pView->worldToDeviceMatrix();

  OdGsDCRect rc;
  OdGePoint3dArray::size_type i;
  for ( i = 0; i < centers.size(); ++i )
  {
    pt = xWorldToScr * centers[i].m_point;

    rc.m_min.x = OdRoundToLong(pt.x);
    rc.m_min.y = OdRoundToLong(pt.y);
    rc.m_max = rc.m_min;

    rc.m_min.x -= snapPtSize;
    rc.m_min.y -= snapPtSize;
    rc.m_max.x += snapPtSize;
    rc.m_max.y += snapPtSize;
    
    m_pView->invalidate(rc);
  }
}

void DgOSnapManager::invalidateViewport(const OdGePoint3d& point) const
{
  OdGePoint3d pt;
  OdGeMatrix3d xWorldToScr = m_pView->worldToDeviceMatrix();

  OdGsDCRect rc;
  if(m_mode)
  {
    pt = xWorldToScr * point;
    rc.m_min.x = OdRoundToLong(pt.x);
    rc.m_min.y = OdRoundToLong(pt.y);
    rc.m_max = rc.m_min;

    rc.m_min.x -= (snapPtSize * 2);
    rc.m_min.y -= (snapPtSize * 2);
    rc.m_max.x += (snapPtSize * 2);
    rc.m_max.y += (snapPtSize * 2);

      /*
    switch(m_mode)
    {
    case OdDb::kOsModeEnd:
      rc.m_min.x -= snapPtSize;
      rc.m_min.y -= snapPtSize;
      rc.m_max.x += snapPtSize;
      rc.m_max.y += snapPtSize;
      break;

    case OdDb::kOsModeMid:
      pts[1].set(pts[0].x - s * 1.2, pts[0].y - s * 0.6, 0.0);
      pts[2].set(pts[0].x,           pts[0].y + s * 1.4, 0.0);
      pts[3].set(pts[0].x + s * 1.2, pts[0].y - s * 0.6, 0.0);
      geom.polygonEye(3, pts+1);

      pts[1].set(pts[1].x - pix, pts[1].y - pix, 0.0);
      pts[2].set(pts[2].x,       pts[2].y + pix, 0.0);
      pts[3].set(pts[3].x + pix, pts[3].y - pix, 0.0);
      geom.polygonEye(3, pts+1);
      break;

    case OdDb::kOsModeCen:
      geom.pushModelTransform(vp.getEyeToWorldTransform());
      geom.circle(pts[0], s * 1.4, OdGeVector3d::kZAxis);
      geom.popModelTransform();
      break;

    case OdDb::kOsModeQuad:
      pts[1].set(pts[0].x - s,  pts[0].y,     0.0);
      pts[2].set(pts[0].x,      pts[0].y + s, 0.0);
      pts[3].set(pts[0].x + s,  pts[0].y,     0.0);
      pts[0].set(pts[0].x,      pts[0].y - s, 0.0);
      geom.polygonEye(4, pts);
      pts[1].set(pts[1].x - pix,  pts[1].y,       0.0);
      pts[2].set(pts[2].x,        pts[2].y + pix, 0.0);
      pts[3].set(pts[3].x + pix,  pts[3].y,       0.0);
      pts[0].set(pts[0].x,        pts[0].y - pix, 0.0);
      geom.polygonEye(4, pts);
      break;

    case OdDb::kOsModePerp:
      pts[1].set(pts[0].x - s,       pts[0].y + s + pix, 0.0);
      pts[2].set(pts[0].x - s,       pts[0].y - s, 0.0);
      pts[3].set(pts[0].x + s + pix, pts[0].y - s, 0.0);
      geom.polylineEye(3, pts+1);
      pts[1].set(pts[1].x - pix, pts[1].y,       0.0);
      pts[2].set(pts[2].x - pix, pts[2].y - pix, 0.0);
      pts[3].set(pts[3].x,       pts[3].y - pix, 0.0);
      geom.polylineEye(3, pts+1);

      pts[1].set(pts[0].x - s,  pts[0].y, 0.0);
      pts[2].set(pts[0].x,      pts[0].y, 0.0);
      pts[3].set(pts[0].x,      pts[0].y - s, 0.0);
      geom.polylineEye(3, pts+1);
      pts[1].set(pts[1].x - pix, pts[1].y + pix, 0.0);
      pts[2].set(pts[2].x + pix, pts[2].y + pix, 0.0);
      pts[3].set(pts[3].x + pix, pts[3].y,       0.0);
      geom.polylineEye(3, pts+1);
      break;

    case OdDb::kOsModeTan:
      geom.pushModelTransform(vp.getEyeToWorldTransform());
      geom.circle(pts[0], s, OdGeVector3d::kZAxis);
      geom.popModelTransform();
      pts[1].set(pts[0].x - s, pts[0].y + s, 0.0);
      pts[2].set(pts[0].x + s, pts[0].y + s, 0.0);
      geom.polylineEye(2, pts+1);
      pts[1].set(pts[1].x, pts[1].y + pix, 0.0);
      pts[2].set(pts[2].x, pts[2].y + pix, 0.0);
      geom.polylineEye(2, pts+1);
      break;

    case OdDb::kOsModeNear:
      pts[1].set(pts[0].x - s, pts[0].y + s, 0.0);
      pts[2].set(pts[0].x + s, pts[0].y - s, 0.0);
      pts[3].set(pts[0].x - s, pts[0].y - s, 0.0);
      pts[0].set(pts[0].x + s, pts[0].y + s, 0.0);
      geom.polygonEye(4, pts);
      pts[1].set(pts[1].x - pix, pts[1].y + pix, 0.0);
      pts[2].set(pts[2].x + pix, pts[2].y - pix, 0.0);
      pts[3].set(pts[3].x - pix, pts[3].y - pix, 0.0);
      pts[0].set(pts[0].x + pix, pts[0].y + pix, 0.0);
      geom.polygonEye(4, pts);
      break;

    default:
      pts[1].set(pts[0].x - s, pts[0].y + s, 0.0);
      pts[2].set(pts[0].x + s, pts[0].y - s, 0.0);
      geom.polygonEye(2, pts + 1);
      pts[1].set(pts[0].x - s, pts[0].y - s, 0.0);
      pts[2].set(pts[0].x + s, pts[0].y + s, 0.0);
      geom.polygonEye(2, pts + 1);
      pts[1].set(pts[0].x - s - pix, pts[0].y + s + pix, 0.0);
      pts[2].set(pts[0].x - s - pix, pts[0].y - s - pix, 0.0);
      pts[3].set(pts[0].x + s + pix, pts[0].y - s - pix, 0.0);
      pts[0].set(pts[0].x + s + pix, pts[0].y + s + pix, 0.0);
      geom.polygonEye(4, pts);
      break;
    }
    */
    m_pView->invalidate(rc);
  }
}

OdUInt32 DgOSnapManager::subSetAttributes(OdGiDrawableTraits* ) const
{
  return kDrawableNone;
}

bool DgOSnapManager::subWorldDraw(OdGiWorldDraw* ) const
{
  return false;
}

#define hitradius 15

#define maxhistory 7

bool DgOSnapManager::snap(OdGsView* pView, OdGePoint3d& point, const OdGePoint3d* pLastPoint)
{
  m_bRedraw = false;
  m_snapPointsBuff.clear();
  m_pView = pView;
  m_pPickPoint = &point;
  m_pLastPoint = pLastPoint;

  DgHistEntryArray prevCenters(m_centers);
  OdGePoint3d prevPoint(m_snapPoint);
  OdDgElement::OsnapMode prevMode(m_mode);

  if(m_mode==0 || !checkpoint(m_mode, m_snapPoint))
  {
    m_dNearDist = std::numeric_limits<double>::max();
    m_snapPoint = OdGePoint3d(1e100, 1e100, 1e100);
    m_mode = OdDgElement::OsnapMode(100);
  }

  OdGePoint2d pt = (pView->worldToDeviceMatrix() * point).convert2d();
  OdGsDCPoint pts[2];
  pts[0].x = OdRoundToLong(pt.x) - hitradius;
  pts[1].x = pts[0].x + hitradius * 2;
  pts[0].y = OdRoundToLong(pt.y) - 5;
  pts[1].y = pts[0].y + hitradius * 2;
  m_dWorldToDevice = pView->worldToDeviceMatrix().getCsXAxis().length();
  pView->select(pts, 2, this);

/*
  unsigned nSnapModes = m_nSnapModes &
                          (
                            OdDb::kOsModeCen|
                            OdDb::kOsModeIns|
                            OdDb::kOsModePerp|
                            OdDb::kOsModeTan|
                            OdDb::kOsModePar
                          );
  OdGeMatrix3d xWorldToEye = pView->viewingMatrix();

  for ( unsigned i = 0; i < m_history.size(); ++i )
  {
    const HistEntry& entry = m_history[i];
    OdDbEntityPtr pEnt = entry.m_objectId.safeOpenObject();
    checkSnapPoints(pEnt, nSnapModes, entry.m_gsMarker,
      entry.m_xModelToWorld, entry.m_xModelToWorld.inverse(), xWorldToEye);
  }
*/
  if(m_mode > 0 && m_mode < 100)
  {
    point = m_snapPoint;
/*
    if(m_history.size() > maxhistory)
      m_history.erase(m_history.begin());
    HistEntry& histEntry = *m_history.append();
*/
  }
  else
  {
    if(prevMode > 0 && prevMode < 100)
      invalidateViewport(prevPoint);
    m_mode = OdDgElement::OsnapMode(0);
  }
  bool bRes = true;
  if(m_snapPoint == prevPoint)
    bRes = false;
  else
  {
    if(prevPoint.x < 1e100)
      invalidateViewport(prevPoint);
    if(m_snapPoint.x < 1e100)
      invalidateViewport(m_snapPoint);
  }
  return bRes|m_bRedraw;
}

bool DgOSnapManager::selected(const OdGiDrawableDesc& )
{
  return false;
}

inline bool DgOSnapManager::checkpoint(OdDgElement::OsnapMode osm, const OdGePoint3d& point)
{
  //double dist = (point-*m_pPickPoint).length() * m_dWorldToDevice;
  OdGeMatrix3d xWorldToScr = m_pView->worldToDeviceMatrix();
  OdGePoint2d p1((xWorldToScr * *m_pPickPoint).convert2d());
  OdGePoint2d p2((xWorldToScr * point).convert2d());
  double dist = (p1 - p2).length();

  if(dist < hitradius)
  {
    if(dist < m_dNearDist && osm <= m_mode)
    {
      m_dNearDist = dist;
      m_snapPoint = point;
      m_mode = osm;
      return true;
    }
  }
  return false;
}

const int nMaxHist = 7;

//template <class A, class Size>
bool DgOSnapManager::appendToQueue(DgOSnapManager::DgHistEntryArray& array, const DgHistEntry& entry)
{
  if(!array.contains(entry))
  {
    if(array.size() > nMaxHist)
    {
      array.erase(array.begin());
    }
    array.append(entry);
    return true;
  }
  return false;
}

void DgOSnapManager::checkSnapPoints(const OdDgElement* pEnt,
                                   const OdGiPathNode& pathNode,
                                   unsigned nSnapModes,
                                   OdGsMarker gsMarker,
                                   const OdGeMatrix3d& xModelToWorld,
                                   const OdGeMatrix3d& xWorldToModel,
                                   const OdGeMatrix3d& xWorldToEye)
{
  OdGePoint3d modelPickPt = xWorldToModel * *m_pPickPoint;
  OdGePoint3d modelLastPt;
  if(m_pLastPoint)
  {
    modelLastPt = xWorldToModel * *m_pLastPoint;
  }

  for ( OdDgElement::OsnapMode osm = OdDgElement::kOsModeEnd; osm <= OdDgElement::kOsModeNear; osm = OdDgElement::OsnapMode(osm+1) )
  {
    if ( nSnapModes & (1<<osm) )
    {
      if(pEnt->isDBRO() && pEnt->getOsnapPoints(OdDgElement::OsnapMode(osm),
                              gsMarker,
                              modelPickPt,
                              modelLastPt,
                              xWorldToEye,
                              m_snapPointsBuff
                             ) == eOk
        )
      {
        OdGePoint3dArray::size_type i;
        for(i=0; i<m_snapPointsBuff.size(); ++i)
        {
          OdGePoint3d& point = m_snapPointsBuff[i];
          point.transformBy(xModelToWorld);
          checkpoint(OdDgElement::OsnapMode(osm), point);
          switch(osm)
          {
          case OdDgElement::kOsModeCen:
            appendToQueue(m_centers, DgHistEntry(pathNode, point));
            m_bRedraw = true;
            break;
          }
        }
        m_snapPointsBuff.clear();
      }
    }
  }
}

OdUInt32 DgOSnapManager::selected(const OdGiPathNode& pathNode, const OdGiViewport& viewInfo)
{
  if(pathNode.transientDrawable()==this)
  {
    OdGsMarker gsMarker = pathNode.selectionMarker();
    if(gsMarker > -1)
    {
      if((m_nSnapModes & (1<<OdDgElement::kOsModeCen)) && (OdGsMarker)m_centers.size() > gsMarker)
      {
        checkpoint(OdDgElement::kOsModeCen, m_centers[gsMarker].m_point);
      }
    }
    return OdUInt32(kContinue);
  }

  OdDgElementPtr pEnt = OdDgElement::cast(OdDgElementId(pathNode.persistentDrawableId()).openObject());
  if(pEnt.isNull())
    return OdUInt32(kSkipDrawable);

  OdGeMatrix3d xModelToWorld = viewInfo.getEyeToWorldTransform() * viewInfo.getModelToEyeTransform();
  OdGeMatrix3d xWorldToModel = viewInfo.getEyeToModelTransform() * viewInfo.getWorldToEyeTransform();

  unsigned nSnapModes = m_nSnapModes;
  if(!m_pLastPoint)
  {
    SETBIT(nSnapModes, (1<<OdDgElement::kOsModePerp)|(1<<OdDgElement::kOsModeTan)|(1<<OdDgElement::kOsModePerp), 0);
  }

  checkSnapPoints(pEnt,
                  pathNode,
                  nSnapModes,
                  pathNode.selectionMarker(),
                  xModelToWorld,
                  xWorldToModel,
                  viewInfo.getWorldToEyeTransform());

  return OdUInt32(kContinue);
}
