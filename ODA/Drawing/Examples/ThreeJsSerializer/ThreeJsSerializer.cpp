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
/*   This console example application converts GS GLES2 data to         */
/*   JSON file and back.                                  */
/************************************************************************/

#include "OdaCommon.h"
//#include "DbDatabase.h"
#include "DbBaseDatabase.h"
#include "Gi/GiCommonDraw.h"
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

#include "ExPrintConsole.h"

#include "RxDynamicModule.h"

//#include "CloudTools.h"
#include "ExtDbModule.h"

#include "Tr/TrVisRenderClient.h"

#include "diagnostics.h"

#include "ThreejsJSONModule.h"

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

static OdExtDbModule* loadExtDbModuleByExtension(const OdString& csExt)
{
  OdString sExt = csExt,
           sModuleName;
  sExt.makeLower();
  if (sExt == L"dgn")
    sModuleName = L"TD_DgnDb.tx";
  if (sExt == L"rvt" || sExt == L"rfa")
    sModuleName = L"TD_BimDb.tx";
  if (sExt == L"dwf")
    sModuleName = L"TD_DwfDb.tx";
  ODA_ASSERT_ONCE(sExt == L"dwg" || sExt == L"dxf" || sExt == L"dxb" || sExt == L"dws" || sExt == L"dwt"); // TODO
  if (sModuleName.isEmpty())
    return NULL;
  return OdExtDbModule_cast(::odrxDynamicLinker()->loadModule(sModuleName));
}

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

static void MyAssert(const char* expresssion, const char* fileName, int nLineNo)
{
  std::cout << "\nAssertion Failed: " << expresssion
            << "\nfile: " << fileName << "\nline: " << nLineNo << "\n";
}

static bool MyCheckAssertGroup(const char* pGroup)
{
  if (pGroup && !odStrCmpA(pGroup, "WR"))
    return false;
  return !pGroup || !*pGroup; // asserts of base group are enabled only (default)
  //return true; // asserts of all groups are enabled
}

/************************************************************************/

class OdGLES2RenderClientImpl : public OdTrVisRenderClient
{
public:
  OdAnsiString m_asError,
    m_asWarning;
  virtual bool hasProperty(const OdChar *pPropName) const { return false; }
  virtual OdRxVariantValue getProperty(const OdChar *pPropName) const { return OdRxVariantValue(false); }
  virtual void setProperty(const OdChar *pPropName, OdRxVariantValue pValue) {}
  virtual int outputWindowWidth() const { return 0; }
  virtual int outputWindowHeight() const { return 0; }
  virtual void emitError(const char *pError)
  {
    if (!m_asError.isEmpty())
      m_asError += "\n";
    m_asError += "Error: ";
    m_asError += pError;
  }
  virtual void emitWarning(const char *pWarn)
  {
    if (!m_asWarning.isEmpty())
      m_asWarning += "\n";
    m_asWarning += "Warning: ";
    m_asWarning += pWarn;
  }
};

/************************************************************************/
/* Define a module map for statically linked modules                    */
/************************************************************************/
#if !defined(_TOOLKIT_IN_DLL_) 
/*
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
#if defined(OLEITEM_HANDLER_MODULE_ENABLED)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdOleItemHandlerModuleImpl);
#endif //OLEITEM_HANDLER_MODULE_ENABLED
//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(TrXmlModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(TD_IdViewObjModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,  ModelerModule)
  ODRX_DEFINE_STATIC_APPLICATION(RX_RASTER_SERVICES_APPNAME, ExRasterModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRasterProcessorModuleName,  OdRasterProcessingServicesImpl)
#if defined(OLEITEM_HANDLER_MODULE_ENABLED)
  ODRX_DEFINE_STATIC_APPLICATION(OdOleItemHandlerModuleName, OdOleItemHandlerModuleImpl)
#endif //OLEITEM_HANDLER_MODULE_ENABLED
//  ODRX_DEFINE_STATIC_APPLICATION(OdTrXmlModuleName, TrXmlModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdAcIdViewObjModuleName,       TD_IdViewObjModule)
ODRX_END_STATIC_MODULE_MAP()
*/
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
  int nRes = 1; // Return value for main

#ifdef OD_HAVE_CCOMMAND_FUNC
  argc = ccommand(&argv);
#endif
  setlocale(LC_NUMERIC, "C"); // saving doubles
  setlocale(LC_TIME, ""); // correct strftime formatting

