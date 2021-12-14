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
#ifndef __DGEXTRAPROPS__H__
#define __DGEXTRAPROPS__H__

#include "DgProperties.h"
#include "DgHatchPattern.h"

// Class OdDgHatchPattern properties. Category "HatchPattern"

/*  1 - PatternType            */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgHatchPattern,PatternType,OdDgHatchPattern::PatternType,"HatchPattern",1,"Hatch pattern type");
/*  2 - UseTolerance           */ ODRX_DECLARE_RX_PROPERTY(OdDgHatchPattern, UseTolerance, bool, "HatchPattern",2, "If true then tolerance of hatch pattern is enabled", "");
/*  3 - Tolerance              */ ODRX_DECLARE_RX_PROPERTY(OdDgHatchPattern,Tolerance,double,"HatchPattern",3,"Tolerance of hatch pattern", "");
/*  4 - UseRotation            */ ODRX_DECLARE_RX_PROPERTY(OdDgHatchPattern, UseRotation, bool, "HatchPattern",4, "If true then rotation of hatch pattern is enabled", "");
/*  5 - Rotation               */ ODRX_DECLARE_RX_PROPERTY(OdDgHatchPattern,Rotation,OdGeQuaternion,"HatchPattern",5,"Pattern rotation for 3d elements", "");
/*  6 - 2dRotation             */ ODRX_DECLARE_RX_PROPERTY_ANGLE(OdDgHatchPattern,2dRotation,double,"HatchPattern",6,"Rotation angle of pattern for 2d elements", "");
/* 19 - UseOffset              */ ODRX_DECLARE_RX_PROPERTY(OdDgHatchPattern, UseOffset, bool, "HatchPattern",19, "If true then offset of hatch pattern is enabled", "");
/* 20 - Offset                 */ ODRX_DECLARE_RX_PROPERTY(OdDgHatchPattern, Offset, OdGePoint3d, "HatchPattern",20, "Offset of hatch pattern", "");
/* 21 - Snappable              */ ODRX_DECLARE_RX_PROPERTY(OdDgHatchPattern, Snappable, bool, "HatchPattern",21, "If true then hatch pattern is snappable", "");
/* 22 - UseMultilineIndices    */ ODRX_DECLARE_RX_PROPERTY(OdDgHatchPattern, UseMultilineIndices, bool, "HatchPattern",22, "If true then multiline indexes of hatch pattern is used", "");
/* 23 - LowMultilineIndex      */ ODRX_DECLARE_RX_PROPERTY(OdDgHatchPattern,LowMultilineIndex,OdUInt32,"HatchPattern",23,"Low profile index of hatched multiline section", "");
/* 24 - HighMultilineIndex     */ ODRX_DECLARE_RX_PROPERTY(OdDgHatchPattern,HighMultilineIndex,OdUInt32,"HatchPattern",24,"High profile index of hatched multiline section", "");

// Class OdDgLinearHatchPattern properties. Category "HatchPattern":

/*  7 - Space                  */ ODRX_DECLARE_RX_PROPERTY(OdDgLinearHatchPattern,Space,double,"HatchPattern",7,"Space value of linear hatch pattern", "");
/*  8 - Angle                  */ ODRX_DECLARE_RX_PROPERTY_ANGLE(OdDgLinearHatchPattern,Angle,double,"HatchPattern",8,"Angle value of linear hatch pattern", "");
/* 13 - UseLineColor           */ ODRX_DECLARE_RX_PROPERTY(OdDgLinearHatchPattern, UseLineColor, bool, "HatchPattern",13, "If true then line style of linear hatch pattern is enabled", "");
/* 14 - LineColor              */ ODRX_DECLARE_RX_PROPERTY(OdDgLinearHatchPattern, LineColor, OdCmEntityColor, "HatchPattern",14, "Line color of linear hatch pattern", "/ColorPalette");
/* 15 - UseLineStyle           */ ODRX_DECLARE_RX_PROPERTY(OdDgLinearHatchPattern, UseLineStyle, bool, "HatchPattern",15, "If true then line style of linear hatch pattern is enabled", "");
/* 16 - LineStyleEntryId       */ ODRX_DECLARE_RX_PROPERTY(OdDgLinearHatchPattern,LineStyleEntryId,OdUInt32,"HatchPattern", 16, "Line style entry id of linear hatch pattern", "/LineStyleTableId/Items [Index]");
/* 17 - UseLineWeight          */ ODRX_DECLARE_RX_PROPERTY(OdDgLinearHatchPattern, UseLineWeight, bool, "HatchPattern", 17, "If true then line weight of linear hatch pattern is enabled", "");
/* 18 - LineWeight             */ ODRX_DECLARE_RX_PROPERTY(OdDgLinearHatchPattern,LineWeight,OdDb::LineWeight,"HatchPattern", 18, "Line weight of linear hatch pattern", "");

// Class OdDgCrossHatchPattern properties. Category "HatchPattern":

