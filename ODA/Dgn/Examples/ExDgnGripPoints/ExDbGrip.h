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




#ifndef _EXDBGRIPDATA_INCLUDED_
#define _EXDBGRIPDATA_INCLUDED_ /* {Secret} */

#include "TD_PackPush.h"

#include "DbGrip.h"

typedef bool (*GripWorldDrawPtr)(
  OdDbGripData* pThis,
  OdGiWorldDraw* pWd,
  OdDbStub* entId,
  OdDbGripOperations::DrawType type,
  OdGePoint3d* imageGripPoint,
  double dGripSize );

typedef void (*GripViewportDrawPtr)(
  OdDbGripData* pThis,
  OdGiViewportDraw* pWd,
  OdDbStub* entId,
  OdDbGripOperations::DrawType type,
  OdGePoint3d* imageGripPoint,
  int gripSize );

typedef OdResult (*GripOperationPtr)(
  OdDbGripData* pThis,
  OdDbStub* entId,
  int iContextFlags );

typedef void (*GripOpStatusPtr)(
  OdDbGripData* pThis,
  OdDbStub* entId,
  OdDbGripOperations::GripStatus status );

typedef OdString (*GripToolTipPtr)( OdDbGripData* pThis );

typedef void (*ContextMenuItemIndexPtr)( unsigned int itemIndex );

//typedef void (*GripDimensionPtr)(
//  OdDbGripData* pThis,
//  OdDbStub* entId,
//  double dimScale,
//  OdDbDimDataPtrArray& dimData );

//typedef OdResult (*GripRtClkHandler)(
//  OdDbGripDataArray& hotGrips,
//  const OdDbStubPtrArray& ents, 
//  OdChar *& menuName, HMENU& menu,
//  ContextMenuItemIndexPtr& cb );

//typedef OdResult (*GripInputPointPtr)(
//  OdDbGripData* pThis, OdDbStub* entId, bool& changedPoint,
//  OdGePoint3d& newPoint, const OdGiViewport& viewContext,
//  OdDbStub* viewportId, bool pointComputed, int history,
//  const OdGePoint3d& lastPoint, const OdGePoint3d& rawPoint,
//  const OdGePoint3d& grippedPoint, const OdGePoint3d& cartesianSnappedPoint,
//  const OdGePoint3d& osnappedPoint, OdDb::OsnapMask osnapMask,
//  const OdArray<OdDbCustomOsnapMode*>& customOsnapModes,
//  OdDb::OsnapMask osnapOverrides,
//  const OdArray<OdDbCustomOsnapMode*>& customOsnapOverrides,
//  const OdDbStubPtrArray& pickedEntities,
//  const OdArray<OdDbStubPtrArray>& nestedPickedEntities,
//  const OdArray<int>& gsSelectionMark,
//  const OdDbStubPtrArray& keyPointEntities,
//  const OdArray<OdDbStubPtrArray>& nestedKeyPointEntities,
//  const OdArray<int>& keyPointGsSelectionMark,
//  const OdArray<OdGeCurve3d*>& alignmentPaths, const OdGePoint3d& computedPoint); 


class ExDbGripData : public OdDbGripData
{
public:
  ExDbGripData();

  ExDbGripData( const ExDbGripData& );

  ExDbGripData& operator = ( const ExDbGripData& );

  virtual OdGePoint3d gripPoint() const
  {
    return m_gripPt;
  }

  virtual DrawStatus worldDraw(
    OdGiWorldDraw* pWd,
    const OdDbObjectId& entId,
    OdDbGripOperations::DrawType type,
    OdGePoint3d* imageGripPoint,
    double dGripSize) const
  {
    if(m_pWorldDraw)
      return (*m_pWorldDraw)(const_cast<ExDbGripData*>(this),
        pWd, entId, type, imageGripPoint, dGripSize) ? kWorldDraw : kViewportDraw;
    return kDefaultDraw;
  }

  virtual void viewportDraw(
    OdGiViewportDraw* pVd,
    const OdDbObjectId& entId,
    OdDbGripOperations::DrawType type,
    OdGePoint3d* imageGripPoint,
    int gripSize ) const 
  {
    if(m_pViewportDraw)
      (*m_pViewportDraw)(const_cast<ExDbGripData*>(this),
      pVd, entId, type, imageGripPoint, gripSize);
  }

