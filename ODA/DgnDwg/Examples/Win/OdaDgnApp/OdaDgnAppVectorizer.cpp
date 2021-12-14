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
// OdaDgnAppVectorizer.cpp : implementation file
//
#include "stdafx.h"

#include <winspool.h>
#include "OdRound.h"
#include "RxVariantValue.h"
#include "Gi/GiDrawableImpl.h"
#include "Gi/GiRasterImage.h"
#include "Gi/GiWorldDraw.h"
#include "Gs/Gs.h"
#include "Gs/GsBaseVectorizer.h"
#include "Gs/GsModel.h"
#include "Gs/GsSelectionReactor.h"
#include "Ge/GeLineSeg2d.h"
#include "MainFrm.h"
#include "OdaDgnApp.h"
#include "OdaDgnAppDoc.h"
#include "OdaDgnAppVectorizer.h"
#include "OdPlatformStreamer.h"
#include "Ps/PlotStyles.h"
#include "OdaDgnAppVectorizer.h"
#include "DynamicLinker.h"
#include "Gi/GiFastExtCalc.h"
#include "ViewFlagsDlg.h"


#include <afxpriv.h>
#include <afxodlgs.h>
#include "ChildFrm.h"
#include "winuser.h"

#include "DgGsManager.h"

#include "SaveState.h"

#include "../ExRender/BmpTilesGen.h"

#include "MemoryStream.h"

typedef OdArray<ODCOLORREF, OdMemoryAllocator<ODCOLORREF> > ODGSPALETTE;

extern COdaDgnApp theApp;

// Default printer settings

COdaDgnAppPrintSettings::COdaDgnAppPrintSettings()
{
  m_uRenderMode      = OdGsView::k2DOptimized;
  m_bPlotToBMP       = true;
  m_strPrinterDriver = OdString::kEmpty;
  m_clrBgColor       = ODRGB(255,255,255);
  m_uPlotRotation    = COdaDgnAppPrintSettings::k0degrees;
  m_uPlotType        = COdaDgnAppPrintSettings::kView;
  m_dPaperWidth      = 297.;
  m_dPaperHeight     = 210.;
  m_dLeftMargin      = 7.5;
  m_dRightMargin     = 7.5;
  m_dTopMargin       = 20.;
  m_dBottomMargin    = 20.;
  m_dPlotOffsetX     = 0.;
  m_dPlotOffsetY     = 0.;
  m_dScale           = 1.0;
  m_bCentered        = true;
}

/////////////////////////////////////////////////////////////////////////////
// COdaDgnAppVectorizer

IMPLEMENT_DYNCREATE(COdaDgnAppVectorizer, CView)

COdaDgnAppVectorizer::COdaDgnAppVectorizer()
  : m_hWindowDC(0)
  , m_bRegenAbort(false)
  , m_mode(kQuiescent)
  , m_pTracker(0)
  , m_bTrackerHasDrawables(false)
  , m_hCursor(0)
  , m_pBasePt(0)
  , m_bPsOverall(false)
  , m_bMaskClip(false)
  , m_bBorderClip(false)
  //, m_currentOperation( this )    // MKU 02/01/10 eliminated warning C4355 'this' : used in base member initializer list
{
}

COdaDgnAppVectorizer::~COdaDgnAppVectorizer()
{
}


BEGIN_MESSAGE_MAP(COdaDgnAppVectorizer, CView)
  //{{AFX_MSG_MAP(COdaDgnAppVectorizer)
  ON_WM_PAINT()
  ON_WM_ERASEBKGND()
  ON_WM_SIZE()
  ON_WM_DESTROY()
  ON_WM_CREATE()
  ON_WM_RBUTTONUP()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEWHEEL()
  ON_WM_MOUSEMOVE()
  ON_WM_RBUTTONDBLCLK()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_CHAR()
  ON_WM_KEYDOWN()
  ON_WM_SETFOCUS()
  //ON_COMMAND(ID_REFRESH, OnRefresh)
  ON_COMMAND(ID_VIEWER_REGEN, OnViewerRegen)
  ON_COMMAND(ID_VIEWER_VPREGEN, OnViewerVpregen)
  ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
  ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
  ON_COMMAND(ID_VIEW_ZOOMEXTENTS, OnViewZoomextents)
  ON_COMMAND(ID_VIEW_ZOOM_WINDOW, OnViewZoomWindow)
  ON_COMMAND(ID_CREATEFENCE_BTN, OnCreateFence)
  ON_COMMAND(ID_CREATEFENCE_CIRCLE, OnCreateFenceCircle)

  ON_COMMAND(ID_VIEW_PAN, OnViewPan)
  ON_COMMAND(ID_VIEW_3DORBIT, OnView3dOrbit)
  ON_COMMAND(ID_VIEWFLAGS, OnViewPresentation)
  ON_UPDATE_COMMAND_UI(ID_VIEW_PAN, OnUpdateViewPan)
  ON_UPDATE_COMMAND_UI(ID_VIEW_3DORBIT, OnUpdateView3dOrbit)

  ON_UPDATE_COMMAND_UI(ID_VIEWER_REGEN, OnUpdateViewerRegen)
  ON_UPDATE_COMMAND_UI(ID_2DWIREFRAME, OnUpdate2dwireframe)
  ON_UPDATE_COMMAND_UI(ID_3DWIREFRAME, OnUpdate3dwireframe)
  ON_UPDATE_COMMAND_UI(ID_HIDDEN, OnUpdateHidden)
  ON_UPDATE_COMMAND_UI(ID_FLAT_SHADED, OnUpdateFlatShaded)
  ON_UPDATE_COMMAND_UI(ID_GOURAUD_SHADED, OnUpdateGouraudShaded)
  ON_UPDATE_COMMAND_UI(ID_FLAT_SHADED_WITH_EDGES, OnUpdateFlatShadedWithEdges)
  ON_UPDATE_COMMAND_UI(ID_GOURAUD_SHADED_EDGES_ON, OnUpdateGouraudShadedEdgesOn)

  ON_COMMAND_RANGE(ID_2DWIREFRAME, ID_GOURAUD_SHADED_EDGES_ON, OnSetRenderMode)
	
  ON_COMMAND_RANGE(ID_XY_PLANE, ID_BACK, On3DViews)
  ON_COMMAND_RANGE(ID_SW_ISOMETRIC, ID_NW_ISOMETRIC, On3DViews)
  
  ON_UPDATE_COMMAND_UI(ID_VIEWER_REGEN, OnUpdateViewerRegen)

  ON_COMMAND_RANGE(ID_3DVIEWS_TOP, ID_3DVIEWS_BACK, On3DViews)
  ON_COMMAND_RANGE(ID_3DVIEWS_SW, ID_3DVIEWS_NW, On3DViews)

  ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
  //}}AFX_MSG_MAP
  ON_COMMAND(ID_CLIPWITH_MASK, &COdaDgnAppVectorizer::OnClipwithMask)
END_MESSAGE_MAP()

void COdaDgnAppVectorizer::On3DViews(UINT nID)
{
	OdDgExEditorObject::_3DViewType type;
	if (nID <= ID_3DVIEWS_NW && nID >= ID_3DVIEWS_TOP)
		type = (OdDgExEditorObject::_3DViewType)(nID - ID_3DVIEWS_TOP + OdDgExEditorObject::k3DViewTop);
	else if (nID <= ID_NW_ISOMETRIC && nID >= ID_XY_PLANE)
		type = (OdDgExEditorObject::_3DViewType)(nID - ID_XY_PLANE + OdDgExEditorObject::k3DViewTop);
	m_editor.set3DView(type);
  sinchronizeWithDgView(m_editor.activeView());
	OnViewZoomextents();
}

COdaDgnAppDoc* COdaDgnAppVectorizer::GetDocument()
{
  ODA_ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COdaDgnAppDoc)));
  return (COdaDgnAppDoc*)m_pDocument;
}

/////////////////////////////////////////////////////////////////////////////
// COdaDgnAppVectorizer drawing

void COdaDgnAppVectorizer::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////

void COdaDgnAppVectorizer::OnSetFocus( CWnd* pOldWnd )
{
  CView::OnSetFocus( pOldWnd );

  COdaDgnAppDoc* pDoc = static_cast<COdaDgnAppDoc*>(GetDocument());
  pDoc->setVectorizer(this);
}

/////////////////////////////////////////////////////////////////////////////

void COdaDgnAppVectorizer::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
  CView::OnBeginPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////

void generateTiles(HDC hdc, RECT& drawrc, OdGsDevice* pBmpDevice, OdUInt32 nTileWidth, OdUInt32 nTileHeight)
{
  CRect destrc = drawrc;
  destrc.NormalizeRect();
  OdGsDCRect step(0,0,0,0);
  OdGsDCRect rc(drawrc.left, drawrc.right, drawrc.bottom, drawrc.top);
  OdUInt32 nWidth = abs(rc.m_max.x-rc.m_min.x);
  rc.m_max.x -= rc.m_min.x;
  if(rc.m_max.x < 0)
  {
    rc.m_min.x = -rc.m_max.x;
    rc.m_max.x = 0;
    step.m_min.x = nTileWidth;
  }
  else
  {
    rc.m_min.x = 0;
    step.m_max.x = nTileWidth;
  }
  OdUInt32 nHeight = abs(rc.m_max.y-rc.m_min.y);
  rc.m_max.y -= rc.m_min.y;
  if(rc.m_max.y < 0)
  {
    rc.m_min.y = -rc.m_max.y;
    rc.m_max.y = 0;
    step.m_min.y = nTileHeight;
  }
  else
  {
    rc.m_min.y = 0;
    step.m_max.y = nTileHeight;
  }

  OdInt32 m = nWidth / nTileWidth + (nWidth % nTileWidth ? 1 : 0);
  OdInt32 n = nHeight / nTileHeight + (nHeight % nTileHeight ? 1 : 0);

  BmpTilesGen tilesGen(pBmpDevice, rc);
  pBmpDevice->onSize(rc);

  OdGiRasterImagePtr pImg;

  int dx = (step.m_max.x - step.m_min.x);
  int dy = (step.m_max.y - step.m_min.y);

  int dx2 = m > 1 ? dx / abs(dx) * 8 : 0;
  int dy2 = n > 1 ? dy / abs(dy) * 8 : 0;

  BITMAPINFO bmi;
  bmi.bmiHeader.biBitCount = (WORD)24;
  bmi.bmiHeader.biWidth  = nTileWidth  + abs(dx2) * 2;
  bmi.bmiHeader.biHeight = nTileHeight;
  bmi.bmiHeader.biClrImportant = 0;
  bmi.bmiHeader.biClrUsed = 0;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biSizeImage = 0;
  bmi.bmiHeader.biXPelsPerMeter = 0;
  bmi.bmiHeader.biYPelsPerMeter = 0;

  HDC bmpDC = CreateCompatibleDC(hdc);
  if(bmpDC)
  {
    void* pBuf;
    HBITMAP hBmp = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &pBuf, NULL, 0);
    if(hBmp)
    {
      HBITMAP hOld = (HBITMAP)SelectObject(bmpDC, hBmp);
      for(OdInt32 i=0; i < m; ++i)
      {
        for(OdInt32 j=0; j < n; ++j)
        {
          int minx = rc.m_min.x + i * dx;
          int maxx = minx + dx;
          int miny = rc.m_min.y + j * dy;
          int maxy = miny + dy;

          // render wider then a tile area to reduce gaps in lines.
          pImg = tilesGen.regenTile(OdGsDCRect(minx - dx2, maxx + dx2, miny - dy2, maxy + dy2));

          pImg->scanLines((OdUInt8*)pBuf, 0, nTileHeight);
          BitBlt(hdc, destrc.left + odmin(minx, maxx), destrc.top + odmin(miny, maxy),
            nTileWidth, nTileHeight, bmpDC, abs(dx2), 0, SRCCOPY);
        }
      }
      SelectObject(bmpDC, hOld);
      DeleteObject(hBmp);
    }
    DeleteDC(bmpDC);
  }
}

/////////////////////////////////////////////////////////////////////////////
#if (TD_VERSION >= 31000 || !defined(TD_VERSION))

void COdaDgnAppVectorizer::preparePlotstyles()
{
  if(isPlotGeneration())
  {
    OdDgHostAppServices* pHostAppServices = getDatabase()->appServices();
    OdString pssFile( pHostAppServices->getMS_PENTABLE() );
    if(!pssFile.isEmpty())
    {
      OdString testpath = pHostAppServices->findFile(pssFile);

      if(!testpath.isEmpty())
      {
        loadPlotStyle(testpath);
      }
    }
  }
}

#else

void COdaDgnAppVectorizer::preparePlotstyles( const OdString& strPlotStyleFileName )
{
  if(isPlotGeneration())
  {
    OdString pssFile = strPlotStyleFileName;

    OdDgHostAppServices* pHostAppServices = getDatabase()->appServices();

    if(!pssFile.isEmpty())
    {
      OdString testpath = pHostAppServices->findFile(pssFile);

      if(!testpath.isEmpty())
      {
        OdStreamBufPtr pStream = OdMemoryStream::createNew();

        OdAnsiString strAnsiFileName = testpath;

        pStream->putBytes( strAnsiFileName.c_str(), strAnsiFileName.getLength() );
        pStream->putByte(0);
        pStream->seek(0,OdDb::kSeekFromStart);

        loadPlotStyleTable(pStream);
      }
    }
  }
}

#endif

