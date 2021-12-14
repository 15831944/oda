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
// OdaDgnApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "resource.h"
#include "OdaDgnAppDoc.h"
#include "OdaDgnTreeView.h"
#include "LoadApps.h"
#include "BgColorTool.h"
#include "MaterialsEditor.h"
#include "RegKey.h"
#include "CSHelperSheet.h"
#include "ModelerOptions.h"
#include "DgModelerGeometry/DgModelerModule.h"
#include "Wr/wrTriangulationParams.h"

#include "diagnostics.h"
#include "ColorMapping.h"
#include "DynamicLinker.h"
#include "RxDynamicModule.h"
#include "DbLibraryInfo.h"
#include "SetRegVars.h"
#include "Gi/GiWorldDraw.h"
#include "DgModel.h"
#include "DgCmColor.h"
#include "ExDgnCloning/ExDgnCloningModule.h"
#include "ExFileUndoController.h"
#include "ExUndoController.h"
#include "MemFileStreamImpl.h"
#include "FileDlgExt.h"
#include "PreviewDib.h"
#include "RxVariantValue.h"

#ifdef DGNEXPORT_ENABLE
#include "Exports/DgnExport/DgnExport.h"
using namespace TD_DGN_EXPORT;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef _TOOLKIT_IN_DLL_
#include "StaticAppSelDlg.h"
#endif

// COdaDgnApp

BEGIN_MESSAGE_MAP(COdaDgnApp, CWinApp)
  //{{AFX_MSG_MAP(COdaDgnApp)
  ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
  // Standard file based document commands
  ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
  ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
  ON_COMMAND(ID_TOOLS_LOADAPPLICATIONS, OnToolsLoadapplications)
  ON_COMMAND(ID_TOOLS_MODELEROPTIONS, OnToolsModelerOptions)
  ON_COMMAND(ID_TOOLS_SOLIDHELPER, OnToolsSolidHelper)
  ON_COMMAND(ID_VECTORIZE_ADDVECTORIZERDLL, OnVectorizeAddvectorizerdll)
  ON_UPDATE_COMMAND_UI(ID_VECTORIZE_ADDVECTORIZERDLL, OnUpdateVectorizeAddvectorizerdll)
  ON_COMMAND(ID_VECTORIZE_CLEARMENU, OnVectorizeClearmenu)
  ON_COMMAND(ID_OPTIONS_USEGSMODELGEOMETRYCACHE, OnOptionsUseGsModel)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_USEGSMODELGEOMETRYCACHE, OnUpdateOptionsUseGsModel)    
  ON_COMMAND(ID_OPTIONS_USEGSBLOCKGEOMETRYCACHE, OnOptionsUseBlockCache)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_USEGSBLOCKGEOMETRYCACHE, OnUpdateOptionsUseBlockCache)
  ON_COMMAND(ID_OPTIONS_MTLOADING, OnOptionsMTLoading)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_MTLOADING, OnUpdateOptionsMTLoading)
  ON_COMMAND(ID_OPTIONS_MTREGEN, OnOptionsMTRegen)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_MTREGEN, OnUpdateOptionsMTRegen)
  ON_COMMAND(ID_OPTIONS_TRANSFORMLSSCALE, OnOptionsTransformLSScale)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_TRANSFORMLSSCALE, OnUpdateOptionsTransformLSScale)
  ON_COMMAND(ID_OPTIONS_ACTIVELEVELALWAYSVISIBLE, OnOptionsActiveLevelAlwaysVisible)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_ACTIVELEVELALWAYSVISIBLE, OnUpdateOptionsActiveLevelAlwaysVisible)
  ON_COMMAND(ID_OPTIONS_EXACTEXTENTS, OnOptionsExactExtents)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_EXACTEXTENTS, OnUpdateOptionsExactExtents)
  ON_COMMAND(ID_OPTIONS_PROCESSTAGASSOCIATION, OnOptionsProcessTagAssociation)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_PROCESSTAGASSOCIATION, OnUpdateOptionsProcessTagAssociation)
  ON_COMMAND(ID_OPTIONS_PROCESSDIMASSOCIATION, OnOptionsProcessDimAssociation)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_PROCESSDIMASSOCIATION, OnUpdateOptionsProcessDimAssociation)
  ON_COMMAND(ID_OPTIONS_PROCESSMLINEASSOCIATION, OnOptionsProcessMLineAssociation)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_PROCESSMLINEASSOCIATION, OnUpdateOptionsProcessMLineAssociation)
  ON_COMMAND(ID_OPTIONS_USESINGLEVIEWMODE, OnOptionsUseSingleViewMode)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_USESINGLEVIEWMODE, OnUpdateOptionsUseSingleViewMode)
  ON_COMMAND(ID_OPTIONS_3DOBJ_WF_ISOLINES, OnOptions3DObjWireframeDrawIsolines)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_3DOBJ_WF_ISOLINES, OnUpdateOptions3DObjWireframeDrawIsolines )
  ON_COMMAND(ID_OPTIONS_USE_XREFCLRTBL, OnOptionsUseXRefColorTable)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_USE_XREFCLRTBL, OnUpdateOptionsUseXRefColorTable )
  ON_COMMAND(ID_OPTIONS_REMAP_CLRIDX, OnOptionsRemapColorIndexes)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_REMAP_CLRIDX, OnUpdateOptionsRemapColorIndexes )
#ifdef _DEBUG
  ON_COMMAND(ID_OPTIONS_DISABLEASSERTS, OnDisableAsserts)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_DISABLEASSERTS, OnUpdateDisableAsserts)
#endif
  ON_UPDATE_COMMAND_UI_RANGE(ID_VECTORIZER_FIRST, ID_VECTORIZER_LAST, OnUpdateGsDevices)
  ON_UPDATE_COMMAND_UI(ID_VECTORIZE_CLEARMENU, OnUpdateVectorizeClearmenu)
  ON_UPDATE_COMMAND_UI(ID_VECTORIZE, OnUpdateVectorize)
  ON_COMMAND(ID_SETREGVARS_MS_SYMBRSRC, OnToolsSetRegVarsMsSymbRsrc)
  ON_COMMAND(ID_SETREGVARS_MS_RSRC, OnToolsSetRegVarsMsRsrc)
  ON_COMMAND(ID_SETREGVARS_FAST_FONT, OnToolsSetRegVarsFastFont)
  ON_COMMAND(ID_SETREGVARS_NURBS, OnToolsSetRegVarsNurbs)
  ON_COMMAND(ID_SETREGVARS_GRIP, OnToolsSetRegVarsGripPoints)
  ON_COMMAND(ID_SETREGVARS_MS_PATTERN, OnToolsSetRegVarsMsPattern)
  ON_COMMAND(ID_SETREGVARS_MS_BUMP   , OnToolsSetRegVarsMsBump)
  ON_COMMAND(ID_SETREGVARS_MS_MATERIAL, OnToolsSetRegVarsMsMaterial)
  ON_COMMAND(ID_SETREGVARS_MS_MATPALETTE, OnToolsSetRegVarsMsMatPalette)
  ON_COMMAND(ID_SETREGVARS_MS_MTBL, OnToolsSetRegVarsMsMtbl)
  ON_COMMAND(ID_SETREGVARS_MS_PENTABLE, OnToolsSetRegVarsMsPenTable)
  ON_COMMAND(ID_SETREGVARS_MS_DEFCTBL, OnToolsSetRegVarsMsDefCtbl)
  ON_COMMAND(ID_SETREGVARS_MS_FONTPATH, OnToolsSetRegVarsMsFontPath)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define AppRegistryKey _T("Open Design Alliance")

OdStaticRxObject<Cmd_SELECT> g_Cmd_SELECT;
OdStaticRxObject<Cmd_ERASE> g_Cmd_ERASE;
OdStaticRxObject<Cmd_ERASE_ALL_BY_TYPE> g_Cmd_ERASE_ALL_BY_TYPE;
OdStaticRxObject<Cmd_ISOLATE> g_Cmd_ISOLATE;
OdStaticRxObject<Cmd_REDUCE> g_Cmd_REDUCE;
OdStaticRxObject<Cmd_ERASE_Invisible> g_Cmd_ERASEInvisible;
OdStaticRxObject<Cmd_COPY_LINE_STYLES_FROM_RSC> g_Cmd_COPY_LINE_STYLES_FROM_RSC;
OdStaticRxObject<Cmd_ZOOM> g_Cmd_ZOOM;
OdStaticRxObject<Cmd_Fence> g_Cmd_Fence;
OdStaticRxObject<Cmd_ERASE_UNUSED_SHARED_CELLS> g_Cmd_ERASE_UNUSED_SHARED_CELLS;
OdStaticRxObject<Cmd_CREATE_VIEW_GROUPS> g_Cmd_CREATE_VIEW_GROUPS;
OdStaticRxObject<Cmd_UNDO> g_Cmd_UNDO;
OdStaticRxObject<Cmd_LINE> g_Cmd_LINE;
OdStaticRxObject<Cmd_ARC> g_Cmd_ARC;
OdStaticRxObject<Cmd_CIRCLE> g_Cmd_CIRCLE;
OdStaticRxObject<Cmd_TESTUNDOPL> g_Cmd_TESTUNDOPL;
OdStaticRxObject<Cmd_CONE> g_Cmd_CONE;
OdStaticRxObject<Cmd_TEXT> g_Cmd_TEXT;
OdStaticRxObject<Cmd_ConvertToExtGraphics> g_Cmd_ConvertToExtGraphics;
OdStaticRxObject<Cmd_ConvertToMesh> g_Cmd_ConvertToMesh;
OdStaticRxObject<Cmd_SetGlobalOrigin> g_Cmd_SetGlobalOrigin;
OdStaticRxObject<Cmd_CraeteDefaultInst> g_Cmd_CraeteDefaultInst;
OdStaticRxObject<Cmd_Scale2> g_Cmd_Scale2;
OdStaticRxObject<Cmd_RotateXY30> g_Cmd_RotateXY30;

