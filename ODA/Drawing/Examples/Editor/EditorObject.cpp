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
#include "Ge/GeRay3d.h"
#include "Gi/GiDrawableImpl.h"
#include "Gi/GiWorldDraw.h"
#include "Gs/Gs.h"
#include "Gs/GsBaseVectorizer.h"
#include "EditorObject.h"
#include "GiContextForDbDatabase.h"
#include "DbLayout.h"
#include "DbCommandContext.h"
#include "DbAbstractViewportData.h"
#include "DbViewport.h"
#include "DbBlockTableRecord.h"
#include "DbViewportTable.h"
#include "DbDictionary.h"
#include "DbVisualStyle.h"
#include "DbHostAppServices.h"
#include "OdDToStr.h"
#include "SaveState.h"
#include "ExTrackers.h"
#include "RxVariantValue.h"

static const int SNAP_SIZE = 10;

class ViewInteractivityMode
{
  bool      m_enabled;
  OdGsView* m_pView;
public:
  ViewInteractivityMode( OdRxVariantValue enable, OdRxVariantValue frameRate, OdGsView* pView )
  {
    m_enabled = false;
    m_pView = pView;
    if( !enable.isNull() )
    {
      m_enabled = (bool)(enable);
      if( m_enabled && !frameRate.isNull() )
      {
        double rate = (double)(( (frameRate.get())->getDouble() ));
        pView->beginInteractivity( rate );
      }
    }
  }
  ~ViewInteractivityMode()
  {
    if( m_enabled ) m_pView->endInteractivity();
  }
};

class EnableEnhRectFrame
{
  OdEdCommandContext *m_pCmdCtx;
public:
  EnableEnhRectFrame(OdEdCommandContext *pCmdCtx)
    : m_pCmdCtx(pCmdCtx)
  {
    m_pCmdCtx->setArbitraryData(OD_T("ExDbCommandContext_EnhRectFrame"), OdRxVariantValue(true));
  }
  ~EnableEnhRectFrame() { m_pCmdCtx->setArbitraryData(OD_T("ExDbCommandContext_EnhRectFrame"), NULL); }
};


void setWorkingSelectionSet(OdDbCommandContext* pCmdCtx, OdDbSelectionSet* pSSet)
{
  pCmdCtx->setArbitraryData(OD_T("OdaMfcApp Working Selection Set"), pSSet);
}

