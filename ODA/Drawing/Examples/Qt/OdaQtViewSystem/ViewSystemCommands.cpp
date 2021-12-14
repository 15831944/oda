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
// ViewSystemCommands.cpp
//

#include <QMainWindow>
#include <QBitmap>
#include <QMdiArea>
#include <QStyle>

#include "OdqViewSystem.h"
#include "OdqView.h"

#include "ViewSystemModule.h"
#include "ExtDbModule.h"
#include "Ed/EdCommandContext.h" // #include "DbCommandContext.h"
#include "DbHostAppServices.h"
#include "DbAbstractViewportData.h"
#include "DbViewport.h"
#include "DbViewportTable.h"
#include "DbBlockTableRecord.h"
#include "OdDToStr.h" // for odStrToD
#include "Gs/GsModel.h"
#include "Gi/GiDrawableImpl.h"
#include "Gi/GiViewport.h"
#include "Gi/GiViewportDraw.h"
#include "DbViewTable.h"
#include "DbViewTableRecord.h"
#include "DbLayerState.h"
#include "DbGsManager.h"
#include "Gs/GsBaseVectorizer.h"
#include "ColorMapping.h"
#include "DbVisualStyle.h"
#include "DbDictionary.h"
#include "RxVariantValue.h"

//////////////////////////////////////////////////////////////////////////

void OdqCmd_QVLayout::execute(OdEdCommandContext* pCmdCtx)
{
  if (!pCmdCtx->baseDatabase())
    throw OdError(eNoDatabase); // "No Database"

  OdString sLayoutName = pCmdCtx->userIO()->getString(OD_T("Enter layout name"));
  if (sLayoutName.isEmpty())
    return;

  OdRxObject* pRxDb = pCmdCtx->baseDatabase();
  OdDbBaseDatabasePE* pDbPE = OdDbBaseDatabasePE::cast(pRxDb);
  OdExtDbModule* pExtDb = NULL;
  if (!pDbPE && (pExtDb = getIApp()->getExtDbModule(pRxDb)) != NULL)
    pDbPE = OdDbBaseDatabasePE::cast(pRxDb = pExtDb->getWrappedDatabase(pRxDb));
  ODA_ASSERT_ONCE(pDbPE);

  try
  {
    pDbPE->setCurrentLayout(pCmdCtx->baseDatabase(), sLayoutName);
  }
  catch (...)
  {
    OdString sMsg = getIApp()->toOdString(
      QObject::tr("Unknown layout name \"%1\"").arg(getIApp()->toQString(sLayoutName)));
    getIApp()->getBaseHostAppServices()->warning(sMsg);
    return;
  }
  getIApp()->refreshQObjetcts("<event-refresh:dwg_layout_changed>"); // , SLOT(refresh(const QString&)));

  IqViewSystem* iVS = getIViewSystem();
  if (!iVS)
    return;

  IqView* iView = iVS->getView(pCmdCtx->baseDatabase());
  if (!iView)
  {
    ODA_FAIL_ONCE();
    return;
  }

  iView->updateLayouts();
}

void OdqCmd_Model::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(pCmdCtx->baseDatabase()).get();
  if (!pDwgDb)
    return; // useless for non DWG drawings 
  QString qsCmd = "qvlayout Model";
  getIConsole()->executeCommand(qsCmd, false, pCmdCtx->baseDatabase());
}

void OdqCmd_Layout::execute(OdEdCommandContext* pCmdCtx)
{
  OdqViewSystemModule* pModule = static_cast<OdqViewSystemModule*>(
     // TODO const_cast<OdRxModule*>(module()));
     ::odrxDynamicLinker()->loadModule(OD_T("OdaQtViewSystem.dll")).get());
  ODA_ASSERT_ONCE(pModule);
  pModule->m_cmd_QVLayout.execute(pCmdCtx);
}

///////////////////////////////////////////////////////////////////////////////

static bool getLayoutExtents(const OdDbObjectId& spaceId, const OdGsView* pGsView, OdGeBoundBlock3d& bbox)
{
  OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject();
  OdDbLayoutPtr pLayout = pSpace->getLayoutId().safeOpenObject();
  OdGeExtents3d ext;
  if (pLayout->getGeomExtents(ext)==eOk)
  {
    ext.transformBy(pGsView->viewingMatrix());
    bbox.set(ext.minPoint(), ext.maxPoint());
    return (ext.minPoint() != ext.maxPoint());
  }
  return false;
}

static void zoom_extents(OdGsView* pGsView, OdRxObject* pRxDatabase)
{
  //OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pRxDatabase);
  //if (!pDwgDb.isNull())
  //{
    OdAbstractViewPEPtr pViewPE = OdAbstractViewPE::cast(pGsView);
    if (pViewPE.isNull() || !pViewPE->zoomExtents(pGsView))
    {
      ODA_ASSERT_VAR(OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDatabase);)
      ODA_ASSERT_ONCE(   (pExtDb && pExtDb->baseModuleName(true) == L"bim") // possible via empty layouts only
                      || (pExtDb && pExtDb->baseModuleName(true) == L"ifc"));
    }
    return;
  //}

  //OdDbObjectPtr pVpObj = pDwgDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
  //if (pVpObj.isNull())
  //{
  //  ODA_FAIL_ONCE();
  //  return;
  //}
  //
  //OdDbDatabase* pDb = pVpObj->database();
  //OdAbstractViewPEPtr pVpPE(pGsView);
  //OdGeBoundBlock3d bbox;
  //bool bBboxValid = pVpPE->viewExtents(pGsView, bbox);
  //
  //// paper space overall view
  //OdDbViewportPtr pVp = OdDbViewport::cast(pVpObj);
  //if (pVp.get() && pVp->number() == 1)
  //{
  //  if (!bBboxValid || !(bbox.minPoint().x < bbox.maxPoint().x && bbox.minPoint().y < bbox.maxPoint().y))
  //    bBboxValid = getLayoutExtents(pDb->getPaperSpaceId(), pGsView, bbox);
  //}
  //else if (!bBboxValid) // model space viewport
  //  bBboxValid = getLayoutExtents(pDb->getPaperSpaceId(), pGsView, bbox);
  //
  //if (!bBboxValid)
  //{
  //  // set to somewhat reasonable (e.g. paper size)
  //  if (pDb->getMEASUREMENT()==OdDb::kMetric)
  //    bbox.set(OdGePoint3d::kOrigin, OdGePoint3d(297., 210., 0.)); // set to papersize ISO A4 (portrait)
  //  else
  //    bbox.set(OdGePoint3d::kOrigin, OdGePoint3d(11., 8.5, 0.)); // ANSI A (8.50 x 11.00) (landscape)
  //  bbox.transformBy(pGsView->viewingMatrix());
  //}
  //
  //pVpPE->zoomExtents(pGsView, &bbox);
}

void zoom_window(OdGePoint3d& pt1, OdGePoint3d& pt2, OdGsView* pGsView)
{
  OdGeMatrix3d xWorldToEye = OdAbstractViewPEPtr(pGsView)->worldToEye(pGsView);
  pt1.transformBy(xWorldToEye);
  pt2.transformBy(xWorldToEye);
  OdGeVector3d eyeVec = pt2 - pt1;
  if (OdZero(eyeVec.x) || OdZero(eyeVec.y))
    return;
  
  OdGePoint3d newPos = pt1 + eyeVec / 2.;

  eyeVec.x = fabs(eyeVec.x);
  eyeVec.y = fabs(eyeVec.y);

  pGsView->dolly(newPos.asVector());

  double wf = pGsView->fieldWidth()  / eyeVec.x;
  double hf = pGsView->fieldHeight() / eyeVec.y;

  pGsView->zoom(odmin(wf, hf));
}

static void dolly(OdGsView* pGsView, double x, double y) 
{
  OdGeVector3d vec(-x, -y, 0.0);
  vec.transformBy((pGsView->screenMatrix() * pGsView->projectionMatrix()).inverse());
  pGsView->dolly(vec);
}

static void zoom_scale_at_position(OdGsView* pGsView, double dScale, 
                                   const OdGePoint3d& cptBase) //OdDbObject* pVpObj)
{
  if (fabs(dScale) < OdGeContext::gTol.equalVector())
  {
    ODA_FAIL_ONCE(); // test
    return;
  }

  OdGePoint3d ptBase(cptBase);
  OdGePoint3d ptPos(pGsView->position());
  ptBase.transformBy(pGsView->worldToDeviceMatrix());
  ptPos.transformBy(pGsView->worldToDeviceMatrix());

  double vx = ptBase.x - ptPos.x,
         vy = ptBase.y - ptPos.y;

  dolly(pGsView, -vx, -vy);
  pGsView->zoom(dScale);
  dolly(pGsView, vx, vy);

  // TODO
  //if (!m_p2dModel.isNull())
  //  m_p2dModel->invalidate(activeTopView());
}

class RTZoomTracker : public OdEdPointTracker
{
  OdGsView* m_pGsView;
  double m_base;
  double m_fw;
  double m_fh;
public:
  RTZoomTracker()
    : m_pGsView(NULL)
    , m_base(0)
    , m_fw(0)
    , m_fh(0)
  {
  }

  void init(OdGsView* pGsView, const OdGePoint3d& base)
  {
    m_pGsView = pGsView;
    m_fw = pGsView->fieldWidth();
    m_fh = pGsView->fieldHeight();
    m_base = (m_pGsView->projectionMatrix() * m_pGsView->viewingMatrix() * base).y;
  }

  virtual void setValue(const OdGePoint3d& value)
  {
    OdGeMatrix3d xWorldToNDC = m_pGsView->projectionMatrix() * m_pGsView->viewingMatrix();
    OdGePoint3d pt2 = xWorldToNDC * value;
    double fac = 1. + fabs(pt2.y - m_base) * 1.5;
    if (pt2.y > m_base)
      fac = 1./fac;
    m_pGsView->setView(m_pGsView->position(),
                       m_pGsView->target(),
                       m_pGsView->upVector(),
                       m_fw * fac,
                       m_fh * fac,
                       m_pGsView->isPerspective() ? OdGsView::kPerspective : OdGsView::kParallel);
  }

  virtual int addDrawables(OdGsView*) // pGsView
  { 
    return 1; 
  }
  virtual void removeDrawables(OdGsView*) // pGsView
  {
  }
};

class SaveCursor
{
  OdRxObject* m_pRxDatabase;
  QCursor m_cursorSave;

public:
  SaveCursor(OdRxObject* pRxDatabase, const QCursor& cursor)
    : m_pRxDatabase(pRxDatabase)
  {
    //if (!m_pRxDatabase)
    //  throw OdError(eNoDatabase);
    IqView* iView = getView();
    if (!iView)
    {
      ODA_FAIL_ONCE();
      throw OdError(eInvalidView);
    }

    m_cursorSave = iView->cursor();
    iView->setCursor(cursor);
  }

  virtual ~SaveCursor()
  {
    IqView* iView = getView();
    if (!iView)
      return;

    iView->setCursor(m_cursorSave); // restore
  }

  IqView* getView()
  {
    IqViewSystem* iVS = getIViewSystem();
    if (!iVS)
      return NULL;
    return iVS->getView(m_pRxDatabase);
  }

  //virtual int addDrawables(OdGsView*)
  //{
  //  return 0;
  //}
  //virtual void removeDrawables(OdGsView*)
  //{
  //}
};

