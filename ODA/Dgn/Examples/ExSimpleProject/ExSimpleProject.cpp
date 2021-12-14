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

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db", OdDgnModule)
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
  /* Parse Command Line strings                                         */
  /**********************************************************************/
  {
    bool bInvalidArgs = (argc < 2);

    if (bInvalidArgs)
    {
      bInvalidArgs = true;
      nRes  = 1;
    }

    if (argc >= 3)
    {
      OdString argv2(argv[2]);
      if (!odStrICmp(argv2, OD_T("-DO")))
      {
        disableOutput = true;
      }
      else
      {
        bInvalidArgs = true;
      }
    }
	STD(cout) << "\nExSimpleProject sample program. Copyright (c) 2019, Open Design Alliance\n";

    if (bInvalidArgs)    
    {
      STD(cout)  << "\n\n\tusage: ExSimpleProject <filename> [-DO]";
      STD(cout)  << "\n\t-DO disables progress meter output.\n";
      return nRes;
    }
  }

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

  /************************************************************************/
  /* Disable/Enable progress meter                                       */
  /************************************************************************/
  svcs.disableProgressMeterOutput( disableOutput );

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
    /* Create a default OdDgDatabase object                             */
    /********************************************************************/
    OdDgDatabasePtr pDb = svcs.createDatabase();
    OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject(OdDg::kForWrite);
    pModel->setWorkingUnit( OdDgModel::kWuMasterUnit );

    /********************************************************************/
    /* Some custom action on file could be here                         */
    /* instead of the code below                                        */
    /********************************************************************/
  /* Note: doesn't forget to include required header files      */
    /********************************************************************/
    {
      pModel->setModelIs3dFlag( true );

      // Create new line
      OdDgLine3dPtr line3d = OdDgLine3d::createObject();

      // Set endpoints
      line3d->setStartPoint( OdGePoint3d( 0., 0., 0. ) );
      line3d->setEndPoint( OdGePoint3d( 10., 10., 10. ) );

      // Add the line to the model
      pModel->addElement(line3d);
    }

    /********************************************************************/
    /* Set an appropriate set of views                                  */
    /********************************************************************/
    pModel->fitToView();

    /********************************************************************/
    /* Write the database                                               */
    /********************************************************************/
    pDb->writeFile(fileName);

    pModel = 0;
    pDb = 0;
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
