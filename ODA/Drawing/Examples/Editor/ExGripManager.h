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

#ifndef _EXGRIPMANAGER_H_INCLUDED_
#define _EXGRIPMANAGER_H_INCLUDED_

#include "DbGsManager.h"
#include "DbDatabaseReactor.h"
#include "StaticRxObject.h"
#include "DbGrip.h"
#include "DbEntity.h"
#include "DbUserIO.h"
#define STL_USING_MAP
#include "OdaSTL.h"
#include "Ed/EdCommandStack.h"
#include "Gi/GiDrawableImpl.h"

class OdBaseGripManager;
class OdExGripData;
typedef OdSmartPtr<OdExGripData> OdExGripDataPtr;

class OdExGripData : public OdGiDrawableImpl<>
{
public:
  OdExGripData();
  virtual ~OdExGripData();
  static OdExGripDataPtr createObject(OdDbStub* id, OdDbGripDataPtr pData, const OdGePoint3d& pt, OdBaseGripManager* pOwner);
  static OdExGripDataPtr createObject(OdDbBaseFullSubentPath entPath, OdDbGripDataPtr pData, const OdGePoint3d& pt, OdBaseGripManager* pOwner);

  virtual OdUInt32 subSetAttributes( OdGiDrawableTraits* pTraits ) const;
  virtual bool subWorldDraw( OdGiWorldDraw* pWorldDraw ) const;
  virtual void subViewportDraw( OdGiViewportDraw* pViewportDraw ) const;

  OdDbGripOperations::DrawType status() const { return m_status; }
  bool isInvisible() const { return m_bInvisible; }
  bool isShared() const { return m_bShared; }
  OdGePoint3d point() const { return m_point; }
  OdDbGripDataPtr data() const { return m_pData; }
  OdDbStub* entityId() const { return m_entPath.objectIds().last(); }
  bool entPath(OdDbBaseFullSubentPath *pPath = NULL) const
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
  OdDbGripDataPtr m_pData;
  OdDbBaseFullSubentPath m_entPath;
  OdBaseGripManager* m_pOwner;
};

typedef OdArray<OdExGripDataPtr> OdExGripDataPtrArray;

typedef OdSmartPtr<class OdExGripDrag> OdExGripDragPtr;

class OdExGripDrag : public OdGiDrawableImpl<>
{
public:
  OdExGripDrag();
  virtual ~OdExGripDrag();
  static OdExGripDragPtr createObject(OdDbStub* id, OdBaseGripManager* pOwner);
  static OdExGripDragPtr createObject(OdDbBaseFullSubentPath entPath, OdBaseGripManager* pOwner);

  virtual OdUInt32 subSetAttributes( OdGiDrawableTraits* pTraits ) const;
  virtual bool subWorldDraw( OdGiWorldDraw* pWorldDraw ) const;
  virtual void subViewportDraw( OdGiViewportDraw* pViewportDraw ) const;

  void cloneEntity();
  void cloneEntity( const OdGePoint3d& ptMoveAt );
  void moveEntity( const OdGePoint3d& ptMoveAt );
  void notifyDragStarted();
  void notifyDragEnded();
  void notifyDragAborted();

  OdDbStub* entityId() const;
  bool entPath(OdDbBaseFullSubentPath *pPath = NULL) const;

protected:
  bool locateActiveGrips( OdIntArray& aIndices );

  OdDbBaseFullSubentPath m_entPath;
  OdGiDrawablePtr m_pClone;
  OdBaseGripManager* m_pOwner;
};

typedef OdArray<OdExGripDragPtr> OdExGripDragPtrArray;

class OdBaseGripManager : public OdEdPointTracker
{
public:
  // Construction. Initialization.
  OdBaseGripManager();
  virtual ~OdBaseGripManager();

  // OdEdPointTracker protocol
  virtual void setValue(const OdGePoint3d& ptValue);

  // Events from Windows.
  virtual void onModified(OdGiDrawable* pGrip) = 0;
  virtual bool onMouseDown(int x, int y, bool bShift);

  virtual void selectionSetChanged(OdSelectionSet* pSSet);