///////////////////////////////////////////////////////////////////////////////

void OdqCmd_Zoom::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbBaseDatabase* pRxDatabase = pCmdCtx->baseDatabase();
  if (!pRxDatabase)
    throw OdError(eNoDatabase);

  IqViewSystem* iVS = getIViewSystem();
  if (!iVS)
    throw OdError(eInvalidView);

  IqView* iView = iVS->getView(pRxDatabase);
  if (!iView)
    throw OdError(eInvalidView);

  //OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDatabase);
  //ODA_ASSERT_ONCE(!pDbPE.isNull());

  // TODO ?
  //OdGsDCRect box;
  //pDbPE->applyLayoutSettings(box, iView->getGsDevice(), pRxDatabase);

  //OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  //OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbBaseDatabaseIOPtr pIO = OdDbBaseDatabaseIO::cast(pCmdCtx->userIO()); // via OdDbUserIO now
  ODA_ASSERT_ONCE(!pIO.isNull());

  OdGsView* pGsView = iView->getActiveTopGsView();
  SaveCursor svCrossCursor(pRxDatabase, QCursor(Qt::CrossCursor));
  
  //bool bOnce = getIConsole()->isInputStringReady(pCmdCtx->baseDatabase());
  OdGePoint3d pt1, pt2;
  OdString sOtherInput;
  try
  {
    FIX_LINUX_EXCEPTION_BEGIN

    const OdChar* szKeywords = OD_T("All Center Dynamic Extents Previous Scale Window Object");

    pt1 = pIO->getPoint(
              //__OD_T("Specify corner of window, enter a scale factor (nX or nXP), or\n")
                __OD_T("Specify corner of window, enter a scale factor (nX), or\n")
                __OD_T("[All/Center/Dynamic/Extents/Previous/Scale/Window/Object]<real time>"),
                OdEd::kInpThrowEmpty | OdEd::kInpThrowOther | OdEd::kGptNoOSnap | OdEd::kGptNoUCS, 
                0, szKeywords);
    pt2 = pIO->getPoint(OD_T("Specify opposite corner:"), 
                        OdEd::kGptNoUCS | OdEd::kGptRectFrame | OdEd::kGptNoOSnap);
    zoom_window(pt1, pt2, pGsView);

    FIX_LINUX_EXCEPTION_END
  }
  catch (const OdEdEmptyInput&) // real time
  {
    OdStaticRxObject<RTZoomTracker> tracker;
    for (;;)
    {
      try
      {
        FIX_LINUX_EXCEPTION_BEGIN

        tracker.init(pGsView, 
                     pIO->getPoint(OD_T("Press ESC or ENTER to exit"),
                       OdEd::kInpThrowEmpty | OdEd::kGptNoUCS 
                     | OdEd::kGptBeginDrag | OdEd::kGptNoOSnap));
        pIO->getPoint(OD_T("Press ESC or ENTER to exit"),
                        OdEd::kInpThrowEmpty | OdEd::kGptNoUCS 
                      | OdEd::kGptEndDrag |OdEd::kGptNoOSnap, 0, OdString::kEmpty, &tracker);

        FIX_LINUX_EXCEPTION_END
      }
      catch (const OdEdCancel&)
      {
        if (!pCmdCtx->baseDatabase()) // (now used as marker to already closed database)
          return;
        break;
      }
    }
  }
  catch (const OdEdOtherInput& otherInput) // nX or nXP
  {
    sOtherInput = otherInput.string();
  }
  catch (const OdEdKeyword& kw)
  {
    switch (kw.keywordIndex())
    {
    case 0: // All
      throw OdError(eNotImplemented); // TODO
    case 1: // Center
      throw OdError(eNotImplemented); // TODO
    case 2: // Dynamic
      throw OdError(eNotImplemented); // TODO
    case 3: // Extents
      zoom_extents(pGsView, pRxDatabase);
      // pDbPE->zoomToExtents(const OdGsDCRect& outputRect, OdGsDevice* pDevice, OdDbBaseDatabase* db) = 0;
      break;
    case 4: // Previous
      throw OdError(eNotImplemented); // TODO
    case 5: // Scale
      sOtherInput = pIO->getString(OD_T("Enter a scale factor (nX)"));
      break;
    case 6: // Window
      pt1 = pIO->getPoint(OD_T("Specify first corner:"),    OdEd::kGptNoUCS|OdEd::kGptNoOSnap);
      pt2 = pIO->getPoint(OD_T("Specify opposite corner:"), OdEd::kGptNoUCS|OdEd::kGptNoOSnap|OdEd::kGptRectFrame);
      zoom_window(pt1, pt2, pGsView);
      break;
    case 7: // Object
      throw OdError(eNotImplemented); // TODO
    }
  }

  if (!sOtherInput.isEmpty())
  {
    OdChar* pEnd = NULL;
    double dScale = odStrToD(sOtherInput, &pEnd);
    if (pEnd <= sOtherInput.c_str())
    {
      pIO->putString(OD_T("Requires a distance, numberX, or option keyword."));
      return;
    }

    OdString sEnd(pEnd);
    if (!*pEnd || !sEnd.iCompare(OD_T("X")))
      pGsView->zoom(dScale);
    else if (!sEnd.iCompare(OD_T("XP")))
      throw OdError(eNotImplemented); // TODO //zoom_scaleXP(dScale);
    else if (!sEnd.iCompare(OD_T("AT")) || !sEnd.iCompare(OD_T("AP")))
    {
      OdGePoint3d pos = pIO->getPoint(OD_T("Specify base point to scale"), 
                                      OdEd::kGptNoUCS | OdEd::kGptNoOSnap);
      zoom_scale_at_position(pGsView, dScale, pos);
    }
  }

  OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRxDatabase).get();
  IqRevSystem* iRS = getIRevSystem();
  if (iRS && iRS->getIRevDiff(iView))
    pDwgDb = NULL;
  OdExtDbModule* pExtDb = !pDwgDb ? getIApp()->getExtDbModule(pRxDatabase) : NULL;
  if (pExtDb)
    pExtDb->setGsViewToDb(pRxDatabase, pGsView);
  else
  {
    // store in database
    ODA_ASSERT_ONCE(pDwgDb);
    // see also OdqView::getActiveTopGsView()
    OdDbObjectPtr pVpObj = pDwgDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
    OdDbAbstractViewportDataPtr pAVD = OdDbAbstractViewportDataPtr(pVpObj);
    ODA_ASSERT_ONCE(!pAVD.isNull());
    //OdAbstractViewPEPtr pAView = pGsView;
    //pAView->setView(OdRxObject* pDestinationView, const OdRxObject* pSourceView) const;

    pAVD->setView(pVpObj, pGsView);
  }

  ODA_ASSERT_ONCE(iVS);
  iVS->updateViews(pRxDatabase);
}

///////////////////////////////////////////////////////////////////////////////

// see also OdExDollyCmd of OdaMfcApp

class RTDollyTracker : public OdEdPointTracker
{
  OdGsView* m_pGsView;
  OdGePoint3d m_pt;
  OdGePoint3d m_pos;

public:
  RTDollyTracker()
    : m_pGsView(NULL)
  {
  }
  
  void reset()
  {
    m_pGsView = NULL; 
  }
  
  void init(OdGsView* pGsView, const OdGePoint3d& pt)
  {
    ODA_ASSERT_ONCE(pGsView);

    m_pGsView = pGsView;
    m_pos = m_pGsView->position();
    m_pt = pt - m_pos.asVector();
  }

  virtual void setValue(const OdGePoint3d& value)
  {
    if (m_pGsView)
    {
      OdGeVector3d delta = (m_pt - (value - m_pos)).asVector();
      m_pt = value - m_pos.asVector();
      delta.transformBy(m_pGsView->viewingMatrix());
      m_pGsView->dolly(delta.x, delta.y, delta.z);
      m_pos = m_pGsView->position();
    }
  }

  virtual int addDrawables(OdGsView*) // pGsView
  {
    return 0;
  }
  virtual void removeDrawables(OdGsView*) // pGsView
  {
  }
};

///////////////////////////////////////////////////////////////////////////////

void OdqCmd_Pan::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbBaseDatabase* pRxDatabase = pCmdCtx->baseDatabase();
  if (!pRxDatabase)
    throw OdError(eNoDatabase);

  IqViewSystem* iVS = getIViewSystem();
  if (!iVS)
    throw OdError(eInvalidView);

  IqView* iView = iVS->getView(pRxDatabase);
  if (!iView)
    throw OdError(eInvalidView);

  //OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDatabase);
  //ODA_ASSERT_ONCE(!pDbPE.isNull());

  //OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  //OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbBaseDatabaseIOPtr pIO = OdDbBaseDatabaseIO::cast(pCmdCtx->userIO()); // via OdDbUserIO now
  ODA_ASSERT_ONCE(!pIO.isNull());

  OdGsView* pGsView = iView->getActiveTopGsView();
  SaveCursor svOpenHandCursor(pRxDatabase, QCursor(Qt::OpenHandCursor));

  bool bOnce = getIConsole()->isInputStringReady(pCmdCtx->baseDatabase());
  OdStaticRxObject<RTDollyTracker> tracker;
  do 
  {
    try
    {
      FIX_LINUX_EXCEPTION_BEGIN

      tracker.init(pGsView, 
                   pIO->getPoint(OD_T("Press ESC or ENTER to exit"),
                                 OdEd::kInpThrowEmpty |OdEd::kGptNoUCS | OdEd::kGptNoOSnap | OdEd::kGptBeginDrag,
                                 0, OdString::kEmpty, &tracker));

      SaveCursor svClosedHandCursor(pRxDatabase, QCursor(Qt::ClosedHandCursor));
      pIO->getPoint(OD_T("Press ESC or ENTER to exit"),
                    OdEd::kInpThrowEmpty | OdEd::kGptNoUCS | OdEd::kGptNoOSnap | OdEd::kGptEndDrag,
                    0, OdString::kEmpty, &tracker);
      tracker.reset();

      FIX_LINUX_EXCEPTION_END
    }
    catch (const OdEdCancel&)
    {
      if (!pCmdCtx->baseDatabase()) // (now used as marker to already closed database) // #8027 
        return;
      break;
    }
  }
  while (!bOnce);

  OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRxDatabase).get();
  IqRevSystem* iRS = getIRevSystem();
  if (iRS && iRS->getIRevDiff(iView))
    pDwgDb = NULL;
  OdExtDbModule* pExtDb = !pDwgDb ? getIApp()->getExtDbModule(pRxDatabase) : NULL;
  if (pExtDb)
    pExtDb->setGsViewToDb(pRxDatabase, pGsView);
  else if (pDwgDb)
  {
    // store in database
    // see also OdqView::getActiveTopGsView()
    OdDbObjectPtr pVpObj = pDwgDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
    OdDbAbstractViewportDataPtr pAVD = OdDbAbstractViewportDataPtr(pVpObj);
    ODA_ASSERT_ONCE(!pAVD.isNull());
    //OdAbstractViewPEPtr pAView = pGsView;

    pAVD->setView(pVpObj, pGsView);
  }

  ODA_ASSERT_ONCE(iVS);
  iVS->updateViews(pRxDatabase);
}

