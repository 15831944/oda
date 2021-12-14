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
#ifndef __DGCURVEELEMENTPROPS__H__
#define __DGCURVEELEMENTPROPS__H__

#include "DgProperties.h"
#include "DgCurveElement2d.h"
#include "DgCurveElement3d.h"
#include "DgLine.h"
#include "DgArc.h"
#include "DgShape.h"
#include "DgCellHeader.h"
#include "DgComplexCurve.h"
#include "DgSharedCellReference.h"

// Class OdDgCurveElement2d properties. Category "Geometry":

/* 1  - StartPoint             */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgCurveElement2d, StartPoint, OdGePoint2d, "Geometry",1, "Start point of curve");
/* 2  - StartParam             */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgCurveElement2d, StartParam, double, "Geometry",2, "Start parameter of curve");
/* 3  - EndPoint               */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgCurveElement2d, EndPoint, OdGePoint2d, "Geometry",3, "End point of curve");
/* 4  - EndParam               */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgCurveElement2d, EndParam, double, "Geometry",4, "End parameter of curve");
/* 8  - Closed                 */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgCurveElement2d, Closed, bool, "Geometry",8, "Is curve closed");
/* 9  - Periodic               */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgCurveElement2d, Periodic, bool, "Geometry",9, "Is curve periodic");
/* 10 - LineStyleScale         */ ODRX_DECLARE_RX_PROPERTY(OdDgCurveElement2d,LineStyleScale,double,"Geometry",10,"Current line style scale for user defined line styles", "");
/* 11 - LineStyleModifiers     */ ODRX_DECLARE_RX_CPLX_PROPERTY(OdDgCurveElement2d,LineStyleModifiers,OdGiDgLinetypeModifiers,"Geometry",11,"Modifiers for user defined line styles", "");

// Class OdDgCurveElement3d properties. Category "Geometry":

/*  1 - StartPoint             */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgCurveElement3d, StartPoint, OdGePoint3d, "Geometry",1, "Start point of curve");
/*  2 - StartParam             */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgCurveElement3d, StartParam, double, "Geometry",2, "Start parameter of curve");
/*  3 - EndPoint               */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgCurveElement3d, EndPoint, OdGePoint3d, "Geometry",3, "End point of curve");
/*  4 - EndParam               */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgCurveElement3d, EndParam, double, "Geometry",4, "End parameter of curve");
/*  8 - Closed                 */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgCurveElement3d, Closed, bool, "Geometry",8, "Is curve closed");
/*  9 - Periodic               */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgCurveElement3d, Periodic, bool, "Geometry",9, "Is curve periodic");
/* 10 - LineStyleScale         */ ODRX_DECLARE_RX_PROPERTY(OdDgCurveElement3d,LineStyleScale,double,"Geometry",10,"Current line style scale for user defined line styles", "");
/* 11 - LineStyleModifiers     */ ODRX_DECLARE_RX_CPLX_PROPERTY(OdDgCurveElement3d,LineStyleModifiers,OdGiDgLinetypeModifiers,"Geometry",11,"Modifiers for user defined line styles", "");

// Class OdDgLine2d properties. Category "Geometry":

/* 1 - StartPoint             */ ODRX_DECLARE_RX_PROPERTY(OdDgLine2d,StartPoint,OdGePoint2d,"Geometry",1,"Start point of 2d line", "");
/* 3 - EndPoint               */ ODRX_DECLARE_RX_PROPERTY(OdDgLine2d,EndPoint,OdGePoint2d,"Geometry",3,"End point of 2d line", "");

// Class OdDgLine3d properties. Category "Geometry":

/* 1 - StartPoint             */ ODRX_DECLARE_RX_PROPERTY(OdDgLine3d,StartPoint,OdGePoint3d,"Geometry",1,"Start point of 3d line", "");
/* 3 - EndPoint               */ ODRX_DECLARE_RX_PROPERTY(OdDgLine3d,EndPoint,OdGePoint3d,"Geometry",3,"End point of 3d line", "");

// Class OdDgArc2d properties. Category "Geometry":

/* 1 - PrimaryAxis            */ ODRX_DECLARE_RX_PROPERTY(OdDgArc2d,PrimaryAxis,double,"Geometry",1,"Major axis of arc", "");
/* 2 - SecondaryAxis          */ ODRX_DECLARE_RX_PROPERTY(OdDgArc2d,SecondaryAxis,double,"Geometry",2,"Minor axis of arc", "");
/* 3 - Origin                 */ ODRX_DECLARE_RX_CPLX_PROPERTY(OdDgArc2d, Origin, OdGePoint2d, "Geometry",3, "Center point of arc", "");
/* 4 - RotationAngle          */ ODRX_DECLARE_RX_PROPERTY_ANGLE(OdDgArc2d,RotationAngle,double,"Geometry",4,"Rotation angle of arc ref vector", "");
/* 6 - StartAngle             */ ODRX_DECLARE_RX_PROPERTY_ANGLE(OdDgArc2d,StartAngle,double,"Geometry",6,"Start angle of arc", "");
/* 7 - SweepAngle             */ ODRX_DECLARE_RX_PROPERTY_ANGLE(OdDgArc2d,SweepAngle,double,"Geometry",7,"Sweep angle of arc", "");

