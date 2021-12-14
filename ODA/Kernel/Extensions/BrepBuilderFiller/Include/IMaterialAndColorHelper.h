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

#include "OdError.h"

class OdBrEdge;
class OdBrFace;
class OdDbStub;
class OdCmEntityColor;
class OdGiMapper;
class OdBrepBuilderFillerParams;


/** \details
  Helper structure for handling materials and colors.

  Corresponding C++ library: TD_BrepBuilderFiller
  <group Extension_Classes>
*/
class OdIMaterialAndColorHelper
{
  friend class OdBrepBuilderFillerHelper;
public:
  /** \details
    Destroys the OdIMaterialAndColorHelper.
  */
  virtual ~OdIMaterialAndColorHelper() {}

protected:
  /** \details
    Initialize the helper before collecting B-Rep data.

    \param fillerParams [in] The OdBrepBuilderFiller parameters.
    \returns eOk if successful, or an appropriate error code otherwise.
    \remarks
    This method called by OdBrepBuilderFiller every time before initialization of OdBrepBuilder.
  */
  virtual OdResult init(const OdBrepBuilderFillerParams& fillerParams) = 0;

public:
  /** \details
    Gets a face visual information specific for OdBrepBuilder.

    \param sourceFace               [in]  The face.
    \param faceMaterial             [out] Receives a id of a material for OdBrepBuilder.
    \param faceMaterialMapping      [out] Receives a material mapping for OdBrepBuilder. Should be ignoredd if applyFaceMaterialMapping is false.
    \param applyFaceMaterialMapping [out] Receives a flag signals to use faceMaterialMapping.
    \param faceColor                [out] Receives a color for OdBrepBuilder. Should be ignored if applyFaceColor is false.
    \param applyFaceColor           [out] Receives a flag signals to use faceColor.
    \returns eOk if successful, or an appropriate error code otherwise.
  */
  virtual OdResult getFaceVisualInfo(const OdBrFace& sourceFace,
    OdDbStub*& faceMaterial,
    OdGiMapper& faceMaterialMapping, bool& applyFaceMaterialMapping,
    OdCmEntityColor& faceColor, bool& applyFaceColor) = 0;

  /** \details
    Gets an edge visual information for OdBrepBuilder.

    \param edge           [in]  The edge.
    \param edgeColor      [out] Receives a color for OdBrepBuilder. Should be ignored if applyEdgeColor is false.
    \param applyEdgeColor [out] Receives a flag signals to use edgeColor.
    \returns eOk if successful, or an appropriate error code otherwise.
  */
  virtual OdResult getEdgeVisualInfo(const OdBrEdge& edge,
    OdCmEntityColor& edgeColor, bool& applyEdgeColor) = 0;

};