// Define module map for statically linked modules:

#ifndef _TOOLKIT_IN_DLL_

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinGDIModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinOpenGLModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgGripPointsModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);   //this library allows to read V7 files
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdPdfExportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(SvgExportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgDumperImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdOleItemHandlerModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgPsPlotStyleServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgCurveFunctionsModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgRecomputeAssocPtsModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwfExportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(RasterExportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRxThreadPoolService);
#if defined (PDFIUM_MODULE_ENABLED)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExPdfToRasterModule);
#endif
#ifdef TDRAWING_ENABLED
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwgModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwgDbModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnLSModule);
#if defined DGNIMPORT_ENABLE
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgnImportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgImportLineStyleModule);
#endif
#if defined DGNEXPORT_ENABLE
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgnExportModule);
#endif
#endif

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPMODULE(L"TG_Db",              OdDgnModule)
  ODRX_DEFINE_STATIC_APPMODULE(L"TG_ModelerGeometry", DgModelerModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdWinGDIModuleName,    WinGDIModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdWinOpenGLModuleName, WinOpenGLModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdWinBitmapModuleName, BitmapModule)
  ODRX_DEFINE_STATIC_APPMODULE(L"ExDgnGripPoints", OdDgGripPointsModule)
  ODRX_DEFINE_STATIC_APPMODULE(RX_RASTER_SERVICES_APPNAME,   ExRasterModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdRasterProcessorModuleName,    OdRasterProcessingServicesImpl)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Dgn7IO",        OdDgn7IOModuleImpl)
  ODRX_DEFINE_STATIC_APPMODULE(L"DgPlotStyleServices", OdDgPsPlotStyleServicesImpl)
  ODRX_DEFINE_STATIC_APPMODULE(OdPdfExportModuleName,             OdPdfExportModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdSvgExportModuleName,             SvgExportModule)
  ODRX_DEFINE_STATIC_APPMODULE(L"ExDgnDumper",             OdDgDumperImpl)
  ODRX_DEFINE_STATIC_APPMODULE(OdOleItemHandlerModuleName,        OdOleItemHandlerModuleImpl)
  ODRX_DEFINE_STATIC_APPMODULE(L"ExDgnRecomputeAssocPoints", OdDgRecomputeAssocPtsModule)
  ODRX_DEFINE_STATIC_APPMODULE(L"ExDgnCurveFunctions", OdDgCurveFunctionsModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdDwf7ExportModuleName,                 OdDwfExportModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdRasterExportModuleName,          RasterExportModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdThreadPoolModuleName, OdRxThreadPoolService)
#if defined (PDFIUM_MODULE_ENABLED)
  ODRX_DEFINE_STATIC_APPMODULE(RX_PDF_TO_RASTER_SERVICES_APPNAME, ExPdfToRasterModule)
#endif
#ifdef TDRAWING_ENABLED
  ODRX_DEFINE_STATIC_APPMODULE(OdDbModuleName, OdDwgModule)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_DwgDb", OdDwgDbModuleImpl)
  ODRX_DEFINE_STATIC_APPMODULE(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdModelerGeometryModuleName, ModelerModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdDgnLSModuleName, OdDgnLSModule)
#if defined DGNIMPORT_ENABLE
  ODRX_DEFINE_STATIC_APPMODULE(OdDgnImportModuleName, DgnImportModule)
  ODRX_DEFINE_STATIC_APPMODULE(L"ExDgnImportLineStyle", OdDgImportLineStyleModule)
#endif
#if defined DGNEXPORT_ENABLE
  ODRX_DEFINE_STATIC_APPMODULE(OdDgnExportModuleName, DgnExportModule)
#endif
#endif
ODRX_END_STATIC_MODULE_MAP()

#endif

/************************************************************************/
/* Add sheet model border-drawing protocol extension                                 */
/************************************************************************/
static void addSheetModelDrawingCustomization()
{

  static class OdDgModelSheetPEImpl : public OdStaticRxObject<OdDgModelSheetPE>
  {

  public:

    //-----------------------------------------------------------------------------------

    virtual bool drawPaper( const OdDgModel* , OdGiWorldDraw* pWd, OdGePoint3d* points )
    {
      return false;
    }

    //-----------------------------------------------------------------------------------

    virtual bool drawBorder( const OdDgModel* pModel, OdGiWorldDraw* pWd, OdGePoint3d* points )
    {
      OdDgCmEntityColor  oldFillColor  = pWd->subEntityTraits().secondaryTrueColor();
      OdUInt32           oldDrawFlags  = pWd->subEntityTraits().drawFlags();
      OdGiFillType       oldFillType   = pWd->subEntityTraits().fillType();

      OdGePoint3d ptsBorder[5];
      ptsBorder[0] = points[0];
      ptsBorder[1] = points[1];
      ptsBorder[2] = points[2];
      ptsBorder[3] = points[3];
      ptsBorder[4] = points[0];

      double dSheetWidth = ptsBorder[0].distanceTo(ptsBorder[1]);
      double dSheetHeight = ptsBorder[0].distanceTo(ptsBorder[3]);

      double dSheetMin = ( dSheetHeight > dSheetWidth ) ? dSheetWidth : dSheetHeight;

      if( OdZero( dSheetMin ) )
        return false;

      OdGeVector3d vrXDir = ptsBorder[1] - ptsBorder[0];
      OdGeVector3d vrYDir = ptsBorder[3] - ptsBorder[0];

      vrXDir.normalize();
      vrYDir.normalize();

      OdDgCmEntityColor  borderColor( 74,74,74 );
      OdDgCmEntityColor  shadowColor( 45,45,45 );

      double dShadowOffsetFactor = 0.013;

      pWd->subEntityTraits().setTrueColor( borderColor );
      pWd->subEntityTraits().setLineWeight(OdDb::kLnWt000);

      OdDgDatabase* pDb = pModel->database();

      if( pDb )
      {
        OdDgElementId lineStyleId = pDb->getLineStyleTable( OdDg::kForRead )->getAt( 0 ); // solid line
        pWd->subEntityTraits().setLineType( lineStyleId );
      }

      pWd->geometry().polyline(5, ptsBorder);

      double dShadowOffset = dSheetMin * dShadowOffsetFactor;

      OdGePoint3d shadowPts[7];

      shadowPts[0] = ptsBorder[0] + vrXDir * dShadowOffset;
      shadowPts[1] = shadowPts[0] - vrYDir * dShadowOffset;
      shadowPts[2] = shadowPts[1] + vrXDir * dSheetWidth;
      shadowPts[3] = shadowPts[2] + vrYDir * dSheetHeight;
      shadowPts[4] = shadowPts[3] - vrXDir * dShadowOffset;
      shadowPts[5] = ptsBorder[1];
      shadowPts[6] = shadowPts[0];

      pWd->subEntityTraits().setTrueColor(shadowColor);
      pWd->subEntityTraits().setSecondaryTrueColor(shadowColor);
      pWd->subEntityTraits().setDrawFlags( pWd->subEntityTraits().drawFlags() | OdGiSubEntityTraits::kDrawContourFill );
      pWd->subEntityTraits().setFillType(kOdGiFillAlways);

      pWd->geometry().polygon(7, shadowPts);

      pWd->subEntityTraits().setFillType(oldFillType);
      pWd->subEntityTraits().setSecondaryTrueColor(oldFillColor);
      pWd->subEntityTraits().setDrawFlags(oldDrawFlags);

      return true;
    }

    //-----------------------------------------------------------------------------------

    virtual bool drawMargins( const OdDgModel* pModel, OdGiWorldDraw* pWd, OdGePoint3d* points )
    {
      double dSheetWidth = points[0].distanceTo(points[1]);
      double dSheetHeight = points[0].distanceTo(points[3]);

      double dSheetMin = ( dSheetHeight > dSheetWidth ) ? dSheetWidth : dSheetHeight;

      if( OdZero( dSheetMin ) )
        return false;

      OdGeVector3d vrXDir = points[1] - points[0];
      OdGeVector3d vrYDir = points[3] - points[0];

      vrXDir.normalize();
      vrYDir.normalize();

      double dMarginOffsetFactor = 0.025;

      OdDgCmEntityColor  borderColor( 74,74,74 );

      double dMarginOffset = dSheetMin * dMarginOffsetFactor;

      pWd->subEntityTraits().setDrawFlags( pWd->subEntityTraits().drawFlags() & ~OdGiSubEntityTraits::kDrawContourFill );
      pWd->subEntityTraits().setTrueColor(borderColor);

      OdGePoint3d marginAnglePts[3];
      OdGePoint3d marginLinePts[8];

      marginAnglePts[0] = points[0];
      marginAnglePts[0] += vrXDir * dMarginOffset;
      marginAnglePts[1] = marginAnglePts[0] - vrXDir * dMarginOffset;
      marginAnglePts[2] = marginAnglePts[1] + vrYDir * dMarginOffset;
      marginLinePts[0] = marginAnglePts[0];
      marginLinePts[7] = marginAnglePts[2];

      pWd->geometry().polyline(3, marginAnglePts);

      marginAnglePts[0] = points[1];
      marginAnglePts[0] -= vrXDir * dMarginOffset;
      marginAnglePts[1] = marginAnglePts[0] + vrXDir * dMarginOffset;
      marginAnglePts[2] = marginAnglePts[1] + vrYDir * dMarginOffset;

      marginLinePts[1] = marginAnglePts[0];
      marginLinePts[2] = marginAnglePts[2];

      pWd->geometry().polyline(3, marginAnglePts);

      marginAnglePts[0] = points[2];
      marginAnglePts[0] -= vrXDir * dMarginOffset;
      marginAnglePts[1] = marginAnglePts[0] + vrXDir * dMarginOffset;
      marginAnglePts[2] = marginAnglePts[1] - vrYDir * dMarginOffset;

      marginLinePts[3] = marginAnglePts[2];
      marginLinePts[4] = marginAnglePts[0];

      pWd->geometry().polyline(3, marginAnglePts);

      marginAnglePts[0] = points[3];
      marginAnglePts[0] += vrXDir * dMarginOffset;
      marginAnglePts[1] = marginAnglePts[0] - vrXDir * dMarginOffset;
      marginAnglePts[2] = marginAnglePts[1] - vrYDir * dMarginOffset;

      marginLinePts[5] = marginAnglePts[0];
      marginLinePts[6] = marginAnglePts[2];

      pWd->geometry().polyline(3, marginAnglePts);

      OdDgDatabase* pDb = pModel->database();

      if( pDb )
      {
        OdDgElementId lineStyleId = pDb->getLineStyleTable( OdDg::kForRead )->getAt( 2 ); // dashed line
        pWd->subEntityTraits().setLineType( lineStyleId );
      }

      pWd->geometry().polyline(2, marginLinePts);
      pWd->geometry().polyline(2, marginLinePts + 2);
      pWd->geometry().polyline(2, marginLinePts + 4);
      pWd->geometry().polyline(2, marginLinePts + 6);

      return true;
    }
  }
  s_SheetModelDrawExt;

  OdDgModel::desc()->addX(OdDgModelSheetPE::desc(), &s_SheetModelDrawExt);
}

