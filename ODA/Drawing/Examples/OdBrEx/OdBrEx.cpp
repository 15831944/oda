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
/* This console application demonstrates the usage of the B-Rep         */
/* traversal API (OdBr*), which can be used to traverse the curve and   */
/* surface information contained in ACIS entities                       */
/* to the console.                                                      */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/*    OdBrEx <filename>                                                 */
/*                                                                      */
/************************************************************************/

#include "OdaCommon.h"

#include "DbDatabase.h"
#include "DbBlockTable.h"
#include "DbBlockTableRecord.h"
#include "Db3dSolid.h"
#include "DbBody.h"
#include "DbRegion.h"

#include "RxObjectImpl.h"

#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "DynamicLinker.h"
#include "ModelerGeometry/ModelerModule.h"
#include "RxDynamicModule.h"

#include "Br/BrBrep.h"

#include "BrSearch.h"
#include "BrTraverse.h"
#include "BrProcess.h"

#include "MemoryStream.h"

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

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,  ModelerModule)
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
};
/************************************************************************/
/* Dump the ACIS data for the specified database                        */
/************************************************************************/
void dumpAcis(OdDbDatabase* pDb, STD(ostream)& os)
{
  /**********************************************************************/
  /* Get a Block Table pointer                                          */
  /**********************************************************************/
  OdDbBlockTablePtr pBlocks = pDb->getBlockTableId().safeOpenObject();

  /**********************************************************************/
  /* Get a Symbol Table Iterator for the Block Table                    */
  /**********************************************************************/
  OdDbSymbolTableIteratorPtr pBlkIter = pBlocks->newIterator();

  /**********************************************************************/
  /* Step through the Block Table                                       */
  /**********************************************************************/
  for (pBlkIter->start(); ! pBlkIter->done(); pBlkIter->step())
  {
    /********************************************************************/
    /* Open the Block Table Record                                      */
    /********************************************************************/
    OdDbBlockTableRecordPtr pBlock = pBlkIter->getRecordId().safeOpenObject();

    /********************************************************************/
    /* Get an Object Iterator for the Block Table Record                */
    /********************************************************************/
    OdDbObjectIteratorPtr pEntIter = pBlock->newIterator();

    /********************************************************************/
    /* Step through the Block Table Record                              */
    /********************************************************************/
    for (; !pEntIter->done(); pEntIter->step())
    {
      /******************************************************************/
      /* Open the Entity                                                */
      /******************************************************************/
      OdDbEntityPtr pEnt = pEntIter->objectId().safeOpenObject();

      /******************************************************************/
      /* Output the entity type and handle                              */
      /******************************************************************/
      wprintf(OD_T("\n\nEntity: %ls <%ls>\n"), pEnt->isA()->name().c_str(), pEnt->getDbHandle().ascii().c_str());

      /******************************************************************/
      /* Process only entities with brep data                           */
      /******************************************************************/
      if ((pEnt->isKindOf(OdDb3dSolid::desc())) || 
        (pEnt->isKindOf(OdDbBody::desc()))    ||  
        (pEnt->isKindOf(OdDbRegion::desc())))
      {
        /****************************************************************/
        /* Do we want to process this entity?                           */
        /****************************************************************/
        STD(cout) << "Process (y/n)? ";
        char choice;
        STD(cin) >> choice;
        if (choice == 'Y' || choice == 'y')
        {
          OdBrBrep br;

          /**************************************************************/
          /* Extract the brep data                                      */
          /**************************************************************/
          if (pEnt->isKindOf(OdDb3dSolid::desc()))
            OdDb3dSolidPtr(pEnt)->brep(br);
          else if (pEnt->isKindOf(OdDbBody::desc()))
            OdDbBodyPtr(pEnt)->brep(br);
          else if (pEnt->isKindOf(OdDbRegion::desc()))
            OdDbRegionPtr(pEnt)->brep(br);

          /**************************************************************/
          /* Loop until done                                            */
          /**************************************************************/
          int testToRun = menu();
          while (testToRun > 0)
          {
            processOption(br, pEnt, testToRun);
            testToRun = menu();
          }
        }
      }
    }
  }
}

/************************************************************************/
/* Create a custom Services instance.                                   */
/************************************************************************/
OdStaticRxObject<MyServices> svcs;

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

  /**********************************************************************/
  /* Verify the argument count and display an error message as required */
  /**********************************************************************/
  if (argc < 2) 
  {
    printf("usage: OdBrEx <filename>\n");
  }
  else
  {
#ifndef _TOOLKIT_IN_DLL_
    ODRX_INIT_STATIC_MODULE_MAP();
#endif

    /********************************************************************/
    /* Initialize Drawings.                                             */
    /********************************************************************/
    svcs.setRecomputeDimBlocksRequired(false);
    odInitialize(&svcs);

    if (argc > 2)
    {
      OdString strParam = argv[2];
      strParam.makeLower();
      if (strParam.compare(OD_T("spamodeler")) == 0)
        ::odrxDynamicLinker()->loadModule(OdSpaModelerModuleName, false);
    }

    try 
    {
      OdDbDatabasePtr pDb;

      /******************************************************************/
      /* Create a database and load the drawing into it.                */
      /******************************************************************/
      OdString f(argv[1]); // for UNIX UNICODE support
      pDb = svcs.readFile(f.c_str()); 

      /******************************************************************/
      /* Dump the ACIS data.                                            */
      /******************************************************************/
      if (!pDb.isNull())
      {
        dumpAcis(pDb, STD(cout));
      }
    }
    catch (OdError& e)
    {
      odPrintConsoleString(L"Teigha Error: %ls\n\n", e.description().c_str());
    }
    catch (...)
    {
      STD(cout) << "\nException thrown during readFile, exiting\n";
      return 0;
    }

    /********************************************************************/
    /* Uninitialize Drawings                                            */
    /********************************************************************/
    odUninitialize();
  }

  return 0;
}