void COdaDgnAppVectorizer::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
  OdDgDatabase* pDb = getDatabase();

  if( !pDb )
  {
    ODA_FAIL_ONCE();
    return;
  }

  OdDgElementId idModel = pDb->getActiveModelId();

  if( idModel.isNull() )
  {
    ODA_FAIL_ONCE();
    return;
  }

  OdDgModelPtr pModel = idModel.openObject(OdDg::kForRead);

  OdDgElementId idDgView = m_idView;

  if( m_idView.isNull() )
  {
    OdDgViewGroupTablePtr pViewGroupTable = pDb->getViewGroupTable( OdDg::kForRead );

    if( !pViewGroupTable.isNull() )
    {
      OdDgElementIteratorPtr pViewGroupIter = pViewGroupTable->createIterator();

      for(; !pViewGroupIter->done(); pViewGroupIter->step() )
      {
        OdDgViewGroupPtr pViewGroup = pViewGroupIter->item().openObject( OdDg::kForRead );

        if( !pViewGroup.isNull() && pViewGroup->getModelId() == idModel )
        {
          OdDgElementIteratorPtr pViewIter = pViewGroup->createIterator();

          for(; !pViewIter->done(); pViewIter->step() )
          {
            OdDgViewPtr pView = pViewIter->item().openObject(OdDg::kForRead);
            idDgView = pView->elementId();
            break;
          }
        }
      }
    }
  }

  if( idDgView.isNull() )
  {
    ODA_FAIL_ONCE();
    return;
  }

  bool bShowFence = false;

  COdaDgnAppDoc* pDoc = GetDocument();

  if( pDoc )
    bShowFence = pDoc->hideFence();

  setPlotGeneration(true);

  struct KeepPrevGiContextParams
  {
    OdGiContextForDgDatabaseToPlotStyleSupport *m_pGiCtx;
    bool m_bPrevGsModelState;
    ODCOLORREF m_crPrevBkgndColor;

    KeepPrevGiContextParams(OdGiContextForDgDatabaseToPlotStyleSupport *pGiCtx) : m_pGiCtx(pGiCtx)
    {
      m_bPrevGsModelState = pGiCtx->useGsModel();
      m_crPrevBkgndColor = pGiCtx->paletteBackground();
    }
    ~KeepPrevGiContextParams()
    {
      m_pGiCtx->enableGsModel(m_bPrevGsModelState);
      m_pGiCtx->setPaletteBackground(m_crPrevBkgndColor);
    }
  } prevGiContextParams(this);

  // To get paper size of selected printer
  //  and to get properties (scale, offset) from PlotSettings
  //  to set using them OverAll View;

  bool bPlotViaBMP = m_printSettings.m_bPlotToBMP;

  if(m_pPrinterDevice.isNull())
  {
    OdGsModulePtr pGsModule;

    if( bPlotViaBMP )
    {
      pGsModule = ::odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName);
    }

    OdGsDevicePtr pPrinterDevice;

    if( bPlotViaBMP && pGsModule.get() )
    {
      pPrinterDevice = pGsModule->createBitmapDevice();
    }
    else
    {
      bPlotViaBMP = false;

      pGsModule = ::odrxDynamicLinker()->loadModule(OdWinGDIModuleName);

      if(pGsModule.get())
      {
        pPrinterDevice = pGsModule->createDevice();
      }
    }

    if (pPrinterDevice.get())
    {
      enableGsModel(false);

      if( !pPrinterDevice->properties().isNull() && pPrinterDevice->properties()->has(OD_T("EnableSoftwareHLR")) )
      {
        pPrinterDevice->properties()->putAt(OD_T("EnableSoftwareHLR"), OdRxVariantValue(true));
      }

      if (/*bPlotViaBMP &&*/ pPrinterDevice->properties()->has(OD_T("DPI"))) // #9633 (1)
      {
        pPrinterDevice->properties()->putAt(OD_T("DPI"), OdRxVariantValue((OdUInt32)odmin(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY))));
      }

      preparePlotstyles();

      m_pPrinterDevice = OdGsDeviceForDgModel::setupModelView( idModel, idDgView, pPrinterDevice, this);

      const ODCOLORREF* pPalette = OdDgColorTable::defaultPalette();

      OdDgColorTablePtr pColorTable = pDb->getColorTable(OdDg::kForRead);

      if( !pColorTable.isNull() )
      {
        pPalette = pColorTable->palette();
      }

      ODCOLORREF* pColorPalette = new COLORREF[256];
      memcpy( pColorPalette, pPalette, 256*sizeof(ODCOLORREF));
      pColorPalette[255] = m_printSettings.m_clrBgColor;
      //pColorPalette[0] = 0;
      OdDgColorTable::correctPaletteForWhiteBackground(pColorPalette);

      m_pPrinterDevice->setLogicalPalette(pColorPalette, 256);
      m_pPrinterDevice->setBackgroundColor( m_printSettings.m_clrBgColor);
      setPaletteBackground(m_pPrinterDevice->getBackgroundColor());

      delete[] pColorPalette;

      bool bCopyViewProperties = false;

      OdGsView* pActiveView = getActiveView();

      for( OdInt32 l = 0; l < m_pPrinterDevice->numViews(); l++ )
      {
        pActiveView->viewParameters( m_pPrinterDevice->viewAt(l) );

        // disable front and back clip
        m_pPrinterDevice->viewAt(l)->setEnableFrontClip(false);
        m_pPrinterDevice->viewAt(l)->setEnableBackClip(false);

        m_pPrinterDevice->viewAt(l)->setMode( m_printSettings.m_uRenderMode );
      }
    }
  }
  else
  {
    bPlotViaBMP = m_pPrinterDevice->properties()->has(OD_T("RasterImage"));
  }

  if( m_pPrinterDevice.get() )
  {
    bool bPrint90 = false;
    bool bPrint0 = false;
    bool bPrint180 = false;
    bool bPrint270 = false;

    // Get printer paper info
    double dPrinterWidth        = pDC->GetDeviceCaps(PHYSICALWIDTH);
    double dPrinterHeight       = pDC->GetDeviceCaps(PHYSICALHEIGHT);
    double dPrinterLeftMargin   = pDC->GetDeviceCaps(PHYSICALOFFSETX);
    double dPrinterTopMargin    = pDC->GetDeviceCaps(PHYSICALOFFSETY);
    double dPrinterMarginWidth  = pDC->GetDeviceCaps(HORZRES);
    double dPrinterMarginHeight = pDC->GetDeviceCaps(VERTRES);
    double dLogPixelX           = pDC->GetDeviceCaps(LOGPIXELSX);
    double dLogPixelY           = pDC->GetDeviceCaps(LOGPIXELSY);
    //    double dPrinterRightMargin  = dPrinterWidth  - dPrinterMarginWidth  - dPrinterLeftMargin;
    double dPrinterBottomMargin = dPrinterHeight - dPrinterMarginHeight - dPrinterTopMargin;
    double koeffX               = dLogPixelX / kMmPerInch;
    double koeffY               = dLogPixelY / kMmPerInch;

    bool bModel = pModel->getType() == OdDgModel::kDesignModel;

    if( (m_printSettings.m_uPlotType == COdaDgnAppPrintSettings::kSheetModel) && 
        (pModel->getType() == OdDgModel::kSheetModel)
      )
    {
      OdDgSheetModelPtr pSheetModel = pModel;

      // update m_printSettings structure by sheet model properties
    }

    bool bScaledToFit =  (m_printSettings.m_uPlotType == COdaDgnAppPrintSettings::kExtents) || 
                         (m_printSettings.m_uPlotType == COdaDgnAppPrintSettings::kSheetModel);

    bool bCentered    =  m_printSettings.m_bCentered;
    bool bMetric      =  true;
    bool bPrintLW     =  true;

    double offsetX    =  m_printSettings.m_dPlotOffsetX; // in mm
    double offsetY    =  m_printSettings.m_dPlotOffsetY; // in mm
   
    OdGePoint2d pio( m_printSettings.m_dPlotOffsetX, m_printSettings.m_dPlotOffsetX); // in mm

    double dLeftMargin   = m_printSettings.m_dLeftMargin;  // in mm
    double dRightMargin  = m_printSettings.m_dRightMargin; // in mm
    double dTopMargin    = m_printSettings.m_dTopMargin;   // in mm
    double dBottomMargin = m_printSettings.m_dBottomMargin;// in mm

    COdaDgnAppPrintSettings::OdDgPlotType plotType = m_printSettings.m_uPlotType;

    if( (plotType == COdaDgnAppPrintSettings::kSheetModel) && 
        (pModel->getType() != OdDgModel::kSheetModel )
      )
    {
      plotType = COdaDgnAppPrintSettings::kExtents;
    }

    if( !bPrintLW )
    {
      OdGsView* pTo = m_pPrinterDevice->viewAt(0);
      pTo->setLineweightToDcScale(odmax(dLogPixelX, dLogPixelY) / kMmPerInch * 0.0001 );
    }
    else
    {
      OdGsView* pTo = m_pPrinterDevice->viewAt(0);
      pTo->setLineweightToDcScale(odmax(dLogPixelX, dLogPixelY) / kMmPerInch * 0.01 );
    }

    // Apply paper rotation to paper parameters
    if( pInfo->m_bPreview )
    {
      double dTmp;

      switch( m_printSettings.m_uPlotRotation )
      {
      case COdaDgnAppPrintSettings::k90degrees:
        dTmp          = dTopMargin;
        dTopMargin    = dRightMargin;
        dRightMargin  = dBottomMargin;
        dBottomMargin = dLeftMargin;
        dLeftMargin   = dTmp;
        //std::swap(pio.x, pio.y);
        std::swap(offsetX, offsetY);
        break;
      case COdaDgnAppPrintSettings::k180degrees:
        std::swap(dBottomMargin, dTopMargin );
        std::swap(dRightMargin , dLeftMargin);
        break;
      case COdaDgnAppPrintSettings::k270degrees:
        dTmp          = dTopMargin;
        dTopMargin    = dLeftMargin;
        dLeftMargin   = dBottomMargin;
        dBottomMargin = dRightMargin;
        dRightMargin  = dTmp;
        //std::swap(pio.x, pio.y);
        std::swap(offsetX, offsetY);
        break;
      };
    }
    else // printing
    {
      COdaDgnAppPrintSettings::OdDgPlotRotation plotRotation = m_printSettings.m_uPlotRotation;

      if (plotRotation == COdaDgnAppPrintSettings::k90degrees || plotRotation == COdaDgnAppPrintSettings::k270degrees)
      {
        // 3962 fixed
        DWORD landOrientation = DeviceCapabilities(pInfo->m_pPD->GetDeviceName(), pInfo->m_pPD->GetPortName(), DC_ORIENTATION, 0, 0);

        if( landOrientation == 270 )
        {
          plotRotation = (plotRotation == COdaDgnAppPrintSettings::k90degrees) ? COdaDgnAppPrintSettings::k270degrees : COdaDgnAppPrintSettings::k90degrees;
        }
      }

      switch (plotRotation)
      {
      case COdaDgnAppPrintSettings::k0degrees:
        bPrint0 = true;
        break;
      case COdaDgnAppPrintSettings::k90degrees:
        bPrint90 = true;
        std::swap(dTopMargin, dRightMargin);
        std::swap(dBottomMargin, dLeftMargin);
        std::swap(dBottomMargin, dTopMargin);
        //std::swap(pio.x, pio.y);
        std::swap(offsetX, offsetY);
        offsetY = -offsetY;
        offsetX = -offsetX;
        break;
      case COdaDgnAppPrintSettings::k180degrees:
        bPrint180 = true;
        std::swap(dRightMargin , dLeftMargin);
        offsetY = -offsetY;
        offsetX = -offsetX;
        break;
      case COdaDgnAppPrintSettings::k270degrees:
        bPrint270 = true;
        std::swap(dTopMargin, dRightMargin);
        std::swap(dBottomMargin, dLeftMargin);
        std::swap(offsetX, offsetY);
        break;
      }
    }

    // Get scale factor
    double factor = m_printSettings.m_dScale;

    // Calculate paper drawable area using margins from layout (in mm).
    double drx1 = (- dPrinterLeftMargin / koeffX + dLeftMargin);                // in mm
    double drx2 = (drx1 + dPrinterWidth / koeffX - dLeftMargin - dRightMargin); // in mm
    double dry1 = (- dPrinterTopMargin  / koeffY + dTopMargin );                // in mm
    double dry2 = (dry1 + dPrinterHeight/ koeffY - dTopMargin - dBottomMargin); // in mm

    // Margin clip box calculation
    dTopMargin    *= koeffY ; /// in printer units
    dRightMargin  *= koeffX ;
    dBottomMargin *= koeffY ;
    dLeftMargin   *= koeffX ;

    CRgn newClipRgn; newClipRgn.CreateRectRgn(0,0,1,1);
    CRect MarginsClipBox;

    int ret = GetClipRgn(pDC->m_hDC, newClipRgn);
    bool bNullMarginsClipBox = !ret || ret && (GetLastError() != ERROR_SUCCESS);

    double dScreenFactorH, dScreenFactorW;
    if (bNullMarginsClipBox)
    { // printing way
      double x = dLeftMargin    - dPrinterLeftMargin;
      double y = dTopMargin     - dPrinterTopMargin;
      MarginsClipBox.SetRect(int(x), int(y)
        , int(x + dPrinterWidth - dLeftMargin - dRightMargin)
        , int(y + dPrinterHeight - dTopMargin - dBottomMargin));

      dScreenFactorH = dScreenFactorW = 1.;
    }
    else
    {
      newClipRgn.GetRgnBox(&MarginsClipBox);
      dScreenFactorH = double(MarginsClipBox.Height()) / dPrinterMarginHeight;
      dScreenFactorW = double(MarginsClipBox.Width())  / dPrinterMarginWidth;

      MarginsClipBox.left += LONG(( dLeftMargin - dPrinterLeftMargin) * dScreenFactorW);
      MarginsClipBox.bottom -= LONG(( dBottomMargin  - dPrinterBottomMargin ) * dScreenFactorH);

      MarginsClipBox.top = MarginsClipBox.bottom  - LONG(( dPrinterHeight - dTopMargin - dBottomMargin) * dScreenFactorH);
      MarginsClipBox.right  = MarginsClipBox.left + LONG(( dPrinterWidth - dLeftMargin - dRightMargin ) * dScreenFactorW);
    }

    // MarginsClipBox is calculated
    CRect ClipBox(MarginsClipBox);

    // Get view and viewport position, direction ...
    OdGsViewPtr pOverallView;

    OdGePoint3d viewportCenter;
    OdGePoint3d viewTarget, oldTarget;
    OdGeVector3d upV, viewDir;
    bool isPerspective;
    double viewportH, viewportW;
    OdGeMatrix3d eyeToWorld, WorldToeye;
    bool SkipClipping = false;

    OdRxObjectPtr pVObject;

    pOverallView = m_pPrinterDevice->activeView(); 

    viewTarget = pOverallView->target(); // ????

    // get info from view, viewport .... etc
    viewportCenter = pOverallView->target();       // in plotPaperUnits
    isPerspective  = pOverallView->isPerspective();
    viewportH      = pOverallView->fieldHeight();  // in plotPaperUnits
    viewportW      = pOverallView->fieldWidth();   // in plotPaperUnits

    OdGeVector3d vrDirection = pOverallView->position() - pOverallView->target();

//     if( !OdZero(vrDirection.length()) )
//     {
//       vrDirection.normalize();
//     }
//     else
//     {
//       vrDirection = OdGeVector3d::kZAxis;
//     }

    viewDir        = vrDirection;    // in plotPaperUnits
    upV            = pOverallView->upVector();     // in plotPaperUnits
    eyeToWorld     = pOverallView->worldToDeviceMatrix();
    eyeToWorld     = eyeToWorld.invert();
    WorldToeye     = pOverallView->worldToDeviceMatrix();

    bool isPlanView = /*viewTarget.isEqualTo(OdGePoint3d(0,0,0)) &&*/ viewDir.normal().isEqualTo(OdGeVector3d::kZAxis);

    oldTarget = viewTarget;

    // To set OverAll View using default settings

    // get rect of drawing to view (in plotPaperUnits)
    double fieldWidth(viewportW), fieldHeight(viewportH);

    if( (plotType ==  COdaDgnAppPrintSettings::kView) || 
        (plotType ==  COdaDgnAppPrintSettings::kFence) 
      )
    {
      viewTarget    = viewportCenter;
      fieldWidth    = viewportW / factor;
      fieldHeight   = viewportH / factor;
    }
    else if( (plotType == COdaDgnAppPrintSettings::kExtents) )
    {
      factor = 1.0;
      OdGeExtents3d extents;

      if( pModel->getGeomExtents(extents) == eOk ) // pIter also skip 'off layers'
      {
        bCentered = true;

        OdGePoint3d realPoints[ 8 ];

        OdGePoint3d ptMin = extents.minPoint(), ptMax = extents.maxPoint();

        realPoints[ 0 ] = ptMin;
        realPoints[ 1 ] = ptMax;
        realPoints[ 2 ] = OdGePoint3d( ptMin.x, ptMax.y, ptMin.z );
        realPoints[ 3 ] = OdGePoint3d( ptMax.x, ptMax.y, ptMin.z );
        realPoints[ 4 ] = OdGePoint3d( ptMax.x, ptMin.y, ptMin.z );
        realPoints[ 5 ] = OdGePoint3d( ptMin.x, ptMin.y, ptMax.z );
        realPoints[ 6 ] = OdGePoint3d( ptMin.x, ptMax.y, ptMax.z );
        realPoints[ 7 ] = OdGePoint3d( ptMax.x, ptMin.y, ptMax.z );

        OdGePoint2d   relativePoints[ 8 ];
        OdGePoint3d   position = pOverallView->position(), target = viewTarget;
        OdGeVector3d  n = upV, m = ( target - position ).crossProduct( n ).normal();

        OdUInt32 k;

        for( k = 0; k < 8; k++ )
        {
          relativePoints[ k ].x = ( realPoints[ k ] - position ).dotProduct( m );
          relativePoints[ k ].y = ( realPoints[ k ] - position ).dotProduct( n );
        }

        OdGeVector2d min, medium, max;

        max.x = relativePoints[ 0 ].x;
        max.y = relativePoints[ 0 ].y;

        min = max;

        for( k = 1; k < 8; k++ )
        {
          if( min.x > relativePoints[ k ].x ) 
          {
            min.x = relativePoints[ k ].x;
          }

          if( max.x < relativePoints[ k ].x )
          {
            max.x = relativePoints[ k ].x;
          }

          if( min.y > relativePoints[ k ].y ) 
          {
            min.y = relativePoints[ k ].y;
          }

          if( max.y < relativePoints[ k ].y )
          {
            max.y = relativePoints[ k ].y;
          }
        }

        medium = ( max + min ) / 2.;

        if( min.x < max.x || min.y < max.y )
        {
          OdGePoint3d ptNewPos = position + m * medium.x + n * medium.y;
          OdGePoint3d ptNewTar = target + m * medium.x + n * medium.y;
          OdGeVector3d vrDir = ptNewTar - ptNewPos;
          double dirLen = vrDir.length();

          if( OdZero(dirLen, 10e-8) )
          {
            ptNewTar += OdGeVector3d(0, 0, -1);
            vrDir = ptNewTar - ptNewPos;
            dirLen = vrDir.length();
          }

          double targetLen = ptNewTar.asVector().length();

          fieldWidth  = (max.x - min.x)*1.05;
          fieldHeight = (max.y - min.y)*1.05;

          viewTarget = ptNewTar;

          if(targetLen > 1. && OdZero(dirLen / targetLen, 10e-8))
          {
            viewDir = ptNewPos / dirLen * targetLen - ptNewTar;
          }
          else
          {
            viewDir = ptNewPos - ptNewTar;
          }
        }
      }
    }
    else if( plotType == COdaDgnAppPrintSettings::kSheetModel )
    {
      factor    = 1.0;
      bCentered = true;

      OdDgSheetModelPtr pSheetModel = pModel;

      double      dAnnotationScale = pSheetModel->getAnnotationScale();
      OdGePoint2d ptOffset         = pSheetModel->getSheetOffset();
      double      dSheetWidth      = pSheetModel->getSheetWidth();
      double      dSheetHeight     = pSheetModel->getSheetHeight();
      double      dSheetLMargin    = pSheetModel->getSheetLeftMargin();
      double      dSheetRMargin    = pSheetModel->getSheetRightMargin();
      double      dSheetTMargin    = pSheetModel->getSheetTopMargin();
      double      dSheetBMargin    = pSheetModel->getSheetBottomMargin();
      double      dSheetAngle      = pSheetModel->getSheetRotation();

      double      dFullSheetWidth  = (dSheetWidth  + dSheetLMargin + dSheetRMargin)*dAnnotationScale;
      double      dFullSheetHeight = (dSheetHeight + dSheetTMargin + dSheetBMargin)*dAnnotationScale;

      OdGeVector3d vrXDir = OdGeVector3d::kXAxis;
      OdGeVector3d vrYDir = OdGeVector3d::kYAxis;

      vrXDir = vrXDir.rotateBy( dSheetAngle, OdGeVector3d::kZAxis );
      vrYDir = vrYDir.rotateBy( dSheetAngle, OdGeVector3d::kZAxis );

      OdGePoint3dArray points;
      points.resize(4);

      points[0].x = ptOffset.x;
      points[0].y = ptOffset.y;
      points[0].z = 0;
      points[1] = points[0] + vrXDir * dFullSheetWidth;
      points[1] -= vrXDir * dSheetRMargin * dAnnotationScale;
      points[1] += vrYDir * dSheetBMargin * dAnnotationScale;
      points[2] = points[1] + vrYDir * dFullSheetHeight;
      points[2] -= vrXDir * dSheetRMargin * dAnnotationScale;
      points[2] -= vrYDir * dSheetTMargin * dAnnotationScale;
      points[3] = points[2] - vrXDir * dFullSheetWidth;
      points[3] += vrXDir * dSheetLMargin * dAnnotationScale;
      points[3] -= vrYDir * dSheetTMargin * dAnnotationScale;

      //

      OdGePoint2d   relativePoints[ 4 ];
      OdGePoint3d   position = pOverallView->position(), target = viewTarget;
      OdGeVector3d  n = upV, m = ( target - position ).crossProduct( n ).normal();

      OdUInt32 k;

      for( k = 0; k < 4; k++ )
      {
        relativePoints[ k ].x = ( points[ k ] - position ).dotProduct( m );
        relativePoints[ k ].y = ( points[ k ] - position ).dotProduct( n );
      }

      OdGeVector2d min, medium, max;

      max.x = relativePoints[ 0 ].x;
      max.y = relativePoints[ 0 ].y;

      min = max;

      for( k = 1; k < 4; k++ )
      {
        if( min.x > relativePoints[ k ].x ) 
        {
          min.x = relativePoints[ k ].x;
        }

        if( max.x < relativePoints[ k ].x )
        {
          max.x = relativePoints[ k ].x;
        }

        if( min.y > relativePoints[ k ].y ) 
        {
          min.y = relativePoints[ k ].y;
        }

        if( max.y < relativePoints[ k ].y )
        {
          max.y = relativePoints[ k ].y;
        }
      }

      medium = ( max + min ) / 2.;

      if( min.x < max.x || min.y < max.y )
      {
        OdGePoint3d ptNewPos = position + m * medium.x + n * medium.y;
        OdGePoint3d ptNewTar = target + m * medium.x + n * medium.y;
        OdGeVector3d vrDir = ptNewTar - ptNewPos;
        double dirLen = vrDir.length();

        if( OdZero(dirLen, 10e-8) )
        {
          ptNewTar += OdGeVector3d(0, 0, -1);
          vrDir = ptNewTar - ptNewPos;
          dirLen = vrDir.length();
        }

        double targetLen = ptNewTar.asVector().length();

        fieldWidth  = relativePoints[0].distanceTo(relativePoints[1]);
        fieldHeight = relativePoints[0].distanceTo(relativePoints[3]);

        viewTarget = ptNewTar;

        if(targetLen > 1. && OdZero(dirLen / targetLen, 10e-8))
        {
          viewDir = ptNewPos / dirLen * targetLen - ptNewTar;
        }
        else
        {
          viewDir = ptNewPos - ptNewTar;
        }
      }
    }

    viewTarget = viewTarget.orthoProject(OdGePlane(oldTarget, viewDir));

    // in plotpaper units
    pOverallView->setView(viewTarget + viewDir, viewTarget, upV, fieldWidth, fieldHeight, isPerspective ? OdGsView::kPerspective : OdGsView::kParallel);

    if (!bMetric)
    {
      fieldWidth *= kMmPerInch;
      fieldHeight *= kMmPerInch;
    }

    double dFactorX = 0;
    double dFactorY = 0;

    if( bScaledToFit ) // Scale factor can be stored in layout, but preview recalculate pIter if bScaledToFit = true.
    {                  // Some times stored factor isn't correct, due to autocad isn't update pIter before saving.
      factor *= odmin( (drx2 - drx1) / fieldWidth, (dry2 - dry1) / fieldHeight );

      dFactorX = factor;
      dFactorY = factor;
    }
    else
    {
      dFactorX = (drx2 - drx1) / fieldWidth;
      dFactorY = (dry2 - dry1) / fieldHeight;
    }

    if( bCentered )    // Offset also can be incorrectly saved.
    {
      offsetX = ( (drx2 - drx1) - fieldWidth  * dFactorX ) / 2. + offsetX;
      offsetY = ( (dry2 - dry1) - fieldHeight * dFactorY ) / 2. + offsetY;

      if (bPrint90 || bPrint180)
      {
        offsetY = -offsetY;
        offsetX = -offsetX;
      }
    }

    if (bPrint180 || bPrint90)
    {
      drx1 = drx2 - fieldWidth  * dFactorX;
      dry2 = dry1 + fieldHeight * dFactorY;
    }
    else if (bPrint0 || bPrint270)
    {
      drx2 = drx1 + fieldWidth  * dFactorX;
      dry1 = dry2 - fieldHeight * dFactorY;
    }
    else // preview
    {
      drx2 = drx1 + fieldWidth  * dFactorX;
      dry1 = dry2 - fieldHeight * dFactorY;
    }

    if (!SkipClipping)
    {
      if( plotType == COdaDgnAppPrintSettings::kView )
      {
        if( dFactorX < dFactorY )
        {
          // Correct y coordinates of clip box.
          double dYOffset = ((ClipBox.bottom - ClipBox.top) - (ClipBox.right - ClipBox.left) / fieldWidth * fieldHeight) / 2.0;
          ClipBox.top    += OdUInt32(fabs(dYOffset));
          ClipBox.bottom -= OdUInt32(fabs(dYOffset));
          dry1 += dYOffset / (koeffY * dScreenFactorH);
          dry2 -= dYOffset / (koeffY * dScreenFactorH);
        }
        else
        {
          // Correct x coordinates of clip box.
          double dXOffset = ((ClipBox.right - ClipBox.left) - (ClipBox.bottom - ClipBox.top) / fieldHeight * fieldWidth) / 2.0;
          ClipBox.left    += OdUInt32(fabs(dXOffset));
          ClipBox.right   -= OdUInt32(fabs(dXOffset));
          drx1 += dXOffset / (koeffX * dScreenFactorW);
          drx2 -= dXOffset / (koeffX * dScreenFactorW);
        }
      }

      if (bPrint180 || bPrint90)
      {
        ClipBox.left   = LONG(ClipBox.right   - (drx2-drx1)* koeffX * dScreenFactorW);
        ClipBox.bottom = LONG(ClipBox.top + (dry2-dry1)* koeffY * dScreenFactorH);
      }
      else if (bPrint0 || bPrint270)
      {
        ClipBox.right  = LONG(ClipBox.left   + (drx2-drx1)* koeffX * dScreenFactorW);
        ClipBox.top    = LONG(ClipBox.bottom - (dry2-dry1)* koeffY * dScreenFactorH);
      }
      else // preview
      {
        ClipBox.right  = LONG(ClipBox.left   + (drx2-drx1)* koeffX * dScreenFactorW);
        ClipBox.top    = LONG(ClipBox.bottom - (dry2-dry1)* koeffY * dScreenFactorH);
      }
      ClipBox.OffsetRect(int(offsetX * koeffX * dScreenFactorW), int(-offsetY * koeffY * dScreenFactorH));
    }

    pOverallView->setViewport(OdGePoint2d(0,0), OdGePoint2d(1,1));

    CRect ResultClipBox;
    ResultClipBox.IntersectRect(&MarginsClipBox, &ClipBox);

    // Apply clip region to screen
    CRgn newClip;
    newClip.CreateRectRgnIndirect(&ResultClipBox);
    pDC->SelectClipRgn(&newClip);

    // Calculate viewport rect in printer units
    long x1 = long( ( offsetX + drx1) * koeffX );
    long x2 = long( ( offsetX + drx2) * koeffX );
    long y1 = long( (-offsetY + dry1) * koeffY );
    long y2 = long( (-offsetY + dry2) * koeffY );

    OdGsDCRect viewportRect;
    if (bPrint180 || bPrint90)
    {
      //viewportRect = OdGsDCRect( x1, x2, y1, y2 );
      viewportRect = OdGsDCRect( x2, x1, y1, y2 );
    }
    else if (bPrint0 || bPrint270)
    {
      //viewportRect = OdGsDCRect( x2, x1, y2, y1 );
      viewportRect = OdGsDCRect( x1, x2, y2, y1 );
    }
    else // preview
    {
      //viewportRect = OdGsDCRect( x2, x1, y2, y1 );
      viewportRect = OdGsDCRect( x1, x2, y2, y1 );
    }

    if(!bPlotViaBMP)
    {
      m_pPrinterDevice->onSize(viewportRect);

      m_pPrinterDevice->properties()->putAt(_T("WindowHDC"), OdRxVariantValue((OdIntPtr)pDC->m_hDC));

      m_pPrinterDevice->update(0);
    }
    else
    {
      CRect rc(viewportRect.m_min.x, viewportRect.m_max.y, viewportRect.m_max.x, viewportRect.m_min.y);
      generateTiles( pDC->m_hDC, rc, m_pPrinterDevice, 1000, 1000 );
    }
  }
  else
  {
    AfxMessageBox(_T("Can't initialize GS for printing..."));
  }

  setPlotGeneration(false);

  CView::OnPrint(pDC, pInfo);

  if( pDoc && bShowFence)
    pDoc->showFence();
}

