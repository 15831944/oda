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
// AppPlotCommands.cpp
//

#include <QPrinterInfo> //#include <QPrinter>
#include <QPainter>
#include <QDir>
//#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
//#include <QPrintEngine>
//// https://bugreports.qt.io/browse/QTBUG-35478
//#include <5.7.1/QtPrintSupport/private/qprintengine_win_p.h>
//#endif

#include "AppModule.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdUserIO.h"
//#include "DbBaseHostAppServices.h"
//#include "DynamicLinker.h"
#include "DbBaseDatabase.h"
#include "DbDatabase.h"

#include "GiContextForDbDatabase.h"
#include "DbGsManager.h" // OdGsModelLayoutHelper
#include "ColorMapping.h"
#include "DbLayout.h"
#include "DbBlockTableRecord.h"
#include "DbHostAppServices.h"
#include "Gi/GiRasterImage.h"
#include "DbViewTable.h"
#include "DbViewTableRecord.h"
#include "DbViewportTable.h"
#include "DbViewportTableRecord.h"
#include "DbViewport.h"
#include "AbstractViewPE.h"
#include "RxVariantValue.h"

#include "DbUserIO.h" // TODO DbBaseDatabaseIO.h like DbBaseDatabase.h //#include "Ed/EdUserIO.h"
#define OdDbBaseDatabaseIO OdDbUserIO
typedef OdSmartPtr<OdDbBaseDatabaseIO> OdDbBaseDatabaseIOPtr;

#include "OdqMainWindow.h"
#include "OdqApplication.h" 
#include "OdqPlotDialog.h" 

#include "ExtDbModule.h"

#include "ExStringIO.h"
#define DWF_DEVICE_NAME L"DWF6 ePlot.pc3" // or #include "DwfMediaList.h"
//possible TODO //#define PDF_DEVICE_NAME L"DWG To PDF.pc3" // or #include "PdfMediaList.h"

void OdqCmd_Print::execute(OdEdCommandContext* pCmdCtx)
{
  OdqMainWindow* pMainWindow = qobject_cast<OdqMainWindow*>(getIApp()->getMainWindow());
  ODA_ASSERT_ONCE(pMainWindow);

  OdqAppModule* pModule = static_cast<OdqAppModule*>(pMainWindow->getAppModule());
  ODA_ASSERT_ONCE(pModule);

  pModule->m_cmd_Plot.execute(pCmdCtx);
}

// printing
static OdSharedPtr<QPrinter> s_pPrinter;

inline QRectF swapRectF(bool bVal, const QRectF& in)
{
  if (!bVal)
    return in;
  return QRectF(in.y(), in.x(), in.height(), in.width()); 
}
#define Q_SWAP_RECT(x) swapRectF(bSwapXY, x)

////TODO #define ODQ_PLOT_USE_TILES
//#ifdef ODQ_PLOT_USE_TILES
//#include "BmpTilesGen.h"
//
//// TODO with using https://stackoverflow.com/questions/18998850/how-to-merge-two-images-into-one-using-qimage-and-qpainter-in-qt
//
//void generateTiles(//HDC hdc, 
//                   RECT& drawrc, OdGsDevice* pBmpDevice, OdUInt32 nTileWidth, OdUInt32 nTileHeight)
//{
//  CRect destrc = drawrc;
//  destrc.NormalizeRect();
//  OdGsDCRect step(0,0,0,0);
//  OdGsDCRect rc(drawrc.left, drawrc.right, drawrc.bottom, drawrc.top);
//  OdUInt32 nWidth = abs(rc.m_max.x-rc.m_min.x);
//  rc.m_max.x -= rc.m_min.x;
//  if(rc.m_max.x < 0)
//  {
//    rc.m_min.x = -rc.m_max.x;
//    rc.m_max.x = 0;
//    step.m_min.x = nTileWidth;
//  }
//  else
//  {
//    rc.m_min.x = 0;
//    step.m_max.x = nTileWidth;
//  }
//  OdUInt32 nHeight = abs(rc.m_max.y-rc.m_min.y);
//  rc.m_max.y -= rc.m_min.y;
//  if(rc.m_max.y < 0)
//  {
//    rc.m_min.y = -rc.m_max.y;
//    rc.m_max.y = 0;
//    step.m_min.y = nTileHeight;
//  }
//  else
//  {
//    rc.m_min.y = 0;
//    step.m_max.y = nTileHeight;
//  }
//
//  OdInt32 m = nWidth / nTileWidth + (nWidth % nTileWidth ? 1 : 0);
//  OdInt32 n = nHeight / nTileHeight + (nHeight % nTileHeight ? 1 : 0);
//
//  BmpTilesGen tilesGen(pBmpDevice, rc);
//  pBmpDevice->onSize(rc);
//
//  OdGiRasterImagePtr pImg;
//
//  int dx = (step.m_max.x - step.m_min.x);
//  int dy = (step.m_max.y - step.m_min.y);
//
//  int dx2 = m > 1 ? dx / abs(dx) * 8 : 0;
//  int dy2 = n > 1 ? dy / abs(dy) * 8 : 0;
//
//  BITMAPINFO bmi;
//  bmi.bmiHeader.biBitCount = (WORD)24;
//  bmi.bmiHeader.biWidth  = nTileWidth  + abs(dx2) * 2;
//  bmi.bmiHeader.biHeight = nTileHeight;
//  bmi.bmiHeader.biClrImportant = 0;
//  bmi.bmiHeader.biClrUsed = 0;
//  bmi.bmiHeader.biCompression = BI_RGB;
//  bmi.bmiHeader.biPlanes = 1;
//  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//  bmi.bmiHeader.biSizeImage = 0;
//  bmi.bmiHeader.biXPelsPerMeter = 0;
//  bmi.bmiHeader.biYPelsPerMeter = 0;
//
//  HDC bmpDC = CreateCompatibleDC(hdc);
//  if(bmpDC)
//  {
//    void* pBuf;
//    HBITMAP hBmp = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &pBuf, NULL, 0);
//    if(hBmp)
//    {
//      HBITMAP hOld = (HBITMAP)SelectObject(bmpDC, hBmp);
//      for(OdInt32 i=0; i < m; ++i)
//      {
//        for(OdInt32 j=0; j < n; ++j)
//        {
//          int minx = rc.m_min.x + i * dx;
//          int maxx = minx + dx;
//          int miny = rc.m_min.y + j * dy;
//          int maxy = miny + dy;
//
//          // render wider then a tile area to reduce gaps in lines.
//          pImg = tilesGen.regenTile(OdGsDCRect(minx - dx2, maxx + dx2, miny - dy2, maxy + dy2));
//
//          pImg->scanLines((OdUInt8*)pBuf, 0, nTileHeight);
//          BitBlt(hdc, destrc.left + odmin(minx, maxx), destrc.top + odmin(miny, maxy),
//            nTileWidth, nTileHeight, bmpDC, abs(dx2), 0, SRCCOPY);
//        }
//      }
//      SelectObject(bmpDC, hOld);
//      DeleteObject(hBmp);
//    }
//    DeleteDC(bmpDC);
//  }
//}
//#endif

