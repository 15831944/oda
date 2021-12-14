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
#include "Gi/GiDrawableImpl.h"
#include "Gi/GiWorldDraw.h"
#include "Gs/Gs.h"
#include "Gs/GsBaseVectorizer.h"
#include "DgEditorObject.h"
#include "DgGiContext.h"
#include "DgModel.h"
#include "OdDToStr.h"
#include "SaveState.h"
#include "AbstractViewPE.h"
#include "Ge/GeLine3d.h"
#include "exedbaseio.h"
#include "MainFrm.h"
#include "OdaDgnApp.h"

static const int SNAP_SIZE = 10;


void setWorkingSelectionSet(OdDgCommandContext* pCmdCtx, OdDgSelectionSet* pSSet)
{
  pCmdCtx->setArbitraryData(OD_T("OdaDgnApp Working Selection Set"), pSSet);
}

OdDgSelectionSetPtr workingSelectionSet(OdDgCommandContext* pCmdCtx)
{
  OdDgSelectionSetPtr pRes;
  if (pCmdCtx)
  {
    pRes = pCmdCtx->arbitraryData(OD_T("OdaDgnApp Working Selection Set"));
    if(pRes.isNull())
    {
      pRes = OdDgSelectionSet::createObject(pCmdCtx->database());
      setWorkingSelectionSet(pCmdCtx, pRes);
    }
  }
  return pRes;
}


class XFormDrawable : public OdGiDrawableImpl<OdGiDrawable>
{
  OdGiDrawablePtr     m_pDrawable;
  const OdGeMatrix3d* m_pXForm;
protected:
  XFormDrawable() : m_pXForm(0) {}
public:
  static OdGiDrawablePtr createObject(OdGiDrawable* pDrawable, const OdGeMatrix3d& xForm)
  {
    OdSmartPtr<XFormDrawable> pRes = OdRxObjectImpl<XFormDrawable>::createObject();
    pRes->m_pDrawable = pDrawable;
    pRes->m_pXForm = &xForm;
    return pRes;
  }

  OdUInt32 subSetAttributes(OdGiDrawableTraits* ) const
  {
    return kDrawableUsesNesting;
  }

  bool subWorldDraw(OdGiWorldDraw* pWd) const
  {
    OdGiModelTransformSaver mt(pWd->geometry(), *m_pXForm);
    pWd->geometry().draw(m_pDrawable);
    return true;
  }
  
  void subViewportDraw(OdGiViewportDraw* ) const {}
};


OdDgExEditorObject::OdDgExEditorObject()
  : m_bSnapOn(true)
  , m_pCmdCtx(0)
{
}

void OdDgExEditorObject::initialize( OdGsDevice* pDevice, OdDgCommandContext* pCmdCtx )
{
  m_pDevice = pDevice;
  m_pCmdCtx = pCmdCtx;

  m_p2dModel = pDevice->createModel();

  if (!m_p2dModel.isNull())
    m_p2dModel->setRenderType(OdGsModel::kDirect); // Skip Z-buffer for 2d drawables

  m_gripManager.init( pDevice, m_p2dModel, pCmdCtx, workingSelectionSet );
}

void OdDgExEditorObject::uninitialize()
{
  OdDgSelectionSetPtr pSSet = workingSSet();
  if(pSSet.get())
  {
    pSSet->clear();
    m_gripManager.selectionSetChanged(pSSet);
  }
  m_gripManager.uninit();

  m_pDevice.release();
  m_pCmdCtx = 0;
}

void OdDgExEditorObject::initSnapping( OdGsView* pView )
{
  pView->add(&m_osnapMan, m_p2dModel);
}

void OdDgExEditorObject::uninitSnapping( OdGsView* pView )
{
  pView->erase(&m_osnapMan);
}

OdDgSelectionSetPtr OdDgExEditorObject::workingSSet() const
{
  return workingSelectionSet(m_pCmdCtx);
}

void OdDgExEditorObject::setWorkingSSet(OdDgSelectionSet* pSSet)
{
  setWorkingSelectionSet(m_pCmdCtx, pSSet);
}

void OdDgExEditorObject::selectionSetChanged()
{
  m_gripManager.selectionSetChanged(workingSSet());
}

const OdGsView* OdDgExEditorObject::activeView() const
{
  return m_pDevice->activeView();
}

OdGsView* OdDgExEditorObject::activeView()
{
  return m_pDevice->activeView();
}

