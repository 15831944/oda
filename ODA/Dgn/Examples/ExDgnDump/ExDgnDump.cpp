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
/* This console application make dump of a DGN file.                    */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/* ExDgnDump <source filename> <destination dump filename>              */
/*                                                                      */
/************************************************************************/

#include "OdaCommon.h"
#include "ExDgnDumpElements.h"
#include "StaticRxObject.h"
#include "ExDgnServices.h"
#include "ExDgnHostAppServices.h"
#include "DgDatabase.h"
#include "DynamicLinker.h"
#include "RxInit.h"
#include "RxObjectImpl.h"
#include "RxDynamicModule.h"
#include "diagnostics.h"
#include "DgLevelFilterTableRecord.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a

// Auxiliary function to display different messages
//void oddgPrintConsoleString(const wchar_t* fmt, ...);

/************************************************************************/
/* Define a Custom Services class.                                      */
/*                                                                      */
/* Combines the platform dependent functionality of                     */
/* OdExDgnSystemServices and OdExDgnHostAppServices                     */ 
/************************************************************************/
class MyServices : public OdExDgnSystemServices, public OdExDgnHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdExDgnSystemServices);
};

/************************************************************************/
/* User defined function to write a message                             */
/* to the console for each ODA_ASSERT                                   */
/************************************************************************/
static void customAssertFunc(const char* expr, const char* fileName, int nLine)
{
  printf( 
    "Assertion has occurs:\n"
    " Expression: %s\n"
    " Filename: %s\n"
    " Line number: %d\n",
    expr, fileName, nLine );
}


#ifndef _TOOLKIT_IN_DLL_
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);
ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db", OdDgnModule)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Dgn7IO", OdDgn7IOModuleImpl)
ODRX_END_STATIC_MODULE_MAP()
#endif

#if defined(OD_USE_WMAIN)
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
  ::odSetAssertFunc( customAssertFunc );

  //parse commands
  if( argc != 3 )
  {
    oddgPrintConsoleString(L"\nExDgnDump sample program. Copyright (c) 2019, Open Design Alliance\n");
    oddgPrintConsoleString(L"\tusage: ExDgnDump <source file> <destination file>\n");
    oddgPrintConsoleString(L"\nPress ENTER to continue...\n");
    STD(cin).get();
    return 1;
  }

  OdString szSource = argv[1], szDump = argv[2]; 

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

  DumpStream = fopen( szDump, "w" );
  if( !DumpStream )
  {
    oddgPrintConsoleString( L"\nThe file %ls has not been opened\n", szDump.c_str() );
    return 1;
  }

  //write some title
  fwprintf( DumpStream, L"Dump of the file %s\n", argv[ 1 ] );

  //load the pDb
  OdDgDatabasePtr pDb;
 
  //create some service
  OdStaticRxObject< MyServices > svcs;

    /**********************************************************************/
    /* Initialize Runtime Extension environment                           */
    /**********************************************************************/
  odrxInitialize( &svcs );

  try
  {
    /**********************************************************************/
    /* Initialize ODA Drawings API for DGN                                */
    /**********************************************************************/
    ::odrxDynamicLinker()->loadModule(L"TG_Db", false);

    OdStringArray arrayRscList;
    //OdString sRscPath( L"font.rsc" );
    OdString sRscPath("C:\\Documents and Settings\\All Users\\Application Data\\Bentley\\WorkSpace\\System\\Symb\\font.rsc");
    arrayRscList.append( sRscPath );
    svcs.setMS_SYMBRSRC( arrayRscList );

    pDb = svcs.readFile( szSource );

    OdDgDumper dumper;
    dumper.rootDump( pDb.get() );

    //finalize the process
    pDb = 0;
    fclose( DumpStream );
  }
  catch (OdError& e)
  {
    oddgPrintConsoleString(L"\nODA Drawings API for DGN Error: %ls\n", e.description().c_str());
  }
  catch (...)
  {
    oddgPrintConsoleString(L"\nUnknown Error.\n");
  }

  /**********************************************************************/
  /* Uninitialize Runtime Extension environment                         */
  /**********************************************************************/
  ::odrxUninitialize();

  return 0;
}


