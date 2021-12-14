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
/* Simple application displays a dump of XData linkages                 */
/* of model elements from a DGN file.                                   */
/************************************************************************/

#include "OdaCommon.h"
#include "StaticRxObject.h"
#include "RxInit.h"
#include "RxDynamicModule.h"
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
#include "DbFiler.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"

/*************************************************************************************/
/* Here to include ODA Drawings API for DGN header files required for your sample    */
/*************************************************************************************/
#include "DgModel.h"
#include "DgLine.h"

/************************************************************************************/
/* Here to include ODA Drawings SDK header files required for your sample           */
/************************************************************************************/
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
  bool getPassword(const OdString& dwgName, bool /*isXref*/, OdPassword& password)
  {
    odPrintConsoleString(L"Enter password to open drawing: %ls\n", dwgName.c_str());
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
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwgDbModuleImpl);
#if defined DGNIMPORT_ENABLE
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgnImportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgImportLineStyleModule);
#endif

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db",        OdDgnModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdDbModuleName,    OdDwgModule)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_DwgDb",  OdDwgDbModuleImpl)
#if defined DGNIMPORT_ENABLE
  ODRX_DEFINE_STATIC_APPMODULE(L"TD_DgnImport", DgnImportModule)
  ODRX_DEFINE_STATIC_APPMODULE(L"ExDgnImportLineStyle", OdDgImportLineStyleModule)
#endif
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
  if ( argc < 3 )
  {
    odPrintConsoleString(L"\nExDwgXData sample program. Copyright (c) 2019, Open Design Alliance\n");
    odPrintConsoleString(L"\tusage:ExDwgXData <option> <file>\n");
    odPrintConsoleString(L"\toption can be:\n");
    odPrintConsoleString(L"\t-r - dump XData from the file\n");
    odPrintConsoleString(L"\t-w - create a file with XData\n");
    odPrintConsoleString(L"\nPress ENTER to continue...\n");
    STD(cin).get();
    return 1;
  }
  OdString fileName = argv[2];
  OdString optionStr(argv[1]);
  bool bDumpXData = true;
  if (!odStrICmp(optionStr,L"-w"))
    bDumpXData = false;

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

  /************************************************************************/
  /* Create a Custom SystemServices class                                 */
  /************************************************************************/
  OdStaticRxObject<MySystemServices> svcs;

  /************************************************************************/
  /* Create a Custom Db HostAppServices class                             */
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
    /* Initialize ODA Drawings SDK                                        */
    /**********************************************************************/
    ::odrxDynamicLinker()->loadModule(OdDbModuleName, false); 
    ::odrxDynamicLinker()->loadModule(L"TG_DwgDb", false);

    /**********************************************************************/
    /* Initialize ODA Drawings API for DGN                                */
    /**********************************************************************/
    ::odrxDynamicLinker()->loadModule(L"TG_Db", false);

    /**********************************************************************/
    /* Read DGN file                                                      */
    /**********************************************************************/
    try
    {
      if ( bDumpXData )
      {
        /********************************************************************/
        /* Load input database from file                                    */
        /********************************************************************/
        OdDgDatabasePtr pDb = dgSvcs.readFile( fileName );
        OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject(OdDg::kForRead);

        /********************************************************************/
        /* Iterate through graphics elements.                               */
        /********************************************************************/
        OdDgElementIteratorPtr pIt = pModel->createGraphicsElementsIterator();
        for ( ; !pIt->done(); pIt->step() )
        {
          OdDgElementPtr pElm = pIt->item().openObject();
          if ( pElm.get() )
          {
            OdResBufPtr pRb = pElm->getXDataLinkage();
            if ( pRb.get() )
            {
              odPrintConsoleString(L"\nElement <%ls> : %ls", pElm->isA()->name().c_str(), pElm->elementId().getHandle().ascii().c_str());
              for ( ; !pRb.isNull(); pRb = pRb->next() ) 
              {
                int resType = pRb->restype();
                odPrintConsoleString(L"\n  ");
                odPrintConsoleString( L"%d", resType );

                OdString rbStr;
                switch ( OdDxfCode::_getType(resType) )
                {
                case OdDxfCode::Name:
                case OdDxfCode::String:
                  rbStr.format(L"%ls", pRb->getString().c_str());
                  break;
                case OdDxfCode::Bool:
                  rbStr.format(L"%ls", (pRb->getBool() ? L"<true>" : L"<false>"));
                  break;
                case OdDxfCode::Integer8:
                  rbStr.format(L"%d", pRb->getInt8());
                  break;
                case OdDxfCode::Integer16:
                  rbStr.format(L"%d", pRb->getInt16());
                  break;
                case OdDxfCode::Integer32:
                  rbStr.format(L"%d", pRb->getInt32());
                  break;
                case OdDxfCode::Double:
                case OdDxfCode::Angle:
                  rbStr.format(L"%f", pRb->getDouble());
                  break;
                case OdDxfCode::Point:
                  {
                    OdGePoint3d pt = pRb->getPoint3d();
                    rbStr.format(L"Point(%f, %f, %f)", pt.x, pt.y, pt.z );
                  }
                  break;
                case OdDxfCode::BinaryChunk:
                  {
                    OdBinaryData data = pRb->getBinaryChunk();
                    rbStr.format(L"BinaryChunk (Size = %d)", data.size());
                  }
                  break;
                case OdDxfCode::LayerName:
                  rbStr.format(L"%ls", pRb->getString().c_str());
                  break;
                case OdDxfCode::Handle:
                  rbStr.format(L"%ls", pRb->getHandle().ascii().c_str());
                  break;
                default:
                  ODA_FAIL_ONCE();
                  rbStr = L"<Unknown type>";
                }
                odPrintConsoleString( L"\n  %ls", rbStr.c_str() );
              }
              odPrintConsoleString(L"\n");
            }
          }
        }

      }
      else
      {
        /********************************************************************/
        /* Create a default OdDgDatabase object                             */
        /********************************************************************/
        OdDgDatabasePtr pDb = dgSvcs.createDatabase();
        OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject(OdDg::kForWrite);
        pModel->setWorkingUnit( OdDgModel::kWuMasterUnit );
        pModel->setModelIs3dFlag( true );

        /********************************************************************/
        /* Create a new element                                             */
        /********************************************************************/
        OdDgLine3dPtr pLine3d = OdDgLine3d::createObject();
        pLine3d->setStartPoint( OdGePoint3d( 0., 0., 0. ) );
        pLine3d->setEndPoint( OdGePoint3d( 10., 10., 10. ) );
        pModel->addElement(pLine3d);
        pModel->fitToView();
        OdDgElementPtr pElement = pLine3d;

        /********************************************************************/
        /* Add XData to the element                                         */
        /********************************************************************/
        OdString appName = L"ExDgnCreate";
        pDb->newRegApp(appName);
        OdResBufPtr pRb = OdResBuf::newRb(1001, appName);
        OdResBufPtr pXDataRb = pRb;

        pRb = pRb->setNext(OdResBuf::newRb(1070));
        pRb->setInt16(255);

        OdBinaryData binData;
        binData.resize(16, 0);
        pRb = pRb->setNext(OdResBuf::newRb(1004));
        pRb->setBinaryChunk(binData);

        pElement->setXDataLinkage(pXDataRb.get());

        pLine3d  = 0;
        pElement = 0;

        /********************************************************************/
        /* Write the database                                               */
        /********************************************************************/
        pDb->writeFile(fileName);
      }
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
