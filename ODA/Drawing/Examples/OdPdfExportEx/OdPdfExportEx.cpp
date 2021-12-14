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
/* This console application reads a DWG/DXF file and saves it to        */
/* a PDF file with the specified export options                         */
/*                                                                      */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/*    OdPdfExportEx <source file> <target file> <options>               */
/*                                                                      */
/************************************************************************/


#ifdef WD_WIN_SYSTEM
#include <io.h>
#include <direct.h>
#endif

#include <stdlib.h>

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a


#include "OdaCommon.h"
#include "DbDatabase.h"
#include "DbBlockTableRecord.h"
#include "DbDictionary.h"
#include "DbViewport.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "RxDynamicModule.h"
#include "PdfExport.h"

#include <locale.h>

#include "DbPlotSettingsValidator.h"
#ifdef ODA_WINDOWS
#include <tchar.h>
#endif

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

using namespace TD_PDF_2D_EXPORT;

/************************************************************************/
/* Define a module map for statically linked modules:                   */
/************************************************************************/
#ifndef _TOOLKIT_IN_DLL_

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(RasterExportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);
#if defined(OD_HAS_OPENGL)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinOpenGLModule);
#endif
#if defined(ODA_WINDOWS)
#if defined(_MSC_VER)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdOleItemHandlerModuleImpl);
#endif
#endif
#if defined (ODA_UNIXOS)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdOleSsItemHandlerModuleImpl);
#endif
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdPsPlotStyleServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdPlotSettingsValidatorModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdPdfExportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(TD_IbModule); //needed for embedded raster images
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(TD_IdViewObjModule);

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelDocObjModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DbConstraints_Module);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(TD_DynBlocksModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(TD_3DSolidHistoryModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPMODULE(OdModelerGeometryModuleName,       ModelerModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName,   OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdRasterExportModuleName,          RasterExportModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdWinBitmapModuleName,             BitmapModule)
#if defined(OD_HAS_OPENGL)
  ODRX_DEFINE_STATIC_APPMODULE(OdWinOpenGLModuleName,             WinOpenGLModule)
#endif
#if defined(ODA_WINDOWS)
#if defined(_MSC_VER)
  ODRX_DEFINE_STATIC_APPMODULE(OdOleItemHandlerModuleName,        OdOleItemHandlerModuleImpl)
#endif
#endif
#if defined (ODA_UNIXOS)
  ODRX_DEFINE_STATIC_APPMODULE(OdOleSsItemHandlerModuleName, OdOleSsItemHandlerModuleImpl)
#endif
  ODRX_DEFINE_STATIC_APPMODULE(RX_RASTER_SERVICES_APPNAME,        ExRasterModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdPlotSettingsValidatorModuleName, OdPlotSettingsValidatorModule)
  ODRX_DEFINE_STATIC_APPLICATION(ODPS_PLOTSTYLE_SERVICES_APPNAME, OdPsPlotStyleServicesImpl)
  ODRX_DEFINE_STATIC_APPMODULE(OdRasterProcessorModuleName,       OdRasterProcessingServicesImpl)
  ODRX_DEFINE_STATIC_APPMODULE(OdPdfExportModuleName,             OdPdfExportModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdAcIdViewObjModuleName,           TD_IdViewObjModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdAeciIbModuleName, TD_IbModule) //needed for embedded raster images

  ODRX_DEFINE_STATIC_APPMODULE(OdModelDocObjModuleName, ModelDocObjModule)
  ODRX_DEFINE_STATIC_APPMODULE(DbConstraintsModuleName, DbConstraints_Module)
  ODRX_DEFINE_STATIC_APPMODULE(OdDynBlocksModuleName, TD_DynBlocksModule)
  ODRX_DEFINE_STATIC_APPMODULE(Od3DSolidHistoryTxModuleName, TD_3DSolidHistoryModule)
ODRX_END_STATIC_MODULE_MAP()

#endif

//----------------------------------------------------------------------------------

class MyServices : public ExSystemServices, public ExHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(ExSystemServices);
private:

/************************************************************************/
/* Compile password support for R18 Drawings                            */
/************************************************************************/
#if defined(_MSC_VER) && !(defined(_WINRT))
bool getPassword(const OdString& dwgName, bool isXref, OdPassword& password)
{
  char buff[_MAX_PATH] = {0};
  STD(cout) << "Enter password to open drawing: " << STD(endl);
  ::CharToOemA((const char*)dwgName, buff);
  STD(cout) << "    " << buff << STD(endl);

  STD(cin).get(buff, '\n').get();
  
  wchar_t pwdBuff[_MAX_PATH] = {0};
  // convert to upper case unicode:
  ::OemToCharW( buff, pwdBuff );
  ::CharUpperW( pwdBuff );
  password = pwdBuff;
  return !password.isEmpty();
}
#endif
public:
  OdGsDevicePtr gsBitmapDevice(OdRxObject* /*pViewObj*/ = NULL,
                               OdDbBaseDatabase* /*pDb*/ = NULL,
                               OdUInt32 flags = 0)
  {
    OdGsModulePtr pM;
    if (GETBIT(flags, kFor2dExportRender))
    {
      if (GETBIT(flags, kFor2dExportRenderHLR))
        return OdGsDevicePtr();
      pM = ::odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName);
    }
    if (pM.isNull())
      pM = ::odrxDynamicLinker()->loadModule(OdWinBitmapModuleName);
    if (pM.isNull())
      return OdGsDevicePtr();

    return pM->createBitmapDevice();
  }
};

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
  
  if (argc < 4) 
  {
    STD(cout) << "usage: OdPdfExportEx <source file> <target file> <exportflags> [options]" << STD(endl)
              << "exportflags is a bit mask:" << STD(endl)
              << " 1st bit - export all layouts, otherwise active layout " << STD(endl)
              << " 2nd and 3rd bits - export solid non gradient hatches " << STD(endl)
              << "     0 - export solid non gradient hatches as bitmaps " << STD(endl)
              << "     1 - export solid non gradient hatches using vectorizer " << STD(endl)
              << "     2 - export solid non gradient hatches as pdf paths " << STD(endl)
              << " 4th bit - export other type hatches using vectorizer, otherwise as bitmaps " << STD(endl)
              << " 5th bit - set zoom to extents " << STD(endl)
              << " 6th bit - use embeded TTF fonts " << STD(endl)
              << " 7th bit - use embeded optimized TTF fonts " << STD(endl)
              << " 8th bit - TTF as a geometry " << STD(endl)
              << "9th bit - SHX as a geomrtry " << STD(endl)
              << "10th bit - enable layer support " << STD(endl)
              << "11th bit - add invisible layers to pdf document " << STD(endl) 
              << "12th bit - add extents bounding box " << STD(endl) 
              << "13th bit - encoded (less size) " << STD(endl) 
              << "14th bit - ASCII hex encoded " << STD(endl) 
              << "15th bit - use hidden line removal algorithm " << STD(endl)
              << "16th bit - make TTF text searchable (if it exported as geometry) " << STD(endl)
              << "17th bit - make SHX text searchable (if it exported as geometry) " << STD(endl)
              << "18th bit - export hyperlinks " << STD(endl)
              << "19th bit - export as WEB PDF (linearized) " << STD(endl)
              << "20th bit - Measuring scale in PDF " << STD(endl)

              << "options could be:" << STD(endl)
              << " --paper A4|Letter|Custom - paper size (work only if zoom to extents flag is enabled) " << STD(endl)
              << " --paper-width XX - custom paper width of XX millimeters " << STD(endl)
              << " --paper-height YY - custom paper height of YY millimeters " << STD(endl)
              << " --geom-res YY - vector resolution is YY dpi " << STD(endl)
              << " --color-res YY - resolution of color raster images is YY dpi " << STD(endl)
              << " --bw-res YY - resolution of monochrome raster images is YY dpi " << STD(endl)
              << " --color-policy - AsIs(default)|MonoPlotstyle(supports in Windows only)|Mono|Gray" << STD(endl)
              << " --archived - None(default)|1b|2b" << STD(endl)
              ;
  }
  else
  {
    /********************************************************************/
    /* For correct Unicode translation, apply the current system locale.*/
    /********************************************************************/
    setlocale(LC_ALL, "");
    /********************************************************************/
    /* But use usual conversion for scanf()/sprintf()                   */
    /********************************************************************/
    setlocale(LC_NUMERIC, "C");

#ifndef _TOOLKIT_IN_DLL_
    ODRX_INIT_STATIC_MODULE_MAP();
#endif

    /********************************************************************/
    /* Create a custom Services instance.                               */
    /********************************************************************/
    OdStaticRxObject<MyServices> svcs;
    odInitialize(&svcs);

    /********************************************************************/
    /* Display the Product and Version that created the executable      */
    /********************************************************************/
    STD(cout) << "Developed using " << (const char*)svcs.product() << ", " << (const char*)svcs.versionString() << STD(endl);
    STD(cout) << "Export flags:" << STD(endl);

    /********************************************************************/
    /* Process command line parameters                                  */
    /********************************************************************/
    int flags = atoi(OdString(argv[3]));

    #define COMMENTFLAG(AFLAG, ACOMMENTTRUE) if(AFLAG) STD(cout) << ACOMMENTTRUE << STD(endl)

    bool bActiveLayout(           GETBIT(flags,      1) ); COMMENTFLAG( bActiveLayout,  "Active layout only"); 
                                                           COMMENTFLAG( !bActiveLayout, "All layouts");
    int nSolidHatchExpType(0);
    SETBIT(nSolidHatchExpType, 1, GETBIT(flags,      2) ); COMMENTFLAG( !GETBIT(flags, 2) && !GETBIT(flags, 4), "Export solid hatches as bitmaps" );
    SETBIT(nSolidHatchExpType, 2, GETBIT(flags,      4) ); COMMENTFLAG( !GETBIT(flags, 2) &&  GETBIT(flags, 4), "Export solid hatches as pdf paths" );   
                                                           COMMENTFLAG(  GETBIT(flags, 2) && !GETBIT(flags, 4), "Export solid hatches using vectorizer" );
    int nOtherHatchExpType(0);
    SETBIT(nOtherHatchExpType, 1, GETBIT(flags,      8) ); COMMENTFLAG( !GETBIT(flags, 8),   "Export other hatches as bitmaps" );   
                                                           COMMENTFLAG(  GETBIT(flags, 8),   "Export other hatches using vectorizer" );
    bool bZoomToExtents(          GETBIT(flags,     16) ); COMMENTFLAG( bZoomToExtents,       "Zoom to extents");
    bool bEmbededTTF(             GETBIT(flags,     32) ); COMMENTFLAG( bEmbededTTF,          "Embeded TTF");
    bool bEmbededOptimizedTTF(    GETBIT(flags,     64) ); COMMENTFLAG( bEmbededOptimizedTTF, "Optimized embeded TTF mode");
    bool bTTFAsAGeometry(         GETBIT(flags,    128) ); COMMENTFLAG( bTTFAsAGeometry,      "TTF as geometry");
    bool bSHXAsAGeometry(         GETBIT(flags,    256) ); COMMENTFLAG( bSHXAsAGeometry,      "SHX as geometry");
    bool bEnableLayersSupport(    GETBIT(flags,    512) ); COMMENTFLAG( bEnableLayersSupport, "Enable layers");
    bool bIncludeOffLayers(       GETBIT(flags,   1024) ); COMMENTFLAG( bIncludeOffLayers,    "Include invisible layers to pdf document");
    bool bExtentsBoundingBox(     GETBIT(flags,   2048) ); COMMENTFLAG( bExtentsBoundingBox,  "Show extents bounding box");
    bool bEncoded(                GETBIT(flags,   4096) ); COMMENTFLAG( bEncoded,             "Encoded");
    bool bASCIIhexEncoded(        GETBIT(flags,   8192) ); COMMENTFLAG( bASCIIhexEncoded,     "ASCII encoded");
    bool bUseHiddenLineAlgo(      GETBIT(flags,  16384) ); COMMENTFLAG(bUseHiddenLineAlgo, "Use hidden line removal algorithm");
    bool bUseSimpleGeomOpt(       GETBIT(flags,  32768) ); COMMENTFLAG(bUseSimpleGeomOpt, "Enable simple geometry optimization");
    bool bSearchableTTF(          GETBIT(flags,  65536) && GETBIT(flags, 128)); COMMENTFLAG(bSearchableTTF, "Make TTF searchable");
    bool bSearchableSHX(          GETBIT(flags, 131072) && GETBIT(flags, 256)); COMMENTFLAG(bSearchableSHX, "Make SHX searchable");
    bool bHyperlinks(             GETBIT(flags, 262144)) ; COMMENTFLAG(bHyperlinks, "Export Hyperlinks");
    bool bLinearized(             GETBIT(flags, 524288)) ; COMMENTFLAG(bLinearized, "Export as WEB PDF");
    bool bMeasuring(              GETBIT(flags, 1048576)); COMMENTFLAG(bMeasuring,  "Measuring scale in PDF");

    bool bMonoPalette = false;
    OdUInt16 iGeomRes = 600;
    OdUInt16 iColorRes = 400;
    OdUInt16 iBWRes = 400;
    PDFExportParams::ColorPolicy cp = PDFExportParams::kNoPolicy;
    PDFExportParams::PDF_A_mode  pdfAmode = PDFExportParams::kPDFA_None;

    STD(map)<OdString, OdString> paramsMap;
    int curIndex = 4;
    if ((argc - curIndex)%2)
    {
      STD(cout) << "Wrong options count." << STD(endl);
      return 1;
    }

    while (curIndex+1 < argc)
    {
      paramsMap[OdString(argv[curIndex])] = OdString(argv[curIndex+1]);
      curIndex += 2;
    }

    STD(map)<OdString, OdString>::iterator iterGeomRes = paramsMap.find(L"--geom-res");
    if (iterGeomRes != paramsMap.end())
    {
      iGeomRes = atoi(iterGeomRes->second);
      if (iGeomRes < 72)
        iGeomRes = 72;
    }

    STD(map) < OdString, OdString > ::iterator iterColorRes = paramsMap.find(L"--color-res");
    if (iterColorRes != paramsMap.end())
    {
      iColorRes = atoi(iterColorRes->second);
      if (iColorRes < 72)
        iColorRes = 72;
      if (iColorRes > iGeomRes)
        iColorRes = iGeomRes;
    }

    STD(map) < OdString, OdString > ::iterator iterBWRes = paramsMap.find(L"--bw-res");
    if (iterBWRes != paramsMap.end())
    {
      iBWRes = atoi(iterBWRes->second);
      if (iBWRes < 72)
        iBWRes = 72;
      if (iBWRes > iGeomRes)
        iBWRes = iGeomRes;
    }

    STD(map) < OdString, OdString > ::iterator iterPDFA = paramsMap.find(L"--archived");
    if (iterPDFA != paramsMap.end())
    {
      if (iterPDFA->second == L"1b")
        pdfAmode = PDFExportParams::kPDFA_1b;
      else if (iterPDFA->second == L"2b")
        pdfAmode = PDFExportParams::kPDFA_2b;
    }

    STD(map) < OdString, OdString > ::iterator iterColor = paramsMap.find(L"--color-policy");
    if (iterColor != paramsMap.end())
    {
      if (iterColor->second == L"MonoPlotstyle")
      {
        bMonoPalette = true;
#ifndef WIN32
          STD(cout) << "Monochrome mode with plotstyles are not supported on non Windows systems yet. Now exiting." << STD(endl);
          return 0;
#endif
      }
      else if (iterColor->second == L"Mono")
        cp = PDFExportParams::kMono;
      else if (iterColor->second == L"Gray")
        cp = PDFExportParams::kGrayscale;
    }

    bool bUsePaperFromOptions(false);
    STD(map)<OdString, OdString>::iterator iterPaper = paramsMap.find(L"--paper");
    // Paper definition is works only with zoom to extents flag
    OdGsPageParams pageParams; // in mm
    if (iterPaper != paramsMap.end() && bZoomToExtents)
    {
      if (iterPaper->second == L"A4")
      {
        bUsePaperFromOptions = true;
        goto next_step; // OdGsPageParams is A4 by default
      }
      else if (iterPaper->second == L"Letter")
      {
        pageParams.set(215.9, 279.4);
        bUsePaperFromOptions = true;
        goto next_step;
      }
      else if (iterPaper->second == L"Custom")
      {
        STD(map)<OdString, OdString>::iterator iterPaperWidth = paramsMap.find(L"--paper-width");
        STD(map)<OdString, OdString>::iterator iterPaperHeight = paramsMap.find(L"--paper-height");
        if (iterPaperWidth != paramsMap.end() && iterPaperHeight != paramsMap.end())
        {
          double dw(.0), dh(.0);
          dw = Od_atof(iterPaperWidth->second);
          dh = Od_atof(iterPaperHeight->second);
          pageParams.set(dw, dh);
          bUsePaperFromOptions = true;
          goto next_step;
        }              
      }

      STD(cout) << "Wrong options." << STD(endl); 
      return 1;
    }

next_step:
    try 
    {
      /******************************************************************/
      /* Create a Pdf export module                                     */
      /******************************************************************/
      OdPdfExportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdPdfExportModuleName);

#if defined (ODA_UNIXOS)
      OdRxModulePtr pOLEModule = odrxDynamicLinker()->loadModule(OdOleSsItemHandlerModuleName);
      if (pOLEModule.isNull())
        odPrintConsoleString(L"Ole module hasn't been loaded. Ole objects will not be printed\n");
#endif

      /******************************************************************/
      /* Create a database and load the drawing into it.                */
      /******************************************************************/

      OdDbDatabasePtr pDb = svcs.readFile(argv[1]); 

      if (!pDb.isNull())
      {
        /****************************************************************/
        /* Initialize the conversion parameters                         */
        /****************************************************************/
            
        OdPdfExportPtr exporter = pModule->create();

        PDFExportParams params;

        params.setDatabase(pDb);
        params.setVersion(PDFExportParams::kPDFv1_5);
        params.setOutput(odSystemServices()->createFile(argv[2], Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways));

        params.setExportFlags(PDFExportParams::PDFExportFlags(
          (bEmbededTTF          ? PDFExportParams::kEmbededTTF             : 0) |
          (bEmbededOptimizedTTF ? PDFExportParams::kEmbededOptimizedTTF    : 0) |
          (bTTFAsAGeometry      ? PDFExportParams::kTTFTextAsGeometry      : 0) |
          (bSHXAsAGeometry      ? PDFExportParams::kSHXTextAsGeometry      : 0) |
          (bEnableLayersSupport ? PDFExportParams::kEnableLayers           : 0) |
          (bIncludeOffLayers    ? PDFExportParams::kIncludeOffLayers       : 0) |
          (bUseSimpleGeomOpt    ? PDFExportParams::kSimpleGeomOptimization : 0) | 
          (bEncoded             ? PDFExportParams::kFlateCompression       : 0) |
          (bASCIIhexEncoded     ? PDFExportParams::kASCIIHexEncoding       : 0) |
          (bUseHiddenLineAlgo   ? PDFExportParams::kUseHLR                 : 0) |
          (bHyperlinks          ? PDFExportParams::kExportHyperlinks       : 0) |
          (bLinearized          ? PDFExportParams::kLinearized             : 0) |
          (bMeasuring           ? PDFExportParams::kMeasuring              : 0) |
          (bZoomToExtents       ? PDFExportParams::kZoomToExtentsMode      : 0)));

        params.setSearchableTextType((PDFExportParams::SearchableTextType)( (bSearchableSHX ? PDFExportParams::kSHX : 0) | (bSearchableTTF ? PDFExportParams::kTTF : 0)));

        if (!bExtentsBoundingBox)
          SETBIT(params.m_reserved1, 1, 1);

        params.setTitle("Batch PDF File");
        params.setAuthor("OdPdfTestEx");
        params.setCreator("Teigha");
        params.setGeomDPI(iGeomRes);
        params.setColorImagesDPI(iColorRes);
        params.setBWImagesDPI(iBWRes);

        params.setArchived(pdfAmode);

        params.setOtherHatchesExportType(GETBIT(nOtherHatchExpType, 1) ? PDFExportParams::kDrawing : PDFExportParams::kBitmap);

        if (cp != PDFExportParams::kNoPolicy)
          params.setColorPolicy(cp);

        switch ( nSolidHatchExpType )
        {
          case 1: // as a drawing (vectorizer)
          {
            params.setSolidHatchesExportType(PDFExportParams::kDrawing);
          }
          break;
          case 2: // as a pdf path
          {
            params.setSolidHatchesExportType(PDFExportParams::kPdfPaths);
          }
          break;
          case 0: // as a bitmap
          default:
          {
            params.setSolidHatchesExportType(PDFExportParams::kBitmap);
          }
          break;
        }

        /****************************************************************/
        /* Initialize the conversion parameters: Layouts                */
        /****************************************************************/

        if (bActiveLayout)
        {
          OdDbBlockTableRecordPtr pLayoutBlock = pDb->getActiveLayoutBTRId().safeOpenObject();
          OdDbLayoutPtr pLayout = pLayoutBlock->getLayoutId().safeOpenObject();
          params.layouts().push_back(pLayout->getLayoutName());
        }
        else
        {
          OdDbDictionaryPtr pLayoutDict = pDb->getLayoutDictionaryId().safeOpenObject();
          OdDbDictionaryIteratorPtr pIt = pLayoutDict->newIterator();

          while(!pIt->done())
          {
            OdDbLayoutPtr pLayout = pIt->getObject();
            OdString sName = pLayout->getLayoutName();

            if (pLayout->getBlockTableRecordId() == pDb->getModelSpaceId())
            {
              params.layouts().insertAt(0, sName);
            }
            else
            {
              OdDbBlockTableRecordPtr pRec = pLayout->getBlockTableRecordId().safeOpenObject();
              OdDbObjectIteratorPtr pIt = pRec->newIterator();
              bool bEmpty = pIt->done();
              if (!bEmpty)
              {
                if (pLayout->overallVportId().isValid())
                {
                  OdDbViewportPtr pOverall = pLayout->overallVportId().safeOpenObject();
                  // test overall view size. Pdf doesn't export bad view and report error.
                  if (!OdZero(pOverall->viewHeight()))
                  {
                    params.layouts().push_back(sName);
                  }
                }
                else
                {
                  params.layouts().push_back(sName);
                }
              }
            }

            pIt->next();
          }
        }

        /****************************************************************/
        /* Initialize the conversion parameters: Monochrome             */
        /****************************************************************/
        // Set current folder to DDPLOTSTYLEPATHS enviroument var to 
        // make possible to find local monochrome.ctb and monochrome.stb 
        // 
        // Or you should to set this env var equal to PrinterStyleSheetDir, 
        // ex. see COdaMfcAppApp::initPlotStyleSheetEnv()
        
#if defined(_WIN32) &&  !defined(__BORLANDC__)

        ::odrxDynamicLinker()->loadModule(OdPlotSettingsValidatorModuleName);
        _tputenv( L"DDPLOTSTYLEPATHS=." );

        bool bMonochrome(false); 
        // Validator creates stylesheets equal to .ctb, .stb files. It's fully depends on DDPLOTSTYLEPATHS env var.
        OdDbPlotSettingsValidatorPtr pValidator = svcs.plotSettingsValidator();
        
        // Get database plot style mode: color-dependent or named
        int i = pDb->getPSTYLEMODE();
        OdString strMono((i == 1) ? "monochrome.ctb" : "monochrome.stb");
        if(bMonoPalette)
        {
          OdArray<const OdChar *> PSSlist;
          pValidator->plotStyleSheetList(PSSlist);

          unsigned int iSize = PSSlist.size();
          if (iSize == 0)
          {
            STD(cout) << "Monochrome mode required files: monochrome.ctb, monochrome.stb to be found. Now exiting." << STD(endl);
            return 2;
          }

          const OdChar* mono = strMono.c_str();
          for(unsigned int i = 0; i < iSize; ++i)
          {
            if (*mono == *PSSlist[i])
            {
              bMonochrome = true;
              break;
            }
          }
        }

        if (bMonochrome)
        {
          OdDbDictionaryPtr pLayoutDict = pDb->getLayoutDictionaryId().safeOpenObject();
          for(unsigned f=0; f<params.layouts().size(); ++f)
          {
            OdDbLayoutPtr pLayout = pLayoutDict->getAt(params.layouts()[f]).safeOpenObject(OdDb::kForWrite);
            pValidator->setCurrentStyleSheet((OdDbPlotSettings*)pLayout, strMono);
          }
        }
#endif
        
        OdUInt32 nPages = params.layouts().size();
        ODA_ASSERT(nPages);
        {
          if (bZoomToExtents)
          {
            OdDbDictionaryPtr pLayoutDict = pDb->getLayoutDictionaryId().safeOpenObject();
            if (bUsePaperFromOptions)
              params.pageParams().resize(nPages, pageParams);
            else
              for(unsigned f=0; f<params.layouts().size(); ++f)
              {
                OdDbLayoutPtr pLayout = pLayoutDict->getAt(params.layouts()[f]).safeOpenObject();
                OdGeExtents3d xts;
                // calculate page sides related to A4 and layout extents
                pLayout->getGeomExtents(xts);
                double scale(.7071);
                if ( fabs(xts.maxPoint().x - xts.minPoint().x) > OdGeTol().equalPoint() ) 
                  scale = fabs(xts.maxPoint().y - xts.minPoint().y) / fabs(xts.maxPoint().x - xts.minPoint().x);              
                pageParams.set(pageParams.getPaperWidth(), pageParams.getPaperWidth()*scale);
                params.pageParams().push_back( pageParams );
              }
          }
          else
          {
            OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject();
            OdDbObjectPtr pModelLayout = pMs->getLayoutId().safeOpenObject();
            OdDbPlotSettingsPtr pPlotSet = OdDbPlotSettings::createObject();
            pPlotSet->copyFrom(pModelLayout);
            double dw(.0), dh(.0);
            pPlotSet->getPlotPaperSize(dw, dh);
            pageParams.set(dw, dh);
            params.pageParams().resize(nPages, pageParams);
          }

          OdUInt32 errCode = exporter->exportPdf(params);
          if (errCode != 0)
          {
            OdString errMes = exporter->exportPdfErrorCode(errCode);
            printf("\nexportPdf error returned : 0x%x. \n%s", (unsigned)errCode, (const char*)errMes);     
          }
        }

      }

    }
    catch (OdError& err)
    {
      OdString msg = err.description();
      STD(cout) << "Teigha Error: " << (const char*)msg << STD(endl) << STD(endl);
    }
    catch (...)
    {
      STD(cout) << "Unknown Error." << STD(endl) << STD(endl);
      return 0;
    }

    odUninitialize();
  }

  return 0;
}
