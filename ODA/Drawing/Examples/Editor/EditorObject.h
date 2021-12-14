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

#ifndef _EditorObject_h_Included_
#define _EditorObject_h_Included_

#include "DbSSet.h"
#include "Ed/EdCommandStack.h"
#include "ExDbCommandContext.h"
#include "DbGsManager.h"
#include "StaticRxObject.h"
#include "OSnapManager.h"
#include "ExGripManager.h"
#include "ExEdInputParser.h"


class OdExZoomCmd : public OdEdCommand
{
public:
  const OdString groupName() const;
  const OdString globalName() const;
  void execute(OdEdCommandContext* pCmdCtx);
};

class OdEx3dOrbitCmd : public OdEdCommand
{
public:
  const OdString groupName() const;
  const OdString globalName() const;
  void execute(OdEdCommandContext* pCmdCtx);
};

class OdExDollyCmd : public OdEdCommand
{
public:
  const OdString groupName() const;
  const OdString globalName() const;
  void execute(OdEdCommandContext* pCmdCtx);
};

class OdExInteractivityModeCmd : public OdEdCommand
{
public:
  const OdString groupName() const;
  const OdString globalName() const;
  void execute(OdEdCommandContext* pCmdCtx);
};

class OdExCollideCmd : public OdEdCommand
{
public:
  const OdString groupName() const;
  const OdString globalName() const;
  void execute(OdEdCommandContext* pCmdCtx);
};

class OdExCollideAllCmd : public OdEdCommand
{
public:
  const OdString groupName() const;
  const OdString globalName() const;
  void execute(OdEdCommandContext* pCmdCtx);
};

class OdExEditorObject
{

  OdGsLayoutHelperPtr               m_pDevice;
  OdStaticRxObject<OSnapManager>    m_osnapMan;
  OdStaticRxObject<OdExGripManager> m_gripManager;
  OdGsModelPtr                      m_p2dModel;
  OdDbCommandContext*               m_pCmdCtx;

  OdStaticRxObject<OdExZoomCmd>     m_cmd_ZOOM;
  OdStaticRxObject<OdEx3dOrbitCmd>  m_cmd_3DORBIT;
  OdStaticRxObject<OdExDollyCmd>    m_cmd_DOLLY;
  OdStaticRxObject<OdExInteractivityModeCmd>    m_cmd_INTERACTIVITY;
  OdStaticRxObject<OdExCollideCmd>  m_cmd_COLLIDE;
  OdStaticRxObject<OdExCollideAllCmd>  m_cmd_COLLIDE_ALL;

  OdEdInputTrackerPtr               m_pTracker;
  OdGePoint3d                       m_basePt;
  const OdGePoint3d*                m_pBasePt;

  enum Flags {
    kSnapOn               =  4,
    kOrbitOn              =  8,
    kDragging             = 16,
    kTrackerHasDrawables  = 32,
  };
  OdUInt32                          m_flags;
public:
  void transformSSetBy(const OdGeMatrix3d& xfm);

  const OdGsView* activeView() const;
  OdGsView* activeView();
  const OdGsView* activeTopView() const;
  OdGsView* activeTopView();
  OdDbObjectId activeVpId() const;
  void ucsPlane(OdGePlane& plane) const;
  void dolly(int x, int y);

  static void zoomAt(OdGsView* pView, int x, int y, short zDelta);
  static void dolly(OdGsView* pView, int x, int y);

public:
  enum _3DViewType
  {
    k3DViewTop,
    k3DViewBottom,
    k3DViewLeft,
    k3DViewRight,
    k3DViewFront,
    k3DViewBack,
    k3DViewSW,
    k3DViewSE,
    k3DViewNE,
    k3DViewNW
  };
  void set3DView(_3DViewType type);
public:
  OdExEditorObject();

  void initialize( OdGsDevice* pDevice, OdDbCommandContext* pCmdCtx );
  OdGsLayoutHelper* device() { return m_pDevice; }
  void uninitialize();

  OdDbSelectionSetPtr workingSSet() const;
  void setWorkingSSet(OdDbSelectionSet* pSSet);
  void selectionSetChanged();

  OdGiDrawablePtr snapDrawable() const;
  bool unselect();

  OdEdCommandPtr command(const OdString& sCmdName);

  OdGePoint3d toEyeToWorld(int x, int y) const;
  bool toUcsToWorld(OdGePoint3d& wcsPt) const;
  OdGePoint3d toScreenCoord(int x, int y) const;
  OdGePoint3d toScreenCoord(const OdGePoint3d& wcsPt) const;

  class OleDragCallback
  {
  public:
    virtual bool beginDragCallback(const OdGePoint3d& pt) = 0;
  };
  bool   OnSize                       (unsigned int nFlags, int w, int h);
  bool   OnPaintFrame                 (unsigned int nFlags = 0, OdGsDCRect* pUpdatedRect = 0);
  bool   OnMouseLeftButtonClick       (unsigned int nFlags, int x, int y, OleDragCallback* pDragCallback = 0);
  bool   OnMouseMove                  (unsigned int nFlags, int x, int y);
  bool   OnMouseWheel                 (unsigned int nFlags, int x, int y, short zDelta);
  bool   OnMouseLeftButtonDoubleClick (unsigned int nFlags, int x, int y);
  bool   OnMouseRightButtonDoubleClick(unsigned int nFlags, int x, int y);
  bool   OnCtrlClick();
  void   OnDestroy();

  bool hasSelection() const { return (workingSSet()->numEntities()>0); }
  bool isSnapOn() const { return GETBIT(m_flags, kSnapOn); }
  void setSnapOn(bool bOn) { SETBIT(m_flags, kSnapOn, bOn); }
  bool isOrbitOn() const { return GETBIT(m_flags, kOrbitOn); }

  void turnOrbitOn(bool bOn);
  bool OnOrbitBeginDrag(int x, int y);
  bool OnOrbitEndDrag(int x, int y);

  bool OnZoomWindowBeginDrag(int x, int y);
  bool OnZoomWindowEndDrag(int x, int y);

  bool snap(OdGePoint3d& pt, const OdGePoint3d* pLastPt = 0);
  unsigned getSnapModes() const;
  void setSnapModes( bool bSnapOn, unsigned modes );
  void resetSnapManager();
  void initSnapping( OdGsView* pView, OdEdInputTracker* pTracker );
  void uninitSnapping( OdGsView* pView );

  inline OdGsModel *gsModel() { return m_p2dModel.get(); }

  void Recalc_Entity_centers(void)
  {
    m_osnapMan.Recalc_Entity_centers();
  }

  void Set_Entity_centers()
  {
    if(hasDatabase())
      m_osnapMan.Set_Entity_centers(m_pCmdCtx->database());
  }

  void setTracker(OdEdInputTracker* pTracker);
  bool updateStringTrackerCursor();
  bool trackString(const OdString& value);
  bool trackPoint(const OdGePoint3d& pt);
  bool hasDatabase() const;
};


inline OdGiDrawablePtr OdExEditorObject::snapDrawable() const
{
  return &m_osnapMan;
}

inline void OdExEditorObject::resetSnapManager()
{
  m_osnapMan.reset();
}

#endif //_EditorObject_h_Included_
