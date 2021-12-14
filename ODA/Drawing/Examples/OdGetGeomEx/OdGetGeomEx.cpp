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
/* This console application reads a DWG file and dumps the geometry     */
/* of the specified object.                                             */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/*    OdGetGeomEx <input file> <entity handle> [<output file>]          */
/*    If <output file> is not specified, output is written to stdout.   */
/*                                                                      */
/************************************************************************/

#include "OdaCommon.h"
#include "DbDatabase.h"
#include "DbGsManager.h"

#include "RxObjectImpl.h"

#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "GiContextForDbDatabase.h"
#include "../OdVectorizeEx/GiDumperImpl.h"
#include "GiDrawObjectForGetGeometry.h"

#include "RxDynamicModule.h"

#define STL_USING_IOSTREAM
#define STL_USING_STREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a
#include <fstream>

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
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

#include "diagnostics.h"

/************************************************************************/
/* Assertion handler                                                    */
/************************************************************************/
static void MyAssert(const char* expresssion, const char* fileName, int nLineNo)
{
  printf("\nAssertion Failed: %s\n"
         "file: %s\n"
         "line: %d\n",
         expresssion, fileName, nLineNo);      
}

/************************************************************************/
/* Define a module map for statically linked modules:                   */
/************************************************************************/
#if !defined(_TOOLKIT_IN_DLL_)

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,  ModelerModule)
ODRX_END_STATIC_MODULE_MAP()

#endif

#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t** argv)
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
  if (argc < 3)
  {
    printf("usage: OdGetGeomEx <input file> <entity handle> [> <output file>]\n");
  }
  else
  {
#if !defined(_TOOLKIT_IN_DLL_)
    ODRX_INIT_STATIC_MODULE_MAP();
#endif
    /********************************************************************/
    /* Specify the assertion function                                   */
    /********************************************************************/
    odSetAssertFunc(MyAssert);

    /********************************************************************/
    /* Create a custom Services instance.                               */
    /********************************************************************/
    OdStaticRxObject<MyServices> svcs;

    /********************************************************************/
    /* Initialize the Drawings SDK.                                     */
    /********************************************************************/
    odInitialize(&svcs);

    /********************************************************************/
    /* Display the Product and Version that created the executable      */
    /********************************************************************/
    printf("Developed using %ls, %ls\n", svcs.product().c_str(), svcs.versionString().c_str());

    try 
    {
      /******************************************************************/
      /* Create a database and load the drawing into it.                */
      /*                                                                */
      /* Specified arguments are as followed:                           */
      /*     filename, allowCPConversion, partialLoad, openMode         */      
      /******************************************************************/
      OdDbDatabasePtr pDb;

      pDb = svcs.readFile( argv[1], false, false, Oda::kShareDenyNo ); 

      if(!pDb.isNull())
      {
        /****************************************************************/
        /* Get the input handle                                         */
        /****************************************************************/
        OdDbHandle han = argv[2];

        /****************************************************************/
        /* Get the object Id from the handle                            */
        /****************************************************************/
        OdDbObjectId entityId = pDb->getOdDbObjectId(han);

        /****************************************************************/
        /* Open the Object                                              */
        /****************************************************************/
        OdDbObjectPtr pObj = entityId.openObject();

        /****************************************************************/
        /* Cast the object to an entity                                 */
        /****************************************************************/
        OdDbEntityPtr pEnt = OdDbEntity::cast(pObj);

        /****************************************************************/
        /* pEnt.isNull() == true if there is a problem with the handle  */
        /****************************************************************/
        if(!pEnt.isNull()) // handling the case if something went wrong
        {
          /**************************************************************/
          /* Create the vectorization context                           */
          /**************************************************************/
          OdGiContextForDbDatabasePtr pDwgContext = OdGiContextForDbDatabase::createObject();
          pDwgContext->setDatabase(pDb);

          /**************************************************************/
          /* Create the dumper for non-conveyor objects                 */
          /* This dumper is implemented in the OdVectorizeEx example    */
          /**************************************************************/
          OdGiDumperPtr pDumper = OdGiDumperImpl::createObject();

          /**************************************************************/
          /* Create the conveyor geometry dumper                        */
          /**************************************************************/
          OdGiConveyorGeometryDumperPtr pConvDumper = OdGiConveyorGeometryDumper::createObject(pDumper);
          
          /**************************************************************/
          /* Create the vectorizer object                               */
          /**************************************************************/
          OdGiDrawObjectForGetGeometryPtr drawObject = 
            OdGiDrawObjectForGetGeometry::createObject(pDwgContext, pConvDumper, pDumper);

          /**************************************************************/
          /* Set the draw context for the dumper                        */
          /**************************************************************/
          pConvDumper->setDrawContext(drawObject->drawContext());
          
          /**************************************************************/
          /* Vectorize the draw object                                  */
          /**************************************************************/
          drawObject.get()->draw(pEnt);
        }
        else
        {
          printf("\nError: there is no entity with such handle\n");
        }
      }
      printf("\nFinished OK.\n");
    }

    catch (const OdError& e)
    {
      printf("\nException Caught: %ls\n", e.description().c_str());      
    }
    catch (...)
    {
      printf("\nUnknown Exception Caught\n");
    }

    /********************************************************************/
    /* Uninitialize the Drawings SDK.                                   */
    /********************************************************************/
    odUninitialize();
  }
  
  STD(cout) << "Press ENTER to exit"  << STD(endl);
  STD(cin).get();
  return 0;
}
