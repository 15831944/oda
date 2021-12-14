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

#include <OdaCommon.h>
#include <OdMutex.h>
#include "iOdaApplication.h"

#include "iOdaAppServices.h"
#include <StaticRxObject.h>
#include <OdFileBuf.h>
#include <OdCharMapper.h>
//#include "DbFiller.h"
#include <RxDynamicModule.h>
#ifdef OD_VISUALIZE_ENABLED
#include "TvModuleNames.h"
#include "TvFactory.h"
#endif
// Enable this definition to use OpenGL ES 2.0 vectorizer
#define USE_GLES2_DEVICE
// Uncomment definition in case you would like to test PDFium functionality
//#define PDFIUM_MODULE_ENABLED

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
#ifndef USE_GLES2_DEVICE
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinOpenGLModule);
#else
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(GLES2Module);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdTrGL2RenderModule);
#endif
//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
#if defined(PDFIUM_MODULE_ENABLED)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT  (PdfUnderlayModule);
#endif
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdBrepBuilderFillerModule);
#ifdef USE_TA_LIBS
#include "AECAppStaticModules.h"
ODRX_DECLARE_ARCH_STATIC_MODULES_ENTRY_POINTS()
#endif

#ifdef TC_MODULES_ENABLED
#include "AECCAppStaticModules.h"
ODRX_DECLARE_CIVIL_STATIC_MODULES_ENTRY_POINTS()
ODRX_DECLARE_CIVIL_UI_STATIC_MODULES_ENTRY_POINTS()
#endif

#ifdef OD_VISUALIZE_ENABLED
//Visualize API
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdTvModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDbIOModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ISMModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(SCENEOEModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdBrepBuilderFillerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdTfModule);
//Visualize models generator
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdTvVisualizeSampleModelsFilerModule);
//Dwg2Visualize
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdTvVisualizeDwgFilerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(VisualizeDeviceModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);

//Obj2Visualize
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdTvVisualizeObjFilerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdObjToolkitModuleImpl);

//Stl2Visualize
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdTvVisualizeStlFilerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdStlImportModuleImpl);

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDbPartialViewingModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRxCommonDataAccessModule);
#endif

#ifndef _TOOLKIT_IN_DLL_
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdBrepBuilderFillerModule);
#endif

#ifndef _TOOLKIT_IN_DLL_
ODRX_BEGIN_STATIC_MODULE_MAP()
ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,       ModelerModule)
ODRX_DEFINE_STATIC_APPLICATION(RX_RASTER_SERVICES_APPNAME,        ExRasterModule)
ODRX_DEFINE_STATIC_APPLICATION(OdRasterProcessorModuleName,       OdRasterProcessingServicesImpl)
#ifndef USE_GLES2_DEVICE
ODRX_DEFINE_STATIC_APPLICATION(OdWinOpenGLModuleName,             WinOpenGLModule)
#else
ODRX_DEFINE_STATIC_APPLICATION(OdWinGLES2ModuleName,              GLES2Module)
ODRX_DEFINE_STATIC_APPMODULE(OdTrGL2ModuleName,                   OdTrGL2RenderModule)
#endif
//ODRX_DEFINE_STATIC_APPLICATION(OdWinBitmapModuleName,             BitmapModule)
ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName,     OdRecomputeDimBlockModule)
#if defined(PDFIUM_MODULE_ENABLED)
ODRX_DEFINE_STATIC_APPMODULE  (OdPdfiumModuleName, PdfUnderlayModule)
#endif
#ifdef USE_TA_LIBS
    ODRX_DEFINE_ARCH_STATIC_APPMODULES()