///////////////////////////////////////////////////////////////////////////////

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

    OdGePoint3d pt1;
    OdGePoint2d pt2;
    vp.getViewportDcCorners((OdGePoint2d&)pt1, pt2);
    pt2.x -= pt1.x;
    pt2.y -= pt1.y;
    double r = odmin(pt2.x, pt2.y) / 9. * 7. / 2.;
    ((OdGePoint2d&)pt1) += (pt2.asVector() / 2.);

    // fix invalid plane clipping for some DGN drawing (like samp4.dgn)
    bool clipFront = false, 
         clipBack = false;
    double front, 
           back;
    if (   vp.getFrontAndBackClipValues(clipFront, clipBack, front, back)
        && clipFront && clipBack)
    {
      OdGePoint3d ptBase = vp.getCameraLocation();
      ptBase += vp.viewDir().normalize() * ((front + back) / 2.0);
      OdGePlane plane(ptBase, vp.viewDir());

      OdGePoint3d pt = vp.getModelToEyeTransform() * pt1;
      pt = vp.getEyeToWorldTransform() * pt;
      if (!plane.isOn(pt))
      {
        OdGePoint3d ptProj;
        if (plane.project(pt, ptProj))
        {
          pt1 = ptProj;

          pt1.transformBy(vp.getWorldToEyeTransform());
          pt1.transformBy(vp.getEyeToModelTransform());
        }
      }
    }

    geom.circle(pt1, r, OdGeVector3d::kZAxis);
    geom.circle(pt1 + OdGeVector3d(0., r, 0.), r / 20., OdGeVector3d::kZAxis);
    geom.circle(pt1 + OdGeVector3d(0.,-r, 0.), r / 20., OdGeVector3d::kZAxis);
    geom.circle(pt1 + OdGeVector3d( r, 0.,0.), r / 20., OdGeVector3d::kZAxis);
    geom.circle(pt1 + OdGeVector3d(-r, 0.,0.), r / 20., OdGeVector3d::kZAxis);
  }
};

typedef enum {
  enNavMode_3dConstrainedOrbit
, enNavMode_3dFreeOrbit
, enNavMode_3dContinuousOrbit
} TNavigationMode;
static TNavigationMode s_modeOrbit = enNavMode_3dFreeOrbit;

class RTOrbitTracker : public OdEdPointTracker
{
  TNavigationMode m_mode;
  OdGsView*       m_pGsView;
  OdGePoint3d     m_pt;
  OdGiDrawablePtr m_pDrw;
  OdGePoint3d     m_pos;
  OdGePoint3d     m_trg;
  OdGeVector3d    m_up;
  OdGeVector3d    m_x;
  OdGePoint3d     m_viewCenter;
  OdGeMatrix3d    m_initViewingMatrixInv;
  OdGeMatrix3d    m_xStep;
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
    OdGePoint3d target = m_pGsView->viewingMatrix() * m_pGsView->target();
    double halfFieldWidth = m_pGsView->fieldWidth() / 2.0;
    double halfFieldHeight = m_pGsView->fieldHeight() / 2.0;
    lower_left.x = target.x - halfFieldWidth;
    lower_left.y = target.y - halfFieldHeight;
    upper_right.x = target.x + halfFieldWidth;
    upper_right.y = target.y + halfFieldHeight;
  }

protected:
  RTOrbitTracker()
    : m_pGsView(0)
    , m_D(0)
    , m_mode(s_modeOrbit)
  {
    s_modeOrbit = enNavMode_3dFreeOrbit;
  }
public:

  TNavigationMode mode() const
  {
    return m_mode;
  }

  void reset()
  { 
    m_pGsView = NULL; 
    m_xStep.setToIdentity();
  }
  void init(OdGsView* pGsView, const OdGePoint3d& pt)
  {
    m_pGsView = pGsView;
    m_pos = pGsView->position();
    m_trg = pGsView->target();
    m_up = pGsView->upVector();
    m_x = m_up.crossProduct(pGsView->target() - m_pos).normal();

    m_initViewingMatrixInv = m_pGsView->viewingMatrix();
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
    if ((pt1 - m_pt).lengthSqrd() <= r2sqrd)
      m_axis = kHorizontal;
    else
    {
      pt1.y -= r;
      pt1.y -= r;
      if ((pt1 - m_pt).lengthSqrd() <= r2sqrd)
        m_axis = kHorizontal;
      else
      {
        pt1.y += r;
        pt1.x += r;
        if ((pt1 - m_pt).lengthSqrd() <= r2sqrd)
          m_axis = kVertical;
        else
        {
          pt1.x -= r;
          pt1.x -= r;
          if ((pt1 - m_pt).lengthSqrd() <= r2sqrd)
            m_axis = kVertical;
          else
          {
            pt1.x += r;
            if ((pt1 - m_pt).lengthSqrd() <= r*r)
              m_axis = kPerpDir;
            else
              m_axis = kEye;
          }
        }
      }
    }

    OdAbstractViewPEPtr pAView = pGsView;
    OdGeBoundBlock3d extents;
    pAView->viewExtents(pGsView, extents);
    m_viewCenter = extents.center();
    m_viewCenter.transformBy(m_initViewingMatrixInv);
  }

  double angle(const OdGePoint3d& value) const
  {
    OdGePoint3d pt2 = m_pGsView->viewingMatrix() * value;
    double dist = 0.0;
    if (m_axis == kHorizontal)
      dist = pt2.y - m_pt.y;
    else if (m_axis == kVertical)
      dist = pt2.x - m_pt.x;
    return dist * OdaPI / m_D;
  }

  double angleZ(const OdGePoint3d& value) const
  {
    OdGePoint3d pt2 = m_pGsView->viewingMatrix() * value;
    OdGePoint3d targ = m_pGsView->viewingMatrix() * m_viewCenter;
    pt2.z = targ.z = m_pt.z;
    return (pt2 - targ).angleTo((m_pt - targ), OdGeVector3d::kZAxis);
  }

  double anglePerp(const OdGePoint3d& value) const
  {
    OdGePoint3d pt2 = m_pGsView->viewingMatrix() * value;
    pt2.z = 0.0;
    return pt2.distanceTo(m_pt) * OdaPI / m_D;
  }

  const OdGeMatrix3d& getLastStep() const
  {
    return m_xStep;
  }

  virtual void setValue(const OdGePoint3d& value)
  {
    if (m_pGsView)
    {
      switch(m_axis)
      {
      case kHorizontal:
        m_xStep.setToRotation(-angle(value), m_x, m_viewCenter);
        break;
      case kVertical:
        m_xStep.setToRotation(-angle(value), m_up, m_viewCenter);
        break;
      case kEye:
        m_xStep.setToRotation(-angleZ(value), m_trg - m_pos, m_viewCenter);
        break;
      case kPerpDir:
        {
          OdGePoint3d value1 = value;
          value1.transformBy(m_pGsView->viewingMatrix());
          value1.z = 0.0;
          OdGeVector2d dir = (value1 - m_pt).convert2d();
          OdGeVector2d perp = dir.perpVector();
          OdGeVector3d perp3d(perp.x, perp.y, 0.0);
          perp3d.normalizeGetLength();
          perp3d.transformBy(m_initViewingMatrixInv);
          m_xStep.setToRotation(-anglePerp(value), perp3d, m_viewCenter);
          break;
        }
      }
      OdGePoint3d newPos = m_xStep * m_pos, newTarget = m_xStep * m_trg;
      OdGeVector3d newPosDir = newPos - newTarget;
      newPosDir.normalizeGetLength();
      newPosDir *= m_pos.distanceTo(m_trg);
      newPos = newTarget + newPosDir;

      m_pGsView->setView(newPos,
                         newTarget,
                         m_xStep * m_up,
                         m_pGsView->fieldWidth(),
                         m_pGsView->fieldHeight(),
                         m_pGsView->isPerspective() ? OdGsView::kPerspective : OdGsView::kParallel);
    }
  }

  virtual int addDrawables(OdGsView* pGsView)
  {
    if (m_mode == enNavMode_3dFreeOrbit)
    {
      m_pDrw = OdRxObjectImpl<OrbitCtrl>::createObject();
      if (m_pModel.isNull())
      {
        m_pModel = pGsView->device()->createModel();
        if (!m_pModel.isNull())
          m_pModel->setRenderType(OdGsModel::kDirect);
      }
      pGsView->add(m_pDrw, m_pModel.get());
    }
    return 1;
  }

  virtual void removeDrawables(OdGsView* pGsView)
  {
    if (!m_pDrw.isNull())
      pGsView->erase(m_pDrw);
  }
};

class RTOrbitContinuousTracker : public OdEdIntegerTracker
{
  IqView* m_iView;
  OdGsView* m_pGsView;
  OdGeMatrix3d m_xStep;

protected:
  RTOrbitContinuousTracker()
    : m_iView(NULL)
    , m_pGsView(NULL)
  {
  }

public:
  void init(IqView* iView, const OdGeMatrix3d& xStep)
  {
    if (xStep.isEqualTo(OdGeMatrix3d::kIdentity))
      return;

    ODA_ASSERT_ONCE(iView);
    m_iView = iView;
    m_pGsView = NULL;
    m_xStep = xStep;
    addDrawables(m_iView->getActiveTopGsView());
  }

  virtual ~RTOrbitContinuousTracker()
  {
    reset();
  }

  void reset()
  {
    if (m_pGsView)
      removeDrawables(m_pGsView);
    m_pGsView = NULL; 
  }

  virtual void setValue(int value)
  {
    if (!m_pGsView)
      return;

    OdGePoint3d pos = m_pGsView->position(),
                trg = m_pGsView->target();
    OdGeVector3d vUp = m_pGsView->upVector();
    //OdGeVector3d m_x = vUp.crossProduct(m_pGsView->target() - pos).normal();

    OdGePoint3d newPos = m_xStep * pos,
                newTarget = m_xStep * trg;
    OdGeVector3d newPosDir = newPos - newTarget;
    newPosDir.normalizeGetLength();
    newPosDir *= pos.distanceTo(trg);
    newPos = newTarget + newPosDir;

    m_pGsView->setView(newPos,
                       newTarget,
                       m_xStep * vUp,
                       m_pGsView->fieldWidth(),
                       m_pGsView->fieldHeight(),
                       m_pGsView->isPerspective() ? OdGsView::kPerspective : OdGsView::kParallel);
  }

  virtual int addDrawables(OdGsView* pGsView)
  { 
    ODA_ASSERT_ONCE(pGsView && !m_pGsView);
    m_pGsView = pGsView;
    ((OdqView*) m_iView)->subscribeTimer(this, 100);
    return 1; 
  }

  virtual void removeDrawables(OdGsView* pGsView)
  {
    ODA_ASSERT_ONCE(m_pGsView == pGsView);
    ((OdqView*) m_iView)->unsubscribeTimer(this);
    m_pGsView = NULL;
  }
};

///////////////////////////////////////////////////////////////////////////////

void OdqCmd_3dConstrainedOrbit::execute(OdEdCommandContext* pCmdCtx)
{
  OdqViewSystemModule* pModule = static_cast<OdqViewSystemModule*>(
                                  // TODO const_cast<OdRxModule*>(module()));
                                  ::odrxDynamicLinker()->loadModule(OD_T("OdaQtViewSystem.dll")).get());
  ODA_ASSERT_ONCE(pModule);
  s_modeOrbit = enNavMode_3dConstrainedOrbit;
  pModule->m_cmd_3dFreeOrbit.execute(pCmdCtx);
}