OdDbSelectionSetPtr workingSelectionSet(OdDbCommandContext* pCmdCtx)
{
  OdDbSelectionSetPtr pRes;
  if (pCmdCtx)
  {
    pRes = pCmdCtx->arbitraryData(OD_T("OdaMfcApp Working Selection Set"));
    if(pRes.isNull())
    {
      pRes = OdDbSelectionSet::createObject(pCmdCtx->database());
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


OdExEditorObject::OdExEditorObject()
  : m_flags(0)
  , m_pCmdCtx(0)
  , m_pBasePt(0)
{
  SETBIT(m_flags, kSnapOn, true);
}

void OdExEditorObject::initialize( OdGsDevice* pDevice, OdDbCommandContext* pCmdCtx )
{
  m_pDevice = pDevice;
  m_pCmdCtx = pCmdCtx;

  m_p2dModel = pDevice->createModel();

  if (!m_p2dModel.isNull())
  {
    m_p2dModel->setRenderType(OdGsModel::kDirect); // Skip Z-buffer for 2d drawables.
    m_p2dModel->setEnableViewExtentsCalculation(false); // Skip extents calculation.
    m_p2dModel->setRenderModeOverride(OdGsView::k2DOptimized); // Setup 2dWireframe mode for all underlying geometry.
    m_p2dModel->setVisualStyle(OdDbDictionary::cast(m_pCmdCtx->database()->getVisualStyleDictionaryId().openObject())->getAt(OdDb::kszVS2DWireframe));
  }

  m_gripManager.init( pDevice, m_p2dModel, pCmdCtx, workingSelectionSet );

  Set_Entity_centers();
}

void OdExEditorObject::uninitialize()
{
  OdDbSelectionSetPtr pSSet = workingSSet();
  if(pSSet.get())
  {
    pSSet->clear();
    m_gripManager.selectionSetChanged(pSSet);
  }
  m_gripManager.uninit();

  m_pDevice.release();
  m_pCmdCtx = 0;
}

void OdExEditorObject::initSnapping( OdGsView* pView, OdEdInputTracker* pTracker )
{
  m_osnapMan.track( pTracker );
  pView->add(&m_osnapMan, m_p2dModel);
}

void OdExEditorObject::uninitSnapping( OdGsView* pView )
{
  pView->erase(&m_osnapMan);
  m_osnapMan.track( NULL );
}

OdDbSelectionSetPtr OdExEditorObject::workingSSet() const
{
  return workingSelectionSet(m_pCmdCtx);
}

void OdExEditorObject::setWorkingSSet(OdDbSelectionSet* pSSet)
{
  setWorkingSelectionSet(m_pCmdCtx, pSSet);
}

void OdExEditorObject::selectionSetChanged()
{
  m_gripManager.selectionSetChanged(workingSSet());
}

const OdGsView* OdExEditorObject::activeView() const
{
  return m_pDevice->activeView();
}

OdGsView* OdExEditorObject::activeView()
{
  return m_pDevice->activeView();
}

const OdGsView* OdExEditorObject::activeTopView() const
{
  const OdGsView *pView = activeView();
  if(hasDatabase()) {
    if (!m_pCmdCtx->database()->getTILEMODE())
    {
      OdDbObjectPtr pVpObj = m_pCmdCtx->database()->activeViewportId().safeOpenObject();
      OdDbAbstractViewportDataPtr pAVD(pVpObj);
      if (!pAVD.isNull() && pAVD->gsView(pVpObj))
        pView = pAVD->gsView(pVpObj);
    }
  }
  return pView;
}

OdGsView* OdExEditorObject::activeTopView()
{
  return const_cast<OdGsView*>( const_cast<const OdExEditorObject*>(this)->activeTopView() );
}

OdDbObjectId OdExEditorObject::activeVpId() const
{
  OdGsClientViewInfo vi;
  ((OdGsView*)activeView())->clientViewInfo(vi);
  return OdDbObjectId(vi.viewportObjectId);
}

void OdExEditorObject::ucsPlane(OdGePlane& plane) const
{
  OdDbObjectPtr pVpObj = activeVpId().safeOpenObject();
  OdDbAbstractViewportDataPtr pAVD(pVpObj);
  OdGePoint3d ucsOrigin;
  OdGeVector3d ucsXAxis, ucsYAxis;
  pAVD->getUcs(pVpObj, ucsOrigin, ucsXAxis, ucsYAxis);
  double dElevation = pAVD->elevation(pVpObj);
  if (!OdZero(dElevation))
  {
    OdGeVector3d vElevation = ucsXAxis.crossProduct(ucsYAxis) * dElevation;
    ucsOrigin += vElevation;
  }
  plane.set(ucsOrigin, ucsXAxis, ucsYAxis);
}

OdGePoint3d OdExEditorObject::toEyeToWorld(int x, int y) const
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

bool OdExEditorObject::toUcsToWorld(OdGePoint3d& wcsPt) const
{
  const OdGsView* pView = activeView();
  OdGePlane plane;
  ucsPlane(plane);
  if (!pView->isPerspective())
  { // For orhogonal projection we simply check intersection between viewing direction and UCS plane.
    OdGeLine3d line(wcsPt, OdAbstractViewPEPtr(pView)->direction(pView));
    return plane.intersectWith(line, wcsPt);
  }
  else
  { // For perspective projection we emit ray from viewer position through WCS point.
    const double focalLength = -1.0 / pView->projectionMatrix()(3, 2);
    const OdGePoint3d pos = pView->target() + (OdAbstractViewPEPtr(pView)->direction(pView).normal() * focalLength);
    OdGeRay3d ray(pos, wcsPt);
    return plane.intersectWith(ray, wcsPt);
  }
}

OdGePoint3d OdExEditorObject::toScreenCoord(int x, int y) const
{
  OdGePoint3d scrPt(x, y, 0.0);
  const OdGsView* pView = activeView();
  scrPt.transformBy((pView->screenMatrix() * pView->projectionMatrix()).inverse());
  scrPt.z =0.;
  return scrPt;
}

OdGePoint3d OdExEditorObject::toScreenCoord(const OdGePoint3d &wcsPt) const
{
  // To DCS
  OdGePoint3d scrPt(wcsPt);
  const OdGsView* pView = activeView();
  OdGsClientViewInfo viewInfo;
  pView->clientViewInfo(viewInfo);
  OdRxObjectPtr pObj = OdDbObjectId(viewInfo.viewportObjectId).openObject();
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

bool OdExEditorObject::OnSize(unsigned int nFlags, int w, int h) {
  if(m_pDevice.get()) {
    m_pDevice->onSize(OdGsDCRect(0, w, h, 0));
    return true;
  }
  return false;
}

bool OdExEditorObject::OnPaintFrame(unsigned int nFlags, OdGsDCRect* pUpdatedRect) {
  if(m_pDevice.get() && !m_pDevice->isValid()) {
    m_pDevice->update(pUpdatedRect);
    return true;
  }
  return false;
}

unsigned OdExEditorObject::getSnapModes() const
{
  return m_osnapMan.snapModes();
}

void OdExEditorObject::setSnapModes( bool bSnapOn, unsigned dwSnapModes )
{
  SETBIT(m_flags, kSnapOn, bSnapOn);
  m_osnapMan.setSnapModes( dwSnapModes );
}

OdEdCommandPtr OdExEditorObject::command(const OdString& sCmdName)
{
  if(sCmdName==m_cmd_ZOOM.globalName())
    return &m_cmd_ZOOM;
  if(sCmdName==m_cmd_3DORBIT.globalName())
    return &m_cmd_3DORBIT;
  if(sCmdName==m_cmd_DOLLY.globalName())
    return &m_cmd_DOLLY;
  if( sCmdName == m_cmd_INTERACTIVITY.globalName() )
    return &m_cmd_INTERACTIVITY;
  if( sCmdName == m_cmd_COLLIDE.globalName() )
    return &m_cmd_COLLIDE;
  if( sCmdName == m_cmd_COLLIDE_ALL.globalName() )
    return &m_cmd_COLLIDE_ALL;
  return OdEdCommandPtr();
}

void OdExEditorObject::set3DView(_3DViewType type)
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
    OdDbObjectPtr pObject = OdDbObjectId(cliInfo.viewportObjectId).safeOpenObject(OdDb::kForWrite);
    OdAbstractViewPEPtr(pObject)->setUcs(pObject, target, axis.crossProduct(position.asVector()), axis);
  }

  pView->setView(position, target, axis, pView->fieldWidth(), pView->fieldHeight(),
                 (pView->isPerspective()) ? OdGsView::kPerspective : OdGsView::kParallel);
}

bool OdExEditorObject::snap(OdGePoint3d& pt, const OdGePoint3d* pLastPt)
{
  if ( isSnapOn() )
  {
    if(m_osnapMan.snap( activeView(), pt, m_pBasePt ))
    {
      if (!m_p2dModel.isNull())
        m_p2dModel->onModified(&m_osnapMan, (OdGiDrawable*)0);
      return true;
    }
  }
  return false;
}

bool OdExEditorObject::unselect()
{
  bool bRes = false;
  OdDbSelectionSetPtr pSSet = workingSSet();
  OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
  while ( !pIter->done() )
  {
    OdDbEntityPtr pEnt = OdDbEntity::cast(pIter->objectId().openObject());
    if( pEnt.get() )
    {
      pEnt->highlight(false);
      bRes = true;
    }
    pIter->next();
  }
  // Don't clear working selection set to prevent previous selection modification
  //pSSet->clear();
  pSSet = OdDbSelectionSet::createObject(pSSet->database());
  setWorkingSelectionSet(m_pCmdCtx, pSSet);
  m_gripManager.selectionSetChanged(pSSet);
  return bRes;
}

bool OdExEditorObject::OnCtrlClick()
{
  return m_gripManager.onControlClick();
}

void OdExEditorObject::OnDestroy() {
  m_pDevice.release();
  m_p2dModel.release();
  m_pCmdCtx = 0;
}

bool OdExEditorObject::OnMouseLeftButtonClick(unsigned int nFlags, int x, int y,
                                              OleDragCallback* pDragCallback)
{
  bool bShift = (OdEdBaseIO::kShiftIsDown & nFlags) != 0;
  bool bCtrl = (OdEdBaseIO::kControlIsDown & nFlags) != 0;
  OdGePoint3d pt = toEyeToWorld(x, y);

  if ( m_gripManager.onMouseDown( x, y, bShift ) )
    return true;

  try
  {
    // Should be here I guess.
    if ( pDragCallback && !bShift )
    {
      OdDbSelectionSetPtr pWorkSet = workingSSet();
      OdDbSelectionSetPtr pAtPointSet = OdDbSelectionSet::select( activeVpId(), 1, &pt, OdDbVisualSelection::kPoint, bCtrl ? OdDbVisualSelection::kEnableSubents:OdDbVisualSelection::kDisableSubents );
      OdDbSelectionSetIteratorPtr pIter = pAtPointSet->newIterator();
      while( !pIter->done() )
      {
        if( pWorkSet->isMember( pIter->objectId() ) && !bCtrl )
        {
          pIter.release();
          break;
        }
        pIter->next();
      }
      if( pIter.isNull() )
      {
        if( pDragCallback->beginDragCallback( pt ) )
        {
          // Not good idea to clear selection set if already selected object has been selected, but if selection
          // set is being cleared - items must be unhighlighted too.
          //workingSSet()->clear();
          //selectionSetChanged();
          unselect();
          return( true );
        }
      }
    }
  }
  catch( const OdError& )
  {
    return( false );
  }

  OdDbUserIO* pIO = m_pCmdCtx->dbUserIO();
  pIO->setLASTPOINT( pt );
  pIO->setPickfirst( 0 );

  int iOpt = OdEd::kSelPickLastPoint | OdEd::kSelSinglePass | OdEd::kSelLeaveHighlighted | OdEd::kSelAllowInactSpaces;
  if(hasDatabase()) {
    if ( bShift )
    {
      if (m_pCmdCtx->database()->appServices()->getPICKADD() > 0)
        iOpt |= OdEd::kSelRemove;
    }
    else
    {
      if (m_pCmdCtx->database()->appServices()->getPICKADD() == 0)
        unselect();
    }
  }
  if (bCtrl)
  {
    iOpt |= OdEd::kSelAllowSubents;
  }

  OdDbSelectionSetPtr pSSet;
  bool savedSnapMode = isSnapOn();
  try
  {
    EnableEnhRectFrame _enhRect(m_pCmdCtx);
    setSnapOn(false);
    pSSet = pIO->select( OdString::kEmpty, iOpt, workingSSet() );
    setWorkingSSet( pSSet );
    setSnapOn(savedSnapMode);
  }
  catch( const OdError& )
  {
    setSnapOn(savedSnapMode);
    return( false );
  }
  catch( ... )
  {
    setSnapOn(savedSnapMode);
    throw;
  }
  selectionSetChanged();

  return true;
}

bool OdExEditorObject::OnMouseLeftButtonDoubleClick(unsigned int nFlags, int x, int y)
{
  OdGsView* pView = activeView();
  m_pDevice->setActiveViewport(OdGePoint2d(x, y));
  bool bChanged = pView!=activeView();
  if (bChanged)
  {
    // @@@ probably move this code to GsLayoutHelper's?
    OdDbObjectPtr pObj = activeVpId().safeOpenObject();
    OdDbDatabase *pDb = pObj->database();
    if (pDb->getTILEMODE())
      OdDbViewportTable::cast(pDb->getViewportTableId().safeOpenObject(OdDb::kForWrite))->SetActiveViewport(activeVpId());
    else
      OdDbLayout::cast(OdDbBlockTableRecord::cast(pDb->getPaperSpaceId().safeOpenObject())->getLayoutId().safeOpenObject(OdDb::kForWrite))->setActiveViewportId(activeVpId());
    unselect();
  }
  return bChanged;
}

bool OdExEditorObject::OnMouseRightButtonDoubleClick(unsigned int nFlags, int x, int y) 
{
  unselect();

  OdGsView* pView = activeView();
  
  // set target to center of the scene, keep view direction:
  OdGePoint3d targ = pView->target();
  
  pView->setView(targ + OdGeVector3d::kZAxis, targ, OdGeVector3d::kYAxis, pView->fieldWidth(), pView->fieldHeight());

  return true;
}

bool OdExEditorObject::OnMouseMove(unsigned int nFlags, int x, int y)
{
  bool bShift = (OdEdBaseIO::kShiftIsDown & nFlags) != 0;
  return m_gripManager.onMouseMove(x, y, bShift);
}

void OdExEditorObject::dolly(int x, int y) 
{
  OdGsView* pView = activeView();
  dolly(pView, x, y);
}

void OdExEditorObject::dolly(OdGsView* pView, int x, int y) {
  OdGeVector3d vec(-x, -y, 0.0);
  vec.transformBy((pView->screenMatrix() * pView->projectionMatrix()).inverse());
  pView->dolly(vec);
}

bool OdExEditorObject::OnMouseWheel(unsigned int nFlags, int x, int y, short zDelta)
{
  OdGsView* pView = activeView();
  zoomAt(pView, x, y, zDelta);

  if (!m_p2dModel.isNull())
    m_p2dModel->invalidate(activeTopView());
  return true;
}

void OdExEditorObject::zoomAt(OdGsView* pView, int x, int y, short zDelta) {
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
  dolly(pView, -vx, -vy);
  pView->zoom(zDelta > 0 ? 1./.9 : .9);
  dolly(pView, vx, vy);
}

const OdString OdExZoomCmd::groupName() const
{
  return globalName();
}

const OdString OdExZoomCmd::globalName() const
{
  return OD_T("ZOOM");
}

void zoom_window(OdGePoint3d& pt1, OdGePoint3d& pt2, OdGsView* pView)
{
  OdGeMatrix3d xWorldToEye = OdAbstractViewPEPtr(pView)->worldToEye(pView);
  pt1.transformBy(xWorldToEye);
  pt2.transformBy(xWorldToEye);
  OdGeVector3d eyeVec = pt2 - pt1;
  if(OdNonZero(eyeVec.x) && OdNonZero(eyeVec.y))
  {
    OdGePoint3d newPos = pt1 + eyeVec / 2.;
    
    eyeVec.x = fabs(eyeVec.x);
    eyeVec.y = fabs(eyeVec.y);
    
    pView->dolly( newPos.asVector() );
    
    double wf = pView->fieldWidth()  / eyeVec.x;
    double hf = pView->fieldHeight() / eyeVec.y;
    
    pView->zoom(odmin(wf, hf));
  }
}

void zoom_window2(const OdGePoint3d& pt1, const OdGePoint3d& pt2, OdGsView* pView) {
  OdGePoint3d pt1c = pt1;
  OdGePoint3d pt2c = pt2;
  zoom_window(pt1c, pt2c, pView);
}

void zoom_scale(double factor)
{
}

static bool getLayoutExtents(const OdDbObjectId& spaceId, const OdGsView* pView, OdGeBoundBlock3d& bbox)
{
  OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject();
  OdDbLayoutPtr pLayout = pSpace->getLayoutId().safeOpenObject();
  OdGeExtents3d ext;
  if(pLayout->getGeomExtents(ext)==eOk)
  {
    ext.transformBy(pView->viewingMatrix());
    bbox.set(ext.minPoint(), ext.maxPoint());
    return (ext.minPoint()!=ext.maxPoint());
  }
  return false;
}

void zoom_extents(OdGsView* pView, OdDbObject* pVpObj)
{
  OdDbDatabase* pDb = pVpObj->database();
  OdAbstractViewPEPtr pVpPE(pView);
  OdGeBoundBlock3d bbox;
  bool bBboxValid = pVpPE->viewExtents(pView, bbox);

  // paper space overall view
  OdDbViewportPtr pVp = OdDbViewport::cast(pVpObj);
  if(pVp.get() && pVp->number() == 1)
  {
    if(!bBboxValid || !(bbox.minPoint().x < bbox.maxPoint().x && bbox.minPoint().y < bbox.maxPoint().y))
    {
      bBboxValid = ::getLayoutExtents(pDb->getPaperSpaceId(), pView, bbox);
    }
  }
  else if(!bBboxValid) // model space viewport
  {
    bBboxValid = ::getLayoutExtents(pDb->getPaperSpaceId(), pView, bbox);
  }

  if(!bBboxValid)
  {
    // set to somewhat reasonable (e.g. paper size)
    if(pDb->getMEASUREMENT()==OdDb::kMetric)
    {
      bbox.set(OdGePoint3d::kOrigin, OdGePoint3d(297., 210., 0.)); // set to papersize ISO A4 (portrait)
    }
    else
    {
      bbox.set(OdGePoint3d::kOrigin, OdGePoint3d(11., 8.5, 0.)); // ANSI A (8.50 x 11.00) (landscape)
    }
    bbox.transformBy(pView->viewingMatrix());
  }

  pVpPE->zoomExtents(pView, &bbox);
}

void zoom_scaleXP(double factor)
{
}

// Zoom command

class RTZoomTracker : public OdEdPointTracker
{
  OdGsView*   m_pView;
  double      m_base;
  double      m_fw;
  double      m_fh;
public:
  void init(OdGsView* pView, const OdGePoint3d& base)
  {
    m_pView = pView;
    m_fw = pView->fieldWidth();
    m_fh = pView->fieldHeight();
    m_base = (m_pView->projectionMatrix() * m_pView->viewingMatrix() * base).y;
  }

  void setValue(const OdGePoint3d& value)
  {
    OdGeMatrix3d xWorldToNDC = m_pView->projectionMatrix() * m_pView->viewingMatrix();
    OdGePoint3d pt2 = xWorldToNDC * value;
    double fac = 1. + fabs(pt2.y - m_base) * 1.5;
    if(pt2.y > m_base)
      fac = 1./fac;
    m_pView->setView(m_pView->position(),
                     m_pView->target(),
                     m_pView->upVector(),
                     m_fw * fac,
                     m_fh * fac,
                     m_pView->isPerspective() ? OdGsView::kPerspective : OdGsView::kParallel
                    );
  }
  int addDrawables(OdGsView* /*pView*/) { return 1; }
  void removeDrawables(OdGsView* pView) {}
};

void OdExZoomCmd::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  
  const OdChar* szKeywords = OD_T("All Center Dynamic Extents Previous Scale Window Object");
  
  OdDbObjectPtr pVpObj = pDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
  OdDbAbstractViewportDataPtr pAVD(pVpObj);

  OdGsView* pView = pAVD->gsView(pVpObj);

  OdGePoint3d pt1, pt2;

  try
  {
    pt1 = pIO->getPoint(L"Specify corner of window, enter a scale factor (nX or nXP), or\n"
      L"[All/Center/Dynamic/Extents/Previous/Scale/Window/Object] <real time>:",
      OdEd::kInpThrowEmpty|OdEd::kInpThrowOther|OdEd::kGptNoOSnap, 0, szKeywords);
    
    pt2 = pIO->getPoint(OD_T("Specify opposite corner:"), OdEd::kGptNoUCS|OdEd::kGptRectFrame|OdEd::kGptNoOSnap);
    zoom_window(pt1, pt2, pView);
  }
  catch(const OdEdEmptyInput) // real time
  {
    OdStaticRxObject<RTZoomTracker> tracker;
    for(;;)
    {
      try
      {
        tracker.init(pView, pIO->getPoint(OD_T("Press ESC or ENTER to exit."),
          OdEd::kInpThrowEmpty|OdEd::kGptNoUCS|OdEd::kGptBeginDrag|OdEd::kGptNoOSnap));
        pIO->getPoint(OD_T("Press ESC or ENTER to exit."),
          OdEd::kInpThrowEmpty|OdEd::kGptNoUCS|OdEd::kGptEndDrag|OdEd::kGptNoOSnap, 0, OdString::kEmpty, &tracker);
      }
      catch(const OdEdCancel)
      {
        break;
      }
    }
  }
  catch(const OdEdOtherInput& otherInput) // nX or nXP
  {
    OdChar* pEnd;
    double scale = odStrToD(otherInput.string(), &pEnd);
    if(pEnd > otherInput.string().c_str())
    {
      OdString sEnd(pEnd);
      if(sEnd.iCompare(OD_T("X"))==0)
      {
        pView->zoom(scale);
      }
      else if(sEnd.iCompare(OD_T("XP"))==0)
      {
        zoom_scaleXP(scale);
      }
      else if(!*pEnd)
      {
        pView->zoom(scale);
      }
    }
    pIO->putString(OD_T("Requires a distance, numberX, or option keyword."));
  }
  catch(const OdEdKeyword& kw)
  {
    switch(kw.keywordIndex())
    {
    case 0: // All
      break;
    case 1: // Center
      break;
    case 2: // Dynamic
      break;
    case 3: // Extents
      ::zoom_extents(pView, pVpObj);
      break;
    case 4: // Previous
      break;
    case 5: // Scale
      break;
    case 6: // Window
      pt1 = pIO->getPoint(OD_T("Specify first corner:"),    OdEd::kGptNoUCS|OdEd::kGptNoOSnap);
      pt2 = pIO->getPoint(OD_T("Specify opposite corner:"), OdEd::kGptNoUCS|OdEd::kGptNoOSnap|OdEd::kGptRectFrame);
      ::zoom_window(pt1, pt2, pView);
      break;
    case 7: // Object
      break;
    }
  }

  pAVD->setView(pVpObj, pView);
}

// 3d orbit command

const OdString OdEx3dOrbitCmd::groupName() const
{
  return globalName();
}

const OdString OdEx3dOrbitCmd::globalName() const
{
  return OD_T("3DORBIT");
}

class OrbitCtrl: public OdGiDrawableImpl<>
{
public:
  OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const
  {
    return kDrawableIsAnEntity | kDrawableRegenDraw;
  }
  bool subWorldDraw(OdGiWorldDraw* pWd) const
  {
    return false;
  }
  void subViewportDraw(OdGiViewportDraw* pVd) const
  {
    OdGiViewport& vp = pVd->viewport();
    OdGiGeometry& geom = pVd->geometry();
    pVd->subEntityTraits().setColor(OdCmEntityColor::kACIGreen);
    pVd->subEntityTraits().setFillType(kOdGiFillNever);

    OdGiModelTransformSaver mt(geom, vp.getEyeToModelTransform());
    OdGiDrawFlagsHelper _dfh(pVd->subEntityTraits(), OdGiSubEntityTraits::kDrawNoPlotstyle);

    OdGePoint3d pt1;
    OdGePoint2d pt2;
    vp.getViewportDcCorners((OdGePoint2d&)pt1, pt2);
    pt2.x -= pt1.x;
    pt2.y -= pt1.y;
    double r = odmin(pt2.x, pt2.y) / 9. * 7. / 2.;
    ((OdGePoint2d&)pt1) += (pt2.asVector() / 2.);
    geom.circle(pt1, r, OdGeVector3d::kZAxis);

    geom.circle(pt1 + OdGeVector3d(0., r, 0.), r / 20., OdGeVector3d::kZAxis);
    geom.circle(pt1 + OdGeVector3d(0.,-r, 0.), r / 20., OdGeVector3d::kZAxis);
    geom.circle(pt1 + OdGeVector3d( r, 0.,0.), r / 20., OdGeVector3d::kZAxis);
    geom.circle(pt1 + OdGeVector3d(-r, 0.,0.), r / 20., OdGeVector3d::kZAxis);
  }
};

class RTOrbitTracker : public OdEdPointTracker
{
  OdGsView*       m_pView;
  OdGePoint3d     m_pt;
  OdGiDrawablePtr m_pDrw;
  OdGePoint3d     m_pos;
  OdGePoint3d     m_trg;
  OdGeVector3d    m_up;
  OdGeVector3d    m_x;
  OdGePoint3d     m_viewCenter;
  OdGeMatrix3d    m_initViewingMatrixInv;
  double          m_D; // diameter of orbit control in projected coordinates
  OdGsModelPtr    m_pModel;

  enum Axis
  {
    kHorizontal,
    kVertical,
    kPerpDir, // orbit around perpendicular to mouse direction
    kEye,
  }
  m_axis;

  void viewportDcCorners(OdGePoint2d& lower_left, OdGePoint2d& upper_right) const
  {
    OdGePoint3d target = m_pView->viewingMatrix() * m_pView->target();
    double halfFieldWidth = m_pView->fieldWidth() / 2.0;
    double halfFieldHeight = m_pView->fieldHeight() / 2.0;
    lower_left.x = target.x - halfFieldWidth;
    lower_left.y = target.y - halfFieldHeight;
    upper_right.x = target.x + halfFieldWidth;
    upper_right.y = target.y + halfFieldHeight;
  }
public:
  RTOrbitTracker()
    : m_pView(0)
    , m_D(0)
  {
  }
  void reset() { m_pView = 0; }
  void init(OdGsView* pView, const OdGePoint3d& pt)
  {
    m_pView = pView;
    m_pos = pView->position();
    m_trg = pView->target();
    m_up = pView->upVector();
    m_x = m_up.crossProduct(pView->target() - m_pos).normal();

    m_initViewingMatrixInv = m_pView->viewingMatrix();
    m_pt = m_initViewingMatrixInv * pt;
    m_pt.z = 0.;
    m_initViewingMatrixInv.invert();

    OdGePoint3d pt1;
    OdGePoint2d pt2;
    viewportDcCorners((OdGePoint2d&)pt1, pt2);
    pt2.x -= pt1.x;
    pt2.y -= pt1.y;
    double r = odmin(pt2.x, pt2.y) / 9. * 7. / 2.;
    m_D = 2.0 * r;
    ((OdGePoint2d&)pt1) += (pt2.asVector() / 2.);
    double r2sqrd = r*r / 400.;

    pt1.y += r;
    if((pt1 - m_pt).lengthSqrd() <= r2sqrd)
    {
      m_axis = kHorizontal;
    }
    else
    {
      pt1.y -= r;
      pt1.y -= r;
      if((pt1 - m_pt).lengthSqrd() <= r2sqrd)
      {
        m_axis = kHorizontal;
      }
      else
      {
        pt1.y += r;
        pt1.x += r;
        if((pt1 - m_pt).lengthSqrd() <= r2sqrd)
        {
          m_axis = kVertical;
        }
        else
        {
          pt1.x -= r;
          pt1.x -= r;
          if((pt1 - m_pt).lengthSqrd() <= r2sqrd)
          {
            m_axis = kVertical;
          }
          else
          {
            pt1.x += r;
            if((pt1 - m_pt).lengthSqrd() <= r*r)
            {
              m_axis = kPerpDir;
            }
            else
            {
              m_axis = kEye;
            }
          }
        }
      }
    }

    bool bComputeExtents = true;
    { // Try to extract cached extents
      OdGsClientViewInfo viewInfo;
      pView->clientViewInfo(viewInfo);
      OdDbObjectId spaceId;
      if (!GETBIT(viewInfo.viewportFlags, OdGsClientViewInfo::kDependentGeometry))
        spaceId = OdDbDatabasePtr(pView->userGiContext()->database())->getModelSpaceId();
      else
        spaceId = OdDbDatabasePtr(pView->userGiContext()->database())->getPaperSpaceId();
      OdDbObjectPtr pBTR = spaceId.openObject();
      OdGeExtents3d wcsExt;
      if (pBTR->gsNode() && pBTR->gsNode()->extents(wcsExt))
        m_viewCenter = wcsExt.center(), bComputeExtents = false;
    }
    if (bComputeExtents)
    { // Compute extents if no extents cached
      OdAbstractViewPEPtr pAView = pView;
      OdGeBoundBlock3d extents;
      pAView->viewExtents(pView, extents);
      m_viewCenter = extents.center();
      m_viewCenter.transformBy(m_initViewingMatrixInv);
    }
  }

  double angle(const OdGePoint3d& value) const
  {
    OdGePoint3d pt2 = m_pView->viewingMatrix() * value;
    double dist = 0.0;
    if(m_axis == kHorizontal)
      dist = pt2.y - m_pt.y;
    else if(m_axis == kVertical)
      dist = pt2.x - m_pt.x;
    return dist * OdaPI / m_D;
  }
  
  double angleZ(const OdGePoint3d& value) const
  {
    OdGePoint3d pt2 = m_pView->viewingMatrix() * value;
    OdGePoint3d targ = m_pView->viewingMatrix() * m_viewCenter;
    pt2.z = targ.z = m_pt.z;
    return (pt2 - targ).angleTo((m_pt - targ), OdGeVector3d::kZAxis);
  }

  double anglePerp(const OdGePoint3d& value) const
  {
    OdGePoint3d pt2 = m_pView->viewingMatrix() * value;
    pt2.z = 0.0;
    return pt2.distanceTo(m_pt) * OdaPI / m_D;
  }
  
  void setValue(const OdGePoint3d& value)
  {
    if(m_pView)
    {
      OdGeMatrix3d x;
      switch(m_axis)
      {
      case kHorizontal:
        x.setToRotation(-angle(value), m_x, m_viewCenter);
        break;
      case kVertical:
        x.setToRotation(-angle(value), m_up, m_viewCenter);
        break;
      case kEye:
        x.setToRotation(-angleZ(value), m_trg - m_pos, m_viewCenter);
        break;
      case kPerpDir:
        {
          OdGePoint3d value1 = value;
          value1.transformBy(m_pView->viewingMatrix());
          value1.z = 0.0;
          OdGeVector2d dir = (value1 - m_pt).convert2d();
          OdGeVector2d perp = dir.perpVector();
          OdGeVector3d perp3d(perp.x, perp.y, 0.0);
          perp3d.normalizeGetLength();
          perp3d.transformBy(m_initViewingMatrixInv);
          x.setToRotation(-anglePerp(value), perp3d, m_viewCenter);
          break;
        }
      }
      OdGePoint3d newPos = x * m_pos, newTarget = x * m_trg;
      OdGeVector3d newPosDir = newPos - newTarget;
      newPosDir.normalizeGetLength();
      newPosDir *= m_pos.distanceTo(m_trg);
      newPos = newTarget + newPosDir;

      m_pView->setView(newPos,
                       newTarget,
                       x * m_up,
                       m_pView->fieldWidth(),
                       m_pView->fieldHeight(),
                       m_pView->isPerspective() ? OdGsView::kPerspective : OdGsView::kParallel
                      );
    }
  }

  int addDrawables(OdGsView* pView)
  {
    m_pDrw = OdRxObjectImpl<OrbitCtrl>::createObject();
    if (m_pModel.isNull())
    {
      m_pModel = pView->device()->createModel();
      if (!m_pModel.isNull())
      {
        m_pModel->setRenderType(OdGsModel::kDirect); // Skip Z-buffer for 2d drawables.
        m_pModel->setEnableViewExtentsCalculation(false); // Skip extents calculation.
        m_pModel->setRenderModeOverride(OdGsView::k2DOptimized); // Setup 2dWireframe mode for all underlying geometry.
        OdDbStub *visualStyleId = GraphTrackerBase::getVisualStyleOverride(pView->userGiContext()->database());
        if (visualStyleId) m_pModel->setVisualStyle(visualStyleId); // 2dWireframe visual style.
      }
    }
    pView->add(m_pDrw, m_pModel.get());
    return 1;
  }

  void removeDrawables(OdGsView* pView)
  {
    pView->erase(m_pDrw);
  }
};

void OdEx3dOrbitCmd::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  
  OdDbObjectPtr pVpObj = pDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
  OdDbAbstractViewportDataPtr pAVD(pVpObj);

  OdGsView* pView = pAVD->gsView(pVpObj);  

  // There is one special case: layout with enabled 'draw viewports first' mode
  {
    if (!pDb->getTILEMODE())
    {
      OdDbLayoutPtr pLayout = pDb->currentLayoutId().openObject();
      if (pLayout->drawViewportsFirst())
      {
        if (pView->device()->viewAt(pView->device()->numViews() - 1) == pView)
          pView = pView->device()->viewAt(0);
      }
    }
  }
  //

  OdRxVariantValue interactiveMode = (OdRxVariantValue)pCmdCtx->arbitraryData( OD_T("OdaMfcApp InteractiveMode" ) );
  OdRxVariantValue interactiveFrameRate = (OdRxVariantValue)pCmdCtx->arbitraryData( OD_T("OdaMfcApp InteractiveFrameRate" ) );
  ViewInteractivityMode mode( interactiveMode, interactiveFrameRate, pView );

  OdStaticRxObject<RTOrbitTracker> tracker;
  for(;;)
  {
    try
    {
      tracker.init(pView, pIO->getPoint(OD_T("Press ESC or ENTER to exit."),
        OdEd::kInpThrowEmpty|OdEd::kGptNoUCS|OdEd::kGptNoOSnap|OdEd::kGptBeginDrag, 0, OdString::kEmpty, &tracker));
      pIO->getPoint(OD_T("Press ESC or ENTER to exit."),
        OdEd::kInpThrowEmpty|OdEd::kGptNoUCS|OdEd::kGptNoOSnap|OdEd::kGptEndDrag, 0, OdString::kEmpty, &tracker);
      tracker.reset();
    }
    catch(const OdEdCancel)
    {
      break;
    }
  }
}