/////////////////////////////////////////////////////////////////////////////

void COdaDgnAppVectorizer::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
  CView::OnEndPrinting(pDC, pInfo);

  m_pPrinterDevice.release();
}

/////////////////////////////////////////////////////////////////////////////

BOOL COdaDgnAppVectorizer::OnPreparePrinting(CPrintInfo* pInfo) 
{
  // Get PlotInfo for active layout (Printer/Plotter name, Paper size...)
	CWinApp* pApp = AfxGetApp();

  bool bPrinterWasSet(false);

  OdString strPrinterName(_T("Model layout"));
  strPrinterName = m_printSettings.m_strPrinterDriver;

  if( strPrinterName.isEmpty() )
  {
    strPrinterName = L"none";
  }

  double dPaperWidth, dPaperHeight;
  dPaperWidth = m_printSettings.m_dPaperWidth;
  dPaperHeight = m_printSettings.m_dPaperHeight;

//   if( m_printSettings.m_uPlotType == COdaDgnAppPrintSettings::kSheetModel )
//   {
//     OdDgDatabase* pDb = getDatabase();
// 
//     if( pDb )
//     {
//       OdDgElementId idModel = pDb->getActiveModelId();
// 
//       if( !idModel.isNull() )
//       {
// 
//         OdDgModelPtr pModel = idModel.openObject(OdDg::kForRead);
// 
//         if( pModel->getType() == OdDgModel::kSheetModel )
//         {
//           OdDgSheetModelPtr pSheetModel = pModel;
// 
//           if( !pSheetModel.isNull() )
//           {
//             m_printSettings.m_dPaperWidth   = pSheetModel->getSheetWidth();
//             m_printSettings.m_dPaperHeight  = pSheetModel->getSheetHeight();
//             m_printSettings.m_dLeftMargin   = pSheetModel->getSheetLeftMargin();
//             m_printSettings.m_dRightMargin  = pSheetModel->getSheetRightMargin();
//             m_printSettings.m_dTopMargin    = pSheetModel->getSheetTopMargin();
//             m_printSettings.m_dBottomMargin = pSheetModel->getSheetBottomMargin();
//           }
//         }
//       }
//     }
//   }
      
  COdaDgnAppPrintSettings::OdDgPlotRotation rotation = m_printSettings.m_uPlotRotation;

  if( rotation == COdaDgnAppPrintSettings::k90degrees || rotation == COdaDgnAppPrintSettings::k270degrees )
  {
    std::swap(dPaperWidth, dPaperHeight);
  }

  HANDLE hPrinter = NULL;
  HGLOBAL hDevnames = NULL;
  HGLOBAL hDevmode = NULL;

  // pointer to printer defaults structure

  // Try to open the same Printer/Plotter as was set for active layout
  if( OpenPrinter( (LPTSTR)(LPCTSTR)strPrinterName,           // pointer to printer or server name
                   &hPrinter,                                 // pointer to printer or server handle
                   NULL //&printerDefault                     // pointer to printer defaults structure
                 )
    )
  {
    // OK! Printer was found!

    DWORD dLevel = 2;     // A PRINTER_INFO_2 structure containing detailed information about the printer
    DWORD cbNeeded;

    // Get count of bytes retrieved for required printer info data structure
    GetPrinter( hPrinter,             // handle to printer of interest
                dLevel,               // version of printer info data structure
                NULL,                 // pointer to array of bytes that receives printer info structure
                0,                    // size, in bytes, of the pPrinter buffer
                &cbNeeded             // pointer to variable with count of bytes retrieved (or required)
              );

    LPBYTE pBuf = new BYTE [cbNeeded];

    if (pBuf)
    {
      // Get required printer info data structure
      DWORD cbBuf = cbNeeded;

      if( GetPrinter( hPrinter,       // handle to printer of interest
                      dLevel,         // version of printer info data structure
                      pBuf,           // pointer to array of bytes that receives printer info structure
                      cbBuf,          // size, in bytes, of the pPrinter buffer
                      &cbNeeded       // pointer to variable with count of bytes retrieved (or required)
                    ))
      {
        PRINTER_INFO_2  printerInfo;
        memcpy(&printerInfo, pBuf, sizeof(PRINTER_INFO_2));

        // To prepare DEVNAMES structure that identifies 
        //  the driver, device, and output port names of a specific printer.

        WORD wBytes = sizeof(DEVNAMES);           // number of bytes to allocate

        OdString strDriver(OD_T("WINSPOOL"));
        OdString strDevice(printerInfo.pPrinterName);
        OdString strOutput(printerInfo.pPortName);

        WORD wDriverOffset = wBytes;
        wBytes = WORD(wBytes + strDriver.getLength() + 1) * sizeof(OdChar);

        WORD wDeviceOffset = wBytes;
        wBytes = WORD(wBytes + strDevice.getLength() + 1) * sizeof(OdChar);
  
        WORD wOutputOffset = wBytes;
        wBytes = WORD(wBytes + strOutput.getLength() + 1) * sizeof(OdChar);

        hDevnames = ::GlobalAlloc(GHND, wBytes);
        LPDEVNAMES lpDevnames = (LPDEVNAMES)::GlobalLock(hDevnames);
        //LPDEVNAMES lpDevnames = (LPDEVNAMES)hDevnames;

        lpDevnames->wDriverOffset = wDriverOffset;
        lpDevnames->wDeviceOffset = wDeviceOffset;
        lpDevnames->wOutputOffset = wOutputOffset;
	      lpDevnames->wDefault = 0; // &= ~DN_DEFAULTPRN; // Without this, print dialog will reload from Defaults...

        _tcscpy( (LPTSTR)lpDevnames + lpDevnames->wDriverOffset, (LPCTSTR)strDriver );
        _tcscpy( (LPTSTR)lpDevnames + lpDevnames->wDeviceOffset, (LPCTSTR)strDevice );
        _tcscpy( (LPTSTR)lpDevnames + lpDevnames->wOutputOffset, (LPCTSTR)strOutput );

        // To prepare DEVMODE structure that specifies information 
        //  about the device initialization and environment of a printer.
            
        LONG nResult = DocumentProperties( NULL,                 // handle of window that displays dialog box
                                           hPrinter,	            // handle of printer object
                                           (LPTSTR)(LPCTSTR)strDevice,	// address of device name
                                           //(LPSTR)lpDevnames + lpDevnames->wDeviceOffset
                                            0,	          // address of modified device mode structure
                                            0,
                                            0); 	// mode flag

        hDevmode = ::GlobalAlloc(GHND, nResult);
        LPDEVMODE lpDevmode = (LPDEVMODE)::GlobalLock(hDevmode);
            
        nResult = DocumentProperties( NULL,                 // handle of window that displays dialog box
                                      hPrinter,	            // handle of printer object
                                      (LPTSTR)(LPCTSTR)strDevice,	// address of device name
                                      //(LPSTR)lpDevnames + lpDevnames->wDeviceOffset
                                      lpDevmode,	          // address of modified device mode structure
                                      0,
                                      DM_OUT_BUFFER); 	// mode flag

        //*lpDevmode = *printerInfo.pDevMode;

        // To change Printer settings

        bool bPageLandscape = (dPaperWidth > dPaperHeight) ? true : false;
				lpDevmode->dmFields |= DM_ORIENTATION;
				lpDevmode->dmOrientation = (short) (bPageLandscape ? DMORIENT_LANDSCAPE : DMORIENT_PORTRAIT);
			  lpDevmode->dmFields |= DM_PAPERLENGTH | DM_PAPERWIDTH | DM_PAPERSIZE;
            
        // in tenths of a millimeter
				lpDevmode->dmPaperLength = (short) ((bPageLandscape ? dPaperWidth  : dPaperHeight) * 10.);
				lpDevmode->dmPaperWidth =  (short) ((bPageLandscape ? dPaperHeight : dPaperWidth ) * 10.);
        lpDevmode->dmPaperSize = DMPAPER_USER;

				nResult = DocumentProperties(
			    	                  NULL,                 // handle of window that displays dialog box
    				                  hPrinter,	            // handle of printer object
    				                  (LPTSTR)(LPCTSTR)strDevice,	// address of device name
                              //(LPSTR)lpDevnames + lpDevnames->wDeviceOffset
                              lpDevmode,	          // address of modified device mode structure
    				                  lpDevmode,
    				                  DM_IN_BUFFER|DM_OUT_BUFFER); 	// mode flag

			  //bool bSuccess = (nResult > 0);

        // To select a specific printer, and release the printer that was previously selected
        pApp->SelectPrinter(hDevnames, hDevmode);

        pInfo->m_pPD->m_pd.hDevNames = hDevnames;
        pInfo->m_pPD->m_pd.hDevMode = hDevmode;

				pInfo->m_pPD->CreatePrinterDC();
        ODA_ASSERT(pInfo->m_pPD->m_pd.hDC != NULL);

        bPrinterWasSet = true;
      }

      if (pBuf)
      {
        delete [] pBuf;
      }
    }

    if( hDevnames )
    {
	    ::GlobalUnlock(hDevnames);
    }

    if( hDevmode )
    {
	    ::GlobalUnlock(hDevmode);
    }
        
    if (hPrinter)
    {
      ClosePrinter(hPrinter);
    }
  }

    if (!bPrinterWasSet)
    {
      // WARNING MESSAGE
      CString str;
      if (strPrinterName == _T("None"))
      {
        str = _T("No plotter is assigned. Use Page Setup Dialog to assign a plotter to the current Layout.");
      }
      else
      {
        str.Format(_T("%s"), strPrinterName.c_str());
        str += _T(": This printer configuraion cannot be used. Use Page Setup Dialog to assign a plotter to the current Layout.");
      }
      ::AfxMessageBox(str);
      
      return false; // cancel printing
    }

	// TODO: call DoPreparePrinting to invoke the Print dialog box
  DoPreparePrinting(pInfo);
  return CView::OnPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// COdaDgnAppVectorizer diagnostics

#ifdef _DEBUG
void COdaDgnAppVectorizer::AssertValid() const
{
  CView::AssertValid();
}

void COdaDgnAppVectorizer::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}
#endif //_DEBUG


