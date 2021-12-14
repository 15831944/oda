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
//#include "../vec/TrVecDevice.h"
//#include "TrGL2LocalRendition.h"
#include "RxObjectImpl.h"
#include "TrRndLocalRenditionHost.h"
#include "TrVecLocalSharingProvider.h"

OdTrVecLocalSharingProvider::OdTrVecLocalSharingProvider()
  : OdTrVecResourceSharingProvider()
{
}

OdTrVecLocalSharingProvider::~OdTrVecLocalSharingProvider()
{
}

OdTrVecResourceSharingProviderPtr OdTrVecLocalSharingProvider::createObject(OdTrRndRenderModule *pRenderModule)
{
  OdSmartPtr<OdTrVecLocalSharingProvider> pLSP = OdRxObjectImpl<OdTrVecLocalSharingProvider>::createObject();
  pLSP->m_pRenderModule = pRenderModule;
  return pLSP;
}

OdTrRndLocalRenditionHost *OdTrVecLocalSharingProvider::renditionHost(OdTrVecDevice *pRequestFor)
{
  ODA_ASSERT(!m_pRenderModule.isNull());
  return m_pRenderModule->createRenditionHost(OdTrVecResourceSharingProvider::rendition(pRequestFor));
}

// Share metafile with secondary device (should be overrided by client, do nothing by default)
bool OdTrVecLocalSharingProvider::shareMetafile(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                                                OdTrVisMetafileId metafileId, void * /*pInternalDataPtr*/, void * /*pOriginatorPtr*/)
{
  OdTrVisMetafileDef metafileDef;
  renditionHost(pRequestedFrom)->getMetafileDef(metafileId, metafileDef);
  OdTrVecResourceSharingProvider::rendition(pRequestedFor)->onMetafileAdded(metafileId, metafileDef);
  if (renditionHost(pRequestedFrom)->supportShallowPropertiesClone(metafileDef.m_pMetafile))
    renditionHost(pRequestedFrom)->shallowPropertiesClone(metafileDef.m_pMetafile);
  else
  { OdUInt32 nProps = renditionHost(pRequestedFrom)->hasProperties(OdTrVisRendition::kMetafileData, metafileId);
    for (OdUInt32 nProp = 0; nProps; nProp++, nProps >>= 1)
    {
      if (GETBIT(nProps, 1))
      { OdTrVisPropertyDef *pPropDef = NULL;
        if (renditionHost(pRequestedFrom)->getPropertyDef(OdTrVisRendition::kMetafileData, metafileId, (OdTrVisPropertyDef::PropertyType)nProp, pPropDef))
        { OdTrVecResourceSharingProvider::rendition(pRequestedFor)->onMetafilePropertyAttached(metafileId, *pPropDef);
          delete pPropDef; }
      }
    }
  }
  return true;
}

// Share metafiles group with secondary device (should be overrided by client, do nothing by default)
bool OdTrVecLocalSharingProvider::shareGroup(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                                              OdTrVisGroupId groupId, void * /*pInternalDataPtr*/, void * /*pOriginatorPtr*/)
{
  OdTrVecResourceSharingProvider::rendition(pRequestedFor)->onGroupAdded(groupId);
  OdUInt32 nProps = renditionHost(pRequestedFrom)->hasProperties(OdTrVisRendition::kGroupData, groupId);
  for (OdUInt32 nProp = 0; nProps; nProp++, nProps >>= 1)
  {
    if (GETBIT(nProps, 1))
    { OdTrVisPropertyDef *pPropDef = NULL;
      if (renditionHost(pRequestedFrom)->getPropertyDef(OdTrVisRendition::kGroupData, groupId, (OdTrVisPropertyDef::PropertyType)nProp, pPropDef))
      { OdTrVecResourceSharingProvider::rendition(pRequestedFor)->onGroupPropertyAttached(groupId, *pPropDef);
        delete pPropDef; }
    }
  }
  return true;
}

// Share texture with secondary device (should be overrided by client, do nothing by default)
bool OdTrVecLocalSharingProvider::shareTexture(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                                               OdTrVisTextureId textureId, void *pInternalDataPtr, void *pOriginatorPtr)
{
  OdTrVisTextureDef textureDef; bool bNeedSend = false;
  bool bRes = internalTextureProc(pRequestedFor, pRequestedFrom, textureId, pInternalDataPtr, pOriginatorPtr, textureDef, bNeedSend);
  if (bNeedSend)
    OdTrVecResourceSharingProvider::rendition(pRequestedFor)->onTextureAdded(textureId, textureDef);
  return bRes;
}

// Share material with secondary device (should be overrided by client, do nothing by default)
bool OdTrVecLocalSharingProvider::shareMaterial(OdTrVecDevice *pRequestedFor, OdTrVecDevice *pRequestedFrom,
                                                OdTrVisMaterialId materialId, void *pInternalDataPtr, void *pOriginatorPtr)
{
  OdTrVisMaterialDef pDef;
  // Access exist material
  renditionHost(pRequestedFrom)->getMaterialDef(materialId, pDef);
  // Send material to new device
  OdTrVecResourceSharingProvider::rendition(pRequestedFor)->onMaterialAdded(materialId, pDef);
  // Send textures if exist
  materialTexturesProc(pRequestedFor, pRequestedFrom, materialId, pInternalDataPtr, pOriginatorPtr);
  return true;
}

//