/************************************************************************/
/* Remove sheet model border drawing protocol extension                 */
/************************************************************************/

static void removeSheetModelDrawingCustomization()
{
  OdDgModel::desc()->delX( OdDgModelSheetPE::desc() );
}

// COdaDgnApp construction

COdaDgnApp::COdaDgnApp()
  : m_bLoading(false)
  , m_bUseGsModel(true)
  , m_bBlocksCache(false)
  , m_pDbHostApp(0)
  , m_bUseTempFiles(false)
{
#ifdef _DEBUG
  m_bDisableAsserts = false;
#endif
  // TODO: add construction code here,
  // Place all significant initialization in InitInstance
}


// The one and only COdaDgnApp object
COdaDgnApp theApp;

#ifdef TDRAWING_ENABLED
/************************************************************************/
/* Create a Custom Db HostAppServices class                            */
/************************************************************************/
OdStaticRxObject<MyDbHostAppServices> dbSvcs;
#endif

/************************************************************************/
/* Simple trace function                                                */
/************************************************************************/
static void myTrace(const OdChar* debugString)
{
#ifndef NDEBUG
  OutputDebugStringW((LPCWSTR)debugString);
#endif
}

/************************************************************************/
/* Displays MessageBox for Assertion failure                            */
/************************************************************************/
static void myAssert(const char* expr, const char* fileName, int nLine)
{
  USES_CONVERSION;
  char sMsg[MAX_PATH*3];
  sprintf(sMsg, "expression: \"%s\"\n\nfile: \"%s\"\nline: %d",
    expr, fileName, nLine);

  switch(MessageBox(NULL, A2CT(sMsg), _T("Assertion failed!"), MB_ICONWARNING|MB_ABORTRETRYIGNORE))
  {
  case IDABORT:
    exit(0);
    break;
  case IDRETRY:
    DebugBreak();
    break;
  case IDIGNORE:
  default:
    break;
  }
}

/************************************************************************/

static void myAssertOff(const char* expr, const char* fileName, int nLine)
{
  // empty function for asserts.
}

/************************************************************************/

static bool myCheckAssertGroup(const char* pGroup)
{
  if (pGroup && OdAnsiString(pGroup) == "DGN")
    return true;
  return !pGroup || !*pGroup; // asserts of base group are enabled only (default)
  //return true; // asserts of all groups are enabled
}

/************************************************************************/

static OdUInt32 splitByChar(OdStringArray &outputArray, const OdString &inputString, OdChar separator, bool bTrim = true)
{
  const OdChar *pStr = inputString.c_str(), *pStrPtr;
  do {
    pStrPtr = pStr;
    while ((*pStrPtr != separator) && (*pStrPtr != 0))
      pStrPtr++;
    if (pStrPtr != pStr)
    {
      outputArray.append(OdString(pStr, (int)(pStrPtr - pStr)));
      if (bTrim) { outputArray.last().trimLeft(); outputArray.last().trimRight(); }
    }
    pStr = pStrPtr + 1;
  } while (*pStrPtr != 0);
  return outputArray.size();
}

/************************************************************************/

// COdaDgnApp initialization

BOOL COdaDgnApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
#if _MSC_VER < 1600
  _set_sbh_threshold(1016);
