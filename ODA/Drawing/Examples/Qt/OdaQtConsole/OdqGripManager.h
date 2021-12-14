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
//
// OdqGripManager.h
//

#ifndef ODA_QT_GRIP_MANAGER_H_
#define ODA_QT_GRIP_MANAGER_H_

#include "ExGripManager.h" //#include "ExGripData.h"
#include "DbGsManager.h"
#include "DbUserIO.h"

#define STL_USING_MAP
#include "OdaSTL.h"

/////////////////////////////////////////////////////////////////////////////// 

typedef OdSmartPtr<class OdqGripManager> OdqGripManagerPtr;

class OdqGripManager : public OdBaseGripManager
{
  ODRX_DECLARE_MEMBERS(OdqGripManager);

protected:
  class OdqConsoleTab* m_pTabConsole; friend class OdqDragTracker;
  bool m_isDragModel2d;

  // Construction
  OdqGripManager();
  virtual ~OdqGripManager();
public:
  static OdqGripManagerPtr createObject(OdqConsoleTab& tabConsole);

  // Initialization.
  void init(OdqConsoleTab& tabConsole);
  void uninit();

  // ----- OdEdPointTracker protocol -----
  //virtual void setValue(const OdGePoint3d& ptValue);
  virtual int addDrawables(OdGsView* pView) ODRX_OVERRIDE;
  virtual void removeDrawables(OdGsView* pView) ODRX_OVERRIDE;

  virtual void onModified(OdGiDrawable* pGrip) ODRX_OVERRIDE;
  void onModified(OdGsView* pView);

  void updateAllGrips();

  bool checkGripSetAt(int x, int y, bool& bMultiplyMode);

  virtual OdGiDrawablePtr cloneEntity(OdDbStub* id) ODRX_OVERRIDE;
  virtual OdGiDrawablePtr openObject(OdDbStub* id, bool isForWriteMode = false) ODRX_OVERRIDE; 
  //
  virtual OdResult getGripPointsAtSubentPath(OdGiDrawable* pEntity, const OdDbBaseFullSubentPath& path, OdDbGripDataPtrArray& grips, double curViewUnitSize, int gripSize, const OdGeVector3d& curViewDir, const OdUInt32 bitflags) const ODRX_OVERRIDE;
  virtual OdResult getGripPoints(OdGiDrawable* pEntity, OdDbGripDataPtrArray& grips, double curViewUnitSize, int gripSize, const OdGeVector3d& curViewDir, int bitFlags) const ODRX_OVERRIDE;
  virtual OdResult getGripPoints(OdGiDrawable* pEntity, OdGePoint3dArray& gripPoints) const ODRX_OVERRIDE;
  virtual OdResult moveGripPointsAtSubentPaths(OdGiDrawable* pEntity, const OdDbBaseFullSubentPathArray& paths, const OdDbVoidPtrArray& gripAppData, const OdGeVector3d& offset, OdUInt32 bitflags) ODRX_OVERRIDE;
  virtual OdResult moveGripPointsAt(OdGiDrawable* pEntity, const OdDbVoidPtrArray& gripAppData, const OdGeVector3d& offset, int bitFlags) ODRX_OVERRIDE;
  virtual OdResult moveGripPointsAt(OdGiDrawable* pEntity, const OdIntArray& indices, const OdGeVector3d& offset) ODRX_OVERRIDE;
  virtual void subentGripStatus(OdGiDrawable* pEntity, OdDb::GripStat status, const OdDbBaseFullSubentPath& subentity) ODRX_OVERRIDE;
  virtual void gripStatus(OdGiDrawable* pEntity, OdDb::GripStat st) ODRX_OVERRIDE;
  virtual void dragStatus(OdGiDrawable* pEntity, OdDb::DragStat st) ODRX_OVERRIDE;
  virtual bool isModel(OdGiDrawable* pEntity) ODRX_OVERRIDE;

  virtual void selectionSetChanged(OdSelectionSet* pSSet) ODRX_OVERRIDE;

  OdUInt32 numGripPoints();
  OdUInt32 numGripPoints(OdDbGripOperations::DrawType eStatus);

  virtual void showGrip(OdExGripData* pGrip, bool bModel) ODRX_OVERRIDE;
  virtual void hideGrip(OdExGripData* pGrip, bool bModel) ODRX_OVERRIDE;

  // Dragging.
  virtual void draggingStarted() ODRX_OVERRIDE;
  virtual void draggingStopped() ODRX_OVERRIDE;

protected:
  virtual OdSelectionSetPtr workingSelectionSet() const ODRX_OVERRIDE;
  virtual OdGsView* activeGsView() const ODRX_OVERRIDE;
  virtual OdGePoint3d eyeToUcsPlane(const OdGePoint3d &pPoint, const OdGePoint3d &pBasePoint) const ODRX_OVERRIDE;
};

#endif // ODA_QT_GRIP_MANAGER_H_