// Class OdDgArc3d properties. Category "Geometry":

/* 1 - PrimaryAxis            */ ODRX_DECLARE_RX_PROPERTY(OdDgArc3d,PrimaryAxis,double,"Geometry",1,"Major axis of arc", "");
/* 2 - SecondaryAxis          */ ODRX_DECLARE_RX_PROPERTY(OdDgArc3d,SecondaryAxis,double,"Geometry",2,"Minor axis of arc", "");
/* 3 - Origin                 */ ODRX_DECLARE_RX_CPLX_PROPERTY(OdDgArc3d, Origin, OdGePoint3d, "Geometry",3, "Center point of arc", "");
/* 4 - RotationAngle          */ ODRX_DECLARE_RX_CPLX_PROPERTY(OdDgArc3d,Rotation,OdGeQuaternion,"Geometry",4,"3d rotation of arc ref vector", "");
/* 5 - Normal                 */ ODRX_DECLARE_RX_CPLX_PROPERTY_RO(OdDgArc3d, Normal, OdGeVector3d, "Geometry",5, "Arcs normal");
/* 6 - StartAngle             */ ODRX_DECLARE_RX_PROPERTY_ANGLE(OdDgArc3d,StartAngle,double,"Geometry",6,"Start angle of arc", "");
/* 7 - SweepAngle             */ ODRX_DECLARE_RX_PROPERTY_ANGLE(OdDgArc3d,SweepAngle,double,"Geometry",7,"Sweep angle of arc", "");

// Class OdDgCellHeader2d properties.

// Category "General":

/* 2 - Level                  */ ODRX_DECLARE_RX_PROPERTY(OdDgCellHeader2d, Level, OdDgElementId, "General", 2, "Level table record element id", "/LevelTableId/Items");
/* 3 - Color                  */ ODRX_DECLARE_RX_PROPERTY(OdDgCellHeader2d, Color, OdCmEntityColor, "General", 3, "Element color", "/ColorPalette");
/* 4 - LineStyle              */ ODRX_DECLARE_RX_PROPERTY(OdDgCellHeader2d, LineStyle, OdDgElementId, "General", 4, "Line style table element id", "/LineStyleTableId/Items");
/* 5 - LineWeight             */ ODRX_DECLARE_RX_PROPERTY(OdDgCellHeader2d, LineWeight, OdDb::LineWeight, "General", 5, "Element line weight", "");

// Category "Geometry":

/* 1 - Name                   */ ODRX_DECLARE_RX_PROPERTY(OdDgCellHeader2d,Name,OdString,"General",1,"Name of cell", "");
/* 3 - Origin                 */ ODRX_DECLARE_RX_CPLX_PROPERTY(OdDgCellHeader2d,Origin,OdGePoint2d,"Geometry",3,"Cell origin", "");
/* 4 - Transformation         */ ODRX_DECLARE_RX_PROPERTY(OdDgCellHeader2d,Transformation,OdGeMatrix2d,"Geometry",4,"Cell transformation", "");
/* 5 - GroupedHole            */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgCellHeader2d, GroupedHole, bool, "Geometry",5, "Is this cell grouped hole");
/* 6 - Elements               */ ODRX_DECLARE_RX_PROPERTY_COLLECTION_TREE(OdDgCellHeader2d, Elements,OdDgElementId,"Geometry",6,"Child elements","Entity");

// Class OdDgCellHeader3d properties. Category "Geometry":

// Category "General":

/* 2 - Level                  */ ODRX_DECLARE_RX_PROPERTY(OdDgCellHeader3d, Level, OdDgElementId, "General", 2, "Level table record element id", "/LevelTableId/Items");
/* 3 - Color                  */ ODRX_DECLARE_RX_PROPERTY(OdDgCellHeader3d, Color, OdCmEntityColor, "General", 3, "Element color", "/ColorPalette");
/* 4 - LineStyle              */ ODRX_DECLARE_RX_PROPERTY(OdDgCellHeader3d, LineStyle, OdDgElementId, "General", 4, "Line style table element id", "/LineStyleTableId/Items");
/* 5 - LineWeight             */ ODRX_DECLARE_RX_PROPERTY(OdDgCellHeader3d, LineWeight, OdDb::LineWeight, "General", 5, "Element line weight", "");