void OdqCmd_3dFreeOrbit::execute(OdEdCommandContext* pCmdCtx)
{
  OdDbBaseDatabase* pRxDatabase = pCmdCtx->baseDatabase();
  if (!pRxDatabase)
    throw OdError(eNoDatabase);

  IqViewSystem* iVS = getIViewSystem();
  if (!iVS)
    throw OdError(eInvalidView);

  IqView* iView = iVS->getView(pRxDatabase);
  if (!iView)
    throw OdError(eInvalidView);

  //OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDatabase);
  //ODA_ASSERT_ONCE(!pDbPE.isNull());
  //OdRxObjectPtr pLayout = pDbPE->currentLayout(pRxDatabase);
  //OdDbBaseLayoutPEPtr pLayoutInfo(pLayout);
  //ODA_ASSERT_ONCE(   pLayoutInfo.get() 
  //                || (getIApp()->getExtDbModule(pRxDatabase) && getIApp()->getExtDbModule(pRxDatabase)->baseModuleName(true) == L"prc"));

  //OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  //OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbBaseDatabaseIOPtr pIO = OdDbBaseDatabaseIO::cast(pCmdCtx->userIO()); // via OdDbUserIO now
  ODA_ASSERT_ONCE(!pIO.isNull());

  bool bOnce = getIConsole()->isInputStringReady(pCmdCtx->baseDatabase());

  if (!static_cast<OdqView*>(iView)->isModelLayout()) // if (pLayoutInfo.isNull() || !pLayoutInfo->isModelLayout(pLayout))
  {
    if (bOnce)
      pIO->getPoint(OD_T("skiped"), // any non-empty
                    OdEd::kInpThrowEmpty | OdEd::kGptNoUCS | OdEd::kGptNoOSnap | OdEd::kGptBeginDrag);
    // "** 3DORBIT only valid in Model space **"
    OdString sMsg = odSystemServices()->formatMsg(sidRequiresModelSpace, 
                                                  OdString(globalName()).makeUpper().c_str());
    pIO->putString(sMsg);
    return;
  }

  OdGsView* pGsView = iView->getActiveTopGsView();
// on iOS specific mouse image is not needed, all events are triggired by gestures
#ifndef __IPHONE_OS_VERSION_MIN_REQUIRED
  QPixmap pixmap(QLatin1String(":/images/view_orb.png"));
  ODA_ASSERT_ONCE(!pixmap.isNull());
  pixmap.setMask(pixmap.createHeuristicMask());
  QCursor cursorUnpressed(pixmap);
  
  pixmap = pixmap.scaled(pixmap.size() *= 1.5);
  pixmap.setMask(pixmap.createHeuristicMask());

  QCursor cursorPressed(pixmap);
#else
  QCursor cursorUnpressed;
  QCursor cursorPressed;
#endif
  SaveCursor svOrbitUnpressed(pRxDatabase, cursorUnpressed);
  OdStaticRxObject<RTOrbitTracker> tracker;
  OdStaticRxObject<RTOrbitContinuousTracker> trackerContinuous;

  do 
  {
    try
    {
      FIX_LINUX_EXCEPTION_BEGIN

      OdEdPointTracker* pFirstTracker = &tracker;
      if (bOnce)
        pFirstTracker = NULL; // PROD-120 ODE: if it is executed by accelerator Ctrl+Shift+Mouse_Wheel_press
      OdGePoint3d pt = pIO->getPoint(
                              OD_T("Press ESC or ENTER to exit"),
                              OdEd::kInpThrowEmpty |OdEd::kGptNoUCS | OdEd::kGptNoOSnap | OdEd::kGptBeginDrag,
                              0, OdString::kEmpty, pFirstTracker);
      if (!bOnce && tracker.mode() == enNavMode_3dContinuousOrbit)
        trackerContinuous.reset();
      tracker.init(pGsView, pt);

      SaveCursor svOrbitPressed(pRxDatabase, cursorPressed);
      pIO->getPoint(OD_T("Press ESC or ENTER to exit"),
                    OdEd::kInpThrowEmpty | OdEd::kGptNoUCS | OdEd::kGptNoOSnap | OdEd::kGptEndDrag,
                    0, OdString::kEmpty, &tracker);

      FIX_LINUX_EXCEPTION_END
    }
    catch (const OdEdCancel&)
    {
      if (!pCmdCtx->baseDatabase()) // (now used as marker to already closed database)
        return;
      break;
    }

    if (!bOnce && tracker.mode() == enNavMode_3dContinuousOrbit)
      trackerContinuous.init(iView, tracker.getLastStep());

    tracker.reset();
  }
  while (!bOnce);

  ODA_ASSERT_ONCE(iVS);
  if (!iVS->getView(pRxDatabase))
    return; // possible via quit during this command

  if (iVS->getView(pRxDatabase)) // problem with close
  {
    OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRxDatabase);
    IqRevSystem* iRS = getIRevSystem();
    if (iRS && iRS->getIRevDiff(iView))
      pDwgDb = NULL;
    OdExtDbModule* pExtDb = !pDwgDb ? getIApp()->getExtDbModule(pRxDatabase) : NULL;
    if (pExtDb)
      pExtDb->setGsViewToDb(pRxDatabase, pGsView);
    else if (pDwgDb)
    {
      // store in database
      // see also OdqView::getActiveTopGsView()
      OdDbObjectPtr pVpObj = pDwgDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
      OdDbAbstractViewportDataPtr pAVD = OdDbAbstractViewportDataPtr(pVpObj);
      ODA_ASSERT_ONCE(!pAVD.isNull());
      //OdAbstractViewPEPtr pAView = pGsView;

      pAVD->setView(pVpObj, pGsView);
    }
  }

  iVS->updateViews(pRxDatabase);
}

void OdqCmd_3dContinuousOrbit::execute(OdEdCommandContext* pCmdCtx)
{
  OdqViewSystemModule* pModule = static_cast<OdqViewSystemModule*>(
                                  // TODO const_cast<OdRxModule*>(module()));
                                  ::odrxDynamicLinker()->loadModule(OD_T("OdaQtViewSystem.dll")).get());
  ODA_ASSERT_ONCE(pModule);
  s_modeOrbit = enNavMode_3dContinuousOrbit;
  pModule->m_cmd_3dFreeOrbit.execute(pCmdCtx);
}

///////////////////////////////////////////////////////////////////////////////

static void view_setDirection(OdEdCommandContext* pCmdCtx, 
                              const OdGeVector3d& vPosition, // from origin to position
                              const OdGeVector3d& axis)
{
  ODA_ASSERT_ONCE(!vPosition.isZeroLength() && OdZero(axis.length() - 1.0));
  IqViewSystem* iVS = getIViewSystem();
  if (!iVS)
    throw OdError(eInvalidView);

  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  IqView* iView = iVS->getView(pRxDatabase);
  if (!iView)
    throw OdError(eInvalidView);

  OdGsView* pGsView = iView->getActiveTopGsView();
  ODA_ASSERT_ONCE(pGsView);

  OdAbstractViewPEPtr pViewPE(pGsView);
  ODA_ASSERT_ONCE(!pViewPE.isNull());
  if (pViewPE.isNull())
    throw OdError(eInvalidView);

  pGsView->setView(OdGePoint3d::kOrigin + vPosition, 
                   OdGePoint3d::kOrigin, // target
                   axis,
                   pGsView->fieldWidth(),
                   pGsView->fieldHeight(),
                   pGsView->isPerspective() ? OdGsView::kPerspective : OdGsView::kParallel);
  pViewPE->zoomExtents(pGsView); // return false if drawing is empty

  OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRxDatabase).get();
  IqRevSystem* iRS = getIRevSystem();
  if (iRS && iRS->getIRevDiff(iView))
    pDwgDb = NULL;
  OdExtDbModule* pExtDb = !pDwgDb ? getIApp()->getExtDbModule(pRxDatabase) : NULL;
  if (pExtDb)
    //pExtDb->setUcs(pRxDatabase, pGsView, OdGePoint3d::kOrigin, axis.crossProduct(vPosition), axis);
    pExtDb->setGsViewToDb(pRxDatabase, pGsView);
  else
  {
    // store in database
    //OdGsClientViewInfo cliInfo;
    //pGsView->clientViewInfo(cliInfo);
    //ODA_ASSERT_ONCE(cliInfo.viewportObjectId);
    //OdDbObjectPtr pVpObj = OdDbObjectId(cliInfo.viewportObjectId).safeOpenObject(OdDb::kForWrite);
    ODA_ASSERT_ONCE(pDwgDb);
    OdDbObjectPtr pVpObj = pDwgDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
    OdDbAbstractViewportDataPtr pAVD = OdDbAbstractViewportDataPtr(pVpObj);
    ODA_ASSERT_ONCE(pAVD.get() && pAVD->gsView(pVpObj) == pGsView);
    //pAVD->setUcs(pVpObj, OdGePoint3d::kOrigin, axis.crossProduct(vPosition), axis);
    pAVD->setView(pVpObj, pGsView);
  }

  ODA_ASSERT_ONCE(iVS);
  iVS->updateViews(pCmdCtx->baseDatabase());
}

static void view_list(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
  if (pDwgDb.isNull())
    throw OdError(eNotImplemented); // TODO for DGN
  OdEdUserIO* pIO = pCmdCtx->userIO();

  OdString sPattern;
  try
  {
    FIX_LINUX_EXCEPTION_BEGIN
    sPattern = pIO->getString(OD_T("Enter view name(s) to list <*>"), // TODO via pmtEnterViewNameToList
                              OdEd::kGstAllowSpaces | OdEd::kGstNoEmpty, // OdEd::kInpDefault | OdEd::kInpThrowEmpty, 
                              OD_T("*"), // default
                              OD_T("*")); // keywords
    FIX_LINUX_EXCEPTION_END
  }
  catch (const OdEdKeyword& kw)
  {
    sPattern = kw.keyword();
  }

  ODA_ASSERT_ONCE(!sPattern.isEmpty());
  QRegExp rxPattern(getIApp()->toQString(sPattern), Qt::CaseInsensitive, QRegExp::Wildcard);

  bool bNoMatching = true;
  OdDbViewTablePtr pViewTable = pDwgDb->getViewTableId().safeOpenObject();
  OdDbSymbolTableIteratorPtr pItr = pViewTable->newIterator();
  pIO->putString(OD_T("Saved views:")); // TODO via msgSavedViews
  for (pItr->start(); !pItr->done(); pItr->step())
  {
    OdDbViewTableRecordPtr pViewRecord = pItr->getRecord();
    ODA_ASSERT_ONCE(!pViewRecord.isNull());
    QString qsName = getIApp()->toQString(pViewRecord->getName());
    if (!rxPattern.exactMatch(qsName))
      continue;

    if (bNoMatching)
      pIO->putString(OD_T("View name                        Space")); // msgViewTabColumns
    bNoMatching = false;

    OdString sMsg;
    sMsg.format(OD_T("%- 35ls%ls"), pViewRecord->getName().c_str(), 
                                    pViewRecord->isPaperspaceView() ? OD_T("P") : OD_T("M"));
    pIO->putString(sMsg);
  } // end for

  if (bNoMatching)
    pIO->putString(OD_T("No matching views found."));
}