#endif
  //setlocale( LC_NUMERIC, "C" ); // saving doubles to dxf
  //setlocale( LC_TIME, "" ); // correct strftime formatting
  // Client application can set the locale
	setlocale(LC_ALL, "");

  // Change the registry key under which our settings are stored.
  SetRegistryKey(AppRegistryKey);

  settings().load(this);

  // Customize diagnostics:
  ::odSetTraceFunc(myTrace);
  ::odSetAssertFunc(myAssert);
  ::odSetCheckAssertGroupFunc(myCheckAssertGroup);
  try
  {
#ifdef _DEBUG
    // 09/01/15 mku - see comment to #5571 - FPU check conflicts with non-ODA products as PdfVI, OpenGL/DirectX & etc.
    // This will cause OdaMfcApp to crash if it will perform illegal FP operation
    //_clear87();
    //_control87(
    //  //              off on
    //  //_EM_INVALID       |
    //  //_EM_DENORMAL      |
    //  //_EM_ZERODIVIDE    |
    //  //_EM_OVERFLOW      |
    //  //_EM_UNDERFLOW     |
    //  //_EM_INEXACT       ,
    //  //_MCW_EM
    //  );
#endif

#ifndef _TOOLKIT_IN_DLL_
    ODRX_INIT_STATIC_MODULE_MAP();
#endif
    OdStringArray arrayRscList;
    OdStringArray arrayTexDifDirList;
    OdStringArray arrayTexBumpDirList;
    OdStringArray arrayMaterialDirList;
    OdStringArray arrayMatPaletteDirList;
    OdStringArray arrayMtblDirList;
    OdStringArray arrayFontPathDirList;

    setDG_EXACT_EXTENTS(AfxGetApp()->GetProfileInt(_T("RegVars"), _T("DG_EXACT_EXTENTS"), 0 ) ? true : false );
    setDG_ACTIVELEVELALWAYSVISIBLE(AfxGetApp()->GetProfileInt(_T("RegVars"), _T("DG_ACTIVELEVELALWAYSVISIBLE"), 0 ) ? true : false );
    setDG_PROCESSTAGASSOC(AfxGetApp()->GetProfileInt(_T("RegVars"), _T("DG_PROCESSTAGASSOC"), 1 ) ? true : false );
    setDG_PROCESSDIMASSOC(AfxGetApp()->GetProfileInt(_T("RegVars"), _T("DG_PROCESSDIMASSOC"), 1) ? true : false);
    setDG_PROCESSMLINEASSOC(AfxGetApp()->GetProfileInt(_T("RegVars"), _T("DG_PROCESSMLINEASSOC"), 1) ? true : false);
    setMS_3DOBJ_WF_ISOLINES(AfxGetApp()->GetProfileInt(_T("RegVars"), _T("MS_3DOBJ_WF_ISOLINES"), 0 ) ? true : false );
    setTG_USE_XREFCTBL(AfxGetApp()->GetProfileInt(_T("RegVars"), _T("TG_USE_XREFCTBL"), 1 ) ? true : false );
    setTG_REMAP_CLRIDX(AfxGetApp()->GetProfileInt(_T("RegVars"), _T("TG_REMAP_CLRIDX"), 1 ) ? true : false );

    bool bMultithreadLoading = GetProfileInt(_T("options"), _T("UseMTLoading"),      1) != 0;

    if( bMultithreadLoading )
      setMtMode(OdDb::kMTLoading, true);
    else
      setMtMode(OdDb::kMTLoading, false);

    bool bMultithreadRegen = GetProfileInt(_T("options"), _T("UseMTRendering"), 0) != 0;

    if( bMultithreadRegen )
      setMtMode(OdDb::kMTRendering, true);
    else
      setMtMode(OdDb::kMTRendering, false);

    bool bTransformLSScale = GetProfileInt(_T("options"), _T("TransformLSScale"), 0) != 0;
    setTG_LSSCALE_TRANSFORM(bTransformLSScale);
      
    m_bUseGsModel  = GetProfileInt(_T("options"), _T("UseGsModel"),                  1) != 0;
    m_bBlocksCache = GetProfileInt(_T("options"), _T("UseGsBlockCache"),             0) != 0;
    m_bUseSingleViewMode = GetProfileInt(_T("options"), _T("UseSingleViewMode"),    0) != 0;
    //OdString sRscPath("C:\\Documents and Settings\\All Users\\Application Data\\Bentley\\WorkSpace\\System\\Symb\\font.rsc");
    OdString sRscPath( AfxGetApp()->GetProfileString(_T("RegVars"), _T("MS_SYMBRSRC"), _T("")) );
    if ( !sRscPath.isEmpty() )
    {
      ::splitByChar(arrayRscList, sRscPath, ';');
      setMS_SYMBRSRC( arrayRscList );
    }

    //OdString sRscPath("C:\\Documents and Settings\\All Users\\Application Data\\Bentley\\WorkSpace\\System\\Materials\\Pattern");
    OdString sRscTexDifPath( AfxGetApp()->GetProfileString(_T("RegVars"), _T("MS_PATTERN"), _T("")) );
    if ( !sRscTexDifPath.isEmpty() )
    {
      ::splitByChar(arrayTexDifDirList, sRscTexDifPath, ';');
      setMS_PATTERN( arrayTexDifDirList );
    }

    //OdString sRscPath("C:\\Documents and Settings\\All Users\\Application Data\\Bentley\\WorkSpace\\System\\Materials\\Bump");
    OdString sRscTexBumpPath( AfxGetApp()->GetProfileString(_T("RegVars"), _T("MS_BUMP"), _T("")) );
    if ( !sRscTexBumpPath.isEmpty() )
    {
      ::splitByChar(arrayTexBumpDirList, sRscTexBumpPath, ';');
      setMS_BUMP( arrayTexBumpDirList );
    }

    //OdString sRscPath("C:\\Documents and Settings\\All Users\\Application Data\\Bentley\\WorkSpace\\System\\Fonts");
    OdString sRscFontPath( AfxGetApp()->GetProfileString(_T("RegVars"), _T("MS_FONTPATH"), _T("")) );
    if ( !sRscFontPath.isEmpty() )
    {
      ::splitByChar(arrayFontPathDirList, sRscFontPath, ';');
      setMS_FONTPATH( arrayFontPathDirList );
    }
    //OdString sRscPath("C:\\Documents and Settings\\All Users\\Application Data\\Bentley\\WorkSpace\\System\\Materials");
    OdString sRscMaterialPath( AfxGetApp()->GetProfileString(_T("RegVars"), _T("MS_MATERIAL"), _T("")) );
    if ( !sRscMaterialPath.isEmpty() )
    {
      ::splitByChar(arrayMaterialDirList, sRscMaterialPath, ';');
      setMS_MATERIAL( arrayMaterialDirList );
    }

    //OdString sRscPath("");
    OdString sRscMatPalettePath( AfxGetApp()->GetProfileString(_T("RegVars"), _T("MS_MATPALETTE"), _T("")) );
    if ( !sRscMatPalettePath.isEmpty() )
    {
      ::splitByChar(arrayMatPaletteDirList, sRscMatPalettePath, ';');
      setMS_MATPALETTE( arrayMatPaletteDirList );
    }

    //OdString sRscPath("");
    OdString sRscMtblPath( AfxGetApp()->GetProfileString(_T("RegVars"), _T("MS_MTBL"), _T("")) );
    if ( !sRscMtblPath.isEmpty() )
    {
      ::splitByChar(arrayMtblDirList, sRscMtblPath, ';');
      setMS_MTBL( arrayMtblDirList );
    }

    setTG_NURBS_GRID( AfxGetApp()->GetProfileInt(_T("RegVars"), _T("TG_NURBS_GRID"), 10) );

    setDG_GRIPSIZE( AfxGetApp()->GetProfileInt(_T("RegVars"), _T("DG_GRIPSIZE"), 5) );
    setDG_GRIPOBJLIMIT( AfxGetApp()->GetProfileInt(_T("RegVars"), _T("DG_GRIPOBJLIMIT"), 100) );
    setDG_GRIPCOLOR( AfxGetApp()->GetProfileInt(_T("RegVars"), _T("DG_GRIPCOLOR"), 119) );
    setDG_GRIPHOVER( AfxGetApp()->GetProfileInt(_T("RegVars"), _T("DG_GRIPHOVER"), 4) );
    setDG_GRIPHOT( AfxGetApp()->GetProfileInt(_T("RegVars"), _T("DG_GRIPHOT"), 2) );

    //OdString sRsrc("C:\\Program Files\\Bentley\\MicroStation\\ustation.rsc");
    OdString sRsrc( AfxGetApp()->GetProfileString(_T("RegVars"), _T("MS_RSRC"), _T("")) );
    if ( !sRsrc.isEmpty() )
    {
      setMS_RSRC( sRsrc );
    }

    OdString strPenTable( AfxGetApp()->GetProfileString(_T("RegVars"), _T("MS_PENTABLE"), _T("")) );
    if ( !strPenTable.isEmpty() )
    {
      setMS_PENTABLE( strPenTable );
    }

    OdString strDefCtbl( AfxGetApp()->GetProfileString(_T("RegVars"), _T("MS_DEFCTBL"), _T("")) );
    if ( !strDefCtbl.isEmpty() )
    {
      setMS_DEFCTBL( strDefCtbl );
    }

    OdString sFastFont = AfxGetApp()->GetProfileString(_T("RegVars"), _T("FAST_FONT"), _T(""));
    if ( !sFastFont.isEmpty() )
    {
      setFAST_FONT( sFastFont );
    }

    m_bApplyGlobalOrigin = GetProfileInt(_T("options"), _T("ApplyGlobalOrigin"), 0) != 0;

#ifdef TDRAWING_ENABLED
    setDbHostAppServices(&dbSvcs);
    dbSvcs.setDgnHostAppServices(this);
#endif

    // Initialize Runtime Extension environment
    odrxInitialize(this);

    // Register ODA Drawings API for DGN
    ::odrxDynamicLinker()->loadModule(L"TG_Db", false);

#ifdef TDRAWING_ENABLED
    // Register ODA Drawings API for DGN
    ::odrxDynamicLinker()->loadModule(OdDbModuleName, false); // for instance, to draw .dwg file XRefs
    ::odrxDynamicLinker()->loadModule(L"TG_DwgDb", false);
    // Dgn level table overrides for dwg reference attachments support
    ::odrxDynamicLinker()->loadModule(L"ExDgnImportLineStyle");
#endif

    ::odrxDynamicLinker()->loadModule(L"ExDgnRecomputeAssocPoints");

    // Sample grip/snap points implementation
    ::odrxDynamicLinker()->loadModule(L"ExDgnGripPoints");

    ::odrxDynamicLinker()->loadModule(L"ExDgnCloning");

    ::odrxDynamicLinker()->loadModule(L"ExDgnDumper");
    ::odrxDynamicLinker()->loadModule(OdRasterExportModuleName);
    ::odrxDynamicLinker()->loadModule(L"ExDgnDatabaseRecovery");



    CLoadApps::rxInit();

    OdDgDatabaseDoc::rxInit();

    addSheetModelDrawingCustomization();

    OdEdCommandStackPtr pCommands = odedRegCmds();
    pCommands->addCommand(&g_Cmd_SELECT);
    pCommands->addCommand(&g_Cmd_COPY_LINE_STYLES_FROM_RSC);
    pCommands->addCommand(&g_Cmd_ERASE);
    pCommands->addCommand(&g_Cmd_CraeteDefaultInst);
	pCommands->addCommand(&g_Cmd_UNDO);
	pCommands->addCommand(&g_Cmd_LINE);
	pCommands->addCommand(&g_Cmd_ARC);
	pCommands->addCommand(&g_Cmd_CIRCLE);
	pCommands->addCommand(&g_Cmd_TESTUNDOPL);
	pCommands->addCommand(&g_Cmd_CONE);
	pCommands->addCommand(&g_Cmd_TEXT);
	pCommands->addCommand(&g_Cmd_ERASE_ALL_BY_TYPE);
    pCommands->addCommand(&g_Cmd_ISOLATE);
    pCommands->addCommand(&g_Cmd_ERASE_UNUSED_SHARED_CELLS);
    pCommands->addCommand(&g_Cmd_ZOOM);
    pCommands->addCommand(&g_Cmd_Fence);
    pCommands->addCommand(&g_Cmd_REDUCE);
    pCommands->addCommand(&g_Cmd_ERASEInvisible);
    pCommands->addCommand(&g_Cmd_CREATE_VIEW_GROUPS);
    pCommands->addCommand(&g_Cmd_ConvertToExtGraphics);
    pCommands->addCommand(&g_Cmd_ConvertToMesh);
    pCommands->addCommand(&g_Cmd_SetGlobalOrigin);
    pCommands->addCommand(&g_Cmd_Scale2);
    pCommands->addCommand(&g_Cmd_RotateXY30);
	rxInitMaterialsEditorObjects();
  }
  catch (OdError& err)
  {
    reportError(_T("ODA Drawings API for DGN Initialization error"), err);
    return FALSE;
  }
