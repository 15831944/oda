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
#include "IMaterialAndColorHelper.h"

#include "DbBaseDatabase.h"
#include "CmColorBase.h"
#include "Gi/GiMaterial.h"

class OdGiContext;


/** \details
  Helper structure for handling materials and colors.
  \remark
  You have to setup source and destination database in OdBrepBuilderFillerParams.
  <group Extension_Classes>
*/
class BREPBUILDERFILLER_DLL OdBaseMaterialAndColorHelper : public OdIMaterialAndColorHelper
{
public:
  /** \details
    Creates a new instance of helper.

    \param pDefaultMaterial [in] Default source material.
  */
  explicit OdBaseMaterialAndColorHelper(OdDbStub* pDefaultMaterial = NULL);
  /** \details
    Destroys the OdBaseMaterialAndColorHelper.
  */
  ~OdBaseMaterialAndColorHelper();

  /** \details
    Sets a material that will be used if B-Rep face does not have any material.

    \param pSourceEntityMaterial [in] Default source material.
  */
  void setSourceEntityMaterial(OdDbStub* pSourceEntityMaterial) {
    m_pSourceEntityMaterial = pSourceEntityMaterial;
  }

  /** \details
    Sets a material mapping that will be used if B-Rep face does not have any material mapping.

    \param materialMapper [in] Default source mapping.
  */
  void setSourceEntityMaterialMapping(const OdGiMapper& materialMapper) {
    m_hasSourceEntityMaterialMapping = true;
    m_sourceEntityMaterialMapping = materialMapper;
  }
  /** \details
    Resets the default source material mapping.
  */
  void resetSourceEntityMapping() {
    m_hasSourceEntityMaterialMapping = false;
    m_sourceEntityMaterialMapping = OdGiMapper();
  }

  /** \details
    Sets a color of source faces and edges that will be used if B-Rep face or edge does not have color.

    \param defaultColor [in] Default source color.
  */
  void setSourceEntityColor(OdCmEntityColor defaultColor) {
    setSourceFaceColor(defaultColor);
    setSourceEdgeColor(defaultColor);
  }
  /** \details
    Resets the default color of source faces and edges.
  */
  void resetSourceEntityColor() {
    resetSourceFaceColor();
    resetSourceEdgeColor();
  }

  /** \details
    Sets a color of source faces that will be used if B-Rep face does not have color.

    \param defaultColor [in] Default source color.
  */
  void setSourceFaceColor(OdCmEntityColor defaultColor) {
    m_hasSourceEntityFaceColor = true;
    m_sourceEntityFaceColor = defaultColor;
  }
  /** \details
    Resets the default color of source faces.
  */
  void resetSourceFaceColor() {
    m_hasSourceEntityFaceColor = false;
    m_sourceEntityFaceColor = OdCmEntityColor::kNone;
  }

  /** \details
    Sets a color of source edges that will be used if B-Rep edge does not have color.

    \param defaultColor [in] Default source color.
  */
  void setSourceEdgeColor(OdCmEntityColor defaultColor) {
    m_hasSourceEntityEdgeColor = true;
    m_sourceEntityEdgeColor = defaultColor;
  }
  /** \details
    Resets the default color of source edges.
  */
  void resetSourceEdgeColor() {
    m_hasSourceEntityEdgeColor = false;
    m_sourceEntityEdgeColor = OdCmEntityColor::kNone;
  }

protected:
  /** \details
    Initialize the helper before collecting B-Rep data.

    \param fillerParams [in] The OdBrepBuilderFiller parameters.
    \returns eOk if successful, or an appropriate error code otherwise.
    \remarks
    This method is called by OdBrepBuilderFiller every time before initialization of OdBrepBuilder.
  */
  OdResult init(const OdBrepBuilderFillerParams& fillerParams) ODRX_OVERRIDE;

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
  OdResult getFaceVisualInfo(const OdBrFace& face,
    OdDbStub*& faceMaterial,
    OdGiMapper& faceMaterialMapping, bool& applyFaceMaterialMapping,
    OdCmEntityColor& faceColor, bool& applyFaceColor) ODRX_OVERRIDE;

  /** \details
    Gets an edge visual information for OdBrepBuilder.

    \param edge           [in]  The edge.
    \param edgeColor      [out] Receives a color for OdBrepBuilder. Should be ignored if applyEdgeColor is false.
    \param applyEdgeColor [out] Receives a flag signals to use edgeColor.
    \returns eOk if successful, or an appropriate error code otherwise.
  */
  OdResult getEdgeVisualInfo(const OdBrEdge& edge,
    OdCmEntityColor& edgeColor, bool& applyEdgeColor) ODRX_OVERRIDE;

