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
/*   XML or BINARY(TGS) file and back.                                  */
/************************************************************************/

#include "OdaCommon.h"
#include "DbDatabase.h"
#include "DbBaseDatabase.h"
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

#include "GlesXmlServerImpl.h"
#include "GlesBinaryServerImpl.h"
#include "SetupLayoutHelper.h"

#include "XmlGlesParser.h"
#include "BinaryGlesParser.h"

#include "CloudTools.h"
#include "ExtDbModule.h"

#include "TrVisRenderClient.h"

#include "diagnostics.h"

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

//#include "LLGFromXmlSrvr.h"
//#include "LLGEncoderTxt.h"
//#include "LLGParserToGLES2.h"
//#include "LLGDecoderTxt.h"

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
  ODRX_INIT_STATIC_MODULE_MAP();
#endif
  odSetAssertFunc(MyAssert);
  odSetCheckAssertGroupFunc(MyCheckAssertGroup);

  // Create a custom Services object.
  OdStaticRxObject<MyServices> svcs;
  // initialize the Drawings SDK
  odInitialize(&svcs);
  // initialize GS subsystem
  //odgsInitialize(); // Not necessary for XmlGLES2 module
  OdXmlGlesIdMapper idMapper;
  try 
  {
    bool bNothingToDump = true;

    OdString sFilePathName,
             sOutPathName; // temp
    int pos;
    OdString fExt;
    OdRxObjectPtr pRxDb;
    OdRxModulePtr pExtDb;
    OdSharedPtr<OdBaseGlesParser> pXmlParser;

    int xSize = 2000, // default of Cloud nodejs server
        ySize = 2000,
        indexLayout = -1,  // current only
        idxLayoutToSetup = -1, // for temp usage
        idxArgToReloadNext = -1;
    unsigned int numberLayouts = 0;
    bool isBinary = false,
         bEnableGsModel = true,
         bSkipShaderDumping = true,
         useClientDependentSectionOnlyFlag = false,
         skipSecondUpdate = true,
         bDiscardBackFaces = true,
         useCompositeMetafiles = true,
         bBlocksCache = false,
         useTTFCache = false,
         bSubentMarkers = false;

    /****************************************************************/
    /* Create the XmlGLES2 rendering device, and set the output     */
    /* stream for the device.                                       */
    /****************************************************************/
    OdGsModulePtr pGs = ::odrxDynamicLinker()->loadModule(OdTrXmlVecModuleName);
    OdGsDevicePtr pDevice;
    OdTrVisRendition* pRendition = NULL;
    OdSharedPtr<OdGlesServerBaseImpl> pXmlServer;

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
      case L's': // Set size output screen rectangle.
        if (idxArg < (argc + 1))
        {
          //bNothingToDump = false;
          sArg = argv[idxArg++];
          xSize = odStrToInt(sArg.c_str());
          sArg = argv[idxArg];
          ySize = odStrToInt(sArg.c_str());
        }
        break;

      case L'b': // true(TGS=Binary) / false(XML)
        if (idxArg < argc)
          isBinary = !((sArg = argv[idxArg]).makeLower() == L"false" || sArg == L"0");
        break;
      case L'l': case L'p': // open input XML file to Parse and Load to Rendition
        bNothingToDump = false;
        pExtDb = NULL;
        pRxDb = NULL;
        pRendition = NULL;
        pDevice = pGs->createDevice();
        {
          OdRxDictionaryPtr pProperties = pDevice->properties();
          if (pProperties->has(L"Rendition"))
            pRendition = (OdTrVisRendition*)OdRxVariantValue(pProperties->getAt(L"Rendition"))->getIntPtr();
        }
        if ((pos = sFilePathName.reverseFind('.')) < 0)
          fExt = isBinary ? L".tgs" : L".xml";
        else if (pos == 0)
          sFilePathName = L"output";
        else
        {
          fExt = sFilePathName.mid(pos);
          if (   fExt.makeLower() == L".dwg" || fExt == L".dxf" 
              || fExt == L".dgn" || fExt == L".rvt" || fExt == L".rfa"
              || fExt == L".dwf")
            fExt = isBinary ? L".tgs" : L".xml";
          sFilePathName = sFilePathName.left(pos);
        }
        if (isBinary || fExt == L".tgs")
          pXmlParser = new OdBinaryGlesParser( &idMapper );
        else
          pXmlParser = new OdXmlGlesParser( &idMapper );
        sOutPathName.format(L"%ls%ls", sFilePathName.c_str(), fExt.c_str());
        if (sOutPathName.find(L"%ls") >= 0)
        {
          if (indexLayout < 0)
          {
            indexLayout = 0;
            idxArgToReloadNext = idxArg - 1;
          }
          sOutPathName = OdString().format(sOutPathName.c_str(), 
                                           OdString().format(L"%d", indexLayout).c_str(),
                                           L"0"); //L"%d"); // possible to split by size limit
        }
        if (!pXmlParser->parse(sOutPathName, pRendition))
        {
          odPrintConsoleString(L"Xml parse error: %ls \n", pXmlParser->errorMessage().c_str());
          argc = 0; // pDevice = NULL;
        }
        // do not it here to keep internal m_mapFakeDbStub in loader //pXmlParser = NULL;
        break;

      //case L'g': // open input LLG file to Parse and Load to Rendition
      //  bNothingToDump = false;
      //  pRxDb = NULL;
      //  {
      //    // send parsed .LLG data back to xml rendition for debugging comparison with orig. xml'ed graphics
      //    class Srvr : public OdGlesXmlServerImpl {
      //    public:
      //      void DropId(const char* pTagName, OdUInt64 data,OdTrVisUniqueID* , OdInt32 ) { DropUInt64(pTagName, data); }
      //    }
      //    xmlServer;
      //    OdStaticRxObject<OdGLES2XmlRendition>  xmlRendition;
      //    xmlRendition.setXmlServer((OdIntPtr)&xmlServer);
      //    xmlServer.setOutPathName(sFilePathName + ".xml");
      //    xmlServer.IncreaseNesting("GsUpdate");
      //    try {
      //      LLGParserToGLES2<LLGDecoderTxt> xmlParser;
      //      xmlParser.parse(sFilePathName, &xmlRendition);
      //      xmlServer.DecreaseNesting("GsUpdate");
      //    } catch(OdError e) {
      //      odPrintConsoleString(L"LLG parse error: %ls \n", e.description().c_str());
      //    } catch(...) {
      //      odPrintConsoleString(L"Unknown LLG parse error\n");
      //    }
      //    argc = 0; // pDevice = NULL;
      //    xmlServer.flushOut();
      //    xmlRendition.setXmlServer((OdIntPtr)0);
      //  }
      //  break;

      case L'm':
        if (idxArg < argc)
          bEnableGsModel = !((sArg = argv[idxArg]).makeLower() == L"false" || sArg == L"0");
      case L'h': 
        if (idxArg < argc)
          bSkipShaderDumping = ((sArg = argv[idxArg]).makeLower() == L"false" || sArg == L"0");
        break;
      case L'c':
        if (idxArg < argc)
          useClientDependentSectionOnlyFlag = !((sArg = argv[idxArg]).makeLower() == L"false" || sArg == L"0");
        break;
      case L'n':
        if (idxArg < argc)
          skipSecondUpdate = !((sArg = argv[idxArg]).makeLower() == L"false" || sArg == L"0");
        break;
      case L'f':
        if (idxArg < argc)
          bDiscardBackFaces  = !((sArg = argv[idxArg]).makeLower() == L"false" || sArg == L"0");
        break;
      case L'u':
        if (idxArg < argc)
          useCompositeMetafiles = !((sArg = argv[idxArg]).makeLower() == L"false" || sArg == L"0");
        break;
      case L'k':
        if (idxArg < argc)
          bBlocksCache = !((sArg = argv[idxArg]).makeLower() == L"false" || sArg == L"0");
        break;
      case L't':
        if (idxArg < argc)
          useTTFCache = !((sArg = argv[idxArg]).makeLower() == L"false" || sArg == L"0");
        break;
      case L'r':
        if (idxArg < argc)
          bSubentMarkers = !((sArg = argv[idxArg]).makeLower() == L"false" || sArg == L"0");
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
        else
          pRxDb = OdExtDbModule_cast(pExtDb)->readFile(sFilePathName.c_str(), Oda::kShareDenyNo);
        indexLayout = -1;
        numberLayouts = numLayouts(pRxDb);
        idxArgToReloadNext = -1;
        break;

      case L'd': // dumping
        if (bNothingToDump)
        {
          odPrintConsoleString(L"There is nothing to dump\n");
          break;
        }
        if ((pos = sFilePathName.reverseFind('.')) < 0)
          fExt = isBinary ? L".tgs" : L".xml";
        else if (pos == 0)
          sFilePathName = L"output";
        else
        {
          fExt = sFilePathName.mid(pos);
          if (   fExt.makeLower() == L".dwg" || fExt == L".dxf" 
              || fExt == L".dgn" || fExt == L".rvt" || fExt == L".rfa"
              || fExt == L".dwf")
            fExt = isBinary ? L".tgs" : L".xml";
          sFilePathName = sFilePathName.left(pos);
        }
        sOutPathName.format(L"%ls%ls", sFilePathName.c_str(), fExt.c_str());
        idxLayoutToSetup = -1;
        if (sOutPathName.find(L"%ls") >= 0)
        {
          if (indexLayout < 0)
            indexLayout = 0;
          sOutPathName = OdString().format(sOutPathName.c_str(), 
                                           OdString().format(L"%d", indexLayout).c_str(),
                                           L"%d"); // possible to split by size limit
          idxLayoutToSetup = indexLayout;
          if (++indexLayout >= (int) numberLayouts)
          {
            indexLayout = -1;
            idxArgToReloadNext = -1;
          }
        }
        if (pRxDb.isNull())
        {
          if (isBinary || fExt == L".tgs")
            pXmlServer = new OdGlesBinaryServerImpl();
          else
            pXmlServer = new OdGlesXmlServerImpl();
          ODA_ASSERT_ONCE(pDevice.get()); 
          OdRxDictionaryPtr pProperties = pDevice->properties();
          ODA_ASSERT_ONCE(pProperties.get() && pProperties->has(L"XmlServer"));
          pProperties->putAt(L"XmlServer", OdRxVariantValue((OdIntPtr)pXmlServer.get()));
          pXmlServer->setSkipShaders(bSkipShaderDumping);
          pXmlServer->setUseFakeDbStub(true);

          pXmlServer->setOutPathName(sOutPathName);

          OdGLES2RenderClientImpl device;
          // instead vectorizer update.
          pRendition->onDeviceUpdateBegin(&device);
          pRendition->onDeviceUpdateEnd(&device);

          pXmlServer->flushOut();

          if (!device.m_asError.isEmpty())
            odPrintConsoleString(OdString().format(L"\n%s", device.m_asError.c_str()));
          else 
            nRes = 0;
          if (!device.m_asWarning.isEmpty())
            odPrintConsoleString(OdString().format(L"\n%s", device.m_asWarning.c_str()));
        }
        else
        {
          ODCOLORREF clrPaletteBackground = ODRGB(255, 255, 255);

          /****************************************************************/
          /* Set the database to be vectorized.                           */
          /****************************************************************/
          pRendition = NULL;
          pDevice = pGs->createDevice();

          /****************************************************************/
          /* Create the vectorization context.                            */
          /* Enable GsModel to have cache.                                */
          /* Prepare the device to render layout in this database.        */
          /* Setup background.                                            */
          /****************************************************************/
          pDevice = setupLayoutHelper(pRxDb, pDevice,
                                      idxLayoutToSetup, bEnableGsModel,
                                      clrPaletteBackground);

          /****************************************************************/
          /* This data must be obtained from client:                      */
          /* 1) setup for output screen rectangle.                        */
          /* 2) setup client settings.                                    */
          /****************************************************************/

          /****************************************************************/
          /* Set output screen rectangle.                                 */
          /****************************************************************/
          OdGsDCRect screenRect(OdGsDCPoint(0, ySize), OdGsDCPoint(xSize, 0));
          pDevice->onSize(screenRect);

          /****************************************************************/
          /* Set client settings.                                         */
          /****************************************************************/
          OdTrVisRendition::ClientSettings cliSets; // <<< obtained from client
          OdRxDictionaryPtr pProperties = pDevice->properties();
          pProperties->putAt(L"ClientSettings", OdRxVariantValue((OdIntPtr)&cliSets));

          /****************************************************************/
          if (pProperties->has(L"DiscardBackFaces"))
            pProperties->putAt(L"DiscardBackFaces", OdRxVariantValue(bDiscardBackFaces));
          if (pProperties->has(L"UseCompositeMetafiles")) 
            pProperties->putAt(L"UseCompositeMetafiles", OdRxVariantValue(useCompositeMetafiles));
          if (pProperties->has(L"BlocksCache")) 
            pProperties->putAt(L"BlocksCache", OdRxVariantValue(bBlocksCache));
          if (pProperties->has(L"UseTTFCache")) 
            pProperties->putAt(L"UseTTFCache", OdRxVariantValue(useTTFCache));
          if (pProperties->has(L"DynamicSubEntHlt"))
            pProperties->putAt(L"DynamicSubEntHlt", OdRxVariantValue(bSubentMarkers));

          /****************************************************************/
          /* Create TGS(Binary) or XML output server.                     */
          /****************************************************************/
          if (isBinary || fExt == L".tgs")
            pXmlServer = new OdGlesBinaryServerImpl(pRxDb);
          else
            pXmlServer = new OdGlesXmlServerImpl(pRxDb);
          pProperties->putAt(OD_T("XmlServer"), OdRxVariantValue((OdIntPtr) pXmlServer.get()));
          pXmlServer->setSkipShaders(bSkipShaderDumping);

          /****************************************************************/
          /* Set output file.                                             */
          /****************************************************************/
          pXmlServer->setOutPathName(sOutPathName);

          /****************************************************************/
          /* Firstly update to get client dependent section only.         */
          /* This is not required, but could be useful for caching servers*/
          /****************************************************************/
          if (useClientDependentSectionOnlyFlag)
            pXmlServer->setProcessingMode(OdTrXmlServer::kClientDependentSectionOnly);
          pDevice->update();
          if (useClientDependentSectionOnlyFlag)
            pXmlServer->setProcessingMode(OdTrXmlServer::kNormalMode);

          if (useClientDependentSectionOnlyFlag || !skipSecondUpdate)
          {
            // Second output stream.
            pXmlServer->setOutPathName(OdString().format(L"%ls1", sOutPathName.c_str()));
            // First full update will drop all initial data to file.
            pDevice->update();
          }

          pXmlServer->flushOut();

          // detach Rendition before it will be destroyed while exiting this scope
          //pProperties->putAt(OD_T("Rendition"), OdRxVariantValue((OdUInt32)0));
          // I assume that rendition erasing is required to avoid dumping of erased objects during device destruction.
          // But rendition will be correctly detached before objects erasing.
          // But since some specific objects can be released by layout helper (GeoMarker, for example),
          // them still can be called for rendition. To avoid crash for this problem we can attach stub instead dumping rendition.
          OdTrVisRenditionPtr renditionStub = OdRxObjectImpl<OdTrVisRenditionDef, OdTrVisRendition>::createObject();
          pProperties->putAt(OD_T("Rendition"), OdRxVariantValue((OdIntPtr)renditionStub.get()));
          pDevice = NULL;
          nRes = 0;
        }
        odPrintConsoleString(L"\nDumping Done.\n");
        break;

      //case L'w': // write llg
      //  {
      //    int n = sFilePathName.reverseFind('.');
      //    if (n < 0)
      //      fExt = L".xml";
      //    else if (n == 0)
      //      sFilePathName = L"output";
      //    else
      //    {
      //      fExt = sFilePathName.mid(n);
      //      if (fExt.makeLower() == L".dwg" || fExt == L".dxf")
      //        fExt = L".xml";
      //      sFilePathName = sFilePathName.left(n);
      //    }
      //  }
      //  if (pRxDb.isNull())
      //  {
      //    LLGFromXmlSrvr<LLGEncoderTxt> llgServer;
      //    ODA_ASSERT_ONCE(pDevice.get()); 
      //    OdRxDictionaryPtr pProperties = pDevice->properties();
      //    ODA_ASSERT_ONCE(pProperties.get() && pProperties->has(L"XmlServer"));
      //    OdStreamBufPtr pOutFile = svcs.createFile(OdString().format(L"%ls%ls", sFilePathName.c_str(), fExt.c_str()), 
      //      Oda::kFileWrite, Oda::kShareDenyWrite, Oda::kCreateAlways);
      //    pProperties->putAt(L"XmlServer", OdRxVariantValue((OdIntPtr)&llgServer));
      //    llgServer.setOutput(pOutFile);
      //
      //    OdGLES2RenderClientImpl device;
      //    // instead vectorizer update.
      //    pRendition->onDeviceUpdateBegin(&device);
      //    pRendition->onDeviceUpdateEnd(&device);
      //
      //    if (!device.m_asError.isEmpty())
      //      odPrintConsoleString(OdString().format(L"\n%s", device.m_asError.c_str()));
      //    if (!device.m_asWarning.isEmpty())
      //      odPrintConsoleString(OdString().format(L"\n%s", device.m_asWarning.c_str()));
      //  }
      //  else
      //  {
      //    // Create the vectorization context.
      //    // This class defines the operations and properties that are
      //    // used in the vectorization of an OdDbDatabase.
      //    OdGiContextForDbDatabasePtr pDwgContext = OdGiContextForDbDatabase::createObject();
      //
      //    // Set the database to be vectorized.
      //    pRendition = NULL;
      //    pDevice = pGs->createDevice();
      //    pDwgContext->setDatabase(OdDbDatabase::cast(pRxDb));
      //
      //    // Enable GsModel to have cache.
      //    pDwgContext->enableGsModel(bEnableGsModel);
      //
      //    // Prepare the device to render the active layout in this database.
      //    pDevice = OdDbGsManager::setupActiveLayoutViews(pDevice, pDwgContext);
      //
      //    // Setup background.
      //    if (OdDbDatabase::cast(pRxDb)->getTILEMODE())
      //    {
      //      // Use black background for ModelSpace
      //      pDevice->setLogicalPalette(::odcmAcadDarkPalette(), 256);
      //      pDevice->setBackgroundColor(ODRGB(0, 0, 0));
      //      pDwgContext->setPaletteBackground(ODRGB(0, 0, 0));
      //    }
      //    else
      //    {
      //      // Use white paper and grey background for ModelSpace
      //      pDevice->setLogicalPalette(::odcmAcadLightPalette(), 256);
      //      pDevice->setBackgroundColor(ODRGB(168, 168, 168));
      //      pDwgContext->setPaletteBackground(ODRGB(255, 255, 255));
      //    }
      //
      //    // This data must be obtained from client:
      //    // 1) setup for output screen rectangle.
      //    // 2) setup client settings.
      //
      //    // Set output screen rectangle.
      //    OdGsDCRect screenRect(OdGsDCPoint(0, ySize), OdGsDCPoint(xSize, 0));
      //    pDevice->onSize(screenRect);
      //
      //    // Set client settings.
      //    OdTrVisRendition::ClientSettings cliSets; // <<< obtained from client
      //    OdRxDictionaryPtr pProperties = pDevice->properties();
      //    pProperties->putAt(L"ClientSettings", OdRxVariantValue((OdIntPtr)&cliSets));
      //
      //    //
      //    if (pProperties->has(L"DiscardBackFaces"))
      //      pProperties->putAt(L"DiscardBackFaces", OdRxVariantValue(bDiscardBackFaces));
      //    if (pProperties->has(L"UseCompositeMetafiles")) 
      //      pProperties->putAt(L"UseCompositeMetafiles", OdRxVariantValue(useCompositeMetafiles));
      //    if (pProperties->has(L"BlocksCache")) 
      //      pProperties->putAt(L"BlocksCache", OdRxVariantValue(bBlocksCache));
      //    if (pProperties->has(L"UseTTFCache")) 
      //      pProperties->putAt(L"UseTTFCache", OdRxVariantValue(useTTFCache));
      //    if (pProperties->has(L"DynamicSubEntHlt"))
      //      pProperties->putAt(L"DynamicSubEntHlt", OdRxVariantValue(bSubentMarkers));
      //
      //    // Create LLG stream.
      //    LLGFromXmlSrvr<LLGEncoderTxt> llgServer;
      //    pProperties->putAt(OD_T("XmlServer"), OdRxVariantValue((OdIntPtr)&llgServer));
      //
      //    // Create output file.
      //    OdStreamBufPtr pOutFile = svcs.createFile(OdString().format(L"%ls%ls", sFilePathName.c_str(), fExt.c_str()), 
      //      Oda::kFileWrite, Oda::kShareDenyWrite, Oda::kCreateAlways);
      //    llgServer.setOutput(pOutFile);
      //
      //    // Firstly update to get client dependent section only.
      //    // This is not required, but could be useful for caching servers.
      //    if (useClientDependentSectionOnlyFlag)
      //      llgServer.setClientDependentSectionOnlyFlag(true);
      //    pDevice->update();
      //    if (useClientDependentSectionOnlyFlag)
      //      llgServer.setClientDependentSectionOnlyFlag(false);
      //
      //    if (useClientDependentSectionOnlyFlag || !skipSecondUpdate) {
      //      // Second output stream.
      //      pOutFile = svcs.createFile(OdString().format(L"%ls%ls1", sFilePathName.c_str(), fExt.c_str()), Oda::kFileWrite, Oda::kShareDenyWrite, Oda::kCreateAlways);
      //      llgServer.setOutput(pOutFile);
      //      // First full update will drop all initial data to file.
      //      pDevice->update();
      //    }
      //
      //    odPrintConsoleString(L"\nDumping Done.\n");
      //    nRes = 0;
      //  }
      //  break;

      } // end switch (chOp)
    } // end for

    pDevice = NULL;
    pXmlServer = NULL;

    if (bNothingToDump)
    {
      // Verify the argument count and display an error message as required
      odPrintConsoleString(L"usage: XmlGlesDumper -key arg ... arg ...  -key arg ... arg\n");
      odPrintConsoleString(L" -o <input DWG/DXF file to Open> \n");
      odPrintConsoleString(L" -d <output file to Dump XML/TGS> \n");
      odPrintConsoleString(L" -b true        to use TGS(Binary)\n");
      odPrintConsoleString(L"      [-b false    - is default(XML)]\n");
      //odPrintConsoleString(L" -w <output file to Write LLG> \n");
      odPrintConsoleString(L" -s <xSize> <ySize>\n");
      odPrintConsoleString(L"      [-s 2000 2000  - is default Size by Cloud NodeJS server]\n");
      //odPrintConsoleString(L" -c true      to use Client dependent section flag\n");
      //odPrintConsoleString(L"      [-c false    - is default]\n");
      odPrintConsoleString(L" -n true       do secoNd update (should be an empty)\n");
      odPrintConsoleString(L"      [-n false    - is default]\n");
      odPrintConsoleString(L" -m false       to disable GS Model\n");
      odPrintConsoleString(L"      [-m true     - is default]\n");
      odPrintConsoleString(L" -h true        to include sHader dumping\n");
      odPrintConsoleString(L"      [-h false    - is default]\n");
      odPrintConsoleString(L" -f false       to discard back Faces\n");
      odPrintConsoleString(L"      [-f true     - is default]\n");
      odPrintConsoleString(L" -u false       to Unuse composite metafiles\n");
      odPrintConsoleString(L"      [-u true     - is default]\n");
      odPrintConsoleString(L" -k true        to use blocKs cache\n");
      odPrintConsoleString(L"      [-k false    - is default]\n");
      odPrintConsoleString(L" -t true        to use Ttf cache\n");
      odPrintConsoleString(L"      [-t false    - is default]\n");
      odPrintConsoleString(L" -r true        to use subentity maRkers\n");
      odPrintConsoleString(L"      [-r false    - is default]\n");
      odPrintConsoleString(L" -l <input XML file to Load>\n");
      //odPrintConsoleString(L" -g <input LLG file to Load>\n");
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
  //odgsUninitialize(); // Not necessary for WinGDI
  odUninitialize();

  if (!nRes)
    odPrintConsoleString(L"\nXmlGlesDumper Finished Successfully");
  return nRes;
}

//