static OdGePoint3d toScreenCoord(const OdGsView* pView, const OdGePoint3d &wcsPt)
{
  OdGePoint3d scrPt(wcsPt);

  //OdGeMatrix3d xWorldToEye = OdAbstractViewPEPtr(pGsView)->worldToEye(pGsView);
  //wcsPt.transformBy(xWorldToEye);
  //return wcsPt;
  //
  // see also OdExEditorObject::toScreenCoord
  // To DCS
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
  scrPt.z = 0.;
  return scrPt;
}

void OdqCmd_Plot::execute(OdEdCommandContext* pCmdCtx)
{
  if (!s_pPrinter.isNull())
    return; // busy // s_pPrinter = NULL;

  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  if (!pRxDatabase)
    FIX_MAC_APP_TROW(OdError(eNoDatabase)); // throw OdError(eNoDatabase);

  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pRxDatabase);
  if (pDwgDb.isNull())
    FIX_MAC_APP_TROW(OdError(eNotImplemented)); // throw OdError(eNotImplemented); // TODO

  IqViewSystem* iVS = getIViewSystem();
  if (!iVS)
    FIX_MAC_APP_TROW(OdError(eNotApplicable)); // throw OdError(eNotApplicable);

  // see also OdqView::resetDevice()

  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDatabase);
  ODA_ASSERT_ONCE(!pDbPE.isNull());
  //OdRxObjectPtr pCurrLayout = pDbPE->currentLayout(pRxDatabase);
  //OdDbBaseLayoutPEPtr pLayoutInfo(pCurrLayout);
  //if (pLayoutInfo.isNull())
  //  FIX_MAC_APP_TROW(OdError(eNotApplicable)); // throw OdError(eNotApplicable);
  ////QString qsCurrLayoutName = getIApp()->toQString(pLayoutInfo->name(pCurrLayout));
  //bool isModel = pLayoutInfo->isModelLayout(pCurrLayout);

  OdGsModulePtr pGs = iVS->getGsModule(pRxDatabase);
  if (pGs.isNull())
    FIX_MAC_APP_TROW(OdError(eNotApplicable));
  //bool isDeviceGles = (pGs->moduleName().makeLower().find(L"odaqtgles") >= 0);

  OdGsDevicePtr pPrinterDevice;
  OdExtDbModule* pExtDb = getIApp()->getExtDbModule(pRxDatabase);
  if (!pExtDb)
    pPrinterDevice = pGs->createBitmapDevice();
  else
    // try to create device as for DgDatabase
    pPrinterDevice = pExtDb->createBitmapDevice(pGs, pRxDatabase);
  if (pPrinterDevice.isNull())
  {
    OdString sMsg;
    sMsg.format(OD_T("Error: Txv-Module %ls cannot create bitmap device."),
                pGs->moduleName().c_str());
    pCmdCtx->userIO()->putString(sMsg); // or getIApp()->getBaseHostAppServices()->warning(sMsg);
    return;
  }

  // after CDwgViewer::OnPreparePrinting

  OdDbBlockTableRecordPtr pLayoutBlock = pDwgDb->getActiveLayoutBTRId().safeOpenObject();
  OdDbLayoutPtr pLayout = pLayoutBlock->getLayoutId().safeOpenObject();
  //OdDbPlotSettingsPtr pLayoutSettings = OdDbPlotSettings::cast(pLayout);
  ODA_ASSERT_ONCE(s_pPrinter.isNull());

  OdqMainWindow* pMainWindow = qobject_cast<OdqMainWindow*>(getIApp()->getMainWindow());
  ODA_ASSERT_ONCE(pMainWindow);

  OdDbPlotSettingsPtr pSvToApplyAtEnd = OdDbPlotSettings::createObject(); // using for DWF export only
  if (pLayout.get())
    pSvToApplyAtEnd->copyFrom(pLayout);

  OdSharedPtr<OdqPlotDialog> pPlotDialog = new OdqPlotDialog(pRxDatabase, pMainWindow);
  int res = QDialog::Rejected;
  IqView* iView = NULL;
  do
  {
    res = pPlotDialog->exec();
    switch (res)
    {
    case QDialog::Rejected:
      pPlotDialog = NULL;
      FIX_MAC_APP_TROW(OdEdCancel()); // throw OdEdCancel(); // via undo 
    case enPlorDlg_WindowArea:
      {
        IqViewSystem* iVS = getIViewSystem();
        if (iVS)
          iView = iVS->getView(pRxDatabase);
        OdDbPlotSettingsValidator* pValidator = pPlotDialog->plotSettingsValidator();
        if (pValidator && iView)
        {
          QCursor cursorSave = iView->cursor();
          try
          {
            OdDbBaseDatabaseIOPtr pIO = OdDbBaseDatabaseIO::cast(pCmdCtx->userIO()); // via OdDbUserIO now
            ODA_ASSERT_ONCE(!pIO.isNull());

            iView->setCursor(QCursor(Qt::CrossCursor));

            OdGePoint3d pt1 = pIO->getPoint(OD_T("Specify first corner:"), 
                                            OdEd::kGptNoUCS | OdEd::kGptNoOSnap);
            OdGePoint3d pt2 = pIO->getPoint(OD_T("Specify opposite corner:"), 
                                            OdEd::kGptNoUCS | OdEd::kGptRectFrame | OdEd::kGptNoOSnap);

            OdGsView* pGsView = iView->getActiveTopGsView();
            pt1 = toScreenCoord(pGsView, pt1); 
            pt2 = toScreenCoord(pGsView, pt2);

            pValidator->setPlotType(pPlotDialog->plotSettings(), 
                                    OdDbPlotSettings::kWindow);
            pPlotDialog->fillPlotArea();
            pPlotDialog->fillPlotOffset();
            pPlotDialog->fillPlotScale();
            pValidator->setPlotWindowArea(pPlotDialog->plotSettings(),
                                          pt1.x, pt1.y, pt2.x, pt2.y);
          }
          catch (...)
          {
            ODA_FAIL_ONCE(); //
          }
          iView->setCursor(cursorSave); // restore
        }
        break;
      }
    //case enPlorDlg_Preview:
    default:
      ODA_FAIL_ONCE(); // TODO
    case QDialog::Accepted:
      break;
    }
  }
  while (res != QDialog::Accepted);

  OdDbPlotSettingsPtr pLayoutSettings = pPlotDialog->plotSettings(); 
  OdString sDeviceName = pLayoutSettings->getPlotCfgName();
  if (sDeviceName == OD_T("None"))
    sDeviceName = OdString::kEmpty;

