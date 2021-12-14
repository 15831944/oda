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
#ifndef __DGTABLEPROPS__H__
#define __DGTABLEPROPS__H__

#include "DgProperties.h"
#include "DgTable.h"

// Class OdDgLevelTable properties. Category "Level Table":

/* 1 - Items      */ ODRX_DECLARE_RX_PROPERTY_COLLECTION_NAMED(OdDgLevelTable,Items,OdDgElementId,"Level Table",1,"Array of level ids", false);

// Class OdDgLineStyleTable properties. Category "Line Style Table":

/* 1 - Items      */ ODRX_DECLARE_RX_PROPERTY_COLLECTION_NAMED(OdDgLineStyleTable,Items,OdDgElementId,"Line Style Table",1,"Array of line style ids", true);

// Class OdDgMaterialTable properties. Category "Material Table":

/* 1 - Items      */ ODRX_DECLARE_RX_PROPERTY_COLLECTION_NAMED(OdDgMaterialTable,Items,OdDgElementId,"Material Table",1,"Array of material ids", false);

// Class OdDgModelTable properties. Category "Model Table":

/* 1 - Items      */ ODRX_DECLARE_RX_PROPERTY_COLLECTION_NAMED(OdDgModelTable, Items, OdDgElementId, "Model Table", 1, "Array of model ids", false);

// Class OdDgDisplayStyleTable properties. Category "Display Style Table":

/* 1 - Items      */ ODRX_DECLARE_RX_PROPERTY_COLLECTION_NAMED(OdDgDisplayStyleTable,Items,OdDgElementId,"Display Style Table",1,"Array of display style ids", false);

// Class OdDgLevelTableRecord properties. Category "Level":

/* 1 - Index      */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgLevelTableRecord,Index,OdUInt32,"Level",1,"Level entry id");
/* 2 - Name       */ ODRX_DECLARE_RX_PROPERTY(OdDgLevelTableRecord,Name,OdString,"Level",2,"Level name", "");

// Class OdDgLineStyleTableRecord properties. Category "Line Style":

/* 1 - Index      */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgLineStyleTableRecord,Index,OdUInt32,"Line Style",1,"Line style entry id");
/* 2 - Name       */ ODRX_DECLARE_RX_PROPERTY(OdDgLineStyleTableRecord,Name,OdString,"Line Style",2,"Line style name", "");

// Class OdDgMaterialTableRecord properties. Category "Material":

/* 1 - Name       */ ODRX_DECLARE_RX_PROPERTY(OdDgMaterialTableRecord,Name,OdString,"Material",1,"Material name", "");

// Class OdDgDisplayStyleTableRecord properties. Category "Display Style":

/* 1 - Index      */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgDisplayStyleTableRecord,Index,OdUInt32,"Display Style",1,"Display style entry id");
/* 2 - Name       */ ODRX_DECLARE_RX_PROPERTY(OdDgDisplayStyleTableRecord,Name,OdString,"Display Style",2,"Display style name", "");

// Class OdDgSharedCellDefinition properties. Category "General":

/* 1 - Name       */ ODRX_DECLARE_RX_PROPERTY(OdDgSharedCellDefinition,Name,OdString,"General",1,"Shared cell definition name", "");
/* 3 - Elements   */ ODRX_DECLARE_RX_PROPERTY_COLLECTION_TREE(OdDgSharedCellDefinition, Elements, OdDgElementId, "",3, "Shared cell definition children","Entity");

// Class OdDgModel properties. No category:

/* 1 - Name       */ ODRX_DECLARE_RX_PROPERTY(OdDgModel, Name, OdString, "General",1, "Model name", "");
/* 3 - Elements   */ ODRX_DECLARE_RX_PROPERTY_COLLECTION_TREE(OdDgModel, Elements, OdDgElementId, "",3, "Collection of model elements. Include all graphics elements and reference attachments.","Entity");

#endif // __DGTABLEPROPS__H__
