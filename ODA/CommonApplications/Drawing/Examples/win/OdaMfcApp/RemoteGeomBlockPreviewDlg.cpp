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
// RemoteGeomBlockPreviewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OdaMfcApp.h"
#include "RemoteGeomBlockPreviewDlg.h"
#include "MainFrm.h"
#include "DbAbstractViewportData.h"
#include "Db3dPolyline.h"
#include "Db3dPolylineVertex.h"
#include "DbBlockTableRecord.h"
#include "DbIdMapping.h"
#include "DbBlockReference.h"

typedef OdArray<ODCOLORREF, OdMemoryAllocator<ODCOLORREF> > ODGSPALETTE;

// CRemoteGeomBlockPreviewDlg dialog

IMPLEMENT_DYNAMIC(CRemoteGeomBlockPreviewDlg, CDialog)

UINT CRemoteGeomBlockPreviewDlg::g_nRedrawMSG = 0;

CRemoteGeomBlockPreviewDlg::CRemoteGeomBlockPreviewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteGeomBlockPreviewDlg::IDD, pParent)
{
  m_pDb = NULL;
  m_hWindowDC = NULL;
}

CRemoteGeomBlockPreviewDlg::~CRemoteGeomBlockPreviewDlg()
{
}

void CRemoteGeomBlockPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CRemoteGeomBlockPreviewDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  if(!g_nRedrawMSG)
    g_nRedrawMSG = ::RegisterWindowMessage(_T("OdaMfcApp::CRemoteGeomBlockPreviewDlg::WM_REDRAW"));

  return TRUE;
}

void CRemoteGeomBlockPreviewDlg::OnDraw(CDC* /*pDC*/)
{

}

void CRemoteGeomBlockPreviewDlg::RegenAll() 
{
  m_pDevice->invalidate();
  if(m_pDevice->gsModel())
    m_pDevice->gsModel()->invalidate(OdGsModel::kInvalidateAll);
  
  PostMessage(WM_PAINT);
}

void CRemoteGeomBlockPreviewDlg::ZoomExtents(OdDbDatabasePtr pDb)
{
  OdGsViewPtr pView = m_pDevice->activeView();
  OdGeExtents3d ext;
  
  pDb->getModelSpaceId().safeOpenObject()->getGeomExtents( ext );
  //ext.addPoint(ext.minPoint() + OdGeVector3d(-10,-10,-10));
  //ext.addPoint(ext.maxPoint() + OdGeVector3d(10,10,10));
  
  double dExt = 100.;

  OdGeVector3d dir(-dExt, dExt, dExt );
  
  OdGePoint3d orig = ext.center();

  pView->setView( orig, orig + dir, dir.perpVector().perpVector(), dExt, dExt, OdGsView::kParallel );

  ext.transformBy( pView->viewingMatrix() );

  dExt = (ext.maxPoint() - ext.minPoint()).length();

  dir.set(-dExt, dExt, dExt );

  pView->setView( orig, orig + dir, dir.perpVector().perpVector(), dExt, dExt, OdGsView::kParallel );

  /*
  OdAbstractViewPEPtr pPEView = pViewObj;
  OdGeVector3d dir(-dExt, dExt, dExt );
  pPEView->setView( pViewObj, OdGePoint3d::kOrigin, dir, dir.perpVector().perpVector(), dExt, dExt, false );
  pPEView->zoomExtents( pViewObj );
*/
  //pDb->writeFile(OD_T("C:\\BODY\\test.dwg"), OdDb::kDwg, OdDb::kDHL_CURRENT );

  /*OdDbObjectPtr pViewObj = pDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
  OdAbstractViewPEPtr pPEView = pViewObj;
  pPEView->zoomExtents( pViewObj );  */
}

