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
#include "OSnapManager.h"
#include "Gi/GiPathNode.h"
#include "OdRound.h"
#include "SaveState.h"
#define STL_USING_LIMITS
#include "OdaSTL.h"

#include "DbBlockTableRecord.h"
#include "DbBlockReference.h"
#include "DbLayout.h"
#include "DbHostAppServices.h"
#include "Gi/GiLocalDrawableDesc.h"
#include "Gs/GsViewImpl.h"

#define snapPtSize 5

OdBaseSnapManager::SubentId::SubentId(const OdGiPathNode& giPath)
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

bool OdBaseSnapManager::SubentId::operator == (const SubentId& op) const
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

#define hitradius 15

//#define maxhistory 7

OdBaseSnapManager::OdBaseSnapManager()
  : m_pTracker(NULL)
  , m_mode(OdDb::OsnapMode(0))
  , m_pView(NULL)
  , m_hitRadius(hitradius)
  , m_pPickPoint(NULL)
  , m_pLastPoint(NULL)
  , m_dWorldToDevice(0.0)
  , m_dNearDist(std::numeric_limits<double>::max())
  , m_bRedraw(false)
{
}

OSnapManager::OSnapManager()
  : m_nSnapModes(0xFFFFFFFF)
{
}

unsigned OSnapManager::snapModes() const 
{ 
  return m_nSnapModes; 
}

void OSnapManager::setSnapModes(unsigned nSnapModes)
{ 
  m_nSnapModes = nSnapModes; 
}

void OdBaseSnapManager::reset()
{
  m_centers.clear();
}

OdInt32 OdBaseSnapManager::getAperture(OdDbDatabase *pDb) const
{
  return pDb->appServices()->getAPERTURE();
}

