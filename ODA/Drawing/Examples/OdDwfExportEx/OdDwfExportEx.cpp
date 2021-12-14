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
/* a DWF file of the specified version.                                 */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/*    OdDwfExportEx <source file> <target file> <dwfversion>            */
/*                                                                      */
/************************************************************************/


#ifdef WD_WIN_SYSTEM
#include <io.h>
#include <direct.h>
#endif

#include <time.h>
#include <sys/stat.h>
#include <math.h>
#include <stack>
#include <assert.h>

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a


#include "OdaCommon.h"
#include "DbDatabase.h"
#include "RxObjectImpl.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "ModelerGeometry/ModelerModule.h"
#include "OdFileBuf.h"
#include "ColorMapping.h"
#include "../../Exports/DwfExport/Include/DwfExport.h"
//#include "../../Exports/DwfExport/Include/3dDwfExport.h"
#include "RxDynamicModule.h"
using namespace TD_DWF_EXPORT;

//#ifdef   linux
#include <locale.h>
//#endif

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

/************************************************************************/
/* Define a module map for statically linked modules:                   */
/************************************************************************/
#ifndef _TOOLKIT_IN_DLL_

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwfExportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,   ModelerModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdWinBitmapModuleName,           BitmapModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdRasterProcessorModuleName,     OdRasterProcessingServicesImpl)
  ODRX_DEFINE_STATIC_APPMODULE(OdDwf7ExportModuleName,               OdDwfExportModule)
  ODRX_DEFINE_STATIC_APPMODULE(RX_RASTER_SERVICES_APPNAME,      ExRasterModule)
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
    STD(cout) << "usage: OdDwfExportEx <source file> <target file> <dwfversion>"
              << STD(endl)
              << "dwfversion can be any of XPS, B6, A6, C5.5, A5.5, B5.5, C4, A4, B4 3D for DWF"
              << STD(endl);
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

    try
    {
      /******************************************************************/
      /* Create a database and load the drawing into it.                */
      /******************************************************************/
      OdDbDatabasePtr pDb = svcs.readFile(argv[1]);
      if (pDb.isNull())
      {
        STD(cout) << "Can't read source DWG " << argv[1] << STD(endl);
        return 1;
      }

      OdDwfExportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdDwf7ExportModuleName);
      OdDwfExportPtr exporter = pModule->create();

      /******************************************************************/
      /* Parse the command line                                         */
      /******************************************************************/
      OdString argv3(argv[3]);
      if (!argv3.iCompare(OD_T("3D")))
      {
        Dwf3dExportParams params;
        params.setDatabase(pDb);
        params.setDwfFileName(argv[2]);
        params.setBackground(ODRGB(255, 255, 255));

        params.setXSize(1024); 
        params.setYSize(768); // export device resolution
        params.setPalette(odcmAcadLightPalette());
        params.setTitle("Model"); // section title in 3D DWF
        exporter->export3dDwf(params);
      }
      else 
      {
        /****************************************************************/
        /* Initialize the conversion parameters                         */
        /****************************************************************/
        DwExportParams params;
        params.setInkedArea(false);                                                        // MKU 1/21/2004
        params.setColorMapOptimize(false);                                                 // MKU 1/21/2004
        params.setDatabase(pDb);
        params.setDwfFileName(argv[2]);
        params.setBackground(ODRGB(255, 255, 255));
        params.setPalette(odcmAcadLightPalette());
        params.setEmbedAllFonts(true);

        if (!odStrICmp(argv3, OD_T("XPS")))
        {
          params.setFormat(DW_XPS);
          params.setVersion(nDwf_v60);
        }
        else if (!odStrICmp(argv3, OD_T("B6")))
        {
          params.setFormat(DW_UNCOMPRESSED_BINARY);
          params.setVersion(nDwf_v60);
        }
        else if (!odStrICmp(argv3, OD_T("A6")))
        {
          params.setFormat(DW_ASCII);
          params.setVersion(nDwf_v60);
        }
        else if (!odStrICmp(argv3, OD_T("C5.5")))
        {
          params.setFormat(DW_COMPRESSED_BINARY);
          params.setVersion(nDwf_v55);
        }
        else if (!odStrICmp(argv3, OD_T("B5.5")))
        {
          params.setFormat(DW_UNCOMPRESSED_BINARY);
          params.setVersion(nDwf_v55);
        }
        else if (!odStrICmp(argv3, OD_T("A5.5")))
        {
          params.setFormat(DW_ASCII);
          params.setVersion(nDwf_v55);
        }
        else if (!odStrICmp(argv3, OD_T("C4")))
        {
          params.setFormat(DW_COMPRESSED_BINARY);
          params.setVersion(nDwf_v42);
        }
        else if (!odStrICmp(argv3, OD_T("B4")))
        {
          params.setFormat(DW_UNCOMPRESSED_BINARY);
          params.setVersion(nDwf_v42);
        }
        else if (!odStrICmp(argv3, OD_T("A4")))
        {
          params.setFormat(DW_ASCII);
          params.setVersion(nDwf_v42);
        }
        else
        {
          STD(cout) << "Unknown <dwfversion>." << STD(endl);
          return 0;
        }
        /****************************************************************/
        /* Write the DWF file                                           */
        /****************************************************************/
        exporter->exportDwf(params);
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