void Test(OdDbDatabasePtr pDb)
{
  OdDbObjectId idPoly;
  {
    OdDb3dPolylinePtr ptrPoly = OdDb3dPolyline::createObject();
    ptrPoly->setPolyType(OdDb::k3dSimplePoly);
    OdDbBlockTableRecordPtr pBlock = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
    idPoly = pBlock->appendOdDbEntity(ptrPoly);
  }

  {
    OdDb3dPolylinePtr ptrPoly = idPoly.openObject(OdDb::kForWrite);

    {
      OdDb3dPolylineVertexPtr vtx = OdDb3dPolylineVertex::createObject();
      vtx->setVertexType(OdDb::k3dSimpleVertex);
      vtx->setPosition(OdGePoint3d(0,0,0));
      ptrPoly->appendVertex(vtx);   
    }

    {
      OdDb3dPolylineVertexPtr vtx = OdDb3dPolylineVertex::createObject();
      vtx->setVertexType(OdDb::k3dSimpleVertex);
      vtx->setPosition(OdGePoint3d(110,110,110));
      ptrPoly->appendVertex(vtx);   
    }
  }

  //ZoomExtents(pDb);  
}

void CRemoteGeomBlockPreviewDlg::ClearDatabase()
{
  OdDbObjectId idModelSpace = m_pDb->getModelSpaceId();
  OdDbBlockTableRecordPtr pModelSpace = idModelSpace.safeOpenObject(OdDb::kForWrite);

  OdDbObjectIteratorPtr pIt = pModelSpace->newIterator();
  for (pIt->start(); !pIt->done(); pIt->step())
  {
    OdDbObjectId id = pIt->objectId();
    OdDbObjectPtr pObj = id.safeOpenObject(OdDb::kForWrite);
    if ( !pObj.isNull() )
      pObj->erase();
  }
}

void CRemoteGeomBlockPreviewDlg::SetPreview(OdDbDatabasePtr pDb, OdDbObjectId idBlock)
{
  ClearDatabase();
  {
    OdDbIdMappingPtr pMap = OdDbIdMapping::createObject();
    OdDbObjectIdArray arrBlocks;
    arrBlocks.append(idBlock);

    pDb->wblockCloneObjects(arrBlocks, m_pDb->getBlockTableId(), *pMap, OdDb::kDrcIgnore, true);
  
    OdDbObjectId idClonedBlock;
    OdDbIdPair idPair( arrBlocks[0] );
    if ( pMap->compute( idPair ) )
      idClonedBlock = idPair.value();

    {
      OdDbBlockTableRecordPtr pMS = m_pDb->getModelSpaceId().safeOpenObject( OdDb::kForWrite );

      OdDbBlockReferencePtr pInsert = OdDbBlockReference::createObject();
      pInsert->setDatabaseDefaults(m_pDb);

      pInsert->setBlockTableRecord(idClonedBlock);            
      pMS->appendOdDbEntity( pInsert );
    }
  }

  ZoomExtents(m_pDb);
  RegenAll();
}

void CRemoteGeomBlockPreviewDlg::CreateViewer()
{
  OdDb::MeasurementValue measure = OdDb::kEnglish;    
  //m_pDb = theApp.createDatabase(true, measure);
  m_pDb = svcs.createDatabase(true, measure);

  OdGiContextForDbDatabase::setDatabase(m_pDb);
  m_hWindowDC = ::GetDC(m_hWnd);

  CreateDevice();
  if(m_pDevice.isNull())
  {
    GetParent()->PostMessage(WM_CLOSE);
    return;
  }
  
  /*m_editor.initialize(m_pDevice, static_cast<COdaMfcAppDoc*>(GetDocument())->cmdCtx() );
  setCursor(::LoadCursor(0, IDC_ARROW));*/  
}