OdDbViewTableRecordPtr view_selectView(OdEdCommandContext* pCmdCtx, const OdString& sPmt)
{
  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
  if (pDwgDb.isNull())
    throw OdError(eNotImplemented); // TODO for DGN
  OdEdUserIO* pIO = pCmdCtx->userIO();

  OdDbViewTableRecordPtr pNamedView;
  OdDbViewTablePtr pViewTable = pDwgDb->getViewTableId().safeOpenObject();
  while (true)
  {
    OdString sInput; 
    try
    {
      FIX_LINUX_EXCEPTION_BEGIN
      sInput = pIO->getString(sPmt, OdEd::kGstAllowSpaces, OD_T("?"), OD_T("?"));
      FIX_LINUX_EXCEPTION_END
    }
    catch (const OdEdKeyword& kw)
    {
      sInput = kw.keyword();
    }
    if (sInput == OD_T("?"))
      view_list(pCmdCtx);
    else
    {
      pNamedView = pViewTable->getAt(sInput).openObject(OdDb::kForWrite);
      if (pNamedView.get())
        break;

      OdString sMsg;
      sMsg.format(OD_T("Cannot find view \"%ls\"."), // msgCannotFindSpecifiedView
                  sInput.c_str());
      pIO->putString(sMsg);
    }
  }
  return pNamedView;
}

static void view_categorize(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
  if (pDwgDb.isNull())
    throw OdError(eNotImplemented); // TODO for DGN
  OdEdUserIO* pIO = pCmdCtx->userIO();

  OdDbViewTableRecordPtr pNamedView = view_selectView(pCmdCtx, OD_T("Enter view name to Categorize or [?]")); // TODO via pmtEnterViewNameToCategorize

  OdString sPrompt;
  sPrompt.format(OD_T("Enter category name or * for none, or [?]: <\"%ls\">"), // pmtEnterCategoryNameOrForNone
                 pNamedView->getCategoryName().c_str());
  OdString sCategory;
  try
  {
    FIX_LINUX_EXCEPTION_BEGIN
    OdString sKeywords = OD_T("? ");
    sKeywords += pNamedView->getCategoryName();
    sCategory = pIO->getString(sPrompt, OdEd::kGstAllowSpaces, pNamedView->getCategoryName(), sKeywords);
    FIX_LINUX_EXCEPTION_END
  }
  catch (const OdEdKeyword& kw)
  {
    sCategory = kw.keyword() ;
  }

  if (sCategory == OD_T("*"))
    sCategory.empty();
  pNamedView->setCategoryName(sCategory);
}

static void view_layerState(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
  if (pDwgDb.isNull())
    throw OdError(eNotImplemented); // TODO for DGN
  OdEdUserIO* pIO = pCmdCtx->userIO();

  OdDbViewTableRecordPtr pNamedView = view_selectView(pCmdCtx, OD_T("Enter view name to edit Layer State or [?]:")); // pmtEnterViewNameToEditLayerState);
  OdString sLSName = pNamedView->getLayerState();

  int indexKeyword = pIO->getKeyword(OD_T("Enter an option [Save/Delete] <Cancel>"), // pmtEnterAnOptionSaveDeleteCancel
                                     OD_T("Save Delete Cancel"), // kwdViewCmd2
                                     2);
  switch (indexKeyword)
  {
  case 0: // save
    if (sLSName.isEmpty())
    {
      OdString name;
      name.format(OD_T("ACAD_VIEWS_%ls"), pNamedView->getName().c_str());
      sLSName = name;
      for (int i = 1; OdDbLayerState::has(pDwgDb, sLSName);)
        sLSName.format(OD_T("%ls(%d)"), name.c_str(), i++);
      pNamedView->upgradeOpen();
      pNamedView->setLayerState(sLSName);
    }
    OdDbLayerState::save(pDwgDb, sLSName, OdDbLayerStateManager::kStateIsHidden | OdDbLayerStateManager::kCurrentViewport);
    break;
  case 1: // delete
    if (!sLSName.isEmpty())
    {
      OdDbLayerState::remove(pDwgDb, sLSName);
      pNamedView->setLayerState(OD_T(""));
    }
    break;
  case 2: // cancel
    break;
  }
}

static void view_orthographic(OdEdCommandContext* pCmdCtx)
{
  OdEdUserIO* pIO = pCmdCtx->userIO();

  int indexKeyword = pIO->getKeyword(OD_T("Enter an option [Top/Bottom/Front/BAck/Left/Right]<Top>"), // TODO via pmtEnterAnOptionOrthographic
                                     OD_T("Top Bottom Front BAck Left Right"), // TODO via kwdViewCmd3
                                     0, OdEd::kInpThrowEmpty);
  switch (indexKeyword)
  {
  case 0: //Top
    view_setDirection(pCmdCtx, OdGeVector3d::kZAxis, OdGeVector3d::kYAxis);
    break;
  case 1: //Bottom
    view_setDirection(pCmdCtx, - OdGeVector3d::kZAxis, OdGeVector3d::kYAxis);
    break;
  case 2: //Front
    view_setDirection(pCmdCtx, - OdGeVector3d::kYAxis, OdGeVector3d::kZAxis);
    break;
  case 3: //BAck
    view_setDirection(pCmdCtx, OdGeVector3d::kYAxis, OdGeVector3d::kZAxis);
    break;
  case 4: //Left
    view_setDirection(pCmdCtx, - OdGeVector3d::kXAxis, OdGeVector3d::kZAxis);
    break;
  case 5: //Right
    view_setDirection(pCmdCtx, OdGeVector3d::kXAxis, OdGeVector3d::kZAxis);
    break;
  }
}

static void view_delete(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
  if (pDwgDb.isNull())
    throw OdError(eNotImplemented); // TODO for DGN
  OdEdUserIO* pIO = pCmdCtx->userIO();

  OdString sPattern;
  try
  {
    FIX_LINUX_EXCEPTION_BEGIN
    sPattern = pIO->getString(OD_T("Enter view name(s) to delete:"), // TODO via pmtEnterViewNamesToDelete
                              OdEd::kGstAllowSpaces | OdEd::kGstNoEmpty, // OdEd::kInpDefault | OdEd::kInpThrowEmpty, 
                              OD_T("*"), // default
                              OD_T("*")); // keywords
    FIX_LINUX_EXCEPTION_END
  }
  catch (const OdEdKeyword& kw)
  {
    sPattern = kw.keyword();
  }

  ODA_ASSERT_ONCE(!sPattern.isEmpty());
  QRegExp rxPattern(getIApp()->toQString(sPattern), Qt::CaseInsensitive, QRegExp::Wildcard);

  bool bNoMatching = true;
  OdDbViewTablePtr pViewTable = pDwgDb->getViewTableId().safeOpenObject();
  OdDbSymbolTableIteratorPtr pItr = pViewTable->newIterator();
  pIO->putString(OD_T("Saved views:")); // TODO via msgSavedViews
  for (pItr->start(); !pItr->done(); pItr->step())
  {
    OdDbViewTableRecordPtr pViewRecord = pItr->getRecord();
    ODA_ASSERT_ONCE(!pViewRecord.isNull());
    QString qsName = getIApp()->toQString(pViewRecord->getName());
    if (!rxPattern.exactMatch(qsName))
      continue;

    bNoMatching = false;

    pViewRecord->upgradeOpen();
    pViewRecord->erase();
  } // end for

  if (bNoMatching)
    pIO->putString(OD_T("No matching view names found."));
}

static void view_restore(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
  if (pDwgDb.isNull())
    throw OdError(eNotImplemented); // TODO for DGN
  OdEdUserIO* pIO = pCmdCtx->userIO();

  OdDbViewTablePtr pViewTable = pDwgDb->getViewTableId().safeOpenObject();
  while (true)
  {
    OdString sName = pIO->getString(OD_T("Enter view name to restore"), // pmtEnterViewNameToRestore
                                    OdEd::kGstAllowSpaces);
    if (sName.isEmpty())
      break;

    OdDbViewTableRecordPtr pNamedView = pViewTable->getAt(sName).openObject();
    if (pNamedView.get())
    {
      OdDbObjectPtr pVpObj = pDwgDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
      OdDbAbstractViewportDataPtr pVpPE(pVpObj);
      pVpPE->setView(pVpObj, pNamedView);
      pVpPE->setUcs(pVpObj, pNamedView);
      pVpPE->setProps(pVpObj, pNamedView);
      OdString sLSName = pNamedView->getLayerState();
      if (!sLSName.isEmpty())
        OdDbLayerState::restore(pDwgDb, sLSName,
                                OdDbLayerStateManager::kUndefDoNothing, OdDbLayerStateManager::kOn | OdDbLayerStateManager::kFrozen);
      IqViewSystem* iVS = getIViewSystem();
      if (iVS)
        iVS->updateViews(pCmdCtx->baseDatabase());
      break;
    }
    
    OdString sMsg;
    sMsg.format(OD_T("Cannot find view \"%ls\"."), // msgCannotFindSpecifiedView
                sName.c_str());
    pIO->putString(sMsg);
  }
}

static void view_save(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
  if (pDwgDb.isNull())
    throw OdError(eNotImplemented); // TODO for DGN
  OdEdUserIO* pIO = pCmdCtx->userIO();

  OdString sName = pIO->getString(OD_T("Enter view name to save"), // TODO via pmtEnterViewNameToSave
                                  OdEd::kGstAllowSpaces);
  if (sName.isEmpty())
    return;
  
  OdDbViewTablePtr pViewTable = pDwgDb->getViewTableId().safeOpenObject(OdDb::kForWrite);
  OdDbViewTableRecordPtr pView = OdDbViewTableRecord::createObject();
  pView->setName(sName);
  try
  {
    pViewTable->add(pView);
  }
  catch(const OdError_DuplicateRecordName& e)
  {
    OdString sMsg; // TODO via pmtViewAlreadyExistsReplaceIt
    sMsg.format(OD_T("View \"%ls\" already exists. Replace it? <N>"), sName.c_str());

    switch (pIO->getKeyword(sMsg, OD_T("Yes No"), 1)) // TODO via kwdYesNoKwds
    {
    case 0: //Yes
      pView = e.existingRecordId().safeOpenObject(OdDb::kForWrite);
      break;
    default: // case 1: //No
      return;
    }
  }

  OdDbObjectPtr pVpObj = pDwgDb->activeViewportId().safeOpenObject();
  OdDbAbstractViewportDataPtr pVpPE(pView);
  pVpPE->setView(pView, pVpObj);
  if (pDwgDb->appServices()->getUCSVIEW())
  {
    pIO->putString(OD_T("UCSVIEW = 1  UCS will be saved with view")); // TODO via msgUcsWillBeSavedWithView;
    pVpPE->setUcs(pView, pVpObj);
  }
  pVpPE->setProps(pView, pVpObj);
}

