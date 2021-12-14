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

#include "StdAfx.h"
#include "DbHostAppServices.h"
#include "RxRasterServices.h"
#include "RxVariantValue.h"
#include "Ge/GeExtents3d.h"
#include "DbSymUtl.h"
#include "GiContextForDbDatabase.h"

#include "GetRasterFilePath.h"

void _BlockIcon_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabase* pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  // Get device for Bitmap creation
  OdDbHostAppServices* pAppSvs = pDb->appServices();
  OdGsDevicePtr pDevice = pAppSvs->gsBitmapDevice();
  if (pDevice.isNull())
    // No device available
    throw OdError(eNotApplicable);

  // Load RxRasterServices. Throw exception if failed
  OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME, false);

  OdDbBlockTableRecordPtr pBlock;
  OdString sBlock;

  while (pBlock.isNull())
  {
    sBlock = pIO->getString(L"Enter block name to save:", OdEd::kGstAllowSpaces);
    pBlock = OdDbSymUtil::getBlockId(sBlock, pDb).openObject();
    if (pBlock->isLayout())
    {
      pIO->putString(L"For Layout block use BMPOUT or BMPOUTBG commands.");
      pBlock = NULL;
    }
  }

  int nColorBits = pIO->getInt(L"BitPerPixel <24>", OdEd::kInpDefault, 24);
  int xsize = pIO->getInt(L"Bitmap width <2000>", OdEd::kInpDefault, 2000);
  int ysize = pIO->getInt(L"Bitmap height <2000>", OdEd::kInpDefault, 2000);
  bool plotGeneration = (pIO->getKeyword(L"PlotGeneration [OFF/ON] ", L"OFF ON", 1) != 0);

  OdGsDCRect gsRect(0, xsize, ysize, 0 );
  OdGiContextForDbDatabasePtr pCtx = OdGiContextForDbDatabase::createObject();
  pCtx->setDatabase(pDb);

  pDevice->setUserGiContext(pCtx);

  OdGsViewPtr pView = pDevice->createView();
  pDevice->addView(pView);

  OdGeExtents3d ext;
  if(pBlock->getGeomExtents(ext) == eOk)
  { //else we can't initialize view properly so draw empty bmp
    pView->add(pBlock, 0);

    // World extents to view CS:
    OdGePoint3d targ = pView->target();
    OdGeVector3d dirFromTarg = OdGeVector3d::kZAxis;

    // set target to center of the scene, keep view direction:
    targ = ext.minPoint() + (ext.maxPoint() - ext.minPoint()) / 2.0;
    targ.transformBy(pView->viewingMatrix().invert());

    double fw = ext.maxPoint().x - ext.minPoint().x;
    double fh = ext.maxPoint().y - ext.minPoint().y;
    pView->setView(targ + dirFromTarg, targ, pView->upVector(), fw * 1.02, fh * 1.02);
  }

  pView->setLineweightToDcScale(0.);
  pView->setViewportBorderVisibility(false);
  
  pDevice->properties()->putAt(L"BitPerPixel", OdRxVariantValue(OdUInt32(nColorBits)));
  pCtx->setPlotGeneration(plotGeneration);
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pDb);
  pDbPE->setupPalette(pDevice, pCtx, pBlock->getLayoutId());

  pDevice->onSize(gsRect);
  pDevice->update();

  OdString sOutput = getRasterFilePathToSave(pRasSvcs, pIO, sBlock, L"png");

  if (sOutput.isEmpty())
    return; // canceled

  pRasSvcs->saveRasterImage(OdGiRasterImagePtr(pDevice->properties()->getAt(L"RasterImage")), 
                            sOutput);
}