void CRemoteGeomBlockPreviewDlg::CreateDevice() 
{
  CRect rc;
  GetClientRect(&rc);

  try
  {
    OdGsModulePtr pGs = ::odrxDynamicLinker()->loadModule(theApp.recentGsDevicePath(), false);
    //OdGsModulePtr pGs = ::odrxDynamicLinker()->loadModule("", false);
    OdGsDevicePtr pDevice = pGs->createDevice();
    OdRxDictionaryPtr pProperties = pDevice->properties();
    if(pProperties.get())
    {
      if(pProperties->has(OD_T("WindowHWND"))) // Check if property is supported
        pProperties->putAt(OD_T("WindowHWND"), OdRxVariantValue((OdIntPtr)m_hWnd)); // hWnd necessary for DirectX device
      if(pProperties->has(OD_T("WindowHDC"))) // Check if property is supported
        pProperties->putAt(OD_T("WindowHDC"),  OdRxVariantValue((OdIntPtr)m_hWindowDC)); // hWindowDC necessary for Bitmap device
      if(pProperties->has(OD_T("DoubleBufferEnabled"))) // Check if property is supported
        pProperties->putAt(OD_T("DoubleBufferEnabled"), OdRxVariantValue(theApp.doubleBufferEnabled()));
      if(pProperties->has(OD_T("EnableSoftwareHLR"))) // Check if property is supported
        pProperties->putAt(OD_T("EnableSoftwareHLR"), OdRxVariantValue(theApp.useSoftwareHLR()));
      if(pProperties->has(OD_T("DiscardBackFaces"))) // Check if property is supported
        pProperties->putAt(OD_T("DiscardBackFaces"), OdRxVariantValue(theApp.discardBackFaces()));
      if(pProperties->has(OD_T("BlocksCache"))) // Check if property is supported
        pProperties->putAt(OD_T("BlocksCache"), OdRxVariantValue(theApp.blocksCacheEnabled()));
    }

    enableGsModel(theApp.useGsModel());

    m_pDevice = OdDbGsManager::setupActiveLayoutViews(pDevice, this);
    m_layoutId = m_pDevice->layoutId();

    const ODCOLORREF* palette = theApp.curPalette();
    ODGSPALETTE pPalCpy;
    pPalCpy.insert(pPalCpy.begin(), palette, palette+256);
    pPalCpy[0] = theApp.activeBackground();
    m_pDevice->setLogicalPalette(pPalCpy.asArrayPtr(), 256);
    
    OdGsPaperLayoutHelperPtr pPSHelper = OdGsPaperLayoutHelper::cast(m_pDevice);
    if(pPSHelper.isNull())
    {
      m_bPsOverall = false;
      m_pDevice->setBackgroundColor(pPalCpy[0]); // for model space
    }
    else
    {
      m_bPsOverall = (pPSHelper->overallView().get() == pPSHelper->activeView().get());
      m_pDevice->setBackgroundColor(ODRGB(173,174,173)); // ACAD's color for paper bg
    }    
    setPaletteBackground(theApp.activeBackground());

    //setViewportBorderProperties();

    OdGsDCRect  gsRect(rc.left, rc.right, rc.bottom, rc.top);
    m_pDevice->onSize(gsRect);

    // Adding plotstyletable info
    //preparePlotstyles();
  }
  catch(const OdError& e)
  {
    DestroyDevice();
    theApp.reportError(_T("Graphic System Initialization Error"), e);
  }  
}

void CRemoteGeomBlockPreviewDlg::DestroyDevice() 
{
  m_pDevice.release();
}

OdString CRemoteGeomBlockPreviewDlg::getString(const OdString& prompt, int options, OdEdStringTracker* pTracker)
{
  //m_sPrompt.Empty();
  //OdSaveState<CString> savePrompt(m_sPrompt);
  //putString(prompt);

  //OdSaveState<Mode>     saved_m_mode(m_mode, kGetString);

  //m_response.m_type = Response::kNone;

  //m_inpOptions = options;

  //SaveViewParams svp(this, pTracker, ::LoadCursor(0, IDC_IBEAM));

  //while ( theApp.PumpMessage() )
  //{
  //  switch(m_response.m_type)
  //  {
  //  case Response::kString:
  //    return m_response.m_string;

  //  case Response::kCancel:
  //    throw OdEdCancel();
  //    break;
  //  }
  //  // Mfc idle loop for toolbars and menus process
  //  LONG lIdle = 0;
  //  while (theApp.OnIdle(lIdle++)) ;  
  //}

  throw OdEdCancel();
}

void CRemoteGeomBlockPreviewDlg::putString(const OdString& string)
{
}

OdUInt32 CRemoteGeomBlockPreviewDlg::getKeyState()
{
  OdUInt32 res = 0;
  if (::GetKeyState(VK_CONTROL) != 0)
    res |= MK_CONTROL;
  if (::GetKeyState(VK_SHIFT) != 0)
    res |= MK_SHIFT;
  return res;
}