#ifndef _DEBUG
  catch (...)
  {
    OdError err(eNotHandled);
    reportError(_T("ODA Drawings API for DGN Initialization error"), err);
    return FALSE;
  }
#endif


  // InitCommonControlsEx() is required on Windows XP if an application
  // manifest specifies use of ComCtl32.dll version 6 or later to enable
  // visual styles.  Otherwise, any window creation will fail.
  INITCOMMONCONTROLSEX InitCtrls;
  InitCtrls.dwSize = sizeof(InitCtrls);
  // Set this to include all the common control classes you want to use
  // in your application.
  InitCtrls.dwICC = ICC_WIN95_CLASSES;
  InitCommonControlsEx(&InitCtrls);

  CWinApp::InitInstance();

  // Initialize OLE libraries
  if (!AfxOleInit())
  {
    AfxMessageBox(IDP_OLE_INIT_FAILED);
    return FALSE;
  }
  AfxEnableControlContainer();
  // Standard initialization
  // If you are not using these features and wish to reduce the size
  // of your final executable, you should remove from the following
  // the specific initialization routines you do not need

  LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views
  CMultiDocTemplate* pDocTemplate;
  pDocTemplate = new CMultiDocTemplate(IDR_ODADGNAPPTYPE,
    RUNTIME_CLASS(COdaDgnAppDoc),
    RUNTIME_CLASS(CChildFrame), // custom MDI child frame
    RUNTIME_CLASS(COdaDgnTreeView));
  if (!pDocTemplate)
    return FALSE;
  AddDocTemplate(pDocTemplate);

  // create main MDI Frame window
  CMainFrame* pMainFrame = new CMainFrame;
  if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
  {
    delete pMainFrame;
    return FALSE;
  }
  m_pMainWnd = pMainFrame;
  // call DragAcceptFiles only if there's a suffix
  //  In an MDI app, this should occur immediately after setting m_pMainWnd


  // Parse command line for standard shell commands, DDE, file open
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);
  if ( cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew ) 
    cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;


  // Dispatch commands specified on the command line.  Will return FALSE if
  // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;

  WINDOWPLACEMENT wp;
  BOOL bRes = pMainFrame->GetWindowPlacement(&wp);

  wp.flags                  = GetProfileInt(_T("MainFrame"), _T("flags"),   wp.flags                  );
  wp.showCmd                = GetProfileInt(_T("MainFrame"), _T("showCmd"), wp.showCmd                );
  wp.rcNormalPosition.left  = GetProfileInt(_T("MainFrame"), _T("left"),    wp.rcNormalPosition.left  );
  wp.rcNormalPosition.top   = GetProfileInt(_T("MainFrame"), _T("top"),     wp.rcNormalPosition.top   );
  wp.rcNormalPosition.bottom= GetProfileInt(_T("MainFrame"), _T("bottom"),  wp.rcNormalPosition.bottom);
  wp.rcNormalPosition.right = GetProfileInt(_T("MainFrame"), _T("right"),   wp.rcNormalPosition.right );

  wp.ptMinPosition.x        = GetProfileInt(_T("MainFrame"), _T("Min.x"),   wp.ptMinPosition.x        );
  wp.ptMinPosition.y        = GetProfileInt(_T("MainFrame"), _T("Min.y"),   wp.ptMinPosition.y        );

  wp.ptMaxPosition.x        = GetProfileInt(_T("MainFrame"), _T("Max.x"),   wp.ptMaxPosition.x        );
  wp.ptMaxPosition.y        = GetProfileInt(_T("MainFrame"), _T("Max.y"),   wp.ptMaxPosition.y        );

  bRes = pMainFrame->SetWindowPlacement(&wp);

  // The main window has been initialized, so show and update it
  pMainFrame->ShowWindow(m_nCmdShow);
  pMainFrame->UpdateWindow();

  return TRUE;
}

void COdaDgnApp::OnOptionsUseGsModel()
{
  m_bUseGsModel = !m_bUseGsModel;
  WriteProfileInt(_T("options"), _T("UseGsModel"), m_bUseGsModel);
}

void COdaDgnApp::OnOptionsUseBlockCache()
{
  m_bBlocksCache = !m_bBlocksCache;
  WriteProfileInt(_T("options"), _T("UseGsBlockCache"), m_bBlocksCache);
}

void COdaDgnApp::OnOptionsMTLoading()
{
  bool bMTLoading = getMtMode( OdDb::kMTLoading );
  WriteProfileInt(_T("options"), _T("UseMTLoading"), !bMTLoading);
  setMtMode( OdDb::kMTLoading, !bMTLoading);
}

void COdaDgnApp::OnUpdateOptionsMTLoading(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck( getMtMode(OdDb::kMTLoading) );
}

void COdaDgnApp::OnOptionsMTRegen()
{
  bool bMTRegen = getMtMode(OdDb::kMTRendering);
  WriteProfileInt(_T("options"), _T("UseMTRendering"), !bMTRegen);

  setMtMode(OdDb::kMTRendering, !bMTRegen);
}

void COdaDgnApp::OnUpdateOptionsMTRegen(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(getMtMode(OdDb::kMTRendering));
}

void COdaDgnApp::OnOptionsTransformLSScale()
{
  bool bTransformLSScale = !getTG_LSSCALE_TRANSFORM();
  setTG_LSSCALE_TRANSFORM(bTransformLSScale);
  WriteProfileInt(_T("options"), _T("TransformLSScale"), bTransformLSScale);
}

void COdaDgnApp::OnUpdateOptionsTransformLSScale(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(getTG_LSSCALE_TRANSFORM());
}

void COdaDgnApp::OnUpdateOptionsUseGsModel(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bUseGsModel);
}

void COdaDgnApp::OnUpdateOptionsUseBlockCache(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bBlocksCache);
}

void COdaDgnApp::OnOptionsActiveLevelAlwaysVisible()
{
  setDG_ACTIVELEVELALWAYSVISIBLE(!getDG_ACTIVELEVELALWAYSVISIBLE());
  WriteProfileInt(_T("RegVars"), _T("DG_ACTIVELEVELALWAYSVISIBLE"), getDG_ACTIVELEVELALWAYSVISIBLE());
}

void COdaDgnApp::OnUpdateOptionsActiveLevelAlwaysVisible(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(getDG_ACTIVELEVELALWAYSVISIBLE());
}

void COdaDgnApp::OnOptionsExactExtents()
{
  setDG_EXACT_EXTENTS(!getDG_EXACT_EXTENTS());
  WriteProfileInt(_T("RegVars"), _T("DG_EXACT_EXTENTS"), getDG_EXACT_EXTENTS());
}

void COdaDgnApp::OnUpdateOptionsExactExtents(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(getDG_EXACT_EXTENTS());
}

void COdaDgnApp::OnOptionsProcessTagAssociation()
{
  setDG_PROCESSTAGASSOC(!getDG_PROCESSTAGASSOC());
  WriteProfileInt(_T("RegVars"), _T("DG_PROCESSTAGASSOC"), getDG_PROCESSTAGASSOC());
}

void COdaDgnApp::OnUpdateOptionsProcessTagAssociation(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(getDG_PROCESSTAGASSOC());
}

void COdaDgnApp::OnOptionsProcessDimAssociation()
{
  setDG_PROCESSDIMASSOC(!getDG_PROCESSDIMASSOC());
  WriteProfileInt(_T("RegVars"), _T("DG_PROCESSDIMASSOC"), getDG_PROCESSDIMASSOC());
}

void COdaDgnApp::OnUpdateOptionsProcessDimAssociation(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(getDG_PROCESSDIMASSOC());
}

void COdaDgnApp::OnOptionsProcessMLineAssociation()
{
  setDG_PROCESSMLINEASSOC(!getDG_PROCESSMLINEASSOC());
  WriteProfileInt(_T("RegVars"), _T("DG_PROCESSMLINEASSOC"), getDG_PROCESSMLINEASSOC());
}

void COdaDgnApp::OnUpdateOptionsProcessMLineAssociation(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(getDG_PROCESSMLINEASSOC());
}

void COdaDgnApp::OnOptionsUseSingleViewMode()
{
	m_bUseSingleViewMode = !m_bUseSingleViewMode;
	WriteProfileInt(_T("options"), _T("UseSingleViewMode"), m_bUseSingleViewMode);
}

void COdaDgnApp::OnUpdateOptionsUseSingleViewMode(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_bUseSingleViewMode);
}

void COdaDgnApp::OnOptions3DObjWireframeDrawIsolines()
{
  setMS_3DOBJ_WF_ISOLINES(!getMS_3DOBJ_WF_ISOLINES());
  WriteProfileInt(_T("RegVars"), _T("MS_3DOBJ_WF_ISOLINES"), getMS_3DOBJ_WF_ISOLINES());
}