const OdGsView* OdDgExEditorObject::activeTopView() const
{
  const OdGsView *pView = activeView();
//   if (!m_pCmdCtx->database()->getTILEMODE())
//   {
//     OdDbObjectPtr pVpObj = m_pCmdCtx->database()->activeViewportId().safeOpenObject();
//     OdDbAbstractViewportDataPtr pAVD(pVpObj);
//     if (!pAVD.isNull() && pAVD->gsView(pVpObj))
//       pView = pAVD->gsView(pVpObj);
//   }
  return pView;
}

OdGsView* OdDgExEditorObject::activeTopView()
{
  OdGsView *pView = activeView();
//   if (!m_pCmdCtx->database()->getTILEMODE())
//   {
//     OdDbObjectPtr pVpObj = m_pCmdCtx->database()->activeViewportId().safeOpenObject();
//     OdDbAbstractViewportDataPtr pAVD(pVpObj);
//     if (!pAVD.isNull() && pAVD->gsView(pVpObj))
//       pView = pAVD->gsView(pVpObj);
//   }
  return pView;
}

OdDgElementId OdDgExEditorObject::activeVpId() const
{
  OdGsClientViewInfo vi;
  ((OdGsView*)activeView())->clientViewInfo(vi);
  return OdDgElementId(vi.viewportObjectId);
}

OdGePoint3d OdDgExEditorObject::toEyeToWorld(int x, int y) const
{
  OdGePoint3d wcsPt(x, y, 0.0);
  const OdGsView* pView = activeView();
  if (pView->isPerspective())
  {
    wcsPt.z = pView->projectionMatrix()(2,3);
  }
  wcsPt.transformBy((pView->screenMatrix() * pView->projectionMatrix()).inverse());
  wcsPt.z =0.;
  // eye CS at this point.

  wcsPt.transformBy(OdAbstractViewPEPtr(pView)->eyeToWorld(pView)); 
  return wcsPt;
}

OdGePoint3d OdDgExEditorObject::toScreenCoord(int x, int y) const
{
  OdGePoint3d scrPt(x, y, 0.0);
  const OdGsView* pView = activeView();
  scrPt.transformBy((pView->screenMatrix() * pView->projectionMatrix()).inverse());
  scrPt.z =0.;
  return scrPt;
}

OdGePoint3d OdDgExEditorObject::toScreenCoord(const OdGePoint3d &wcsPt) const
{
  // To DCS
  OdGePoint3d scrPt(wcsPt);
  const OdGsView* pView = activeView();
  OdGsClientViewInfo viewInfo;
  pView->clientViewInfo(viewInfo);
  OdRxObjectPtr pObj = OdDgElementId(viewInfo.viewportObjectId).openObject();
  OdAbstractViewPEPtr pVp(pObj);
  OdGeVector3d vecY = pVp->upVector(pObj);
  OdGeVector3d vecZ = pVp->direction(pObj);
  OdGeVector3d vecX = vecY.crossProduct(vecZ).normal();
  OdGeVector2d offset = pVp->viewOffset(pObj);
  OdGePoint3d prTarg = pVp->target(pObj) - vecX * offset.x - vecY * offset.y;
  scrPt.x = vecX.dotProduct(wcsPt - prTarg);
  scrPt.y = vecY.dotProduct(wcsPt - prTarg);
  scrPt.z =0.;
  return scrPt;
}

unsigned OdDgExEditorObject::getSnapModes() const
{
  return m_osnapMan.snapModes();
}

void OdDgExEditorObject::setSnapModes( bool bSnapOn, unsigned dwSnapModes )
{
  m_bSnapOn = bSnapOn;
  m_osnapMan.setSnapModes( dwSnapModes );
}