void OdExEditorObject::turnOrbitOn(bool bOn) {
  SETBIT(m_flags, kOrbitOn, bOn);
  setTracker(bOn ? OdRxObjectImpl<RTOrbitTracker>::createObject().get() : 0);
}

bool OdExEditorObject::OnOrbitBeginDrag(int x, int y) {
  if(isOrbitOn()) {
    ((RTOrbitTracker*)m_pTracker.get())->init(activeView(), toEyeToWorld(x,y));
    return true;
  }
  return false;
}

bool OdExEditorObject::OnOrbitEndDrag(int x, int y) {
  if(isOrbitOn()) {
    ((RTOrbitTracker*)m_pTracker.get())->reset();
    return true;
  }
  return false;
}

bool OdExEditorObject::OnZoomWindowBeginDrag(int x, int y) {
  OdGePoint3d pt = toEyeToWorld(x,y);
  setTracker(RectFrame::create(pt, gsModel()));
  trackPoint(pt);
  return true;
}

bool OdExEditorObject::OnZoomWindowEndDrag(int x, int y) {
  ::zoom_window2(OdEdPointDefTrackerPtr(m_pTracker)->basePoint(), toEyeToWorld(x,y), activeView());
  setTracker(0);
  return true;
}


// Dolly command

const OdString OdExDollyCmd::groupName() const
{
  return globalName();
}

