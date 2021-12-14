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
// ExInsertRaster.cpp: implementation of the ExInsertRaster function.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h" // here contains OdaCommon.h  etc
#include "RxRasterServices.h"
#include "DbOle2Frame.h"
#include "OleItemHandler.h"
#include "Gi/GiRasterWrappers.h"

struct OleSsInsertTracker : OdEdRealTracker
{
  OdSmartPtr<OdDbOle2Frame> _pOleFrame;
  OdGePoint3d _orig;
  OdGeVector2d _size;
  double _dist;

  void setValue(double dist)
  {
    _dist = dist;
    OdRectangle3d r3d;

    OdGeVector2d size = _size.normal() * _dist;
    r3d.lowLeft .set(_orig.x,          _orig.y,          _orig.z);
    r3d.upLeft  .set(_orig.x,          _orig.y + size.y, _orig.z);
    r3d.upRight .set(_orig.x + size.x, _orig.y + size.y, _orig.z);
    r3d.lowRight.set(_orig.x + size.x, _orig.y,          _orig.z);

    _pOleFrame->setPosition(r3d);
  }

  int addDrawables(OdGsView* pView)
  {
    pView->add(_pOleFrame, 0);
    return 1;
  }
  void removeDrawables(OdGsView* pView)
  {
    pView->erase(_pOleFrame);
  }
};

void _InsertRaster_func(OdEdCommandContext* pCmdCtx)
{
  OdDbDatabasePtr pDb = pCmdCtx->baseDatabase();
  OdDbUserIOPtr pIO = pCmdCtx->userIO();

  OdString filter;
  filter += L"Bitmap files (*.bmp)|*.bmp|";
  filter += L"Jpeg files (*.jpg)|*.jpg|";
  filter += L"PNG files (*.png)|*.png|";
  filter += L"All files (*.*)|*.*|";
  filter += L"|";
  OdString path = pIO->getFilePath(L"Select file to insert", 
                                    OdEd::kGfpForOpen, 
                                    L"Select file to insert", L"bmp", L"", filter);
  if (path.isEmpty())
    return;
  
  OdRxRasterServicesPtr pRasSvcs = ::odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
  ODA_ASSERT_ONCE(pRasSvcs.get());
  if (pRasSvcs.isNull())
  {
    pIO->putError(L"Failed to load raster services...");
    return;
  }
  OdGiRasterImagePtr pImage = pRasSvcs->loadRasterImage(path);
  ODA_ASSERT_ONCE(pImage.get());
  if (pImage.isNull())
  {
    pIO->putError(OdString().format(L"Failed to load raster image from file \"%ls\"", path.c_str()));
    return;
  }

  // CORE-13074 InsertRaster fails for specific file
  OdUInt32 width = pImage->pixelWidth(),
           height = pImage->pixelHeight();
  OdUInt32 max = odmax(width, height);
  const OdUInt32 cMaxLim = 2048; // it looks like this limit depends of hardware
  if (   max > cMaxLim 
      || pImage->pixelFormat().isRGB() || pImage->pixelFormat().isRGBA()) // OLE supports BGR & BGRA only
  {
    if (max > cMaxLim)
    {
      OdUInt32 coef = OdUInt32(double(max) / cMaxLim + 1.0 - 1e-7);
      width = width / coef;
      height = height / coef;
    }
    OdSmartPtr<OdGiRasterImageDesc> pDesc = OdGiRasterImageDesc::createObject(pImage);
    pDesc->setPixelWidth(width);
    pDesc->setPixelHeight(height);
    if (pImage->pixelFormat().isRGB())
    {
      //pDesc->setColorDepth(24);
      pDesc->pixelFormat().setBGR(); 
    }
    else if (pImage->pixelFormat().isRGBA())
    {
      //pDesc->setColorDepth(32);
      pDesc->pixelFormat().setBGRA();
    }
    pDesc->setScanLinesAlignment(4);
    pImage = pImage->convert(true, 50., 50., 0.0, 0, false, false, false, pDesc);
    ODA_ASSERT_ONCE(width == pImage->pixelWidth() && height == pImage->pixelHeight());
  }

  OdStaticRxObject<OleSsInsertTracker> tracker;
  //::odrxDynamicLinker()->loadModule(OdOlePlatformItemHandlerModuleName);
  OdDbOle2FramePtr pOleFrame = tracker._pOleFrame = OdDbOle2Frame::createObject();
  pOleFrame->setDatabaseDefaults(pDb);
  OdOleItemHandler* pItem = pOleFrame->getItemHandler();
  if (!pItem->embedRaster(pImage, pDb))
  {
    pIO->putError(L"embedRaster is unsupported...");
    return;
  }
  pOleFrame->setOutputQuality(OdDbOle2Frame::kHighGraphics);

  try
  {
    OdGeVector2d size(pImage->pixelWidth(), pImage->pixelHeight());

    pOleFrame->unhandled_setHimetricSize(OdUInt16(size.x), OdUInt16(size.y));

    tracker._orig = pIO->getPoint(L"Specify insertion point <0,0>:", OdEd::kGptDefault, &OdGePoint3d::kOrigin);

    // Set initial OLE frame position to origin, and size in world units:
    // Convert MM_HIMETRIC (0.01 millimeter) to Space Units:
    double s = pDb->getMEASUREMENT()==OdDb::kEnglish ? 0.01 / 25.4 : 0.01;
    tracker._size.set(double(size.x) * s, double(size.y) * s);

    OdString sPmt;
    sPmt.format(OD_T("Specify size <%g>:"), tracker._size.length());
    tracker.setValue(pIO->getDist(sPmt, OdEd::kGdsFromLastPoint | OdEd::kGptRubberBand, 0., OdString::kEmpty, &tracker));
    OdDbBlockTableRecordPtr pSpace = pDb->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite);
    pSpace->appendOdDbEntity(pOleFrame);

    //pOleDocument->SetModifiedFlag(FALSE); // suppress boring save dialog on exit...
  }
  catch (OdError& err)
  {
    if (pOleFrame->isDBRO())
      pOleFrame->erase();
    pIO->putError(L"Failed to insert OLE object from file...");
    throw err;
  }
}

