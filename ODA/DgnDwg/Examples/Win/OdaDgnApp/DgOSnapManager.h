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

#ifndef __ODDGEDOSNAPMAN_H__
#define __ODDGEDOSNAPMAN_H__  /** {Secret} **/



#include "Gi/GiDrawableImpl.h"
#include "Gs/Gs.h"
#include "Si/SiSpatialIndex.h"
#include "DgElement.h"
#include "Gs/GsSelectionReactor.h"



class OdEdOSnapMan;
typedef OdSmartPtr<OdEdOSnapMan> OdEdOSnapManPtr;

class DgOSnapManager : public OdGiDrawableImpl<OdGiDrawable>,
                       public OdGsSelectionReactor
{
  void checkSnapPoints(const OdDgElement* pEnt,
    const OdGiPathNode& pathNode,
    unsigned nSnapModes,
    OdGsMarker gsMarker,
    const OdGeMatrix3d& xModelToWorld,
    const OdGeMatrix3d& xWorldToModel,
    const OdGeMatrix3d& xWorldToEye);

  bool checkpoint(OdDgElement::OsnapMode osm, const OdGePoint3d& point);

  OdGsView*           m_pView;
  OdGePoint3d*        m_pPickPoint;
  const OdGePoint3d*  m_pLastPoint;
  unsigned            m_nSnapModes;
  OdGePoint3dArray    m_snapPointsBuff;

  double              m_dWorldToDevice;
  double              m_dNearDist;
  OdGePoint3d         m_snapPoint;
  OdDgElement::OsnapMode m_mode;
  bool                m_bRedraw;


  struct DgSubentId
  {
    DgSubentId() { }
    OdDgElementIdArray m_path;
    OdGsMarker  m_gsMarker;
    DgSubentId(const OdGiPathNode& giPath);
    bool operator == (const DgSubentId& op) const;
  };

  struct DgHistEntry
  {
    DgHistEntry()
    {
    }
    DgHistEntry(const OdGiPathNode& giPath, const OdGePoint3d& point)
      : m_subentId(giPath)
      , m_point(point)
    {
    }
    bool operator == (const DgHistEntry& op) const
    {
      return op.m_subentId==m_subentId;
    }

    DgSubentId          m_subentId;
    OdGePoint3d       m_point;
  };

  typedef OdArray<DgHistEntry> DgHistEntryArray;
  static bool appendToQueue(DgHistEntryArray& array, const DgHistEntry& entry);

  DgHistEntryArray      m_centers;
  
  OdUInt32 subSetAttributes(OdGiDrawableTraits* ) const;
  bool subWorldDraw(OdGiWorldDraw* pWd) const;
  void subViewportDraw(OdGiViewportDraw*) const;

  bool selected(const OdGiDrawableDesc& pDrawableDesc);
  OdUInt32 selected(const OdGiPathNode& pathNode, const OdGiViewport& viewInfo);
  void invalidateViewport(const OdGePoint3d& point) const;
  void invalidateViewport(const DgHistEntryArray& centers) const;
protected:
  DgOSnapManager();
public:

  bool snap(OdGsView* pView, OdGePoint3d& point, const OdGePoint3d* pLastPoint);

  unsigned snapModes() const { return m_nSnapModes; }

  void setSnapModes( unsigned nSnapModes ) { m_nSnapModes = nSnapModes; }

  void reset();
};

#endif // #ifndef __ODDGEDOSNAPMAN_H__