  virtual OdGiDrawablePtr cloneEntity(OdDbStub* id) = 0;
  virtual OdGiDrawablePtr openObject(OdDbStub* id, bool isForWriteMode = false) = 0; 
  // alternative to OdDbGripPointsPE methods
  // possible TODO instead next methods
  // (redesign OdDbGripPointsPE to use OdGiDrawable & use some base class of it instead OdDgGripPointsPE) 
  virtual OdResult getGripPointsAtSubentPath(OdGiDrawable* pEntity,
                                             const OdDbBaseFullSubentPath& path, OdDbGripDataPtrArray& grips,
                                             double curViewUnitSize, int gripSize,
                                             const OdGeVector3d& curViewDir, const OdUInt32 bitflags) const = 0;
  virtual OdResult getGripPoints(OdGiDrawable* pEntity,
                                 OdDbGripDataPtrArray& grips,
                                 double curViewUnitSize, int gripSize, 
                                 const OdGeVector3d& curViewDir, int bitFlags) const = 0;
  virtual OdResult getGripPoints(OdGiDrawable* pEntity, OdGePoint3dArray& gripPoints) const = 0;
  virtual OdResult moveGripPointsAtSubentPaths(OdGiDrawable* pEntity,
                                               const OdDbBaseFullSubentPathArray& paths, const OdDbVoidPtrArray& gripAppData,
                                               const OdGeVector3d& offset, OdUInt32 bitflags) = 0;
  virtual OdResult moveGripPointsAt(OdGiDrawable* pEntity, const OdDbVoidPtrArray& gripAppData, const OdGeVector3d& offset, int bitFlags) = 0;
  virtual OdResult moveGripPointsAt(OdGiDrawable* pEntity, const OdIntArray& indices, const OdGeVector3d& offset) = 0;
  virtual void subentGripStatus(OdGiDrawable* pEntity, OdDb::GripStat status, const OdDbBaseFullSubentPath& subentity) = 0;
  virtual void gripStatus(OdGiDrawable* pEntity, OdDb::GripStat st) = 0;
  virtual void dragStatus(OdGiDrawable* pEntity, OdDb::DragStat st) = 0;
  virtual bool isModel(OdGiDrawable* ) { return true; }

  // Hover control.
  OdResult startHover(int x, int y, bool bShift);
  bool endHover();

  // Grip selection.
  void updateEntityGrips(OdDbStub* id );
  void removeEntityGrips(OdDbStub* id, bool bFireDone);
  void updateInvisibleGrips();

  // Dragging.
  virtual void draggingStarted() = 0;
  virtual void draggingStopped() = 0;

  //bool handleMappedRtClk( OdExGripDataPtrArray &aActiveKeys, int x, int y );

  void locateGripsAt( int x, int y, OdExGripDataPtrArray& aResult );
  void locateGripsByStatus( OdDbGripOperations::DrawType eStatus, OdExGripDataPtrArray& aResult );
protected:
  // Adds/Removes drawables to/from viewports.
  virtual void showGrip(OdExGripData* pGrip, bool bModel) = 0;
  virtual void hideGrip(OdExGripData* pGrip, bool bModel) = 0;

  //
  virtual OdGsView* activeGsView() const = 0;
  virtual OdDbStub* activeVpId() const;
  double activeViewUnitSize() const;
  OdGeVector3d activeViewDirection() const;
  virtual OdGePoint3d eyeToUcsPlane(const OdGePoint3d &pPoint, const OdGePoint3d &pBasePoint) const = 0;

  virtual OdGsModel* getGsModel() { return NULL; }
  virtual OdGsLayoutHelper* getGsLayoutHelper() { return NULL; }

  bool m_bDisabled;
  virtual void disable(bool bDisable);

public:
  bool isDisabled()  { return m_bDisabled; }

  struct OdExGripDataSubent
  {
    OdDbBaseFullSubentPath m_entPath;
    OdExGripDataPtrArray m_pSubData;
  };
  struct OdExGripDataExt
  {
    OdExGripDataPtrArray m_pDataArray;
    OdArray<OdExGripDataSubent> m_pDataSub;
  };
  //
  typedef std::map<OdDbStub*, OdExGripDataExt> GripDataMap;
  GripDataMap m_aGripData;
protected:

  OdExGripDataPtrArray m_aHoverGrips;
  clock_t clockStartHover;

  OdGePoint3d m_ptBasePoint;
  OdGePoint3d m_ptLastPoint;
  OdExGripDragPtrArray m_aDrags;

  int             m_GRIPSIZE;
  int             m_GRIPOBJLIMIT;
  OdCmEntityColor m_GRIPCOLOR;
  OdCmEntityColor m_GRIPHOVER;
  OdCmEntityColor m_GRIPHOT;

  // Selection set.
  virtual OdSelectionSetPtr workingSelectionSet() const = 0;
  friend class OdExGripData;
  friend class OdExGripDrag;
};

///////////////////////////////////////////////////////////////////////////////

class OdExGripDbReactor : public OdDbDatabaseReactor
{
public:
  OdExGripDbReactor();
  virtual void objectAppended( const OdDbDatabase* pDb, const OdDbObject* pDbObj );
  virtual void objectModified( const OdDbDatabase* pDb, const OdDbObject* pDbObj );
  virtual void objectErased( const OdDbDatabase* pDb, const OdDbObject* pDbObj, bool pErased = true );

public:
  class OdExGripManager* m_pOwner;
};