const OdString OdExDollyCmd::globalName() const
{
  return OD_T("DOLLY");
}

class RTDollyTracker : public OdEdPointTracker
{
  OdGsView*       m_pView;
  OdGePoint3d     m_pt;
  OdGePoint3d     m_pos;
public:
  RTDollyTracker()
    : m_pView(0)
  {
  }
  void reset() { m_pView = 0; }
  void init(OdGsView* pView, const OdGePoint3d& pt)
  {
    m_pView = pView;
    m_pos = pView->position();
    m_pt = pt - m_pos.asVector();
  }
  
  void setValue(const OdGePoint3d& value)
  {
    if(m_pView)
    {
      OdGeVector3d delta = (m_pt - (value - m_pos)).asVector();
      m_pt = value - m_pos.asVector();
      delta.transformBy(m_pView->viewingMatrix());
      m_pView->dolly(delta.x, delta.y, delta.z);
      m_pos = m_pView->position();
    }
  }

  int addDrawables(OdGsView* /*pView*/)
  {
    return 0;
  }
  void removeDrawables(OdGsView* /*pView*/)
  {
  }
};

void OdExDollyCmd::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  
  OdDbObjectPtr pVpObj = pDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
  OdDbAbstractViewportDataPtr pAVD(pVpObj);

  OdGsView* pView = pAVD->gsView(pVpObj);

  // @@@ There is one special case: layout with enabled 'draw viewports first' mode
  {
    if (!pDb->getTILEMODE())
    {
      OdDbLayoutPtr pLayout = pDb->currentLayoutId().openObject();
      if (pLayout->drawViewportsFirst())
      {
        if (pView->device()->viewAt(pView->device()->numViews() - 1) == pView)
          pView = pView->device()->viewAt(0);
      }
    }
  }
  //

  OdRxVariantValue interactiveMode = (OdRxVariantValue)pCmdCtx->arbitraryData( OD_T("OdaMfcApp InteractiveMode" ) );
  OdRxVariantValue interactiveFrameRate = (OdRxVariantValue)pCmdCtx->arbitraryData( OD_T("OdaMfcApp InteractiveFrameRate" ) );
  ViewInteractivityMode mode( interactiveMode, interactiveFrameRate, pView );

  OdStaticRxObject<RTDollyTracker> tracker;
  for(;;)
  {
    try
    {
      tracker.init(pView, pIO->getPoint(OD_T("Press ESC or ENTER to exit."),
        OdEd::kInpThrowEmpty|OdEd::kGptNoUCS|OdEd::kGptNoOSnap|OdEd::kGptBeginDrag, 0, OdString::kEmpty, &tracker));
      pIO->getPoint(OD_T("Press ESC or ENTER to exit."),
        OdEd::kInpThrowEmpty|OdEd::kGptNoUCS|OdEd::kGptNoOSnap|OdEd::kGptEndDrag, 0, OdString::kEmpty, &tracker);
      tracker.reset();
    }
    catch(const OdEdCancel)
    {
      break;
    }
  }
}