static void view_window(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
  if (pDwgDb.isNull())
    throw OdError(eNotImplemented); // TODO for DGN
  OdDbBaseDatabaseIOPtr pIO = OdDbBaseDatabaseIO::cast(pCmdCtx->userIO()); // via OdDbUserIO now

  OdString sName = pIO->getString(OD_T("Enter view name to save"));
  if (sName.isEmpty())
    return;

  OdGePoint3d ptFirst = pIO->getPoint(OD_T("Specify first corner"),
                                      OdEd::kGptNoOSnap | OdEd::kGptNoUCS);
  OdGePoint3d ptSecond = pIO->getPoint(OD_T("Specify opposite corner"), 
                                       OdEd::kGptRectFrame | OdEd::kGptNoUCS | OdEd::kGptNoOSnap);
  
  OdDbViewTableRecordPtr pView = OdDbViewTableRecord::createObject();
  OdAbstractViewPEPtr pViewPE(pView);

  OdDbObjectPtr pVpObj = pDwgDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
  pView->setName(sName);
  pViewPE->setView(pView, pVpObj);

  OdGeMatrix3d world2Eye = pViewPE->worldToEye(pVpObj);
  ptFirst.transformBy(world2Eye);
  ptSecond.transformBy(world2Eye);
  OdGeBoundBlock3d extents(ptFirst, ptSecond);
  pViewPE->zoomExtents(pView, &extents);
    
  OdDbViewTablePtr pViewTable = pDwgDb->getViewTableId().safeOpenObject(OdDb::kForWrite);
  try
  {
    FIX_LINUX_EXCEPTION_BEGIN

    pViewTable->add(pView);

    FIX_LINUX_EXCEPTION_END
  }
  catch(const OdError_DuplicateRecordName& e)
  {
    OdString sMsg; // TODO via pmtViewAlreadyExistsReplaceIt
    sMsg.format(OD_T("View \"%ls\" already exists. Replace it? <N>"), sName.c_str());

    switch (pIO->getKeyword(sMsg, OD_T("Yes No"), 1)) // TODO via kwdYesNoKwds
    {
    case 0: //Yes
      pView = e.existingRecordId().safeOpenObject(OdDb::kForWrite);
      break;
    default: // case 1: //No
      return;
    }
  }
}

static void view_ucs(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
  if (pDwgDb.isNull())
    throw OdError(eNotImplemented); // TODO for DGN
  OdEdUserIO* pIO = pCmdCtx->userIO();

  OdString sMsg = OD_T("Save current UCS with named views?[Yes/No]<Yes>");
  int indexDefault = 0;
  if (!pDwgDb->appServices()->getUCSVIEW())
  {
    indexDefault = 1;
    sMsg += OD_T("<No>");
  }
  else
    sMsg += OD_T("<Yes>");
  
  int nValue = !pIO->getKeyword(sMsg,  OD_T("Yes No"), indexDefault) ? 1 : 0;
  pDwgDb->appServices()->setUCSVIEW(nValue);
}

///////////////////////////////////////////////////////////////////////////////

void OdqCmd_View::execute(OdEdCommandContext* pCmdCtx)
{
  if (!pCmdCtx->baseDatabase())
    throw OdError(eNoDatabase);
  OdEdUserIO* pIO = pCmdCtx->userIO();

  OdString sPrompt(OD_T("Enter an option [?/Delete/Orthographic/Restore/Save/sEttings/Window]")), // TODO via pmtViewCmdEnterAnOption
           sKeywords(OD_T("? Delete Orthographic Restore Save sEttings Window")); // TODO via kwdViewCmd1
  sKeywords += OD_T(" Top Bottom Front BAck Left RIght SWIso SEIso NEIso NWIso");
  sKeywords += OD_T(" Categorize lAyer Ucs");

  while (true)
  {
    int indexKeyword = pIO->getKeyword(sPrompt, sKeywords, -1, OdEd::kInpThrowEmpty); // OdEd::kInpDefault | OdEd::kGstNoEmpty);
    switch (indexKeyword)
    {
    case -1:
      return;
    case 0: // ?
      view_list(pCmdCtx);
      return;
    case 1: // Delete
      view_delete(pCmdCtx);
      return;
    case 2: // Orthographic
      view_orthographic(pCmdCtx);
      return;
    case 3: // Restore
      view_restore(pCmdCtx);
      return;
    case 4: // Save
      view_save(pCmdCtx);
      return;
    case 5: // sEttings
      indexKeyword = pIO->getKeyword(OD_T("Enter an option [Background/Categorize/Layer snapshot/live Section/Ucs/Visual style]"),
                                     OD_T("Background Categorize Layer Section Ucs Visual"),
                                     -1, OdEd::kInpThrowEmpty);
      switch (indexKeyword)
      {
      case -1:
        break;
      case 0: // Background 
        view_categorize(pCmdCtx);
        break;
      case 1: // Categorize 
        view_categorize(pCmdCtx);
        break;
      case 2: // Layer snapshot
        view_layerState(pCmdCtx);
        break;
      case 3: // live Section
        throw OdError(eNotImplemented); // TODO
      case 4: // Ucs
        view_ucs(pCmdCtx);
        break;
      case 5: // Visual style
        throw OdError(eNotImplemented); // TODO
      } // end switch
      continue;
    case 6: // Window
      view_window(pCmdCtx);
      break;
    case 7: // Top 
      view_setDirection(pCmdCtx, OdGeVector3d::kZAxis, OdGeVector3d::kYAxis);
      return;  
    case 8: // Bottom
      view_setDirection(pCmdCtx, - OdGeVector3d::kZAxis, OdGeVector3d::kYAxis);
      return;  
    case 9: // Front
      view_setDirection(pCmdCtx, - OdGeVector3d::kYAxis, OdGeVector3d::kZAxis);
      return;  
    case 10: // BAck
      view_setDirection(pCmdCtx, OdGeVector3d::kYAxis, OdGeVector3d::kZAxis);
      return;  
    case 11: // Left
      view_setDirection(pCmdCtx, - OdGeVector3d::kXAxis, OdGeVector3d::kZAxis);
      return;  
    case 12: // RIght
      view_setDirection(pCmdCtx, OdGeVector3d::kXAxis, OdGeVector3d::kZAxis);
      return;  
    case 13: // SWIso
      view_setDirection(pCmdCtx, OdGeVector3d(-1.0, -1.0, 1.0), OdGeVector3d(0.5, 0.5, 1.0).normal());
      return;
    case 14: // SEIso
      view_setDirection(pCmdCtx, OdGeVector3d(1.0, -1.0, 1.0), OdGeVector3d(-0.5, 0.5, 1.0).normal());
      return;
    case 15: // NEIso
      view_setDirection(pCmdCtx, OdGeVector3d(1.0, 1.0, 1.0), OdGeVector3d(-0.5, -0.5, 1.0).normal());
      return;
    case 16: // NWIso
      view_setDirection(pCmdCtx, OdGeVector3d(-1.0, 1.0, 1.0), OdGeVector3d(0.5, -0.5, 1.0).normal());
      return;
    case 17: // Categorize
      view_categorize(pCmdCtx);
      continue;
    case 18: // lAyer
      view_layerState(pCmdCtx);
      continue;
    case 19: // Ucs
      view_ucs(pCmdCtx);
      continue;
    } // end switch  
  } // end while
}

static bool checkBackground(OdEdCommandContext* pCmdCtx, IqView*& iView)
{
  iView = NULL;
  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  if (!pRxDatabase)
    return false; // throw OdError(eNoDatabase);
  IqViewSystem* iVS = getIViewSystem();
  if (!iVS)
    throw OdError(eInvalidView);
  iView = iVS->getView(pRxDatabase);
  if (!iView)
    throw OdError(eInvalidView);

  OdGsDevice* pGsDevice = iView->getGsDevice();
  ODA_ASSERT_ONCE(pGsDevice);

  //OdGiContextForDbDatabasePtr pGiCtx = pGsDevice->userGiContext();
  OdGiDefaultContextPtr pGiCtx = OdGiDefaultContext::cast(pGsDevice->userGiContext());
  ODA_ASSERT_ONCE(!pGiCtx.isNull());

  if (static_cast<OdqView*>(iView)->isModelLayout())
  {
    ODCOLORREF clrBackground = getIAppProps()->toColorref(OD_T("Options/Display/ModelBackground"),
                                                          ODRGB(255, 255, 255));
    OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRxDatabase).get();
    OdExtDbModule* pExtDb = !pDwgDb ? getIApp()->getExtDbModule(pRxDatabase) : NULL;
    if (pExtDb)
      // try to get background via DgDatabase models
      pExtDb->getModelBackground(pRxDatabase, clrBackground);

    if (clrBackground != pGsDevice->getBackgroundColor())
    {
      // refresh background of model

      const ODCOLORREF* pColors = odcmAcadPalette(clrBackground); // odcmAcadDarkPalette() or odcmAcadLightPalette();
      ODGSPALETTE palette;
      palette.resize(256);
      for (int index = 0; index < 256; index++)
        palette.setAt(index, pColors[index]);
      palette[0] = clrBackground;
      pGsDevice->setLogicalPalette(palette.asArrayPtr(), 256);

      pGiCtx->setPaletteBackground(clrBackground);
      iView->setBackgroundColor(clrBackground);
      iView->setViewportBorderProperties();
    }
  }
  else
  {
    ODCOLORREF clrPaper = getIAppProps()->toColorref(OD_T("Options/Display/PaperColor"),
                                                     ODRGB(255, 255, 255));
    if (clrPaper != pGiCtx->paletteBackground())
      pGiCtx->setPaletteBackground(clrPaper);

    ODCOLORREF clrPaperBackground = getIAppProps()->toColorref(OD_T("Options/Display/PaperBackground"),
                                                               ODRGB(173, 173, 173)); // ACAD's color for paper background
    if (clrPaperBackground != pGsDevice->getBackgroundColor())
    {
      iView->setBackgroundColor(clrPaperBackground);
      iView->setViewportBorderProperties();
    }
  }

  pGsDevice->invalidate();
  return true;
}

void OdqCmd_RegenAll::execute(OdEdCommandContext* pCmdCtx)
{
  IqView* iView = NULL;
  if (!checkBackground(pCmdCtx, iView) || !iView)
    return;

  OdGsModel* pGsModel = iView->getGsModel();
  if (pGsModel) // it is NULL if GS cache off 
    pGsModel->invalidate(OdGsModel::kInvalidateAll);
  iView->updateGL(); // repaint
}

// for active (currently zoomed) viewport only
void OdqCmd_Regen::execute(OdEdCommandContext* pCmdCtx)
{
  IqView* iView = NULL;
  if (!checkBackground(pCmdCtx, iView) || !iView)
    return;

  OdGsModel* pGsModel = iView->getGsModel();
  if (pGsModel) // it is NULL if GS cache off
  {
    OdGsView* pGsView = iView->getActiveTopGsView(); // iView->getActiveGsView();
    pGsModel->invalidate(pGsView);

    //OdGsDevice* pGsDevice = iView->getGsDevice();
    //OdGsPaperLayoutHelperPtr pPSHelper = OdGsPaperLayoutHelper::cast(pGsDevice);
    //if (pPSHelper.get())
    if (!static_cast<OdqView*>(iView)->isModelLayout()) 
    {
      pGsModel->invalidate(OdGsModel::kInvalidateAll);
      // test
      //int nCount = pGsDevice->numViews(),
      //    indexActive = -1;
      //for (int index = 0; index < nCount; index++)
      //{
      //  if (pGsView != pGsDevice->viewAt(index))
      //    continue;
      //  indexActive = index;
      //  break;
      //}
      //int indexContent = (nCount > 1) ? 1 : 0;
      //ODA_ASSERT_ONCE(indexContent >= 0 && indexContent < nCount);
      //if (indexContent != indexActive && indexContent < nCount)
      //  pGsModel->invalidate(pGsDevice->viewAt(indexContent));

    } // end if (!pPSHelper.isNull())
  }
  iView->updateGL(); // repaint
}