#ifdef PDF_DEVICE_NAME
  // User defined device sample. Add papers for PDF device.
  if (sDeviceName == PDF_DEVICE_NAME)
  {
    QString qsCmd = getIApp()->toQString(L"export");
    OdSmartPtr<ExStringIO> pStringIO = ExStringIO::create(L"PDFONLY");
    pCmdCtx->setArbitraryData(L"EXPORT/SUBCOMMAND", pStringIO);
    getIConsole()->executeCommand(qsCmd, false, pRxDatabase);
    pCmdCtx->setArbitraryData(L"EXPORT/SUBCOMMAND", NULL);
    return;
  }
#endif
#ifdef DWF_DEVICE_NAME
  // User defined device sample. Add papers for DWF device.
  if (sDeviceName == DWF_DEVICE_NAME)
  {
    bool bApplied = pPlotDialog->isAppliedToLayout();
    if (!bApplied)
    { // PROD-137
      if (GETBIT(flags(), OdEdCommand::kNoUndoMarker))
        pDwgDb->startTransaction();
      pPlotDialog->applyToLayout();
    }

    QString qsCmd = getIApp()->toQString(L"export");
    OdSmartPtr<ExStringIO> pStringIO = ExStringIO::create(L"DWFONLY");
    pCmdCtx->setArbitraryData(L"EXPORT/SUBCOMMAND", pStringIO);
    getIConsole()->executeCommand(qsCmd, false, pRxDatabase);
    pCmdCtx->setArbitraryData(L"EXPORT/SUBCOMMAND", NULL);

    if (!bApplied)
    { // PROD-137
      if (GETBIT(flags(), OdEdCommand::kNoUndoMarker))
      {
        pDwgDb->endTransaction();
        pDwgDb->undo();
      }
      else if (pSvToApplyAtEnd.get())
      {
        pPlotDialog->applyToLayout(pSvToApplyAtEnd.get());
        pSvToApplyAtEnd = NULL;
      }
      else
        throw OdEdCancel();
    }
    return;
  }