void COdaDgnApp::OnUpdateOptions3DObjWireframeDrawIsolines(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(getMS_3DOBJ_WF_ISOLINES());
}

void COdaDgnApp::OnOptionsUseXRefColorTable()
{
  setTG_USE_XREFCTBL(!getTG_USE_XREFCTBL());
  WriteProfileInt(_T("RegVars"), _T("TG_USE_XREFCTBL"), getTG_USE_XREFCTBL());
}

void COdaDgnApp::OnUpdateOptionsUseXRefColorTable(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(getTG_USE_XREFCTBL());
}

void COdaDgnApp::OnOptionsRemapColorIndexes()
{
  setTG_REMAP_CLRIDX(!getTG_REMAP_CLRIDX());
  WriteProfileInt(_T("RegVars"), _T("TG_REMAP_CLRIDX"), getTG_REMAP_CLRIDX());
}

void COdaDgnApp::OnUpdateOptionsRemapColorIndexes(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(getTG_REMAP_CLRIDX());
}

#ifdef _DEBUG

void COdaDgnApp::OnDisableAsserts()
{
  m_bDisableAsserts = !m_bDisableAsserts;

  if( m_bDisableAsserts )
  {
    ::odSetAssertFunc(myAssertOff);
  }
  else
  {
    ::odSetAssertFunc(myAssert);
  }
}

void COdaDgnApp::OnUpdateDisableAsserts(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bDisableAsserts);
}

#endif

int COdaDgnApp::ExitInstance()
{
  try
  {
    OdEdCommandStackPtr pCommands = odedRegCmds();
    pCommands->removeCmd(&g_Cmd_SELECT);
    pCommands->removeCmd(&g_Cmd_COPY_LINE_STYLES_FROM_RSC);
    pCommands->removeCmd(&g_Cmd_ERASE);
    pCommands->removeCmd(&g_Cmd_CraeteDefaultInst);
	pCommands->removeCmd(&g_Cmd_LINE);
	pCommands->removeCmd(&g_Cmd_ARC);
	pCommands->removeCmd(&g_Cmd_CIRCLE);
	pCommands->removeCmd(&g_Cmd_TESTUNDOPL);
	pCommands->removeCmd(&g_Cmd_CONE);
	pCommands->removeCmd(&g_Cmd_TEXT);
	pCommands->removeCmd(&g_Cmd_UNDO);
    pCommands->removeCmd(&g_Cmd_ZOOM);
    pCommands->removeCmd(&g_Cmd_ERASEInvisible);
    pCommands->removeCmd(&g_Cmd_REDUCE);
    pCommands->removeCmd(&g_Cmd_ConvertToExtGraphics);
    pCommands->removeCmd(&g_Cmd_ConvertToMesh);
    pCommands->removeCmd(&g_Cmd_SetGlobalOrigin);

    settings().save(this);

    OdDgDatabaseDoc::rxUninit();

    CLoadApps::rxUninit();

	rxUninitMaterialsEditorObjects();

    removeSheetModelDrawingCustomization();

    ::odrxUninitialize();
  }
  catch(const OdError& e)
  {
    theApp.reportError(_T(""), e);
  }
  return CWinApp::ExitInstance();
}

OdDgDatabasePtr COdaDgnApp::openFile(LPCTSTR lpszPathName)
{
  // Open an existing document
  OdDgDatabasePtr pDb;
  CMainFrame* pMFrame = (CMainFrame*)GetMainWnd();
  try
  {
#ifdef DGNEXPORT_ENABLE
    OdString path(lpszPathName);

    if(path.right(4).makeUpper().iCompare(L".DWG") == 0 || path.right(4).makeUpper().iCompare(L".DXF") == 0)
    {
      OdDgnExportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdDgnExportModuleName, false);
      OdDgnExportPtr pExporter = pModule->create();

      pExporter->properties()->putAt(L"DgnServices", static_cast<OdDgHostAppServices*>(this));
      pExporter->properties()->putAt(L"DwgPath", OdRxVariantValue(path));

      pMFrame->StartTimer();

      OdDgnExport::ExportResult res = pExporter->exportDb();

      if( res == OdDgnExport::success )
        pDb = pExporter->properties()->getAt(L"DgnDatabase");
      else
      {
        switch (res)
        {
          case OdDgnExport::bad_database:
            messageBox(_T("DGN export"), _T("Bad database"), MB_OK | MB_ICONERROR);
          break;

          case OdDgnExport::bad_file:
            messageBox(_T("DGN export"), _T("Bad file"), MB_OK | MB_ICONERROR);
          break;

          case OdDgnExport::encrypted_file:
          case OdDgnExport::bad_password:
            messageBox(_T("DGN export"), _T("The file is encrypted"), MB_OK | MB_ICONERROR);
          break;

          case OdDgnExport::fail:
            messageBox(_T("DGN export"), _T("Unknown import error"), MB_OK | MB_ICONERROR);
          break;
        }
      }
      pMFrame->StopTimer(_T("Loading"));
    }
    else
#endif
    {
      pMFrame->StartTimer();
      CWaitCursor wait;
      m_bLoading = true;
      pDb = readFile(OdString(lpszPathName));
      m_bLoading = false;

      if (m_bApplyGlobalOrigin && !pDb.isNull())
      {
        OdDgModelTablePtr pModelTable = pDb->getModelTable(OdDg::kForWrite);
        OdDgElementIteratorPtr pIter = pModelTable->createIterator();

        for (; !pIter->done(); pIter->step())
        {
          OdDgModelPtr pModel = pIter->item().openObject(OdDg::kForWrite);
          pModel->enableGlobalOriginUsage(true);
        }
      }

      pMFrame->StopTimer(_T("Loading"));
    }
  }
  catch(const OdError& e)
  {
    setStatusText(0, _T(""));
    pDb = 0;
    reportError(_T("Loading Error..."), e);
  }
  catch(const COdaDgnAppUserBreak&)
  {
    setStatusText(0, _T(""));
    setStatusText(_T("Operation was canceled by user."));
  }
  m_bLoading = false;

  return pDb;
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();

// Dialog Data
  enum { IDD = IDD_ABOUTBOX };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();

// Implementation
protected:
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

BOOL CAboutDlg::OnInitDialog()
{
  OdDbLibraryInfo*  libInfo = oddgGetLibraryInfo();
  //  OdString  sCompany = libInfo->getCompanyName();
  OdString  sLib = libInfo->getBuildComments();
  OdString  sCopyright = libInfo->getCopyright();

  CDialog::OnInitDialog();

  SetDlgItemText(IDC_INF_COPYRIGHT, sCopyright);
  //  SetDlgItemText(IDC_INF_COMPANY, sCompany.c_str());
  SetDlgItemText(IDC_INF_LIBRARY, sLib);

  return TRUE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void COdaDgnApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}


// COdaDgnApp message handlers


void COdaDgnApp::OnFileOpen()
{
  CString strFormat = _T("DGN files (*.dgn)|*.dgn|Cell Library files (*.cel)|*.cel|Dgn Library files (*.dgnlib)|*.dgnlib|Redline files (*.rdl)|*.rdl|");

#ifdef DGNEXPORT_ENABLE
  if (!::odrxDynamicLinker()->loadApp(OdDgnExportModuleName, true).isNull())
    strFormat += _T("DWG files (*.dwg)|*.dwg|DXF files (*.dxf)|*.dxf|");
#endif

  strFormat += _T("All Files (*.*)|*.*||");

  CString sPath = BrowseWithPreview(GetMainWnd()->GetSafeHwnd(), strFormat);

  if ( sPath.GetLength() )
  {
    OpenDocumentFile(sPath);
  }
}

#if !defined(TD_DLL_VERSION_SUFFIX_STR)
#define TD_DLL_VERSION_SUFFIX_STR L""
#endif

CString COdaDgnApp::BrowseWithPreview(HWND hWndParent, LPCTSTR caFilter)
{
  CString sFileBuff;

  HINSTANCE hinstLib = LoadLibrary(OdString(_T("FileDlgExt")) 
#ifndef _UNICODE
    + "Ansi"
#endif
    + OdString(TD_DLL_VERSION_SUFFIX_STR) + OdString(_T(".dll")));
  if (NULL != hinstLib)
  {
    ODA_OPEN_DLGPROC fpDlgProc = (ODA_OPEN_DLGPROC)GetProcAddress(hinstLib, "CreateOpenWithPreviewDlg");
    if (NULL != fpDlgProc)
    {
      CDgPreviewDIB statDib;
      OpenWithPreviewDlg *pDlg;
      (fpDlgProc)(&statDib, hWndParent, NULL, caFilter,
        OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR |OFN_PATHMUSTEXIST,
        &pDlg);
      if (IDOK == pDlg->ShowModal())
      {
        long nSize = _MAX_PATH;

        pDlg->GetFullFileName(sFileBuff.GetBuffer(nSize), nSize);
        sFileBuff.ReleaseBuffer();
      }
      pDlg->ReleaseDlg();
    }
    FreeLibrary(hinstLib);
  }
  else
  {    // use alternate method for OpenFile dialog
    OPENFILENAME  ofn;

    // replace the '|' character with '\0'
    CString filter;
    long l = (long)_tcslen(caFilter), l2 = l;
    LPTSTR buff = filter.GetBuffer(l);
    while (--l >= 0)
    {
      if (caFilter[l] == '|')
        buff[l] = '\0';
      else
        buff[l] = caFilter[l];
    }
    filter.ReleaseBuffer(l2);

    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWndParent;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = sFileBuff.GetBuffer(_MAX_PATH);
    ofn.nMaxFile = _MAX_PATH;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR |OFN_PATHMUSTEXIST;

    GetOpenFileName(&ofn);
    sFileBuff.ReleaseBuffer();
  }
  return sFileBuff;
}