//Interactivity commands
const OdString OdExInteractivityModeCmd::groupName() const
{
  return globalName();
}
const OdString OdExInteractivityModeCmd::globalName() const
{
  return OD_T("INTERACTIVITY");
}
void OdExInteractivityModeCmd::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  bool enable = pIO->getInt( OD_T("\nSet 0 to disable or non-zero to enable Interactivity Mode: ") ) != 0;
  if( enable )
  {
    double frameRate = pIO->getReal( OD_T("\nSpecify frame rate (Hz): ") );
    pCmdCtx->setArbitraryData( OD_T("OdaMfcApp InteractiveMode" ), OdRxVariantValue( true ) );
    pCmdCtx->setArbitraryData( OD_T("OdaMfcApp InteractiveFrameRate" ), OdRxVariantValue( frameRate ) );
  }
  else
  {
    pCmdCtx->setArbitraryData( OD_T("OdaMfcApp InteractiveMode" ), OdRxVariantValue( false ) );
  }
}

//Collision detection commands
const OdString OdExCollideCmd::groupName() const
{
  return globalName();
}
const OdString OdExCollideCmd::globalName() const
{
  return OD_T("COLLIDE");
}
#include "Gs/GsModel.h"
#include "Gi/GiPathNode.h"
#include "DbBlockReference.h"

