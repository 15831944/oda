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

#include "OdaCommon.h"

#include "BaseMaterialAndColorHelper.h"
#include "BrepBuilderInitialParams.h"
#include "Br/BrEdge.h"
#include "Br/BrFace.h"
#include "Gi/GiCommonDraw.h"
#include "Gi/GiMaterialTraitsData.h"


namespace
{
  OdBaseMaterialAndColorHelper::ProductFlags getConvertationFlags(const OdBrepBuilderFillerParams& params)
  {
    if (OdBrepBuilderFillerParams::kBrepAcisDwg == params.destinationBrepType()) {
      return OdBaseMaterialAndColorHelper::fFaceColorAndMappingEdgeColor;
    }
    if (OdBrepBuilderFillerParams::kBrepAcisDwg == params.sourceBrepType() && OdBrepBuilderFillerParams::kBrepMd == params.destinationBrepType()) {
      return OdBaseMaterialAndColorHelper::fFaceColorAndMappingEdgeColor;
    }
    return OdBaseMaterialAndColorHelper::fNoVisual;
  }

  OdDbBaseDatabase* getSourceDb(const OdBrepBuilderFillerParams& fillerParams) {
    return OdBrepBuilderFillerParams::kBrepMd != fillerParams.sourceBrepType() ? fillerParams.sourceDb() : fillerParams.destinationDb();
  }
  OdDbBaseDatabase* getDestinationDb(const OdBrepBuilderFillerParams& fillerParams) {
    return OdBrepBuilderFillerParams::kBrepMd != fillerParams.destinationBrepType() ? fillerParams.destinationDb() : fillerParams.sourceDb();
  }
}


OdBaseMaterialAndColorHelper::OdBaseMaterialAndColorHelper(OdDbStub* pDefaultMaterial)
  : m_pSourceEntityMaterial(pDefaultMaterial)
  , m_hasSourceEntityMaterialMapping(false)
  , m_hasSourceEntityFaceColor(false)
  , m_sourceEntityFaceColor(OdCmEntityColor::kNone)
  , m_hasSourceEntityEdgeColor(false)
  , m_sourceEntityEdgeColor(OdCmEntityColor::kNone)
  , m_destinationFlags(fNoVisual)
  , m_sameDbType(false)
{
}

OdBaseMaterialAndColorHelper::~OdBaseMaterialAndColorHelper()
{
}

OdDbStub * OdBaseMaterialAndColorHelper::findMaterialInUserCache(const MaterialDataStore & matData)
{
  return NULL;
}

void OdBaseMaterialAndColorHelper::addMaterialToUserCache(const MaterialDataStore & matData)
{
}

OdResult OdBaseMaterialAndColorHelper::init(const OdBrepBuilderFillerParams& fillerParams)
{
  m_pSourceGiContext.release();

  OdBrepBuilderFillerParams::BrepType sourceType = fillerParams.sourceBrepType();
  OdBrepBuilderFillerParams::BrepType destType = fillerParams.destinationBrepType();
  if (OdBrepBuilderFillerParams::kBrepUnknown == sourceType || OdBrepBuilderFillerParams::kBrepUnknown == destType) {
    return eNotInitializedYet;
  }
  if (OdBrepBuilderFillerParams::kBrepMd == sourceType && OdBrepBuilderFillerParams::kBrepMd == destType) {
    return eNotApplicable;
  }

  m_pSourceDb = getSourceDb(fillerParams);
  m_pDestinationDb = getDestinationDb(fillerParams);
  if (!m_pSourceDb || !m_pDestinationDb) {
    return eNotInitializedYet;
  }

  OdDbBaseDatabasePEPtr pSourceDbPE(m_pSourceDb);
  OdDbBaseDatabasePEPtr pDestinationDbPE(m_pDestinationDb);
  if (pSourceDbPE.isNull() || pDestinationDbPE.isNull()) {
    return eNotInitializedYet;
  }

  m_destinationFlags = getConvertationFlags(fillerParams);
  m_sameDbType = m_pSourceDb.get() == m_pDestinationDb.get() || sourceType == destType;
  m_pSourceGiContext = pSourceDbPE->createGiContext(m_pSourceDb);

  return eOk;
}

