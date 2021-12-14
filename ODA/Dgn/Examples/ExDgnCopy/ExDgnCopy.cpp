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

#include "ExDgnServices.h"
#include "ExDgnHostAppServices.h"
#include "DgIdMapping.h"

/************************************************************************/
/* Here to include header files required for your sample        */
/************************************************************************/
#include "DgLine.h"      // This file puts OdDgLine3d in the output file


#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std::a

// Auxiliary function to display different messages
//void oddgPrintConsoleString(const wchar_t* fmt, ...);

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
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRxThreadPoolService);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db", OdDgnModule)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Dgn7IO", OdDgn7IOModuleImpl)
  ODRX_DEFINE_STATIC_APPLICATION(OdThreadPoolModuleName, OdRxThreadPoolService)
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
  bool  bMTLoading    = false;

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
  STD(cout) << "\nExDgnCopy sample program. Copyright (c) 2019, Open Design Alliance\n";
  //STD(cout) << "\nExDgnCopy developed using ODA Drawings API for DGN";
  //<< (const TCHAR*)svcs.product() << OD_T(" ver ") 
  //<< (const TCHAR*)svcs.versionString();

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
    STD(cout)  << "\n\n\tusage: ExDgnCopy <source file> <destination file> [-DO] [MtLoading]";
    STD(cout)  << "\n\tMtLoading enable multi thread loading.\n";
    STD(cout)  << "\n\t-DO disables progress meter output.\n";
    return nRes;
  }

  /**********************************************************************/
  /* Disable progress meter                                             */
  /**********************************************************************/
  if (argc >= 4)
  {
    for( OdInt32 i = 3; i < argc; i++ )
    {
      OdString argv2(argv[i]);
      if(!odStrICmp(argv2, OD_T("-DO")))
      {
        disableOutput = true;
      }
      else if(!odStrICmp(argv2, OD_T("MtLoading")))
      {
        bMTLoading = true;
      }
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

  if( bMTLoading )
  {
    svcs.setMtMode( OdDb::kMTLoading, true );
    svcs.setNumThreads( OdDb::kMTLoading, 0 );
  }
  else
    svcs.setMtMode( OdDb::kMTLoading, false );

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
    OdDgDatabasePtr pDbIn = svcs.readFile( srcFileName );

    /********************************************************************/
    /* Create an output OdDgDatabase object                             */
    /********************************************************************/
    OdDgDatabasePtr pDbOut = svcs.createDatabase();

    OdDgModelTablePtr pModelTableIn  = pDbIn->getModelTable(OdDg::kForRead);
    OdDgModelTablePtr pModelTableOut = pDbOut->getModelTable(OdDg::kForWrite);

    /********************************************************************/
    /* Clean up out database                                            */
    /********************************************************************/

    if( !pModelTableOut->getAt(0).isNull() )
    {
      OdDgModelPtr pModel = pModelTableOut->getAt(0).openObject(OdDg::kForWrite);
      pModel->erase(true);
    }

    OdDgViewGroupTablePtr pViewGroupTableOut = pDbOut->getViewGroupTable(OdDg::kForWrite);

    if( !pViewGroupTableOut.isNull() )
    {
      OdDgElementIteratorPtr pVGIter = pViewGroupTableOut->createIterator();

      for(; !pVGIter->done(); pVGIter->step() )
      {
        OdDgElementPtr pVGroup = pVGIter->item().openObject(OdDg::kForWrite);

        if( !pVGroup.isNull() )
        {
          pVGroup->erase(true);
        }
      }
    }

    OdDgLevelTablePtr pLevelTableOut = pDbOut->getLevelTable(OdDg::kForWrite);

    if( !pLevelTableOut.isNull() )
    {
      OdDgElementIteratorPtr pLevelIter = pLevelTableOut->createIterator();

      for(; !pLevelIter->done(); pLevelIter->step() )
      {
        OdDgElementPtr pLevel = pLevelIter->item().openObject(OdDg::kForWrite);

        if( !pLevel.isNull() )
        {
          pLevel->erase(true);
        }
      }
    }

    /********************************************************************/
    /* Clone all models                                                 */
    /********************************************************************/

    OdDgElementIdArray arrModelIds;

    OdDgElementIteratorPtr pModelIter = pModelTableIn->createIterator();

    for(; !pModelIter->done(); pModelIter->step() )
    {
      arrModelIds.push_back( pModelIter->item() );
    }

    bool bInitActiveModelId = false;

    if( arrModelIds.size() )
    {
      OdDgIdMappingPtr pMap = OdDgIdMapping::createObject();
      pMap->setDestDb(pDbOut);

      pDbIn->wblockCloneObjects( arrModelIds, pModelTableOut, *pMap, OdDg::kDrcIgnore, false );

/********************************************************************/
/* Set active model and active viewgroup                            */
/********************************************************************/
      OdDgElementId idViewGroup = pDbIn->getActiveViewGroupId();

      if (!idViewGroup.isNull())
      {
        OdDgIdPair obj2obj;

        obj2obj.setKey(idViewGroup);

        if (pMap->compute(obj2obj))
        {
          OdDgElementId idNewViewGroup = obj2obj.value();

          if (!idNewViewGroup.isNull())
          {
            pDbOut->setActiveViewGroupId(idNewViewGroup);
          }
        }
      }

      OdDgElementId idOldModel = pDbIn->getActiveModelId();

      if( !idOldModel.isNull() )
      {
        OdDgIdPair obj2obj;

        obj2obj.setKey(idOldModel);

        if( pMap->compute(obj2obj) )
        {
          OdDgElementId idNewActiveModel = obj2obj.value();

          if( !idNewActiveModel.isNull() )
          {
            pDbOut->setActiveModelId( idNewActiveModel );
            pDbOut->setDefaultModelId( idNewActiveModel );
            bInitActiveModelId = true;
          }
        }
      }
    }

    if( !bInitActiveModelId )
    {
      pModelIter = pModelTableOut->createIterator();

      if( !pModelIter.isNull() && !pModelIter->done() )
      {
        OdDgModelPtr pModel = pModelIter->item().openObject(OdDg::kForWrite);

        pDbOut->setActiveModelId( pModel->elementId() );
        pDbOut->setDefaultModelId( pModel->elementId() );

        pModel->fitToView();
      }
    }

    /********************************************************************/
    /* Write the database                                               */
    /********************************************************************/
    pDbOut->writeFile( dstFileName );
    pDbOut = 0;
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
  ::odrxUninitialize();

  if (nRes)
  {
    STD(cout) << "\n\nPress ENTER to continue...";
    STD(cin).get();
  }
  STD(cout) << "\n\n";
  return nRes;
}