//#include "DbLine.h"
//{
//  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
//  OdDbDatabasePtr pDb = pDbCmdCtx->database();
//  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();
//
//  OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(pDb);
//  OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);
//
//  OdGeVector3d vNormal(ExCommandsUtils::ucsNormal(pDb));
//
//  OdString strNext_U(L"Specify next point or [Undo]:");
//  OdString strKeywords_U(L"Undo");
//
//  OdString strNext_CU(L"Specify next point or [Undo/Close]:");
//  OdString strKeywords_CU(L"Undo Close");
//  OdGePoint3d ptFirst, ptStart, ptEnd;
//  int nLines = 0;
//
//  for (bool bFinished = false; !bFinished;)
//  {
//    ptStart = ptFirst = pIO->getPoint(L"Specify first point:");
//
//    for (;;)
//    {
//      try
//      {
//        if (nLines >= 2)
//        {
//          ptEnd = pIO->getPoint(strNext_CU, OdEd::kGptRubberBand|OdEd::kInpThrowEmpty, NULL, strKeywords_CU);
//        }
//        else
//        {
//          ptEnd = pIO->getPoint(strNext_U, OdEd::kGptRubberBand|OdEd::kInpThrowEmpty, NULL, strKeywords_U);
//        }
//      }
//      catch (const OdEdKeyword &kwd)
//      {
//        if (kwd.keywordIndex() == 1)
//        { //Close
//          ptEnd = ptFirst;
//          bFinished = true;
//        }
//        else
//        { //Undo
//          if (nLines == 0)
//          { //Undo first point
//            break;
//          }
//          OdDbObjectId idLast = oddbEntLast(pDb);
//          OdDbLinePtr pLine = idLast.openObject(OdDb::kForWrite);
//          if (!pLine.isNull())
//          {
//            ptStart = pLine->startPoint();
//            pLine->erase();
//            pIO->setLASTPOINT(ptStart);
//            --nLines;
//          }
//          else
//          {
//            ODA_FAIL();
//          }
//          continue;
//        }
//      }
//      catch (const OdEdCancel& )
//      {
//        return;
//      }
//
//      OdDbLinePtr pNewLine = OdDbLine::createObject();
//      pNewLine->setDatabaseDefaults(pDb);
//      pNewLine->setNormal(vNormal);
//      pNewLine->setStartPoint(ptStart);
//      pNewLine->setEndPoint(ptEnd);
//      pSpace->appendOdDbEntity(pNewLine);
//      ptStart = ptEnd;
//      ++nLines;
//
//      if (bFinished)
//        return;
//    }
//  }
//}