#endif

  QString qsDeviceName = getIApp()->toQString(sDeviceName);
  if (!qsDeviceName.isEmpty())
  {
    s_pPrinter = NULL;
    foreach (QPrinterInfo info, QPrinterInfo::availablePrinters())
    {
      if (info.isNull() || !s_pPrinter.isNull()) // problem on Mac Tiger without it
        continue;
      QString qsName = info.printerName();

      if (qsDeviceName.toLower() != qsName.toLower()) //if (qsDeviceName.compare(qsName, Qt::CaseInsensitive))
        continue;

      s_pPrinter = new QPrinter(info); // , QPrinter::HighResolution); // QPrinter::PrinterResolution);
      break;
    }
    ODA_ASSERT_ONCE(!s_pPrinter.isNull());
  }
  if (s_pPrinter.isNull())
  {
    // Use Page Setup Dialog to assign a plotter to the current Layout.
    QString qsMsg = QObject::tr("No plotter is assigned."); 
    if (!qsDeviceName.isEmpty())
      qsMsg = QObject::tr("This printer configuraion \"%1\" cannot be used.").arg(qsDeviceName);
    pCmdCtx->userIO()->putString(getIApp()->toOdString(qsMsg)); 
    return;
  }

  OdDbPlotSettings::PlotRotation rotation = pLayoutSettings->plotRotation();
  bool bSwapXY = (   rotation == OdDbPlotSettings::k90degrees 
                  || rotation == OdDbPlotSettings::k270degrees);

  {
    double dPaperWidth, dPaperHeight;
    pLayoutSettings->getPlotPaperSize(dPaperWidth, dPaperHeight);
    
    //if (bSwapXY)
    //  qSwap(dPaperWidth, dPaperHeight);
    //QSizeF szPaper = (dPaperWidth > dPaperHeight) ? QSizeF(dPaperHeight, dPaperWidth) 
    //                                              : QSizeF(dPaperWidth, dPaperHeight);
    QSizeF szPaper = QSizeF(dPaperWidth, dPaperHeight);

    s_pPrinter->setPaperSize(szPaper, QPrinter::Millimeter);
  }

  //QPrinter::Orientation orientation = ((dPaperWidth > dPaperHeight) ? QPrinter::Landscape : QPrinter::Portrait);
  s_pPrinter->setOrientation(QPrinter::Portrait); // QPrinter::Landscape maybe unsupported on other platforms // (orientation);
  //ODA_ASSERT_ONCE(orientation == s_pPrinter->orientation());

  int nDpi = pLayoutSettings->shadePlotCustomDPI();
  ODA_ASSERT_ONCE(nDpi >= 100);
  if (nDpi >= 100)
  {
    s_pPrinter->setResolution(nDpi);
    nDpi = s_pPrinter->resolution();
    //ODA_ASSERT_ONCE(nDpi >= (pLayoutSettings->shadePlotCustomDPI() * 3 / 4));
  }

  OdString sFileName;

  bool isPlotToFile = false;
  if (!pPlotDialog.isNull())
    isPlotToFile = pPlotDialog->isPrintToFile();
  if (isPlotToFile)
  {
    sFileName = pCmdCtx->userIO()->getFilePath(OD_T("Plot to file"), OdEd::kGfpForSave, 
                                               OD_T("Browse for plot file"), // dialog caption 
                                               OD_T("dwg"), // default extension
                                               pLayout->getLayoutName(), // file name
                                               OD_T("Plot file (*.plt)|*.plt||")); // filter
    if (sFileName.isEmpty())
      return;
    s_pPrinter->setOutputFileName(getIApp()->toQString(sFileName));
  }

  int numCopies = 1;
  if (!pPlotDialog.isNull())
    isPlotToFile = pPlotDialog->numberOfCopies();
  if (!sFileName.isEmpty())
    numCopies = 1;
  s_pPrinter->setNumCopies(numCopies);

  // see also OdqView::resetDevice()

  OdGiDefaultContextPtr pCtx = pDbPE->createGiContext(pRxDatabase);
  ODA_ASSERT_ONCE(pRxDatabase == pCtx->database()); 

  // copy info from gsView to active.
  {
    OdDbObjectPtr pVpObj = pDwgDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
    OdDbAbstractViewportDataPtr pAVD(pVpObj);
    const OdGsView* pView = iVS->getView(pRxDatabase)->getActiveGsView();
    if (pView)
      pAVD->setView(pVpObj, pView);
  }

  // set GS cache
  OdGiContextForDbDatabasePtr pDbCtx = OdGiContextForDbDatabase::cast(pCtx); 
  bool bGsCache = false;
  if (pDbCtx.get())
  {
    bGsCache = getIAppProps()->toBool(OD_T("Vars/dwggscache"), true);
    pDbCtx->enableGsModel(bGsCache);
  }
  else if (pExtDb)
  {
    OdString sMnemo = pExtDb->baseModuleName(true); // dgn, bim, dwf, ... 
    bGsCache = getIAppProps()->toBool(OdString().format(L"Vars/%lsgscache", sMnemo.c_str()), true); // "Vars/dgngscache" "Vars/bimgscache" "Vars/dwfgscache"
    pExtDb->enableGsModel(pCtx, bGsCache);
  }
  else
  {
    ODA_FAIL_ONCE(); // TODO
  }

  pCtx->setPlotGeneration(true);

  // Prepare the device to render the active layout in this database.
  OdGsDevicePtr pPrinterHelper = pDbPE->setupActiveLayoutViews(pPrinterDevice, pCtx);

  // Prepare palette :

  ODCOLORREF clrBackground = getIAppProps()->toColorref(OD_T("Options/Display/ModelBackground"),
                                                        ODRGB(255, 255, 255));
  // try to get background via DgDatabase models
  if (pExtDb)
    pExtDb->getModelBackground(pRxDatabase, clrBackground);

  const ODCOLORREF* pColors = odcmAcadPalette(clrBackground);
  pPrinterHelper->setLogicalPalette(pColors, 256);
  pPrinterHelper->setBackgroundColor(clrBackground);
  pDbCtx->setPaletteBackground(pPrinterHelper->getBackgroundColor());

  // Prepare plot styles :

  if (pDbCtx->plotStyleType() == OdGiContext::kPsNone) // protected - if (!pDbCtx->m_pPlotStyleTable.get())
  {
    bool bPlotPlotstyle = pLayoutSettings->plotPlotStyles(),
         bShowPlotstyle = pLayoutSettings->showPlotStyles();

    if (pDbCtx->isPlotGeneration() ? bPlotPlotstyle : bShowPlotstyle)
    {
      OdString sFileName(pLayoutSettings->getCurrentStyleSheet());
      if (!sFileName.isEmpty())
      {
        sFileName = pDwgDb->appServices()->findFile(sFileName, pDwgDb);
        if (!sFileName.isEmpty())
        {
          OdStreamBufPtr pFileBuf = odSystemServices()->createFile(sFileName);
          if(pFileBuf.get())
            pDbCtx->loadPlotStyleTable(pFileBuf);
        }
      }
    }
  }

  // -----

  QPainter painter; // (s_pPrinter);
  if (!painter.begin(s_pPrinter))
  {
    // canceled in dialog to choose output file
    s_pPrinter = NULL;
    //pPlotDialog = NULL;
    FIX_MAC_APP_TROW(OdEdCancel()); // throw OdEdCancel(); // via undo 
  }

  // -----
  // see also CDwgViewer::OnPrint(...)

  bool bPrint90 = false;
  bool bPrint0 = false;
  bool bPrint180 = false;
  bool bPrint270 = false;

  // Get printer paper info
  QRectF rectPage = Q_SWAP_RECT(s_pPrinter->pageRect(QPrinter::DevicePixel));
  double dPrinterWidth = rectPage.width(); // =  pDC->GetDeviceCaps(PHYSICALWIDTH);
  double dPrinterHeight = rectPage.height(); // = pDC->GetDeviceCaps(PHYSICALHEIGHT);
  QRectF rectPaper = Q_SWAP_RECT(s_pPrinter->paperRect(QPrinter::DevicePixel));
  double dPrinterLeftMargin = rectPaper.x(); // = pDC->GetDeviceCaps(PHYSICALOFFSETX);
  double dPrinterTopMargin = rectPaper.y(); // = pDC->GetDeviceCaps(PHYSICALOFFSETY);
  double dPrinterMarginWidth = rectPaper.width(); // = pDC->GetDeviceCaps(HORZRES);
  double dPrinterMarginHeight = rectPaper.height(); // = pDC->GetDeviceCaps(VERTRES);

  double dLogPixelX = nDpi; // s_pPrinter->physicalDpiX() is always 600 for wins // = pDC->GetDeviceCaps(LOGPIXELSX);
  double dLogPixelY = nDpi; // s_pPrinter->physicalDpiY() is always 600 for wins // = pDC->GetDeviceCaps(LOGPIXELSY);

  //    double dPrinterRightMargin  = dPrinterWidth  - dPrinterMarginWidth  - dPrinterLeftMargin;
  double dPrinterBottomMargin = dPrinterHeight - dPrinterMarginHeight - dPrinterTopMargin;
  double koeffX = dLogPixelX / kMmPerInch;
  double koeffY = dLogPixelY / kMmPerInch;

  bool bModel = pPrinterHelper->isKindOf(OdGsModelLayoutHelper::desc());

  // Get Layout info

  bool bScaledToFit =    pLayoutSettings->useStandardScale() 
                      && (OdDbPlotSettings::kScaleToFit == pLayoutSettings->stdScaleType());
  bool bCentered = pLayoutSettings->plotCentered();
  bool bMetric = pLayoutSettings->plotPaperUnits() != OdDbPlotSettings::kInches;
  bool bPrintLW = pLayoutSettings->printLineweights() || pLayoutSettings->showPlotStyles();

  double offsetX, offsetY;
  pLayoutSettings->getPlotOrigin(offsetX, offsetY);         // in mm
  OdGePoint2d pio = pLayoutSettings->getPaperImageOrigin(); // in mm

  double dLeftMargin   = pLayoutSettings->getLeftMargin();  // in mm
  double dRightMargin  = pLayoutSettings->getRightMargin(); // in mm
  double dTopMargin    = pLayoutSettings->getTopMargin();   // in mm
  double dBottomMargin = pLayoutSettings->getBottomMargin();// in mm

  OdDbPlotSettings::PlotType plotType = pLayoutSettings->plotType();

  // set LineWeight scale factor for model space
  if (bPrintLW && bModel)
  {
    OdGsView* pTo = pPrinterHelper->viewAt(0);
    pTo->setLineweightToDcScale(odmax(dLogPixelX, dLogPixelY) / kMmPerInch * 0.01 );
  }

  // Apply paper rotation to paper parameters
  if (rotation == OdDbPlotSettings::k90degrees)
    rotation = OdDbPlotSettings::k270degrees;

  switch (rotation)
  {
  case OdDbPlotSettings::k0degrees:
    bPrint0 = true;
    break;
  case OdDbPlotSettings::k90degrees:
    bPrint90 = true;
    std::swap(dTopMargin, dRightMargin);
    std::swap(dBottomMargin, dLeftMargin);
    std::swap(dBottomMargin, dTopMargin);
    //std::swap(pio.x, pio.y);
    std::swap(offsetX, offsetY);
    offsetY = -offsetY;
    offsetX = -offsetX;
    break;
  case OdDbPlotSettings::k180degrees:
    bPrint180 = true;
    std::swap(dRightMargin , dLeftMargin);
    offsetY = -offsetY;
    offsetX = -offsetX;
    break;
  case OdDbPlotSettings::k270degrees:
    bPrint270 = true;
    std::swap(dTopMargin, dRightMargin);
    std::swap(dBottomMargin, dLeftMargin);
    std::swap(offsetX, offsetY);
    break;
  }

  // Get scale factor
  double factor;
  if (pLayoutSettings->useStandardScale())
  {
    pLayoutSettings->getStdScale(factor);
  }
  else
  {
    double numerator, denominator;
    pLayoutSettings->getCustomPrintScale(numerator, denominator);
    factor = numerator / denominator;
  }

  if (dLeftMargin < dPrinterLeftMargin / koeffX)
    dLeftMargin = dPrinterLeftMargin / koeffX;
  if (dTopMargin < dPrinterTopMargin  / koeffY)
    dTopMargin = dPrinterTopMargin  / koeffY;

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

  //CRect newClipRgn;   
  //newClipRgn.CreateRectRgn(0,0,1,1); // from left, top, right, and bottom
  //CRect MarginsClipBox;
  //int ret = GetClipRgn(pDC->m_hDC, newClipRgn);
  //bool bNullMarginsClipBox = !ret || ret && (GetLastError() != ERROR_SUCCESS);
  QRectF MarginsClipBox(painter.clipRegion().boundingRect());
  bool bNullMarginsClipBox = !MarginsClipBox.isValid();

  double dScreenFactorH, dScreenFactorW;
  if (bNullMarginsClipBox)
  { // printing way
    double x = dLeftMargin    - dPrinterLeftMargin;
    double y = dTopMargin     - dPrinterTopMargin;
    MarginsClipBox.setCoords( // SetRect(
      int(x), int(y),
      int(x + dPrinterWidth - dLeftMargin - dRightMargin),
      int(y + dPrinterHeight - dTopMargin - dBottomMargin));

    dScreenFactorH = dScreenFactorW = 1.;
  }
  else
  {
    MarginsClipBox.setCoords(0, 0, 1, 1);  // newClipRgn.GetRgnBox(&MarginsClipBox);
    dScreenFactorH = double(MarginsClipBox.height()) / dPrinterMarginHeight;
    dScreenFactorW = double(MarginsClipBox.width())  / dPrinterMarginWidth;

    double dLeft = MarginsClipBox.left() + (( dLeftMargin - dPrinterLeftMargin) * dScreenFactorW),
           dBottom = MarginsClipBox.bottom() - (( dBottomMargin  - dPrinterBottomMargin ) * dScreenFactorH);
    double dRight = dLeft + (( dPrinterWidth - dLeftMargin - dRightMargin ) * dScreenFactorW),
           dTop = dBottom - (( dPrinterHeight - dTopMargin - dBottomMargin) * dScreenFactorH);
    MarginsClipBox.setCoords(dLeft, dTop, dRight, dBottom);
  }

  // MarginsClipBox is calculated
  QRectF ClipBox(MarginsClipBox); //CRect

  // Get view and viewport position, direction ...
  OdGsViewPtr pOverallView;

  OdGePoint3d viewTarget, oldTarget;
  bool SkipClipping = false;

  OdAbstractViewPEPtr pAbstractViewPE;
  OdRxObjectPtr pVObject;

  pOverallView = bModel ? OdGsModelLayoutHelperPtr(pPrinterHelper)->activeView() 
                        : OdGsPaperLayoutHelperPtr(pPrinterHelper)->overallView();

  if (plotType == OdDbPlotSettings::kView)
  {
    OdString sPlotViewName = pLayoutSettings->getPlotViewName();
    OdDbViewTableRecordPtr pVtr =
      ((OdDbViewTablePtr)(pDwgDb->getViewTableId().safeOpenObject()))->getAt(sPlotViewName).safeOpenObject();

    viewTarget = pVtr->target();     // in plotPaperUnits
    pAbstractViewPE = OdAbstractViewPEPtr(pVObject = pVtr);
  }
  else if (bModel)
  {
    OdDbViewportTablePtr pVPT = pDwgDb->getViewportTableId().safeOpenObject();
    OdDbViewportTableRecordPtr pActiveVP = pVPT->getActiveViewportId().safeOpenObject();

    viewTarget = pActiveVP->target();     // in plotPaperUnits
    pAbstractViewPE = OdAbstractViewPEPtr(pVObject = pActiveVP);
  }
  else
  {
    OdDbObjectId overallVpId = pLayout->overallVportId();
    OdDbViewportPtr pActiveVP = overallVpId.safeOpenObject();

    viewTarget = pActiveVP->viewTarget();   // in plotPaperUnits
    pAbstractViewPE = OdAbstractViewPEPtr(pVObject = pActiveVP);
  }

  // get info from view, viewport .... etc
  OdGePoint3d viewportCenter = pAbstractViewPE->target(pVObject);       // in plotPaperUnits
  bool isPerspective = pAbstractViewPE->isPerspective(pVObject);
  double viewportH = pAbstractViewPE->fieldHeight(pVObject),  // in plotPaperUnits
         viewportW = pAbstractViewPE->fieldWidth(pVObject);   // in plotPaperUnits
  OdGeVector3d viewDir = pAbstractViewPE->direction(pVObject), // in plotPaperUnits
               upV = pAbstractViewPE->upVector(pVObject);      // in plotPaperUnits
  OdGeMatrix3d eyeToWorld = pAbstractViewPE->eyeToWorld(pVObject),
               WorldToeye = pAbstractViewPE->worldToEye(pVObject);

  bool isPlanView = /*viewTarget.isEqualTo(OdGePoint3d(0,0,0)) &&*/ viewDir.normal().isEqualTo(OdGeVector3d::kZAxis);

  oldTarget = viewTarget;

  // To set OverAll View using default settings

  // get rect of drawing to view (in plotPaperUnits)
  double fieldWidth(viewportW),
         fieldHeight(viewportH);
  if (plotType == OdDbPlotSettings::kWindow || (plotType == OdDbPlotSettings::kLimits && isPlanView))
  {
    double xmin, ymin, xmax, ymax;
    if (plotType == OdDbPlotSettings::kWindow)
    {
      pLayoutSettings->getPlotWindowArea(xmin, ymin, xmax, ymax); // in plotPaperUnits
    }
    else
    {
      xmin = pDwgDb->getLIMMIN().x; ymin = pDwgDb->getLIMMIN().y;
      xmax = pDwgDb->getLIMMAX().x; ymax = pDwgDb->getLIMMAX().y;
    }

    fieldWidth  = xmax - xmin;
    fieldHeight = ymax - ymin;

    OdGeVector3d tmp = viewportCenter - viewTarget;
    viewTarget.set((xmin + xmax) / 2., (ymin + ymax) / 2., 0);
    //if (!bModel) // incorrect
    viewTarget.transformBy(eyeToWorld);
    viewTarget -= tmp;
  } 
  else if (plotType == OdDbPlotSettings::kDisplay)
  {
    viewTarget = viewportCenter;
    fieldWidth  = viewportW;
    fieldHeight = viewportH;
  }
  else if (plotType == OdDbPlotSettings::kExtents || (plotType == OdDbPlotSettings::kLimits && !isPlanView))
  {
    OdGeBoundBlock3d extents;
    if (pAbstractViewPE->plotExtents(pVObject, extents)) // pIter also skip 'off layers'
    {
      extents.transformBy(eyeToWorld);
      viewTarget = (extents.minPoint() + extents.maxPoint().asVector()) / 2.;
      extents.transformBy(WorldToeye);

      fieldWidth  = fabs(extents.maxPoint().x - extents.minPoint().x);
      fieldHeight = fabs(extents.maxPoint().y - extents.minPoint().y);
    }
  }
  else if (plotType == OdDbPlotSettings::kView)
  {
    viewTarget = viewportCenter;

    fieldWidth  = viewportW;
    fieldHeight = viewportH;
  }
  else if (plotType == OdDbPlotSettings::kLayout)
  {
    SkipClipping = true; // used full paper drawing area.

    fieldWidth  = (drx2-drx1) / factor; // drx in mm -> fieldWidth in mm
    fieldHeight = (dry2-dry1) / factor;

    viewTarget.set(fieldWidth / 2. - pio.x - offsetX / factor, fieldHeight / 2. - pio.y - offsetY / factor, 0); // in mm
    if (!bMetric)
    {
      viewTarget /= kMmPerInch; // must be in plotpaper units
      fieldWidth /= kMmPerInch; 
      fieldHeight /= kMmPerInch;
    }

    offsetX = offsetY = pio.x = pio.y = 0.; // pIter was applied to viewTarget, reset pIter.
    bCentered = bScaledToFit = false;       // kLayout doesn't support pIter.
  }

  if (plotType != OdDbPlotSettings::kView)
    viewTarget = viewTarget.orthoProject(OdGePlane(oldTarget, viewDir));

  // in plotpaper units
  pOverallView->setView(viewTarget + viewDir, viewTarget, upV, fieldWidth, fieldHeight, isPerspective ? OdGsView::kPerspective : OdGsView::kParallel);

  if (!bMetric)
  {
    fieldWidth *= kMmPerInch;
    fieldHeight *= kMmPerInch;
  }

  if (bScaledToFit) // Scale factor can be stored in layout, but preview recalculate pIter if bScaledToFit = true.
  {                 // Some times stored factor isn't correct, due to autocad isn't update pIter before saving.
    factor = odmin( (drx2 - drx1) / fieldWidth, (dry2 - dry1) / fieldHeight );
  }

  if (bCentered)    // Offset also can be incorectly saved.
  {
    offsetX = ( (drx2 - drx1) - fieldWidth  * factor ) / 2.;
    offsetY = ( (dry2 - dry1) - fieldHeight * factor ) / 2.;

    if (bPrint90 || bPrint180)
    {
      offsetY = -offsetY;
      offsetX = -offsetX;
    }
  }

  if (bPrint180 || bPrint90)
  {
    drx1 = drx2 - fieldWidth  * factor;
    dry2 = dry1 + fieldHeight * factor;
  }
  else if (bPrint0 || bPrint270)
  {
    drx2 = drx1 + fieldWidth  * factor;
    dry1 = dry2 - fieldHeight * factor;
  }
  else // preview
  {
    drx2 = drx1 + fieldWidth  * factor;
    dry1 = dry2 - fieldHeight * factor;
  }

  if (!SkipClipping)
  {
    if (bPrint180 || bPrint90)
    {
      ClipBox.moveLeft(ClipBox.right() - (drx2-drx1)* koeffX * dScreenFactorW); // setLeft
      ClipBox.setHeight((dry2-dry1)* koeffY * dScreenFactorH); // setBottom(ClipBox.top() + ...
    }
    else if (bPrint0 || bPrint270)
    {
      ClipBox.setWidth((drx2-drx1)* koeffX * dScreenFactorW); // setRight(ClipBox.left() + ... 
      ClipBox.moveTop(ClipBox.bottom() - (dry2-dry1)* koeffY * dScreenFactorH); // setTop
    }
    else // preview
    {
      ClipBox.setWidth((drx2-drx1)* koeffX * dScreenFactorW); // setRight(ClipBox.left() + ... 
      ClipBox.moveTop(ClipBox.bottom() - (dry2-dry1)* koeffY * dScreenFactorH); // setTop
    }
    //OffsetRect
    ClipBox.translate(offsetX * koeffX * dScreenFactorW,
                      -offsetY * koeffY * dScreenFactorH);
  }

  pOverallView->setViewport(OdGePoint2d(0,0), OdGePoint2d(1,1));

  // Apply clip region to screen
  ////CRect ResultClipBox;
  ////ResultClipBox.IntersectRect(&MarginsClipBox, &ClipBox);
  ////CRgn newClip;
  ////newClip.CreateRectRgnIndirect(&ResultClipBox);
  QRectF newClip = MarginsClipBox.intersected(ClipBox);
  // useless for clipping (used to correct OnSize) //painter.setClipRegion(QRegion(newClip));
  painter.setClipping(false);

  // Calculate viewport rect in printer units
  long x1 = long( ( offsetX + drx1) * koeffX );
  long x2 = long( ( offsetX + drx2) * koeffX );
  long y1 = long( (-offsetY + dry1) * koeffY );
  long y2 = long( (-offsetY + dry2) * koeffY );

  OdGsDCRect viewportRect;
  // by Preview + offset to zero (instead pDC->SelectClipRgn(&newClip));
  // top-left of newClip is used to correct OnSize instead SelectClipRgn
  double dOffsetX = - newClip.left(),
         dOffsetY = - newClip.top();
  viewportRect = OdGsDCRect(x1 + dOffsetX, x2 + dOffsetX, y2 + dOffsetY, y1 + dOffsetY);

  QSize sizeDevice(qAbs(viewportRect.m_max.x - viewportRect.m_min.x),
                   qAbs(viewportRect.m_min.y - viewportRect.m_max.y));
  QSize sizeImage = sizeDevice;
  OdRxDictionaryPtr pProperties = pPrinterHelper->properties();
  if (pProperties->has(OD_T("FBLimit")))
  {
    int limDeviceMax = OdRxVariantValue(pProperties->getAt(OD_T("FBLimit"))).get()->getInt32();
    if (limDeviceMax < 100)
    {
      ODA_FAIL_ONCE();
      limDeviceMax = 3385;
    }

    int maxDevice = qMax(sizeDevice.width(), sizeDevice.height());
    int coeff = 1;
    while (limDeviceMax < maxDevice)
    {
      coeff *= 2;
      maxDevice /= 2;
    }
    if (coeff > 1)
    {
      viewportRect = OdGsDCRect(viewportRect.m_min.x / coeff,
                                viewportRect.m_max.x / coeff,
                                viewportRect.m_min.y / coeff,
                                viewportRect.m_max.y / coeff);
      sizeDevice = QSize(qAbs(viewportRect.m_max.x - viewportRect.m_min.x),
                         qAbs(viewportRect.m_min.y - viewportRect.m_max.y));
    }
  }

  // -----

  QImage image;
  if (pProperties->has(L"QImageFrame"))
    pProperties->putAt(L"QImageFrame", OdRxVariantValue((OdIntPtr) &image));
  else
  {
    ODA_ASSERT_ONCE(pProperties->has(OD_T("RasterImage")));

    if (pProperties->has(OD_T("WindowHDC")))
    {
      ODA_FAIL_ONCE();
#    if (defined(Q_OS_WIN32) || defined(Q_OS_WIN64)) && defined(QPRINTENGINE_WIN_P_H)
      QPrintEngine* printEngine = s_pPrinter->printEngine();
      QWin32PrintEngine* win32PrintEngine = dynamic_cast<QWin32PrintEngine*>(printEngine);
      ODA_ASSERT_ONCE(win32PrintEngine);
      if (!win32PrintEngine)
        return;
      HDC hdc = win32PrintEngine->getDC();
      pProperties->putAt(L"WindowHDC", OdRxVariantValue((OdIntPtr) hdc));
#    endif
    }
  }

  getIApp()->toStatusBar(QObject::tr("Please wait - image preparing..."));
  qApp->processEvents(); // show it 