//
// OdDgDumper
//


OdDgDumper::OdDgDumper()
{
  OdRxObject_Dumper::rxInit();

  OdDgDatabase::desc()->addX( OdRxObject_Dumper::desc(), &m_databaseDumper );
  OdDgModel::desc()->addX( OdRxObject_Dumper::desc(), &m_modelDumper );
  OdDgLine2d::desc()->addX( OdRxObject_Dumper::desc(), &m_line2dDumper );
  OdDgLine3d::desc()->addX( OdRxObject_Dumper::desc(), &m_line3dDumper );
  OdDgLineString2d::desc()->addX( OdRxObject_Dumper::desc(), &m_lineString2dDumper );
  OdDgLineString3d::desc()->addX( OdRxObject_Dumper::desc(), &m_lineString3dDumper );
  OdDgText2d::desc()->addX( OdRxObject_Dumper::desc(), &m_text2dDumper );
  OdDgText3d::desc()->addX( OdRxObject_Dumper::desc(), &m_text3dDumper );
  OdDgTextNode2d::desc()->addX( OdRxObject_Dumper::desc(), &m_textNode2dDumper );
  OdDgTextNode3d::desc()->addX( OdRxObject_Dumper::desc(), &m_textNode3dDumper );
  OdDgShape2d::desc()->addX( OdRxObject_Dumper::desc(), &m_shape2dDumper );
  OdDgShape3d::desc()->addX( OdRxObject_Dumper::desc(), &m_shape3dDumper );
  OdDgCurve2d::desc()->addX( OdRxObject_Dumper::desc(), &m_curve2dDumper );
  OdDgCurve3d::desc()->addX( OdRxObject_Dumper::desc(), &m_curve3dDumper );
  OdDgEllipse2d::desc()->addX( OdRxObject_Dumper::desc(), &m_ellipse2dDumper );
  OdDgEllipse3d::desc()->addX( OdRxObject_Dumper::desc(), &m_ellipse3dDumper );
  OdDgArc2d::desc()->addX( OdRxObject_Dumper::desc(), &m_arc2dDumper );
  OdDgArc3d::desc()->addX( OdRxObject_Dumper::desc(), &m_arc3dDumper );
  OdDgCone::desc()->addX( OdRxObject_Dumper::desc(), &m_coneDumper );
  OdDgComplexString::desc()->addX( OdRxObject_Dumper::desc(), &m_complexStringDumper );
  OdDgComplexShape::desc()->addX( OdRxObject_Dumper::desc(), &m_complexShapeDumper );
  OdDgPointString2d::desc()->addX( OdRxObject_Dumper::desc(), &m_pointString2dDumper );
  OdDgPointString3d::desc()->addX( OdRxObject_Dumper::desc(), &m_pointString3dDumper );
  OdDgDimension::desc()->addX( OdRxObject_Dumper::desc(), &m_dimensionDumper );
  OdDgMultiline::desc()->addX( OdRxObject_Dumper::desc(), &m_multilineDumper );
  OdDgBSplineCurve2d::desc()->addX( OdRxObject_Dumper::desc(), &m_bSplineCurve2dDumper );
  OdDgBSplineCurve3d::desc()->addX( OdRxObject_Dumper::desc(), &m_bSplineCurve3dDumper );
  OdDgSurface::desc()->addX( OdRxObject_Dumper::desc(), &m_surfaceDumper );
  OdDgSolid::desc()->addX( OdRxObject_Dumper::desc(), &m_solidDumper );
  OdDgRasterAttachmentHeader::desc()->addX( OdRxObject_Dumper::desc(), &m_rasterAttachmentHeaderDumper );
  OdDgRasterHeader2d::desc()->addX( OdRxObject_Dumper::desc(), &m_rasterHeader2dDumper );
  OdDgRasterHeader3d::desc()->addX( OdRxObject_Dumper::desc(), &m_rasterHeader3dDumper );
  OdDgRasterComponent::desc()->addX( OdRxObject_Dumper::desc(), &m_rasterComponentDumper );
  OdDgTagElement::desc()->addX( OdRxObject_Dumper::desc(), &m_tagElementDumper );
  OdDgCellHeader2d::desc()->addX( OdRxObject_Dumper::desc(), &m_cellHeader2dDumper );
  OdDgCellHeader3d::desc()->addX( OdRxObject_Dumper::desc(), &m_cellHeader3dDumper );
  OdDgBSplineSurface::desc()->addX( OdRxObject_Dumper::desc(), &m_bSplineSurfaceDumper );
  OdDgLevelTable::desc()->addX( OdRxObject_Dumper::desc(), &m_levelTableDumper );
  OdDgLevelTableRecord::desc()->addX( OdRxObject_Dumper::desc(), &m_levelTableRecordDumper );
  OdDgLevelFilterTable::desc()->addX( OdRxObject_Dumper::desc(), &m_levelFilterTableDumper );
  OdDgLevelFilterTableRecord::desc()->addX( OdRxObject_Dumper::desc(), &m_levelFilterTableRecordDumper );
  OdDgFontTable::desc()->addX( OdRxObject_Dumper::desc(), &m_fontTableDumper );
  OdDgFontTableRecord::desc()->addX( OdRxObject_Dumper::desc(), &m_fontTableRecordDumper );
  OdDgTextStyleTable::desc()->addX( OdRxObject_Dumper::desc(), &m_textStyleTableDumper );
  OdDgTextStyleTableRecord::desc()->addX( OdRxObject_Dumper::desc(), &m_textStyleTableRecordDumper );
  OdDgDimStyleTable::desc()->addX( OdRxObject_Dumper::desc(), &m_dimStyleTableDumper );
  OdDgDimStyleTableRecord::desc()->addX( OdRxObject_Dumper::desc(), &m_dimStyleTableRecordDumper );
  OdDgMultilineStyleTable::desc()->addX( OdRxObject_Dumper::desc(), &m_multilineStyleTableDumper );
  OdDgMultilineStyleTableRecord::desc()->addX( OdRxObject_Dumper::desc(), &m_multilineStyleTableRecordDumper );
  OdDgLineStyleTable::desc()->addX( OdRxObject_Dumper::desc(), &m_lineStyleTableDumper );
  OdDgTextStyleTable::desc()->addX(OdRxObject_Dumper::desc(), &m_textStyleTableDumper);
  OdDgLineStyleTableRecord::desc()->addX( OdRxObject_Dumper::desc(), &m_lineStyleTableRecordDumper );
  OdDgRegAppTable::desc()->addX( OdRxObject_Dumper::desc(), &m_regAppTableDumper );
  OdDgRegAppTableRecord::desc()->addX( OdRxObject_Dumper::desc(), &m_regAppTableRecordDumper );
  OdDgViewGroupTable::desc()->addX( OdRxObject_Dumper::desc(), &m_viewGroupTableDumper );
  OdDgViewGroup::desc()->addX( OdRxObject_Dumper::desc(), &m_viewGroupDumper );
  OdDgView::desc()->addX( OdRxObject_Dumper::desc(), &m_viewDumper );
  OdDgNamedViewTable::desc()->addX( OdRxObject_Dumper::desc(), &m_namedViewTableDumper );
  OdDgSharedCellDefinitionTable::desc()->addX( OdRxObject_Dumper::desc(), &m_sharedCellDefinitionTableDumper );
  OdDgSharedCellDefinition::desc()->addX( OdRxObject_Dumper::desc(), &m_sharedCellDefinitionDumper );
  OdDgSharedCellReference::desc()->addX( OdRxObject_Dumper::desc(), &m_sharedCellReferenceDumper );
  OdDgTagDefinitionSetTable::desc()->addX( OdRxObject_Dumper::desc(), &m_tagSetDefinitionTableDumper );
  OdDgTagDefinitionSet::desc()->addX( OdRxObject_Dumper::desc(), &m_tagSetDefinitionDumper );
  OdDgTagDefinition::desc()->addX( OdRxObject_Dumper::desc(), &m_tagDefinitionDumper );
  OdDgColorTable::desc()->addX( OdRxObject_Dumper::desc(), &m_colorTableDumper );
  OdDgReferenceAttachmentHeader::desc()->addX( OdRxObject_Dumper::desc(), &m_referenceAttachmentDumper );
  OdDgMeshFaceLoops::desc()->addX( OdRxObject_Dumper::desc(), &m_meshFaceLoopsDumper );
  OdDgMeshPointCloud::desc()->addX( OdRxObject_Dumper::desc(), &m_meshPointCloudDumper );
  OdDgMeshTriangleList::desc()->addX( OdRxObject_Dumper::desc(), &m_meshTriangleListDumper );
  OdDgMeshQuadList::desc()->addX( OdRxObject_Dumper::desc(), &m_meshQuadListDumper );
  OdDgMeshTriangleGrid::desc()->addX( OdRxObject_Dumper::desc(), &m_meshTriangleGridDumper );
  OdDgMeshQuadGrid::desc()->addX( OdRxObject_Dumper::desc(), &m_meshQuadGridDumper );
  OdDgProxyElement::desc()->addX( OdRxObject_Dumper::desc(), &m_proxyDumper );
  OdDgApplicationData::desc()->addX( OdRxObject_Dumper::desc(), &m_applicationDataDumper );
}


