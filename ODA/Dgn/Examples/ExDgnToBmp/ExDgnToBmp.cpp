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
/* Simple application to load a DGN file and save as BMP                */
/************************************************************************/

#include "OdaCommon.h"

#include "StaticRxObject.h"
#include "RxInit.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"
#include "DgDatabase.h"
#include "RxDynamicModule.h"

#include "ExDgnServices.h"
#include "ExDgnHostAppServices.h"

#include "OdPlatformStreamer.h"
#include "RxObjectImpl.h"
#include "RxVariantValue.h"
#include "DgGsManager.h"
#include "DgGiContext.h"
#include "Gi/GiRasterImage.h"
#include "Gs/Gs.h"
#include "Gs/GsBaseInclude.h"
#include "ColorMapping.h"
#include "RxRasterServices.h"


#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std::a

#ifndef _tcout
#ifdef _UNICODE
#define _tcout wcout
#else
#define _tcout cout
#endif
#endif

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

/************************************************************************/
/* Define a custom services class                                       */
/*                                                                      */
/* It combines the platform dependent functionality of                  */
/* ExSystemServices and ExHostAppServices                               */ 
/************************************************************************/

class MyServices : public OdExDgnSystemServices, public OdExDgnHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdExDgnSystemServices);
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


#ifndef _TOOLKIT_IN_DLL_

#include "RxDynamicModule.h"

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);   //this library allows to read V7 files
//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwgModule);
//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwgDbModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db",              OdDgnModule)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Dgn7IO",          OdDgn7IOModuleImpl)
//  ODRX_DEFINE_STATIC_APPLICATION(OdDbModuleName,  OdDwgModule)
//  ODRX_DEFINE_STATIC_APPLICATION(L"TG_DwgDb",     OdDwgDbModuleImpl)
  ODRX_DEFINE_STATIC_APPMODULE  (OdWinBitmapModuleName,  BitmapModule)
  ODRX_DEFINE_STATIC_APPMODULE(RX_RASTER_SERVICES_APPNAME,   ExRasterModule)
ODRX_END_STATIC_MODULE_MAP()

#endif


/************************************************************************/
/* Main                                                                 */
/************************************************************************/

#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
  int   nRes = 0;               // Return value for the function
  bool  disableOutput = false;  // Disable output if it is indicated in command line