  virtual unsigned int bitFlags() const
  {
    return m_bitFlags;
  }

  virtual void status( const OdDbObjectId& entId, OdDbGripOperations::GripStatus status ) const
  {
    if(m_pGripOpStatFunc)
      (*m_pGripOpStatFunc)(const_cast<ExDbGripData*>(this), entId, status);
  }

  virtual bool toolTip( OdString& res ) const
  {
    if(m_pToolTipFunc)
    {
      OdString s = (*m_pToolTipFunc)(const_cast<ExDbGripData*>(this));
      if(!s.isEmpty())
      {
        res = s;
        return true;
      }
    }
    return false;
  }

  virtual bool alternateBasePoint( OdGePoint3d& res ) const
  {
    if(m_pAltBasePt)
    {
      res = *m_pAltBasePt;
      return true;
    }
    return false;
  }

  virtual OdResult hot( const OdDbObjectId& entId, int iContextFlags )
  {
    if(m_pHotGripFunc)
      return (*m_pHotGripFunc)(const_cast<ExDbGripData*>(this), entId, iContextFlags);
    return eNotImplemented;
  }

  virtual OdResult hover( const OdDbObjectId& entId, int iContextFlags )
  {
    if(m_pHoverFunc)
      return (*m_pHoverFunc)(const_cast<ExDbGripData*>(this), entId, iContextFlags);
    return eNotImplemented;
  }

  void setGripPoint( const OdGePoint3d& pt );

  void* appData() const;
  void setAppData( void* pAppData );
  OdRxObjectPtr appDataPtr() const;
  void setAppDataPtr( const OdRxObject* pAppData );
  int appDataInt() const;
  void setAppDataInt( int nAppData );

  GripOperationPtr hotGripFunc() const;
  void setHotGripFunc( GripOperationPtr pFunc );

  GripOperationPtr hoverFunc() const;
  void setHoverFunc( GripOperationPtr pFunc );

  GripWorldDrawPtr worldDraw() const;
  void setWorldDraw( GripWorldDrawPtr pFunc );
  
  GripViewportDrawPtr viewportDraw() const;
  void setViewportDraw( GripViewportDrawPtr pFunc );
  
  GripOpStatusPtr gripOpStatFunc() const;
  void setGripOpStatFunc( GripOpStatusPtr pFunc );

  GripToolTipPtr toolTipFunc() const;
  void setToolTipFunc( GripToolTipPtr pFunc );

  OdGePoint3d* alternateBasePoint() const;
  void setAlternateBasePoint( OdGePoint3d* altBasePt );

  void setBitFlags( unsigned int flags );

  bool skipWhenShared() const;
  void setSkipWhenShared( bool skip );

  bool isRubberBandLineDisabled() const;
  void disableRubberBandLine( bool disable );

  bool areModeKeywordsDisabled() const;
  void disableModeKeywords( bool disable );

  bool drawAtDragImageGripPoint() const;
  void setDrawAtDragImageGripPoint( bool atDragPoint );

  bool triggerGrip() const;
  void setTriggerGrip( bool trigger );

  bool forcedPickOn() const;
  void setForcedPickOn( bool on );

  bool mapGripHotToRtClk() const;
  void setMapGripHotToRtClk( bool on );

  bool gizmosEnabled() const;
  void setGizmosEnabled( bool on );

  //GripDimensionPtr hoverDimensionFunc() const;
  //void setHoverDimensionFunc( GripDimensionPtr pFunc );

  //GripDimensionPtr hotGripDimensionFunc() const;
  //void setHotGripDimensionFunc( GripDimensionPtr pFunc );

  //GripRtClkHandler rtClk() const;
  //void setRtClk( GripRtClkHandler pFunc );