/*  7 - Space1                 */ ODRX_DECLARE_RX_PROPERTY(OdDgCrossHatchPattern,Space1,double,"HatchPattern", 7, "Space1 value of cross hatch pattern", "");
/*  8 - Angle1                 */ ODRX_DECLARE_RX_PROPERTY_ANGLE(OdDgCrossHatchPattern,Angle1,double,"HatchPattern", 8, "Angle1 value of cross hatch pattern", "");
/*  9 - Space2                 */ ODRX_DECLARE_RX_PROPERTY(OdDgCrossHatchPattern,Space2,double,"HatchPattern", 9, "Space2 value of cross hatch pattern", "");
/* 10 - Angle2                 */ ODRX_DECLARE_RX_PROPERTY_ANGLE(OdDgCrossHatchPattern,Angle2,double,"HatchPattern", 10, "Angle2 value of cross hatch pattern", "");
/* 13 - UseLineColor           */ ODRX_DECLARE_RX_PROPERTY(OdDgCrossHatchPattern, UseLineColor, bool, "HatchPattern", 13, "If true then line style of cross hatch pattern is enabled", "");
/* 14 - LineColor              */ ODRX_DECLARE_RX_PROPERTY(OdDgCrossHatchPattern, LineColor, OdCmEntityColor, "HatchPattern", 14, "Line color of cross hatch pattern", "/ColorPalette");
/* 15 - UseLineStyle           */ ODRX_DECLARE_RX_PROPERTY(OdDgCrossHatchPattern, UseLineStyle, bool, "HatchPattern", 15, "If true then line style of cross hatch pattern is enabled", "");
/* 16 - LineStyleEntryId       */ ODRX_DECLARE_RX_PROPERTY(OdDgCrossHatchPattern,LineStyleEntryId,OdUInt32,"HatchPattern", 16, "Line style entry id of cross hatch pattern", "");
/* 17 - UseLineWeight          */ ODRX_DECLARE_RX_PROPERTY(OdDgCrossHatchPattern, UseLineWeight, bool, "HatchPattern", 17, "If true then line weight of cross hatch pattern is enabled", "");
/* 18 - LineWeight             */ ODRX_DECLARE_RX_PROPERTY(OdDgCrossHatchPattern,LineWeight,OdDb::LineWeight,"HatchPattern", 18, "Line weight of cross hatch pattern", "");


// Class OdDgSymbolHatchPattern properties. Category "HatchPattern":

/*  7 - Space1                 */ ODRX_DECLARE_RX_PROPERTY(OdDgSymbolHatchPattern,Space1,double,"HatchPattern",7, "Space1 value of symbol hatch pattern", "");
/*  8 - Angle1                 */ ODRX_DECLARE_RX_PROPERTY_ANGLE(OdDgSymbolHatchPattern,Angle1,double,"HatchPattern", 8, "Angle1 value of symbol hatch pattern", "");
/*  9 - Space2                 */ ODRX_DECLARE_RX_PROPERTY(OdDgSymbolHatchPattern,Space2,double,"HatchPattern", 9, "Space2 value of symbol hatch pattern", "");
/* 10 - Angle2                 */ ODRX_DECLARE_RX_PROPERTY_ANGLE(OdDgSymbolHatchPattern,Angle2,double,"HatchPattern", 10, "Angle2 value of symbol hatch pattern", "");
/* 11 - SymbolId               */ ODRX_DECLARE_RX_PROPERTY(OdDgSymbolHatchPattern, SymbolId, OdUInt64, "HatchPattern", 11, "Symbol element id", "/SharedCellDefinitionIdArray");
/* 12 - TrueScale              */ ODRX_DECLARE_RX_PROPERTY(OdDgSymbolHatchPattern, TrueScale, bool, "HatchPattern", 12, "If true then true scale of symbol hatch pattern is enabled", "");
/* 13 - UseColor               */ ODRX_DECLARE_RX_PROPERTY(OdDgSymbolHatchPattern, UseColor, bool, "HatchPattern", 13, "If true then line style of symbol hatch pattern is enabled", "");
/* 14 - Color                  */ ODRX_DECLARE_RX_PROPERTY(OdDgSymbolHatchPattern, Color, OdCmEntityColor, "HatchPattern", 14, "Line color of symbol hatch pattern", "");
/* 15 - UseLineStyle           */ ODRX_DECLARE_RX_PROPERTY(OdDgSymbolHatchPattern, UseLineStyle, bool, "HatchPattern", 15, "If true then line style of symbol hatch pattern is enabled", "");
/* 16 - LineStyleEntryId       */ ODRX_DECLARE_RX_PROPERTY(OdDgSymbolHatchPattern,LineStyleEntryId,OdUInt32,"HatchPattern", 16, "Line style entry id of symbol hatch pattern", "");
/* 17 - UseLineWeight          */ ODRX_DECLARE_RX_PROPERTY(OdDgSymbolHatchPattern, UseLineWeight, bool, "HatchPattern", 17, "If true then line weight of symbol hatch pattern is enabled", "");
/* 18 - LineWeight             */ ODRX_DECLARE_RX_PROPERTY(OdDgSymbolHatchPattern,LineWeight,OdDb::LineWeight,"HatchPattern", 18, "Line weight of symbol hatch pattern", "");

// Class OdDgDWGHatchPattern properties. Category "HatchPattern":

/* 7 - PatternName            */ ODRX_DECLARE_RX_PROPERTY(OdDgDWGHatchPattern,PatternName,OdString,"HatchPattern",7, "Name of dwg based hatch pattern", "");
/* 8 - DwgLines               */ ODRX_DECLARE_RX_PROPERTY_INDEXED(OdDgDWGHatchPattern,DwgLines,DWGHatchLine,"HatchPattern",8, "Lines of dwg based hatch pattern");

#endif // __DGEXTRAPROPS__H__