#define GDI_DEVICE_NAME OdWinGDIModuleName

/////////////////////////////////////////////////////////////////////////////
// COdaDgnAppVectorizer message handlers
void COdaDgnAppVectorizer::OnInitialUpdate() 
{
  CView::OnInitialUpdate();

  m_currentOperation.setWindow( this ); // MKU 02/01/10 eliminated warning C4355 'this' : used in base member initializer list

  COdaDgnAppDoc* pDoc = static_cast<COdaDgnAppDoc*>(GetDocument());
  OdDgDatabase* pDb = pDoc->database();

  OdGiContextForDgDatabase::setDatabase(pDb);
  OdGiContextForDgDatabase::setView( pDoc->vectorizedViewId().openObject( OdDg::kForRead ) );
  m_hWindowDC = ::GetDC(m_hWnd);
  
  createDevice( pDoc->vectorizedModelId(), pDoc->vectorizedViewId() );
  if(m_pDevice.isNull())
  {
    GetParent()->PostMessage(WM_CLOSE);
    return;
  }
  m_editor.initialize(m_pDevice, static_cast<COdaDgnAppDoc*>(GetDocument())->cmdCtx() );

  setCursor(::LoadCursor(0, IDC_ARROW));
}

bool COdaDgnAppVectorizer::regenAbort() const
{
  return false;

  // to prevent artifacts in preview (in bitmap mode)
//  if(isPlotGeneration())
//    return false;
//
//  if(!m_bRegenAbort)
//  {
//    // this message processing is necessary to avoid GUI lock during regeneration process
//    MSG msg;
//    if(::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
//    {
//      switch(msg.message)
//      {
//      case WM_CLOSE:
//      case WM_QUIT:
//      case WM_COMMAND:
//      case WM_SYSCOMMAND:
//
//      case WM_LBUTTONDOWN:
//      case WM_LBUTTONUP:
//      case WM_LBUTTONDBLCLK:
//      case WM_RBUTTONDOWN:
//      case WM_RBUTTONUP:
//      case WM_RBUTTONDBLCLK:
//      case WM_MBUTTONDOWN:
//      case WM_MBUTTONUP:
//      case WM_MBUTTONDBLCLK:
//      case WM_MOUSEWHEEL:
//        // messages that abort regen
//        m_bRegenAbort = true;
//        break;
//
//      //case WM_MOUSEMOVE:
//      //  if(m_pTracker || m_mode==kGetPoint)
//      //  {
//      //    m_bRegenAbort = true;
//      //    break;
//      //  }
//      default:
//        // message that might do not abort regen could be processed here
//        theApp.PumpMessage();
//        break;
//      }
//    }
//  }
//  return m_bRegenAbort;
}