class OdExCollideGsPath
{
  struct Node : OdGiPathNode
  {
    const Node*     m_pParent;
    OdDbStub*       m_pId;
    OdGiDrawablePtr m_pDrawable;
    OdGsMarker        m_gsMarker;

    const OdGiPathNode* parent() const { return m_pParent; }
    OdDbStub* persistentDrawableId() const { return m_pId; }
    const OdGiDrawable* transientDrawable() const { return m_pDrawable; }
    OdGsMarker selectionMarker() const { return m_gsMarker; }
  };
  const Node* m_pLeaf;

  void add(const OdGiDrawable* pDrawable, const OdDbObjectId& drawableId, OdGsMarker gsMarker = -1)
  {
    Node* pNode = new Node();
    pNode->m_pParent    = m_pLeaf;
    m_pLeaf = pNode;

    pNode->m_pDrawable  = pDrawable;
    pNode->m_pId        = drawableId;
    pNode->m_gsMarker   = gsMarker;
  }

  void addNode(OdDbObjectIdArray::const_iterator& iter)
  {
    OdDbObjectPtr pObj = iter->safeOpenObject();
    addNode(pObj);
    OdDbBlockReferencePtr pInsert = OdDbBlockReference::cast(pObj);
    if(pInsert.get())
      addNode(pInsert->blockTableRecord());
    ++iter;
  }
public:
  OdExCollideGsPath()
    : m_pLeaf(0)
  {
  }
  ~OdExCollideGsPath()
  {
	  clear();
  }
  OdExCollideGsPath(const OdDbFullSubentPath& path): m_pLeaf(0)
  {
	  set(path);
  }

  void clear()
  {
    while(m_pLeaf)
    {
      const Node* pNode = m_pLeaf;
      m_pLeaf = pNode->m_pParent;
      delete pNode;
    }
    m_pLeaf = NULL;
  }

  void set(const OdDbFullSubentPath& path)
  {
    set(path, kNullSubentIndex);
  }
  void set(const OdDbFullSubentPath& path, OdGsMarker gsMarker)
  {
    clear();
    const OdDbObjectIdArray& ids = path.objectIds();

    OdDbObjectIdArray::const_iterator iter = ids.begin();
    if(iter==ids.end())
      throw OdError(eInvalidInput);

    OdDbObjectPtr pObj = iter->safeOpenObject();
    addNode(pObj->ownerId());
    for(; iter != ids.end()-1; ++iter)
      addNode(*iter);

    addNode(*iter, gsMarker);
  }

  void addNode(const OdDbObjectId& drawableId, OdGsMarker gsMarker = kNullSubentIndex)
  {
    add(0, drawableId, gsMarker);
  }
  void addNode(const OdGiDrawable* pDrawable, OdGsMarker gsMarker = kNullSubentIndex)
  {
    add(pDrawable->isPersistent() ? 0 : pDrawable, pDrawable->id(), gsMarker);
  }

  operator const OdGiPathNode&() const
  {
    return *m_pLeaf;
  }
};

#define STL_USING_MAP
#include "OdaSTL.h"