#endif
#ifdef TC_MODULES_ENABLED
ODRX_DEFINE_CIVIL_STATIC_APPMODULES()
ODRX_DEFINE_CIVIL_UI_STATIC_APPMODULES()
#endif
#ifdef OD_VISUALIZE_ENABLED
ODRX_DEFINE_STATIC_APPMODULE(OdTvModuleName,                         OdTvModule)
ODRX_DEFINE_STATIC_APPMODULE(RxCommonDataAccessModuleName,           OdRxCommonDataAccessModule)
ODRX_DEFINE_STATIC_APPMODULE(L"SCENEOE",                             SCENEOEModule)
ODRX_DEFINE_STATIC_APPMODULE(OdDbIOAppName,                          OdDbIOModule)
ODRX_DEFINE_STATIC_APPMODULE(L"ISM",                                 ISMModule)
ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,          ModelerModule)
ODRX_DEFINE_STATIC_APPMODULE(TfModuleName,                           OdTfModule)
ODRX_DEFINE_STATIC_APPMODULE(OdTvModelsGeneratorModuleName,          OdTvVisualizeSampleModelsFilerModule)
ODRX_DEFINE_STATIC_APPMODULE(OdTvDwg2VisualizeModuleName,            OdTvVisualizeDwgFilerModule)
ODRX_DEFINE_STATIC_APPMODULE(OdTvVisualizeDeviceModuleName,          VisualizeDeviceModule)
ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName,        OdRecomputeDimBlockModule)
ODRX_DEFINE_STATIC_APPMODULE(OdTvObj2VisualizeModuleName,            OdTvVisualizeObjFilerModule)
ODRX_DEFINE_STATIC_APPMODULE(OdObjToolkitModuleName,                 OdObjToolkitModuleImpl)
ODRX_DEFINE_STATIC_APPMODULE(OdTvStl2VisualizeModuleName,            OdTvVisualizeStlFilerModule)
//ODRX_DEFINE_STATIC_APPMODULE(OdStlImportModuleName,                  OdStlImportModuleImpl)

ODRX_DEFINE_STATIC_APPMODULE(OdDbPartialViewingModuleName,           OdDbPartialViewingModuleImpl)
#endif
ODRX_END_STATIC_MODULE_MAP()
#endif


static iOdaApplication* global_iOdaApplication_instance = NULL;

@implementation iOdaApplication

+(iOdaApplication*) getInstance {
	if (global_iOdaApplication_instance == NULL) {
		@synchronized(self) {
			if (global_iOdaApplication_instance == NULL) {
				global_iOdaApplication_instance = [[iOdaApplication alloc] init];
			}
		}
	}
	return global_iOdaApplication_instance;
}

//Disabled in new version of application
- (void) odWriteEx: (NSString*)outputFile 
{
#if 0
	OdDbDatabasePtr database = iOdaAppServices::getInstance()->createDatabase();
	
	try {
		
		DbFiller filler;
		filler.fillDatabase(database);
		
	} catch (OdError& e) {

	} catch (...) {
		
	}
	
	char cBuf[1024];
	[outputFile getCString:cBuf];

	OdString saveToStr = cBuf;
	OdWrFileBuf wb(saveToStr);
	
	database->writeFile(&wb, OdDb::kDwg, OdDb::vAC24);
#endif
}

-(void) appInitialize {
#ifndef _TOOLKIT_IN_DLL_
    ODRX_INIT_STATIC_MODULE_MAP();
#endif
    
	odInitialize(iOdaAppServices::getInstance());
#ifdef OD_VISUALIZE_ENABLED
    odTvInitialize();
#endif
    
	NSString* adinitPath = [[NSBundle mainBundle]pathForResource:@"adinit" ofType:@"dat"];
    OdString adinitFile = iOdaAppServices::getInstance()->findFile([adinitPath cStringUsingEncoding:NSUTF8StringEncoding]);
	if (!adinitFile.isEmpty())
		OdCharMapper::initialize(adinitFile);
    
#if defined(PDFIUM_MODULE_ENABLED)
    OdRxModulePtr pRxModule = ::odrxDynamicLinker()->loadModule(OdPdfiumModuleName);
    if (pRxModule.isNull()) {
        OdString tmp = OdString("Missing ") + OdPdfiumModuleName + ".\n";
        odPrintConsoleString((const char *)tmp);
        throw OdError(eTxError);
    }
    odPrintConsoleString("PdfiumModule has been loaded\n");
#endif

}

-(void) appUninitialize {
#ifdef OD_VISUALIZE_ENABLED
    odTvUninitialize();
#endif
	odUninitialize();
}

-(void) dealloc {
}

@end