void COdaDgnAppVectorizer::OnPaint() 
{
  CView::OnPaint();

  CMainFrame* pMainFrame = (CMainFrame*)theApp.GetMainWnd();
  m_bRegenAbort = false;

  try
  {
    pMainFrame->StartTimer();
    if ( m_pDevice.get() )
    {
      setViewportBorderProperties();
      m_pDevice->update();
    }
    pMainFrame->StopTimer(_T("Regen"));
    //if(!regenAbort())
    //{
    //  pMainFrame->StopTimer(_T("Regen"));
    //}
  }
  catch(const OdError& e)
  {
    theApp.reportError(0, _T("Rendering aborted"), e);
    GetParent()->PostMessage(WM_CLOSE);
  }
#ifndef _DEBUG
  catch(...)
  {
    theApp.reportError(0, _T("Rendering aborted"), OdError(_T("Unknown exception is caught...")));
    GetParent()->PostMessage(WM_CLOSE);
  }
#endif //#ifndef _DEBUG
}

BOOL COdaDgnAppVectorizer::OnEraseBkgnd(CDC* /*pDC*/) 
{
  return TRUE;
}

void COdaDgnAppVectorizer::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);

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


void COdaDgnAppVectorizer::OnDestroy() 
{
  COdaDgnAppDoc* pDoc = GetDocument();
  pDoc->OnCloseVectorizer( this );

  m_editor.uninitialize();

  destroyDevice();

  m_pPrinterDevice.release();

  ::ReleaseDC(m_hWnd, m_hWindowDC);
  m_hWindowDC = 0;
  CView::OnDestroy();
//  ((CMainFrame*)theApp.GetMainWnd())->releaseViewerBar();
}

int COdaDgnAppVectorizer::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CView::OnCreate(lpCreateStruct) == -1)
    return -1;

  m_dropTarget.Register(this);
  //CFrameWnd* pFrameWnd = GetParentFrame();

  //if (!m_toolBar.CreateEx(pFrameWnd,
	 // TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC)
	 // ||
	 // !m_toolBar.LoadToolBar(IDR_VECTORIZER))
  //{
	 // TRACE0("Failed to create vectorizer toolbar\n");
	 // return 0;      // fail to create
  //}

  //m_toolBar.EnableDocking(CBRS_ALIGN_ANY);
  //pFrameWnd->EnableDocking(CBRS_ALIGN_ANY);
  //pFrameWnd->DockControlBar(&m_toolBar);

  //return 0;
  //((CMainFrame*)theApp.GetMainWnd())->createViewerBar();

  return 0;
}

OdGsView* COdaDgnAppVectorizer::getActiveView()
{
  if( !m_pDevice.isNull() )
  {
    return m_pDevice->activeView();
  }

  return 0;
}

const OdGsView* COdaDgnAppVectorizer::getActiveView() const
{
  if( !m_pDevice.isNull() )
  {
    return m_pDevice->activeView();
  }

  return 0;
}
void COdaDgnAppVectorizer::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  CView::OnChar(nChar, nRepCnt, nFlags);
  m_response.m_string = m_inpars.result();
  switch(nChar)
  {
  case VK_BACK:
    while(nRepCnt--)
      m_inpars.eraseChar();
    break;

  case VK_ESCAPE:
    m_response.m_type = Response::kCancel;
    m_inpars.reset(false);

    switch(m_mode)
    {
    case kQuiescent:
      if(m_editor.unselect())
        PostMessage(WM_PAINT);
      break;

    case kGetPoint:
      // no op
      break;
    }
    break;

  default:
    while(nRepCnt--)
    {
      if(!m_inpars.addChar((OdChar)nChar))
      {
        m_inpars.reset(false);
        switch(m_mode)
        {
        case kQuiescent:
          if(m_response.m_string.isEmpty())
            GetDocument()->ExecuteCommand(GetDocument()->recentCmdName());
          else
            GetDocument()->ExecuteCommand(m_response.m_string);
          break;

        case kGetPoint:
        case kGetString:
          m_response.m_type = Response::kString;
          break;
        }
      }
    }
    break;
  }

  if(m_pTracker && m_mode==kGetString &&
     m_response.m_type != Response::kString &&
     m_inpars.result() != (LPCTSTR)m_response.m_string)
  {
    static_cast<OdEdStringTracker*>(m_pTracker)->setValue(m_inpars.result());
//    if(m_bTrackerHasDrawables)
//    {
//      getActiveTopView()->invalidate();
//      PostMessage(WM_PAINT);
//    }
  }

  if(m_sPrompt.IsEmpty())
    m_sPrompt = _T("command: ");
  else if(m_inpars.result().isEmpty())
    theApp.setStatusText(m_sPrompt);
  else
    theApp.setStatusText(m_inpars.result());
}

//void COdaDgnAppVectorizer::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
//{
//  CView::OnChar(nChar, nRepCnt, nFlags);
//
//  switch(nChar)
//  {
//    case VK_ESCAPE:
//    {
//      m_response.m_type = Response::kCancel;
//
//      switch(m_mode)
//      {
//        case kQuiescent:
//        {
//          if(m_editor.unselect())
//          {
//            PostMessage(WM_PAINT);
//          }
//        }
//        break;
//
//        case kGetPoint:
//        // no op
//        break;
//      }
//    }
//    break;
//
//    default:
//    {
//
//    }
//    break;
//  }
//}

void COdaDgnAppVectorizer::OnViewZoomWindow () 
{
  GetDocument()->ExecuteCommand( OD_T("zoom w ") );
}

void COdaDgnAppVectorizer::OnCreateFence() 
{
  GetDocument()->ExecuteCommand( OD_T("Fence r") );
}
void COdaDgnAppVectorizer::OnCreateFenceCircle() 
{
  GetDocument()->ExecuteCommand( OD_T("Fence c") );
}

void COdaDgnAppVectorizer::OnViewZoomextents1() 
{
  GetDocument()->ExecuteCommand(OD_T("zoom e "));
}

void COdaDgnAppVectorizer::OnViewPresentation()
{
  OdDgViewPtr pView = getView(OdDg::kForWrite);

  CViewFlagsDlg dlg( this, getDatabase(), pView.get() );

  dlg.DoModal();

  if( dlg.m_bRegenerateDrawing )
  {
    //Redraw vectorizers
    m_pDevice->invalidate();
    if (m_pDevice->gsModel())
      m_pDevice->gsModel()->invalidate(OdGsModel::kInvalidateAll);
    PostMessage(WM_PAINT);
  }

  pView = 0;
}

void COdaDgnAppVectorizer::OnViewZoomextents() 
{
  OdGsViewPtr view = getActiveView();

  //get active pModel
  COdaDgnAppDoc* pDoc = static_cast<COdaDgnAppDoc*>(GetDocument());
  OdDgDatabase* pDb = pDoc->database();
  OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject();

  //get the extents of that pModel
  OdGeExtents3d extents;

  //it is not calculated yet for perspective cameras
  // initial actions for perspective camera
  // find a better solution
  if( view->isPerspective() )
  {
    pModel->getGeomExtents(m_idView, extents);
    if (!extents.isValidExtents())
      return;

    view->zoomExtents( extents.minPoint(), extents.maxPoint() );
    sinchronizeWithDgView( view );
    PostMessage( WM_PAINT );
    return;
  }
  else if (!getViewId().isNull())
  {
    OdDgViewPtr pDgView = getViewId().openObject(OdDg::kForWrite, true);
    pDgView->zoomToExtents();

    OdGsDeviceForDgModelPtr pDevice = getDevice();

    if (!pDevice.isNull() && !pDgView.isNull())
    {
      for (int j = 0; j < pDevice->numViews(); j++)
      {
        OdGsView* pGsView = pDevice->viewAt(j);

        OdAbstractViewPEPtr pAVP(pGsView);
        pAVP->setView(pGsView, pDgView);
      }
      PostMessage(WM_PAINT);
      return;
    }

    pModel->getGeomExtents(m_idView, extents);
    if (!extents.isValidExtents())
      return;
  }
  else
  {
    pModel->getGeomExtents(m_idView, extents);
    if (!extents.isValidExtents())
      return;
  }

  //get all points
  OdGePoint3d realPoints[ 8 ];
  { 
    OdGePoint3d min = extents.minPoint(), max = extents.maxPoint();

    realPoints[ 0 ] = min;
    realPoints[ 1 ] = max;
    realPoints[ 2 ] = OdGePoint3d( min.x, max.y, min.z );
    realPoints[ 3 ] = OdGePoint3d( max.x, max.y, min.z );
    realPoints[ 4 ] = OdGePoint3d( max.x, min.y, min.z );
    realPoints[ 5 ] = OdGePoint3d( min.x, min.y, max.z );
    realPoints[ 6 ] = OdGePoint3d( min.x, max.y, max.z );
    realPoints[ 7 ] = OdGePoint3d( max.x, min.y, max.z );
  }

  //get 'relative' points
  OdGePoint2d   relativePoints[ 8 ];
  OdGePoint3d   position = view->position(), target = view->target();
  OdGeVector3d   dir = target - position;
  OdGeVector3d  n = view->upVector(), m = ( target - position ).crossProduct( n ).normal();

  n.normalize();
  m.normalize();

  {
    int         i;

    for( i = 0; i < 8; i++ )
    {
      relativePoints[ i ].x = ( realPoints[ i ] - position ).dotProduct( m );
      relativePoints[ i ].y = ( realPoints[ i ] - position ).dotProduct( n );
    }
  }

  //get characteristic points
  OdGeVector2d min, medium, max;
  {
    int i;

    max.x = relativePoints[ 0 ].x;
    max.y = relativePoints[ 0 ].y;

    min = max;

    for( i = 0; i < 8; i++ )
    {
      if( min.x > relativePoints[ i ].x ) 
      {
        min.x = relativePoints[ i ].x;
      }
      if( max.x < relativePoints[ i ].x )
      {
        max.x = relativePoints[ i ].x;
      }
      if( min.y > relativePoints[ i ].y ) 
      {
        min.y = relativePoints[ i ].y;
      }
      if( max.y < relativePoints[ i ].y )
      {
        max.y = relativePoints[ i ].y;
      }
    }

    medium = ( max + min ) / 2.;
  }

  //shift the camera (if new size is not zero; it is prohibited by Ge library)
  if( min.x < max.x || min.y < max.y )
  {
    try
    {
      //view->setView(  position + m * medium.x + n * medium.y, target + m * medium.x + n * medium.y, n,
      //                ( max.x - min.x ) * 1.1, ( max.y - min.y ) * 1.1 );

      OdGePoint3d pPos = position + m * medium.x + n * medium.y;
      OdGePoint3d pTar = target + m * medium.x + n * medium.y;
      OdGeVector3d vDir = pTar - pPos;
      double dirLen = vDir.length();
      if (OdZero(dirLen, 10e-8))
      {
        pTar += OdGeVector3d(0, 0, -1);
        vDir = pTar - pPos;
        dirLen = vDir.length();
      }
      double targetLen = pTar.asVector().length();

      // #9204 (Possibly Invalid Model extents - cannot zoom to full extents)
      //        As a result 'position' point is equal 'target' point that it is wrong.
      if( targetLen > 1e20 )
      {
        view->setView( pPos/ dirLen * targetLen, pTar, n,
          ( max.x - min.x ) * 1.1, ( max.y - min.y ) * 1.1 , view->isPerspective() ? OdGsView::kPerspective : OdGsView::kParallel);
      }
      else if(targetLen > 1. && OdZero(dirLen / targetLen, 10e-8))
      {
        view->setView( pTar - vDir / dirLen * targetLen, pTar, n,
          ( max.x - min.x ) * 1.1, ( max.y - min.y ) * 1.1 , view->isPerspective() ? OdGsView::kPerspective : OdGsView::kParallel);
      }
      else
      {
        view->setView( pPos, pTar, n,
                       ( max.x - min.x ) * 1.1, ( max.y - min.y ) * 1.1  , view->isPerspective() ? OdGsView::kPerspective : OdGsView::kParallel);
      }

      sinchronizeWithDgView( view );

      PostMessage( WM_PAINT );
    }
    catch(const OdError& e)
    {
      theApp.reportError(_T("OnViewZoomextents error "), e);
      GetParent()->PostMessage(WM_CLOSE);
    }  
  }
}

