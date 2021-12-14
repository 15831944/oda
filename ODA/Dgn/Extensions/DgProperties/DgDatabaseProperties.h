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
#ifndef __DGDATABASEPROPS__H__
#define __DGDATABASEPROPS__H__

#include "DgProperties.h"
#include "DgDatabase.h"

// Class OdDgDatabase properties. Category "Database Properties":

// Category "Database Properties"

/* 1  - FileName               */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgDatabase, FileName, OdString, "Database Properties", 1, "Path to current database file");
/* 2  - MajorVersion           */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgDatabase, MajorVersion, OdUInt32, "Database Properties", 2, "Major version of current database file");
/* 3  - MinorVersion           */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgDatabase, MinorVersion, OdUInt32, "Database Properties", 3, "Minor version of current database file");
/* 4  - LevelTableId           */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgDatabase, LevelTableId,OdDgElementId,"Database Properties",4,"Level table element id");
/* 5  - LineStyleTableId       */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgDatabase, LineStyleTableId, OdDgElementId, "Database Properties", 5, "Line style table element id");
/* 6  - TextStyleTableId       */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgDatabase, TextStyleTableId, OdDgElementId, "Database Properties", 6, "Text style table element id");
/* 7  - FontTableId            */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgDatabase, FontTableId, OdDgElementId, "Database Properties", 7, "Font table element id");
/* 8  - MaterialTableId        */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgDatabase, MaterialTableId, OdDgElementId, "Database Properties", 8, "Material table element id");
/* 9  - ColorPalette           */ ODRX_DECLARE_RX_PROPERTY_COLLECTION(OdDgDatabase, ColorPalette, OdCmEntityColor, "Database Properties", 9, "Current color palette");
/* 10 - DisplayStyleTableId    */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgDatabase, DisplayStyleTableId, OdDgElementId, "Database Properties", 10, "Display style table element id");
/* 11 - LevelFilterTableId     */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgDatabase,LevelFilterTableId,OdDgElementId,"Database Properties",11,"Level table element id");
/* 12 - Models                 */ ODRX_DECLARE_RX_PROPERTY_COLLECTION_TREE(OdDgDatabase,Models,OdDgElementId,"Database Properties",12,"Array of model ids","Model");
/* 13 - SharedCellDefinitions  */ ODRX_DECLARE_RX_PROPERTY_COLLECTION_NAMED(OdDgDatabase,SharedCellDefinitions,OdDgElementId,"Database Properties",13,"Array of shared cell definition ids", false);


#endif // __DGDATABASEPROPS__H__