typedef OdSmartPtr<OdExGripDbReactor> OdExGripDbReactorPtr;

class OdDbCommandContext;
typedef OdDbSelectionSetPtr (*GetSelectionSetPtr)(OdDbCommandContext* pCmdCtx);

typedef OdDbSelectionSetPtr (*GetSelectionSetPtr)(OdDbCommandContext* pCmdCtx);

class OdExGripManager : public OdBaseGripManager
{
public:
  // Construction. Initialization.
  OdExGripManager();
  virtual ~OdExGripManager();
  void init( OdGsDevice* pDevice, OdGsModel* pGsModel, OdDbCommandContext* pCmdCtx, GetSelectionSetPtr getSSet );
  void uninit();

  // OdEdPointTracker protocol
  virtual int addDrawables( OdGsView* pView );
  virtual void removeDrawables( OdGsView* pView );

  virtual void onModified(OdGiDrawable* pGrip);

  virtual OdGiDrawablePtr cloneEntity(OdDbStub* id);
  virtual OdGiDrawablePtr openObject(OdDbStub* id, bool isForWriteMode = false); 
  virtual OdResult getGripPointsAtSubentPath(OdGiDrawable* pEntity, const OdDbBaseFullSubentPath& path, OdDbGripDataPtrArray& grips,
                                             double curViewUnitSize, int gripSize, const OdGeVector3d& curViewDir, const OdUInt32 bitflags) const;
  virtual OdResult getGripPoints(OdGiDrawable* pEntity, OdDbGripDataPtrArray& grips, double curViewUnitSize, int gripSize, 
                                 const OdGeVector3d& curViewDir, int bitFlags) const;
  virtual OdResult getGripPoints(OdGiDrawable* pEntity, OdGePoint3dArray& gripPoints) const;
  virtual OdResult moveGripPointsAtSubentPaths(OdGiDrawable* pEntity, const OdDbBaseFullSubentPathArray& paths, 
                                               const OdDbVoidPtrArray& gripAppData, const OdGeVector3d& offset, OdUInt32 bitflags);
  virtual OdResult moveGripPointsAt(OdGiDrawable* pEntity, const OdDbVoidPtrArray& gripAppData, const OdGeVector3d& offset, int bitFlags);
  virtual OdResult moveGripPointsAt(OdGiDrawable* pEntity, const OdIntArray& indices, const OdGeVector3d& offset);
  virtual void subentGripStatus(OdGiDrawable* pEntity, OdDb::GripStat status, const OdDbBaseFullSubentPath& subentity);
  virtual void gripStatus(OdGiDrawable* pEntity, OdDb::GripStat st);
  virtual void dragStatus(OdGiDrawable* pEntity, OdDb::DragStat st);
  virtual bool isModel(OdGiDrawable* pEntity);

  // Events from Windows.
  virtual bool onMouseDown(int x, int y, bool bShift);
  bool onMouseMove( int x, int y, bool bShift);
  bool onControlClick();

  // Dragging.
  virtual void draggingStarted();
  virtual void draggingStopped();
private:
  bool handleMappedRtClk( OdExGripDataPtrArray &aActiveKeys, int x, int y );
  void addToDrag(OdExGripDataPtrArray& aActiveKeys);

 // Adds/Removes drawables to/from viewports.
  virtual void showGrip( OdExGripData* pGrip, bool bModel );
  virtual void hideGrip( OdExGripData* pGrip, bool bModel );

  virtual OdGsView* activeGsView() const;

  virtual OdGePoint3d eyeToUcsPlane(const OdGePoint3d &pPoint, const OdGePoint3d &pBasePoint) const;

  OdGsLayoutHelperPtr m_pDevice;
  OdDbCommandContext* m_pCmdCtx;
  OdGsModel*          m_pGsModel;
  virtual OdGsModel* getGsModel() { return m_pGsModel; }
  virtual OdGsLayoutHelper* getGsLayoutHelper() { return m_pDevice.get(); }
  virtual void disable(bool bDisable);
private:
  OdStaticRxObject<OdExGripDbReactor> m_cDbReactor;

  // Selection set.
  virtual OdSelectionSetPtr workingSelectionSet() const;
  GetSelectionSetPtr m_pGetSelectionSetPtr;
  struct OdExGripCommand : OdEdCommand
  {
    OdExGripManager* m_parent;
    const OdString groupName() const{ return L"EDIT"; }
    const OdString globalName() const{ return L"GROUP_STRETCH"; }
    void execute(OdEdCommandContext* pCmdCtx);
  };
  OdStaticRxObject<OdExGripCommand> m_gripStretchCommand;
  friend struct OdExGripCommand;
};

#endif //_EXGRIPMANAGER_H_INCLUDED_
