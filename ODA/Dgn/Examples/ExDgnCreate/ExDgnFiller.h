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
#ifndef __EX_DGNFILLER_H__
#define __EX_DGNFILLER_H__

#include "EntityBoxes.h"
#include "Ge/GePoint2d.h"
#include "DgModel.h"

class OdDgModel;

class OdExDgnFiller
{
public:
  OdExDgnFiller();

  void fillDatabase(OdDgDatabase* pDb);
  void fillSummaryInfo(OdDgDatabase* pDb);
  void fillModels();
  void enableCreationOfSS3Materials( bool bEnable );

private:
  void createEntityBoxes2d( OdDgModelPtr pModel );
  void createEntityBoxes3d();

  void addLevelTableRecord();
  void addMaterialTableRecord();
  void addMaterialV8TableRecord();
  void addLevelFilterTableRecord();
  void addFontTableRecord();
  void addDimStyleTableRecord();
  void addLineStyleTableRecord();
  void addMultilineStyleTableRecord();
  void addRegAppTableRecord();
  void addTextStyleTableRecord();
  void addColorTableRecord();
  void addElementTemplates();
  void addColorBooks();
  void addACS();
  void addEnvironmentMapTable();
  void addDisplayStyleTableRecord();
  void addURLandFileLink( OdDgElementPtr& pElement );
  void addItemTypesLibrary();
  void addVariblesList();
  void addXmlCustomScheme();
  void addPrintStyle();
  void addDictionaryTableAndFence();

  void addTextWithVolumeClipping( int boxRow, int boxCol );
  void addEllipse         (int boxRow, int boxCol);
  void addArc             (int boxRow, int boxCol);
  void addCone            (int boxRow, int boxCol);
  void addLines           ( int boxRow, int boxColumn );
  void addLineStyleLines  ( int boxRow, int boxColumn );
  void addText            ( int boxRow, int boxColumn );
  void addTextWithJustifucation( int boxRow, int boxColumn );
  void addTextNode        ( int boxRow, int boxColumn );
  void addShape           ( int boxRow, int boxColumn );
  void addFilledShape     ( int boxRow, int boxColumn );
  void addCurve           ( int boxRow, int boxColumn );
  void addPointString     ( int boxRow, int boxColumn );
  void addLineStringWithBreak( int boxRow, int boxColumn );
  void addDimension       ( int boxRow, int boxColumn );
  void addComplexString   ( int boxRow, int boxCol);
  void addMultiline       ( int boxRow, int boxColumn );
  void addComplexShape    ( int boxRow, int boxColumn );
  void addBSplineCurve    ( int boxRow, int boxColumn );
  void addBSplineSurface  ( int boxRow, int boxColumn );
  void addSurface         ( int boxRow, int boxColumn );
  void addSolid           ( int boxRow, int boxColumn );
  void addLights          ( int boxRow, int boxColumn );
  void addRaster          ( int boxRow, int boxColumn );
  void addByteRaster      ( int boxRow, int boxColumn );
  void addRasterAttach    ( int boxRow, int boxColumn );
  void addRasterAttachBmp ( int boxRow, int boxColumn );
  void addRasterAttachPdf ( int boxRow, int boxColumn );
  void addTagElement      ( int boxRow, int boxColumn );
  void addMesh            ( int boxRow, int boxColumn );
  void addColorMesh       ( int boxRow, int boxColumn );
  void addNormAndTexMesh  ( int boxRow, int boxColumn );
  void addAttach          ( int boxRow, int boxColumn );  
  void addSharedCells     ( int boxRow, int boxColumn );  
  void addCell            ( int boxRow, int boxColumn );  
  void addSmartSolid      ( int boxRow, int boxColumn );
  void addOleFrame        ( int boxRow, int boxColumn );
  void addPatterns        ( int boxRow, int boxColumn );
  void addShapeWithHole   ( int boxRow, int boxColumn );
  void addDBLinkages      ( int boxRow, int boxColumn );
  void addTrueColorShapes ( int boxRow, int boxColumn );
  void addShapeWithTransparency( int boxRow, int boxColumn );
  void addLineWithTemp    ( int boxRow, int boxColumn );
  void addTitleText       ( int boxRow, int boxColumn );
  void addSectionMarker   ( int boxRow, int boxColumn );
  void addArrowMarker     ( int boxRow, int boxColumn );
  void addDetailMarker    ( int boxRow, int boxColumn );
  void addAnnotationCell  ( int boxRow, int boxColumn );
  void addSectionClip     ( int boxRow, int boxColumn );
  void addTerrain         ( int boxRow, int boxColumn );
  void addItemTypes       ( int boxRow, int boxColumn );
  void addTableElement    ( int boxRow, int boxColumn );
	void addAssocRegion			( int boxRow, int boxColumn );
  void addNamedGroup();
  void addBSplineCurveWithKnots    ( int boxRow, int boxColumn );
  void addClosedBSplineCurveWithKnots    ( int boxRow, int boxColumn );

  void addStringLinkage( OdDgElement* pElm );
  void createBox3d( OdGePoint3d ptCenter, double dLength, double dHeight, double dDepth, 
                    const OdGeMatrix3d& matRotation, OdDgModelPtr& pModel, OdUInt32 uColorIndex );

  EntityBoxes m_EntityBoxes;
  OdDgModelPtr m_pModel3d;
  OdDgModelPtr m_pModel2d;
  OdDgSheetModelPtr m_pSheetModel;
  bool         m_bCreateSS3Materials;
  static const OdUInt8 m_uOleDataArr[];

  static const OdUInt32 m_uRasterWidth;
  static const OdUInt32 m_uRasterHeight;
  static const OdUInt8 m_uByteRasterDataArr[];

  OdArray<OdUInt64> m_uNamedGroupElementArr;
};

#endif // __EX_DGNFILLER_H__
