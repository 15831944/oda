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
#include "OdaCommon.h"
#include "RxDynamicModule.h"
#include "OdModuleNames.h"

#ifdef MECHANICAL_MODE
#include "TmModuleNames.h"
#endif // MECHANICAL_MODE

#ifndef _TOOLKIT_IN_DLL_

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ColladaModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExCustObjsModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExCommandsModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(RasterExportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DbCommandsModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdOleItemHandlerModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinGDIModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinOpenGLModule);
#if !defined(WINDIRECTX_DISABLED)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinDirectXModule);
#endif //WINDIRECTX_DISABLED
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdPsPlotStyleServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdGripPointsModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(SvgExportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdPlotSettingsValidatorModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(Dwf7ImportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(TD_DynBlocksModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExDynamicBlocksModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(AutoSurfServicesModule);
#if defined(DWFDB_ENABLED)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DwfUnderlayModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DwfDbModule);
#endif
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdCurveFunctionsModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdAveModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExFieldEvaluatorModule);
//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdTrueTypeEngineModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(TD_IdViewObjModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerCommandsModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdPdfExportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(TD_3DSolidHistoryModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwfExportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExFieldEvaluatorModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DbPointCloudObjModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExPointCloudHostModule);
#if(POINTCLOUD_ENABLED)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(RcsFileServicesModule);
#endif
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DbCryptModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExTimeProviderWin);
#ifdef PDFIUM_MODULE_ENABLED
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(PdfImportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(PdfUnderlayModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExPdfToRasterModule);
#endif
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DbConstraints_Module);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelDocObjModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(SynergyObjDPWModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdTfModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDbPropertiesModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRxPropertiesModule);
#ifdef DGNIMPORT_ENABLE
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwgDbModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnLSModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgnImportModule);
#endif

//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRxThreadPoolImpl); // uncomment for multicore/multicpu support in statically linked version

//ODRX_DECLARE_ARCH_STATIC_MODULES_ENTRY_POINTS(); // uncomment for Architecture objects support in statically linked version

//ODRX_DECLARE_IFC2DWG_STATIC_MODULES_ENTRY_POINTS(); // uncomment for Ifc2Dwg in static configurations

ODRX_BEGIN_STATIC_MODULE_MAP()

//ODRX_DEFINE_IFC2DWG_STATIC_APPMODULES() // uncomment for Ifc2Dwg in static configurations