void OdDgExEditorObject::set3DView(_3DViewType type)
{
  OdGePoint3d position, target = OdGePoint3d::kOrigin;
  OdGeVector3d axis;
  switch (type)
  {
    case k3DViewTop:
      position = OdGePoint3d::kOrigin + OdGeVector3d::kZAxis;
      axis = OdGeVector3d::kYAxis;
    break;
    case k3DViewBottom:
      position = OdGePoint3d::kOrigin - OdGeVector3d::kZAxis;
      axis = OdGeVector3d::kYAxis;
    break;
    case k3DViewLeft:
      position = OdGePoint3d::kOrigin - OdGeVector3d::kXAxis;
      axis = OdGeVector3d::kZAxis;
    break;
    case k3DViewRight:
      position = OdGePoint3d::kOrigin + OdGeVector3d::kXAxis;
      axis = OdGeVector3d::kZAxis;
    break;
    case k3DViewFront:
      position = OdGePoint3d::kOrigin - OdGeVector3d::kYAxis;
      axis = OdGeVector3d::kZAxis;
    break;
    case k3DViewBack:
      position = OdGePoint3d::kOrigin + OdGeVector3d::kYAxis;
      axis = OdGeVector3d::kZAxis;
    break;
	case k3DViewSW:
		position = OdGePoint3d::kOrigin + OdGeVector3d(-1.0, -1.0, 1.0);
		axis = OdGeVector3d(0.5, 0.5, 1.0).normal();
	break;
	case k3DViewSE:
		position = OdGePoint3d::kOrigin + OdGeVector3d(1.0, -1.0, 1.0);
		axis = OdGeVector3d(-0.5, 0.5, 1.0).normal();
	break;
	case k3DViewNE:
		position = OdGePoint3d::kOrigin + OdGeVector3d(1.0, 1.0, 1.0);
		axis = OdGeVector3d(-0.5, -0.5, 1.0).normal();
	break;
	case k3DViewNW:
		position = OdGePoint3d::kOrigin + OdGeVector3d(-1.0, 1.0, 1.0);
		axis = OdGeVector3d(0.5, -0.5, 1.0).normal();
	break;
  };

  unselect();

  OdGsView* pView = activeView();
  
  { // #7727
    OdGsClientViewInfo cliInfo;
    pView->clientViewInfo(cliInfo);
    OdDgElementPtr pObject = OdDgElementId(cliInfo.viewportObjectId).safeOpenObject(OdDg::kForWrite);
    //OdAbstractViewPEPtr(pObject)->setUcs(pObject, target, axis.crossProduct(position.asVector()), axis);
  }

  pView->setView(position, target, axis, pView->fieldWidth(), pView->fieldHeight(),
                 (pView->isPerspective()) ? OdGsView::kPerspective : OdGsView::kParallel);
}

bool OdDgExEditorObject::snap(OdGePoint3d& pt, const OdGePoint3d* pLastPt)
{
  if ( m_bSnapOn )
  {
    if(m_osnapMan.snap( activeView(), pt, pLastPt ))
    {
      if (!m_p2dModel.isNull())
        m_p2dModel->onModified(&m_osnapMan, (OdGiDrawable*)0);
      return true;
    }
  }
  return false;
}

bool OdDgExEditorObject::unselect()
{
  bool bRes = false;
  OdDgSelectionSetPtr pSSet = workingSSet();
  OdDgSelectionSetIteratorPtr pIter = pSSet->newIterator();
  while ( !pIter->done() )
  {
    OdDgElementPtr pEnt = OdDgElement::cast(pIter->objectId().openObject());
    if( pEnt.get() )
    {
      pEnt->highlight(false);
      bRes = true;
    }
    pIter->next();
  }
  // Don't clear working selection set to prevent previous selection modification
  //pSSet->clear();
  pSSet = OdDgSelectionSet::createObject(pSSet->database());
  setWorkingSelectionSet(m_pCmdCtx, pSSet);
  m_gripManager.selectionSetChanged(pSSet);
  return bRes;
}

bool OdDgExEditorObject::OnCtrlClick()
{
  return m_gripManager.onControlClick();
}

void OdDgExEditorObject::ShowStatusBarSelectedElementId(OdDgSelectionSetPtr pSSet)
{
	CMainFrame* pWnd = (CMainFrame*)theApp.GetMainWnd();
	if ((pSSet.isNull()) || (pSSet->elementIdArray().empty()))
	{
		pWnd->setStatusText(0, OD_T("Ready"));
		return;
	}
		
	OdDgElementId selectedElementId = pSSet->elementIdArray().first();

	if (!selectedElementId.isNull())
	{
		UINT uSelectedElementId = (OdUInt64)selectedElementId.getHandle();

		char text[500];
		sprintf(text, "Selected element id: %x", uSelectedElementId);
		OdString sSelectedElementId(text);

		pWnd->setStatusText(0, sSelectedElementId);
	}
	else
	{
		pWnd->setStatusText(0, OD_T("Ready"));
	}
}

bool OdDgExEditorObject::OnMouseLeftButtonClick(unsigned int nFlags, int x, int y,
                                              DgOleDragCallback* pDragCallback)
{
  bool bShift = (OdEdBaseIO::kShiftIsDown & nFlags) != 0;
  bool bCtrl = (OdEdBaseIO::kControlIsDown & nFlags) != 0;

  OdGePoint3d pt = toEyeToWorld(x, y);

  if ( m_gripManager.onMouseDown( x, y, bShift ) )
  {
    m_osnapMan.reset();
    return true;
  }

  OdDgUserIO* pIO = m_pCmdCtx->dbUserIO();
  pIO->setLASTPOINT( pt );
  pIO->setPickfirst( 0 );

  int iOpt = OdEd::kSelPickLastPoint | OdEd::kSelSinglePass | OdEd::kSelLeaveHighlighted | OdEd::kSelAllowInactSpaces;

  if( !bCtrl )
  {
    unselect();
  }

  OdDgSelectionSetPtr pSSet;
  try
  {
    OdDgSelectionSetPtr pWorkingSSet = workingSSet();
    OdSaveState<bool> ss_m_bSnapOn(m_bSnapOn, false);
    pSSet = pIO->select( activeView(), OdString::kEmpty, iOpt, pWorkingSSet );
    setWorkingSSet( pSSet );	
	ShowStatusBarSelectedElementId(pSSet);
  }
  catch( const OdError& )
  {
    return( false );
  }
  selectionSetChanged();

  return true;
}