#if !defined(_TOOLKIT_IN_DLL_) 
//  ODRX_INIT_STATIC_MODULE_MAP();
#endif
  odSetAssertFunc(MyAssert);
  odSetCheckAssertGroupFunc(MyCheckAssertGroup);

  // Create a custom Services object.
  OdStaticRxObject<MyServices> svcs;
  // initialize the Drawings SDK
  odInitialize(&svcs);
  try 
  {
    bool bNothingToDump = true;

    OdString sFilePathName,
             sOutPathName; // temp
    int pos;
    OdString fExt;
    OdRxObjectPtr pRxDb;
    OdRxModulePtr pExtDb;

    int indexLayout = -1,  // current only
        idxLayoutToSetup = -1, // for temp usage
        idxArgToReloadNext = -1;
    unsigned int numberLayouts = 0;
    bool bEnabledFaces = true;

    /****************************************************************/
    /* Create the JsonGLES2 rendering device, and set the output     */
    /* stream for the device.                                       */
    /****************************************************************/
    TD_THREEJSJSON_EXPORT::ThreejsJSONModulePtr pModule = odrxDynamicLinker()->loadApp(OdThreejsJSONExportModuleName);
    OdGsDevicePtr pDevice;
    OdTrVisRendition* pRendition = NULL;

    char chOp = L'\0';
    for (int idxArg = 1; true; idxArg++)
    {
      if (indexLayout >= 0)
      {
        ODA_ASSERT_ONCE(   indexLayout >= (chOp == L'l' ? 0 : 1) && numberLayouts > 0 && idxArg >= 2
                        && (chOp == L'd' || chOp == L'l' || chOp == L'p'));
        if (idxArgToReloadNext >= 0)
        {
          if (chOp == L'd')
            idxArg = idxArgToReloadNext;
        }
        else
          idxArg -= 2;
      }
      if (idxArg >= argc)
        break;

      OdString sArg = argv[idxArg];
      sArg.makeLower().replace(L'/', L'-');
      chOp = L'd';
      if (sArg[0] == L'-')
      {
        chOp = sArg[1];
        idxArg++;
      }
      
      if (idxArg < argc && (sArg = argv[idxArg])[0] != L'/' && sArg[0] != L'-')
        sFilePathName = sArg;

      switch (chOp)
      {
      case L'f':
        if (idxArg < argc)
          bEnabledFaces  = !((sArg = argv[idxArg]).makeLower() == L"false" || sArg == L"0");
        break;

      case L'o': // open input DWG/DXF file
        if (bNothingToDump)
          odPrintConsoleString(L"Developed using %ls ver %ls\n", svcs.product().c_str(), svcs.versionString().c_str());
        bNothingToDump = false;
        pExtDb = NULL;
        pRxDb = NULL;

        /******************************************************************/
        /* Create a database and load the drawing into it.                */
        /*                                                                */
        /* Specified arguments are as followed:                           */
        /*     filename, allowCPConversion, partialLoad, openMode         */
        /******************************************************************/
        pos = sFilePathName.reverseFind(L'.');
        if (pos++ > 0)
          pExtDb = loadExtDbModuleByExtension(sFilePathName.mid(pos));
        if (pExtDb.isNull())
          pRxDb = svcs.readFile(sFilePathName.c_str(), false, false, Oda::kShareDenyNo);       
        indexLayout = -1;
        idxArgToReloadNext = -1;
        break;

      case L'j': // serialize to json
        if (bNothingToDump)
        {
          odPrintConsoleString(L"There is nothing to serialize\n");
          break;
        }
        if ((pos = sFilePathName.reverseFind('.')) < 0)
          fExt = L".json";
        else if (pos == 0)
          sFilePathName = L"output";
        else
        {
          fExt = L".json";
          sFilePathName = sFilePathName.left(pos);
        }
        sOutPathName.format(L"%ls%ls", sFilePathName.c_str(), fExt.c_str());
        idxLayoutToSetup = -1;
       
        if (pRxDb.isNull());
        else
        {
          ODCOLORREF clrPaletteBackground = ODRGB(255, 255, 255);

          /****************************************************************/
          /* Set the database to be vectorized.                           */
          /****************************************************************/
          pRendition = NULL;

          OdSmartPtr<OdStreamBuf> buff = ::odrxSystemServices()->createFile(sOutPathName, Oda::kFileWrite, Oda::kShareDenyWrite, Oda::kCreateAlways);

          if (!pModule.isNull())
          {
            OdUInt32 errorCode = pModule->exportThreejsJSON(pRxDb.get(), buff.get(), clrPaletteBackground, bEnabledFaces);
          }
        }
        odPrintConsoleString(L"\nSerialize Done.\n");
        break;

      } // end switch (chOp)
    } // end for

    pDevice = NULL;

    if (bNothingToDump)
    {
      // Verify the argument count and display an error message as required
      odPrintConsoleString(L"usage: ThreeJsSerializer -key arg ... arg ...  -key arg ... arg\n");
	  odPrintConsoleString(L" -f false       to enabled faces\n");
	  odPrintConsoleString(L"      [-f true     - is default]\n");
      odPrintConsoleString(L" -o <input DWG/DXF file to Open> \n");
      odPrintConsoleString(L" -j <output file to Dump JSON> \n");
      return 1;
    }
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

  if (!nRes)
    odPrintConsoleString(L"\nXmlGlesDumper Finished Successfully");
  return nRes;
}

//