//#ifdef ODQ_PLOT_USE_TILES
//  bool bPlotViaTiles = false; // TODO
//#endif
  try
  {
//#  ifdef ODQ_PLOT_USE_TILES
//    if (!bPlotViaTiles)
//#  endif
    {
      pPrinterHelper->onSize(viewportRect);
      pPrinterHelper->update(0);
    }
//#  ifdef ODQ_PLOT_USE_TILES
//    else
//    {
//      CRect rc(viewportRect.m_min.x, viewportRect.m_max.y, viewportRect.m_max.x, viewportRect.m_min.y);
//      generateTiles(//pDC->m_hDC, 
//                    rc, m_pPrinterHelper, 1000, 1000 );
//    }
//#  endif
  }
  catch (OdError& err)
  {
    ODA_FAIL_ONCE();
    painter.end();
    s_pPrinter = NULL;
    throw err;
  }

  if (!pProperties->has(OD_T("QImageFrame")) && pProperties->has(OD_T("RasterImage"))) // can support both
  {
    OdGiRasterImagePtr pRasterImage = pProperties->getAt(OD_T("RasterImage"));

    OdUInt32 width = pRasterImage->pixelWidth(),
             height = pRasterImage->pixelHeight();
    OdUInt32 szData = pRasterImage->scanLineSize();
    ODA_ASSERT_ONCE(width > 0 && height > 0); 
    const OdUInt8* pData = pRasterImage->scanLines();

    //// test
    //image = QImage(width, height, QImage::Format_RGB32);
    //image.fill(ODRGB(0, 255, 0));

    QImage::Format fmt = QImage::Format_RGB32; // QImage::Format_ARGB32);
    if (szData < (width * 4))
    {
      fmt = QImage::Format_RGB16;
      ODA_ASSERT_ONCE(szData >= (width * 2)); // TODO via convertor RxRasterServices
    }
    image = QImage((const uchar *) pData, width, height, fmt); 
    image = image.mirrored();
  }
  ODA_ASSERT_ONCE(!image.isNull());

  QPixmap pixmap = QPixmap::fromImage(image);

  if (sizeImage != sizeDevice)
    pixmap = pixmap.scaled(sizeImage);
  if (pProperties->has(OD_T("OpenRLContextsCount")) && viewportRect.m_min.x < 0)
  {
    // TODO check the next (see upper)
    //if ((x1 + dOffsetX) < 0.0)
    //  dOffsetX = -x1;
    //if ((y1 + dOffsetY) < 0.0)
    //  dOffsetY = -y1;

    QPainter painter(&pixmap);
    painter.fillRect(sizeImage.width() - 2 *(-viewportRect.m_min.x), 0,
                     2 * (-viewportRect.m_min.x), sizeImage.height(), 
                     QColor(Qt::white)); // clrBackground);
  }

 #if defined(_DEBUG)
  QString qsPath = QDir::toNativeSeparators(QDir::homePath() + "/ODA/plot");
  if (QDir(qsPath).exists())
  {
    qsPath = QDir::toNativeSeparators(qsPath + "/plot.png");
    pixmap.save(qsPath);
  }
 #endif

  getIApp()->toStatusBar(QObject::tr("Please wait - printing..."));
  qApp->processEvents(); // show it 

  // adjust rect page
  rectPage = QRectF(rectPage.x() - dOffsetX, rectPage.y() - dOffsetY, 
                    pixmap.width(), pixmap.height());
  rectPage = Q_SWAP_RECT(rectPage); 
  ODA_ASSERT_ONCE(!OdDbPlotSettings::k0degrees);
  rotation = pLayoutSettings->plotRotation(); // restore
  double dAngle = rotation * 90.0; 
  if (dAngle > 0.0)
    pixmap = pixmap.transformed(QTransform().rotate(dAngle));

 #if !defined(Q_OS_LINUX)
  //painter.drawPixmap(rectPage.topLeft(), pixmap); // painter.drawPixmap(rectPage, pixmap, pixmap.rect());
  // to get more visible viewport bounds and others via XPS Document Writer
  painter.drawTiledPixmap(rectPage, pixmap);
 #else
  // the result of drawTiledPixmap on Linux is shifted unlike other platforms
  painter.drawPixmap(rectPage, pixmap, pixmap.rect());
 #endif

  painter.end();

  pCtx->setPlotGeneration(false);
  pCtx = NULL;
  pOverallView = NULL;
  pPrinterDevice = NULL;
  pPrinterHelper = NULL;
  s_pPrinter = NULL;
  pProperties = NULL;

  getIApp()->toStatusBar(QObject::tr("Done"), 100);

  // -----

  if (!iView)
  {
    if (IqViewSystem* iVS = getIViewSystem())
      iView = iVS->getView(pRxDatabase);
  }
  ODA_ASSERT_ONCE(iView);
  if (iView)
    iView->updateLayouts(); // PROD-131
  qApp->processEvents(); // show it 
}

