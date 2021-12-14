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

#ifndef _TO_STRING_H
#define _TO_STRING_H

#include "OdaCommon.h"

#include "CmColorBase.h"
#include "DbHandle.h"
#include "Ge/Ge.h"
#include "Ge/GeExtents3d.h"
#include "Ge/GeScale2d.h"
#include "Ge/GeScale3d.h"
#include "Gi/Gi.h"
#include "Gi/GiRasterImage.h"
#include "Gi/GiGeometry.h"
#include "Gi/GiPolyline.h"
#include "Gi/GiTextStyle.h"
#include "Gs/Gs.h"

#include "DgElementId.h"
#include "DgCmColor.h"

OdString shortenPath(OdString path, int maxPath = 40);
OdString toArcSymbolTypeString (const int                   val);
OdString toDegreeString(const double                        val);
OdString toHexString(const int                              val);
OdString toLooptypeString(const int                         val);
OdString toOdGiVisibilityString(const OdUInt8               val);
OdString toString(const OdDgCmEntityColor&                  val);
OdString toString(const OdCmTransparency&                   val);
OdString toString(const OdGe::EntityId                      val);
OdString toString(const OdGePoint2d&                        val);
OdString toString(const OdGePoint3d&                        val);
OdString toString(const OdGeScale2d&                        val);
OdString toString(const OdGeScale3d&                        val);
OdString toString(const OdGeVector2d&                       val);
OdString toString(const OdGeVector3d&                       val);
OdString toString(const OdGeVector3d*                      pVal);
OdString toString(const OdGiArcType                         val);
OdString toString(const OdGiFillType                        val);
OdString toString(const OdGiOrientationType                 val);
OdString toString(const OdGiPolyline::SegType               val);
OdString toString(const OdGiRasterImage::Units              val);
OdString toString(const OdGsView::RenderMode                val); 
OdString toString(const OdRxClass*                          val);
OdString toString(const OdString&                           val);
OdString toString(const OdChar*                             val);
OdString toString(const OdString& fmt, double               val);
OdString toString(const OdString& fmt, int                  val);
OdString toString(const bool                                val);
OdString toString(const double                              val);
OdString toString(const int                                 val);
OdString toString(const OdDbHandle&                         val);
OdString toString(const OdDgElementId&                      val);

OdString toRGBString(const OdUInt32& val);

// Auxiliary function to display different messages
// defined in ExDgnHostAppServices.cpp
void oddgPrintConsoleString(const wchar_t* fmt, ...);

#endif