ODRX_DEFINE_STATIC_APPMODULE(OdColladaExportModuleName, ColladaModule)
ODRX_DEFINE_STATIC_APPMODULE(OdAutoSurfServicesModuleName, AutoSurfServicesModule)
#if defined(DWFDB_ENABLED)
ODRX_DEFINE_STATIC_APPMODULE(OdDwfUnderlayModuleName, DwfUnderlayModule)
ODRX_DEFINE_STATIC_APPMODULE(OdDwfDbModuleName, DwfDbModule)
#endif
ODRX_DEFINE_STATIC_APPMODULE(OdExCustObjsModuleName, ExCustObjsModule)
ODRX_DEFINE_STATIC_APPMODULE(OdExCommandsModuleName, ExCommandsModule)
ODRX_DEFINE_STATIC_APPMODULE(OdRasterExportModuleName, RasterExportModule)
ODRX_DEFINE_STATIC_APPMODULE(OdDbCommandsModuleName, DbCommandsModule)
ODRX_DEFINE_STATIC_APPMODULE(OdOleItemHandlerModuleName, OdOleItemHandlerModuleImpl)
ODRX_DEFINE_STATIC_APPMODULE(OdWinGDIModuleName, WinGDIModule)
ODRX_DEFINE_STATIC_APPMODULE(OdWinOpenGLModuleName, WinOpenGLModule)
#if !defined(WINDIRECTX_DISABLED)
ODRX_DEFINE_STATIC_APPMODULE(OdWinDirectXModuleName, WinDirectXModule)
#endif //WINDIRECTX_DISABLED
ODRX_DEFINE_STATIC_APPMODULE(OdWinBitmapModuleName, BitmapModule)
ODRX_DEFINE_STATIC_APPMODULE(RX_RASTER_SERVICES_APPNAME, ExRasterModule)
ODRX_DEFINE_STATIC_APPMODULE(OdModelerGeometryModuleName, ModelerModule)
ODRX_DEFINE_STATIC_APPMODULE(OdGripPointsModuleName, OdGripPointsModule)
ODRX_DEFINE_STATIC_APPMODULE(OdSvgExportModuleName, SvgExportModule)
ODRX_DEFINE_STATIC_APPMODULE(OdPlotSettingsValidatorModuleName, OdPlotSettingsValidatorModule)
ODRX_DEFINE_STATIC_APPMODULE(OdDwf7ImportModuleName, Dwf7ImportModule)
ODRX_DEFINE_STATIC_APPLICATION(ODPS_PLOTSTYLE_SERVICES_APPNAME, OdPsPlotStyleServicesImpl)
ODRX_DEFINE_STATIC_APPMODULE(OdDynBlocksModuleName, TD_DynBlocksModule)
ODRX_DEFINE_STATIC_APPMODULE(OdRasterProcessorModuleName, OdRasterProcessingServicesImpl)
ODRX_DEFINE_STATIC_APPMODULE(OdExDynamicBlocksModuleName, ExDynamicBlocksModule)
ODRX_DEFINE_STATIC_APPMODULE(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
ODRX_DEFINE_STATIC_APPMODULE(OdCurveFunctionsModuleName, OdCurveFunctionsModule)
ODRX_DEFINE_STATIC_APPMODULE(OdAveModuleName, OdAveModuleImpl)
ODRX_DEFINE_STATIC_APPMODULE(OdExFieldEvaluatorModuleName, ExFieldEvaluatorModule)
//ODRX_DEFINE_STATIC_APPMODULE(OdFTFontEngineModuleName,        OdTrueTypeEngineModule)
ODRX_DEFINE_STATIC_APPMODULE(OdAcIdViewObjModuleName, TD_IdViewObjModule)
ODRX_DEFINE_STATIC_APPMODULE(OdModelerCommandsModuleName, ModelerCommandsModule)
//ODRX_DEFINE_STATIC_APPMODULE(OdThreadPoolModuleName,          OdRxThreadPoolImpl) // uncomment for multicore/multicpu support in statically linked version
//ODRX_DEFINE_ARCH_STATIC_APPMODULES() // uncomment for Architecture objects support in statically linked version
ODRX_DEFINE_STATIC_APPMODULE(OdPdfExportModuleName, OdPdfExportModule)
ODRX_DEFINE_STATIC_APPMODULE(Od3DSolidHistoryTxModuleName, TD_3DSolidHistoryModule)
ODRX_DEFINE_STATIC_APPMODULE(OdDwf7ExportModuleName, OdDwfExportModule)
ODRX_DEFINE_STATIC_APPMODULE(DbPointCloudObjModuleName, DbPointCloudObjModule)
ODRX_DEFINE_STATIC_APPMODULE(OdExPointCloudHostModuleName, ExPointCloudHostModule)
#if(POINTCLOUD_ENABLED)
ODRX_DEFINE_STATIC_APPMODULE(RX_RCS_FILE_SERVICES, RcsFileServicesModule)
#endif
ODRX_DEFINE_STATIC_APPMODULE(DbCryptModuleName, DbCryptModule)
ODRX_DEFINE_STATIC_APPMODULE(OdTimeProviderModuleName, ExTimeProviderWin)
#ifdef PDFIUM_MODULE_ENABLED
ODRX_DEFINE_STATIC_APPMODULE(OdPdfImportModuleName, PdfImportModule)
ODRX_DEFINE_STATIC_APPMODULE(OdPdfiumModuleName, PdfUnderlayModule)
ODRX_DEFINE_STATIC_APPMODULE(RX_PDF_TO_RASTER_SERVICES_APPNAME, ExPdfToRasterModule)
#endif
ODRX_DEFINE_STATIC_APPMODULE(DbConstraintsModuleName, DbConstraints_Module)
ODRX_DEFINE_STATIC_APPMODULE(OdModelDocObjModuleName, ModelDocObjModule)
ODRX_DEFINE_STATIC_APPMODULE(SynergyObjDPWModuleName, SynergyObjDPWModule)
ODRX_DEFINE_STATIC_APPMODULE(TfModuleName, OdTfModule)
ODRX_DEFINE_STATIC_APPMODULE(RxPropertiesModuleName, OdRxPropertiesModule)
ODRX_DEFINE_STATIC_APPMODULE(DbPropertiesModuleName, OdDbPropertiesModule)
#ifdef DGNIMPORT_ENABLE
ODRX_DEFINE_STATIC_APPMODULE(L"TG_Db", OdDgnModule)
ODRX_DEFINE_STATIC_APPLICATION(L"TG_Dgn7IO", OdDgn7IOModuleImpl)
ODRX_DEFINE_STATIC_APPMODULE(OdDgnLSModuleName, OdDgnLSModule)
ODRX_DEFINE_STATIC_APPLICATION(L"TG_DwgDb", OdDwgDbModuleImpl)
ODRX_DEFINE_STATIC_APPMODULE(L"TG_ModelerGeometry", DgModelerModule)
ODRX_DEFINE_STATIC_APPMODULE(OdDgnImportModuleName, DgnImportModule)
#endif
ODRX_END_STATIC_MODULE_MAP()

#endif