void COdaDgnAppVectorizer::OnViewZoomin() 
{
  zoom( 1. );
}

void COdaDgnAppVectorizer::OnViewZoomout() 
{
  zoom( -1. );
}

void COdaDgnAppVectorizer::OnView3dOrbit() 
{
  m_currentOperation.setType( m_currentOperation.k3dOrbit );

  GetDocument()->ExecuteCommand(OD_T("3dorbit "));

  m_currentOperation.setType( m_currentOperation.kNone );
}

void COdaDgnAppVectorizer::OnUpdateView3dOrbit(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck( m_currentOperation.getType() == m_currentOperation.k3dOrbit );
}

void COdaDgnAppVectorizer::OnViewPan()
{
  m_currentOperation.setType( m_currentOperation.kPan );

  GetDocument()->ExecuteCommand(OD_T("dolly "));

  m_currentOperation.setType( m_currentOperation.kNone );
}

void COdaDgnAppVectorizer::OnUpdateViewPan(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck( m_currentOperation.getType() == m_currentOperation.kPan );
}

void COdaDgnAppVectorizer::OnRButtonUp(UINT nFlags, CPoint point) 
{
  CView::OnRButtonUp(nFlags, point);

  // load and display popup menu
  CMenu menu;
  menu.LoadMenu(IDR_VECTORIZE_POPUP_MENU);
  CMenu* pPopup = menu.GetSubMenu(0);
  ASSERT(pPopup);

  CPoint pos(GetMessagePos());

  int popupMenuXPos = pos.x;
  int popupMenuYPos = pos.y;

  CRect rc;

  pPopup->TrackPopupMenu(
    TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
    popupMenuXPos, popupMenuYPos, this, &rc);
}

void COdaDgnAppVectorizer::OnMouseMove(UINT nFlags, CPoint point) 
{
  CView::OnMouseMove(nFlags, point);

  ::SetCursor(m_hCursor);

  if(point!=m_oldPoint)
  {

    m_oldPoint = point;

    switch(m_mode)
    {
      case kQuiescent:
      {
        m_editor.OnMouseMove(nFlags, point.x, point.y);
      }
      break;

      case kGetPoint:
      {
        OdGePoint3d pt = m_editor.toEyeToWorld(point.x, point.y);
        bool bRegen = false;

        if(!GETBIT(m_inpOptions, OdEd::kGptNoOSnap))
        {
          bRegen = m_editor.snap(pt, m_pBasePt);
        }

        if(m_pTracker)
        {
          static_cast<OdEdPointTracker*>(m_pTracker)->setValue(pt);
        }
      }
      break;
    }
    if(!getActiveView()->isValid() )
    {
      //getActiveView()->invalidate();
      PostMessage(WM_PAINT);
    }
  }
  
  if( nFlags & MK_LBUTTON )
  {
    m_currentOperation.drawMarker( point );
  }
  else
  {
    //they could press the left button inside, move cursor out, unhold the button and then return the cursor inside
    m_currentOperation.undoDrawnMarker();
  }
}

bool COdaDgnAppVectorizer::beginDragCallback(const OdGePoint3d& pt)
{
  OdSaveState<Mode> saved_m_mode(m_mode, kDragDrop);
/*  GetDocument()->startDrag(pt);*/
  return true;
}

void COdaDgnAppVectorizer::OnLButtonDown(UINT nFlags, CPoint point) 
{
  CView::OnLButtonDown(nFlags, point);
  m_pPrevPoint = point;
  m_currentOperation.setStartPoint( point );
  m_currentOperation.drawMarker( point );

  OdGePoint2d correctScreenPoint( point.x, point.y );
  OdGePoint3d ptWorld = getWorldCoordinateByScreen( correctScreenPoint );
  //OdGePoint3d pt = toEyeToWorld(x, y);
  bool bCtrl = (OdEdBaseIO::kControlIsDown & nFlags) != 0;

  CurrentOperation::Type operation = m_currentOperation.getType();

  switch(m_mode)
  {

  case kQuiescent:
    if(m_editor.OnMouseLeftButtonClick( nFlags, point.x, point.y, this))
    {
      //getActiveView()->invalidate();
      PostMessage(WM_PAINT);
    }
    break;

//   case kGetPoint:
//     m_response.m_point = m_editor.toEyeToWorld(point.x, point.y);
//     m_editor.snap(m_response.m_point, m_pBasePt);
//     m_response.m_type = Response::kPoint;
//     break;

  }
}

void COdaDgnAppVectorizer::OnLButtonUp(UINT nFlags, CPoint point) 
{
  CView::OnLButtonUp(nFlags, point);
  if (m_bMaskClip)
  {
    // 0. get active model
    COdaDgnAppDoc* pDoc = static_cast<COdaDgnAppDoc*>(GetDocument());
    OdDgModelPtr pModel = pDoc->database()->getActiveModelId().safeOpenObject(OdDg::kForWrite);
    // 1. get raster attachment header
    OdDgRasterAttachmentHeaderPtr pHeader;    
    OdDgElementIteratorPtr pIter = pModel->createControlElementsIterator();
    for ( ; !pIter->done(); pIter->step() )
    {
      OdDgElementPtr pElm = pIter->item().openObject(OdDg::kForWrite);
      if (pElm->isA() == OdDgRasterAttachmentHeader::desc())
      {
        pHeader = pElm; 
      }
    }

    if (!pHeader.isNull())
    {
      OdGePoint3d origin;
      OdGeVector3d uVector;
      OdGeVector3d vVector;
      // 2. get raster orientation
      pHeader->getOrientation(origin, uVector, vVector);

      // 3. convert cursor coordinates to world coordinates
      OdGePoint3d pt_prev = m_editor.toEyeToWorld(m_pPrevPoint.x, m_pPrevPoint.y);
      OdGePoint3d pt_cur = m_editor.toEyeToWorld(point.x, point.y);

      // 4. find points projections to raster plane
      OdGePlane plane(origin, uVector, vVector);
      OdGeVector3d normal = uVector.crossProduct(vVector);
      OdGePoint3d projection = pt_prev.project(plane, normal);
      OdGePoint2d ptPrevConv((projection.x - origin.x) / uVector.x, (projection.y - origin.y) / vVector.y);
      projection = pt_cur.project(plane, normal);
      OdGePoint2d ptCurConv((projection.x - origin.x) / uVector.x, (projection.y - origin.y) / vVector.y);
      
      // 5. create clip element (rectangle) with the coordinates (point 5)
      OdGePoint2dArray vertices;
      vertices.resize(5);
      vertices[0].x = ptPrevConv.x; vertices[0].y = ptPrevConv.y;
      vertices[1].x = ptPrevConv.x; vertices[1].y = ptCurConv.y;
      vertices[2].x = ptCurConv.x;  vertices[2].y = ptCurConv.y;
      vertices[3].x = ptCurConv.x;  vertices[3].y = ptPrevConv.y;
      vertices[4].x = ptPrevConv.x; vertices[4].y = ptPrevConv.y;

      pHeader->addMaskClipVertices(vertices);
      OdGsModel* pGsModel = m_editor.gsModel();

      if( pGsModel )
      {
        pGsModel->onModified( pHeader, pHeader->ownerId() );
      }
    }

    m_bMaskClip = false;
  }

  bool bCtrl = (OdEdBaseIO::kControlIsDown & nFlags) != 0;

  CurrentOperation::Type operation = m_currentOperation.getType();

  if(m_mode==kGetPoint /*&& GetCapture()*/)
  {
    m_response.m_point = m_editor.toEyeToWorld(point.x, point.y);
    m_editor.snap(m_response.m_point, m_pBasePt);
    m_response.m_type = Response::kPoint;
    ReleaseCapture();
  }

  if( operation == CurrentOperation::kNone )
  {
    return;
  }

  m_currentOperation.undoDrawnMarker();

  OdGsView* view = getActiveView();
  OdGeVector3d upVector = view->upVector();
  OdGePoint3d position = view->position(), target = view->target();
  double width = view->fieldWidth(), height = view->fieldHeight();
  bool isPerspective = view->isPerspective();

  view->setView( position, target, upVector, width, height, isPerspective ? OdGsView::kPerspective : OdGsView::kParallel  );

  sinchronizeWithDgView( view );

  OnPaint();
}

CRect COdaDgnAppVectorizer::viewportRect() const
{
  CRect rc;
  GetClientRect(&rc);
  return rc;
}

CRect COdaDgnAppVectorizer::viewRect(OdGsView* pView)
{
  OdGePoint3d ll, ur;
  pView->getViewport((OdGePoint2d&)ll, (OdGePoint2d&)ur);
  OdGeMatrix3d x = pView->screenMatrix();
  ll.transformBy(x);
  ur.transformBy(x);
  return CRect(OdRoundToLong(ll.x), OdRoundToLong(ur.y), OdRoundToLong(ur.x), OdRoundToLong(ll.y));
}

void COdaDgnAppVectorizer::dolly(int x, int y) 
{
  OdGsView* pView = getActiveView();
  OdGeVector3d vec(-x, -y, 0.0);
  vec.transformBy((pView->screenMatrix() * pView->projectionMatrix()).inverse());
  pView->dolly(vec);
}

BOOL COdaDgnAppVectorizer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
  ScreenToClient(&pt);
  OdGePoint2d correctScreenPoint( pt.x, pt.y );

  OdGePoint3d worldPoint = getWorldCoordinateByScreen( correctScreenPoint );
  zoom( zDelta / WHEEL_DELTA );
  
  //shift back
  {
    OdGsView* view = getActiveView();

    //get new screen point of the same world point
    OdGePoint3d newScreenPoint;
    {
      OdGeMatrix3d worldToDevice = view->worldToDeviceMatrix();
      newScreenPoint = worldToDevice* worldPoint;
    }

    //get world points on the focal plane
    OdGePoint3d
      movingPoint = getWorldCoordinateByScreen( OdGePoint2d( newScreenPoint.x, newScreenPoint.y ) ),
      destinationPoint = getWorldCoordinateByScreen( correctScreenPoint );  

    OdGePoint3d position = view->position(), target = view->target();
    OdGeVector3d upVector = view->upVector();
    double width = view->fieldWidth(), height = view->fieldHeight();
    bool isPerspective = view->isPerspective();

    //shift the camera so points coincide
    OdGeVector3d offset = destinationPoint - movingPoint;
    position -= offset;
    target -= offset;

    view->setView( position, target, upVector, width, height, isPerspective ? OdGsView::kPerspective : OdGsView::kParallel );
  }

  if(m_editor.OnMouseWheel(nFlags, pt.x, pt.y, zDelta))
  {
    PostMessage(WM_PAINT);
    propagateActiveViewChanges();
  }

  return CView::OnMouseWheel(nFlags, zDelta, pt);
}

inline OdGsViewPtr activeView(OdGsDevice* pDevice) 
{
  OdGsViewPtr pActiveView;
  OdGsDeviceForDgModelPtr pLHelper = OdGsDeviceForDgModel::cast(pDevice);
  if(pLHelper.get())
    pActiveView = pLHelper->activeView();
  return pActiveView;
}

void COdaDgnAppVectorizer::setViewportBorderProperties() 
{
  OdGsViewPtr pActiveView = activeView(m_pDevice);
  int n = m_pDevice->numViews();
  if(n>1)
  {
    for(int i = 0; i<n; ++i)
    {
      OdGsViewPtr pView = m_pDevice->viewAt(i);
      pView->setViewportBorderVisibility(true);
    }
  }
}

OdGiContext::PStyleType COdaDgnAppVectorizer::plotStyleType() const
{
  return OdGiContextForDgDatabaseToPlotStyleSupport::plotStyleType();
}

void COdaDgnAppVectorizer::UpdateVectorizersColorPalette(const COLORREF* pColorPaletteRef)
{
	//TODO: Set new palette

	ODGSPALETTE pPalCpy;
	pPalCpy.insert(pPalCpy.begin(), pColorPaletteRef, pColorPaletteRef + 256);

	OdDgModelPtr pModel = m_modelId.safeOpenObject();
	COLORREF background = pModel->getBackground();
	theApp.settings().setActiveBackground(background);

	// Color with #255 always defines backround. The background of the active model must be considered in the device palette.
	pPalCpy[255] = background;
	// Note: This method should be called to resolve "white background issue" before setting device palette
	bool bCorrected = OdDgColorTable::correctPaletteForWhiteBackground(pPalCpy.asArrayPtr());

	m_pDevice->setLogicalPalette(pPalCpy.asArrayPtr(), 256);
	m_pDevice->setBackgroundColor(theApp.settings().getActiveBackground()); // ACAD's color for paper bg
	setPaletteBackground(theApp.settings().getActiveBackground());


	//Redraw vectorizers
	m_pDevice->invalidate();
	if (m_pDevice->gsModel())
		m_pDevice->gsModel()->invalidate(OdGsModel::kInvalidateAll);
	PostMessage(WM_PAINT);
}

