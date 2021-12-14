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

#ifndef __FMMDL_DRAW_BODY_H__
#define __FMMDL_DRAW_BODY_H__

#include "Modeler/FMMdlBase.h"
#include "FMBuildSettings.h"

class OdGiCommonDraw;

namespace FacetModeler
{

  /** \details
     Draws the specified body to OdGiCommonDraw with the view direction specified.

     \param pDraw [in/out] Object to draw on.
     \param rBody     [in] Body to draw.
     \param pViewDir  [in] View direction vector for calculating silhouettes as OdGeVector3d.
     \param bUseRGBColors  [in] Defines whether the colors format of this Body is RGB (Arhitecture uses color indices)
     \param bUseMappingCoords  [in] Defines whether mapping coordinates of vertices will be set to VertexData
  */
  void FMGEOMETRY_API DrawBody(OdGiCommonDraw *pDraw, const Body &rBody, 
    const OdGeVector3d *pViewDir = NULL, 
    bool bUseRGBColors = false, 
    bool bUseMappingCoords = false);

} // namespace
#endif //#ifndef __FMMDL_DRAW_BODY_H__
