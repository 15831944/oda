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

#ifndef _DGEXGRIPMANAGER_H_INCLUDED_
#define _DGEXGRIPMANAGER_H_INCLUDED_


#include "DgGsManager.h"
#include "DgDatabaseReactor.h"
#include "StaticRxObject.h"
#include "DbGrip.h"
#include "DgSubentId.h"
#include "DgGsManager.h"

class OdDgExGripManager;

class OdDgExGripData;
typedef OdSmartPtr<OdDgExGripData> OdDgExGripDataPtr;

class OdDgExGripData : public OdGiDrawableImpl<>
{
public:
  OdDgExGripData();
  virtual ~OdDgExGripData();
  static OdDgExGripDataPtr createObject(OdDgElementId id, OdDgGripDataPtr pData, const OdGePoint3d& pt,  OdDgExGripManager* pOwner);
  static OdDgExGripDataPtr createObject(OdDgFullSubentPath entPath, OdDgGripDataPtr pData, const OdGePoint3d& pt,  OdDgExGripManager* pOwner);

  virtual OdUInt32 subSetAttributes( OdGiDrawableTraits* pTraits ) const;
  virtual bool subWorldDraw( OdGiWorldDraw* pWorldDraw ) const;
  virtual void subViewportDraw( OdGiViewportDraw* pViewportDraw ) const;

  OdDbGripOperations::DrawType status() const { return m_status; }
  bool isInvisible() const { return m_bInvisible; }
  bool isShared() const { return m_bShared; }
  OdGePoint3d point() const { return m_point; }
  OdDgGripDataPtr data() const { return m_pData; }
  OdDgElementId entityId() const { return m_entPath.objectIds().last(); }
  bool entPath(OdDgFullSubentPath *pPath = NULL) const
  {
    if (pPath)
      *pPath = m_entPath;
    return m_entPath.subentId() != OdDbSubentId();
  }

  void setStatus(OdDbGripOperations::DrawType val) { m_status = val; }
  void setInvisible(bool val) { m_bInvisible = val; }
  void setShared(bool val) { m_bShared = val; }

private:
  bool computeDragPoint( OdGePoint3d& ptOverride ) const;

  OdDbGripOperations::DrawType m_status;
  bool m_bInvisible;
  bool m_bShared;
  OdGePoint3d m_point;
  OdDgGripDataPtr m_pData;
  OdDgFullSubentPath m_entPath;
  OdDgExGripManager* m_pOwner;
};

typedef OdArray<OdDgExGripDataPtr> OdDgExGripDataPtrArray;

class OdDgExGripDrag;
typedef OdSmartPtr<OdDgExGripDrag> OdDgExGripDragPtr;

class OdDgExGripDrag : public OdGiDrawableImpl<>
{
public:
  OdDgExGripDrag();
  virtual ~OdDgExGripDrag();
  static OdDgExGripDragPtr createObject(OdDgElementId entPath, OdDgExGripManager* pOwner);
  static OdDgExGripDragPtr createObject(OdDgFullSubentPath entPath, OdDgExGripManager* pOwner);

  virtual OdUInt32 subSetAttributes( OdGiDrawableTraits* pTraits ) const;
  virtual bool subWorldDraw( OdGiWorldDraw* pWorldDraw ) const;
  virtual void subViewportDraw( OdGiViewportDraw* pViewportDraw ) const;

  void cloneEntity();
  void cloneEntity( const OdGePoint3d& ptMoveAt );
  void moveEntity( const OdGePoint3d& ptMoveAt );
  void notifyDragStarted();
  void notifyDragEnded();
  void notifyDragAborted();

  OdDgElementId entityId() const { return m_entPath.objectIds().last(); }
  bool entPath(OdDgFullSubentPath *pPath = NULL) const
  {
    if (pPath)
      *pPath = m_entPath;
    return m_entPath.subentId() != OdDbSubentId();
  }

private:
  bool locateActiveGrips( OdIntArray& aIndices );

  OdDgFullSubentPath m_entPath;
  OdDgElementPtr m_pClone;
  OdDgExGripManager* m_pOwner;
};

typedef OdArray<OdDgExGripDragPtr> OdDgExGripDragPtrArray;