void COdaDgnAppVectorizer::createDevice( const OdDgElementId& modelId, const OdDgElementId& viewId )
{
  CRect rc;
  GetClientRect(&rc);
  try
  {
    OdString str = theApp.settings().getGsDevicePath();
    if (str.isEmpty())
      return;

    OdGsModulePtr pGs = ::odrxDynamicLinker()->loadModule(str, false);
    OdGsDevicePtr pDevice = pGs->createDevice();
    OdRxDictionaryPtr pProperties = pDevice->properties();
    if(pProperties.get())
    {
      if(pProperties->has(OD_T("WindowHWND")))
        pProperties->putAt(OD_T("WindowHWND"), OdRxVariantValue((OdIntPtr)m_hWnd)); // hWnd necessary for DirectX device
      if(pProperties->has(OD_T("WindowHDC")))
        pProperties->putAt(OD_T("WindowHDC"),  OdRxVariantValue((OdIntPtr)m_hWindowDC)); // hWindowDC necessary for Bitmap device
      //if(pProperties->has(OD_T("DoubleBufferEnabled")))
      //  pProperties->putAt(OD_T("DoubleBufferEnabled"), OdRxVariantValue(theApp.doubleBufferEnabled()));
      //if(pProperties->has(OD_T("EnableSoftwareHLR")))
      //  pProperties->putAt(OD_T("EnableSoftwareHLR"), OdRxVariantValue(theApp.useSoftwareHLR()));
      //if(pProperties->has(OD_T("DiscardBackFaces")))
      //  pProperties->putAt(OD_T("DiscardBackFaces"), OdRxVariantValue(theApp.discardBackFaces()));
      if (pProperties->has(OD_T("UseVisualStyles"))) // Check if property is supported
        pProperties->putAt(OD_T("UseVisualStyles"), OdRxVariantValue(true));
      if(pProperties->has(OD_T("BlocksCache"))) // Check if property is supported
        pProperties->putAt(OD_T("BlocksCache"), OdRxVariantValue(theApp.useBlockCahce()));
    }

    enableGsModel(theApp.useGsModel()); // TODO: use theApp.useGsModel() setting

    //   There are some problems with microstation line weight support for WinGDI mode,
    // thus we set ACad line weight style ( rounded caps and joints )
    // Support of that line weight style use only default WinGDI algorithms.

    OdString strGDIModuleName = OdWinGDIModuleName;
    strGDIModuleName = strGDIModuleName.left( strGDIModuleName.getLength() - 4 );

    if( str.find(strGDIModuleName) != -1 )
    {
      setLineWeightConfiguration( kPointLineWeight, 1 );
      setLineWeightConfiguration( kLineCapStyle, (OdUInt32)(OdPs::kLesRound) );
      setLineWeightConfiguration( kLineJoinStyle, (OdUInt32)(OdPs::kLjsRound) );
    }
    else
    {
      setLineWeightConfiguration( kPointLineWeight, 1 );
      setLineWeightConfiguration( kLineCapStyle, (OdUInt32)(OdPs::kLesButt) );
      setLineWeightConfiguration( kLineJoinStyle, (OdUInt32)(OdPs::kLjsDiamond) );
    }
    //

    m_pDevice = OdGsDeviceForDgModel::setupModelView(modelId, viewId, pDevice, this);
    m_modelId = modelId;
    m_idView  = viewId;

    COdaDgnAppDoc* pDoc = static_cast<COdaDgnAppDoc*>(GetDocument());
    OdDgDatabase* pDb = pDoc->database();
    const ODCOLORREF* refColors = OdDgColorTable::currentPalette( pDb );

    ODGSPALETTE pPalCpy;
    pPalCpy.insert(pPalCpy.begin(), refColors, refColors + 256);

    OdDgModelPtr pModel = m_modelId.safeOpenObject();
    COLORREF background = pModel->getBackground();
    theApp.settings().setActiveBackground( background );

    // Color with #255 always defines backround. The background of the active model must be considered in the device palette.
    pPalCpy[255] = background;
    // Note: This method should be called to resolve "white background issue" before setting device palette
    bool bCorrected = OdDgColorTable::correctPaletteForWhiteBackground(pPalCpy.asArrayPtr());

    m_pDevice->setLogicalPalette(pPalCpy.asArrayPtr(), 256);
    m_pDevice->setBackgroundColor(theApp.settings().getActiveBackground()); // ACAD's color for paper bg
    setPaletteBackground(theApp.settings().getActiveBackground());
  
    OdGsDCRect  gsRect(rc.left, rc.right, rc.bottom, rc.top);
    m_pDevice->onSize(gsRect);

    // Adding plotstyletable info
    // !!! comment next lines after debugging of dgn plot style loading
    // setPlotGeneration( true );
    // preparePlotstyles();
  }
  catch(const OdError& e)
  {
    // MKU 23/12/09 - ('warning' issue)   //DWORD err = ::GetLastError();
    destroyDevice();
    theApp.reportError(_T("Graphic System Initialization Error"), e);
  }  
}

void COdaDgnAppVectorizer::destroyDevice() 
{
  //while(m_linkReactors.size())
  //{
  //  static_cast<OdDgGsLinkReactor*>(m_linkReactors.last().get())->detach();
  //  m_linkReactors.removeLast();
  //}

  m_pDevice.release();
}

HCURSOR COdaDgnAppVectorizer::cursor() const
{
  return m_hCursor;
}

void COdaDgnAppVectorizer::setCursor(HCURSOR hCursor)
{
  m_hCursor = hCursor;
  ::SetCursor(hCursor);
}

void COdaDgnAppVectorizer::OnSetRenderMode(UINT nID)
{
  ODA_ASSUME((ID_2DWIREFRAME             -ID_2DWIREFRAME) == OdGsView::k2DOptimized &&
             (ID_3DWIREFRAME             -ID_2DWIREFRAME) == OdGsView::kWireframe &&
             (ID_HIDDEN                  -ID_2DWIREFRAME) == OdGsView::kHiddenLine &&
             (ID_FLAT_SHADED             -ID_2DWIREFRAME) == OdGsView::kFlatShaded &&
             (ID_GOURAUD_SHADED          -ID_2DWIREFRAME) == OdGsView::kGouraudShaded &&
             (ID_FLAT_SHADED_WITH_EDGES  -ID_2DWIREFRAME) == OdGsView::kFlatShadedWithWireframe &&
             (ID_GOURAUD_SHADED_EDGES_ON -ID_2DWIREFRAME) == OdGsView::kGouraudShadedWithWireframe);

  OdGsView::RenderMode rm = OdGsView::RenderMode(OdGsView::k2DOptimized + (nID - ID_2DWIREFRAME));
  OdGsView* pView = getActiveView();
  if (pView->mode() != rm)
  {
    pView->setMode(rm);

    for( int i = 0; i < m_pDevice->numViews(); i++ )
    {
      OdGsView* pXRefView = m_pDevice->viewAt(i);

      if( pXRefView == pView )
      {
        continue;
      }

      pXRefView->setMode( rm );
    }

    if(pView->mode() != rm)
    {
      COdaDgnApp::messageBox(m_hWnd, _T("Rendering"), _T("Mode is not supported by current device."), MB_ICONWARNING);
    }
    else
    {
      PostMessage(WM_PAINT);
    }

    sinchronizeWithDgView( pView, true );
  }
}

void COdaDgnAppVectorizer::OnViewerRegen() 
{
  m_pDevice->invalidate();
  if(m_pDevice->gsModel())
    m_pDevice->gsModel()->invalidate(OdGsModel::kInvalidateAll);
  PostMessage(WM_PAINT);
}

void COdaDgnAppVectorizer::OnViewerVpregen() 
{
  m_pDevice->invalidate();
  if(m_pDevice->gsModel())
    m_pDevice->gsModel()->invalidate(getActiveView());
  PostMessage(WM_PAINT);
}

void COdaDgnAppVectorizer::OnUpdateViewerRegen(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_pDevice->gsModel()!=0);
}

bool COdaDgnAppVectorizer::canClose() const
{
  if(m_mode != kQuiescent)
  {
    AfxMessageBox(_T("Can not exit while command is active."), MB_OK|MB_ICONEXCLAMATION);
    return false;
  }
  return true;
}

void COdaDgnAppVectorizer::OnRefresh()
{
  PostMessage(WM_PAINT);
}

void COdaDgnAppVectorizer::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  switch( nChar ) 
  {
  case VK_ESCAPE:
    break;

  case VK_F5:
    PostMessage(WM_PAINT);
    break;

  case VK_DELETE:
    ((COdaDgnAppDoc*)GetDocument())->DeleteSelection();
    PostMessage(WM_PAINT);
    break;
  }

  CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL COdaDgnAppVectorizer::PreCreateWindow(CREATESTRUCT& cs)
{
  HCURSOR cursor = LoadCursor( NULL, IDC_CROSS );
  cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW|CS_OWNDC, cursor );
  return CView::PreCreateWindow(cs);
}

void COdaDgnAppVectorizer::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
  CView::OnUpdate(pSender, lHint, pHint);
  //if ( !m_pDevice.isNull() )
    //OnViewerRegen();
}

void COdaDgnAppVectorizer::OnUpdate2dwireframe(CCmdUI* pCmdUI) 
{
  if( getActiveView() )
    pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::k2DOptimized);
}

bool checkAcceptability( UINT nID, OdGsView* pView )
{
  bool bEnable = true;
  OdGsView::RenderMode rm = OdGsView::RenderMode(OdGsView::k2DOptimized + (nID - ID_2DWIREFRAME));
  OdGsView::RenderMode rmActual = pView->mode();
  pView->setMode( rm );
  if (pView->mode() != rm)
  {
    bEnable = false;
  }
  pView->setMode( rmActual ); // restore

  return bEnable;
}

void COdaDgnAppVectorizer::OnUpdate3dwireframe(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( checkAcceptability( pCmdUI->m_nID, getActiveView() ));
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kWireframe);
}

void COdaDgnAppVectorizer::OnUpdateHidden(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( checkAcceptability( pCmdUI->m_nID, getActiveView() ));
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kHiddenLine);
}

void COdaDgnAppVectorizer::OnUpdateFlatShaded(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( checkAcceptability( pCmdUI->m_nID, getActiveView() ));
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kFlatShaded);
}

void COdaDgnAppVectorizer::OnUpdateFlatShadedWithEdges(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( checkAcceptability( pCmdUI->m_nID, getActiveView() ));
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kFlatShadedWithWireframe);
}

void COdaDgnAppVectorizer::OnUpdateGouraudShaded(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( checkAcceptability( pCmdUI->m_nID, getActiveView() ));
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kGouraudShaded);
}

void COdaDgnAppVectorizer::OnUpdateGouraudShadedEdgesOn(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( checkAcceptability( pCmdUI->m_nID, getActiveView() ));
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kGouraudShadedWithWireframe);
}

void COdaDgnAppVectorizer::respond(const OdString& s)
{
  m_response.m_type = Response::kString;
  m_response.m_string = s;
}

OdEdCommandPtr COdaDgnAppVectorizer::command(const OdString& commandName)
{
  //if (commandName.iCompare(L"REGEN") == 0)
  //{
  //  OdSmartPtr<OdExRegenCmd> c = OdRxObjectImpl<OdExRegenCmd>::createObject();
  //  c->m_pView = this;
  //  c->m_pDevice = m_pDevice;
  //  return c;
  //}
  //else
  return m_editor.command(commandName);
}

OdDgExEditorObject &COdaDgnAppVectorizer::editorObject()
{
  return m_editor;
}

const OdDgExEditorObject &COdaDgnAppVectorizer::editorObject() const
{
  return m_editor;
}

void COdaDgnAppVectorizer::sinchronizeWithDgView( const OdGsView* pGsView, bool bResetDisplayStyle )
{
  COdaDgnAppDoc* pDoc = static_cast<COdaDgnAppDoc*>(GetDocument());
  OdDgDatabase* pDb = pDoc->database();

  OdDgViewPtr pDgView;

  pDb->startUndoRecord();

  if( !getViewId().isNull() )
  {
    pDgView = getViewId().openObject(OdDg::kForWrite, true);
  }

  if( !pDgView.isNull() )
  {
    OdAbstractViewPEPtr pAVP( pDgView );
    pAVP->setView(pDgView, pGsView);
  }

  if( bResetDisplayStyle )
  {
    OdDgDisplayStyleViewPEPtr pViewDSPE = pDgView;
    pViewDSPE->setDisplayStyle(pDgView, NULL);
  }

  pDgView = 0;
}

#define UPDATED_PERSPECTIVE_ALGORITHM

void COdaDgnAppVectorizer::zoom( double steps )
{
  OdGsView* view = getActiveView();
  
#ifndef UPDATED_PERSPECTIVE_ALGORITHM
  if( view->isPerspective() )
  {
    // initial actions for perspective camera
    // find a better solution
    view->zoom( 1.);

    OdGePoint3d position = view->position(), target = view->target();
    OdGeVector3d direction = target - position;
    OdGeVector3d upVector = view->upVector();
    double width = view->fieldWidth(), height = view->fieldHeight();

    //calculate an appropriate offset using real size of the frame
    if( width < height )
    {
      steps *= width / 2.;
    }
    else
    {
      steps *= height / 2.;
    }

    direction *= steps;

    position += direction;
    target += direction;

    view->setView( position, target, upVector, width, height, OdGsView::kPerspective );
  }
  else
#endif
  {
    view->zoom( pow( 1.11, double( steps ) ) );
  }

  sinchronizeWithDgView( view );

  PostMessage( WM_PAINT );
}

