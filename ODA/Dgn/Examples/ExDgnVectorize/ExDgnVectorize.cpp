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
/* Simple application creating new DGN file                             */
/* and filling it with some objects                                     */
/************************************************************************/

#include "OdaCommon.h"

#include "StaticRxObject.h"
#include "RxInit.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"
#include "DgDatabase.h"
#include "RxDynamicModule.h"

#include "ExSystemServices.h"
#include "ExDgnServices.h"
#include "ExDgnHostAppServices.h"

#include "DgGiContext.h"
#include "DgGsManager.h"

#include "ExGsSimpleDevice.h"

// Auxiliary function to display different messages
//void oddgPrintConsoleString(const wchar_t* fmt, ...);


/************************************************************************/
/* Here to include header files required for your sample        */
/************************************************************************/
#include "DgLine.h"      // This file puts OdDgLine3d in the output file


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

#ifndef _T
#define _T(x) x
#endif

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif


/************************************************************************/
/* Define a ñustom services class                                       */
/*                                                                      */
/* It combines the platform dependent functionality of                  */
/* ExSystemServices and ExHostAppServices                               */ 
/************************************************************************/

class MyServices : public OdExDgnSystemServices, public OdExDgnHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdExDgnSystemServices);
};


#ifndef _TOOLKIT_IN_DLL_

#include "RxDynamicModule.h"

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);   //this library allows to read V7 files
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgModelerModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db", OdDgnModule)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Dgn7IO",          OdDgn7IOModuleImpl)
  ODRX_DEFINE_STATIC_APPMODULE(L"TG_ModelerGeometry", DgModelerModule)
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

#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif

  /********************************************************************/
  /* Create a custom Services instance.                               */
  /********************************************************************/
  OdStaticRxObject<MyServices> svcs;

  /**********************************************************************/
  /* Parse Command Line strings                                         */
  /**********************************************************************/
  {
    bool bInvalidArgs = (argc < 2);

    if (bInvalidArgs)
    {
      bInvalidArgs = true;
      nRes  = 1;
    }

	STD(cout) << "\nExDgnVectorize sample program. Copyright (c) 2019, Open Design Alliance\n";

    if (bInvalidArgs)    
    {
      STD(cout)  << "\n\n\tusage: ExDgnVectorize <filename> [stlFilename] [-DO]";
      STD(cout)  << "\n\t-DO disables progress meter output.\n";
      return nRes;
    }
  }

    OdString szSource = argv[1];

    OdString strStlFilename = OdString::kEmpty;

    if( argc > 2 )
    {
      strStlFilename = argv[2];

      if( strStlFilename == L"-DO")
        strStlFilename = OdString::kEmpty;
    }

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

  /**********************************************************************/
  /* Initialize Runtime Extension environment                           */
  /**********************************************************************/
  odrxInitialize(&svcs);
  odgsInitialize();


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

    OdDgDatabasePtr pDb = svcs.readFile( szSource );

    if (!pDb.isNull())
    {
      // Set device palette from dgn color table

      OdDgColorTablePtr clolortable = pDb->getColorTable();
      OdGsDevicePtr pDevice = ExGsSimpleDevice::createObject(ExGsSimpleDevice::k3dDevice);

      const ODCOLORREF* refColors = OdDgColorTable::currentPalette( pDb );
      ODGSPALETTE pPalCpy;
      pPalCpy.insert(pPalCpy.begin(), refColors, refColors + 256);
      OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject();
      ODCOLORREF background = pModel->getBackground();
      // Color with #255 always defines background. The background of the active model must be considered in the device palette.
      pPalCpy[255] = background;
      // Note: This method should be called to resolve "white background issue" before setting device palette
      bool bCorrected = OdDgColorTable::correctPaletteForWhiteBackground(pPalCpy.asArrayPtr());
      pDevice->setLogicalPalette(pPalCpy.asArrayPtr(), 256);
      pDevice->setBackgroundColor( background );

      // Find first active view.

      OdDgViewPtr pView;
	  
      OdDgViewGroupPtr pViewGroup = pDb->getActiveViewGroupId().openObject(OdDg::kForRead);
      
      if( !pViewGroup.isNull() )
      {
        OdDgElementIteratorPtr pIt = pViewGroup->createIterator();
        for( ; !pIt->done(); pIt->step() )
        {
          OdDgViewPtr pCurView = OdDgView::cast( pIt->item().openObject(OdDg::kForRead) );
		  
          if( pCurView.get() && pCurView->getVisibleFlag() )
          {
            pView = pCurView;
            break;
          }
        }
      }

      if( !pView.isNull() )
      {
			   //create the context with OdDgView element given (to transmit some properties)

        OdGiContextForDgDatabasePtr pDgnContext = OdGiContextForDgDatabase::createObject( pDb, pView );
			  pDgnContext->enableGsModel(true);
						
			  OdDgElementId vectorizedViewId  = pView->elementId();
        OdDgElementId vectorizedModelId = pView->getModelId();

        pDevice = OdGsDeviceForDgModel::setupModelView(vectorizedModelId, vectorizedViewId, pDevice, pDgnContext);

			  for( int iii = 0; iii < pDevice->numViews(); iii++ )
				  pDevice->viewAt(iii)->setMode(OdGsView::kGouraudShaded);
			
        OdGsDCRect screenRect(OdGsDCPoint(0, 0), OdGsDCPoint(1000, 1000));
        pDevice->onSize(screenRect);
				
        pDevice->update();
      }

      // Write triangle data to stl.

      if( !strStlFilename.isEmpty() )
        OdGiDumper::writeFaceDataToStlFile(strStlFilename, OdGiDumper::getStlTriangles());

      OdGiDumper::clearStlTriangles();
    }

    pDb = 0;
  }
  catch (OdError& e)
  {
    oddgPrintConsoleString(L"\nODA Drawings API for DGN Error: %ls\n", e.description().c_str());
  }
  catch (...)
  {
    STD(cout) << "\n\nUnexpected error.";
    nRes = -1;
    throw;
  }

  /**********************************************************************/
  /* Uninitialize Runtime Extension environment                         */
  /**********************************************************************/

  odgsUninitialize();
  ::odrxUninitialize();

  if (nRes)
  {
    STD(cout) << "\n\nPress ENTER to continue...";
    STD(cin).get();
  }
  STD(cout) << "\n\n";
  return nRes;
}