void OdqCmd_PageSetup::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  if (!pRxDatabase)
    FIX_MAC_APP_TROW(OdError(eNoDatabase)); // throw OdError(eNoDatabase);

  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pRxDatabase);
  if (pDwgDb.isNull())
    FIX_MAC_APP_TROW(OdError(eNotImplemented)); // throw OdError(eNotImplemented); // TODO for DGN

  //OdqMainWindow* pMainWindow = qobject_cast<OdqMainWindow*>(getIApp()->getMainWindow());
  //ODA_ASSERT_ONCE(pMainWindow);

  //OdqPageSetupeDialog dlg(pRxDatabase, pMainWindow);
  //if (dlg.exec() != QDialog::Accepted)
  //  return;

  ODA_FAIL_ONCE(); // TODO
  FIX_MAC_APP_TROW(OdError(eNotImplemented)); // throw OdError(eNotImplemented);
}

void OdqCmd_PlotPreview::execute(OdEdCommandContext* pCmdCtx)
{
  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  if (!pRxDatabase)
    FIX_MAC_APP_TROW(OdError(eNoDatabase)); // throw OdError(eNoDatabase);

  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(pRxDatabase);
  if (pDwgDb.isNull())
    FIX_MAC_APP_TROW(OdError(eNotImplemented)); // throw OdError(eNotImplemented); // TODO for DGN

  ODA_FAIL_ONCE(); // TODO
  FIX_MAC_APP_TROW(OdError(eNotImplemented)); // throw OdError(eNotImplemented);
}