class CollideMoveTracker : public OdStaticRxObject<OdEdPointTracker>
{
  OdArray<OdDbEntityPtr>  m_ents;   // Selection set entities
  OdGeMatrix3d            m_xForm;  // Last transformation

  OdArray< OdExCollideGsPath* > m_pathes;
  OdArray< OdExCollideGsPath* > m_prevHLPathes;

  OdArray< const OdGiPathNode* > inputArray;

protected:
  OdGePoint3d             m_ptBase;
  OdDbDatabasePtr         m_pDb;
  OdGsView*               m_pView;
  OdGsModel*              m_pModel;
  bool m_bDynHLT;

  virtual OdGeMatrix3d getTransform(const OdGePoint3d& value)
  {
    OdGeMatrix3d mRet;
    mRet.setTranslation(value - m_ptBase);
    return mRet;
  }
public:
  CollideMoveTracker(OdGePoint3d ptBase, OdDbSelectionSet* pSSet, OdDbDatabasePtr pDb, OdGsView* pView, bool bDynHLT)
    : m_ptBase(ptBase), m_bDynHLT( bDynHLT )
  {
    m_pDb = pDb;
    m_pView = pView;
    OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();
    m_pModel = NULL;

    //obtain GsModel

    while (!pIter->done())
    {
      OdDbObjectId objId  = pIter->objectId();
      OdDbEntityPtr pEnt  = objId.openObject(OdDb::kForWrite);

      if( !m_pModel && pEnt->gsNode() )
      {
        m_pModel = pEnt->gsNode()->model();
      }

      if (!pEnt.isNull())
      {
        OdDbEntityPtr pSubEnt;
        if (pIter->subentCount() == 0)
        {
          m_ents.push_back(pEnt);
        }
        else
        {
          OdDbFullSubentPath pathSubent;
          OdDbFullSubentPathArray arrPaths;

          for (unsigned int i = 0; i < pIter->subentCount(); i++)
          {
            pIter->getSubentity(i,pathSubent);
            pSubEnt = pEnt->subentPtr(pathSubent);
            if (!pSubEnt.isNull())
              m_ents.push_back(pSubEnt);
          }
        }
      }
      if( pEnt.isNull() ) continue;
      if( pIter->subentCount() == 0 )
      {
        OdExCollideGsPath* gsPath = new OdExCollideGsPath;
        gsPath->addNode( pIter->objectId().safeOpenObject()->ownerId() );
        gsPath->addNode( pIter->objectId() );
        m_pathes.push_back( gsPath );
        pEnt->dragStatus( OdDb::kDragStart );
      }
      else
      {
        for( OdUInt32 i = 0; i < pIter->subentCount(); ++i )
        {
          OdDbFullSubentPath p;
          if( pIter->getSubentity( i, p ) )
          {
            OdGsMarkerArray gsMarkers;
            pEnt->getGsMarkersAtSubentPath( p, gsMarkers );
            if( !gsMarkers.isEmpty() )
            {
              for (OdGsMarkerArray::iterator sm = gsMarkers.begin(); sm != gsMarkers.end(); ++sm)
              {
                OdExCollideGsPath* gsPath = new OdExCollideGsPath;
                gsPath->set( p, *sm );
                m_pathes.push_back( gsPath );
                OdDbEntityPtr pSubEnt = pEnt->subentPtr(p);
                pSubEnt->dragStatus( OdDb::kDragStart );
              }
            }
            else
            {
              OdExCollideGsPath* gsPath = new OdExCollideGsPath( p );
              m_pathes.push_back( gsPath );
            }
          }
        }
      }

      pIter->next();
    }
  
    for( OdUInt32 i = 0; i < m_pathes.size(); ++i )
    {
      m_pModel->highlight( (m_pathes[i]->operator const OdGiPathNode &()), false );
      inputArray.push_back( &(m_pathes[i]->operator const OdGiPathNode &()) );
    }
  }

  virtual ~CollideMoveTracker()
  {
    if( !m_prevHLPathes.empty() )
    {
      for( OdUInt32 i = 0; i < m_prevHLPathes.size(); ++i )
      {
        m_pModel->highlight( m_prevHLPathes[i]->operator const OdGiPathNode &(), false );
        delete m_prevHLPathes[i];
      }
      m_prevHLPathes.clear();
    }
    inputArray.clear();
    for( OdUInt32 i = 0; i < m_pathes.size(); ++i )
    {
      delete m_pathes[i];
    }
    m_pathes.clear();
    m_pView->invalidate();
    m_pView->update();
  }

  virtual void setValue(const OdGePoint3d& value)
  {
    OdGeMatrix3d matNewTransform = getTransform(value);
    // Compensate previous transform
    OdGeMatrix3d xTrans = m_xForm.inverse();
    xTrans.preMultBy(matNewTransform);
    // Remember last transform
    m_xForm = matNewTransform;
    for ( int i = m_ents.size() - 1; i >= 0; --i)
    {
      m_ents[i]->transformBy(xTrans);
    }
    doCollideWithAll();
  }

  virtual void doCollideWithAll();

  virtual void highlight( OdArray< OdExCollideGsPath* >& newPathes );

  virtual int addDrawables(OdGsView* pView)
  {
    for ( int i = m_ents.size() - 1; i >= 0; --i)
    {
      pView->add(m_ents[i], 0);
    }
    return 1;
  }

  virtual void removeDrawables(OdGsView* pView)
  {
    for ( int i = m_ents.size() - 1; i >= 0; --i)
    {
      pView->erase(m_ents[i]);
    }
  }
};


bool addNodeToPath( OdExCollideGsPath* result, const OdGiPathNode* pPath, bool bTruncateToRef = false )
{
  bool bAdd = true;
  if( pPath->parent() )
  {
    bAdd = addNodeToPath( result, pPath->parent(), bTruncateToRef );
  }
  if( bAdd )
  {
    result->addNode( pPath->persistentDrawableId() ? pPath->persistentDrawableId() : pPath->transientDrawable()->id(), (bTruncateToRef) ? 0 : pPath->selectionMarker() );
    if( bTruncateToRef && pPath->persistentDrawableId() )
    {
      OdDbObjectId id(pPath->persistentDrawableId());
      OdDbObjectPtr pObj = id.safeOpenObject();
      if( !pObj.isNull() )
      {
        if( pObj->isKindOf( OdDbBlockReference::desc() ) )
        {
          bAdd = false;
        }
      }
    }
  }
  return bAdd;
}

OdExCollideGsPath* fromGiPath( const OdGiPathNode* pPath, bool bTruncateToRef = false )
{
  if( !pPath ) return NULL;
  OdExCollideGsPath* res = new OdExCollideGsPath;
  addNodeToPath( res, pPath, bTruncateToRef );
  return res;
}

void CollideMoveTracker::doCollideWithAll()
{
  class OdExCollisionDetectionReactor : public OdGsCollisionDetectionReactor
  {
    OdArray< OdExCollideGsPath* > m_pathes;
    bool m_bDynHLT;
  public:
    OdExCollisionDetectionReactor( bool bDynHLT ) : m_bDynHLT( bDynHLT )
    {
    };
    ~OdExCollisionDetectionReactor()
    {
    }
    virtual OdUInt32 collisionDetected(const OdGiPathNode* /*pPathNode1*/, const OdGiPathNode* pPathNode2 )
    {
      OdExCollideGsPath* p = fromGiPath( pPathNode2, !m_bDynHLT );
      if( p || pPathNode2->persistentDrawableId() )
      {
        m_pathes.push_back(p);
      }
      return OdUInt32(OdGsCollisionDetectionReactor::kContinue);
    }

    OdArray< OdExCollideGsPath* >& pathes() { return m_pathes; }
  };

  OdExCollisionDetectionReactor reactor( m_bDynHLT );

  m_pView->collide( inputArray.asArrayPtr(), inputArray.size(), &reactor, NULL, 0 );
  

  highlight( reactor.pathes() );
}