OdResult OdBaseMaterialAndColorHelper::getFaceVisualInfo(const OdBrFace& face,
  OdDbStub*& faceMaterial,
  OdGiMapper& faceMaterialMapping, bool& applyFaceMaterialMapping,
  OdCmEntityColor& faceColor, bool& applyFaceColor)
{
  // init
  faceMaterial = 0;
  faceMaterialMapping = OdGiMapper::kIdentity;
  applyFaceMaterialMapping = false;
  faceColor = OdCmEntityColor::kNone;
  applyFaceColor = false;

  if (!m_pSourceDb || !m_pDestinationDb) {
    return eNotInitializedYet;
  }

  OdResult status = eOk;

  // source info
  OdDbStub* pSourceMaterial = getSourceMaterialId(face);
  OdGiMapper sourceMaterialMapping;
  bool hasSourceMaterialMapping = getSourceMaterialMapper(face, sourceMaterialMapping);
  OdCmEntityColor sourceFaceColor = OdCmEntityColor::kNone;
  bool hasSourceFaceColor = getSourceFaceColor(face, sourceFaceColor);
  if (!pSourceMaterial && !hasSourceMaterialMapping && !hasSourceFaceColor) {
    return eOk;
  }

  // same db => use source as out info
  if (m_pSourceDb.get() == m_pDestinationDb.get())
  {
    faceMaterial = pSourceMaterial;
    if (GETBIT(m_destinationFlags, fFaceHasMaterialMapping)
      && hasSourceMaterialMapping)
    {
      applyFaceMaterialMapping = true;
      faceMaterialMapping = sourceMaterialMapping;
    }
    if (GETBIT(m_destinationFlags, fFaceHasColor)
      && hasSourceFaceColor)
    {
      applyFaceColor = true;
      faceColor = sourceFaceColor;
    }

    return eOk;
  }

  // different product => need convertation
  MaterialDataStore materialInfo;
  materialInfo.sourceMaterial = pSourceMaterial;

  OdGiMapper* pMaterialMapper = NULL;
  if (hasSourceMaterialMapping)
  {
    if (GETBIT(m_destinationFlags, fFaceHasMaterialMapping))
    {
      applyFaceMaterialMapping = true;
      faceMaterialMapping = sourceMaterialMapping;
    }
    else
    {
      materialInfo.sourceMapper = sourceMaterialMapping;
      pMaterialMapper = &materialInfo.sourceMapper;
    }
  }

  OdCmEntityColor* pMaterialColor = NULL;
  if (hasSourceFaceColor)
  {
    if (GETBIT(m_destinationFlags, fFaceHasColor))
    {
      applyFaceColor = true;
      faceColor = sourceFaceColor;
    }
    else
    {
      status = convertColor(sourceFaceColor, materialInfo.sourceColor);
      if (eOk != status) {
        return status;
      }
      pMaterialColor = &materialInfo.sourceColor;
    }
  }

  MaterialCache::size_type idx = 0;
  if (m_materialCache.find(materialInfo, idx))
  {
    faceMaterial = m_materialCache[idx].materialId;
    return eOk;
  }
  OdDbStub * userMat = findMaterialInUserCache(materialInfo);
  if (userMat)
  {
    faceMaterial = userMat;
    materialInfo.materialId = faceMaterial;
    m_materialCache.append(materialInfo);
    return eOk;
  }

  // internal material
  OdGiMaterialTraitsTaker materialTraits;
  OdGiMaterialTraitsTaker* pMaterialTraits = NULL;
  if (pSourceMaterial)
  {
    OdDbBaseDatabasePEPtr pSourceDbPE(m_pSourceDb);
    if (pSourceDbPE.isNull()) {
      return eNotInitializedYet;
    }
    OdGiDrawablePtr pGiMaterial = pSourceDbPE->openObject(pSourceMaterial);
    if (pGiMaterial.isNull()) {
      return eNullObjectPointer;
    }

    OdUInt32 flags = pGiMaterial->setAttributes(&materialTraits);
    pMaterialTraits = &materialTraits;
  }

  if (pMaterialTraits || pMaterialMapper || pMaterialColor)
  {
    OdDbBaseDatabasePEPtr pDestinationDbPE(m_pDestinationDb);
    if (pDestinationDbPE.isNull()) {
      return eNotInitializedYet;
    }
    status = pDestinationDbPE->faceConversionHelper(faceMaterial, m_pSourceDb, m_pDestinationDb, pMaterialTraits, pMaterialMapper, pMaterialColor);
    if (eOk == status && faceMaterial)
    {
      materialInfo.materialId = faceMaterial;
      m_materialCache.append(materialInfo);
      addMaterialToUserCache(materialInfo);
    }
  }

  return status;
}

