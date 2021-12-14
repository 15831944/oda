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
#ifndef __EX_DGNCELLTRANSFORMFILLER_H__
#define __EX_DGNCELLTRANSFORMFILLER_H__

#include "DgModel.h"
#include "OdaCommon.h"
#include "DgDatabase.h"

class OdDgnCellTransformFiller
{

public:
  OdDgnCellTransformFiller( OdDgDatabase* pDb );
  virtual ~OdDgnCellTransformFiller();

  void fillModel2d();
  void fillModel3d();

private:

  void create2DBoxes( OdUInt32 nRows, OdUInt32 nCols );
  void create3DBoxes( OdUInt32 nRows, OdUInt32 nCols );

  OdGePoint2d get2dBoxCenter( OdUInt32 iRow, OdUInt32 iCol );
  OdGePoint3d get3dBoxCenter( OdUInt32 iRow, OdUInt32 iCol );

  void addLine3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addLineString3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addArc3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addEllipse3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addText3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addTextNode3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addAttribute3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addDimension3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addShape3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addComplexShape3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addComplexString3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addCellHeader3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addSolid( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addSurface( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addBSplineSurface( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addSmartSolid( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addBSplineCurve3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addMesh( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addSharedCell3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addRasterHeader3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addCone( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addMultiline3d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );

  void addLine2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addLineString2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addArc2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addEllipse2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addText2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addTextNode2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addAttribute2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addDimension2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addShape2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addComplexShape2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addComplexString2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addCellHeader2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addBSplineCurve2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addSharedCell2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addRasterHeader2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );
  void addMultiline2d( OdUInt32 iRow, OdUInt32 iCol, bool bRoatate = true );

private:

  OdDgDatabase* m_pDb;
  OdDgElementId m_idModel3d;
  OdDgModelPtr  m_pModel3d;
  OdDgModelPtr  m_pModel2d;

  OdGeMatrix3d  m_matRotation3d;
  OdGeMatrix2d  m_matRotation2d;

  static const double   boxItemSize;
  static const OdUInt32 iBoxColorIndex;
  static const OdUInt32 iControlLineColorIndex;
  static const OdUInt32 iCenterMarkColorIndex;
  static const OdUInt32 iBoxLStyleId;
  static const OdUInt32 iControlLineLStyleId;
  static const OdUInt32 iCellColorIndex;
  static const OdUInt32 iUnrotatedCellColorIndex;

};

#endif // __EX_DGNCELLTRANSFORMFILLER_H__