void CollideMoveTracker::highlight( OdArray< OdExCollideGsPath* >& newPathes )
{
  //1) Unhighlight old pathes
  if( !m_prevHLPathes.empty() )
    {
      for( OdUInt32 i = 0; i < m_prevHLPathes.size(); ++i )
      {
        m_pModel->highlight( m_prevHLPathes[i]->operator const OdGiPathNode &(), false );
        delete m_prevHLPathes[i];
      }
      m_prevHLPathes.clear();
    }
  //2) Highlight new pathes
  for( OdUInt32 i = 0; i < newPathes.size(); ++i )
  {
    m_pModel->highlight( newPathes[i]->operator const OdGiPathNode &(), true );
    m_prevHLPathes.push_back( newPathes[i] );
  }
}


void OdExCollideCmd::execute(OdEdCommandContext* pCmdCtx)
{
  class OdExTransactionSaver
  {
  private:
    OdDbDatabasePtr m_pDb;
    bool m_bInTransaction;
  public:
    OdExTransactionSaver( OdDbDatabasePtr pDb )
    {
      m_pDb = pDb;
      m_bInTransaction = false;
    }
    ~OdExTransactionSaver()
    {
      if( m_bInTransaction )
      {
        m_pDb->abortTransaction();
        m_bInTransaction = false;
      }
    }
    void startTransaction()
    {
      if( m_bInTransaction )
      {
        m_pDb->abortTransaction();
      }
      m_bInTransaction = true;
      m_pDb->startTransaction();
    }
  };

  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  OdRxVariantValue dynHlt = (OdRxVariantValue)pCmdCtx->arbitraryData( OD_T("DynamicSubEntHlt" ) );
  bool bDynHLT = (bool)(dynHlt);

  //Get active view
  OdGsView *pView = NULL;
  if(!pDb.isNull())
  {
    OdDbObjectPtr pVpObj = pDb->activeViewportId().safeOpenObject();
    OdDbAbstractViewportDataPtr pAVD(pVpObj);
    if (!pAVD.isNull() && pAVD->gsView(pVpObj))
      pView = pAVD->gsView(pVpObj);
  }
  if( !pView )
  {
    ODA_ASSERT( false );
    throw OdEdCancel();
  }

   OdDbSelectionSetPtr pSSet = pIO->select(L"Collide: Select objects to be checked:", 
                                          OdEd::kSelAllowObjects | 
                                          OdEd::kSelAllowSubents |
                                          OdEd::kSelLeaveHighlighted);

   if (!pSSet->numEntities()) throw OdEdCancel();

   OdExTransactionSaver saver( pDb );
   saver.startTransaction();

   OdGePoint3d ptBase   = pIO->getPoint(L"Collide: Specify base point:");

   CollideMoveTracker tracker( ptBase, pSSet, pDb, pView, bDynHLT );
   OdGePoint3d ptOffset = pIO->getPoint(L"Collide: Specify second point:", OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand,
                                         0, OdString::kEmpty, &tracker);
}


//Collision detection commands
const OdString OdExCollideAllCmd::groupName() const
{
  return globalName();
}

const OdString OdExCollideAllCmd::globalName() const
{
  return OD_T("COLLIDEALL");
}

void OdExCollideAllCmd::execute(OdEdCommandContext* pCmdCtx)
{
  class OdExCollisionDetectionReactor : public OdGsCollisionDetectionReactor
  {
    OdArray< OdExCollideGsPath* > m_pathes;
    bool m_bDynHLT;
  public:
    OdExCollisionDetectionReactor( bool bDynHLT ) : m_bDynHLT( bDynHLT )
    {
    };
    ~OdExCollisionDetectionReactor()
    {
    }
    virtual OdUInt32 collisionDetected(const OdGiPathNode* pPathNode1, const OdGiPathNode* pPathNode2 )
    {
      OdExCollideGsPath* p1 = fromGiPath( pPathNode1, !m_bDynHLT );
      OdExCollideGsPath* p2 = fromGiPath( pPathNode2, !m_bDynHLT );
      m_pathes.push_back( p1 );
      m_pathes.push_back( p2 );
      return OdUInt32(OdGsCollisionDetectionReactor::kContinue);
    }

    OdArray< OdExCollideGsPath* >& pathes() {return m_pathes; }
  };

  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdDbDatabasePtr pDb = pDbCmdCtx->database();

  //Get active view
  OdGsView *pView = NULL;
  if(!pDb.isNull())
  {
    OdDbObjectPtr pVpObj = pDb->activeViewportId().safeOpenObject();
    OdDbAbstractViewportDataPtr pAVD(pVpObj);
    if (!pAVD.isNull() && pAVD->gsView(pVpObj))
      pView = pAVD->gsView(pVpObj);
  }
  if( !pView )
  {
    ODA_ASSERT( false );
    throw OdEdCancel();
  }
  OdGsModel* pModel = pView->getModelList()[0];

  int nChoise = pIO->getInt( OD_T("Input 1 to detect only intersections, any other to detect all"), 0, 0 );

  OdGsCollisionDetectionContext cdCtx;
  cdCtx.setIntersectionOnly( nChoise == 1 );

  OdRxVariantValue dynHlt = (OdRxVariantValue)pCmdCtx->arbitraryData( OD_T("DynamicSubEntHlt" ) );
  bool bDynHLT = (bool)(dynHlt); 

  OdExCollisionDetectionReactor reactor( dynHlt );

  pView->collide( NULL, 0, &reactor, NULL, 0, &cdCtx );

  OdArray< OdExCollideGsPath* >& pathes = reactor.pathes();
  for( OdUInt32 i = 0; i < pathes.size(); ++i )
  {
    const OdGiPathNode* p = &(pathes[i]->operator const OdGiPathNode &());
    pModel->highlight( *p );
    //delete pathes[i];
  }
  pIO->getInt( OD_T("Specify any number to exit"), 0, 0 );
  for( OdUInt32 i = 0; i < pathes.size(); ++i )
  {
    const OdGiPathNode* p = &(pathes[i]->operator const OdGiPathNode &());
    pModel->highlight( *p, false );
    delete pathes[i];
  }
  pathes.clear();
}


void OdExEditorObject::setTracker(OdEdInputTracker* pTracker) {
  if(m_pTracker.get())
    m_pTracker->removeDrawables(activeTopView());

  m_pTracker = pTracker;

  m_pBasePt = 0;
  if(pTracker) {
    SETBIT(m_flags, kTrackerHasDrawables, pTracker->addDrawables(activeTopView())!=0);
    OdEdPointDefTrackerPtr pPointDefTracker = OdEdPointDefTracker::cast(pTracker);
    if(pPointDefTracker.get()) {
      m_basePt = pPointDefTracker->basePoint();
      m_pBasePt = &m_basePt;
    }
  }
  else {
    SETBIT(m_flags, kTrackerHasDrawables, false);
  }
}

bool OdExEditorObject::trackString(const OdString& value) {
  if(m_pTracker.get()) {
    ODA_ASSERT(m_pTracker->isKindOf(OdEdStringTracker::desc()));
    ((OdEdStringTracker*)m_pTracker.get())->setValue(value);
    return GETBIT(m_flags, kTrackerHasDrawables);
  }
  return false;
}

bool OdExEditorObject::trackPoint(const OdGePoint3d& pt) {
  if(m_pTracker.get()) {
    ODA_ASSERT(m_pTracker->isKindOf(OdEdPointTracker::desc()));
    ((OdEdPointTracker*)m_pTracker.get())->setValue(pt);
    return GETBIT(m_flags, kTrackerHasDrawables);
  }
  return false;
}

bool OdExEditorObject::hasDatabase() const {
  return m_pCmdCtx->baseDatabase()!=0;
}
