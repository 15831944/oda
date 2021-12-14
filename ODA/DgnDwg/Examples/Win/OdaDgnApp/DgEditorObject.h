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

#ifndef _DgEditorObject_h_Included_
#define _DgEditorObject_h_Included_

#include "DgSSet.h"
#include "DgGsManager.h"
#include "StaticRxObject.h"
#include "DgCommandContext.h"
#include "DgOSnapManager.h"
#include "DgExGripManager.h"
#include "OdaDgnAppDoc.h"

class OdDgExEditorObject
{

  OdGsDeviceForDgModelPtr m_pDevice;
  bool                    m_bSnapOn;
  OdStaticRxObject<DgOSnapManager>
                          m_osnapMan;
  OdStaticRxObject<OdDgExGripManager> 
                          m_gripManager;
  OdGsModelPtr            m_p2dModel;
  OdDgCommandContext*     m_pCmdCtx;

  OdStaticRxObject<Cmd_ZOOM>
                          m_cmd_ZOOM;
  OdStaticRxObject<OdDgEx3dOrbitCmd>
                          m_cmd_ORBIT;
  OdStaticRxObject<OdDgExDollyCmd>
                          m_cmd_DOLLY;

  void transformSSetBy(const OdGeMatrix3d& xfm);

  const OdGsView* activeTopView() const;
  OdGsView* activeTopView();
  OdDgElementId activeVpId() const;
  void dolly(int x, int y);

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
  OdDgExEditorObject();

  void initialize( OdGsDevice* pDevice, OdDgCommandContext* pCmdCtx );
  void uninitialize();

  OdDgSelectionSetPtr workingSSet() const;
  void setWorkingSSet(OdDgSelectionSet* pSSet);
  void selectionSetChanged();

  OdGiDrawablePtr snapDrawable() const;
  bool unselect();

  OdEdCommandPtr command(const OdString& sCmdName);

  OdGePoint3d toEyeToWorld(int x, int y) const;
  OdGePoint3d toScreenCoord(int x, int y) const;
  OdGePoint3d toScreenCoord(const OdGePoint3d& wcsPt) const;

  const OdGsView* activeView() const;
  OdGsView* activeView();

  class DgOleDragCallback
  {
  public:
    virtual bool beginDragCallback(const OdGePoint3d& pt) = 0;
  };

  bool   OnMouseLeftButtonClick       (unsigned int nFlags, int x, int y, DgOleDragCallback* pDragCallback = 0 );
  bool   OnMouseMove                  (unsigned int nFlags, int x, int y);
  bool   OnMouseWheel                 (unsigned int nFlags, int x, int y, short zDelta);
  bool   OnMouseLeftButtonDoubleClick (unsigned int nFlags, int x, int y);
  bool   OnMouseRightButtonDoubleClick(unsigned int nFlags, int x, int y);
  bool   OnCtrlClick();

  void ShowStatusBarSelectedElementId(OdDgSelectionSetPtr pSSet);

  bool hasSelection() const { return (workingSSet()->numEntities()>0); }
  bool isSnapOn() const { return m_bSnapOn; }
  bool snap(OdGePoint3d& pt, const OdGePoint3d* pLastPt);
  unsigned getSnapModes() const;
  void setSnapModes( bool bSnapOn, unsigned modes );
  void resetSnapManager();
  void initSnapping( OdGsView* pView );
  void uninitSnapping( OdGsView* pView );

  inline OdGsModel *gsModel() { return m_p2dModel.get(); }
};


inline OdGiDrawablePtr OdDgExEditorObject::snapDrawable() const
{
  return &m_osnapMan;
}

inline void OdDgExEditorObject::resetSnapManager()
{
  m_osnapMan.reset();
}



#endif //_DgEditorObject_h_Included_
