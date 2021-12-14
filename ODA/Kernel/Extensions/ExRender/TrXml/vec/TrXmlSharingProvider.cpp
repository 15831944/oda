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
// GLES2 device local resource sharing provider

#include "OdaCommon.h"
#include "TrVisRenderClient.h"
#include "../TrXmlRendition.h"
#include "TrXmlSharingProvider.h"

OdTrXmlSharingProvider::OdTrXmlSharingProvider()
  : OdTrVecResourceSharingProvider()
{
}

OdTrXmlSharingProvider::~OdTrXmlSharingProvider()
{
}

OdTrXmlRenditionRedir *OdTrXmlSharingProvider::rendition(OdTrVecDevice *pRequestFor)
{
  return static_cast<OdTrXmlRenditionRedir*>(OdTrVecResourceSharingProvider::rendition(pRequestFor));
}

// Share metafile with secondary device (should be overrided by client, do nothing by default)
bool OdTrXmlSharingProvider::shareMetafile(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                                              OdTrVisMetafileId metafileId, void * /*pInternalDataPtr*/, void * /*pOriginatorPtr*/)
{
  shareIdRegistration(pRequestedFor, pRequestedFrom, metafileId, OdTrVisRendition::kMetafileData);
  OdTrVisUniqueID *pRegId = rendition(pRequestedFor)->idRegistrator()->get(OdTrVisRendition::kMetafileData, metafileId);
  if (pRegId) rendition(pRequestedFor)->xmlRendition().xml()->RegisterId(metafileId, pRegId, OdTrXmlServer::kMetafileId);
  // Returns true since metafiles already available on client side
  return true;
}

// Share metafiles group with secondary device (should be overrided by client, do nothing by default)
bool OdTrXmlSharingProvider::shareGroup(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                                           OdTrVisGroupId groupId, void * /*pInternalDataPtr*/, void * /*pOriginatorPtr*/)
{
  shareIdRegistration(pRequestedFor, pRequestedFrom, groupId, OdTrVisRendition::kGroupData);
  OdTrVisUniqueID *pRegId = rendition(pRequestedFor)->idRegistrator()->get(OdTrVisRendition::kGroupData, groupId);
  if (pRegId) rendition(pRequestedFor)->xmlRendition().xml()->RegisterId(groupId, pRegId, OdTrXmlServer::kGroupId);
  // Returns true since metafile groups already available on client side
  return true;
}

// Share texture with secondary device (should be overrided by client, do nothing by default)
bool OdTrXmlSharingProvider::shareTexture(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                                             OdTrVisTextureId textureId, void *pInternalDataPtr, void *pOriginatorPtr)
{
  shareIdRegistration(pRequestedFor, pRequestedFrom, textureId, OdTrVisRendition::kTextureData);
  if ((clientSettings(pRequestedFor).isSupportNonPow2Textures() != clientSettings(pRequestedFrom).isSupportNonPow2Textures()) ||
      (clientSettings(pRequestedFor).isSupportNonPow2TexturesWrapping() != clientSettings(pRequestedFrom).isSupportNonPow2TexturesWrapping()) ||
      (clientSettings(pRequestedFor).m_nMaxTextureSize != clientSettings(pRequestedFrom).m_nMaxTextureSize) ||
      (clientSettings(pRequestedFor).m_nTextureAlignment != clientSettings(pRequestedFrom).m_nTextureAlignment) ||
      (clientSettings(pRequestedFor).isRequireBGR() != clientSettings(pRequestedFrom).isRequireBGR()) ||
      (clientSettings(pRequestedFor).isSupportBGR() != clientSettings(pRequestedFrom).isSupportBGR()) ||
      (clientSettings(pRequestedFor).isSupportBGRA() != clientSettings(pRequestedFrom).isSupportBGRA()) ||
      (clientSettings(pRequestedFor).isLutOverride() != clientSettings(pRequestedFrom).isLutOverride()) ||
      (clientSettings(pRequestedFor).isLutMonochrome() != clientSettings(pRequestedFrom).isLutMonochrome()) ||
      (clientSettings(pRequestedFor).isLutPalette() != clientSettings(pRequestedFrom).isLutPalette()) ||
      (renderClient(pRequestedFor)->getProperty(OD_T("UseLutPalette"))->getUInt32() != renderClient(pRequestedFrom)->getProperty(OD_T("UseLutPalette"))->getUInt32()))
  { // Texture settings differ, so we need regenerate textures with newer settings
    OdTrVisTextureDef textureDef; bool bNeedSend = false;
    bool bRes = internalTextureProc(pRequestedFor, pRequestedFrom, textureId, pInternalDataPtr, pOriginatorPtr, textureDef, bNeedSend);
    if (bNeedSend)
      rendition(pRequestedFor)->onTextureAdded(textureId, textureDef);
    return bRes;
  }
  OdTrVisUniqueID *pRegId = rendition(pRequestedFor)->idRegistrator()->get(OdTrVisRendition::kTextureData, textureId);
  if (pRegId) rendition(pRequestedFor)->xmlRendition().xml()->RegisterId(textureId, pRegId, OdTrXmlServer::kTextureId);
  // Returns true since textures with compatible settings already available on client side
  return true;
}

// Share material with secondary device (should be overrided by client, do nothing by default)
bool OdTrXmlSharingProvider::shareMaterial(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                                              OdTrVisMaterialId materialId, void *pInternalDataPtr, void *pOriginatorPtr)
{
  shareIdRegistration(pRequestedFor, pRequestedFrom, materialId, OdTrVisRendition::kMaterialData);
  OdTrVisUniqueID *pRegId = rendition(pRequestedFor)->idRegistrator()->get(OdTrVisRendition::kMaterialData, materialId);
  if (pRegId) rendition(pRequestedFor)->xmlRendition().xml()->RegisterId(materialId, pRegId, OdTrXmlServer::kMaterialId);
  // This call process nested material textures
  materialTexturesProc(pRequestedFor, pRequestedFrom, materialId, pInternalDataPtr, pOriginatorPtr);
  // Returns true since materials already available on client side
  return true;
}

//
