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

#pragma once

#include "BrepBuilderFillerDef.h"

#include "Ge/GeCurve2dPtrArray.h"
#include "Ge/GeCurve3dPtrArray.h"
#include "Ge/GeSurfacePtrArray.h"
#include "Ge/GeCurve2d.h"
#include "Ge/GeCurve3d.h"
#include "Ge/GeSurface.h"
#include "BrepBuilderInitialParams.h"

class OdBrepBuilder;
class OdBrBrep;
struct BrepBuilderInitialData;
class OdIMaterialAndColorHelper;


/** \details
  This class implements BrepBuilderFiller.
  Corresponding C++ library: TD_BrepBuilderFiller
  <group Extension_Classes>
*/
class BREPBUILDERFILLER_DLL OdBrepBuilderFiller
{
  OdGeCurve3dPtrArray       m_edges;
  OdGeCurve2dPtrArray       m_coedges;
  OdGeSurfacePtrArray       m_surfaces;
  OdBrepBuilderFillerParams m_params;

  //DOM-IGNORE-BEGIN
  void clearTempArrays();
  //DOM-IGNORE-END

  friend class OdBrepBuilderFillerHelper;

public:
  /** \details
    Returns properties of filler.

    \returns Properties of filler.
  */
  OdBrepBuilderFillerParams& params() { return m_params; }

  /** \details
    Returns properties of filler.

    \returns Properties of filler.
  */
  const OdBrepBuilderFillerParams& params() const { return m_params; }

  /** \details
    Init the OdBrepBuilder by data.

    \param builder [out] OdBrepBuilder that will be inited.
    \param data    [in]  Helper structure for initialize the builder.
    \returns eOk if successful, or an appropriate error code otherwise.
  */
  OdResult initFrom(OdBrepBuilder& builder, const BrepBuilderInitialData& data);

  /** \details
    Init the OdBrepBuilder with geometries from OdBrBrep.

    \param builder         [out] OdBrepBuilder that will be inited.
    \param brep            [in]  B-Rep which will be used to collect data.
    \param pMaterialHelper [in/out] Helper object for convert materials and colors.
    \returns eOk if successful, or an appropriate error code otherwise.
  */
  OdResult initFrom(
    OdBrepBuilder& builder,
    const OdBrBrep& brep,
    OdIMaterialAndColorHelper* pMaterialHelper = NULL);

  //DOM-IGNORE-BEGIN
  OdResult initFromNURBSingleFace(OdBrepBuilder& builder, const OdBrBrep& brep);
  //DOM-IGNORE-END
};
