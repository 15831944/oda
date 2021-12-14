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
#ifndef __DGELEMENTPROPS__H__
#define __DGELEMENTPROPS__H__

#include "DgProperties.h"
#include "OdTimeStamp.h"

// Class OdDgElement properties. Category "Raw Data":

// No category.

/* 1 - ElementId        */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgElement,ElementId,OdDgElementId,"",1,"Element id value");

// Category "Raw Data"

/* 1 - ODAUniqueID      */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgElement, ODAUniqueID, OdUInt64, "Raw Data",1, "Handle value of element");
/* 2 - ElementType      */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgElement,ElementType,OdDgElement::ElementTypes,"Raw Data",2,"Base element type");
/* 3 - ElementSubType   */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgElement,ElementSubType,OdUInt8,"Raw Data",3,"Element subentity type");
/* 4 - ModificationTime */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgElement,ModificationTime,OdTimeStamp,"Raw Data",4,"Last modification time");
/* 5 - New              */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgElement,New,bool,"Raw Data",5,"Is new element");
/* 6 - Modified         */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgElement,Modified,bool,"Raw Data",6,"Is element modified");
/* 7 - Locked           */ ODRX_DECLARE_RX_PROPERTY(OdDgElement,Locked,bool,"Raw Data",7,"Is element locked", "");

// Category "General"

/* 1 - Name             */ ODRX_DECLARE_RX_PROPERTY_RO(OdDgElement, Name, OdString, "General",1, "Default name of element");

#endif // __DGELEMENTPROPS__H__