class OdExGripDgReactor : public OdDgDatabaseReactor
{
public:
  OdExGripDgReactor();
  virtual void objectAppended( const OdDgDatabase* pDb, const OdDgElement* pDbObj );
  virtual void objectModified( const OdDgDatabase* pDb, const OdDgElement* pDbObj );
  virtual void objectErased( const OdDgDatabase* pDb, const OdDgElement* pDbObj, bool pErased = true );

public:
  OdDgExGripManager* m_pOwner;
};

typedef OdSmartPtr<OdExGripDgReactor> OdExGripDgReactorPtr;

typedef OdDgSelectionSetPtr (*GetSelectionSetPtr)(OdDgCommandContext* pCmdCtx);

class OdDgExGripManager : public OdEdPointTracker
{
public:
  // Construction. Initialization.
  OdDgExGripManager();
  virtual ~OdDgExGripManager();
  void init( OdGsDevice* pDevice, OdGsModel* pGsModel, OdDgCommandContext* pCmdCtx, GetSelectionSetPtr getSSet );
  void uninit();

  // OdEdPointTracker protocol
  virtual void setValue( const OdGePoint3d& ptValue );
  virtual int addDrawables( OdGsView* pView );
  virtual void removeDrawables( OdGsView* pView );

  // Events from Windows.
  bool onMouseDown( int x, int y, bool bShift);

  bool onMouseMove( int x, int y );
  bool onControlClick();

  void selectionSetChanged(OdDgSelectionSetPtr pSSet);

  // Hover control.
  bool startHover( int x, int y );
  bool endHover();

  // Grip selection.
  void updateEntityGrips( const OdDgElementId& id );
  void removeEntityGrips( const OdDgElementId& id, bool bFireDone );
  void updateInvisibleGrips();

private:
  bool handleMappedRtClk( OdDgExGripDataPtrArray &aActiveKeys, int x, int y );

  void locateGripsAt( int x, int y, OdDgExGripDataPtrArray& aResult );
  void locateGripsByStatus( OdDbGripOperations::DrawType eStatus, OdDgExGripDataPtrArray& aResult );

  // Adds/Removes drawables to/from viewports.
  void showGrip( OdDgExGripData* pGrip, bool bModel );
  void hideGrip( OdDgExGripData* pGrip, bool bModel );

  //
  OdDgElementId activeVpId() const;
  double activeViewUnitSize() const;
  OdGeVector3d activeViewDirection() const;
  OdGePoint3d eyeToUcsPlane(const OdGePoint3d &pPoint, const OdGePoint3d &pBasePoint) const;

  OdGsDeviceForDgModelPtr m_pDevice;
  OdDgCommandContext* m_pCmdCtx;
  OdGsModel*          m_pGsModel;

  //
  bool m_bDisabled;

  //
public:
  struct OdDgExGripDataSubent
  {
    OdDgFullSubentPath m_entPath;
    OdDgExGripDataPtrArray m_pSubData;
  };
  struct OdDgExGripDataExt
  {
    OdDgExGripDataPtrArray m_pDataArray;
    OdArray<OdDgExGripDataSubent> m_pDataSub;
  };
private:
  //
  typedef std::map<OdDgElementId, OdDgExGripDataExt> DgGripDataMap;
  DgGripDataMap m_aGripData;

  //
  OdDgExGripDataPtrArray m_aHoverGrips;

  //
  OdGePoint3d m_ptBasePoint;
  OdGePoint3d m_ptLastPoint;
  OdDgExGripDragPtrArray m_aDrags;

  OdStaticRxObject<OdExGripDgReactor> m_cDbReactor;

  int               m_GRIPSIZE;
  int               m_GRIPOBJLIMIT;
  OdDgCmEntityColor m_GRIPCOLOR;
  OdDgCmEntityColor m_GRIPHOVER;
  OdDgCmEntityColor m_GRIPHOT;

  // Selection set.
  OdDgSelectionSetPtr workingSelectionSet() const;
  GetSelectionSetPtr m_pGetSelectionSetPtr;

  friend class OdDgExGripData;
  friend class OdDgExGripDrag;
};

#endif //_DGEXGRIPMANAGER_H_INCLUDED_
