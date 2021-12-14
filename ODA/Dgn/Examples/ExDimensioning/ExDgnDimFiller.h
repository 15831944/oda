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

class OdDgModel;

class OdExDgnFiller
{
public:
  OdExDgnFiller();
  
  void fillModel(OdDgModel* pModel);
  void MakeEntityBox(OdDgModel* pModel,OdGePoint3d currentPoint,double hCurBox, double wCurBox);

private:
  void createEntityBoxes(OdDgModel* pModel);
  void addFontTableRecord();
/*
  void addLevelTableRecord();
  void addFontTableRecord();
  void addDimStyleTableRecord();
  void addLineStyleTableRecord();
  void addMultilineStyleTableRecord();
  void addRegAppTableRecord();
  void addTextStyleTableRecord();
*/
  void addSizeArrowDimension( int boxRow, int boxColumn );
  void addInsideSizeArrowDimension( int boxRow, int boxColumn );
  void addStackedSizeArrowDimension( int boxRow, int boxColumn );
  void addSingleLocationDimension( int boxRow, int boxColumn );
  void addSizeStrokeDimension( int boxRow, int boxColumn );
  void addStackedLocationDimension( int boxRow, int boxColumn );
  void addCustomLinearDimension( int boxRow, int boxColumn );
  void addOrdinateDimension( int boxRow, int boxColumn );

  void addRadiusDimension( int boxRow, int boxColumn );
  void addRadiusExtendedDimension( int boxRow, int boxColumn );
  void addDiameterExtended(  );
  void addDiameterExtended2(  );

  void addCenterMarkDimension( int boxRow, int boxCol );
  void addAngleAxisXDimension( int boxRow, int boxColumn );
  void addAngleAxisYDimension( );
  void addLabelLineDimension( );
  void addNoteDimension( );
  void addMicrostationStyleNote( int boxRow, int boxColumn );
  void addAngleSizeDimension( int boxRow, int boxColumn );
  void addAngleSizeDimension2( int boxRow, int boxColumn );
  void addAngleLinesDimension( int boxRow, int boxColumn );
  void addAngleLinesDimension2( int boxRow, int boxColumn );
  void addAngleLocationDimension( int boxRow, int boxColumn );
  void addArcSizeDimension( int boxRow, int boxColumn );
  void addArcLocationDimension( int boxRow, int boxColumn );
  void addAlignmentOptions( int boxRow, int boxCol );
  void addOrdinates();
  void addCustomDim();
  void addRotVarSizeArrows();

  void addLinearDimensionRelativePoints();
  void addAssociativeLinearDimension();
  void addLinearDimensionWithLeaderAndRotatedText();
  void addLinearDimensionWithUserDefinedText();

  EntityBoxes m_EntityBoxes;
  OdDgModel* m_pModel3d;
};

#endif // __EX_DGNFILLER_H__