  //DOM-IGNORE-BEGIN
  enum ProductFlags
  {
    fFaceHasColor = 1 << 0,// if true, return face color to Odbrepbuilder, otherwise face color will be in material
    fFaceHasMaterialMapping = 1 << 1,// if true, return material mapping to Odbrepbuilder, otherwise material mapping will be in material
    fEdgeHasColor = 1 << 2,// if true, return edge color to Odbrepbuilder, otherwise edge color will be ignored
    fNoVisual = 0,
    fFaceColorAndMappingEdgeColor = fFaceHasColor | fFaceHasMaterialMapping | fEdgeHasColor,
  };
  //DOM-IGNORE-END

protected:
  /** \details
    Gets a color specific to destinion database.

    \param sourceDbColor      [in]  Color from source B-Rep.
    \param destinationDbColor [out] Receives a color for OdBrepBuilder.
    \returns eNotImplemented.
  */
  virtual OdResult convertColor(
    const OdCmEntityColor& sourceDbColor,
    OdCmEntityColor& destinationDbColor
  );

  /** \details
    Gets a source face material id.

    \param pSourceDb [in] Source database.
    \param face      [in] The face.
    \returns The face material id.
  */
  OdDbStub* getSourceMaterialId(const OdBrFace& face) const;
  /** \details
    Gets a source face material mapping.

    \param face           [in]  The face.
    \param materialMapper [out] Receives a material mapping.
    \returns true if the face has material mapping or false otherwise.
  */
  bool getSourceMaterialMapper(const OdBrFace& face, OdGiMapper& materialMapper) const;
  /** \details
    Gets a source face color.

    \param face      [in]  The face.
    \param faceColor [out] Receives a color.
    \returns true if the face has color or false otherwise.
  */
  bool getSourceFaceColor(const OdBrFace& face, OdCmEntityColor& faceColor) const;
  /** \details
    Return source edge color.
  */
  /** \details
    Gets a source edge color.

    \param edge      [in]  The edge.
    \param edgeColor [out] Receives a color.
    \returns true if the edge has color or false otherwise.
  */
  bool getSourceEdgeColor(const OdBrEdge& edge, OdCmEntityColor& edgeColor) const;

  mutable OdDbBaseDatabasePtr m_pSourceDb;
  mutable OdDbBaseDatabasePtr m_pDestinationDb;

  OdGiDefaultContextPtr m_pSourceGiContext;
  OdDbStub* m_pSourceEntityMaterial;
  bool m_hasSourceEntityMaterialMapping;
  OdGiMapper m_sourceEntityMaterialMapping;
  bool m_hasSourceEntityFaceColor;
  OdCmEntityColor m_sourceEntityFaceColor;
  bool m_hasSourceEntityEdgeColor;
  OdCmEntityColor m_sourceEntityEdgeColor;

  ProductFlags m_destinationFlags;
  bool m_sameDbType;


  /** \details
    Helper structure for cache materials.
  */
  struct MaterialDataStore
  {
    OdDbStub* sourceMaterial;
    OdGiMapper sourceMapper;
    OdCmEntityColor sourceColor;

    OdDbStub* materialId;

    /** \details
      Creates a new instance of material item.
    */
    MaterialDataStore()
      : sourceMaterial(NULL)
      , sourceMapper(OdGiMapper::kIdentity)
      , sourceColor(OdCmEntityColor::kNone)
      , materialId(NULL)
    {}

    //DOM-IGNORE-BEGIN
    bool operator == (const MaterialDataStore& other) const
    {
      return sourceMaterial == other.sourceMaterial
        && sourceMapper == other.sourceMapper
        && sourceColor == other.sourceColor;
    }
    //DOM-IGNORE-END
  };

  typedef OdArray<MaterialDataStore> MaterialCache;
  MaterialCache m_materialCache;

  /** \details
    Gets a material by a material cache item.

    \param matData [in] The material cache item.
    \returns The material id.
  */
  virtual OdDbStub * findMaterialInUserCache(const MaterialDataStore & matData);
  /** \details
    Registers a new material cache item.

    \param matData [in] The material cache item.
  */
  virtual void addMaterialToUserCache(const MaterialDataStore & matData);
};