  //GripInputPointPtr inputPointFunc() const;
  //void setInputPointFunc( GripInputPointPtr pFunc );

private:
  union Data
  {
    int   _n;
    void* _p;
  }                       m_appData;
  OdRxObjectPtr           m_pAppData;
  OdGePoint3d             m_gripPt;
  OdGePoint3d*            m_pAltBasePt;
  GripOperationPtr        m_pHotGripFunc;
  GripOperationPtr        m_pHoverFunc;
  GripWorldDrawPtr        m_pWorldDraw;
  GripViewportDrawPtr     m_pViewportDraw;
  GripOpStatusPtr         m_pGripOpStatFunc;
  GripToolTipPtr          m_pToolTipFunc;
  unsigned int            m_bitFlags;
  //GripInputPointPtr   m_pInputPointFunc;
  //GripRtClkHandler    m_pRtClk;
  //GripDimensionPtr    m_pHoverDimensionFunc;
  //GripDimensionPtr    m_pHotGripDimensionFunc;
};

//
// Inline methods
//

inline ExDbGripData::ExDbGripData()
{
  m_gripPt = OdGePoint3d::kOrigin;
  m_pAltBasePt = 0;
  m_appData._n = 0;
  m_pHotGripFunc = 0;
  m_pHoverFunc = 0;
  m_pWorldDraw = 0;
  m_pViewportDraw = 0;
  m_pGripOpStatFunc = 0;
  m_pToolTipFunc = 0;
  m_bitFlags = 0;
}

inline ExDbGripData::ExDbGripData( const ExDbGripData& rData )
{
  m_gripPt = rData.gripPoint();
  m_appData = rData.m_appData;
  m_pAppData = rData.m_pAppData;
  m_pHotGripFunc = rData.hotGripFunc();
  m_pHoverFunc = rData.hoverFunc();
  m_pWorldDraw = rData.worldDraw();
  m_pViewportDraw = rData.viewportDraw();
  m_pGripOpStatFunc = rData.gripOpStatFunc();
  m_pToolTipFunc = rData.toolTipFunc();
  m_bitFlags = rData.bitFlags();
  m_pAltBasePt = rData.alternateBasePoint();
}

inline ExDbGripData& ExDbGripData::operator = ( const ExDbGripData& rData )
{
  m_gripPt = rData.gripPoint();
  m_appData = rData.m_appData;
  m_pAppData = rData.m_pAppData;
  m_pHotGripFunc = rData.hotGripFunc();
  m_pHoverFunc = rData.hoverFunc();
  m_pWorldDraw = rData.worldDraw();
  m_pViewportDraw = rData.viewportDraw();
  m_pGripOpStatFunc = rData.gripOpStatFunc();
  m_pToolTipFunc = rData.toolTipFunc();
  m_bitFlags = rData.bitFlags();
  m_pAltBasePt = rData.alternateBasePoint();
  return *this;
}

inline void ExDbGripData::setGripPoint( const OdGePoint3d& pt )
{
  m_gripPt = pt;
}

inline void* ExDbGripData::appData() const
{
  return m_appData._p;
}

inline void ExDbGripData::setAppData( void* pAppData )
{
  m_appData._p = pAppData;
}

inline OdRxObjectPtr ExDbGripData::appDataPtr() const
{
  return m_pAppData;
}

inline void ExDbGripData::setAppDataPtr( const OdRxObject* pAppData )
{
  m_pAppData = pAppData;
}

inline int ExDbGripData::appDataInt() const
{
  return m_appData._n;
}

inline void ExDbGripData::setAppDataInt( int nAppData )
{
  m_appData._n = nAppData;
}

inline GripOperationPtr ExDbGripData::hotGripFunc() const
{
  return m_pHotGripFunc;
}

inline void ExDbGripData::setHotGripFunc( GripOperationPtr pFunc )
{
  m_pHotGripFunc = pFunc;
}

inline GripOperationPtr ExDbGripData::hoverFunc() const
{
  return m_pHoverFunc;
}

inline void ExDbGripData::setHoverFunc( GripOperationPtr pFunc )
{
  m_pHoverFunc = pFunc;
}

inline GripWorldDrawPtr ExDbGripData::worldDraw() const
{
  return m_pWorldDraw;
}

inline void ExDbGripData::setWorldDraw( GripWorldDrawPtr pFunc )
{
  m_pWorldDraw = pFunc;
}

inline GripViewportDrawPtr ExDbGripData::viewportDraw() const
{
  return m_pViewportDraw;
}

inline void ExDbGripData::setViewportDraw( GripViewportDrawPtr pFunc )
{
  m_pViewportDraw = pFunc;
}