// MKU 02/01/10 eliminated warning C4355 'this' : used in base member initializer list
COdaDgnAppVectorizer::CurrentOperation::CurrentOperation( )
: m_type( kNone )
//, m_window( window )
{
}

void COdaDgnAppVectorizer::CurrentOperation::drawMarker( const CPoint& currentPoint )
{
  undoDrawnMarker();

  m_drawnMarker.m_point = currentPoint;
  m_drawnMarker.m_type = m_type;
}

void COdaDgnAppVectorizer::CurrentOperation::setStartPoint( const CPoint& startPoint )
{
  m_startPoint = startPoint;
}

void COdaDgnAppVectorizer::CurrentOperation::drawMarkerFor3dOrbit( const CPoint& currentPoint )
{
  RECT extent;
  m_window->GetClientRect( &extent );

  double sizeFactor;
  {
    double x = ( extent.right - extent.left ) / 10., y = (extent.bottom - extent.top ) / 10.;
    sizeFactor = x < y ? x : y;

    //zero value can insult the further calculations
    if( !sizeFactor )
    {
      return;
    }
  }
  OdGePoint2d startPoint( m_startPoint.x, m_startPoint.y ), _currentPoint( currentPoint.x, currentPoint.y );

  CClientDC context( m_window );
  context.SetROP2( R2_NOT );

  //determine a rotation
  {
    OdGeVector2d line = _currentPoint - startPoint;
    m_resultRotation.setToRotation( atan( line.length() / sizeFactor / 10. ) * 4., OdGeVector3d( line.y, line.x, 0. ) );
  }

  //draw a cube
  {
    OdGePoint3d center3d( ( extent.right + extent.left ) / 2., ( extent.top + extent.bottom ) / 2., 0. ), point3d;
    OdGeVector3d directions[ 10 ];
    OdGePoint2d points[ 10 ];

    directions[ 0 ].set( -sizeFactor, -sizeFactor, sizeFactor );
    directions[ 1 ].set( -sizeFactor, sizeFactor, sizeFactor );
    directions[ 2 ].set( sizeFactor, sizeFactor, sizeFactor );
    directions[ 3 ].set( sizeFactor, -sizeFactor, sizeFactor );
    directions[ 4 ].set( -sizeFactor, -sizeFactor, -sizeFactor );
    directions[ 5 ].set( -sizeFactor, sizeFactor, -sizeFactor );
    directions[ 6 ].set( sizeFactor, sizeFactor, -sizeFactor );
    directions[ 7 ].set( sizeFactor, -sizeFactor, -sizeFactor );
    directions[ 8 ].set( 0., sizeFactor * .3, sizeFactor );
    directions[ 9 ].set( 0., -sizeFactor, sizeFactor );
    
    {
      int i;
      OdGeMatrix3d invertY;
      
      invertY[ 1 ][ 1 ] = -1.;
      invertY = invertY * m_resultRotation;
      for( i = 0; i < 10; i++ )
      {
        point3d = center3d + invertY * directions[ i ];
        points[ i ].set( point3d.x, point3d.y );
      }
    }

    context.MoveTo( toPOINT( points[ 0 ] ) );
    context.LineTo( toPOINT( points[ 1 ] ) );
    context.LineTo( toPOINT( points[ 2 ] ) );
    context.LineTo( toPOINT( points[ 6 ] ) );
    context.LineTo( toPOINT( points[ 7 ] ) );
    context.LineTo( toPOINT( points[ 4 ] ) );
    context.LineTo( toPOINT( points[ 5 ] ) );
    context.LineTo( toPOINT( points[ 1 ] ) );
    context.MoveTo( toPOINT( points[ 2 ] ) );
    context.LineTo( toPOINT( points[ 3 ] ) );
    context.LineTo( toPOINT( points[ 0 ] ) );
    context.LineTo( toPOINT( points[ 4 ] ) );
    context.MoveTo( toPOINT( points[ 5 ] ) );
    context.LineTo( toPOINT( points[ 6 ] ) );
    context.MoveTo( toPOINT( points[ 7 ] ) );
    context.LineTo( toPOINT( points[ 3 ] ) );
    context.LineTo( toPOINT( points[ 8 ] ) );
    context.LineTo( toPOINT( points[ 9 ] ) );
  }
}

void COdaDgnAppVectorizer::CurrentOperation::drawMarkerForPan( const CPoint& currentPoint )
{
  RECT extent;
  m_window->GetClientRect( &extent );

  //sizeFactor determines the size of arrow (its width, its elements)
  double sizeFactor;
  {
    double x = ( extent.right - extent.left ) / 100., y = (extent.bottom - extent.top ) / 100.;
    sizeFactor = x < y ? x : y;

    //zero value can insult the further calculations
    if( !sizeFactor )
    {
      return;
    }
  }
  OdGePoint2d startPoint( m_startPoint.x, m_startPoint.y ), _currentPoint( currentPoint.x, currentPoint.y );

  CClientDC context( m_window );
  context.SetROP2( R2_NOT );

  OdGeVector2d direction, perpedicular;
  double length;
  {
    direction = _currentPoint - startPoint;
    length = direction.length();
    if( length < sizeFactor )
    {
      m_resultOffset.set( 0., 0. );
      return;
    }

    //scale the arrow to fit to the length
    if( length < sizeFactor * 5. )
    {
      sizeFactor = length / 5.;
    }

    //store the result
    {
      m_resultOffset.set( direction.x, direction.y );
    }

    direction.normalize();
    perpedicular.set( -direction.y, direction.x );
  }

  //arrow to the current position
  {
    POINT points[ 8 ];

    points[ 0 ] = toPOINT( startPoint + perpedicular * sizeFactor );
    points[ 1 ] = toPOINT( _currentPoint + perpedicular * sizeFactor - direction * sizeFactor * 5. );
    points[ 2 ] = toPOINT( _currentPoint + perpedicular * sizeFactor * 2. - direction * sizeFactor * 5. );
    points[ 3 ] = toPOINT( _currentPoint );
    points[ 4 ] = toPOINT( _currentPoint - perpedicular * sizeFactor * 2. - direction * sizeFactor * 5. );
    points[ 5 ] = toPOINT( _currentPoint - perpedicular * sizeFactor - direction * sizeFactor * 5. );
    points[ 6 ] = toPOINT( startPoint - perpedicular * sizeFactor );
    points[ 7 ] = toPOINT( startPoint + perpedicular * sizeFactor );

    context.Polyline( points, 8 );
  }
}

void COdaDgnAppVectorizer::CurrentOperation::setType( Type type )
{
  m_type = type;
}

COdaDgnAppVectorizer::CurrentOperation::Type COdaDgnAppVectorizer::CurrentOperation::getType() const
{
  return m_type;
}

CPoint COdaDgnAppVectorizer::CurrentOperation::toCPoint( const OdGePoint2d& original ) const
{
  return CPoint( int( original.x ), int( original.y ) );
}

void COdaDgnAppVectorizer::CurrentOperation::undoDrawnMarker()
{
  if( m_drawnMarker.m_type == kNone )
  {
    return;
  }

  m_drawnMarker.m_type = kNone;
}

POINT COdaDgnAppVectorizer::CurrentOperation::toPOINT( const OdGePoint2d& original ) const
{
  POINT a = { long( original.x ), long( original.y ) };
  return a;
}

COdaDgnAppVectorizer::CurrentOperation::DrawnMarker::DrawnMarker()
: m_type( kNone )
{
}

const OdGeMatrix3d& COdaDgnAppVectorizer::CurrentOperation::getResultRotation() const
{
  return m_resultRotation;
}

const OdGeVector2d& COdaDgnAppVectorizer::CurrentOperation::getResultOffset() const
{
  return m_resultOffset;
}

OdGePoint3d COdaDgnAppVectorizer::getWorldCoordinateByScreen( const OdGePoint2d& screenPoint ) const
{
  OdGsViewPtr view = getActiveView();

  //directions, origin, etc
  OdGePoint3d target = view->target();
  OdGeVector3d xVector, yVector;
  {
    OdGeVector3d direction = target - view->position();
    yVector = view->upVector();
    xVector = direction.crossProduct( yVector ).normal();
  }

  //the simplest way I found is to make back calculations...
  OdGePoint3d center, stepX, stepY;
  {
    center = target;
    stepX = target + xVector;
    stepY = target + yVector;

    OdGeMatrix3d matrix = view->worldToDeviceMatrix();
    stepX.transformBy( matrix );
    stepY.transformBy( matrix );
    center.transformBy( matrix );
  }

  //make the calculation
  OdGePoint3d result;
  {
    double x, y;
    x = ( screenPoint.x - center.x ) / ( stepX.x - center.x );
    y = ( screenPoint.y - center.y ) / ( stepY.y - center.y );

    result = target + xVector * x + yVector * y;
  }

  return result;
}
/*
OdEdCommandPtr COdaDgnAppVectorizer::command(const OdString& commandName)
{
  //if (commandName.iCompare(L"REGEN") == 0)
  //{
  //  OdSmartPtr<OdExRegenCmd> c = OdRxObjectImpl<OdExRegenCmd>::createObject();
  //  c->m_pView = this;
  //  c->m_pDevice = m_pDevice;
  //  return c;
  //}
  //else
  return m_editor.command(commandName);
}
*/

COdaDgnAppPrintSettings COdaDgnAppVectorizer::getPrintSettings() const
{
  return m_printSettings;
}

void COdaDgnAppVectorizer::setPrintSettings( const COdaDgnAppPrintSettings& printSettings )
{
  m_printSettings = printSettings;
}

void COdaDgnAppVectorizer::propagateActiveViewChanges() const
{
#pragma MARKMESSAGE("TODO: 3.5 - editing - COdaDgnAppVectorizer::propagateActiveViewChanges() ")
}

void COdaDgnAppVectorizer::track(OdEdInputTracker* pTracker)
{
  if( m_pTracker )
  {
    m_pTracker->removeDrawables(getActiveView());
  }

  m_pTracker = pTracker;

  m_pBasePt = 0;
  if(pTracker)
  {
    m_bTrackerHasDrawables = pTracker->addDrawables(getActiveView())!=0;
    OdEdPointDefTrackerPtr pPointDefTracker = OdEdPointDefTracker::cast(m_pTracker);
    if(pPointDefTracker.get())
    {
      m_basePt = pPointDefTracker->basePoint();
      m_pBasePt = &m_basePt;
    }
  }
  else
    m_bTrackerHasDrawables = false;
}

class DgSaveViewParams
{
  COdaDgnAppVectorizer*   m_pView;
  HCURSOR       m_hOldCursor;
public:
  DgSaveViewParams(COdaDgnAppVectorizer* pView, OdEdInputTracker* pTracker, HCURSOR hCursor)
    : m_pView(pView)
    , m_hOldCursor(pView->cursor())
  {
    pView->track(pTracker);
    pView->setCursor(hCursor);
    pView->m_editor.initSnapping(pView->getActiveView());
  }
  ~DgSaveViewParams()
  {
    m_pView->track(0);
    m_pView->setCursor(m_hOldCursor);
    m_pView->m_editor.uninitSnapping(m_pView->getActiveView());
  }
};

OdString COdaDgnAppVectorizer::getString(const OdString& prompt, int options, OdEdStringTracker* pTracker)
{
  m_sPrompt.Empty();
  OdSaveState<CString> savePrompt(m_sPrompt);
  putString(prompt);

  OdSaveState<Mode>     saved_m_mode(m_mode, kGetString);

  m_response.m_type = Response::kNone;

  m_inpOptions = options;

  DgSaveViewParams svp(this, pTracker, ::LoadCursor(0, IDC_IBEAM));

  while ( theApp.PumpMessage() )
  {
    switch(m_response.m_type)
    {
    case Response::kString:
      return m_response.m_string;

    case Response::kCancel:
      throw OdEdCancel();
      break;
    }
    // Mfc idle loop for toolbars and menus process
    LONG lIdle = 0;
    while (theApp.OnIdle(lIdle++)) ;  
  }

  throw OdEdCancel();
}

void COdaDgnAppVectorizer::putString(const OdString& string)
{
  m_sPrompt = (LPCTSTR)string;
  int n = m_sPrompt.ReverseFind('\n');
  if (n >= 0)
  {
    theApp.setStatusText(((LPCTSTR)m_sPrompt) + n + 1);
  }
  else
    theApp.setStatusText(m_sPrompt);
}

OdGePoint3d COdaDgnAppVectorizer::getPoint(const OdString& prompt, int options, OdEdPointTracker* pTracker)
{
  m_sPrompt.Empty();
  OdSaveState<CString> savePrompt(m_sPrompt);
  putString(prompt);

  OdSaveState<Mode>     saved_m_mode(m_mode, kGetPoint);

  m_response.m_type = Response::kNone;

  m_inpOptions = options;

  DgSaveViewParams svp(this, pTracker, ::LoadCursor(0, IDC_CROSS));

  while ( theApp.PumpMessage() )
  {
    switch(m_response.m_type)
    {
    case Response::kPoint:
      if(GETBIT(m_inpOptions, OdEd::kGptBeginDrag))
        SetCapture();
      return m_response.m_point;

    case Response::kString:
      throw OdEdOtherInput(m_response.m_string);

    case Response::kCancel:
      throw OdEdCancel();
    }
    // Mfc idle loop for toolbars and menus process
    LONG lIdle = 0;
    while (theApp.OnIdle(lIdle++)) ;  
  }

  throw OdEdCancel();
}
void COdaDgnAppVectorizer::OnClipwithMask()
{
  m_bMaskClip = true;
}