bool CRemoteGeomBlockPreviewDlg::beginDragCallback(const OdGePoint3d& pt)
{
  /*OdSaveState<Mode> saved_m_mode(m_mode, kDragDrop);
  GetDocument()->startDrag(pt);*/
  return true;
}


BEGIN_MESSAGE_MAP(CRemoteGeomBlockPreviewDlg, CDialog)
  ON_WM_PAINT()
  ON_WM_ERASEBKGND()
  ON_WM_SIZE()
  ON_WM_DESTROY()
  ON_REGISTERED_MESSAGE(g_nRedrawMSG, OnRedraw)
END_MESSAGE_MAP()


afx_msg LRESULT CRemoteGeomBlockPreviewDlg::OnRedraw(WPARAM wParam, LPARAM lParam)
{
  /*if (m_bInRegen)
    return 1;
  m_bInRegen = true;
  m_bRegenAbort = false;*/

  CMainFrame* pMainFrame = (CMainFrame*)theApp.GetMainWnd();
  if(!regenAbort())
  {
    try
    {
      if(m_pDevice.get())
      {
        //setViewportBorderProperties();
        m_pDevice->update();
      }
    }
    catch(const OdError& e)
    {
      theApp.reportError(0, _T("Rendering aborted"), e);
      GetParent()->PostMessage(WM_CLOSE);
    }
    catch(const UserBreak&)
    {
      theApp.reportError(0, _T("Rendering aborted"), OdError(eUserBreak));
      GetParent()->PostMessage(WM_CLOSE);
    }
  }

 /* m_bRegenAbort = false;
  m_bInRegen = false;
  m_paintMode = PaintMode_Redraw;*/
  return 1;
}

void CRemoteGeomBlockPreviewDlg::OnPaint() 
{
  //m_bRegenAbort = true;

  //Process WM_PAINT;
  PAINTSTRUCT paintStruct;
  BeginPaint(&paintStruct);
  EndPaint(&paintStruct);

  if(g_nRedrawMSG != 0)
  {
    MSG msg;
    while(::PeekMessage(&msg, NULL, g_nRedrawMSG, g_nRedrawMSG, PM_REMOVE))
    {
      ;
    }
    PostMessage(g_nRedrawMSG);
  }  
}

BOOL CRemoteGeomBlockPreviewDlg::OnEraseBkgnd(CDC* /*pDC*/) 
{
  return TRUE;
}

void CRemoteGeomBlockPreviewDlg::OnSize(UINT nType, int cx, int cy) 
{
  CDialog::OnSize(nType, cx, cy);
  if(cx && cy)
  {
    if(m_pDevice.get())
    {
      CRect rc;
      GetClientRect(rc);
      OdGsDCRect outputRect(OdGsDCPoint(rc.left, rc.bottom), OdGsDCPoint(rc.right, rc.top));
      m_pDevice->onSize(outputRect);
    }
  }
}

void CRemoteGeomBlockPreviewDlg::OnDestroy() 
{
  //m_editor.uninitialize();

  DestroyDevice();

  ::ReleaseDC(m_hWnd, m_hWindowDC);
  m_hWindowDC = 0;
  
  CDialog::OnDestroy();  
}

OdGsView* CRemoteGeomBlockPreviewDlg::getActiveView()
{
  return m_pDevice->activeView();
}

const OdGsView* CRemoteGeomBlockPreviewDlg::getActiveView() const
{
  return m_pDevice->activeView();
}

OdGsView* CRemoteGeomBlockPreviewDlg::getActiveTopView()
{
  OdGsView *pView = getActiveView();
  if (!getDatabase()->getTILEMODE())
  {
    /*OdDbLayoutPtr pLayout = m_pDevice->layoutId().openObject();
    if (pLayout->drawViewportsFirst())
    {*/
      //if (pView->device()->viewAt(0) == pView)
      //  pView = pView->device()->viewAt(pView->device()->numViews() - 1);
      OdDbObjectPtr pVpObj = getDatabase()->activeViewportId().safeOpenObject();
      OdDbAbstractViewportDataPtr pAVD(pVpObj);
      if (!pAVD.isNull() && pAVD->gsView(pVpObj))
        pView = pAVD->gsView(pVpObj);
    /*}*/
    // Transients still draw before viewports if 'drawViewportsFirst' layout mode disabled.
    // @@@TODO: if layout has disabled 'drawViewportsFirst' for transient drawables may be created separate helper view also (as for
    // enabled mode), but it must has only transients. I think this helper view may be created always (for all states and modes) in
    // user application. And works only with transient drawables, so this function (and all other activeTopView() functions in OdMfcApp)
    // can return pointer to it. But this solution need some work with GsLayoutManager also.
  }
  return pView;
}

