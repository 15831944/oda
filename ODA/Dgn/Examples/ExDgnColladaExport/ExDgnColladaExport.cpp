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

/*****************************************************************************/
/* This console application reads a .dgn file and writes Collada file        */
/*                                                                           */
/* Calling sequence:                                                         */
/*                                                                           */
/*   ExDgnColladaExport <input file> <output file>                           */
/*****************************************************************************/

#include "OdaCommon.h"
#include "StaticRxObject.h"
#include "RxInit.h"
#include "DynamicLinker.h"
#include "ExDgnServices.h"
#include "ExDgnHostAppServices.h"
#include "DgDatabase.h"
#include "ColladaModule.h"
#include "ColorMapping.h"
#include "DgModel.h"
#include "Gs/GsBaseInclude.h"

#include "diagnostics.h"
using namespace TD_COLLADA_EXPORT;


#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

/************************************************************************/
/* Define a Custom Services class.                                      */
/*                                                                      */
/* Combines the platform dependent functionality of                     */
/* ExSystemServices and ExHostAppServices                               */ 
/************************************************************************/
class MyServices : public OdExDgnSystemServices, public OdExDgnHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdExDgnSystemServices);
};

/************************************************************************/
/* Define a our own Assert function.                                    */
/************************************************************************/

static void MyAssert(const char* expression, const char* fileName, int nLineNo)
{
  OdString message;
  message.format(L"\n!!! Assertion failed: \"%s\"\n    file: %ls, line %d\n", OdString(expression).c_str(), OdString(fileName).c_str(), nLineNo);
  oddgPrintConsoleString(message);
}

/************************************************************************/
/* Define a module map for statically linked modules                    */
/************************************************************************/
#if !defined(_TOOLKIT_IN_DLL_)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ColladaModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPMODULE(OdColladaExportModuleName, ColladaModule)
  ODRX_DEFINE_STATIC_APPMODULE(L"TG_Db", OdDgnModule)
  ODRX_DEFINE_STATIC_APPMODULE(L"TG_ModelerGeometry", DgModelerModule)
  ODRX_DEFINE_STATIC_APPMODULE(L"TG_Dgn7IO", OdDgn7IOModuleImpl)
  ODRX_DEFINE_STATIC_APPMODULE(RX_RASTER_SERVICES_APPNAME,  ExRasterModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdRasterProcessorModuleName, OdRasterProcessingServicesImpl)
ODRX_END_STATIC_MODULE_MAP()
#endif

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
#if defined(_UNICODE) && defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif

  oddgPrintConsoleString(L"\nExDgnColladaExport test program. Copyright (c) 2019, Open Design Alliance\n");
  
  /**********************************************************************/
  /* Verify the argument count and display an error message as required */
  /**********************************************************************/
  if (argc < 3) 
  {
    oddgPrintConsoleString(L"usage: ExDgnColladaExport <input file> <output file>\n");
    oddgPrintConsoleString(L"   <input file>  - .dgn file\n");
    oddgPrintConsoleString(L"   <output file> - .dae file\n");
    return 1;
  }

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif
  odSetAssertFunc(MyAssert);

  // Create a custom Services object.
  OdStaticRxObject<MyServices> svcs;
  // initialize ODA Platform

  int iRetIndex = 0;
  odrxInitialize(&svcs);

  //oddgPrintConsoleString(L"Developed using %ls ver %ls\n", svcs.product().c_str(), svcs.versionString().c_str());

  try 
  {
    ::odrxDynamicLinker()->loadModule(L"TG_Db", false);

    //OdDbDatabasePtr pDb;
    OdString f(argv[1]);
    OdDgDatabasePtr pDb = svcs.readFile( f );

    if (!pDb.isNull())
    {
      ColladaModulePtr module = ::odrxDynamicLinker()->loadModule(OdColladaExportModuleName);
      if (module.isNull())
      {
        oddgPrintConsoleString( L"Cannot load module : ");
        oddgPrintConsoleString( OdColladaExportModuleName );
        iRetIndex = 2;
      }

      if (!iRetIndex)
      {
        const ODCOLORREF* refColors = OdDgColorTable::currentPalette( pDb );
        ODGSPALETTE pPalCpy;
        pPalCpy.insert(pPalCpy.begin(), refColors, refColors + 256);


        OdDgElementId elementId = pDb->getActiveModelId();
        if( elementId.isNull() )
        {
          elementId = pDb->getDefaultModelId();
          pDb->setActiveModelId(elementId);
        }
        OdDgElementId elementActId = pDb->getActiveModelId();
        OdDgModelPtr pModel = elementId.safeOpenObject();
        ODCOLORREF background = pModel->getBackground();

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

        if( vectorizedViewId.isNull() && !pViewGroup.isNull() )
        {
          OdDgElementIteratorPtr pIt = pViewGroup->createIterator();

          if( !pIt->done() )
          {
            OdDgViewPtr pView = OdDgView::cast( pIt->item().openObject(OdDg::kForWrite) );

            if( pView.get() )
            {
              pView->setVisibleFlag(true);
              vectorizedViewId = pIt->item();
            }
          }
        }

        if( vectorizedViewId.isNull() )
        {
          oddgPrintConsoleString(L"Can not find an active view group or all its views are disabled\n");
          iRetIndex = 3;
        }
        else
        {
          OdGeExtents3d extModel;
          pModel->getGeomExtents(vectorizedViewId, extModel);
          // Color with #255 always defines backround. The background of the active model must be considered in the device palette.
          pPalCpy[255] = background;
          // Note: This method should be called to resolve "white background issue" before setting device palette
          bool bCorrected = OdDgColorTable::correctPaletteForWhiteBackground(pPalCpy.asArrayPtr());

          // Calculate deviations for dgn elements

          std::map<OdDbStub*, double > mapDeviations;

          OdDgElementIteratorPtr pIter = pModel->createGraphicsElementsIterator();

          for(; !pIter->done(); pIter->step() )
          {
            OdGeExtents3d extElm;
            OdDgElementPtr pItem = pIter->item().openObject(OdDg::kForRead);

            if( pItem->isKindOf(OdDgGraphicsElement::desc()) )
            {
              pItem->getGeomExtents(extElm);

              if( extElm.isValidExtents() )
                mapDeviations[pItem->elementId()] = extElm.maxPoint().distanceTo(extElm.minPoint()) / 1e4;
            }
          }

          //

          OdString arg2(argv[2]);
          OdResult res = module->exportCollada(pDb, arg2, pPalCpy.asArrayPtr(), 256, OdGeMatrix3d::translation( -1.0 * extModel.minPoint().asVector() ), &mapDeviations );
          if (eOk == res)
          {
            oddgPrintConsoleString( L"Successfully exported." );
          }
          else
          {
            OdString tmp = OD_T("Error : ") + OdError(res).description();
            oddgPrintConsoleString( tmp );
            iRetIndex = 3;
          }
        }
      }
    }
    if (!iRetIndex)
      oddgPrintConsoleString(L"\nDone.\n");
  }

  /********************************************************************/
  /* Display the error                                                */
  /********************************************************************/
  catch (const OdError& e)
  {
    oddgPrintConsoleString(L"\nException Caught: %ls\n", e.description().c_str());
  }
  catch (...)
  {
    oddgPrintConsoleString(L"\nUnknown Exception Caught\n");
  }
  odrxUninitialize();
  return iRetIndex;
}

//
