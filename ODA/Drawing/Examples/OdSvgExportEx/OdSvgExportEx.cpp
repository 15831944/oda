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
/* This console application reads a DWG/DXF file                        */
/* and saves it to a SVG file                                           */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/*    OdSvgExportEx <source file> <target file>                         */
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
//#include "ModelerGeometry/ModelerModule.h"
#include "OdFileBuf.h"
#include "ColorMapping.h"
//#include "../../Exports/DwfExport/Include/DwfExport.h"
#include "RxDynamicModule.h"
#include "RxVariantValue.h"
#include "GiContextForDbDatabase.h"
#include "DbGsManager.h"
#include "DbViewportTableRecord.h"
#include "DbBlockTableRecord.h"

//#ifdef   linux
#include <locale.h>
//#endif

#ifdef OD_HAVE_CONSOLE_H_FILE
#include <console.h>
#endif

//#define SVG_WORLD_COORDINATES
//#define SVG_WHOLE_DRAWING

/************************************************************************/
/* Define a module map for statically linked modules:                   */
/************************************************************************/
#ifndef _TOOLKIT_IN_DLL_

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(SvgExportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdSvgExportModuleName,         SvgExportModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,   ModelerModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdWinBitmapModuleName,           BitmapModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdRasterProcessorModuleName,     OdRasterProcessingServicesImpl)
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
#if defined(_MSC_VER) && !defined(_WINRT)
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

  if (argc < 3)
  {
    STD(cout) << "usage: OdSvgExportEx <source file> <target file>"
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

      if (!pDb.isNull())
      {
        OdGsModulePtr pModule = ::odrxDynamicLinker()->loadModule(OdSvgExportModuleName, false);
        OdGsDevicePtr dev = pModule->createDevice();

        if (!dev.isNull())
        {
          OdStreamBufPtr file = odSystemServices()->createFile(argv[2], Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);   
          // Setup output stream
          dev->properties()->putAt("Output", file.get());
          // size of pixel in device units
          dev->properties()->putAt("LineWeightScale", OdRxVariantValue(1.0));
          // prefix to prepend to image names
          dev->properties()->putAt("ImageUrl", OdRxVariantValue( OdString("./")));
          // "ColorPolicy" - 0 = Colors are unchanged, 1 = Drawing is converted to monochrome, 2 = Drawing is converted to grayscale
          dev->properties()->putAt("ColorPolicy", OdRxVariantValue((OdInt32)0));
          // Set active palette
          dev->setLogicalPalette(odcmAcadLightPalette(), 256);
          // Prepare database context for device
          OdGiContextForDbDatabasePtr pDwgContext = OdGiContextForDbDatabase::createObject();
          pDwgContext->setDatabase(pDb);
          // do not render paper background and borders
          pDwgContext->setPlotGeneration(true);
          // Prepare the device to render the active layout in this database.
          OdGsDevicePtr wrapper = OdDbGsManager::setupActiveLayoutViews( dev, pDwgContext );
          // Setup device coordinate space
#if defined(SVG_WORLD_COORDINATES)
          // this mode illustrates preserving world coordinates in svg
          // used for interaction with InkScape-like vector editors using svg as a base format
          // note that the drawing will look upside-down in svg viewer
          {
            pDb->setTILEMODE(true);
            OdDbBlockTableRecordPtr ms = pDb->getModelSpaceId().safeOpenObject();
            OdGeExtents3d ext;
            ms->getGeomExtents(ext);
            // calculate minimal square viewport size covering all the model
            double x_max = odmax(-ext.minPoint().x, ext.maxPoint().x);
            double y_max = odmax(-ext.minPoint().y, ext.maxPoint().y);
            double size = ceil(odmax(x_max, y_max))*2;
            // set the active viewport to that square
            OdDbViewportTableRecordPtr vp = pDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
            vp->setWidth(size);
            vp->setHeight(size);
            vp->setViewDirection(OdGeVector3d::kZAxis);
            vp->setTarget(OdGePoint3d::kOrigin);
            vp->setCenterPoint(OdGePoint2d::kOrigin);
            OdGsDCRect screenRect(OdGsDCPoint(-size/2, -size/2), OdGsDCPoint(size/2, size/2));
            wrapper->onSize(screenRect);
          }
#elif defined(SVG_WHOLE_DRAWING)
          // this mode illustrates exporting whole drawing, ignoring current viewport
          {
            pDb->setTILEMODE(true);
            OdDbObjectId id = pDb->activeViewportId();
            if (!id.isNull())
            {
              OdDbObjectPtr pVp = id.safeOpenObject(OdDb::kForWrite);
              OdAbstractViewPEPtr pAVP(pVp);
              pAVP->zoomExtents(pVp);
            }
            wrapper->onSize(OdGsDCRect(0, 1024, 768, 0)); // svg coordinates are upside-down
          }
#else
          // just export the current viewport
          wrapper->onSize(OdGsDCRect(0, 1024, 768, 0)); // svg coordinates are upside-down
#endif
          // Initiate rendering.
          wrapper->update( 0 );
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
