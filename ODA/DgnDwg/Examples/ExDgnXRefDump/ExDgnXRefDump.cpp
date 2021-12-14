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
/* This console application make dump of a DGN file.                    */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/* ExDgnDump <source filename> <destination dump filename>              */
/*                                                                      */
/************************************************************************/

#include "OdaCommon.h"
#include "StaticRxObject.h"
#include "RxInit.h"
#include "RxObjectImpl.h"
#include "DynamicLinker.h"
#include "ExDgnXRefDumpElements.h"

/************************************************************************/
/* Required ODA Platform .dgn files header files                        */
/************************************************************************/
#include "DgDatabase.h"
#include "ExDgnServices.h"
#include "ExDgnHostAppServices.h"

/************************************************************************/
/* Required ODA Drawings SDK header files                               */
/************************************************************************/
#include "DbDatabase.h"
//#include "DbFiler.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

// Auxiliary function to display different messages
//void oddgPrintConsoleString(const wchar_t* fmt, ...);

/************************************************************************/
/* Define a Custom SystemServices class                                 */
/************************************************************************/
class MySystemServices : public ExSystemServices
{
};

/************************************************************************/
/* Define a Custom Db HostAppServices class                            */
/************************************************************************/
class MyDbHostAppServices : public ExHostAppServices
{
#ifdef _MSC_VER
public:
  /**********************************************************************/
  /* Define a console interface for password support.                   */
  /**********************************************************************/
  bool getPassword(const OdString& sName, bool /*isXref*/, OdPassword& password)
  {
    odPrintConsoleString(L"Enter password to open drawing: %ls\n", sName.c_str());
    wchar_t pwdBuff[_MAX_PATH] = {0};
    STD(wcin).get(pwdBuff, L'\n').get();
    ::CharUpperW(pwdBuff);
    password = pwdBuff;
    return !password.isEmpty();
  }
#endif

public:
  virtual OdDbBaseHostAppServices* getDgnHostAppServices() const
  {
    return m_pDgnHostApp;
  }

public:
  MyDbHostAppServices()
    : m_pDgnHostApp(0)
  {}

  void setDgnHostAppServices(OdDbBaseHostAppServices* pHostApp)
  {
    m_pDgnHostApp = pHostApp;
  }

private:
  OdDbBaseHostAppServices* m_pDgnHostApp;
};

/************************************************************************/
/* Define a Custom Dgn HostAppServices class                            */
/************************************************************************/
class MyDgHostAppServices : public OdExDgnHostAppServices
{
public:
  virtual OdDbBaseHostAppServices* getDbHostAppServices() const
  {
    return m_pTdHostApp;
  }

public:
  MyDgHostAppServices()
    : m_pTdHostApp(0)
  {}

  void setDbHostAppServices(OdDbBaseHostAppServices* pHostApp)
  {
    m_pTdHostApp = pHostApp;
  }

private:
  OdDbBaseHostAppServices* m_pTdHostApp;
};


#ifndef _TOOLKIT_IN_DLL_

#include "RxDynamicModule.h"

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwgModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwgDbModuleImpl);
#if defined DGNIMPORT_ENABLE
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgnImportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgImportLineStyleModule);
#endif
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnLSModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db", OdDgnModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdDbModuleName, OdDwgModule)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_DwgDb", OdDwgDbModuleImpl)
#if defined DGNIMPORT_ENABLE
  ODRX_DEFINE_STATIC_APPMODULE(L"TD_DgnImport", DgnImportModule)
  ODRX_DEFINE_STATIC_APPMODULE(L"ExDgnImportLineStyle", OdDgImportLineStyleModule)
