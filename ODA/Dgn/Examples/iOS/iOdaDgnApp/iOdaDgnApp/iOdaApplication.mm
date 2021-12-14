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
//#include <OdFileBuf.h>
#include <OdCharMapper.h>
//#include "DbFiller.h"
#include <RxDynamicModule.h>

// Enable this definition to use OpenGL ES 2.0 vectorizer
#define USE_GLES2_DEVICE

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
#ifndef USE_GLES2_DEVICE
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinOpenGLModule);
#else
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdTrGL2RenderModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(GLES2Module);
#endif
//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
#ifdef USE_TA_LIBS
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECArchBase);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECArchBase40);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECArchBase50);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECArchBase60);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECArchDACHBase);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECArchDACHBase40);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECArchDACHBase50);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECArchDACHBase60);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECAreaCalculationBase);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECAreaCalculationBase40);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECAreaCalculationBase50);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECAreaCalculationBase60);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECBase);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECBase40);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECBase50);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECBase60);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECSchedule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECSchedule40);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECSchedule50);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECSchedule60);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECScheduleData);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECScheduleData40);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECScheduleData50);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECScheduleData60);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECStructureBase);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECStructureBase40);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECStructureBase50);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AECStructureBase60);
#endif

//Dgn Import
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);   //this library allows to read V7 files
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwgModule);


#ifndef _TOOLKIT_IN_DLL_
ODRX_BEGIN_STATIC_MODULE_MAP()
ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,       ModelerModule)
ODRX_DEFINE_STATIC_APPLICATION(RX_RASTER_SERVICES_APPNAME,        ExRasterModule)
ODRX_DEFINE_STATIC_APPLICATION(OdRasterProcessorModuleName,       OdRasterProcessingServicesImpl)
#ifndef USE_GLES2_DEVICE
ODRX_DEFINE_STATIC_APPLICATION(OdWinOpenGLModuleName,             WinOpenGLModule)
#else
ODRX_DEFINE_STATIC_APPLICATION(OdTrGL2ModuleName,                 OdTrGL2RenderModule)
ODRX_DEFINE_STATIC_APPLICATION(OdWinGLES2ModuleName,              GLES2Module)
#endif
//ODRX_DEFINE_STATIC_APPLICATION(OdWinBitmapModuleName,             BitmapModule)
ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName,     OdRecomputeDimBlockModule)
#ifdef USE_TA_LIBS
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecArchBase"),               AECArchBase)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecArchBase40"),             AECArchBase40)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecArchBase50"),             AECArchBase50)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecArchBase60"),             AECArchBase60)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecArchDACHBase"),           AECArchDACHBase)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecArchDACHBase40"),         AECArchDACHBase40)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecArchDACHBase50"),         AECArchDACHBase50)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecArchDACHBase60"),         AECArchDACHBase60)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecAreaCalculationBase"),    AECAreaCalculationBase)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecAreaCalculationBase40"),  AECAreaCalculationBase40)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecAreaCalculationBase50"),  AECAreaCalculationBase50)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecAreaCalculationBase60"),  AECAreaCalculationBase60)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecBase"),                   AECBase)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecBase40"),                 AECBase40)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecBase50"),                 AECBase50)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecBase60"),                 AECBase60)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecSchedule"),               AECSchedule)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecSchedule40"),             AECSchedule40)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecSchedule50"),             AECSchedule50)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecSchedule60"),             AECSchedule60)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecScheduleData"),           AECScheduleData)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecScheduleData40"),         AECScheduleData40)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecScheduleData50"),         AECScheduleData50)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecScheduleData60"),         AECScheduleData60)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecStructureBase"),          AECStructureBase)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecStructureBase40"),        AECStructureBase40)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecStructureBase50"),        AECStructureBase50)
ODRX_DEFINE_STATIC_APPLICATION(OD_T("AecStructureBase60"),        AECStructureBase60)
#endif
ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db", OdDgnModule)
ODRX_DEFINE_STATIC_APPLICATION(L"TG_Dgn7IO", OdDgn7IOModuleImpl)
ODRX_DEFINE_STATIC_APPMODULE(L"TG_ModelerGeometry",   DgModelerModule)
ODRX_DEFINE_STATIC_APPMODULE(OdDbModuleName,          OdDwgModule)

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
	odInitialize(iOdaAppServices::getInstance());
    
	NSString* adinitPath = [[NSBundle mainBundle]pathForResource:@"adinit" ofType:@"dat"];
    OdString adinitFile = iOdaAppServices::getInstance()->findFile([adinitPath cStringUsingEncoding:NSUTF8StringEncoding]);
	if (!adinitFile.isEmpty())
		OdCharMapper::initialize(adinitFile);
#ifndef _TOOLKIT_IN_DLL_
	ODRX_INIT_STATIC_MODULE_MAP();
#endif
    ::odrxDynamicLinker()->loadModule(L"TG_Db", false);
}

-(void) appUninitialize {
	odUninitialize();
}

-(void) dealloc {
}

@end