static OdDb::RenderMode toRenderMode(OdDbVisualStyle* pVisStyle)
{
  const OdGiFaceStyle& face = pVisStyle->faceStyle();
  const OdGiEdgeStyle& edge = pVisStyle->edgeStyle();
  if (   face.faceColorMode() == OdGiFaceStyle::kBackgroundColor
      || face.lightingModel() == OdGiFaceStyle::kInvisible)
  {
    if (edge.edgeModel() == OdGiEdgeStyle::kFacetEdges)
    {
      if (edge.isEdgeStyleFlagSet(OdGiEdgeStyle::kObscured))
        return OdDb::kHiddenLine;
    }
    return OdDb::kWireframe;
  }
  
  bool bWf = edge.edgeModel() == OdGiEdgeStyle::kFacetEdges;
  bool bFlat = face.lightingQuality() == OdGiFaceStyle::kPerFaceLighting;
  if (bWf)
  {
    if (bFlat)
      return OdDb::kFlatShadedWithWireframe;
    return OdDb::kGouraudShadedWithWireframe;
  }
  else
  {
    if (bFlat)
      return OdDb::kFlatShaded;
    return OdDb::kGouraudShaded;
  }
}

static OdDbStub* getVsIdByName(OdDbBaseDatabase* pRxDb, const OdString& sName, OdDbStub* idVsDef = NULL)
{
  OdDbBaseDatabasePE* pDbPE = OdDbBaseDatabasePE::cast(pRxDb).get();
  ODA_ASSERT_ONCE(pRxDb && pDbPE && !sName.isEmpty());

  for (OdRxIteratorPtr pVsItr = pDbPE->visualStyles(pRxDb); 
       pVsItr.get() && !pVsItr->done(); pVsItr->next())
  {
    OdRxObjectPtr pVsObject = pVsItr->object();
    OdDbBaseVisualStylePE* pVsPE = OdDbBaseVisualStylePE::cast(pVsObject).get();
    ODA_ASSERT_ONCE(pVsPE);
    if (!pVsPE || sName != pVsPE->name(pVsObject) || pVsPE->isInternal(pVsObject))
      continue;
    
    OdGiDrawable* pDrw = OdGiDrawable::cast(pVsObject).get();
    ODA_ASSERT_ONCE(pDrw && pDrw->id());

    return pDrw->id();
  }
  return idVsDef;
}

static OdString getVsNameByRm(OdGsView::RenderMode rm, const OdString& sVsDef = OdString::kEmpty)
{
  switch (rm)
  {
  case OdGsView::k2DOptimized: // via DGN
  case OdGsView::kWireframe:
    return OdDb::kszVS3DWireframe; 
  case OdGsView::kHiddenLine:
    return OdDb::kszVSHidden;
    break;
  case OdGsView::kGouraudShaded: 
    //rm = OdGsView::kFlatShaded;
    //rm = OdGsView::kFlatShadedWithWireframe;
    return OdDb::kszVSRealistic;
  case OdGsView::kGouraudShadedWithWireframe:
    return OdDb::kszVSConceptual;
  }
  return sVsDef;
}

void OdqCmd_VSCurrent::execute(OdEdCommandContext* pCmdCtx)
{
  if (!pCmdCtx->baseDatabase())
    throw OdError(eNoDatabase);
  IqViewSystem* iVS = getIViewSystem();
  if (!iVS)
    throw OdError(eInvalidView);
  IqView* iView = iVS->getView(pCmdCtx->baseDatabase());
  if (!iView)
    throw OdError(eInvalidView);
  OdEdUserIO* pIO = pCmdCtx->userIO();

  OdDbBaseDatabase* pRxDb = pCmdCtx->baseDatabase();
  OdDbBaseDatabasePE* pDbPE = OdDbBaseDatabasePE::cast(pRxDb).get();
  OdDbDatabase* pDwgDb = OdDbDatabase::cast(pRxDb).get();

  OdDbStub* idVs = NULL,
          * idVsPrev = NULL;

  OdGsView* pGsView = iView->getActiveTopGsView(); // iView->getActiveGsView();
  if (pDwgDb && !pDwgDb->getTILEMODE() && pGsView == pGsView->device()->viewAt(0))
  {
    pIO->putString(L"** Cannot set a visual style in paper space **");
    return;
  }

  OdGsClientViewInfo clientViewInfo;
  pGsView->clientViewInfo(clientViewInfo);
  OdRxObjectPtr pViewport = pDbPE->openObject(clientViewInfo.viewportObjectId);
  OdAbstractViewPEPtr pViewPE = OdAbstractViewPE::cast(pViewport);

  OdRxObjectPtr pVsObj;
  OdDbBaseVisualStylePE* pVsPE = NULL;
  OdGsView::RenderMode rm = pGsView->mode(); // undef

  if (pViewport.get() && pViewPE.get())
  {
    idVs = pViewPE->visualStyle(pViewport);
    pVsObj = pDbPE->openObject(idVs);
    pVsPE = OdDbBaseVisualStylePE::cast(pVsObj).get();
    ODA_ASSERT_ONCE((idVs && pVsObj.get() && pVsPE) || !pDwgDb);

    rm = (OdGsView::RenderMode) pViewPE->renderMode(pViewport);
  }
  idVsPrev = idVs;

  bool bSkipOther = !pVsPE;
  if (bSkipOther)
  {
    OdRxIteratorPtr pVsItr = pDbPE->visualStyles(pRxDb);
    if (pVsItr.get() && !pVsItr->done())
      bSkipOther = false;
  }

  OdString sKeywords = L"2dwireframe Wireframe 3dHidden Realistic Conceptual", // " Other",
           sPrompt,
           sDefault = getVsNameByRm(rm, OdDb::kszVS2DWireframe);
  int indexDefault = 0;
  if (!bSkipOther) 
  {
    sKeywords += L" Other";
    QString qsKeyword = getIApp()->toQString(pVsPE ? pVsPE->name(pVsObj) : sDefault);
    QString qsKeywordLow = qsKeyword.toLower(); 
    qsKeywordLow.remove(' ');
    QStringList lstKeywords = getIApp()->toQString(sKeywords).split(' ');
    int index = -1;
    indexDefault = -1;
    foreach(QString qsKwd, lstKeywords)
    {
      index++;
      if (qsKeywordLow != qsKwd.toLower())
        continue;
      qsKeyword = qsKwd;
      indexDefault = index;
      break;
    }
    sDefault = getIApp()->toOdString(qsKeyword);
    if (indexDefault < 0)
      indexDefault = ++index;
  }
  sPrompt.format(L"Enter an option [2dwireframe/Wireframe/3dHidden/Realistic/Conceptual%ls]<%ls>", 
                 !bSkipOther ? L"/Other" : L"", sDefault.c_str());
  
  OdString sName; 
  //int indexKeyword = pIO->getKeyword(sPrompt, sKeywords, indexDefault, OdEd::kGstAllowSpaces);
  // no way not to get default string with spaces (possible as current visual style) via getKeyword
  int indexKeyword = indexDefault;
  try
  {
    FIX_LINUX_EXCEPTION_BEGIN
    while (true)
    {
      sName = pIO->getString(sPrompt, OdEd::kGstAllowSpaces | OdEd::kGstNoEmpty, sDefault, sKeywords);
      if (sName == sDefault) // sDefault ma contain spaces for custom VS
        break;
      pIO->putString(OD_T(""));
      pIO->putString(OD_T("Invalid option keyword."));
    }
    FIX_LINUX_EXCEPTION_END
  }
  catch (const OdEdKeyword& kw)
  {
    indexKeyword = kw.keywordIndex();
  }

  switch (indexKeyword)
  {
  default: // kszVSBasic             "Basic" ...
    ODA_FAIL_ONCE(); // TODO
  case 0: // OdDb::kszVS2DWireframe "2dwireframe"
    idVs = getVsIdByName(pRxDb, OdDb::kszVS2DWireframe, pDwgDb ? idVs : NULL);
    sName = OdDb::kszVS2DWireframe;
    rm = OdGsView::k2DOptimized;
    break;
  case 1: // kszVS3DWireframe       "Wireframe"
    idVs = getVsIdByName(pRxDb, OdDb::kszVS3DWireframe, pDwgDb ? idVs : NULL); 
    sName = OdDb::kszVS3DWireframe;
    rm = OdGsView::kWireframe;
    break;
  case 2: // kszVSHidden            "3D Hidden"
    idVs = getVsIdByName(pRxDb, OdDb::kszVSHidden, pDwgDb ? idVs : NULL); 
    sName = OdDb::kszVSHidden;
    rm = OdGsView::kHiddenLine;
    break;
  case 3: // kszVSRealistic         "Realistic"
    idVs = getVsIdByName(pRxDb, OdDb::kszVSRealistic, pDwgDb ? idVs : NULL);
    sName = OdDb::kszVSRealistic;
    rm = OdGsView::kGouraudShaded;
    //rm = OdGsView::kFlatShaded;
    //rm = OdGsView::kFlatShadedWithWireframe;
    break;
  case 4: // kszVSConceptual        "Conceptual"
    idVs = getVsIdByName(pRxDb, OdDb::kszVSConceptual, pDwgDb ? idVs : NULL); 
    sName = OdDb::kszVSConceptual;
    rm = OdGsView::kGouraudShadedWithWireframe;
    break;
  case 5: // Other
    if (pDbPE || pDwgDb)
    {
      while (true)
      {
        try
        {
          FIX_LINUX_EXCEPTION_BEGIN
          sPrompt = OD_T("Enter a visual style name or [?]");
          sName = pIO->getString(sPrompt, OdEd::kGstAllowSpaces | OdEd::kGstNoEmpty, OD_T("?"), OD_T("?"));
          FIX_LINUX_EXCEPTION_END
        }
        catch (const OdEdKeyword& kw)
        {
          sName = kw.keyword();
        }
        if (sName == OD_T("?"))
        {
          if (pDbPE)
          {
            for (OdRxIteratorPtr pVsItr = pDbPE->visualStyles(pRxDb); 
                 pVsItr.get() && !pVsItr->done(); pVsItr->next())
            {
              OdRxObjectPtr pVsObj = pVsItr->object();
              OdDbBaseVisualStylePE* pVsPE = OdDbBaseVisualStylePE::cast(pVsObj).get();
              ODA_ASSERT_ONCE(pVsPE);
              if (!pVsPE || pVsPE->isInternal(pVsObj))
                continue;
              sName = L"  " + pVsPE->name(pVsObj);
              pIO->putString(sName);
            }
          }
          else
          {
            ODA_FAIL_ONCE(); // TODO
          }
          continue;
        }

        if (pDbPE)
        {
          idVs = getVsIdByName(pRxDb, sName); 
          if (!idVs)
          {
            idVs = idVsPrev;
            OdString sMsg = getIApp()->toOdString(QObject::tr("The \"%1\" visual style does not exist.")
                                                              .arg(getIApp()->toQString(sName)));
            pIO->putString(sMsg);
            continue;
          }
          if (pDwgDb)
          {
            OdDbVisualStylePtr pVisStyle = OdDbVisualStyle::cast(pDbPE->openObject(idVs));
            ODA_ASSERT_ONCE(pVisStyle.get());
            if (pVisStyle.get())
              rm = (OdGsView::RenderMode) toRenderMode(pVisStyle); // translate custom visual style to render mode
          }
        }
        else
        {
          ODA_FAIL_ONCE(); // TODO
        }
        break;
      }  // end while
    }
    else
    {
      ODA_FAIL_ONCE(); // TODO
    }
    break;
  case 6: // Default with non-standard name
    // sDefault is name (already current)
    return;
  }

  bool bRefreshView = false,
       bRefreshViewForce = false;

  IqRevDiff* pRevDiff = getIRevSystem() ? getIRevSystem()->getIRevDiff(iView) : NULL;

  if (idVs != idVsPrev)
  {
    ODA_ASSERT_ONCE(idVs || !pDwgDb);
    if( pRevDiff )
    {
      pRevDiff->setVisualStyle( sName );
      bRefreshView = true;
    }
    else
    {
      pGsView->setVisualStyle(idVs);
      pViewPE->setVisualStyle(pViewport, idVs);
      if (((OdDbStub*) idVsPrev) != pViewPE->visualStyle(pViewport))
        bRefreshView = bRefreshViewForce = true;
      if (((OdDbStub*) idVs) != pViewPE->visualStyle(pViewport))
      {
        OdString sMsg = getIApp()->toOdString(QObject::tr("Rendering: Visual Style is not supported."));
        getIApp()->getBaseHostAppServices()->warning(sMsg);
      }
    }
  }
  
  if (rm != pGsView->mode() && !pRevDiff)
  {
    bRefreshView = true;
    if (rm >= OdGsView::kFlatShaded && pGsView->mode() >= OdGsView::kFlatShaded)
      bRefreshViewForce = true;
    if (pViewport.get() && pViewPE.get())
      pViewPE->setRenderMode(pViewport, (OdDb::RenderMode) rm);
    pGsView->setMode(rm); 
    if (rm != pGsView->mode())
    {
      OdString sMsg = getIApp()->toOdString(QObject::tr("Rendering: Mode is not supported by current device."));
      getIApp()->getBaseHostAppServices()->warning(sMsg);
    }
  }

  if (bRefreshView && pGsView->device()->properties()->has(OD_T("OpenRLVersion"))) // or via OpenRLContextsCount
  {
    bRefreshView = false; // unsupported for Caustic device
    // already contained in device as : ODA Warning: Rendering: Mode is not supported by current device.
    //static OdGsDevice* s_pDeviceOfLastWarn = NULL;
    //if (s_pDeviceOfLastWarn != pGsView->device())
    //{
    //  s_pDeviceOfLastWarn = pGsView->device();
    //  OdString sMsg = getIApp()->toOdString(
    //                    QObject::tr("ODA Warning: Rendering: There is not visible"
    //                                " difference between visual styles for OpenRL-based devices."));
    //  getIApp()->getBaseHostAppServices()->warning(sMsg);
    //}
  }

  if (bRefreshView)
  {
    OdGsModel* pGsModel = iView->getGsModel();
    if (pGsModel)
    {
      if (bRefreshViewForce)
        pGsModel->invalidate(OdGsModel::kInvalidateAll);
      else
        pGsModel->invalidate(pGsView);
    }
    iView->updateGL(); // repaint
  }
}

