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
/* This console application creates DGN file with internal meterials.   */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/* ExDgnMaterials <filename> [-DO]                                      */
/*                                                                      */
/*    -DO disables progress meter output                                */
/*                                                                      */
/************************************************************************/

#include "OdaCommon.h"
#include "StaticRxObject.h"
#include "DynamicLinker.h"
#include "RxInit.h"
#include "DgDatabase.h"
#include "DgModel.h"

#include "RxDynamicModule.h"

#include "ExDgnServices.h"
#include "ExDgnHostAppServices.h"
#include "ExDgnMatFiller.h"
#include "diagnostics.h"

#include <stdio.h>
#include <locale.h>

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

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
/* Define a Custom Services class.                                      */
/*                                                                      */
/* Combines the platform dependent functionality of                     */
/* OdExDgnSystemServices and OdExDgnHostAppServices                     */ 
/************************************************************************/
class MyServices : public OdExDgnSystemServices, public OdExDgnHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdExDgnSystemServices);
};

/************************************************************************/
/* User defined function to write a message                             */
/* to the console for each ODA_ASSERT                                   */
/************************************************************************/
static void customAssertFunc(const char* expr, const char* fileName, int nLine)
{
  printf( 
    "Assertion has occurs:\n"
    " Expression: %s\n"
    " Filename: %s\n",
    expr, fileName );
}

#ifndef _TOOLKIT_IN_DLL_
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
  ::odSetAssertFunc( customAssertFunc );

  int nRes = 0; // Return value for main

  /**********************************************************************/
  /* Set command line defaults                                          */
  /**********************************************************************/
  bool disableOutput = false;
 
#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif

  setlocale(LC_TIME, ""); // set current user locale (not OD_T("C")), for strftime

  /********************************************************************/
  /* Create a custom Services instance.                               */
  /********************************************************************/
  OdStaticRxObject<MyServices> svcs;

  /**********************************************************************/
  /* Display the Product and Version that created the executable        */
  /**********************************************************************/
  STD(cout) << "\nExDgnMaterials sample program. Copyright (c) 2019, Open Design Alliance\n";
  //<< (const TCHAR*)svcs.product() << OD_T(" ver ") 
  //<< (const TCHAR*)svcs.versionString();

  /**********************************************************************/
  /* Parse Command Line inputs                                          */
  /**********************************************************************/
  bool bInvalidArgs = (argc < 2);
  if (bInvalidArgs)
  {
    bInvalidArgs = true;
    nRes  = 1;
  }

  /**********************************************************************/
  /* Disable progress meter                                             */
  /**********************************************************************/
  if (argc >= 3)
  {
    OdString argv2(argv[2]);
    if (!odStrICmp(argv2, OD_T("-DO")))
    {
      disableOutput = true;
    }
  }

  if (bInvalidArgs)    
  {
    STD(cout)  << "\n\n\tusage: ExDgnMaterials <filename> [-DO]";
    STD(cout)  << "\n\t-DO disables progress meter output.\n";
    return nRes;
  }

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
    // NOTE: ODA_DG is an environment variable. It could be set as user variable among system properties
    //        to specify a path or list of paths that could contain symbology resource files.
    /********************************************************************/
    // NOTE: The following Registry Variables could be used to specify RSC fonts : 
    //          FAST_FONT specifies the name of the default RSC font;
    //          MS_SYMBRSRC specifies a list of symbology resource files. The last file in the list has the highest priority;
    //          MS_RSRC specifies the main default resource file.
    //
    // For instance:
    //
    //  OdDgHostAppServices* pHostApp;
    //  OdStringArray arrayRscList;
    //  OdString sRscPath("C:\\Documents and Settings\\All Users\\Application Data\\Bentley\\WorkSpace\\System\\Symb\\font.rsc");
    //  arrayRscList.append( sRscPath );
    //  pHostApp->setMS_SYMBRSRC( arrayRscList );
    //  pHostApp->setMS_RSRC( "C:\\Program Files\\Bentley\\MicroStation\\ustation.rsc" );
    //  pHostApp->setFAST_FONT( "CHAR_FAST_FONT" );
    //
    //  Values for MS_SYMBRSRC and MS_RSRC could be both absolute and relative file names.
    //    In the last case it would be recommented to set ODA_DG environment variable.
    //
    //  Attention: Setting FAST_FONT different from default value ("CHAR_FAST_FONT") be sure that you supplied 
    //              and specified the proper symbology resource.
    //

    // In the case below the setting of MS_SYMBRSRC brings an expected result if 
    //  - ODA_DG points to rsc file location;
    //  - rsc file put in proper place.
    OdStringArray arrayRscList;
    OdString sRscPath( L"font.rsc" );
    //OdString sRscPath("C:\\Documents and Settings\\All Users\\Application Data\\Bentley\\WorkSpace\\System\\Symb\\badrsc.rsc");
    arrayRscList.append( sRscPath );
    svcs.setMS_SYMBRSRC( arrayRscList );

    /********************************************************************/
    /* Create a default OdDgDatabase object                             */
    /********************************************************************/
    OdDgDatabasePtr pDb = svcs.createDatabase();

    /********************************************************************/
    /* Fill the database                                                */
    /********************************************************************/
    OdExDgnMatFiller filler;
    filler.fillDatabase(pDb);

    /********************************************************************/
    /* Write the database                                               */
    /********************************************************************/
    //Oda::FileAccessMode nDesiredAccess = Oda::kFileRead;
    //Oda::FileShareMode shareMode = Oda::kShareDenyNo;
    //OdStreamBufPtr pFileStream = svcs.createFile(filename, nDesiredAccess, shareMode);
    //pDb->writeFile( pFileStream );
    pDb->writeFile(fileName);
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