#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif

  /********************************************************************/
  /* Create a custom Services instance.                               */
  /********************************************************************/
  OdStaticRxObject<MyServices> svcs;

  /**********************************************************************/
  /* Display the Product and Version that created the executable        */
  /**********************************************************************/
  STD(cout) << "\nExDgnToBmp sample program. Copyright (c) 2019, Open Design Alliance\n";
  //STD(cout) << "\nExDgnToBmp developed using ODA Drawings API for DGN "
  //            << (const char*)svcs.product() << " ver "
  //            << (const char*)svcs.versionString() << "\n";

  /**********************************************************************/
  /* Parse Command Line inputs                                          */
  /**********************************************************************/
  bool bInvalidArgs = (argc < 3);
  if (bInvalidArgs)
  {
    bInvalidArgs = true;
    nRes  = 1;
  }

  if (bInvalidArgs)    
  {
    STD(cout)  << "\n\tusage: ExDgnToBmp <source file> <destination file> [-DO]";
    STD(cout)  << "\n\t-DO disables progress meter output.\n";
    return nRes;
  }

  /**********************************************************************/
  /* Disable progress meter                                             */
  /**********************************************************************/
  if (argc >= 4)
  {
    OdString argv2(argv[3]);
    if (!odStrICmp(argv2, OD_T("-DO")))
    {
      disableOutput = true;
    }
  }

  OdString srcFileName = argv[1];
  OdString dstFileName = argv[2];

  /************************************************************************/
  /* Disable/Enable progress meter                                       */
  /************************************************************************/
  svcs.disableProgressMeterOutput( disableOutput );

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

  /**********************************************************************/
  /* Initialize Runtime Extension environment                           */
  /**********************************************************************/
  odrxInitialize(&svcs);

  /**********************************************************************/
  /* Find the folder of the output file                                 */
  /**********************************************************************/
  OdString fileName(argv[1]);

  try
  {
    /**********************************************************************/
    /* Initialize ODA Drawings API for DGN                                */
    /**********************************************************************/
    ::odrxDynamicLinker()->loadModule(L"TG_Db", false);

    /********************************************************************/
    /* Load input database from file                                    */
    /********************************************************************/
    OdDgDatabasePtr pDb = svcs.readFile( srcFileName );

    //extended check for bad files
    OdDgModelPtr pModel;
    {
      OdDgElementId activeModelId = pDb->getActiveModelId();

      if( activeModelId.isNull() )
      {
        // MKU 24\11\2010 #8852 (case: /dgn/V7_V8legacy/Regression/out/ExLoadToModel_dll/Building/Dgn/BSI300AE9-Atrium.dgn
        //  If Model is is invalid let's try to get & use a Default.
        activeModelId = pDb->getDefaultModelId();
        pDb->setActiveModelId (activeModelId);
      }

      if( activeModelId.isNull() )
      {
        STD(cout) << "Incorrect Active Model Id\n";
        throw OdError( eInvalidInput );
      }

      pModel = OdDgModel::cast( activeModelId.openObject( OdDg::kForRead ) );
      if( pModel.isNull() )
      {
        STD(cout ) << "Can not open Active Model\n";
        throw OdError( eInvalidInput );
      }
    }

    OdGsModulePtr pGsModule = ::odrxDynamicLinker()->loadModule( OdWinBitmapModuleName );

#ifdef _TOOLKIT_IN_DLL_
    if ( pGsModule.isNull() )
    {
      pGsModule = ::odrxDynamicLinker()->loadModule( OdWinGDIModuleName );
    }
#endif

    OdGsDevicePtr pBitmapDevice;
    if ( pGsModule.get() )
    {
      pBitmapDevice = pGsModule->createBitmapDevice();
    }

    if ( pBitmapDevice.get() )
    {
      OdGiContextForDgDatabasePtr pDgnContext = OdGiContextForDgDatabase::createObject();
      pDgnContext->setDatabase(pDb);
      OdDgElementId vectorizedViewId;
      OdDgViewGroupPtr pViewGroup = pDb->getActiveViewGroupId().openObject();
      if( pViewGroup.isNull() )
      {
        //  Some files can have invalid id for View Group. Try to get & use a valid (recommended) View Group object.
        pViewGroup = pDb->recommendActiveViewGroupId().openObject();
        if( pViewGroup.isNull() )
        {
          // Add View group
          pModel->createViewGroup();
          pModel->fitToView();
          pViewGroup = pDb->recommendActiveViewGroupId().openObject();
        }
      }
      if( !pViewGroup.isNull() )
      {
        OdDgElementIteratorPtr pIt = pViewGroup->createIterator();
        for( ; !pIt->done(); pIt->step() )
        {
          OdDgViewPtr pView = OdDgView::cast( pIt->item().openObject() );
          if( pView.get() && pView->getVisibleFlag() )
          {
            vectorizedViewId = pIt->item();
            break;
          }
        }
      }

      if( vectorizedViewId.isNull() )
      {
        STD(cout ) << "Can not find an active view group or all its views are disabled\n";
        throw OdError( eInvalidInput );
      }

      pDgnContext->setView( vectorizedViewId.openObject( OdDg::kForRead ) );

      OdGsDevicePtr pDevice = OdGsDeviceForDgModel::setupModelView(pDb->getActiveModelId(), vectorizedViewId, pBitmapDevice, pDgnContext);

      // MKU 30\09\11 - set actual background and palette
      //ODCOLORREF bgColor( ODRGB(173,174,173) );
      //pDgnContext->setPaletteBackground( bgColor );
      //pDevice->setBackgroundColor( bgColor );
      //pDevice->setLogicalPalette( odcmAcadPalette(bgColor), 256);
      const ODCOLORREF* refColors = OdDgColorTable::currentPalette( pDb );
      ODGSPALETTE pPalCpy;
      pPalCpy.insert(pPalCpy.begin(), refColors, refColors + 256);
      OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject();
      ODCOLORREF background = pModel->getBackground();
      // Color with #255 always defines backround. The background of the active model must be considered in the device palette.
      pPalCpy[255] = background;
      // Note: This method should be called to resolve "white background issue" before setting device palette
      bool bCorrected = OdDgColorTable::correctPaletteForWhiteBackground(pPalCpy.asArrayPtr());
      pDevice->setLogicalPalette(pPalCpy.asArrayPtr(), 256);
      pDevice->setBackgroundColor( background );
      pDgnContext->setPaletteBackground( background );

      OdGsDCRect screenRect(OdGsDCPoint(0, 768), OdGsDCPoint(1024, 0));
      pDevice->onSize(screenRect);

      pDevice->properties()->putAt(OD_T("BitPerPixel"), OdRxVariantValue(OdUInt32(24)));

      pDevice->update();

      OdGiRasterImagePtr ptrImage = pDevice->properties()->getAt( OD_T("RasterImage") );

      OdRxRasterServicesPtr pRasSvcs = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
      ODA_ASSERT_ONCE(pRasSvcs.get())
      if (pRasSvcs.isNull())
        throw OdError(eNotApplicable);
      pRasSvcs->saveRasterImage(OdGiRasterImagePtr(pDevice->properties()->getAt(OD_T("RasterImage"))), dstFileName);
    }
    else
    {
      STD(cout) << "BMP skipped: inaccessible bitmap device." << STD(endl);
    }
  
    pModel = 0;
    pDb = 0;
  }
  catch (...)
  {
    STD(cout) << "\n\nUnexpected error.";
    nRes = -1;
  }

  /**********************************************************************/
  /* Uninitialize Runtime Extension environment                         */
  /**********************************************************************/
  ::odrxUninitialize();

  STD(cout) << "\n\n";
  return nRes;
}
