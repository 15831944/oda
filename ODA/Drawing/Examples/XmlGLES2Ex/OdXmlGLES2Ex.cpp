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
/* This console application reads a DWG file and writes output TXT file */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/*   OdXmlExportEx <input file> <output file> <xSize> <ySize>           */
/************************************************************************/

#include "OdaCommon.h"
#include "DbDatabase.h"
#include "DbGsManager.h"
#include "RxVariantValue.h"
#include "AbstractViewPE.h"
#include "ColorMapping.h"

#include "RxObjectImpl.h"
#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#include <fstream>
#include <locale.h>

#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "GiContextForDbDatabase.h"

#include "RxDynamicModule.h"

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

/************************************************************************/
/* Define a our own Assert function.                                    */
/************************************************************************/
#include "diagnostics.h"

static void MyAssert(const char* expresssion, const char* fileName, int nLineNo)
{
  std::cout << "\nAssertion Failed: " << expresssion
    << "\nfile: " << fileName << "\nline: " << nLineNo << "\n";
}

/************************************************************************/
/* Define a module map for statically linked modules                    */
/************************************************************************/
#if !defined(_TOOLKIT_IN_DLL_) 

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
#if defined(OLEITEM_HANDLER_MODULE_ENABLED)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdOleItemHandlerModuleImpl);
#endif //OLEITEM_HANDLER_MODULE_ENABLED
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdTrXmlRenderModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(TrXmlModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(TD_IdViewObjModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,  ModelerModule)
  ODRX_DEFINE_STATIC_APPLICATION(RX_RASTER_SERVICES_APPNAME, ExRasterModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRasterProcessorModuleName,  OdRasterProcessingServicesImpl)
#if defined(OLEITEM_HANDLER_MODULE_ENABLED)
  ODRX_DEFINE_STATIC_APPLICATION(OdOleItemHandlerModuleName, OdOleItemHandlerModuleImpl)
#endif //OLEITEM_HANDLER_MODULE_ENABLED
  ODRX_DEFINE_STATIC_APPLICATION(OdTrXmlModuleName, OdTrXmlRenderModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdTrXmlVecModuleName, TrXmlModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdAcIdViewObjModuleName,       TD_IdViewObjModule)
ODRX_END_STATIC_MODULE_MAP()

#endif

#include "XmlServerBaseImpl.h"
#include "TrVisRendition.h"


#if defined(_UNICODE) && defined(OD_USE_WMAIN)
#define OdStrToL wcstol
#else
#define OdStrToL strtol
#endif

// XmlServer implementation
class OdTrXmlServerImpl : public OdTrXmlServerBaseImpl
{
  OdStreamBufPtr m_pOutStream;

public:
  OdTrXmlServerImpl(const OdDbBaseDatabase *pDb = NULL)
    : OdTrXmlServerBaseImpl(OdDbBaseDatabasePE::cast(pDb))
  {
  }

  void setOutputStream(OdStreamBuf *pBuf) { m_pOutStream = pBuf; }

  virtual void ident(const char* pTag, const char* pText)
  {
    for (int n = 0; n < m_nIdent; n++)
      m_pOutStream->putByte((OdUInt8(' ')));
    if (pTag)
    {
      m_pOutStream->putByte((OdUInt8('<')));
      m_pOutStream->putBytes(pTag, (OdUInt32)odStrLenA(pTag));
      m_pOutStream->putByte((OdUInt8('>')));
    }
    if (pText)
      m_pOutStream->putBytes(pText, (OdUInt32)odStrLenA(pText));
    if (pTag)
    {
      m_pOutStream->putBytes("</", 2);
      m_pOutStream->putBytes(pTag, (OdUInt32)odStrLenA(pTag));
      m_pOutStream->putByte((OdUInt8('>')));
    }
    m_pOutStream->putBytes("\r\n", 2);
  }
};

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
#if defined(_UNICODE) && defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
  int nRes = 1; // Return value for main

#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif

  /**********************************************************************/
  /* Set specific application locale settings.                          */
  /**********************************************************************/
  setlocale(LC_ALL, "");      // reset all to default
  setlocale(LC_NUMERIC, "C"); // saving doubles point format

  /**********************************************************************/
  /* Verify the argument count and display an error message as required */
  /**********************************************************************/
  if (argc < 5)
  {
    odPrintConsoleString(L"usage: XmlGLES2Ex <input file> <output file> <xSize> <ySize> [-x]\n");
    return 1;
  }

#if !defined(_TOOLKIT_IN_DLL_) 
  ODRX_INIT_STATIC_MODULE_MAP();
#endif
  odSetAssertFunc(MyAssert);

  // Create a custom Services object.
  OdStaticRxObject<MyServices> svcs;
  // initialize the Drawings SDK
  odInitialize(&svcs);
  // initialize GS subsystem
  //odgsInitialize(); // Not necessary for XmlGLES2 module

  odPrintConsoleString(L"Developed using %ls ver %ls\n", svcs.product().c_str(), svcs.versionString().c_str());

  try 
  {
    /******************************************************************/
    /* Create a database and load the drawing into it.                */
    /*                                                                */
    /* Specified arguments are as followed:                           */
    /*     filename, allowCPConversion, partialLoad, openMode         */
    /******************************************************************/
    OdDbDatabasePtr pDb;
    OdString f(argv[1]);
    pDb = svcs.readFile( f.c_str(), false, false, Oda::kShareDenyNo ); 

    if (!pDb.isNull())
    {
      /****************************************************************/
      /* Create the vectorization context.                            */
      /* This class defines the operations and properties that are    */
      /* used in the vectorization of an OdDbDatabase.                */
      /****************************************************************/
      OdGiContextForDbDatabasePtr pDwgContext = OdGiContextForDbDatabase::createObject();

      /****************************************************************/
      /* Create the XmlGLES2 rendering device, and set the output     */
      /* stream for the device.                                       */
      /****************************************************************/
      OdGsModulePtr pGs = ::odrxDynamicLinker()->loadModule(OdTrXmlVecModuleName);
      OdGsDevicePtr pDevice = pGs->createDevice();

      /****************************************************************/
      /* Set the database to be vectorized.                           */
      /****************************************************************/
      pDwgContext->setDatabase(pDb);

      /****************************************************************/
      /* Enable GsModel to have cache.                                */
      /****************************************************************/
      pDwgContext->enableGsModel(true);

      /****************************************************************/
      /* Prepare the device to render the active layout in            */
      /* this database.                                               */
      /****************************************************************/
      pDevice = OdDbGsManager::setupActiveLayoutViews(pDevice, pDwgContext);

      /****************************************************************/
      /* Setup background.                                            */
      /****************************************************************/
      if (pDb->getTILEMODE())
      {
        // Use black background for ModelSpace
        pDevice->setLogicalPalette(::odcmAcadDarkPalette(), 256);
        pDevice->setBackgroundColor(ODRGB(0, 0, 0));
        pDwgContext->setPaletteBackground(ODRGB(0, 0, 0));
      }
      else
      {
        // Use white paper and grey background for ModelSpace
        pDevice->setLogicalPalette(::odcmAcadLightPalette(), 256);
        pDevice->setBackgroundColor(ODRGB(168, 168, 168));
        pDwgContext->setPaletteBackground(ODRGB(255, 255, 255));
      }

      /****************************************************************/
      /* This data must be obtained from client:                      */
      /* 1) setup for output screen rectangle.                        */
      /* 2) setup client settings.                                    */
      /****************************************************************/

      /****************************************************************/
      /* Set output screen rectangle.                                 */
      /****************************************************************/
      OdGsDCRect screenRect(OdGsDCPoint(0, OdStrToL(argv[4], NULL, 10)), OdGsDCPoint(OdStrToL(argv[3], NULL, 10), 0));
      pDevice->onSize(screenRect);

      /****************************************************************/
      /* Set client settings.                                         */
      /****************************************************************/
      OdTrVisRendition::ClientSettings cliSets; // <<< obtained from client
      OdRxDictionaryPtr pProperties = pDevice->properties();
      pProperties->putAt(OD_T("ClientSettings"), OdRxVariantValue((OdIntPtr)&cliSets));

      /****************************************************************/
      /* Create Xml stream.                                           */
      /****************************************************************/
      OdTrXmlServerImpl xmlServer(pDb);
      pProperties->putAt(OD_T("XmlServer"), OdRxVariantValue((OdIntPtr)&xmlServer));

      /****************************************************************/
      /* Extended test (optional) generates 3 separate files instead  */
      /* of single. 1st file represents client-dependent data section,*/
      /* 2nd file represents client-independent data section and 3rd  */
      /* file contains changes after GsView modification.             */
      /****************************************************************/
#if defined(_UNICODE) && defined(OD_USE_WMAIN)
      const bool bExtendedTest = (argc > 5) && (odStrCmp(argv[5], OD_T("-x")) == 0);
#else
      const bool bExtendedTest = (argc > 5) && (odStrCmpA(argv[5], "-x") == 0);
#endif
      /****************************************************************/
      /* Create output file.                                          */
      /****************************************************************/
      OdString fName(argv[2]);
      OdString fExt;
      {
        int n = fName.reverseFind('.');
        if (n < 0)
          fExt = L".txt";
        else if (n == 0)
          fName = L"output";
        else
        {
          fExt = fName.right(fName.getLength() - n);
          fName = fName.left(n);
        }
      }
      if (!bExtendedTest)
      {
        /****************************************************************/
        /* Simple test only generate single file for opened drawing.    */
        /****************************************************************/
        OdStreamBufPtr pOutFile = svcs.createFile(OdString().format(L"%s%s", fName.c_str(), fExt.c_str()), Oda::kFileWrite, Oda::kShareDenyWrite, Oda::kCreateAlways);
        xmlServer.setOutputStream(pOutFile);
        pDevice->update();
      }
      else
      {
        OdStreamBufPtr pOutFile = svcs.createFile(OdString().format(L"%s1%s", fName.c_str(), fExt.c_str()), Oda::kFileWrite, Oda::kShareDenyWrite, Oda::kCreateAlways);
        xmlServer.setOutputStream(pOutFile);

        /****************************************************************/
        /* Firstly update to get client dependent section only.         */
        /* This is not required, but could be useful for caching servers*/
        /****************************************************************/
        xmlServer.setProcessingMode(OdTrXmlServer::kClientDependentSectionOnly);
        pDevice->update();
        xmlServer.setProcessingMode(OdTrXmlServer::kNormalMode);

        /****************************************************************/
        /* Second output stream.                                        */
        /****************************************************************/
        pOutFile = svcs.createFile(OdString().format(L"%s2%s", fName.c_str(), fExt.c_str()), Oda::kFileWrite, Oda::kShareDenyWrite, Oda::kCreateAlways);
        xmlServer.setOutputStream(pOutFile);

        /****************************************************************/
        /* First full update will drop all initial data to file.        */
        /****************************************************************/
        pDevice->update();

        /****************************************************************/
        /* Third output stream.                                         */
        /****************************************************************/
        pOutFile = svcs.createFile(OdString().format(L"%s3%s", fName.c_str(), fExt.c_str()), Oda::kFileWrite, Oda::kShareDenyWrite, Oda::kCreateAlways);
        xmlServer.setOutputStream(pOutFile);

        /****************************************************************/
        /* Now try to modify something to have diff.                    */
        /****************************************************************/
        pDevice->viewAt(0)->zoom(0.5);

        /****************************************************************/
        /* Only changed data will be droppend into third stream.        */
        /****************************************************************/
        pDevice->update();
      }
      // detach Rendition before it will be destroyed while exitin this scope
      pProperties->putAt(OD_T("Rendition"), OdRxVariantValue((OdUInt32)0));
    }

    odPrintConsoleString(L"\nDone.\n");
    nRes = 0;
  }

  /********************************************************************/
  /* Display the error                                                */
  /********************************************************************/
  catch (const OdError& e)
  {
    odPrintConsoleString(L"\nException Caught: %ls\n", e.description().c_str());
  }
  catch (...)
  {
    odPrintConsoleString(L"\nUnknown Exception Caught\n");
  }
  //odgsUninitialize(); // Not necessary for XmlGLES2 module
  odUninitialize();

  if (!nRes)
  {
    odPrintConsoleString(L"\nXmlGLES2Ex Finished Successfully");
  }
  return nRes;
}

//