void OdBaseSnapManager::subViewportDraw(OdGiViewportDraw* pVpDraw) const
{
  OdGePoint3d pts[4];
  OdGiViewportGeometry& geom = pVpDraw->geometry();
  OdGiViewport& vp = pVpDraw->viewport();
  OdGeMatrix3d xWorldToEye = vp.getWorldToEyeTransform();
  vp.getNumPixelsInUnitSquare(vp.getCameraTarget() /*OdGePoint3d::kOrigin*/, (OdGePoint2d&)pts[0]);
  double pix = 1. / pts[0].x;
  double s = snapPtSize * pix;

  OdGiSubEntityTraits& traits = pVpDraw->subEntityTraits();
  OdGiDrawFlagsHelper _dfh(traits, OdGiSubEntityTraits::kDrawNoPlotstyle);
  if ((m_mode > 0) && ((OdUInt32)m_mode < 100))
  {
    traits.setTrueColor(snapTrueColor());

    traits.setFillType(kOdGiFillNever);
    traits.setSelectionMarker(kNullSubentIndex);
    pts[0] = xWorldToEye * m_snapPoint;
    vp.doPerspective(pts[0]);

    switch(m_mode)
    {
    case OdDb::kOsModeEnd:
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
      {
        OdGiModelTransformSaver mt(geom, vp.getEyeToWorldTransform());
        geom.circle(pts[0], s * 1.4, OdGeVector3d::kZAxis);
      }
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
  }
  OdGsMarker gsMarker = 0;
  if(m_centers.size())
  {
    pVpDraw->subEntityTraits().setTrueColor(centerTrueColor());

    for(OdUInt32 i=0; i < m_centers.size(); i++, gsMarker++)
    {
      traits.setSelectionMarker(gsMarker);

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

void OdBaseSnapManager::invalidateViewport(const OdBaseSnapManager::HistEntryArray& centers) const
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

void OdBaseSnapManager::invalidateViewport(const OdGePoint3d& point) const
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

OdUInt32 OdBaseSnapManager::subSetAttributes(OdGiDrawableTraits* ) const
{
  return kDrawableNone;
}

bool OdBaseSnapManager::subWorldDraw(OdGiWorldDraw* ) const
{
  return false;
}

bool OdBaseSnapManager::snap(OdGsView* pView, OdGePoint3d& point, const OdGePoint3d* pLastPoint)
{
  OdEdPointTrackerWithSnapInfo *pTrackerSnapInfo = dynamic_cast<OdEdPointTrackerWithSnapInfo *>(m_pTracker);

  if ( pTrackerSnapInfo )
    pTrackerSnapInfo->m_SnapContext.bValid = false;

  m_bRedraw = false;
  m_snapPointsBuff.clear();
  m_pView = pView;
  m_pPickPoint = &point;
  m_pLastPoint = pLastPoint;

  HistEntryArray prevCenters(m_centers);
  OdGePoint3d prevPoint(m_snapPoint);
  OdDb::OsnapMode prevMode(m_mode);

  if(m_mode==0 || !checkpoint(m_mode, m_snapPoint))
  {
    m_dNearDist = std::numeric_limits<double>::max();
    m_snapPoint = OdGePoint3d(1e100, 1e100, 1e100);
    m_mode = OdDb::OsnapMode(100);
  }

  OdGePoint2d pt = (pView->worldToDeviceMatrix() * point).convert2d();
  OdGsDCPoint pts[2];
  OdInt32 aperture = getAperture(static_cast<OdDbDatabase*>(pView->userGiContext()->database()));
  pts[0].x = OdRoundToLong(pt.x) - aperture;
  pts[1].x = pts[0].x + aperture * 2;
  pts[0].y = OdRoundToLong(pt.y) - aperture;
  pts[1].y = pts[0].y + aperture * 2;
  m_hitRadius = (double)aperture;
  m_dWorldToDevice = pView->worldToDeviceMatrix().getCsXAxis().length();

  OdGsViewImpl *pViewImpl = dynamic_cast<OdGsViewImpl *>(pView);
  if ( pViewImpl )
    pViewImpl->setSnapping(true);
  
  m_selectedEntityDataArray.clear();

  pView->select(pts, 2, this);

  if(m_selectedEntityDataArray.size()) { // dna: only one can be selected currently
    checkSnapPoints(m_selectedEntityDataArray[0], pViewImpl->worldToEyeMatrix());
  }

  if ( pViewImpl )
    pViewImpl->setSnapping(false);

/*
  unsigned nSnapModes = snapModes() &
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
  if(m_mode > 0 && (OdUInt32)m_mode < 100)
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
    if(prevMode > 0 && (OdUInt32)prevMode < 100)
      invalidateViewport(prevPoint);
    m_mode = OdDb::OsnapMode(0);
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

bool OdBaseSnapManager::selected(const OdGiDrawableDesc& )
{
  return false;
}

inline bool OdBaseSnapManager::checkpoint(OdDb::OsnapMode osm, const OdGePoint3d& point)
{
  //double dist = (point-*m_pPickPoint).length() * m_dWorldToDevice;
  OdGeMatrix3d xWorldToScr = m_pView->worldToDeviceMatrix();
  OdGePoint2d p1((xWorldToScr * *m_pPickPoint).convert2d());
  OdGePoint2d p2((xWorldToScr * point).convert2d());
  double dist = (p1 - p2).length();

  OdEdPointTrackerWithSnapInfo *pTrackerSnapInfo = dynamic_cast<OdEdPointTrackerWithSnapInfo *>( m_pTracker );

  if(dist < m_hitRadius)
  {
    if(dist < m_dNearDist/* && osm <= m_mode*/)
    {
      m_dNearDist = dist; 
      m_snapPoint = point;
      m_mode = osm;
      
      if ( pTrackerSnapInfo )
      {
        pTrackerSnapInfo->m_SnapContext.point = point;
        pTrackerSnapInfo->m_SnapContext.mode = osm;
        pTrackerSnapInfo->m_SnapContext.bValid = true;
      }
      return true;
    }
    else if (dist == m_dNearDist)
      return true;
  }
  return false;
}

const int nMaxHist = 7;

//template <class A, class Size>
bool OdBaseSnapManager::appendToQueue(OdBaseSnapManager::HistEntryArray& array, const HistEntry& entry)
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

void OdBaseSnapManager::checkSnapPoints(
  const SelectedEntityData& data,
  const OdGeMatrix3d& xWorldToEye)
{
  OdGeMatrix3d xModelToWorld = data.xModelToWorld;
  bool insertionMatrix = (xModelToWorld!=OdGeMatrix3d::kIdentity);
  OdGePoint3d modelPickPt = xModelToWorld.inverse() * *m_pPickPoint;
  OdGePoint3d modelLastPt;
  unsigned nSnapModes = snapModes();
  if (m_pLastPoint)
  {
    modelLastPt = xModelToWorld * *m_pLastPoint;
  }
  else
  {
    SETBIT(nSnapModes, toSnapModes(OdDb::kOsModePerp) | toSnapModes(OdDb::kOsModeTan), 0);
  }

  OdEdPointTrackerWithSnapInfo *pTrackerSnapInfo = dynamic_cast<OdEdPointTrackerWithSnapInfo *>( m_pTracker );

  OdDbEntityPtr pEnt = data.subentId.m_path.first().safeOpenObject();
  OdGsMarker gsMarker = data.subentId.m_gsMarker;
  if ( !pTrackerSnapInfo )
  {
    for ( OdDb::OsnapMode osm = OdDb::kOsModeEnd; osm <= OdDb::kOsModeNear; osm = OdDb::OsnapMode( osm + 1 ) )
    {
      if (nSnapModes & toSnapModes(osm)) // so not all types are tested
      {
        OdResult res;
        if ( insertionMatrix ) {
          res = pEnt->getOsnapPoints(osm,
            gsMarker,
            modelPickPt,
            modelLastPt,
            xWorldToEye,
            m_snapPointsBuff,
            xModelToWorld
          );
        }
        else {
          res = pEnt->getOsnapPoints(osm,
            gsMarker,
            modelPickPt,
            modelLastPt,
            xWorldToEye,
            m_snapPointsBuff
          );
        }
        if ( res == eOk )
        {
          OdGePoint3dArray::size_type i;
          for ( i = 0; i < m_snapPointsBuff.size(); ++i )
          {
            OdGePoint3d& point = m_snapPointsBuff[i];
            point.transformBy( xModelToWorld );
            checkpoint(osm, point);
            switch ( osm )
            {
              case OdDb::kOsModeCen:
                appendToQueue( m_centers, HistEntry( data.subentId, point ) );
                m_bRedraw = true;
                break;
              default:
                // no op
                break;
            }
          }
          m_snapPointsBuff.clear();
        }
      }
    }
  }
  else
  {
    if ( !pTrackerSnapInfo->isTargetEntity( pEnt ) )
    {
      return;
    }

    OdSaveState<double> ssHitRadius(m_hitRadius, 1500.);

    OdArray<OdDb::OsnapMode> snapModes;

    pTrackerSnapInfo->getSnapModes( pEnt, snapModes );

    OdArray<OdDb::OsnapMode>::iterator it;
    for ( it = snapModes.begin(); it != snapModes.end(); it++ )
    {
      if ( pEnt->getOsnapPoints( *it,
                                gsMarker,
                                modelPickPt,
                                modelLastPt,
                                xWorldToEye,
                                m_snapPointsBuff
                               ) == eOk
          )
      {
        pTrackerSnapInfo->m_SnapContext.entId = pEnt->objectId();
        pTrackerSnapInfo->m_SnapContext.marker = gsMarker;

        OdGePoint3dArray::size_type i;
        for ( i = 0; i < m_snapPointsBuff.size(); ++i )
        {
          OdGePoint3d& point = m_snapPointsBuff[i];
          point.transformBy( xModelToWorld );
          checkpoint( *it, point );
        }
        
      }
    }
  }
}

OdUInt32 OdBaseSnapManager::selected(const OdGiPathNode& pathNode, const OdGiViewport& viewInfo)
{
  if(pathNode.transientDrawable()==this)
  {
    OdGsMarker gsMarker = pathNode.selectionMarker();
    if(gsMarker > -1)
    {
      if((snapModes() & toSnapModes(OdDb::kOsModeCen)) && (OdGsMarker)m_centers.size() > gsMarker)
      {
        checkpoint(OdDb::kOsModeCen, m_centers[gsMarker].m_point);
      }
    }
    return OdUInt32(kContinue);
  }

  OdDbEntityPtr pEnt = OdDbEntity::cast(OdDbObjectId(pathNode.persistentDrawableId()).openObject());
  if(pEnt.isNull())
    return OdUInt32(kSkipDrawable);

  m_selectedEntityDataArray.append()->set(pathNode);

  //DNA: 16/10/2018 fix below seems incorrect because snapping works only with first selected entity
  // (but there might be few close to or overlapping each other)
  //FAE: 07/27/2016 Changes due tio small performance of snap for DGNUnderlay
  // Of we found something - we should stop the process
  return OdUInt32(kSkipDrawable);
}
//----------------------------------------------------------------------------------------------------------------------------
// guv: recalculation m_centers, on case when entity move or delete

void OdBaseSnapManager::Recalc_Entity_centers()
{
  for(int i = m_centers.size()-1; i >= 0; --i)
  {
    SubentId sub_id = m_centers[i].m_subentId;

    if (sub_id.m_path.size() <= 0) continue;

    OdDbEntityPtr pEnt = OdDbEntity::cast(sub_id.m_path[0].openObject());

    if (pEnt.isNull())                            // Entity delete
    {  
      m_centers.erase(m_centers.begin() + i);
      continue;
    }

    OdGePoint3dArray snapPoints;
    pEnt->getOsnapPoints(OdDb::kOsModeCen, OdGsMarker(), OdGePoint3d(), OdGePoint3d(), OdGeMatrix3d(), snapPoints);

    if (snapPoints.size() > 0)
    {
      m_centers[i].m_point = snapPoints[0];       // recalculation center
    }

  }
}
//----------------------------------------------------------------------------------------------------------------------------
// guv: calculation m_centers for all entitys

bool OdBaseSnapManager::Set_Entity_centers(OdRxObject* pRxDb)
{
  m_centers.clear();

  OdDbDatabase* pDb = OdDbDatabase::cast(pRxDb).get();
  if (!pDb)
    return false;

  OdDbBlockTableRecordPtr pBTR = pDb->getActiveLayoutBTRId().safeOpenObject();  // Layout table

  if (pDb->getModelSpaceId() != pBTR->objectId())                               // it's not ModelSpace, it's PaperSpace
  {                                                                             // which can have many ModelSpace
    OdDbLayoutPtr pLayout = pBTR->getLayoutId().safeOpenObject();              

    if (pLayout->overallVportId() != OdDbObjectId(pDb->activeViewportId()))
    {
      pBTR = pDb->getModelSpaceId().safeOpenObject();                           // get active ModelSpace for PaperSpace
    }
  }

  Set_Entity_centers(pBTR);
  return true;
}
//----------------------------------------------------------------------------------------------------------------------------

void OdBaseSnapManager::Set_Entity_centers(OdDbBlockTableRecord* pBTR, const OdGeMatrix3d& matrix)
{
  OdGiDrawableDesc *dd0 = NULL;
  OdGiLocalDrawableDesc dd(dd0);         // need for build OdGiPathNode

  for(OdDbObjectIteratorPtr pIter=pBTR->newIterator(); !pIter->done() && m_centers.size()<nMaxHist; pIter->step() )
  {
    OdDbEntityPtr pEnt = pIter->entity();

    OdDbBlockReferencePtr pBR = OdDbBlockReference::cast(pEnt);
    if (!pBR.isNull())
    {
      OdDbBlockTableRecordPtr pBRec = OdDbBlockTableRecord::cast(pBR->blockTableRecord().openObject());
      if (!pBRec.isNull())
        Set_Entity_centers(pBRec, pBR->blockTransform());
      continue;
    }

    if (pEnt.isNull()) continue;         // Entity delete

    dd.persistId = pEnt->objectId();

    OdGePoint3dArray snapPoints;
    pEnt->getOsnapPoints(OdDb::kOsModeCen, OdGsMarker(), OdGePoint3d::kOrigin, OdGePoint3d::kOrigin, OdGeMatrix3d(), snapPoints);

    for(OdUInt32 i=0; i<snapPoints.size() && m_centers.size() < nMaxHist; i++)
    {
      m_centers.append(HistEntry(dd, snapPoints[i].transformBy(matrix) ));
    }
  }
}

void OdBaseSnapManager::track( OdEdInputTracker* pTracker )
{
  //ODA_ASSERT_VAR(OdEdPointTrackerWithSnapInfo* pTrackerSnapInfo = dynamic_cast<OdEdPointTrackerWithSnapInfo*>(pTracker);)
  m_pTracker = pTracker;
}

//----------------------------------------------------------------------------------------------------------------------------