OdDgDumper::~OdDgDumper()
{
  OdDgDatabase::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgModel::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgLine2d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgLine3d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgLineString2d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgLineString3d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgText2d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgText3d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgTextNode2d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgTextNode3d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgShape2d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgShape3d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgCurve2d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgCurve3d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgEllipse2d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgEllipse3d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgArc2d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgArc3d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgCone::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgComplexString::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgComplexShape::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgPointString2d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgPointString3d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgDimension::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgMultiline::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgBSplineCurve2d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgBSplineCurve3d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgSurface::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgSolid::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgRasterAttachmentHeader::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgRasterHeader2d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgRasterHeader3d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgRasterComponent::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgTagElement::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgCellHeader2d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgCellHeader3d::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgBSplineSurface::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgLevelTable::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgLevelTableRecord::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgLevelFilterTable::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgLevelFilterTableRecord::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgFontTable::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgFontTableRecord::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgTextStyleTable::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgTextStyleTableRecord::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgDimStyleTable::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgDimStyleTableRecord::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgMultilineStyleTable::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgMultilineStyleTableRecord::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgTextStyleTable::desc()->delX(OdRxObject_Dumper::desc());
  OdDgLineStyleTable::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgLineStyleTableRecord::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgRegAppTable::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgRegAppTableRecord::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgViewGroupTable::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgViewGroup::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgView::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgNamedViewTable::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgSharedCellDefinitionTable::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgSharedCellDefinition::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgSharedCellReference::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgTagDefinitionSetTable::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgTagDefinitionSet::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgTagDefinition::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgColorTable::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgReferenceAttachmentHeader::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgMeshFaceLoops::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgMeshPointCloud::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgMeshTriangleList::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgMeshQuadList::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgMeshTriangleGrid::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgMeshQuadGrid::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgProxyElement::desc()->delX( OdRxObject_Dumper::desc() );
  OdDgApplicationData::desc()->delX( OdRxObject_Dumper::desc() );

  OdRxObject_Dumper::rxUninit();
}


void OdDgDumper::rootDump( OdRxObjectPtr database ) const
{
  OdSmartPtr< OdRxObject_Dumper > database_dumper = database;

  //run the ordinal method
  database_dumper->dump( database );
}