inline GripOpStatusPtr ExDbGripData::gripOpStatFunc() const
{
  return m_pGripOpStatFunc;
}

inline void ExDbGripData::setGripOpStatFunc( GripOpStatusPtr pFunc )
{
  m_pGripOpStatFunc = pFunc;
}

inline GripToolTipPtr ExDbGripData::toolTipFunc() const
{
  return m_pToolTipFunc;
}

inline void ExDbGripData::setToolTipFunc( GripToolTipPtr pFunc )
{
  m_pToolTipFunc = pFunc;
}

inline OdGePoint3d* ExDbGripData::alternateBasePoint() const
{
  return m_pAltBasePt;
}

inline void ExDbGripData::setAlternateBasePoint( OdGePoint3d* altBasePt )
{
  m_pAltBasePt = altBasePt;
}

inline void ExDbGripData::setBitFlags( unsigned int flags )
{
  m_bitFlags = flags;
}

inline bool ExDbGripData::skipWhenShared() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kDisableRubberBandLine);
}

inline void ExDbGripData::setSkipWhenShared( bool skip )
{
  if (skip)
    m_bitFlags |= OdDbGripOperations::kSkipWhenShared;
  else
    m_bitFlags &= ~OdDbGripOperations::kSkipWhenShared;
}

inline bool ExDbGripData::isRubberBandLineDisabled() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kDisableModeKeywords);
}

inline void ExDbGripData::disableRubberBandLine( bool disable )
{
  if (disable)
    m_bitFlags |= OdDbGripOperations::kDisableRubberBandLine;
  else
    m_bitFlags &= ~OdDbGripOperations::kDisableRubberBandLine;
}

inline bool ExDbGripData::areModeKeywordsDisabled() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kDisableModeKeywords);
}

inline void ExDbGripData::disableModeKeywords( bool disable )
{
  if (disable)
    m_bitFlags |= OdDbGripOperations::kDisableModeKeywords;
  else
    m_bitFlags &= ~OdDbGripOperations::kDisableModeKeywords;
}

inline bool ExDbGripData::drawAtDragImageGripPoint() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kDrawAtDragImageGripPoint);
}

inline void ExDbGripData::setDrawAtDragImageGripPoint( bool atDragPoint )
{
  if (atDragPoint)
    m_bitFlags |= OdDbGripOperations::kDrawAtDragImageGripPoint;
  else
    m_bitFlags &= ~OdDbGripOperations::kDrawAtDragImageGripPoint;
}

inline bool ExDbGripData::triggerGrip() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kTriggerGrip);
}

inline void ExDbGripData::setTriggerGrip( bool trigger )
{
  if (trigger)
    m_bitFlags |= OdDbGripOperations::kTriggerGrip | OdDbGripOperations::kSkipWhenShared;
  else
    m_bitFlags &= ~(OdDbGripOperations::kTriggerGrip | OdDbGripOperations::kSkipWhenShared);
}

inline bool ExDbGripData::forcedPickOn() const
{
    return 0 != (m_bitFlags & OdDbGripOperations::kTurnOnForcedPick);
}

inline void ExDbGripData::setForcedPickOn( bool on )
{
  if (on)
    m_bitFlags |= OdDbGripOperations::kTurnOnForcedPick;
  else
    m_bitFlags &= ~OdDbGripOperations::kTurnOnForcedPick;
}

inline bool ExDbGripData::mapGripHotToRtClk() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kMapGripHotToRtClk);
}

inline void ExDbGripData::setMapGripHotToRtClk( bool on )
{
  if (on)
    m_bitFlags |= OdDbGripOperations::kMapGripHotToRtClk;
  else
    m_bitFlags &= ~OdDbGripOperations::kMapGripHotToRtClk;
}

inline bool ExDbGripData::gizmosEnabled() const
{
  return 0 != (m_bitFlags & OdDbGripOperations::kGizmosEnabled);
}

inline void ExDbGripData::setGizmosEnabled( bool on )
{
  if (on)
    m_bitFlags |= OdDbGripOperations::kGizmosEnabled;
  else
    m_bitFlags &= ~OdDbGripOperations::kGizmosEnabled;
}

#include "TD_PackPop.h"

#endif // _EXDBGRIPDATA_INCLUDED_
