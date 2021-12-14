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

/************************************************************************/
/* Extracting Image Files from OdDbOle2Frame Entities.                  */
/*                                                                      */
/************************************************************************/

static const char cUsage[] =
{
"Usage:\n"
"OdExtractOlePreviews <source file>\n\n"
};

/************************************************************************/

#include "OdaCommon.h"
#include "DbDatabase.h"
#include "DbAudit.h"
#include "RxDynamicModule.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"

//#include "DbHostAppServices.h"
#include "DbObjectIterator.h"
#include "DbSymbolTable.h"
#include "DbBlockTableRecord.h"
#include "RxRasterServices.h"
#include "DbOle2Frame.h"
#include "OleItemHandlerBase.h"

#include "OdFileBuf.h"
#include "diagnostics.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

/************************************************************************/
/* Define a module map for statically linked modules:                   */
/************************************************************************/
#ifndef _TOOLKIT_IN_DLL_

//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdOleItemHandlerModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdOleSsItemHandlerModuleImpl);

ODRX_BEGIN_STATIC_MODULE_MAP()
  //ODRX_DEFINE_STATIC_APPMODULE(OdWinBitmapModuleName, BitmapModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPMODULE(RX_RASTER_SERVICES_APPNAME, ExRasterModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRasterProcessorModuleName, OdRasterProcessingServicesImpl)
  //ODRX_DEFINE_STATIC_APPLICATION(OdOleItemHandlerModuleName, OdOleItemHandlerModuleImpl)
  ODRX_DEFINE_STATIC_APPMODULE(OdOleSsItemHandlerModuleName, OdOleSsItemHandlerModuleImpl)
ODRX_END_STATIC_MODULE_MAP()

#endif

/************************************************************************/
/* Define a Custom Services class.                                      */
/*                                                                      */
/* Combines the platform dependent functionality of                     */
/* ExSystemServices and ExHostAppServices                               */ 
/************************************************************************/
class MyServices : public ExSystemServices, public ExHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(ExSystemServices);
  OdGsDevicePtr gsBitmapDevice(OdRxObject* /*pViewObj*/ = NULL,
                               OdDbBaseDatabase* /*pDb*/ = NULL,
                               OdUInt32 /*flags*/ = 0)
  {
    try
    {
      OdGsModulePtr pGsModule = ::odrxDynamicLinker()->loadModule(OdWinBitmapModuleName);
      return pGsModule->createBitmapDevice();
    }
    catch(const OdError&)
    {
    }
    return OdGsDevicePtr();
  }
};

/********************************************************************************/
/* Define Assert function to not crash Debug application if assertion is fired. */
/********************************************************************************/
static void MyAssert(const char* expression, const char* fileName, int nLineNo)
{
  OdString message;
  message.format(L"\n!!! Assertion failed: \"%s\"\n    file: %ls, line %d\n\n", OdString(expression).c_str(), OdString(fileName).c_str(), nLineNo);
  odPrintConsoleString(message);
}

/********************************************************************************/
/* Returns number of extracted previews in block                                */

