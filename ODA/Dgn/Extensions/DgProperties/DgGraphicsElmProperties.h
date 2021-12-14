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
#ifndef __DGGRAPHICSELEMENTPROPS__H__
#define __DGGRAPHICSELEMENTPROPS__H__

#include "DgProperties.h"
#include "DgGraphicsElement.h"

// Class OdDgGraphicsElement properties. Category "General":

// Category "General"

/*  2 - Level              */ ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement, Level, OdDgElementId, "General",2, "Level table record element id", "/LevelTableId/Items");
/*  3 - Color              */ ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement,Color,OdCmEntityColor,"General",3,"Element color","/ColorPalette");
/*  4 - LineStyle          */ ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement, LineStyle, OdDgElementId, "General",4, "Line style table element id", "/LineStyleTableId/Items");
/*  5 - LineWeight         */ ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement, LineWeight, OdDb::LineWeight, "General",5, "Element line weight", "");
/*  6 - Class              */ ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement, Class, OdDgGraphicsElement::Class, "General",6, "Element class", "");
/*  7 - Transparency       */ ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement, Transparency, OdCmTransparency, "General",7, "Transparency of element", "");
/*  8 - Priority           */ ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement, Priority, OdUInt32, "General",8, "Drawing order priority", "");
/*  9 - Thickness          */ ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement, Thickness, double, "General",9, "Element thickness", "");
/* 10 - DisplayStyle       */ ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement, DisplayStyle, OdDgElementId, "General",10, "Display style id", "DisplayStyleTableId/Items");
/* 11 - GraphicsGroupIndex */ ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement, GraphicsGroupIndex,OdUInt32,"General",11,"Element graphics group index","");
/* 12 - Invisible          */ ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement, Invisible,bool,"General",12,"Invisible flag of element","");
/* 13 - 3D                 */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgGraphicsElement,3D,bool,"General",13,"3d format flag of element");
/* 14 - ViewIndependent    */ ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement,ViewIndependent,bool,"General",14,"View independent flag of element","");
/* 15 - NonPlanar          */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgGraphicsElement,NonPlanar,bool,"General",15,"Non planar flag of element");
/* 16 - NotSnappable       */ ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement,NotSnappable,bool,"General",16,"Not snappable flag of element","");
/* 17 - HBit               */ ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement,HBit,bool,"General",17,"Hole bit flag of element","");
/* 18 - Extents            */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgGraphicsElement,Extents,OdGeExtents3d,"General",18,"Extents of element");

// no category.

/* 2 - ItemTypeInstances  */ ODRX_DECLARE_RX_CPLX_PROPERTY_INDEXED(OdDgGraphicsElement, ItemTypeInstances, OdDgECPropertyInstanceFullPath, "",2, "Item type instance array");

/* ColorIndex         */ //ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement,ColorIndex,OdUInt32,"General","Index of element color", "");
/* LevelIndex         */ //ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement,LevelIndex,OdUInt32,"","Level table element entry id","/LevelTableId/Items [Index]");
/* LevelName          */ //ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement,LevelName,OdString,"General","Level name","/LevelTableId/Items [Name]");
/* LineStyleIndex     */ //ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement,LineStyleIndex,OdUInt32,"","Line style table element entry id","/LineStyleTableId/Items [Index]");
/* LineStyleName      */ //ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement,LineStyleName,OdString,"General","Line style name","/LineStyleTableId/Items [Name]");
/* DisplayStyleName   */ //ODRX_DECLARE_RX_PROPERTY(OdDgGraphicsElement,DisplayStyleName,OdString,"","Display style name","DisplayStyleTableId/Items [Name]");

#endif // __DGGRAPHICSELEMENTPROPS__H__