// Category "Geometry":

/* 1 - Name                   */ ODRX_DECLARE_RX_PROPERTY(OdDgCellHeader3d,Name,OdString,"General",1,"Name of cell", "");
/* 3 - Origin                 */ ODRX_DECLARE_RX_CPLX_PROPERTY(OdDgCellHeader3d,Origin,OdGePoint3d,"Geometry",3,"Cell origin", "");
/* 4 - Transformation         */ ODRX_DECLARE_RX_PROPERTY(OdDgCellHeader3d,Transformation,OdGeMatrix3d,"Geometry",4,"Cell transformation", "");
/* 5 - GroupedHole            */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgCellHeader3d, GroupedHole, bool, "Geometry",5, "Is this cell grouped hole");
/* 6 - Elements               */ ODRX_DECLARE_RX_PROPERTY_COLLECTION_TREE(OdDgCellHeader3d,Elements,OdDgElementId,"Geometry",6,"Child elements","Entity");

// Class OdDgShape2d properties. Category "Geometry":

/* 1  - Vertices               */ ODRX_DECLARE_RX_CPLX_PROPERTY_INDEXED(OdDgShape2d,Vertices,OdGePoint2d,"Geometry",1,"Shape vertices");
/* 1  - Material               */ ODRX_DECLARE_RX_PROPERTY(OdDgShape2d,Material,OdDgElementId,"Material",1,"Material id of shape", "MaterialTableId/Items");
/* 2  - MaterialName           */ //ODRX_DECLARE_RX_PROPERTY(OdDgShape2d,MaterialName,OdString,"Material",2,"Name of shape material", "MaterialTableId/Items [Name]");
/* 12 - Fill                   */ ODRX_DECLARE_RX_PROPERTY_UNIONTYPE(OdDgShape2d,Fill,void,"General",12,"Fill of shape", "","OdCmEntityColor;OdDgGradientFill");
/* 13 - HatchPattern           */ ODRX_DECLARE_RX_PROPERTY_UNIONTYPE(OdDgShape2d,HatchPattern,OdRxObjectPtr,"General",13,"Hatch pattern of shape", "", "OdDgLinearHatchPattern;OdDgCrossHatchPattern;OdDgSymbolHatchPattern;OdDgDWGHatchPattern");

// Class OdDgShape3d properties. Category "Geometry":

/*  1 - Vertices               */ ODRX_DECLARE_RX_CPLX_PROPERTY_INDEXED(OdDgShape3d,Vertices,OdGePoint3d,"Geometry",1,"Shape vertices");
/*  1 - Material               */ ODRX_DECLARE_RX_PROPERTY(OdDgShape3d,Material,OdDgElementId,"Material",1,"Material id of shape", "MaterialTableId/Items");
/*  2 - MaterialName           */ //ODRX_DECLARE_RX_PROPERTY(OdDgShape3d,MaterialName,OdString,"Material",2,"Name of shape material", "MaterialTableId/Items [Name]");
/* 12 - Fill                   */ ODRX_DECLARE_RX_PROPERTY_UNIONTYPE(OdDgShape3d,Fill,void,"General",12,"Fill of shape", "", "OdCmEntityColor;OdDgGradientFill");
/* 13 - HatchPattern           */ ODRX_DECLARE_RX_PROPERTY_UNIONTYPE(OdDgShape3d,HatchPattern,OdRxObjectPtr,"General",13,"Hatch pattern of shape", "","OdDgLinearHatchPattern;OdDgCrossHatchPattern;OdDgSymbolHatchPattern;OdDgDWGHatchPattern");

// Class OdDgComplexCurve properties. Category "Geometry":

/* 14 - Elements               */ ODRX_DECLARE_RX_PROPERTY_COLLECTION_TREE(OdDgComplexCurve, Elements, OdDgElementId, "Geometry",14, "Child elements","Entity");

// Class OdDgSharedCellReference properties. Category "Geometry":

/* 15 - BlockDefinition        */ ODRX_DECLARE_RX_PROPERTY_TREE(OdDgSharedCellReference, BlockDefinition, OdDgElementId, "Geometry",15, "Element id of block definition","/SharedCellDefinitions/Items","Block");

// Class OdDgReferenceAttachmentHeader properties. Category "ReferenceAttachment":

/* 1- - ReferencedModel        */ ODRX_DECLARE_RX_PROPERTY_TREE_RO(OdDgReferenceAttachmentHeader, ReferencedModel, OdDgElementId, "ReferenceAttachment", 1, "Element id of referenced model", "/Models/Items", "Database");

#endif // __DGCURVEELEMENTPROPS__H__
