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
// OdqDragTracker.h
//

#ifndef ODA_QT_DRAG_TRACKER_H_
#define ODA_QT_DRAG_TRACKER_H_

#include "OdqGripManager.h"
//#include "OSnapManager.h"

//////////////////////////////////////////////////////////////////////////

typedef OdSmartPtr<class OdqDragTracker> OdqDragTrackerPtr;

class OdqDragTracker : public OdEdPointTracker // possible TODO for some snapping // public  OdEdPointTrackerWithSnapInfo
{
  ODRX_DECLARE_MEMBERS(OdqDragTracker);

protected:
  friend class OdExGripDrag;
  mutable OdqGripManagerPtr m_pGripManager;
  OdGsView* m_pDrawablesWereAddedToGsView;

  bool m_bCanceled;

  // Construction
  OdqDragTracker();
  virtual ~OdqDragTracker();
public:
  static OdqDragTrackerPtr createObject(OdqGripManagerPtr pGripManager,
                                        const OdGePoint3d& ptBase,
                                        bool bUseClones = true);

  // Initialization.
  bool init(OdqGripManagerPtr pGripManager,
            const OdGePoint3d& ptBase,
            bool bUseClones = true);
  void uninit();

  bool finishAt(const OdGePoint3d& ptFinal);

  // ----- OdEdPointTracker protocol -----
  virtual void setValue(const OdGePoint3d& ptValue);
  virtual int addDrawables(OdGsView* pView);
  virtual void removeDrawables(OdGsView* pView);
};

#endif // ODA_QT_DRAG_TRACKER_H_