const OdGsView* CRemoteGeomBlockPreviewDlg::getActiveTopView() const
{
  const OdGsView *pView = getActiveView();
  if (!getDatabase()->getTILEMODE())
  {
    /*OdDbLayoutPtr pLayout = m_pDevice->layoutId().openObject();
    if (pLayout->drawViewportsFirst())
    {*/
      //if (pView->device()->viewAt(0) == pView)
      //  pView = pView->device()->viewAt(pView->device()->numViews() - 1);
      OdDbObjectPtr pVpObj = getDatabase()->activeViewportId().safeOpenObject();
      OdDbAbstractViewportDataPtr pAVD(pVpObj);
      if (!pAVD.isNull() && pAVD->gsView(pVpObj))
        pView = pAVD->gsView(pVpObj);
    /*}*/
  }
  return pView;
}

void CRemoteGeomBlockPreviewDlg::propagateActiveViewChanges() const
{
  // @@@ probably move this functionality into GsLayoutHelper's?
  OdGsViewPtr pView = getActiveView();
  OdGsClientViewInfo viewInfo;
  pView->clientViewInfo(viewInfo);
  OdRxObjectPtr pObj = OdDbObjectId(viewInfo.viewportObjectId).openObject(OdDb::kForWrite);
  OdAbstractViewPEPtr pVp(pObj);
  if (!pVp.isNull())
  {
    OdGePoint3d ptTarget(pView->target());
    OdGeVector3d vecDir(pView->position() - ptTarget);
    OdGeVector3d vecUp(pView->upVector());
    double dFieldWidth = pView->fieldWidth(), dFieldHeight = pView->fieldHeight();
    bool bPersp = pView->isPerspective();
    double dLensLength = pView->lensLength();
    if (vecDir.isZeroLength())
    {
      vecDir = pView->viewingMatrix().inverse().getCsZAxis();
      if (vecDir.isZeroLength())
        vecDir = OdGeVector3d::kZAxis;
      else
        vecDir.normalize();
    }
    if (!pVp->target(pObj).isEqualTo(ptTarget) ||
        !pVp->direction(pObj).isEqualTo(vecDir) ||
        !pVp->upVector(pObj).isEqualTo(vecUp) ||
        !OdEqual(pVp->fieldWidth(pObj), dFieldWidth) || !OdEqual(pVp->fieldHeight(pObj), dFieldHeight) ||
        pVp->isPerspective(pObj) != bPersp || !OdEqual(pVp->lensLength(pObj), dLensLength))
    {
      pVp->setView(pObj, ptTarget, vecDir, vecUp, dFieldWidth, dFieldHeight, bPersp);
      pVp->setLensLength(pObj, dLensLength);
    }
    OdDb::RenderMode rm = OdDb::k2DOptimized;
    switch (pView->mode())
    {
      case OdGsView::kWireframe:
        rm = OdDb::kWireframe;
      break;
      case OdGsView::kHiddenLine:
        rm = OdDb::kHiddenLine;
      break;
      case OdGsView::kFlatShaded:
        rm = OdDb::kFlatShaded;
      break;
      case OdGsView::kGouraudShaded:
        rm = OdDb::kGouraudShaded;
      break;
      case OdGsView::kFlatShadedWithWireframe:
        rm = OdDb::kFlatShadedWithWireframe;
      break;
      case OdGsView::kGouraudShadedWithWireframe:
        rm = OdDb::kGouraudShadedWithWireframe;
      break;
    }
    if (pVp->renderMode(pObj) != rm)
      pVp->setRenderMode(pObj, rm);
    if (pVp->visualStyle(pObj) != OdDbObjectId(pView->visualStyle()))
      pVp->setVisualStyle(pObj, pView->visualStyle());
  }
}