static int exractOleImageFilesInBlock(OdDbObjectId blockId,
                                      OdRxRasterServices* pRasSvcs,
                                      const OdString& sDatabaseFileName)
{
  if (   odrxDynamicLinker()->getModule(OdOleSsItemHandlerModuleName).isNull()
      || blockId.isNull() || !pRasSvcs)
    return 0;
  OdDbBlockTableRecordPtr pBlock = OdDbBlockTableRecord::cast(blockId.openObject(OdDb::kForRead));
  if (pBlock.isNull() || pBlock->isFromExternalReference())
    return 0;

  int num = 0;
  for (OdDbObjectIteratorPtr pItr = pBlock->newIterator(); !pItr->done(); pItr->step())
  {
    OdDbOle2FramePtr pOle2Frame = OdDbOle2Frame::cast(pItr->entity());
    if (pOle2Frame.isNull())
      continue;

    OdOleItemHandlerBasePtr pHandler = pOle2Frame->getItemHandler();
    if (pHandler.isNull())
      continue;
    if (pOle2Frame.isNull())
      continue;
    
    OdString sImageFileName = sDatabaseFileName;
    sImageFileName += L".";
    sImageFileName += pOle2Frame->objectId().getHandle().ascii();

    OdGiRasterImagePtr pImage = pHandler->getRaster();
    if (pImage.get())
    {
      if (pRasSvcs->isRasterImageTypeSupported(OdRxRasterServices::kPNG))
      {
        sImageFileName += L".png";

        OdStreamBufPtr pIO = ::odrxSystemServices()->createFile(sImageFileName,
                                                                Oda::kFileWrite,
                                                                Oda::kShareDenyNo,
                                                                Oda::kCreateAlways);
        if (pIO.get())
          pRasSvcs->convertRasterImage(pImage, OdRxRasterServices::kPNG, pIO);
        num++;
      }
      continue;
    }

    OdBinaryData data;
    if (pHandler->getWmfData(data))
    {
      sImageFileName += L".wmf";
      OdStreamBufPtr pIO = ::odrxSystemServices()->createFile(sImageFileName,
                               Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
      if (pIO.get())
      {
        pIO->putBytes(data.asArrayPtr(), data.size());
        num++;
      }
    }
  }
  return num;
}

/********************************************************************************/
/* Returns number of extracted previews                                         */

static int exractOleImageFiles(OdDbHostAppServices* pAppServ,
                               const OdString& sDatabaseFileName)
{
  OdRxModulePtr pModule = odrxDynamicLinker()->loadModule(OdOleSsItemHandlerModuleName);
  if (pModule.isNull())
    throw OdError(L"OdOleSsItemHandler module is missing.");
  OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()
                                     ->loadApp(RX_RASTER_SERVICES_APPNAME);
  if (pRasSvcs.isNull())
    throw OdError(L"OdRxRasterServices module is missing");

  OdDbDatabasePtr pDb = pAppServ->readFile(sDatabaseFileName);
  if (pDb.isNull())
    throw OdError(eNoDatabase);

  int num = 0;

  OdDbBlockTableRecordPtr pSpace = pDb->getActiveLayoutBTRId()
    .safeOpenObject(OdDb::kForRead);

  num += exractOleImageFilesInBlock(pDb->getActiveLayoutBTRId(), pRasSvcs, sDatabaseFileName);

  OdDbSymbolTablePtr pBlocks = pDb->getBlockTableId().safeOpenObject(OdDb::kForRead);
  OdDbSymbolTableIteratorPtr pBlkIter = pBlocks->newIterator();
  for (pBlkIter->start(); !pBlkIter->done(); pBlkIter->step())
  {
    if (pBlkIter->getRecordId() == pDb->getActiveLayoutBTRId())
      continue; // already done before
    num += exractOleImageFilesInBlock(pBlkIter->getRecordId(), pRasSvcs, sDatabaseFileName);
  }
  return num;
}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif

#ifndef _TOOLKIT_IN_DLL_
    ODRX_INIT_STATIC_MODULE_MAP();
#endif

  int nRes = 0;
  /**********************************************************************/
  /* Set customized assert function                                     */
  /**********************************************************************/
  odSetAssertFunc(MyAssert);

  /**********************************************************************/
  /* Define a Custom Services object                                    */
  /**********************************************************************/
  OdStaticRxObject<MyServices> svcs;
  odInitialize(&svcs);

  /**********************************************************************/
  /* Verify the argument count and display an error message as required */
  /**********************************************************************/
  if (argc < 2) 
  {
    STD(cout) << cUsage;
    nRes = 1;
  }
  else
  {
    /********************************************************************/
    /* Disable the progress meter                                       */
    /********************************************************************/
    svcs.disableOutput(true);

    try
    {
      for (int idx = 1; idx < argc; idx++)
      {
        OdString sFileName(argv[idx]);
        int num = exractOleImageFiles(&svcs, sFileName);
        if (!num)
          STD(cout) << "OLE preview was not found in file\"" << sFileName.c_str() << "\"\n";
      }
    }
    catch (OdError& err)
    {
      odPrintConsoleString(L"\nOdError thrown: %ls\n",err.description().c_str());
      nRes = -1;
    }
    catch (...)
    {
      STD(cout) << "\nGeneral exception thrown, exiting\n";
      nRes = -1;
    }
  }

  /**********************************************************************/
  /* Uninitialize the Drawings SDK                                          */
  /**********************************************************************/
  odUninitialize();

  STD(cout) << "Press ENTER to exit..." << STD(endl);
  STD(cin).get();

  return nRes;
}