void COdaDgnApp::OnToolsSolidHelper()
{
  CSHelperSheet appDlg(_T("Solid Helper"));
  appDlg.DoModal();
}

void COdaDgnApp::OnToolsSetRegVarsMsSymbRsrc()
{
  CSetRegVarsMsSymbRsrc appDlg;
  appDlg.setHostAppServices(this);
  appDlg.DoModal();
}

void COdaDgnApp::OnToolsSetRegVarsMsRsrc()
{
  CSetRegVarsMsRsrc appDlg;
  appDlg.setHostAppServices(this);
  appDlg.DoModal();
}

void COdaDgnApp::OnToolsSetRegVarsMsPenTable()
{
  CSetRegVarsMsRsrc appDlg( NULL, CSetRegVarsMsRsrc::kMsPenTable);
  appDlg.setHostAppServices(this);
  appDlg.DoModal();
}

void COdaDgnApp::OnToolsSetRegVarsMsDefCtbl()
{
  CSetRegVarsMsRsrc appDlg( NULL, CSetRegVarsMsRsrc::kMsDefCtbl);
  appDlg.setHostAppServices(this);
  appDlg.DoModal();
}

void COdaDgnApp::OnToolsSetRegVarsMsFontPath()
{
  CSetRegVarsMsTexDir appDlg(NULL, CSetRegVarsMsTexDir::kFontPath);
  appDlg.setHostAppServices(this);
  appDlg.DoModal();
}

void COdaDgnApp::OnToolsSetRegVarsFastFont()
{
  CSetRegVarsFastFont appDlg;
  appDlg.setHostAppServices(this);
  appDlg.DoModal();
}

void COdaDgnApp::OnToolsSetRegVarsNurbs()
{
  CSetRegVarsNurbs appDlg(NULL);
  appDlg.setHostAppServices(this);
  appDlg.DoModal();
}

void COdaDgnApp::OnToolsSetRegVarsGripPoints()
{
  CSetRegVarsGripPoints appDlg(NULL);
  appDlg.setHostAppServices(this);
  appDlg.DoModal();
}

void COdaDgnApp::OnToolsSetRegVarsMsMatPalette()
{
  CSetRegVarsMsSymbRsrc appDlg( NULL, CSetRegVarsMsSymbRsrc::kMatPalette );
  appDlg.setHostAppServices(this);
  appDlg.DoModal();
}

void COdaDgnApp::OnToolsSetRegVarsMsPattern()
{
  CSetRegVarsMsTexDir appDlg(NULL, CSetRegVarsMsTexDir::kPattern);
  appDlg.setHostAppServices(this);
  appDlg.DoModal();
}

void COdaDgnApp::OnToolsSetRegVarsMsBump()
{
  CSetRegVarsMsTexDir appDlg(NULL, CSetRegVarsMsTexDir::kBump);
  appDlg.setHostAppServices(this);
  appDlg.DoModal();
}

void COdaDgnApp::OnToolsSetRegVarsMsMaterial()
{
  CSetRegVarsMsTexDir appDlg(NULL, CSetRegVarsMsTexDir::kMaterial);
  appDlg.setHostAppServices(this);
  appDlg.DoModal();
}

void COdaDgnApp::OnToolsSetRegVarsMsMtbl()
{
  CSetRegVarsMsTexDir appDlg(NULL, CSetRegVarsMsTexDir::kMtbl);
  appDlg.setHostAppServices(this);
  appDlg.DoModal();
}

void COdaDgnApp::OnToolsModelerOptions()
{
  OdRxModulePtr pModule = ::odrxDynamicLinker()->loadModule("TG_ModelerGeometry");
  DgModelerModulePtr DgModule = DgModelerModule::cast(pModule);
  if (!DgModule.isNull())
  {
    CModelerOptions appDlg;

    wrTriangulationParams currentParams;
    DgModule->getTriangulationParams(currentParams);

    appDlg.m_dDegree = currentParams.normalTolerance;
    appDlg.m_nKnots= currentParams.BetweenKnots;

    if ( appDlg.DoModal() == IDOK )
    {
      currentParams.normalTolerance = appDlg.m_dDegree;
      currentParams.BetweenKnots = (OdUInt16)appDlg.m_nKnots;

      DgModule->setTriangulationParams(currentParams);
    }
  }
}

void COdaDgnApp::OnToolsLoadapplications()
{
  CLoadApps appDlg;
  appDlg.DoModal();
}

CMenu* getVectorizeMenu2(CMenu* pMainMenu) 
{
  unsigned menuPos = 4;
  MENUITEMINFO menuItemInfo;
  menuItemInfo.cbSize = sizeof(menuItemInfo);
  menuItemInfo.fMask = MIIM_TYPE;
  menuItemInfo.fType = MFT_STRING;
  menuItemInfo.dwTypeData = 0;
  pMainMenu->GetMenuItemInfo(menuPos, &menuItemInfo, TRUE);
  if ( menuItemInfo.cch != odStrLenA("Vectorize") )
    menuPos = 5;
  return pMainMenu->GetSubMenu(menuPos);
}

bool AddGsMenuItem(CMenu* pVectorizeMenu, DWORD& nIndex, LPCTSTR szPath) 
{
  if(ID_VECTORIZER_FIRST + nIndex <= ID_VECTORIZER_LAST)
  {
    MENUITEMINFO menuItemInfo;
    menuItemInfo.cbSize = sizeof(menuItemInfo);
    menuItemInfo.fMask = MIIM_DATA;

    pVectorizeMenu->InsertMenu(nIndex, MF_BYPOSITION, ID_VECTORIZER_FIRST + nIndex, szPath);

    menuItemInfo.dwItemData = theApp.getGSMenuItemMarker();
    VERIFY(::SetMenuItemInfo(pVectorizeMenu->m_hMenu, nIndex, TRUE, &menuItemInfo));

    pVectorizeMenu->EnableMenuItem(nIndex, MF_BYPOSITION|MF_ENABLED);
    if ( theApp.settings().getGsDevicePath().iCompare( OdString(szPath) ) == 0 )
    {
      pVectorizeMenu->CheckMenuItem(nIndex, MF_BYPOSITION|MF_CHECKED);
    }
    else
    {
      pVectorizeMenu->CheckMenuItem(nIndex, MF_BYPOSITION|MF_UNCHECKED);
    }
    ++nIndex;
    return true;
  }
  return false;
}

CString COdaDgnApp::getApplicationPath()
{
  TCHAR path[MAX_PATH];
  ::GetModuleFileName(::GetModuleHandle(0), path, MAX_PATH);
  CString s_path(path);
  int nLen = s_path.ReverseFind('\\');
  s_path = s_path.Left(nLen);
  return s_path;
}

void COdaDgnApp::OnVectorizeAddvectorizerdll()
{
  CMenu* pVectorizeMenu = getVectorizeMenu2(GetMainWnd()->GetMenu());

#ifdef _TOOLKIT_IN_DLL_
  CFileDialog dlg( TRUE, _T("gs"), _T(""),
    OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST,
    _T("Graphic System DLL (*.txv)|*.txv|Windows DLL (*.dll)|*.dll||"), ::AfxGetMainWnd() );

  dlg.m_ofn.lpstrTitle = _T("Select Graphic System DLL");

  CString s_path = getApplicationPath();
  dlg.m_ofn.lpstrInitialDir = s_path.GetBuffer(s_path.GetLength());

#else
  CStaticAppSelDlg dlg(::AfxGetMainWnd());
#endif //#ifdef _TOOLKIT_IN_DLL_

  if ( dlg.DoModal() == IDOK )
  {
    OdString path(dlg.GetFileName());
    DWORD nIndex = settings().getNumGSMenuItems();

    try
    {
      if( !path.isEmpty() )
        ::odrxDynamicLinker()->loadModule(path, false);
    }
    catch(...)
    {
      AfxMessageBox(L"Wrong version of rendering device library.", MB_OK);
      return;
    }

    if( !path.isEmpty() )
    {
      settings().setGsDevicePath( path );
    }

    AddGsMenuItem(pVectorizeMenu, nIndex, path );
    settings().setNumGSMenuItems(nIndex);
    WriteProfileString(_T("options\\vectorizers"), settings().getGsDevicePath(), _T(""));
    GetMainWnd()->SendMessage(WM_COMMAND, ID_VECTORIZE);
  }
}