#endif
  ODRX_DEFINE_STATIC_APPMODULE(OdDgnLSModuleName, OdDgnLSModule)
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
  //parse commands
  if( argc != 3 )
  {
    oddgPrintConsoleString(L"\nExDgnXRefDump sample program. Copyright (c) 2019, Open Design Alliance\n");
    oddgPrintConsoleString(L"\tusage: ExDgnXRefDump <source file> <destination file>\n");
    oddgPrintConsoleString(L"\nPress ENTER to continue...\n");
    STD(cin).get();
    return 1;
  }

  OdString szSource = argv[1], szDump = argv[2]; 

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

  DumpStream = fopen( szDump, "w" );
  if( !DumpStream )
  {
    oddgPrintConsoleString( L"\nThe file %ls has not been opened\n", szDump.c_str() );
    return 1;
  }

  //write some title
  fwprintf( DumpStream, L"Dump of the file %ls\n", szSource.c_str() );
 
  /************************************************************************/
  /* Create a Custom SystemServices class                                 */
  /************************************************************************/
  OdStaticRxObject<MySystemServices> svcs;

  /************************************************************************/
  /* Create a Custom Db HostAppServices class                            */
  /************************************************************************/
  OdStaticRxObject<MyDbHostAppServices> dbSvcs;

  /************************************************************************/
  /* Create a Custom Dgn HostAppServices class                            */
  /************************************************************************/
  OdStaticRxObject<MyDgHostAppServices> dgSvcs;

  /************************************************************************/
  /* Set references to supported HostAppServices class                    */
  /************************************************************************/
  dgSvcs.setDbHostAppServices(&dbSvcs);
  dbSvcs.setDgnHostAppServices(&dgSvcs);

  /************************************************************************/
  /* Initialize Runtime Extension environment                             */
  /************************************************************************/
  odrxInitialize(&svcs);

  try
  {
    /**********************************************************************/
    /* Initialize ODA Drawings API for DGN                                */
    /**********************************************************************/
    ::odrxDynamicLinker()->loadModule(L"TG_Db", false);

    /**********************************************************************/
    /* Initialize ODA Drawings SDK                                        */
    /**********************************************************************/
    ::odrxDynamicLinker()->loadModule(OdDbModuleName, false);
    ::odrxDynamicLinker()->loadModule(L"TG_DwgDb", false);

    /********************************************************************/
    /* Load input database from file                                    */
    /********************************************************************/
    OdDgDatabasePtr pDb = dgSvcs.readFile( szSource );


    OdDgDumper dumper;
    dumper.rootDump( pDb.get() );


    //finalize the process
    fclose( DumpStream );
  }
  catch (OdError& e)
  {
    oddgPrintConsoleString(L"\nODA Drawings API for DGN Error: %ls\n", e.description().c_str());
  }
  catch (...)
  {
    oddgPrintConsoleString(L"\nUnknown Error.\n");
  }

  /**********************************************************************/
  /* Uninitialize Runtime Extension environment                         */
  /**********************************************************************/
  ::odrxUninitialize();

  return 0;
}


//
// OdDgDumper
//


OdDgDumper::OdDgDumper()
{
  OdDgObject_Dumper::rxInit();

  OdDgDatabase::desc()->addX( OdDgObject_Dumper::desc(), &m_databaseDumper );
  OdDgModel::desc()->addX( OdDgObject_Dumper::desc(), &m_modelDumper );

  OdDgReferenceAttachmentHeader::desc()->addX( OdDgObject_Dumper::desc(), &m_referenceAttachmentDumper );
}


OdDgDumper::~OdDgDumper()
{
  OdDgReferenceAttachmentHeader::desc()->delX( OdDgObject_Dumper::desc() );

  OdDgDatabase::desc()->delX( OdDgObject_Dumper::desc() );
  OdDgModel::desc()->delX( OdDgObject_Dumper::desc() );

  OdDgObject_Dumper::rxUninit();
}


void OdDgDumper::rootDump( OdRxObjectPtr database ) const
{
  OdSmartPtr< OdDgObject_Dumper > database_dumper = database;

  //run the ordinal method
  database_dumper->dump( database );
}
