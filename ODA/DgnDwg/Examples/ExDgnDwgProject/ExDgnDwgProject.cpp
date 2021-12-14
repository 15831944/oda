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
/* Simple application creating new .dgn file and new .dwg file          */
/* and filling them with some objects                                   */
/************************************************************************/

#include "OdaCommon.h"
#include "StaticRxObject.h"
#include "RxInit.h"
#include "DynamicLinker.h"

/************************************************************************/
/* Required ODA Drawings API for DGN header files                       */
/************************************************************************/
#include "DgDatabase.h"
#include "ExDgnHostAppServices.h"

/************************************************************************/
/* Required ODA Drawings SDK header files                               */
/************************************************************************/
#include "DbDatabase.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"

/**************************************************************************************/
/* Here to include ODA Drawings API for DGN header files required for your sample     */
/*************************************************************************************/
#include "DgModel.h"
#include "DgLine.h"

/**************************************************************************/
/* Here to include ODA Drawings SDK header files required for your sample */
/**************************************************************************/
#include "DbBlockTableRecord.h"
#include "DbLine.h"


#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std::a


#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif


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
  bool getPassword(const OdString& sDwgName, bool /*isXref*/, OdPassword& password)
  {
    odPrintConsoleString(L"Enter password to open drawing: %ls\n", sDwgName.c_str());
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
    return m_pDbHostApp;
  }

public:
  MyDgHostAppServices()
    : m_pDbHostApp(0)
  {}

  void setDbHostAppServices(OdDbBaseHostAppServices* pHostApp)
  {
    m_pDbHostApp = pHostApp;
  }

private:
  OdDbBaseHostAppServices* m_pDbHostApp;
};


#ifndef _TOOLKIT_IN_DLL_

#include "RxDynamicModule.h"

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwgModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule); 
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwgDbModuleImpl);
#if defined DGNIMPORT_ENABLE
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgnImportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgImportLineStyleModule);
#endif
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnLSModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db",        OdDgnModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdDbModuleName,  OdDwgModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_DwgDb",     OdDwgDbModuleImpl)
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
#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif

  /**********************************************************************/
  /* Verify the argument count and display an error message as required */
  /**********************************************************************/
  if ( 2 != argc )
  {
    odPrintConsoleString(L"\nExDgnDwgProject sample program. Copyright (c) 2019, Open Design Alliance\n");
    odPrintConsoleString(L"\tusage:ExDgnDwgProject <destination file>\n");
    odPrintConsoleString(L"\nPress ENTER to continue...\n");
    STD(cin).get();
    return 1;
  }
  OdString fileName = argv[1];
  OdString dgnFileName = fileName;
  OdString dwgFileName = fileName;

  if( dgnFileName.getLength() > 3 )
  {
    OdString strExtension = fileName.right(4);
    strExtension.makeLower();

    if( strExtension == L".dgn" )
      dwgFileName += L".dwg";
    else if( strExtension == L".dwg")
      dgnFileName += L".dgn";
    else
    {
      dwgFileName += L".dwg";
      dgnFileName += L".dgn";
    }
  }

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

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
    /* Initialize ODA Drawings SDK files                                  */
    /**********************************************************************/
    ::odrxDynamicLinker()->loadModule(OdDbModuleName, false);
    ::odrxDynamicLinker()->loadModule(L"TG_DwgDb", false);

    /**********************************************************************/
    /* Initialize ODA Drawings API for DGN                                */
    /**********************************************************************/
    ::odrxDynamicLinker()->loadModule(L"TG_Db", false);

    /**********************************************************************/
    /* Write DGN file                                                     */
    /**********************************************************************/
    try
    {
      /********************************************************************/
      /* Create a default database object                                 */
      /********************************************************************/
      OdDgDatabasePtr pDb = dgSvcs.createDatabase();

      /********************************************************************/
      /* Fill the database                                                */
      /*                                                                  */
      /* Some custom action on file could be here                         */
      /* instead of the code below                                        */
      /********************************************************************/
      {
        OdDgElementId modelId = pDb->getActiveModelId();
        OdDgModelPtr pModel = modelId.safeOpenObject(OdDg::kForWrite);
        pModel->setWorkingUnit( OdDgModel::kWuMasterUnit );
        pModel->setModelIs3dFlag( true );

        // Create a new line
        OdDgLine3dPtr pLine3d = OdDgLine3d::createObject();
        pLine3d->setStartPoint( OdGePoint3d( 0., 0., 0. ) );
        pLine3d->setEndPoint( OdGePoint3d( 10., 10., 10. ) );

        // Add the line to the model
        pModel->addElement(pLine3d);

        // Set an appropriate set of views
        pModel->fitToView();
      }

      /********************************************************************/
      /* Write the database                                               */
      /********************************************************************/
      pDb->writeFile(dgnFileName);
    }
    catch (OdError& e)
    {
      odPrintConsoleString(L"\nODA Drawings API for DGN Error: %ls\n", e.description().c_str());
    }

    /**********************************************************************/
    /* Write .dwg file                                                    */
    /**********************************************************************/
    try
    {
      /********************************************************************/
      /* Create a default database object                                 */
      /********************************************************************/
      OdDbDatabasePtr pDb = dbSvcs.createDatabase();

      /********************************************************************/
      /* Fill the database                                                */
      /*                                                                  */
      /* Some custom action on file could be here                         */
      /* instead of the code below                                        */
      /********************************************************************/
      {
        OdDbObjectId msId = pDb->getModelSpaceId();
        OdDbBlockTableRecordPtr pMs = msId.safeOpenObject(OdDb::kForWrite);

        // Create a new line
        OdDbLinePtr pLine = OdDbLine::createObject();
        pLine->setStartPoint( OdGePoint3d( 0., 0., 0. ) );
        pLine->setEndPoint( OdGePoint3d( 10., 10., 10. ) );

        // Add the line to the ModelSpace
        pMs->appendOdDbEntity(pLine);
      }

      /********************************************************************/
      /* Write the database                                               */
      /********************************************************************/
      pDb->writeFile(dwgFileName, OdDb::kDwg, OdDb::kDHL_CURRENT, true);
    }
    catch (OdError& e)
    {
      odPrintConsoleString(L"\nODA Drawings API for DGN Error: %ls\n", e.description().c_str());
    }
  }
  catch (...)
  {
    odPrintConsoleString(L"\nUnknown Error.\n");
  }

  /************************************************************************/
  /* Uninitialize Runtime Extension environment                           */
  /************************************************************************/
  ::odrxUninitialize();

  return 0;
}