bool OdDgExEditorObject::OnMouseLeftButtonDoubleClick(unsigned int nFlags, int x, int y)
{
  // Change viewport.
  OdGsView* pView = activeView();
  m_pDevice->setActiveViewport(OdGePoint2d(x, y));
  bool bChanged = pView!=activeView();
  if (bChanged)
  {
    // @@@ probably move this code to GsLayoutHelper's?
    OdDgElementPtr pObj = activeVpId().safeOpenObject();
    OdDgDatabase *pDb = pObj->database();
//     if (pDb->getTILEMODE())
//       OdDbViewportTable::cast(pDb->getViewportTableId().safeOpenObject(OdDb::kForWrite))->SetActiveViewport(activeVpId());
//     else
//       OdDbLayout::cast(OdDbBlockTableRecord::cast(pDb->getPaperSpaceId().safeOpenObject())->getLayoutId().safeOpenObject(OdDb::kForWrite))->setActiveViewportId(activeVpId());
    unselect();
  }
  return false;
}

bool OdDgExEditorObject::OnMouseRightButtonDoubleClick(unsigned int nFlags, int x, int y) 
{
  unselect();

  OdGsView* pView = activeView();
  
  // set target to center of the scene, keep view direction:
  OdGePoint3d targ = pView->target();
  
  pView->setView(targ + OdGeVector3d::kZAxis, targ, OdGeVector3d::kYAxis, pView->fieldWidth(), pView->fieldHeight());

  return true;
}

bool OdDgExEditorObject::OnMouseMove(unsigned int nFlags, int x, int y)
{
  return m_gripManager.onMouseMove( x, y );
//  if ( hasSelection() && m_pGripPoints.get() )
//  {
//    OdGsView* pView = activeView();
//    bool bRes = m_pGripPoints->onMouseMove( OdGePoint2d(x, y), pView->worldToDeviceMatrix() );
//    if( bRes )
//    {
//#pragma MARKMESSAGE(Rect of active grip point should be passed in OdGsView::invalidate())
//      pView->invalidate();
//    }
//    return bRes;
//  }
//  return false;
}

void OdDgExEditorObject::dolly(int x, int y) 
{
  OdGsView* pView = activeView();
  OdGeVector3d vec(-x, -y, 0.0);
  vec.transformBy((pView->screenMatrix() * pView->projectionMatrix()).inverse());
  pView->dolly(vec);
}

bool OdDgExEditorObject::OnMouseWheel(unsigned int nFlags, int x, int y, short zDelta)
{
  OdGsView* pView = activeView();

  OdGePoint3d pos(pView->position());
  pos.transformBy(pView->worldToDeviceMatrix());
  // In 2d mode perspective zoom change lens length instead of fieldWidth/fieldHeight. This is non-standart
  // mode. Practically 2d mode can't be perspective.
  if (pView->isPerspective() && pView->mode() == OdGsView::k2DOptimized)
    pos = OdGePoint3d(0.5, 0.5, 0.0).transformBy(pView->screenMatrix());
  int vx, vy;
  vx = (int)OdRound(pos.x);
  vy = (int)OdRound(pos.y);
  vx = x - vx;
  vy = y - vy;
  dolly(-vx, -vy);
  pView->zoom(zDelta > 0 ? 1./.9 : .9);
  dolly(vx, vy);

  if (!m_p2dModel.isNull())
    m_p2dModel->invalidate(activeTopView());
  return true;
}

OdEdCommandPtr OdDgExEditorObject::command(const OdString& sCmdName)
{
  if(sCmdName==m_cmd_ZOOM.globalName())
    return &m_cmd_ZOOM;
  if(sCmdName==m_cmd_ORBIT.globalName())
    return &m_cmd_ORBIT;
  if(sCmdName==m_cmd_DOLLY.globalName())
    return &m_cmd_DOLLY;
  return OdEdCommandPtr();
}