OdResult OdBaseMaterialAndColorHelper::getEdgeVisualInfo(const OdBrEdge& edge,
  OdCmEntityColor& edgeColor, bool& applyEdgeColor)
{
  edgeColor = OdCmEntityColor::kNone;
  applyEdgeColor = false;

  if (!m_pSourceDb || !m_pDestinationDb) {
    return eNotInitializedYet;
  }

  if (!GETBIT(m_destinationFlags, fEdgeHasColor)) {
    return eOk;
  }

  OdCmEntityColor sourceEdgeColor;
  applyEdgeColor = getSourceEdgeColor(edge, sourceEdgeColor);
  if (!applyEdgeColor) {
    return eOk;
  }

  // same product => use same color
  if (m_sameDbType) {
    edgeColor = sourceEdgeColor;
    return eOk;
  }

  // different product => convert
  return convertColor(sourceEdgeColor, edgeColor);
}

OdResult OdBaseMaterialAndColorHelper::convertColor(
  const OdCmEntityColor& sourceDbColor,
  OdCmEntityColor& destinationDbColor)
{
  return eNotImplemented;
}


// Getter of source visual info:

OdDbStub* OdBaseMaterialAndColorHelper::getSourceMaterialId(const OdBrFace& face) const
{
  if (!m_pSourceGiContext.isNull())
  {
    OdUInt64 matId = 0;
    if (face.getMaterialID(matId)) {
      return m_pSourceGiContext->getStubByMaterialId(m_pSourceDb, matId);
    }

    OdString matName;
    if (face.getMaterialString(matName)) {
      return m_pSourceGiContext->getStubByMatName(m_pSourceDb, matName);
    }
  }

  return m_pSourceEntityMaterial;
}

bool OdBaseMaterialAndColorHelper::getSourceMaterialMapper(const OdBrFace& face, OdGiMapper& materialMapper) const
{
  OdGeMatrix3d mx;
  OdBrFace::Projection projection;
  OdBrFace::Tiling tiling;
  OdBrFace::AutoTransform autoTransform;

  bool rc = face.getMaterialMapper(mx, projection, tiling, autoTransform);

  if (rc)
  {
    materialMapper = OdGiMapper();
    materialMapper.setProjection(OdGiMapper::Projection(projection));
    materialMapper.setUTiling(OdGiMapper::Tiling(tiling));
    materialMapper.setVTiling(OdGiMapper::Tiling(tiling));
    materialMapper.setAutoTransform(OdGiMapper::AutoTransform(autoTransform));
    materialMapper.transform() = mx;
  }
  else if (m_hasSourceEntityMaterialMapping)
  {
    materialMapper = m_sourceEntityMaterialMapping;
    rc = true;
  }

  return rc;
}

bool OdBaseMaterialAndColorHelper::getSourceFaceColor(const OdBrFace& face, OdCmEntityColor& color) const
{
  bool rc = face.getColor(color);
  if (!rc && m_hasSourceEntityFaceColor) {
    color = m_sourceEntityFaceColor;
    rc = true;
  }

  return rc;
}

bool OdBaseMaterialAndColorHelper::getSourceEdgeColor(const OdBrEdge& edge, OdCmEntityColor& color) const
{
  bool rc = edge.getColor(color);
  if (!rc && m_hasSourceEntityEdgeColor) {
    color = m_sourceEntityEdgeColor;
    rc = true;
  }

  return rc;
}