#if defined(ODA_WINDOWS) && defined(_TOOLKIT_IN_DLL_)
void OdqCmd_GlesDlg::execute(OdEdCommandContext* pCmdCtx)
{
  OdSmartPtr<OdDbUserIO> pIO = pCmdCtx->userIO();

  OdRxDictionaryPtr pProperties;
  IqView* iView = getIViewSystem()->getView(pCmdCtx->baseDatabase());
  if (iView)
    pProperties = iView->getGsDevice()->properties();
  if (pProperties.isNull())
  {
    pIO->putString(L"  Device is missing.\n");
    return;
  }
  if (!pProperties->has(L"RenderSettingsDlg"))
  {
    pIO->putString(L"  RenderSettingsDlg is unsupported for current device.\n");
    return;
  }
  bool isOn = OdRxVariantValue(pProperties->getAt(L"RenderSettingsDlg"))->getIntPtr() != 0;
  if (isOn)
  {
    //pIO->putString(L"  RenderSettingsDlg is already on.\n");
    pProperties->putAt(L"RenderSettingsDlg", OdRxVariantValue((OdIntPtr) 0));
    return;
  }

  OdString sCompositeMfName = L"UseCompositeMetafiles"; // L"ExGLES2CompositeMf" is obsolete
  ODA_ASSERT_ONCE(pProperties->has(sCompositeMfName));
  if (pProperties->has(sCompositeMfName))
  {
    isOn = OdRxVariantValue(pProperties->getAt(sCompositeMfName))->getBool();

    bool bEnabled = getIAppProps()->toBool(L"Options/Display/ExGLES2CompositeMf", false);
    if (!bEnabled)
    {
      bEnabled = (0 == pIO->getKeyword(L"Enable GLES2 Composite Metafiles Mode Yes/No]",
                                       L"Yes No",
                                       1));
      if (bEnabled)
        bEnabled = getIAppProps()->set(L"Options/Display/ExGLES2CompositeMf", bEnabled, L"bool");
    }
    if (bEnabled != isOn)
      pProperties->putAt(sCompositeMfName, OdRxVariantValue(bEnabled));
  }

  ODA_ASSERT_ONCE(pProperties->has(L"UseVisualStyles"));
  if (pProperties->has(L"UseVisualStyles"))
    pProperties->putAt(L"UseVisualStyles", OdRxVariantValue(false));

  void* hVal = (void*)getIApp()->getMainWindow()->winId(); // AfxGetMainWnd()->GetSafeHwnd();
  pProperties->putAt(L"RenderSettingsDlg", OdRxVariantValue((OdIntPtr)hVal));
}
#endif

void OdqCmd_SysWindows::execute(OdEdCommandContext* pCmdCtx)
{
  OdqViewSystem* pVS = (OdqViewSystem*) getIViewSystem();
  if (   !pVS 
      || !pVS->getMdiArea()
      || !pVS->getMdiArea()->activeSubWindow() // currentSubWindow()
      || !pVS->getMdiArea()->viewport())
  {
    ODA_FAIL_ONCE();
    return;
  }

  int indexKeyword = pCmdCtx->userIO()
        ->getKeyword(OD_T("Enter an option [Cascade/tile Horizontal/tile Vertical/Arrange windows]"),
                     OD_T("Cascade Horizontal Vertical Arrange"));
  //QMdiSubWindow* pActiveSubWindow = pVS->getMdiArea()->activeSubWindow(); // currentSubWindow()
  QList<QMdiSubWindow*> lstSubWindows;
  QMdiSubWindow* pSubWindow = NULL;
  foreach(pSubWindow, pVS->getMdiArea()->subWindowList(QMdiArea::CreationOrder))
  {
    if (!pSubWindow->isVisible())
      continue;
    if (pSubWindow->isMinimized() && !pSubWindow->isShaded())
      continue;
    if (pSubWindow->isMaximized() || pSubWindow->isShaded())
      pSubWindow->showNormal();

    lstSubWindows << pSubWindow;
    
    OdqView* pView = qobject_cast<OdqView*>(pSubWindow);
    if (!pView)
      continue;
    pView->m_pRect = NULL;
    pView->m_pSize = NULL;
  }
  int numWnds = lstSubWindows.size();
  if (!numWnds)
    return;

  switch (indexKeyword)
  {
  case 0: // Cascade 
    pVS->getMdiArea()->cascadeSubWindows();
    return;
  case 1: // Horizontal 
  case 2: // Vertical 
    {
      QRect rectDomain = pVS->getMdiArea()->viewport()->rect();
      int numCols = 1,
          numRows = 1;
      if (indexKeyword == 1) // Horizontal 
        numRows = numWnds;
      else
      {
        ODA_ASSERT_ONCE(indexKeyword == 2); // Vertical 
        numCols = numWnds;
      }

      int nspecial = (numWnds % numCols) ? (numCols - (numWnds % numCols)) : 0;
      int dx = rectDomain.width()  / numCols;
      int dy = rectDomain.height() / numRows;

      int index = 0;
      for (int row = 0; row < numRows; ++row) 
      {
        int y1 = (int) row * (dy + 1);
        for (int col = 0; col < numCols; ++col) 
        {
          if (row == 1 && col < nspecial)
            continue;
          int x1 = (int) col * (dx + 1),
              x2 = (int) x1 + dx,
              y2 = (int) y1 + dy;
          if (!row && col < nspecial) 
          {
            y2 *= 2;
            if (numRows != 2)
              y2 += 1;
            else
              y2 = rectDomain.bottom();
          }
          if (col == (numCols - 1) && x2 != rectDomain.right())
            x2 = rectDomain.right();
          if (row == (numRows - 1) && y2 != rectDomain.bottom())
            y2 = rectDomain.bottom();

          pSubWindow = lstSubWindows.at(index++);
          QRect rectNewGeometry = QStyle::visualRect(pSubWindow->layoutDirection(), 
                                                     rectDomain, 
                                                     QRect(QPoint(x1, y1), QPoint(x2, y2)));
          pSubWindow->setGeometry(rectNewGeometry);
        }
      }
      return;
    }
  case 3: // Arrange
    pVS->getMdiArea()->tileSubWindows();
    return;
  }
  ODA_ASSERT_ONCE(indexKeyword == -1); // TODO ?

  pCmdCtx->userIO()->putString("Invalid option keyword.");
}

static OdSharedPtr<ODCOLORREF> s_pSvAppBackground;

void OdqCmd_BackgroundToApp::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  if (!s_pSvAppBackground.isNull() || !pRxDatabase || OdDbDatabase::cast(pRxDatabase).get())
    return;
  ODCOLORREF clrBackground;
  OdExtDbModule* pExtDb = getIApp()->getExtDbModuleByExtension(L"dgn");
  if (   !pExtDb
      || !pExtDb->getModelBackground(pRxDatabase, clrBackground))
    return;

  s_pSvAppBackground = new ODCOLORREF;
  *s_pSvAppBackground = getIAppProps()->toColorref(OD_T("Options/Display/ModelBackground"),
                                                   ODRGB(255, 255, 255));
  getIAppProps()->set(OD_T("Options/Display/ModelBackground"), clrBackground);
}

void OdqCmd_AppToBackground::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  if (!pRxDatabase)
    return;
  if (s_pSvAppBackground.get() && OdDbDatabase::cast(pRxDatabase).isNull())
  {
    OdExtDbModule* pExtDb = getIApp()->getExtDbModuleByExtension(L"dgn");
    if (pExtDb)
    {
      ODCOLORREF clrBackground = getIAppProps()->toColorref(OD_T("Options/Display/ModelBackground"),
                                                            ODRGB(255, 255, 255));
      getIAppProps()->set(OD_T("Options/Display/ModelBackground"), *s_pSvAppBackground); // restore
      s_pSvAppBackground = NULL;
      if (!pExtDb->setModelBackground(pRxDatabase, clrBackground))
      {
        ODA_FAIL_ONCE();
      }
    }
  }
  IqView* iView = getIViewSystem()->getView(pRxDatabase);
  if (iView)
    iView->updateLayouts(); // #CORE-10836
}

void OdqCmd_SetActiveWindowByTitle::execute(OdEdCommandContext* pCmdCtx)
{
  OdString sWindowTitle = pCmdCtx->userIO()->getString(OD_T("Enter window title"));
  if (sWindowTitle.isEmpty())
    return;

  if (!getIViewSystem()->setActiveViewbyTitle(sWindowTitle))
  {
    getIApp()->getBaseHostAppServices()->warning(
      OdString().format(OD_T("Window with title \"%ls\" was not found."), sWindowTitle.c_str()));
  }
}