void COdaDgnApp::OnUpdateVectorizeAddvectorizerdll(CCmdUI *pCmdUI)
{
  if ( settings().getNumGSMenuItems() == 0 )
  {
    CMenu* pVectorizeMenu = getVectorizeMenu2(GetMainWnd()->GetMenu());
	dgn::CRegKey rk;
    rk.Create(HKEY_CURRENT_USER, _T("Software\\") AppRegistryKey _T("\\OdaDgnApp\\options\\vectorizers"));

    CString path;
    DWORD pathSize;
    for(;;)
    {
      pathSize = _MAX_FNAME + _MAX_EXT;
      DWORD err = ::RegEnumValue(rk, settings().getNumGSMenuItems(),
        path.GetBuffer(pathSize), &pathSize, NULL, NULL, NULL, NULL);
      path.ReleaseBuffer();
      if ( err == eOk )
      {
        DWORD nIndex = settings().getNumGSMenuItems();
        if ( settings().getGsDevicePath().isEmpty() )
        {
          OdString sPath = path;
          settings().setGsDevicePath( OdString(sPath) );
        }
        if ( AddGsMenuItem(pVectorizeMenu, nIndex, path) )
        {
          settings().setNumGSMenuItems(nIndex);
        }
        else
        {
          break;
        }
      }
      else
        break;
    }
  }
}

void COdaDgnApp::OnVectorizeClearmenu()
{
  CMenu* pVectorizeMenu = getVectorizeMenu2(GetMainWnd()->GetMenu());
  while ( pVectorizeMenu->GetMenuItemCount() > 3 )
  {
    pVectorizeMenu->RemoveMenu(0, MF_BYPOSITION);
  }
  dgn::CRegKey rk;
  rk.Create(HKEY_CURRENT_USER, _T("Software\\") AppRegistryKey _T("\\OdaDgnApp\\options"));
  rk.RecurseDeleteKey(_T("vectorizers"));
  settings().setGsDevicePath(OdString::kEmpty);
  settings().setNumGSMenuItems(0);
}

void COdaDgnApp::OnUpdateGsDevices(CCmdUI *pCmdUI)
{
  CString str;
  pCmdUI->Enable( TRUE );

  if (pCmdUI->m_pMenu)
  {
    pCmdUI->m_pMenu->GetMenuString(pCmdUI->m_nIndex, str, MF_BYPOSITION);
    BOOL b = str.CompareNoCase(settings().getGsDevicePath().c_str()) == 0;
    pCmdUI->SetCheck(b);
  }
}

void COdaDgnApp::OnUpdateVectorizeClearmenu(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(settings().getNumGSMenuItems() > 0);
}

void COdaDgnApp::OnUpdateVectorize(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!settings().getGsDevicePath().isEmpty());
}

const ODCOLORREF* COdaDgnApp::curPalette() const
{
  return odcmAcadPalette(settings().getActiveBackground());
}

void COdaDgnApp::setStatusText(LPCTSTR msg)
{
  ((CMainFrame*)GetMainWnd())->setStatusText(msg);
}

void COdaDgnApp::setStatusText(int nCol, LPCTSTR msg)
{
  ((CMainFrame*)GetMainWnd())->setStatusText(nCol, msg);
}

void COdaDgnApp::warning(const OdString& sm)
{
  if ( m_bLoading )
  {
    OdString sMsg(sm);
    sMsg += _T("\n\nDo you want to proceed ?");
    if (MessageBox (NULL, sMsg, _T("Warning!"), MB_ICONWARNING|MB_YESNO) == IDNO )
      throw COdaDgnAppUserBreak();
  }
}

OdString COdaDgnApp::fileDialog(int nFlags, const OdString& prompt, 
                                   const OdString& defExt, const OdString& fileName, const OdString& filter)
{
  DWORD dlgFlags = OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST;
  if (GETBIT(nFlags, OdEd::kGfpOverwritePrompt))
    dlgFlags |= OFN_OVERWRITEPROMPT;

  CFileDialog dlg( !GETBIT(nFlags, OdEd::kGfpForSave), defExt, fileName, dlgFlags, filter, ::AfxGetMainWnd() );

  dlg.m_ofn.lpstrTitle = prompt;

  if ( dlg.DoModal() == IDOK )
    return OdString((LPCTSTR)dlg.GetPathName());

  throw OdEdCancel();
}


/////////////////////////////////////////////////////////////////////////////
// Add 'commands' to Menu

const int nCommandMenuIndex = 8;

CMenu* COdaDgnApp::CommandMenu(CMenu** ppEditMenu)
{
  // Locate the edit submenu
  CMenu* pEditMenu = NULL;
  CMenu* pTopMenu = GetMainWnd()->GetMenu();
  int n = pTopMenu->GetMenuItemCount();
  for (int i = 0; i < n; ++i)
  {
    CMenu* pMenu = pTopMenu->GetSubMenu(i);
    if (pMenu && pMenu->GetMenuItemID(0) == ID_EDIT_UNDO)
    {
      pEditMenu = pMenu;
      break;
    }
  }
  ASSERT(pEditMenu != NULL);
  if(ppEditMenu)
  {
    *ppEditMenu = pEditMenu;
  }

  CMenu* pCommandMenu = pEditMenu->GetSubMenu(nCommandMenuIndex);

  ASSERT(pCommandMenu != NULL);

  return pCommandMenu;
}

void COdaDgnApp::RefreshCommandMenu()
{
  MENUITEMINFO menuItemInfo;
  menuItemInfo.cbSize = sizeof(menuItemInfo);
  menuItemInfo.fMask = MIIM_DATA;

  CMenu* pEditMenu = NULL;
  CMenu* pCommandMenu = CommandMenu(&pEditMenu);

  int i;
  for(i = pCommandMenu->GetMenuItemCount()-1; i >= 0; i--)
  {
    CMenu* pSubMenu = pCommandMenu->GetSubMenu(i);
    if(pSubMenu)
    {
      pSubMenu->DestroyMenu();
    }
    pCommandMenu->DeleteMenu(i, MF_BYPOSITION);
  }

  // Then add an item for each available command
  OdEdCommandStackPtr pCommandStack = ::odedRegCmds();
  bool bHasNoCommand = pCommandStack->newIterator()->done();
  pEditMenu->EnableMenuItem(nCommandMenuIndex, MF_BYPOSITION|(bHasNoCommand ? MF_GRAYED : MF_ENABLED));
  i = _APS_NEXT_COMMAND_VALUE + 100; // leave 100 ids for scales
  if(!bHasNoCommand)
  {
    OdRxIteratorPtr pGroups = pCommandStack->newGroupIterator();
    while(!pGroups->done())
    {
      OdRxDictionaryPtr pGroup = pGroups->object();
      CMenu groupMenu;
      groupMenu.CreateMenu();
      OdRxIteratorPtr pCommandIter = pGroup->newIterator(OdRx::kDictSorted);
      OdString groupName;
      while(!pCommandIter->done())
      {
        OdEdCommandPtr pCmd = pCommandIter->object().get();
        if (groupName.isEmpty())
        {
          groupName = pCmd->groupName();
        }
        groupMenu.AppendMenu(MF_STRING, i, OdString(pCmd->globalName()));
        menuItemInfo.dwItemData = (LPARAM)pCmd.get();
        VERIFY(::SetMenuItemInfo(groupMenu.m_hMenu, i, FALSE, &menuItemInfo));

        pCommandIter->next();
        ++i;
      }
      pCommandMenu->AppendMenu(MF_STRING|MF_POPUP, (LPARAM)groupMenu.Detach(), groupName);
      pGroups->next();
      groupName.empty();
    }
  }
  m_numCustomCommands = i - _APS_NEXT_COMMAND_VALUE - 100;
}

void COdaDgnApp::setRecentCmd(const OdString& cmd)
{
  if ( !cmd.isEmpty() && cmd != m_settings.getRecentCmd() )
  {
    m_settings.setRecentCmd( cmd );
  }
}

/************************************************************************/
/* Returns the static OdRxClass description object associated with the  */
/* database                                                             */
/************************************************************************/
OdRxClass* COdaDgnApp::databaseClass() const
{
  return OdDgDatabaseDoc::desc();
}

OdDbUndoControllerPtr COdaDgnApp::newUndoController()
{
  if (undoType())
  {
    ExFileUndoControllerPtr pController = OdRxObjectImpl<ExFileUndoController>::createObject();
    pController->setStorage(newUndoStream());
    return pController;
  }

  return OdRxObjectImpl<ExUndoController>::createObject();

}

OdStreamBufPtr COdaDgnApp::newUndoStream()
{
  // OdMemFileStreamImpl = mix of memory and file streams
  return OdRxObjectImpl<OdMemFileStreamImpl<OdStreamBuf> >::createObject();
}

bool COdaDgnApp::setUndoType(bool bUseTempFiles)
{ 
  bool oldType = m_bUseTempFiles;
  m_bUseTempFiles = bUseTempFiles;
  return oldType;
}

/////////////////////////////////////////////////////////////////////////////
#ifdef _MT
#ifndef _DLL

// For MT configurations only
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
extern "C" {

  ALLOCDLL_EXPORT void* odrxAlloc(size_t s)
  {
    return ::malloc(s);
  }

  ALLOCDLL_EXPORT void* odrxRealloc(void* p, size_t new_size, size_t /*old_size*/)
  {
    return ::realloc(p, new_size);
  }

  ALLOCDLL_EXPORT void odrxFree(void* p) 
  {
    ::free(p);
  }

} // extern "C"
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#endif
#endif
