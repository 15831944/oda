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
// XmlGlesLoader.cpp

#include "OdaCommon.h"
#include "XmlGlesLoader.h"
#include "TrVisUniqueId.h"
#include "CloudTools.h"
#include "OdFakeDbStub.h"

#ifdef OD_TGS_ENABLED
#undef ODA_ASSERT_ONCE
#define ODA_ASSERT_ONCE(a)
#endif


///////////////////////////////////////////////////////////////////////////////

//static OdSharedPtr<std::map<OdAnsiString, OdSharedPtr<OdAnsiString> > > s_pMapFakeDbStub;
// redesigned to way of CspGsModel (to be compatible with current Core/Extensions/ExRender/GLES2/remote/JS-API.cpp)
// see also  Core/Extensions/ExRender/GLES2/remote/CspGsModel.h
static OdSharedPtr<std::map<OdAnsiString, OdSharedPtr<OdFakeDbStub> > > s_pMapFakeDbStub;

OdXmlGlesLoader::OdXmlGlesLoader( OdTrVisRendition* pRendition, OdXmlGlesLoaderIdMapper* pIdMapper )
  : m_id(0)
  , m_pIdMapper(pIdMapper)
  , m_pRendition(pRendition)
  , m_typeElement(OdTrVisArrayWrapper::Type_Invalid) // undef
  , m_indexElement(0)
  , m_numDataElement(0)
  , m_uaMarkerIndex(0)
  , m_pVisualStyleProperty(NULL)
  , m_typeVisualStyle(OdGiVariant::kUndefined)
  , m_propertyType(OdTrVisPropertyDef::kNumPropTypes)
  , m_objectChangedFlag(0xFF)
  , m_mfUserEntryNumData(0)
{
  ODA_ASSERT_ONCE(m_pRendition);
  m_bDestroyIdMapped = false;
  if( !m_pIdMapper )
  {
    m_pIdMapper = new OdXmlGlesIdMapper;
    m_bDestroyIdMapped = true;
  }
  m_pMapFakeDbStub = m_pIdMapper->getMapFakeDbStub();
  s_pMapFakeDbStub = m_pMapFakeDbStub; // to hold this map after destroying of OdXmlGlesLoader
}

OdXmlGlesLoader::~OdXmlGlesLoader()
{
  if( m_bDestroyIdMapped )
  {
    delete m_pIdMapper;
  }
}

OdTrVisMetafileContainer* OdXmlGlesLoader::metafile()
{
  if (m_pMetafileDef.get())
  {
    if (m_pMetafileDef->m_pMetafile.isNull())
    {
      m_pMetafileDef->m_pMetafile = new OdTrVisMetafileContainer;
      m_pMetafileDef->m_pMetafile->setDefault();
    }
    return m_pMetafileDef->m_pMetafile.get();
  }
  if (m_pBackgroundDef.get())
  {
    if (m_pBackgroundDef->m_pStream.isNull())
    {
      m_pBackgroundDef->m_pStream = new OdTrVisMetafileContainer;
      static_cast<OdTrVisMetafileContainer*>(m_pBackgroundDef->m_pStream.get())->setDefault();
    }
    return (OdTrVisMetafileContainer*) m_pBackgroundDef->m_pStream.get();
  }
  return NULL;
}

#define PK_CASE(enKey) case pk_##enKey
#define PK_CASE_EX(enKey) case pk_##MetafileAdded_MetafileData##enKey: case pk_##ViewportBackgroundChanged_MetafileData##enKey

bool OdXmlGlesLoader::DropBegin(OdEnPathKey pk)
{
  switch (pk)
  {
  PK_CASE(MaterialAdded):
    ODA_ASSERT_ONCE(m_pMaterialDef.isNull()); 
    m_pMaterialDef = new OdTrVisMaterialDef();
    m_pMaterialDef->setDefault();
  PK_CASE(MaterialDeleted):
    m_id = kTrVisNullId;
    break;
  PK_CASE(TextureAdded):
    ODA_ASSERT_ONCE(m_pTextureDef.isNull());
    m_pTextureDef = new OdTrVisTextureDef();
    m_pTextureDef->setDefault();
    m_curTexture.isValid = true;
  PK_CASE(TextureDeleted):
    m_id = kTrVisNullId;
    break;
  PK_CASE(OverlayAdded):
  PK_CASE(OverlayVisibilityChanged):
  PK_CASE(OverlayRenderOrderChanged):
    ODA_ASSERT_ONCE(m_pOverlayDef.isNull());
    m_pOverlayDef = new OdTrVisOverlayDef();
    m_pOverlayDef->setDefault();
  PK_CASE(OverlayDeleted):
    m_id = kTrVisNullId;
    break;
  PK_CASE(OverlayViewParamsOverride):
    ODA_ASSERT_ONCE(m_pOverlayViewParamsOverrideDef.isNull());
    m_pOverlayViewParamsOverrideDef = new OdTrXmlOverlayViewParamsOverrideDef();
    m_pOverlayViewParamsOverrideDef->setDefault();
    break;
  PK_CASE(ViewportInserted):
    m_nViewportInsertedIndex = 0;
  PK_CASE(ViewportAdded):
    ODA_ASSERT_ONCE(m_pViewportDef.isNull());
    m_pViewportDef = new OdTrVisViewportDef();
    m_pViewportDef->setDefault();
  PK_CASE(ViewportDeleted):
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportViewParamsModified):
  PK_CASE(OverlayViewParamsOverride_ViewParams):
    ODA_ASSERT_ONCE(m_pViewParamsDef.isNull());
    m_pViewParamsDef = new OdTrVisViewParamsDef();
    m_pViewParamsDef->setDefault();
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportLineStylesModified):
    ODA_ASSERT_ONCE(m_pViewportLinestyleDef.isNull());
    m_pViewportLinestyleDef = new OdTrVisViewportLineStyleDef();
    m_pViewportLinestyleDef->setDefault();
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportFadingModified):
    ODA_ASSERT_ONCE(m_pViewportFadingDef.isNull());
    m_pViewportFadingDef = new OdTrVisViewportFadingDef();
    m_pViewportFadingDef->setDefault();
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportLineweightsModified):
    ODA_ASSERT_ONCE(m_pViewportLwdDef.isNull());
    m_pViewportLwdDef = new OdTrVisViewportLineweightsDef();
    m_pViewportLwdDef->setDefault();
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportShadingModified):
    ODA_ASSERT_ONCE(m_pViewportShadingDef.isNull());
    m_pViewportShadingDef = new OdTrVisViewportShadingDef();
    m_pViewportShadingDef->setDefault();
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportClipModified):
    ODA_ASSERT_ONCE(m_pViewportClipDef.isNull());
    m_pViewportClipDef = new OdTrVisViewportClipDef();
    m_pViewportClipDef->setDefault();
    m_id = kTrVisNullId; m_objectChangedFlag = 0;
    break;
  PK_CASE(VisualStyleAdded): PK_CASE(VisualStyleModified):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.isNull() && !m_pVisualStyleProperty && m_typeVisualStyle == OdGiVariant::kUndefined);
    m_pVisualStyleDef = new OdTrVisVisualStyle();
    break;
  PK_CASE(VisualStyleAdded_Prop): PK_CASE(VisualStyleModified_Prop):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.get() && !m_pVisualStyleProperty && m_typeVisualStyle == OdGiVariant::kUndefined);
    break;
  PK_CASE(VisualStyleDeleted):
    m_id = kTrVisNullId;
    break;
  PK_CASE(LayerAdded):
    ODA_ASSERT_ONCE(m_pLayerDef.isNull());
    m_pLayerDef = new OdTrVisLayerDef();
    m_pLayerDef->setDefault();
    break;
  PK_CASE(LayerDeleted):
    ODA_ASSERT_ONCE(m_pLayerDef.isNull());
    m_id = kTrVisNullId;
    break;
  //PK_CASE(LayerAdded_VpDepSpecs_VpDepSpec): PK_CASE(LayerModified_VpDepSpecs_VpDepSpec):
  //  break;
  PK_CASE(MetafileAdded):
    ODA_ASSERT_ONCE(m_pMetafileDef.isNull()); 
    m_pMetafileDef = new OdTrVisMetafileDef(),
    m_pMetafileDef->setDefault();
    break;
  PK_CASE_EX(_MetafileStream):
    if (metafile())
    {
      ODA_ASSERT_ONCE(m_pWriter.isNull());
      m_pWriter = static_cast<OdTrVisFlatMetafileContainerWriter*>(metafile()->createWriter());
      m_pWriter->setContainer(metafile());
      m_streamWriter.setContainerIO(m_pWriter);
    }
    break;
  PK_CASE_EX(_Array_Data):
    if (metafile())
    {
      ODA_ASSERT_ONCE(   m_typeElement == OdTrVisArrayWrapper::Type_Marker
                      && m_indexElement < metafile()->m_ArrayElements.size()
                      && !m_uaMarkerIndex && !m_aMarkers.size()
                      && m_numDataElement);
      m_uaMarkerIndex = 0;
      m_aMarkers.resize(m_numDataElement);
    }
    break;
  PK_CASE_EX(_MetafileStream_UninitTexture):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_UninitTexture);
    break;
  PK_CASE_EX(_MetafileStream_PopMatrix):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_PopMatrix);
    break;
  PK_CASE(SurfaceChanged):
    ODA_ASSERT_ONCE(m_pSurfaceDef.isNull());
    m_pSurfaceDef = new OdTrVisSurfaceDef(),
    m_pSurfaceDef->setDefault();
    break;
  PK_CASE(OverlayExtentsOverride):
    ODA_ASSERT_ONCE(m_pExtentsDef.isNull());
    m_pExtentsDef = new OdTrXmlOverlayExtentsOverrideDef(),
    m_pExtentsDef->setDefault();
    break;
  PK_CASE(MetafileOrderChanged):
    ODA_ASSERT_ONCE(m_pMetafileOrderDef.isNull());
    m_pMetafileOrderDef = new OdTrXmlMetafileOrderDef(),
    m_pMetafileOrderDef->setDefault();
    break;
  PK_CASE(MetafileOrderInheritance):
    ODA_ASSERT_ONCE(m_pMetafileOrderInheritance.isNull());
    m_pMetafileOrderInheritance = new OdTrXmlMetafileOrderInheritanceDef(),
    m_pMetafileOrderInheritance->setDefault();
    break;
  PK_CASE(ViewportBackgroundChanged):
    ODA_ASSERT_ONCE(m_pBackgroundDef.isNull());
    m_pBackgroundDef = new OdTrXmlBackgroundDef(),
    m_pBackgroundDef->setDefault();
    break;
  PK_CASE(ViewportFogChanged):
    ODA_ASSERT_ONCE(m_pFogDef.isNull());
    m_pFogDef = new OdTrVisFogDef(),
    m_pFogDef->setDefault();
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportSectioningChanged):
    ODA_ASSERT_ONCE(m_pSectionDef.isNull());
    m_pSectionDef = new OdTrVisSectionDef(),
    m_pSectionDef->setDefault();
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportSectioningChanged_PlanesList_Plane):
    ODA_ASSERT_ONCE(m_pSectionDef.get());
    m_pSectionDef->m_planes.append();
    break;
  PK_CASE(LightModified):
    ODA_ASSERT_ONCE(m_pLightOverDef.isNull());
    m_pLightOverDef = new OdTrXmlLightOverridesDef();
    m_pLightOverDef->setDefault();
  PK_CASE(LightAdded):
  PK_CASE(ViewportAmbientLightChanged):
    ODA_ASSERT_ONCE(m_pLightDef.isNull());
    m_pLightDef = new OdTrVisLightDef();
    m_pLightDef->setDefault();
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportDefaultLightsChanged):
    ODA_ASSERT_ONCE(m_pLightsListDef.isNull());
    m_pLightsListDef = new OdTrXmlLightsListDef(),
    m_pLightsListDef->setDefault();
    break;
  PK_CASE(ViewportLightAttached):
  PK_CASE(ViewportLightDetached):
    ODA_ASSERT_ONCE(m_pLightOverDef.isNull());
    m_pLightOverDef = new OdTrXmlLightOverridesDef();
    m_pLightOverDef->setDefault();
    m_id = kTrVisNullId;
    break;
  PK_CASE(LightDeleted):
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light):
    ODA_ASSERT_ONCE(m_pLightDef.isNull());
    m_pLightDef = new OdTrVisLightDef();
    m_pLightDef->setDefault();
    break;
  PK_CASE(MetafileDeleted):
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportVisibilityChanged):
  PK_CASE(MetafileVisibilityChanged):
  PK_CASE(MetafileHighlightingChanged):
  PK_CASE(MetafileFadingChanged):
  PK_CASE(LayerVisibilityChanged):
  PK_CASE(LayerFadingChanged):
  PK_CASE(GroupMetafilesVisibilityChanged):
  PK_CASE(GroupMetafilesHighlightingChanged):
  PK_CASE(GroupMetafilesFadingChanged):
  PK_CASE(ExtensionObjectVisibilityChanged):
    m_objectChangedFlag = 0xFF;
    break;
  PK_CASE(StateBranchAdded):
  PK_CASE(StateBranchModified):
    ODA_ASSERT_ONCE(m_pStateBranchDef.isNull()); 
    m_pStateBranchDef = new OdTrVisStateBranchDef();
    break;
  PK_CASE(StateBranchAdded_Childs):
  PK_CASE(StateBranchModified_Childs):
    ODA_ASSERT_ONCE( m_pStateBranchDef.get() );
    break;
  PK_CASE(StateBranchAdded_Markers):
  PK_CASE(StateBranchModified_Markers):
    ODA_ASSERT_ONCE( m_pStateBranchDef.get() );
    break;
  PK_CASE(StateBranchDeleted):
    m_id = kTrVisNullId;
    break;
  PK_CASE(LayerVpDepPropsOverride):
    m_pLayerOverrideDef = new OdTrXmlLayerOverrideDef();
    m_pLayerOverrideDef->setDefault();
    break;
  PK_CASE(GroupAdded):
  PK_CASE(GroupDeleted):
  //PK_CASE(GroupMetafilesDeleted):
  PK_CASE(GroupMetafilesAttached):
  PK_CASE(GroupMetafilesDetached):
    m_id = kTrVisNullId;
    break;
  PK_CASE(GroupMetafilesAttached_Metafiles):
  PK_CASE(GroupMetafilesDetached_Metafiles):
    m_pGroupMetafilesDef = new OdTrXmlGroupMetafilesDef();
    m_pGroupMetafilesDef->setDefault();
    break;
  PK_CASE(ExtensionObjectAdded):
    ODA_ASSERT_ONCE(m_pExtensionObject.isNull());
    m_pExtensionObject = OdRxObjectImpl<OdTrVisExtensionObjectProxy>::createObject();
  PK_CASE(ExtensionObjectDeleted):
    m_id = kTrVisNullId;
    break;
  PK_CASE(ExtensionObjectSpecificData):
    m_mfUserEntryNumData = 0;
  PK_CASE(MetafilePropertyAttached):        PK_CASE(MetafilePropertyDetached):
  PK_CASE(GroupPropertyAttached):           PK_CASE(GroupPropertyDetached):
  PK_CASE(LayerPropertyAttached):           PK_CASE(LayerPropertyDetached):
  PK_CASE(VisualStylePropertyAttached):     PK_CASE(VisualStylePropertyDetached):
  PK_CASE(MaterialPropertyAttached):        PK_CASE(MaterialPropertyDetached):
  PK_CASE(TexturePropertyAttached):         PK_CASE(TexturePropertyDetached):
  PK_CASE(LightPropertyAttached):           PK_CASE(LightPropertyDetached):
  PK_CASE(StateBranchPropertyAttached):     PK_CASE(StateBranchPropertyDetached):
  PK_CASE(ViewportPropertyAttached):        PK_CASE(ViewportPropertyDetached):
  PK_CASE(OverlayPropertyAttached):         PK_CASE(OverlayPropertyDetached):
  PK_CASE(ExtensionObjectPropertyAttached): PK_CASE(ExtensionObjectPropertyDetached):
    m_id = kTrVisNullId;
    m_propertyType = OdTrVisPropertyDef::kNumPropTypes;
    break;
  PK_CASE(MetafilePropertyAttached_PropVal):
  PK_CASE(GroupPropertyAttached_PropVal):
  PK_CASE(LayerPropertyAttached_PropVal):
  PK_CASE(VisualStylePropertyAttached_PropVal):
  PK_CASE(MaterialPropertyAttached_PropVal):
  PK_CASE(TexturePropertyAttached_PropVal):
  PK_CASE(LightPropertyAttached_PropVal):
  PK_CASE(StateBranchPropertyAttached_PropVal):
  PK_CASE(ViewportPropertyAttached_PropVal):
  PK_CASE(OverlayPropertyAttached_PropVal):
  PK_CASE(ExtensionObjectPropertyAttached_PropVal):
  PK_CASE(ExtensionObjectSpecificData_PropVal):
    ODA_ASSERT_ONCE(!m_pPropertyDef.get() && m_propertyType < OdTrVisPropertyDef::kNumPropTypes);
    switch (m_propertyType)
    {
      case OdTrVisPropertyDef::kOwningProperty:
        m_pPropertyDef = new OdTrVisOwningPropertyDef(kTrVisNullId);
      break;
      case OdTrVisPropertyDef::kXformProperty:
        m_pPropertyDef = new OdTrVisXformPropertyDef(OdGeMatrix3d::kIdentity);
      break;
      case OdTrVisPropertyDef::kLayerProperty:
        m_pPropertyDef = new OdTrVisLayerPropertyDef(kTrVisNullId);
      break;
      case OdTrVisPropertyDef::kHlBranchProperty:
      case OdTrVisPropertyDef::kVsBranchProperty:
        m_pPropertyDef = new OdTrVisStateBranchPropertyDef(kTrVisNullId,
          (m_propertyType == OdTrVisPropertyDef::kHlBranchProperty) ? OdTrVisStateBranchPropertyDef::kHl : OdTrVisStateBranchPropertyDef::kVs);
      break;
      case OdTrVisPropertyDef::kVisualStyleProperty:
        m_pPropertyDef = new OdTrVisVisualStylePropertyDef(kTrVisNullId);
      break;
      case OdTrVisPropertyDef::kSelStyleProperty:
        m_pPropertyDef = new OdTrVisSelStylePropertyDef(0);
      break;
      case OdTrVisPropertyDef::kNameProperty:
        m_pPropertyDef = new OdTrVisNamePropertyDef(OdString::kEmpty);
      break;
      case OdTrVisPropertyDef::kSysDefaultProperty:
        m_pPropertyDef = new OdTrVisSysDefaultPropertyDef(false);
      break;
      default:
        ODA_FAIL();
    }
    break;
  PK_CASE(ViewportAntiAliasingChanged):
  PK_CASE(FrameSwitchInfo):
    m_mfUserEntryNumData = 0;
    break;
  }
  return true;
}

bool OdXmlGlesLoader::DropEnd(OdEnPathKey pk)
{
  switch (pk)
  {
  //if (m_pProgramDef.get())
  //{
  //  m_pProgramDef = NULL;
  //  ODA_FAIL_ONCE(); // TODO
  //  return false;
  //}
  PK_CASE(MaterialAdded):
    ODA_ASSERT_ONCE(m_pMaterialDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onMaterialAdded((OdTrVisMetafileId) m_id, *m_pMaterialDef.get());
    m_id = kTrVisNullId;
    m_pMaterialDef = NULL;
    break;
  PK_CASE(MaterialDeleted):
    ODA_ASSERT_ONCE( m_id > OdTrVisId(100));
    m_pRendition->onMaterialDeleted( m_id );
    m_id = kTrVisNullId;
    break;
  PK_CASE(TextureAdded):
    ODA_ASSERT_ONCE(m_pTextureDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onTextureAdded((OdTrVisTextureId) m_id, *m_pTextureDef.get());
    m_id = kTrVisNullId;
    m_pTextureDef = NULL;
    break;
  PK_CASE(TextureDeleted):
    ODA_ASSERT_ONCE( m_id > OdTrVisId(100));
    m_pRendition->onTextureDeleted( m_id );
    m_id = kTrVisNullId;
    break;
  PK_CASE(OverlayAdded):
    ODA_ASSERT_ONCE(m_pOverlayDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onOverlayAdded((OdTrVisOverlayId) m_id, *m_pOverlayDef.get());
    m_id = kTrVisNullId;
    m_pOverlayDef = NULL;
    break;
  PK_CASE(OverlayDeleted):
    ODA_ASSERT_ONCE(m_pOverlayDef.isNull() && m_id > OdTrVisId(100));
    m_pRendition->onOverlayDeleted((OdTrVisOverlayId) m_id);
    m_id = kTrVisNullId;
    break;
  PK_CASE(OverlayVisibilityChanged):
    ODA_ASSERT_ONCE(m_pOverlayDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onOverlayVisibilityChanged((OdTrVisOverlayId) m_id, m_pOverlayDef->isOverlayEnabled());
    m_id = kTrVisNullId;
    m_pOverlayDef = NULL;
    break;
  PK_CASE(OverlayRenderOrderChanged):
    ODA_ASSERT_ONCE(m_pOverlayDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onOverlayRenderOrderChanged((OdTrVisOverlayId) m_id, m_pOverlayDef->m_renderOrder);
    m_id = kTrVisNullId;
    m_pOverlayDef = NULL;
    break;
  PK_CASE(OverlayViewParamsOverride):
    ODA_ASSERT_ONCE(m_pOverlayViewParamsOverrideDef.get());
    m_pRendition->onOverlayViewParamsOverride(m_pOverlayViewParamsOverrideDef->m_viewportId, m_pOverlayViewParamsOverrideDef->m_overlayId,
                                              m_pOverlayViewParamsOverrideDef->m_bOverride, m_pOverlayViewParamsOverrideDef->m_viewParams);
    m_pOverlayViewParamsOverrideDef = NULL;
    break;
  PK_CASE(ViewportAdded):
    ODA_ASSERT_ONCE(m_pViewportDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onViewportAdded((OdTrVisViewportId)m_id, *m_pViewportDef);
    m_pViewportDef = NULL;
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportInserted):
    ODA_ASSERT_ONCE(m_pViewportDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onViewportInserted(m_id, m_nViewportInsertedIndex, *m_pViewportDef);
    m_pViewportDef = NULL;
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportDeleted):
    ODA_ASSERT_ONCE( m_id > OdTrVisId(100) );
    m_pRendition->onViewportDeleted( m_id );
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportVisibilityChanged):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_objectChangedFlag <= 1);
    m_pRendition->onViewportVisibilityChanged((OdTrVisViewportId)m_id, !!m_objectChangedFlag);
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportAntiAliasingChanged):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100));
    m_pRendition->onViewportAntiAliasingChanged((OdTrVisViewportId)m_id, m_mfUserEntryNumData);
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportViewParamsModified):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onViewportViewParamsModified((OdTrVisViewportId)m_id, *m_pViewParamsDef);
    m_pViewParamsDef = NULL;
    m_id = kTrVisNullId;
    break;
  PK_CASE(OverlayViewParamsOverride_ViewParams):
    ODA_ASSERT_ONCE(m_pOverlayViewParamsOverrideDef.get() && m_pViewParamsDef.get());
    m_pOverlayViewParamsOverrideDef->m_viewParams = *m_pViewParamsDef;
    m_pViewParamsDef = NULL;
    break;
  PK_CASE(ViewportLineStylesModified):
    ODA_ASSERT_ONCE(m_pViewportLinestyleDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onViewportLineStylesModified((OdTrVisViewportId)m_id, *m_pViewportLinestyleDef);
    m_pViewportLinestyleDef = NULL;
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportFadingModified):
    ODA_ASSERT_ONCE(m_pViewportFadingDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onViewportFadingModified((OdTrVisViewportId)m_id, *m_pViewportFadingDef);
    m_pViewportFadingDef = NULL;
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportLineweightsModified):
    ODA_ASSERT_ONCE(m_pViewportLwdDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onViewportLineweightsModified((OdTrVisViewportId)m_id, *m_pViewportLwdDef);
    m_pViewportLwdDef = NULL;
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportShadingModified):
    ODA_ASSERT_ONCE(m_pViewportShadingDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onViewportShadingModified((OdTrVisViewportId)m_id, *m_pViewportShadingDef);
    m_pViewportShadingDef = NULL;
    m_id = kTrVisNullId;
    break;
  PK_CASE(ViewportClipModified):
    ODA_ASSERT_ONCE(m_pViewportClipDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onViewportClipModified((OdTrVisViewportId)m_id, *m_pViewportClipDef, GETBIT(m_objectChangedFlag, 1), GETBIT(m_objectChangedFlag, 2));
    m_pViewportClipDef = NULL;
    m_id = kTrVisNullId; m_objectChangedFlag = 0xFF;
    break;
  PK_CASE(VisualStyleAdded):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && m_id > OdTrVisId(100) 
                    && !m_pVisualStyleProperty && m_typeVisualStyle == OdGiVariant::kUndefined);
    m_pRendition->onVisualStyleAdded((OdTrVisVisualStyleId) m_id, *m_pVisualStyleDef.get());
    m_id = kTrVisNullId;
    m_pVisualStyleDef = NULL; 
    break;
  PK_CASE(VisualStyleModified):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && m_id > OdTrVisId(100) 
                    && !m_pVisualStyleProperty && m_typeVisualStyle == OdGiVariant::kUndefined);
    m_pRendition->onVisualStyleModified((OdTrVisVisualStyleId) m_id, *m_pVisualStyleDef.get());
    m_id = kTrVisNullId;
    m_pVisualStyleDef = NULL; 
    break;
  PK_CASE(VisualStyleAdded_Prop): PK_CASE(VisualStyleModified_Prop):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.get() && m_pVisualStyleProperty && m_typeVisualStyle != OdGiVariant::kUndefined);
    m_pVisualStyleProperty->setModified(true);
    m_pVisualStyleProperty = NULL;
    m_typeVisualStyle = OdGiVariant::kUndefined;
    break;
  PK_CASE(VisualStyleDeleted):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.isNull() && m_id > OdTrVisId(100));
    m_pRendition->onVisualStyleDeleted((OdTrVisVisualStyleId) m_id);
    m_id = kTrVisNullId;
    break;
  PK_CASE(LayerAdded):
    ODA_ASSERT_ONCE(m_pLayerDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onLayerAdded((OdTrVisLayerId) m_id, *m_pLayerDef.get());
    m_id = kTrVisNullId;
    m_pLayerDef = NULL; 
    break;
  PK_CASE(LayerDeleted):
    ODA_ASSERT_ONCE(m_pLayerDef.isNull() && m_id > OdTrVisId(100));
    m_pRendition->onLayerDeleted((OdTrVisLayerId) m_id);
    m_id = kTrVisNullId;
    break;
  PK_CASE(LayerVisibilityChanged):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_objectChangedFlag <= 1);
    m_pRendition->onLayerVisibilityChanged((OdTrVisLayerId)m_id, !!m_objectChangedFlag);
    break;
  PK_CASE(LayerFadingChanged):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_objectChangedFlag <= 1);
    m_pRendition->onLayerFadingChanged((OdTrVisLayerId)m_id, !!m_objectChangedFlag);
    break;
  PK_CASE(MetafileAdded):
    ODA_ASSERT_ONCE(m_pMetafileDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onMetafileAdded((OdTrVisMetafileId) m_id, *m_pMetafileDef.get());
    m_id = kTrVisNullId;
    m_pMetafileDef = NULL;
    break;
  PK_CASE_EX(_Array_Data):
    if (metafile())
    {
      ODA_ASSERT_ONCE(   m_typeElement == OdTrVisArrayWrapper::Type_Marker
                      && m_indexElement < metafile()->m_ArrayElements.size()
                      && m_aMarkers.size() == m_numDataElement
                      && m_uaMarkerIndex == m_numDataElement); 
      OdTrVisArrayWrapper& array = metafile()->m_ArrayElements[m_indexElement++];
      array.setArray(m_typeElement, m_aMarkers.getPtr(), m_aMarkers.size() * sizeof(OdTrVisDefProcMark));
      m_uaMarkerIndex = 0;
      m_aMarkers.resize(0);
      m_typeElement = OdTrVisArrayWrapper::Type_Invalid;
      m_numDataElement = 0;
    }
    break;
  PK_CASE_EX(_MetafileStream):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.setContainerIO(NULL);
    m_pWriter->completeWriting();
    m_pWriter = NULL;
    break;
  PK_CASE(SurfaceChanged):
    ODA_ASSERT_ONCE(m_pSurfaceDef.get());
    m_pRendition->onSurfaceChanged(*m_pSurfaceDef.get());
    m_id = kTrVisNullId;
    m_pSurfaceDef = NULL;
    break;
  PK_CASE(SurfaceInvalidated):
    m_pRendition->onSurfaceInvalidated();
    break;
  PK_CASE(OverlayExtentsOverride):
    ODA_ASSERT_ONCE(m_pExtentsDef.get());
    m_pRendition->onOverlayExtentsOverride(m_pExtentsDef->m_viewportId, m_pExtentsDef->m_overlayId, *m_pExtentsDef);
    m_pExtentsDef = NULL;
    break;
  PK_CASE(MetafileOrderChanged):
    ODA_ASSERT_ONCE(m_pMetafileOrderDef.get());
    m_pRendition->onMetafileOrderChanged(m_pMetafileOrderDef->m_viewportId,
                                         m_pMetafileOrderDef->m_overlayId,
                                         m_pMetafileOrderDef->m_nInvalidFrom,
                                         m_pMetafileOrderDef->m_nInvalidLen,
                                         m_pMetafileOrderDef->m_metafileOrder.getPtr(),
                                         m_pMetafileOrderDef->m_metafileOrder.size());
    m_pMetafileOrderDef = NULL;
    break;
  PK_CASE(MetafileOrderInheritance):
    ODA_ASSERT_ONCE(m_pMetafileOrderInheritance.get());
    m_pRendition->onMetafileOrderInheritance( m_pMetafileOrderInheritance->m_viewportId1,
                                              m_pMetafileOrderInheritance->m_overlayId,
                                              m_pMetafileOrderInheritance->m_viewportId2 );
    m_pMetafileOrderInheritance = NULL;
    break;
  PK_CASE(ViewportBackgroundChanged):
    ODA_ASSERT_ONCE(m_pBackgroundDef.get());
    m_pRendition->onViewportBackgroundChanged(m_pBackgroundDef->m_viewportId, m_pBackgroundDef->m_pStream, m_pBackgroundDef->m_baseProgramId);
    m_pBackgroundDef = NULL;
    break;
  PK_CASE(ViewportFogChanged):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pFogDef.get());
    m_pRendition->onViewportFogChanged(m_id, *m_pFogDef);
    m_pFogDef = NULL;
    break;
  PK_CASE(ViewportSectioningChanged):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pSectionDef.get());
    m_pRendition->onViewportSectioningChanged(m_id, *m_pSectionDef);
    m_pSectionDef = NULL;
    break;
  PK_CASE(ViewportAmbientLightChanged):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pLightDef.get());
    m_pRendition->onViewportAmbientLightChanged(m_id, m_pLightDef->m_color);
    m_pLightDef = NULL;
    break;
  PK_CASE(ViewportDefaultLightsChanged):
    ODA_ASSERT_ONCE(m_pLightsListDef.get());
    m_pRendition->onViewportDefaultLightsChanged(m_pLightsListDef->m_viewportId, m_pLightsListDef->m_bDefaultLightsOn,
                                                 m_pLightsListDef->m_Defs.getPtr(), m_pLightsListDef->m_Defs.size());
    m_pLightsListDef = NULL;
    break;
  PK_CASE(ViewportLightAttached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pLightOverDef.get());
    m_pRendition->onViewportLightAttached(m_id, m_pLightOverDef->m_lightId, m_pLightOverDef->m_nKindOfMod, (!m_pLightDef.isNull()) ? *m_pLightDef : OdTrVisLightDef());
    m_pLightOverDef = NULL; m_pLightDef = NULL;
    break;
  PK_CASE(ViewportLightDetached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pLightOverDef.get());
    m_pRendition->onViewportLightDetached(m_id, m_pLightOverDef->m_lightId);
    m_pLightOverDef = NULL;
    break;
  PK_CASE(LightAdded):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pLightDef.get());
    m_pRendition->onLightAdded(m_id, *m_pLightDef);
    m_pLightDef = NULL;
    break;
  PK_CASE(LightModified):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pLightDef.get() && m_pLightOverDef.get());
    m_pRendition->onLightModified(m_id, m_pLightOverDef->m_nKindOfMod, *m_pLightDef);
    m_pLightOverDef = NULL; m_pLightDef = NULL;
    break;
  PK_CASE(LightDeleted):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100));
    m_pRendition->onLightDeleted(m_id);
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light):
    ODA_ASSERT_ONCE(m_pLightsListDef.get() && m_pLightDef.get());
    m_pLightsListDef->m_Defs.push_back(*m_pLightDef);
    m_pLightDef = NULL;
    break;
  PK_CASE(MetafileDeleted):
    ODA_ASSERT_ONCE(!m_pMetafileDef.get() && m_id > OdTrVisId(100));
    m_pRendition->onMetafileDeleted((OdTrVisMetafileId) m_id);
    m_id = kTrVisNullId;
    break;
  PK_CASE(MetafileVisibilityChanged):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_objectChangedFlag <= 1);
    m_pRendition->onMetafileVisibilityChanged((OdTrVisMetafileId)m_id, !!m_objectChangedFlag);
    break;
  PK_CASE(MetafileHighlightingChanged):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_objectChangedFlag <= 1);
    m_pRendition->onMetafileHighlightingChanged((OdTrVisMetafileId)m_id, !!m_objectChangedFlag);
    break;
  PK_CASE(MetafileFadingChanged):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_objectChangedFlag <= 1);
    m_pRendition->onMetafileFadingChanged((OdTrVisMetafileId)m_id, !!m_objectChangedFlag);
    break;
  PK_CASE(StateBranchAdded):
    ODA_ASSERT_ONCE( m_pStateBranchDef.get() && m_id > OdTrVisId(100) ); 
    m_pRendition->onStateBranchAdded( m_id, *m_pStateBranchDef.get() );
    m_pStateBranchDef = NULL;
    break;
  PK_CASE(StateBranchAdded_Childs):
  PK_CASE(StateBranchAdded_Markers):
  PK_CASE(StateBranchModified_Childs):
  PK_CASE(StateBranchModified_Markers):
    ODA_ASSERT_ONCE( m_pStateBranchDef.get() && m_id > OdTrVisId(100) );
    break;
  PK_CASE(StateBranchModified):
    ODA_ASSERT_ONCE( m_pStateBranchDef.get() && m_id > OdTrVisId(100) );
    m_pRendition->onStateBranchModified( m_id, *m_pStateBranchDef.get(), !m_pStateBranchDef->m_childs.empty(), !m_pStateBranchDef->m_markers.empty() );
    m_pStateBranchDef = NULL;
    break;
  PK_CASE( StateBranchDeleted ):
    ODA_ASSERT_ONCE( m_id > OdTrVisId(100) ); 
    m_pRendition->onStateBranchDeleted( m_id );
    break;
  PK_CASE(LayerVpDepPropsOverride):
    ODA_ASSERT_ONCE( m_pLayerOverrideDef.get() && m_id > OdTrVisId(100) && m_pLayerOverrideDef->m_overrideViewportId > OdTrVisId(100) );
    m_pRendition->onLayerVpDepPropsOverride(m_id, m_pLayerOverrideDef->m_overrideViewportId,
                                            m_pLayerOverrideDef->m_bOverrideLayer, m_pLayerOverrideDef->m_overrideLayerProps);
    m_pLayerOverrideDef = NULL;
    break;
  PK_CASE(GroupAdded):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100));
    m_pRendition->onGroupAdded(m_id);
    break;
  PK_CASE(GroupDeleted):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100));
    m_pRendition->onGroupDeleted(m_id);
    break;
  PK_CASE(GroupMetafilesAttached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100));
    m_pRendition->onGroupMetafilesAttached(m_id, (!m_pGroupMetafilesDef.isNull()) ? m_pGroupMetafilesDef->m_metafiles.getPtr() : NULL,
                                                 (!m_pGroupMetafilesDef.isNull()) ? m_pGroupMetafilesDef->m_metafiles.size() : 0);
    m_pGroupMetafilesDef = NULL;
    break;
  PK_CASE(GroupMetafilesDetached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100));
    m_pRendition->onGroupMetafilesDetached(m_id, (!m_pGroupMetafilesDef.isNull()) ? m_pGroupMetafilesDef->m_metafiles.getPtr() : NULL,
                                                 (!m_pGroupMetafilesDef.isNull()) ? m_pGroupMetafilesDef->m_metafiles.size() : 0);
    m_pGroupMetafilesDef = NULL;
    break;
  //PK_CASE(GroupMetafilesDeleted):
  //  ODA_ASSERT_ONCE(m_id > OdTrVisId(100));
  //  m_pRendition->onGroupMetafilesDeleted(m_id);
  //  break;
  PK_CASE(GroupMetafilesVisibilityChanged):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_objectChangedFlag <= 1);
    m_pRendition->onGroupMetafilesVisibilityChanged((OdTrVisMetafileId)m_id, !!m_objectChangedFlag);
    break;
  PK_CASE(GroupMetafilesHighlightingChanged):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_objectChangedFlag <= 1);
    m_pRendition->onGroupMetafilesHighlightingChanged((OdTrVisMetafileId)m_id, !!m_objectChangedFlag);
    break;
  PK_CASE(GroupMetafilesFadingChanged):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_objectChangedFlag <= 1);
    m_pRendition->onGroupMetafilesFadingChanged((OdTrVisMetafileId)m_id, !!m_objectChangedFlag);
    break;
  PK_CASE(ExtensionObjectAdded):
    ODA_ASSERT_ONCE(m_pExtensionObject.get() && m_id > OdTrVisId(100));
    m_pRendition->onExtensionObjectAdded((OdTrVisExtensionId)m_id, m_pExtensionObject.get());
    m_id = kTrVisNullId;
    m_pExtensionObject.release();
    break;
  PK_CASE(ExtensionObjectDeleted):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100));
    m_pRendition->onExtensionObjectDeleted(m_id);
    m_id = kTrVisNullId;
    break;
  PK_CASE(ExtensionObjectVisibilityChanged):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_objectChangedFlag <= 1);
    m_pRendition->onExtensionObjectVisibilityChanged((OdTrVisMetafileId)m_id, !!m_objectChangedFlag);
    break;
  PK_CASE(MetafilePropertyAttached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pPropertyDef.get());
    m_pRendition->onMetafilePropertyAttached(m_id, *m_pPropertyDef);
    m_pPropertyDef = NULL;
    break;
  PK_CASE(MetafilePropertyDetached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_propertyType < OdTrVisPropertyDef::kNumPropTypes);
    m_pRendition->onMetafilePropertyDetached(m_id, m_propertyType);
    break;
  PK_CASE(GroupPropertyAttached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pPropertyDef.get());
    m_pRendition->onGroupPropertyAttached(m_id, *m_pPropertyDef);
    m_pPropertyDef = NULL;
    break;
  PK_CASE(GroupPropertyDetached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_propertyType < OdTrVisPropertyDef::kNumPropTypes);
    m_pRendition->onGroupPropertyDetached(m_id, m_propertyType);
    break;
  PK_CASE(LayerPropertyAttached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pPropertyDef.get());
    m_pRendition->onLayerPropertyAttached(m_id, *m_pPropertyDef);
    m_pPropertyDef = NULL;
    break;
  PK_CASE(LayerPropertyDetached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_propertyType < OdTrVisPropertyDef::kNumPropTypes);
    m_pRendition->onLayerPropertyDetached(m_id, m_propertyType);
    break;
  PK_CASE(VisualStylePropertyAttached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pPropertyDef.get());
    m_pRendition->onVisualStylePropertyAttached(m_id, *m_pPropertyDef);
    m_pPropertyDef = NULL;
    break;
  PK_CASE(VisualStylePropertyDetached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_propertyType < OdTrVisPropertyDef::kNumPropTypes);
    m_pRendition->onVisualStylePropertyDetached(m_id, m_propertyType);
    break;
  PK_CASE(MaterialPropertyAttached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pPropertyDef.get());
    m_pRendition->onMaterialPropertyAttached(m_id, *m_pPropertyDef);
    m_pPropertyDef = NULL;
    break;
  PK_CASE(MaterialPropertyDetached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_propertyType < OdTrVisPropertyDef::kNumPropTypes);
    m_pRendition->onMaterialPropertyDetached(m_id, m_propertyType);
    break;
  PK_CASE(TexturePropertyAttached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pPropertyDef.get());
    m_pRendition->onTexturePropertyAttached(m_id, *m_pPropertyDef);
    m_pPropertyDef = NULL;
    break;
  PK_CASE(TexturePropertyDetached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_propertyType < OdTrVisPropertyDef::kNumPropTypes);
    m_pRendition->onTexturePropertyDetached(m_id, m_propertyType);
    break;
  PK_CASE(LightPropertyAttached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pPropertyDef.get());
    m_pRendition->onLightPropertyAttached(m_id, *m_pPropertyDef);
    m_pPropertyDef = NULL;
    break;
  PK_CASE(LightPropertyDetached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_propertyType < OdTrVisPropertyDef::kNumPropTypes);
    m_pRendition->onLightPropertyDetached(m_id, m_propertyType);
    break;
  PK_CASE(StateBranchPropertyAttached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pPropertyDef.get());
    m_pRendition->onStateBranchPropertyAttached(m_id, *m_pPropertyDef);
    m_pPropertyDef = NULL;
    break;
  PK_CASE(StateBranchPropertyDetached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_propertyType < OdTrVisPropertyDef::kNumPropTypes);
    m_pRendition->onStateBranchPropertyDetached(m_id, m_propertyType);
    break;
  PK_CASE(ViewportPropertyAttached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pPropertyDef.get());
    m_pRendition->onViewportPropertyAttached(m_id, *m_pPropertyDef);
    m_pPropertyDef = NULL;
    break;
  PK_CASE(ViewportPropertyDetached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_propertyType < OdTrVisPropertyDef::kNumPropTypes);
    m_pRendition->onViewportPropertyDetached(m_id, m_propertyType);
    break;
  PK_CASE(OverlayPropertyAttached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pPropertyDef.get());
    m_pRendition->onOverlayPropertyAttached(m_id, *m_pPropertyDef);
    m_pPropertyDef = NULL;
    break;
  PK_CASE(OverlayPropertyDetached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_propertyType < OdTrVisPropertyDef::kNumPropTypes);
    m_pRendition->onOverlayPropertyDetached(m_id, m_propertyType);
    break;
  PK_CASE(ExtensionObjectPropertyAttached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pPropertyDef.get());
    m_pRendition->onExtensionObjectPropertyAttached(m_id, *m_pPropertyDef);
    m_pPropertyDef = NULL;
    break;
  PK_CASE(ExtensionObjectPropertyDetached):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_propertyType < OdTrVisPropertyDef::kNumPropTypes);
    m_pRendition->onExtensionObjectPropertyDetached(m_id, m_propertyType);
    break;
  PK_CASE(ExtensionObjectSpecificData):
    ODA_ASSERT_ONCE(m_id > OdTrVisId(100) && m_pPropertyDef.get());
    m_pRendition->onExtensionObjectSpecificData(m_id, m_mfUserEntryNumData, *m_pPropertyDef);
    m_pPropertyDef = NULL;
    break;
  PK_CASE(FrameSwitchInfo):
    m_pRendition->onFrameSwitchInfo(m_mfUserEntryNumData);
    break;
  }

  return true;
}

bool OdXmlGlesLoader::DropId(OdEnPathKey pk, const OdAnsiString& asValue)
{
  ODA_ASSERT_ONCE(!asValue.isEmpty());

  // unfortunately asValue canbe same for kTextureData & kMaterialData //OdAnsiString asKey = asValue;
  OdAnsiString asKey;
  OdTrVisRendition::DataType typeToRegId = (enum  OdTrVisRendition::DataType) -1; // undef
  switch (pk)
  {
  PK_CASE(TextureAdded_TextureID):
  PK_CASE(MaterialAdded_AmbientChannel_RealisticComponent_TextureID):
  PK_CASE(MaterialAdded_AmbientChannel_ShadingComponent_TextureID):
  PK_CASE(MaterialAdded_DiffuseChannel_RealisticComponent_TextureID):
  PK_CASE(MaterialAdded_DiffuseChannel_ShadingComponent_TextureID):
  PK_CASE(MaterialAdded_SpecularChannel_RealisticComponent_TextureID):
  PK_CASE(MaterialAdded_SpecularChannel_ShadingComponent_TextureID):
  PK_CASE(MaterialAdded_EmissionChannel_RealisticComponent_TextureID):
  PK_CASE(MaterialAdded_EmissionChannel_ShadingComponent_TextureID):
  PK_CASE(MaterialAdded_OpacityChannel_RealisticComponent_TextureID):
  PK_CASE(MaterialAdded_OpacityChannel_ShadingComponent_TextureID):
  PK_CASE(MaterialAdded_BumpMapChannel_RealisticComponent_TextureID):
  PK_CASE(MaterialAdded_BumpMapChannel_ShadingComponent_TextureID):
  PK_CASE(MaterialAdded_ReflectionChannel_RealisticComponent_TextureID):
  PK_CASE(MaterialAdded_ReflectionChannel_ShadingComponent_TextureID):
  PK_CASE(MaterialAdded_RefractionChannel_RealisticComponent_TextureID):
  PK_CASE(MaterialAdded_RefractionChannel_ShadingComponent_TextureID):
  PK_CASE(MaterialAdded_NormalMapChannel_RealisticComponent_TextureID):
  PK_CASE(MaterialAdded_NormalMapChannel_ShadingComponent_TextureID):
  PK_CASE(MaterialAdded_TintChannel_RealisticComponent_TextureID):
  PK_CASE(MaterialAdded_TintChannel_ShadingComponent_TextureID):
  PK_CASE(TextureDeleted_TextureID):
  PK_CASE(TexturePropertyAttached_TextureID):
  PK_CASE(TexturePropertyDetached_TextureID):
  PK_CASE_EX(_MetafileStream_InitTexture_TextureID):
    asKey = "textureid"; 
    typeToRegId = OdTrVisRendition::kTextureData;
    break;
  PK_CASE(MaterialAdded_MaterialID):
  PK_CASE(MaterialDeleted_MaterialID):
  PK_CASE(MaterialPropertyAttached_MaterialID):
  PK_CASE(MaterialPropertyDetached_MaterialID):
  PK_CASE_EX(_MetafileStream_Material_MaterialID):
    asKey = "materialid"; 
    typeToRegId = OdTrVisRendition::kMaterialData;
    break;
  PK_CASE(MetafileAdded_MetafileID):
  PK_CASE_EX(_MetafileStream_TtfText_Text_Char_MetafileID):
  PK_CASE_EX(_MetafileStream_Metafile_MetafileID):
  PK_CASE_EX(_MetafileStream_MetafileMarker_Marker):
  PK_CASE(MetafileOrderChanged_Ordering_MetafileID):
  PK_CASE(MetafileHighlightingChanged_MetafileID):
  PK_CASE(MetafileVisibilityChanged_MetafileID):
  PK_CASE(MetafileFadingChanged_MetafileID):
  PK_CASE(MetafileDeleted_MetafileID):
  PK_CASE(GroupMetafilesAttached_Metafiles_MetafileID):
  PK_CASE(GroupMetafilesDetached_Metafiles_MetafileID):
  PK_CASE(MetafilePropertyAttached_MetafileID):
  PK_CASE(MetafilePropertyDetached_MetafileID):
    asKey = "metafileid"; 
    typeToRegId = OdTrVisRendition::kMetafileData;
    break;
  PK_CASE(MetafileAdded_GeneratorID):
  PK_CASE(ViewportAdded_ViewportID): PK_CASE(ViewportAdded_OverallVpId):
  PK_CASE(ViewportInserted_ViewportID): PK_CASE(ViewportInserted_OverallVpId):
  PK_CASE(ViewportDeleted_ViewportID):
  PK_CASE(ViewportVisibilityChanged_ViewportID):
  PK_CASE(ViewportAntiAliasingChanged_ViewportID):
  PK_CASE(ViewportViewParamsModified_ViewportID):
  PK_CASE(ViewportLineStylesModified_ViewportID):
  PK_CASE(ViewportFadingModified_ViewportID):
  PK_CASE(ViewportLineweightsModified_ViewportID):
  PK_CASE(ViewportShadingModified_ViewportID):
  PK_CASE(ViewportClipModified_ViewportID):
  PK_CASE(MetafileOrderChanged_ViewportID):
  PK_CASE(MetafileOrderInheritance_ViewportID1):
  PK_CASE(MetafileOrderInheritance_ViewportID2):
  PK_CASE(OverlayExtentsOverride_ViewportID):
  PK_CASE(ViewportBackgroundChanged_ViewportID):
  PK_CASE(ViewportFogChanged_ViewportID):
  PK_CASE(ViewportSectioningChanged_ViewportID):
  PK_CASE(ViewportAmbientLightChanged_ViewportID):
  PK_CASE(ViewportDefaultLightsChanged_ViewportID):
  PK_CASE(ViewportLightAttached_ViewportID):
  PK_CASE(ViewportLightDetached_ViewportID):
  PK_CASE(OverlayViewParamsOverride_ViewportID):
  PK_CASE(LayerVpDepPropsOverride_ViewportID):
  PK_CASE(ViewportPropertyAttached_ViewportID):
  PK_CASE(ViewportPropertyDetached_ViewportID):
    asKey = "viewportid";
    typeToRegId = OdTrVisRendition::kViewportData;
    break;
  PK_CASE(ViewportShadingModified_VisualStyleId):
  PK_CASE(VisualStyleAdded_VisualStyleID): PK_CASE(VisualStyleModified_VisualStyleID): 
  PK_CASE(VisualStyleDeleted_VisualStyleID):
  PK_CASE(VisualStylePropertyAttached_VisualStyleID):
  PK_CASE(VisualStylePropertyDetached_VisualStyleID):
  PK_CASE(MetafilePropertyAttached_PropVal_VisualStyleID):
  PK_CASE(GroupPropertyAttached_PropVal_VisualStyleID):
  PK_CASE(LayerPropertyAttached_PropVal_VisualStyleID):
  PK_CASE(VisualStylePropertyAttached_PropVal_VisualStyleID):
  PK_CASE(MaterialPropertyAttached_PropVal_VisualStyleID):
  PK_CASE(TexturePropertyAttached_PropVal_VisualStyleID):
  PK_CASE(LightPropertyAttached_PropVal_VisualStyleID):
  PK_CASE(StateBranchPropertyAttached_PropVal_VisualStyleID):
  PK_CASE(ViewportPropertyAttached_PropVal_VisualStyleID):
  PK_CASE(OverlayPropertyAttached_PropVal_VisualStyleID):
  PK_CASE(ExtensionObjectPropertyAttached_PropVal_VisualStyleID):
  PK_CASE(ExtensionObjectSpecificData_PropVal_VisualStyleID):
  PK_CASE_EX( _MetafileStream_VisualStyle_VisualStyleID ):
    asKey = "visualstyleid";
    typeToRegId = OdTrVisRendition::kVisualStyleData;
    break;
  PK_CASE(LayerAdded_LayerID): PK_CASE(LayerDeleted_LayerID):
  PK_CASE(LayerVisibilityChanged_LayerID):
  PK_CASE(LayerFadingChanged_LayerID):
  PK_CASE(LayerVpDepPropsOverride_LayerID):
  PK_CASE(LayerPropertyAttached_LayerID):
  PK_CASE(LayerPropertyDetached_LayerID):
  PK_CASE(MetafilePropertyAttached_PropVal_LayerID):
  PK_CASE(GroupPropertyAttached_PropVal_LayerID):
  PK_CASE(LayerPropertyAttached_PropVal_LayerID):
  PK_CASE(VisualStylePropertyAttached_PropVal_LayerID):
  PK_CASE(MaterialPropertyAttached_PropVal_LayerID):
  PK_CASE(TexturePropertyAttached_PropVal_LayerID):
  PK_CASE(LightPropertyAttached_PropVal_LayerID):
  PK_CASE(StateBranchPropertyAttached_PropVal_LayerID):
  PK_CASE(ViewportPropertyAttached_PropVal_LayerID):
  PK_CASE(OverlayPropertyAttached_PropVal_LayerID):
  PK_CASE(ExtensionObjectPropertyAttached_PropVal_LayerID):
  PK_CASE(ExtensionObjectSpecificData_PropVal_LayerID):
    asKey = "layerid";
    typeToRegId = OdTrVisRendition::kLayerData;
    break;
  PK_CASE(OverlayExtentsOverride_OverlayID):
  PK_CASE(MetafileOrderChanged_OverlayID):
  PK_CASE(OverlayAdded_OverlayID):
  PK_CASE(OverlayDeleted_OverlayID):
  PK_CASE(OverlayVisibilityChanged_OverlayID):
  PK_CASE(OverlayRenderOrderChanged_OverlayID):
  PK_CASE(OverlayViewParamsOverride_OverlayID):
  PK_CASE(MetafileOrderInheritance_OverlayID):
  PK_CASE(OverlayPropertyAttached_OverlayID):
  PK_CASE(OverlayPropertyDetached_OverlayID):
    asKey = "overlayid";
    typeToRegId = OdTrVisRendition::kOverlayData;
    break;
  PK_CASE(ViewportLightAttached_LightID):
  PK_CASE(ViewportLightDetached_LightID):
  PK_CASE(LightAdded_LightID):
  PK_CASE(LightModified_LightID):
  PK_CASE(LightDeleted_LightID):
  PK_CASE(LightPropertyAttached_LightID):
  PK_CASE(LightPropertyDetached_LightID):
    asKey = "lightid";
    typeToRegId = OdTrVisRendition::kLightData;
    break;
  PK_CASE(StateBranchAdded_StateBranchID):
  PK_CASE(StateBranchAdded_Childs_StateBranchID):
  PK_CASE(StateBranchModified_StateBranchID):
  PK_CASE(StateBranchModified_Childs_StateBranchID):
  PK_CASE(StateBranchDeleted_StateBranchID):
  PK_CASE(StateBranchPropertyAttached_StateBranchID):
  PK_CASE(StateBranchPropertyDetached_StateBranchID):
  PK_CASE(MetafilePropertyAttached_PropVal_StateBranchID):
  PK_CASE(GroupPropertyAttached_PropVal_StateBranchID):
  PK_CASE(LayerPropertyAttached_PropVal_StateBranchID):
  PK_CASE(VisualStylePropertyAttached_PropVal_StateBranchID):
  PK_CASE(MaterialPropertyAttached_PropVal_StateBranchID):
  PK_CASE(TexturePropertyAttached_PropVal_StateBranchID):
  PK_CASE(LightPropertyAttached_PropVal_StateBranchID):
  PK_CASE(StateBranchPropertyAttached_PropVal_StateBranchID):
  PK_CASE(ViewportPropertyAttached_PropVal_StateBranchID):
  PK_CASE(OverlayPropertyAttached_PropVal_StateBranchID):
  PK_CASE(ExtensionObjectPropertyAttached_PropVal_StateBranchID):
  PK_CASE(ExtensionObjectSpecificData_PropVal_StateBranchID):
    asKey = "statebranchid";
    typeToRegId = OdTrVisRendition::kStateBranchData;
    break;
  PK_CASE(ViewportBackgroundChanged_BaseProgramID):
  PK_CASE(ViewportShadingModified_ShaderId):
  PK_CASE_EX(_MetafileStream_Program_ProgramID):
    asKey = "programid";
    typeToRegId = OdTrVisRendition::kProgramData;
    break;
  PK_CASE(GroupAdded_GroupID):
  PK_CASE(GroupDeleted_GroupID):
  PK_CASE(GroupMetafilesAttached_GroupID):
  PK_CASE(GroupMetafilesDetached_GroupID):
  //PK_CASE(GroupMetafilesDeleted_GroupID):
  PK_CASE(GroupMetafilesVisibilityChanged_GroupID):
  PK_CASE(GroupMetafilesHighlightingChanged_GroupID):
  PK_CASE(GroupMetafilesFadingChanged_GroupID):
  PK_CASE(GroupPropertyAttached_GroupID):
  PK_CASE(GroupPropertyDetached_GroupID):
    asKey = "groupid";
    typeToRegId = OdTrVisRendition::kGroupData;
    break;
  PK_CASE(ExtensionObjectAdded_ExtensionID):
  PK_CASE(ExtensionObjectDeleted_ExtensionID):
  PK_CASE(ExtensionObjectVisibilityChanged_ExtensionID):
  PK_CASE(ExtensionObjectSpecificData_ExtensionID):
  PK_CASE(ExtensionObjectPropertyAttached_ExtensionID):
  PK_CASE(ExtensionObjectPropertyDetached_ExtensionID):
  PK_CASE_EX(_MetafileStream_ExtensionObject_ExtensionID):
    asKey = "extensionid";
    typeToRegId = OdTrVisRendition::kExtensionData;
    break;
  default:
    ODA_FAIL_ONCE();
    return false;
  }
  asKey += "-";
  asKey += asValue;

  OdTrVisId id = kTrVisNullId;
  bool isNewId = !m_pIdMapper->hasId( asValue, typeToRegId);
  if (isNewId && (asValue == "0")) // special (TextureID ...)
  {
    isNewId = false;
    id = kTrVisNullId;
  }
  else
  {
    id = m_pIdMapper->getId( asValue, typeToRegId );
  }
  ODA_ASSERT_ONCE(id > OdTrVisId(100) || id == kTrVisNullId);

  //SEA
  //Let's check ID: if it new but we expect that object should already exist
  //or if it not new but we expect that object should be new
  //let's throw exception
  if( isNewId )
  {
    switch( pk )
    {
    PK_CASE(MetafileFadingChanged_MetafileID):
    PK_CASE(MetafileVisibilityChanged_MetafileID):
    PK_CASE(MetafileHighlightingChanged_MetafileID):
    PK_CASE(MetafileOrderChanged_Ordering_MetafileID):
    PK_CASE(GroupMetafilesAttached_Metafiles_MetafileID):
    PK_CASE(GroupMetafilesDetached_Metafiles_MetafileID):
    PK_CASE(ExtensionObjectDeleted_ExtensionID):
    PK_CASE(ExtensionObjectVisibilityChanged_ExtensionID):
    PK_CASE(ExtensionObjectSpecificData_ExtensionID):
    PK_CASE(MetafilePropertyAttached_MetafileID):
    PK_CASE(MetafilePropertyDetached_MetafileID):
    PK_CASE(GroupPropertyAttached_GroupID):
    PK_CASE(GroupPropertyDetached_GroupID):
    PK_CASE(LayerPropertyAttached_LayerID):
    PK_CASE(LayerPropertyDetached_LayerID):
    PK_CASE(VisualStylePropertyAttached_VisualStyleID):
    PK_CASE(VisualStylePropertyDetached_VisualStyleID):
    PK_CASE(MaterialPropertyAttached_MaterialID):
    PK_CASE(MaterialPropertyDetached_MaterialID):
    PK_CASE(TexturePropertyAttached_TextureID):
    PK_CASE(TexturePropertyDetached_TextureID):
    PK_CASE(LightPropertyAttached_LightID):
    PK_CASE(LightPropertyDetached_LightID):
    PK_CASE(StateBranchPropertyAttached_StateBranchID):
    PK_CASE(StateBranchPropertyDetached_StateBranchID):
    PK_CASE(ViewportPropertyAttached_ViewportID):
    PK_CASE(ViewportPropertyDetached_ViewportID):
    PK_CASE(OverlayPropertyAttached_OverlayID):
    PK_CASE(OverlayPropertyDetached_OverlayID):
    PK_CASE(ExtensionObjectPropertyAttached_ExtensionID):
    PK_CASE(ExtensionObjectPropertyDetached_ExtensionID):
      throw OdError( eInvalidInput );
    }
  }
  //else // This check isn't actual for most data types, since Id's in IdMap never deleted, so reappend will always fail.
  //{
  //  switch( pk )
  //  {
  //  PK_CASE(MetafileAdded_MetafileID):
  //    throw OdError( eInvalidInput );
  //  }
  //}

  switch (pk)
  {
  PK_CASE(TextureAdded_TextureID):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_id = id;
    break;
  PK_CASE(TextureDeleted_TextureID):
    m_id = id;
    break;
  PK_CASE(MaterialAdded_MaterialID):
    ODA_ASSERT_ONCE(m_pMaterialDef.get());
    m_id = id;
    break;
  PK_CASE(MaterialDeleted_MaterialID):
    m_id = id;
    break;
#define PK_CASE_FOR_MATERIAL_CHANNEL_(channelName, memberName, componentName, componentLwrCase) \
  PK_CASE(MaterialAdded_##channelName##Channel_##componentName##Component_TextureID): \
    ODA_ASSERT_ONCE(m_pMaterialDef.get()); \
    m_pMaterialDef->m_##memberName##Channel.m_##componentLwrCase##Component.m_textureId = id; \
    break
#define PK_CASE_FOR_MATERIAL_CHANNEL(channelName, memberName) \
    PK_CASE_FOR_MATERIAL_CHANNEL_(channelName, memberName, Realistic, realistic); \
    PK_CASE_FOR_MATERIAL_CHANNEL_(channelName, memberName, Shading, shading)
  PK_CASE_FOR_MATERIAL_CHANNEL(Ambient, ambient);
  PK_CASE_FOR_MATERIAL_CHANNEL(Diffuse, diffuse);
  PK_CASE_FOR_MATERIAL_CHANNEL(Specular, specular);
  PK_CASE_FOR_MATERIAL_CHANNEL(Emission, emission);
  PK_CASE_FOR_MATERIAL_CHANNEL(Opacity, opacity);
  PK_CASE_FOR_MATERIAL_CHANNEL(BumpMap, bumpMap);
  PK_CASE_FOR_MATERIAL_CHANNEL(Reflection, reflect);
  PK_CASE_FOR_MATERIAL_CHANNEL(Refraction, refract);
  PK_CASE_FOR_MATERIAL_CHANNEL(NormalMap, normMap);
  PK_CASE_FOR_MATERIAL_CHANNEL(Tint, tint);
#undef PK_CASE_FOR_MATERIAL_CHANNEL
#undef PK_CASE_FOR_MATERIAL_CHANNEL_
  PK_CASE(VisualStyleAdded_VisualStyleID): PK_CASE(VisualStyleModified_VisualStyleID):
  PK_CASE(VisualStyleDeleted_VisualStyleID):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.get());
    m_id = id;
    break;
  PK_CASE(LayerAdded_LayerID):
    ODA_ASSERT_ONCE(m_pLayerDef.get());
  PK_CASE(LayerDeleted_LayerID):
  PK_CASE(LayerVisibilityChanged_LayerID):
  PK_CASE(LayerFadingChanged_LayerID):
    m_id = id;
    break;
  PK_CASE(LayerVpDepPropsOverride_LayerID):
    m_id = id;
    break;
  PK_CASE(LayerVpDepPropsOverride_ViewportID):
    ODA_ASSERT_ONCE(m_pLayerOverrideDef.get());
    m_pLayerOverrideDef->m_overrideViewportId = id;
    break;
  PK_CASE(MetafileAdded_MetafileID):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    m_id = id;
    break;
  PK_CASE(MetafileDeleted_MetafileID):
  PK_CASE(MetafileVisibilityChanged_MetafileID):
  PK_CASE(MetafileHighlightingChanged_MetafileID):
  PK_CASE(MetafileFadingChanged_MetafileID):
    ODA_ASSERT_ONCE(!m_pMetafileDef.get());
    m_id = id;
    break;
  PK_CASE(MetafileAdded_GeneratorID):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    m_pMetafileDef->m_generatorId = id;
    break;
  PK_CASE_EX(_MetafileStream_Material_MaterialID):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_Material);
    m_streamWriter.writeUInt64((OdUInt64) id);
    break;
  PK_CASE_EX(_MetafileStream_InitTexture_TextureID):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_InitTexture);
    m_streamWriter.writeUInt64((OdUInt64) id);
    break;
  //PK_CASE_EX(_MetafileStream_TtfText_Text_Char_MetafileID):
  //  ODA_ASSERT_ONCE(m_pWriter.get());
  //  ...
  //  break;
  PK_CASE_EX(_MetafileStream_Metafile_MetafileID):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_Metafile);
    m_streamWriter.writeUInt64((OdUInt64) id);
    break;
  PK_CASE_EX(_MetafileStream_MetafileMarker_Marker):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_MetafileMarker),
    m_streamWriter.writeInt64((OdUInt64) id);
    break;
  PK_CASE(ViewportAdded_ViewportID):
  PK_CASE(ViewportInserted_ViewportID):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_id = id;
    break;
  PK_CASE(ViewportDeleted_ViewportID):
  PK_CASE(ViewportVisibilityChanged_ViewportID):
  PK_CASE(ViewportAntiAliasingChanged_ViewportID):
  PK_CASE(ViewportViewParamsModified_ViewportID):
  PK_CASE(ViewportLineStylesModified_ViewportID):
  PK_CASE(ViewportFadingModified_ViewportID):
  PK_CASE(ViewportLineweightsModified_ViewportID):
  PK_CASE(ViewportShadingModified_ViewportID):
  PK_CASE(ViewportClipModified_ViewportID):
  PK_CASE(ViewportFogChanged_ViewportID):
  PK_CASE(ViewportSectioningChanged_ViewportID):
  PK_CASE(ViewportAmbientLightChanged_ViewportID):
    m_id = id;
    break;
  PK_CASE(ViewportAdded_OverallVpId):
  PK_CASE(ViewportInserted_OverallVpId):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->setOverallViewportId((OdTrVisViewportId)id);
    break;
  PK_CASE(ViewportShadingModified_VisualStyleId):
    ODA_ASSERT_ONCE(m_pViewportShadingDef.get());
    m_pViewportShadingDef->setVisualStyle(id);
    break;
  PK_CASE(MetafileOrderChanged_ViewportID):
    ODA_ASSERT_ONCE(m_pMetafileOrderDef.get());
    m_pMetafileOrderDef->m_viewportId = id;
    break;
  PK_CASE(MetafileOrderInheritance_ViewportID1):
    ODA_ASSERT_ONCE(m_pMetafileOrderInheritance.get());
    m_pMetafileOrderInheritance->m_viewportId1 = id;
    break;
  PK_CASE(MetafileOrderInheritance_ViewportID2):
    ODA_ASSERT_ONCE(m_pMetafileOrderInheritance.get());
    m_pMetafileOrderInheritance->m_viewportId2 = id;
    break;
  PK_CASE(MetafileOrderInheritance_OverlayID):
    ODA_ASSERT_ONCE(m_pMetafileOrderInheritance.get());
    m_pMetafileOrderInheritance->m_overlayId = id;
    break;
  PK_CASE(MetafileOrderChanged_Ordering_MetafileID):
    ODA_ASSERT_ONCE(m_pMetafileOrderDef.get());
    m_pMetafileOrderDef->m_metafileOrder.push_back(id);
    break;
  PK_CASE(OverlayExtentsOverride_ViewportID):
    ODA_ASSERT_ONCE(m_pExtentsDef.get());
    m_pExtentsDef->m_viewportId = id;
    break;
  PK_CASE(ViewportBackgroundChanged_ViewportID):
    ODA_ASSERT_ONCE(m_pBackgroundDef.get());
    m_pBackgroundDef->m_viewportId = id;
    break;
  PK_CASE(OverlayViewParamsOverride_ViewportID):
    ODA_ASSERT_ONCE(m_pOverlayViewParamsOverrideDef.get());
    m_pOverlayViewParamsOverrideDef->m_viewportId = id;
    break;
  PK_CASE(ViewportDefaultLightsChanged_ViewportID):
    ODA_ASSERT_ONCE(m_pLightsListDef.get());
    m_pLightsListDef->m_viewportId = id;
    break;
  PK_CASE(ViewportLightAttached_ViewportID):
  PK_CASE(ViewportLightDetached_ViewportID):
  PK_CASE(LightAdded_LightID):
  PK_CASE(LightModified_LightID):
  PK_CASE(LightDeleted_LightID):
    m_id = id;
    break;
  PK_CASE(ViewportLightAttached_LightID):
  PK_CASE(ViewportLightDetached_LightID):
    ODA_ASSERT_ONCE(m_pLightOverDef.get());
    m_pLightOverDef->m_lightId = id;
    break;
  PK_CASE(OverlayAdded_OverlayID):
  PK_CASE(OverlayVisibilityChanged_OverlayID):
  PK_CASE(OverlayRenderOrderChanged_OverlayID):
    ODA_ASSERT_ONCE(m_pOverlayDef.get());
    m_id = id;
    break;
  PK_CASE(OverlayDeleted_OverlayID):
    ODA_ASSERT_ONCE(!m_pOverlayDef.get());
    m_id = id;
    break;
  PK_CASE(MetafileOrderChanged_OverlayID):
    ODA_ASSERT_ONCE(m_pMetafileOrderDef.get());
    m_pMetafileOrderDef->m_overlayId = id;
    break;
  PK_CASE(OverlayExtentsOverride_OverlayID):
    ODA_ASSERT_ONCE(m_pExtentsDef.get());
    m_pExtentsDef->m_overlayId = id;
    break;
  PK_CASE(OverlayViewParamsOverride_OverlayID):
    ODA_ASSERT_ONCE(m_pOverlayViewParamsOverrideDef.get());
    m_pOverlayViewParamsOverrideDef->m_overlayId = id;
    break;
  PK_CASE(StateBranchAdded_StateBranchID):
  PK_CASE(StateBranchModified_StateBranchID):
    ODA_ASSERT_ONCE( m_pStateBranchDef.get() );
    m_id = id;
    break;
  PK_CASE(StateBranchAdded_Childs_StateBranchID):
  PK_CASE(StateBranchModified_Childs_StateBranchID):
    ODA_ASSERT_ONCE( m_pStateBranchDef.get() );
    m_pStateBranchDef->m_childs.push_back( id );
    break;
  PK_CASE(StateBranchDeleted_StateBranchID):
    m_id = id;
    break;
  PK_CASE_EX(_MetafileStream_Program_ProgramID):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_Program),
    m_streamWriter.writeUInt64((OdUInt64) id);
    break;
  PK_CASE(ViewportShadingModified_ShaderId):
    ODA_ASSERT_ONCE(m_pViewportShadingDef.get());
    m_pViewportShadingDef->setRenderProgram((OdTrVisProgramId)id);
    break;
  PK_CASE(ViewportBackgroundChanged_BaseProgramID):
    ODA_ASSERT_ONCE(m_pBackgroundDef.get());
    m_pBackgroundDef->m_baseProgramId = (OdTrVisProgramId) id;
    break;
  PK_CASE_EX(_MetafileStream_VisualStyle_VisualStyleID):
    ODA_ASSERT_ONCE( m_pWriter.get() );
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_VisualStyle);
    m_streamWriter.writeUInt64((OdUInt64) id);
    break;
  PK_CASE_EX(_MetafileStream_ExtensionObject_ExtensionID):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_ExtensionObject);
    m_streamWriter.writeUInt64((OdUInt64) id);
    break;
    PK_CASE(GroupAdded_GroupID):
    PK_CASE(GroupDeleted_GroupID):
    PK_CASE(GroupMetafilesAttached_GroupID):
    PK_CASE(GroupMetafilesDetached_GroupID):
    //PK_CASE(GroupMetafilesDeleted_GroupID):
    PK_CASE(GroupMetafilesVisibilityChanged_GroupID):
    PK_CASE(GroupMetafilesHighlightingChanged_GroupID):
    PK_CASE(GroupMetafilesFadingChanged_GroupID):
      m_id = id;
    break;
    PK_CASE(GroupMetafilesAttached_Metafiles_MetafileID):
    PK_CASE(GroupMetafilesDetached_Metafiles_MetafileID):
      ODA_ASSERT_ONCE(m_pGroupMetafilesDef.get());
      m_pGroupMetafilesDef->m_metafiles.push_back(id);
    break;
    PK_CASE(ExtensionObjectAdded_ExtensionID):
      ODA_ASSERT_ONCE(m_pExtensionObject.get());
    PK_CASE(ExtensionObjectDeleted_ExtensionID):
    PK_CASE(ExtensionObjectVisibilityChanged_ExtensionID):
    PK_CASE(ExtensionObjectSpecificData_ExtensionID):
      m_id = id;
    break;
    PK_CASE(MetafilePropertyAttached_MetafileID):
    PK_CASE(MetafilePropertyDetached_MetafileID):
    PK_CASE(GroupPropertyAttached_GroupID):
    PK_CASE(GroupPropertyDetached_GroupID):
    PK_CASE(LayerPropertyAttached_LayerID):
    PK_CASE(LayerPropertyDetached_LayerID):
    PK_CASE(VisualStylePropertyAttached_VisualStyleID):
    PK_CASE(VisualStylePropertyDetached_VisualStyleID):
    PK_CASE(MaterialPropertyAttached_MaterialID):
    PK_CASE(MaterialPropertyDetached_MaterialID):
    PK_CASE(TexturePropertyAttached_TextureID):
    PK_CASE(TexturePropertyDetached_TextureID):
    PK_CASE(LightPropertyAttached_LightID):
    PK_CASE(LightPropertyDetached_LightID):
    PK_CASE(StateBranchPropertyAttached_StateBranchID):
    PK_CASE(StateBranchPropertyDetached_StateBranchID):
    PK_CASE(ViewportPropertyAttached_ViewportID):
    PK_CASE(ViewportPropertyDetached_ViewportID):
    PK_CASE(OverlayPropertyAttached_OverlayID):
    PK_CASE(OverlayPropertyDetached_OverlayID):
    PK_CASE(ExtensionObjectPropertyAttached_ExtensionID):
    PK_CASE(ExtensionObjectPropertyDetached_ExtensionID):
      m_id = id;
    break;
    PK_CASE(MetafilePropertyAttached_PropVal_LayerID):
    PK_CASE(GroupPropertyAttached_PropVal_LayerID):
    PK_CASE(LayerPropertyAttached_PropVal_LayerID):
    PK_CASE(VisualStylePropertyAttached_PropVal_LayerID):
    PK_CASE(MaterialPropertyAttached_PropVal_LayerID):
    PK_CASE(TexturePropertyAttached_PropVal_LayerID):
    PK_CASE(LightPropertyAttached_PropVal_LayerID):
    PK_CASE(StateBranchPropertyAttached_PropVal_LayerID):
    PK_CASE(ViewportPropertyAttached_PropVal_LayerID):
    PK_CASE(OverlayPropertyAttached_PropVal_LayerID):
    PK_CASE(ExtensionObjectPropertyAttached_PropVal_LayerID):
    PK_CASE(ExtensionObjectSpecificData_PropVal_LayerID):
      ODA_ASSERT_ONCE(m_pPropertyDef.get() && m_pPropertyDef->underlyingDataType() == OdTrVisPropertyDef::kIdProperty);
      static_cast<OdTrVisIdPropertyDef*>(m_pPropertyDef.get())->m_idVal = id;
    break;
    PK_CASE(MetafilePropertyAttached_PropVal_StateBranchID):
    PK_CASE(GroupPropertyAttached_PropVal_StateBranchID):
    PK_CASE(LayerPropertyAttached_PropVal_StateBranchID):
    PK_CASE(VisualStylePropertyAttached_PropVal_StateBranchID):
    PK_CASE(MaterialPropertyAttached_PropVal_StateBranchID):
    PK_CASE(TexturePropertyAttached_PropVal_StateBranchID):
    PK_CASE(LightPropertyAttached_PropVal_StateBranchID):
    PK_CASE(StateBranchPropertyAttached_PropVal_StateBranchID):
    PK_CASE(ViewportPropertyAttached_PropVal_StateBranchID):
    PK_CASE(OverlayPropertyAttached_PropVal_StateBranchID):
    PK_CASE(ExtensionObjectPropertyAttached_PropVal_StateBranchID):
    PK_CASE(ExtensionObjectSpecificData_PropVal_StateBranchID):
      ODA_ASSERT_ONCE(m_pPropertyDef.get() && m_pPropertyDef->underlyingDataType() == OdTrVisPropertyDef::kIdProperty);
      static_cast<OdTrVisIdPropertyDef*>(m_pPropertyDef.get())->m_idVal = id;
    break;
    PK_CASE(MetafilePropertyAttached_PropVal_VisualStyleID):
    PK_CASE(GroupPropertyAttached_PropVal_VisualStyleID):
    PK_CASE(LayerPropertyAttached_PropVal_VisualStyleID):
    PK_CASE(VisualStylePropertyAttached_PropVal_VisualStyleID):
    PK_CASE(MaterialPropertyAttached_PropVal_VisualStyleID):
    PK_CASE(TexturePropertyAttached_PropVal_VisualStyleID):
    PK_CASE(LightPropertyAttached_PropVal_VisualStyleID):
    PK_CASE(StateBranchPropertyAttached_PropVal_VisualStyleID):
    PK_CASE(ViewportPropertyAttached_PropVal_VisualStyleID):
    PK_CASE(OverlayPropertyAttached_PropVal_VisualStyleID):
    PK_CASE(ExtensionObjectPropertyAttached_PropVal_VisualStyleID):
    PK_CASE(ExtensionObjectSpecificData_PropVal_VisualStyleID):
      ODA_ASSERT_ONCE(m_pPropertyDef.get() && m_pPropertyDef->underlyingDataType() == OdTrVisPropertyDef::kIdProperty);
      static_cast<OdTrVisIdPropertyDef*>(m_pPropertyDef.get())->m_idVal = id;
    break;
  //default:
  //  ODA_FAIL_ONCE();
  //  return false;
  }
  ODA_ASSERT_ONCE(typeToRegId >= 0);
  if (typeToRegId < 0)
    return false;

  if (isNewId && (id == kTrVisNullId)) // special
  {
    m_pIdMapper->setSpecialId( asValue, id, typeToRegId );
  }
  else if (isNewId)
  {

    OdTrVisRendition::IdRegistrator* pIdRegistrator = m_pRendition->idRegistrator();
    if (isNewId && pIdRegistrator)
    {
      OdTrVisUniqueID uId(asValue); 
      ODA_ASSERT_ONCE(typeToRegId >= 0 && asValue == uId.transform());

      OdAnsiString asDbStub = asValue;
      int pos = asDbStub.find('-');
      ODA_ASSERT_ONCE(pos);
      if (pos > 0)
        asDbStub = asDbStub.left(pos);

        // do not touch content - it is used in other places via (OdDbStub*)(*m_pMapFakeDbStub)[asDbStub].get()
      if (m_pMapFakeDbStub->find(asDbStub) == m_pMapFakeDbStub->end())
        (*m_pMapFakeDbStub)[asDbStub] = new OdFakeDbStub(odStrToUInt64(asDbStub)); // = new OdAnsiString(asDbStub);
      //OdDbStub* pFakeDbStub = (OdDbStub*) (*m_pMapFakeDbStub)[asDbStub]->c_str();
      //ODA_ASSERT_ONCE(asDbStub == (const char*) pFakeDbStub);
      ODA_ASSERT_ONCE(asDbStub == OdAnsiString().format("%" PRIX64, (*m_pMapFakeDbStub)[asDbStub]->handle));
      ////pIdRegistrator->reg(typeToRegId, id, pFakeDbStub, uId);

      pIdRegistrator->reg(typeToRegId, id, uId);
    }
  }

  return true;
}

bool OdXmlGlesLoader::DropDbOwning(OdEnPathKey pk, const OdAnsiString& asValue)
{
  ODA_ASSERT_ONCE(!asValue.isEmpty() && asValue != "0");

  // do not touch content - it is used in other places via (OdDbStub*)(*m_pMapFakeDbStub)[asValue].get()
  if (m_pMapFakeDbStub->find(asValue) == m_pMapFakeDbStub->end())
    (*m_pMapFakeDbStub)[asValue] = new OdFakeDbStub(odStrToUInt64(asValue)); // = new OdAnsiString(asValue);
  OdFakeDbStub* pFakeDbStub = (*m_pMapFakeDbStub)[asValue].get();
  //ODA_ASSERT_ONCE(asValue == (const char*) pFakeDbStub);
  ODA_ASSERT_ONCE(asValue == OdAnsiString().format("%" PRIX64, pFakeDbStub->handle));

  switch (pk)
  {
  PK_CASE(MetafilePropertyAttached_PropVal_OwnerID):
  PK_CASE(GroupPropertyAttached_PropVal_OwnerID):
  PK_CASE(LayerPropertyAttached_PropVal_OwnerID):
  PK_CASE(VisualStylePropertyAttached_PropVal_OwnerID):
  PK_CASE(MaterialPropertyAttached_PropVal_OwnerID):
  PK_CASE(TexturePropertyAttached_PropVal_OwnerID):
  PK_CASE(LightPropertyAttached_PropVal_OwnerID):
  PK_CASE(StateBranchPropertyAttached_PropVal_OwnerID):
  PK_CASE(ViewportPropertyAttached_PropVal_OwnerID):
  PK_CASE(OverlayPropertyAttached_PropVal_OwnerID):
  PK_CASE(ExtensionObjectPropertyAttached_PropVal_OwnerID):
  PK_CASE(ExtensionObjectSpecificData_PropVal_OwnerID):
    ODA_ASSERT_ONCE(m_pPropertyDef.get() && m_pPropertyDef->propertyType() == OdTrVisPropertyDef::kOwningProperty);
    static_cast<OdTrVisIdPropertyDef*>(m_pPropertyDef.get())->m_idVal = OdTrVisPtrToId(pFakeDbStub);
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropChars(OdEnPathKey pk, const OdAnsiString& asValue)
{
  switch (pk)
  {
  PK_CASE(ExtensionObjectAdded_ModuleName):
    ODA_ASSERT_ONCE(m_pExtensionObject.get());
    m_pExtensionObject->setModuleName(asValue);
    break;
  PK_CASE(VisualStyleAdded_Prop_SVal): PK_CASE(VisualStyleModified_Prop_SVal):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && m_pVisualStyleProperty 
                    && m_typeVisualStyle == OdGiVariant::kString);
    m_pVisualStyleProperty->set(OdString_toUnicode(asValue), m_pVisualStyleProperty->operation());
    ODA_ASSERT_ONCE(m_typeVisualStyle == m_pVisualStyleProperty->type());
    break;
  PK_CASE(MetafilePropertyAttached_PropVal_Text):
  PK_CASE(GroupPropertyAttached_PropVal_Text):
  PK_CASE(LayerPropertyAttached_PropVal_Text):
  PK_CASE(VisualStylePropertyAttached_PropVal_Text):
  PK_CASE(MaterialPropertyAttached_PropVal_Text):
  PK_CASE(TexturePropertyAttached_PropVal_Text):
  PK_CASE(LightPropertyAttached_PropVal_Text):
  PK_CASE(StateBranchPropertyAttached_PropVal_Text):
  PK_CASE(ViewportPropertyAttached_PropVal_Text):
  PK_CASE(OverlayPropertyAttached_PropVal_Text):
  PK_CASE(ExtensionObjectPropertyAttached_PropVal_Text):
  PK_CASE(ExtensionObjectSpecificData_PropVal_Text):
    ODA_ASSERT_ONCE(m_pPropertyDef.get() && m_pPropertyDef->underlyingDataType() == OdTrVisPropertyDef::kStringProperty);
    static_cast<OdTrVisStringPropertyDef*>(m_pPropertyDef.get())->m_charVal = OdString_toUnicode(asValue);
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropBoolean(OdEnPathKey pk, bool data)
{
  switch (pk)
  {
  PK_CASE(TextureAdded_Smooth):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_pTextureDef->setSmoothFilter(data);
    break;
  PK_CASE(TextureAdded_Modulate):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_pTextureDef->setColorModulation(data);
    break;
  PK_CASE(TextureAdded_IntensityMap):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_pTextureDef->setIntensityMap(data);
    break;
  PK_CASE(TextureAdded_Negative):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_pTextureDef->setNegativeImage(data);
    break;
  PK_CASE(TextureAdded_NormalMap):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_pTextureDef->setNormalMap(data);
    break;
  PK_CASE(TextureAdded_SphereMap):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_pTextureDef->setSphericalMap(data);
    break;
  PK_CASE(TextureAdded_CubeMap):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_pTextureDef->setCubeMap(data);
    break;
  PK_CASE(TextureAdded_Grayscale):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_pTextureDef->setGrayscaleImage(data);
    break;
  PK_CASE(MetafileAdded_Visibility):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setMetafileVisible(data);
    break;
  PK_CASE(MetafileAdded_Highlighted):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setMetafileHighlighted(data);
    break;
  PK_CASE(MetafileAdded_Temporary):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setTemporaryMetafile(data);
    break;
  PK_CASE(MetafileAdded_Nested):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setNestedMetafile(data);
    break;
  PK_CASE(MetafileAdded_BlockRef):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setBlockRefMetafile(data);
    break;
  PK_CASE(MetafileAdded_Utilitary):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setUtilitaryMetafile(data);
    break;
  PK_CASE(MetafileAdded_Sectionable):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setSectionableMetafile(data);
    break;
  PK_CASE(MetafileAdded_Composite):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setCompositeMetafile(data);
    break;
  PK_CASE(MetafileAdded_XRef):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setXRefMetafile(data);
    break;
  PK_CASE(MetafileAdded_Faded):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setMetafileFaded(data);
    break;
  PK_CASE(MetafileAdded_ShadowsCast):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setMetafileCastShadows(data);
    break;
  PK_CASE(MetafileAdded_ShadowsReceive):
    ODA_ASSERT_ONCE(m_pMetafileDef.get());
    metafile()->setMetafileReceiveShadows(data);
    break;
  PK_CASE(ViewportVisibilityChanged_Visibility):
  PK_CASE(ViewportAntiAliasingChanged_AntiAliasing):
  PK_CASE(MetafileHighlightingChanged_Highlight):
  PK_CASE(MetafileVisibilityChanged_Visibility):
  PK_CASE(MetafileFadingChanged_Fade):
  PK_CASE(LayerVisibilityChanged_Visibility):
  PK_CASE(LayerFadingChanged_Fade):
  PK_CASE(GroupMetafilesHighlightingChanged_Highlight):
  PK_CASE(GroupMetafilesVisibilityChanged_Visibility):
  PK_CASE(GroupMetafilesFadingChanged_Fade):
  PK_CASE(ExtensionObjectVisibilityChanged_Visibility):
    ODA_ASSERT_ONCE(m_objectChangedFlag == 0xFF);
    m_objectChangedFlag = (data) ? 1 : 0;
    break;
  PK_CASE(SurfaceChanged_TransparentBackground):
    ODA_ASSERT_ONCE(m_pSurfaceDef.get());
    m_pSurfaceDef->setTransparentBackground(data);
    break;
  PK_CASE(ViewportAdded_ViewportVisible):
  PK_CASE(ViewportInserted_ViewportVisible):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->setViewportVisible(data);
    break;
  PK_CASE(ViewportViewParamsModified_ViewParams_PerspectiveProjection):
  PK_CASE(OverlayViewParamsOverride_ViewParams_PerspectiveProjection):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_bPerspectiveProjection = data;
    break;
  PK_CASE(ViewportViewParamsModified_ViewParams_FrontClip):
  PK_CASE(OverlayViewParamsOverride_ViewParams_FrontClip):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_frontClip.m_bClipped = data;
    break;
  PK_CASE(ViewportViewParamsModified_ViewParams_BackClip):
  PK_CASE(OverlayViewParamsOverride_ViewParams_BackClip):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_backClip.m_bClipped = data;
    break;
  PK_CASE(ViewportAdded_PSOverallViewport):
  PK_CASE(ViewportInserted_PSOverallViewport):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->setPSOverallViewport(data);
    break;
  PK_CASE(ViewportAdded_PSModelViewport):
  PK_CASE(ViewportInserted_PSModelViewport):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->setPSModelViewport(data);
    break;
  PK_CASE(ViewportAdded_PSHelperViewport):
  PK_CASE(ViewportInserted_PSHelperViewport):
    ODA_ASSERT_ONCE(m_pViewportDef.get());
    m_pViewportDef->setPSHelperViewport(data);
    break;
  PK_CASE(ViewportShadingModified_DepthBuffer):
    ODA_ASSERT_ONCE(m_pViewportShadingDef.get() && m_pViewportShadingDef->renderMode().controlVersion() == 5);
    m_pViewportShadingDef->renderMode().setDepthBufferEnabled(data);
    break;
  PK_CASE(ViewportShadingModified_StencilBuffer):
    ODA_ASSERT_ONCE(m_pViewportShadingDef.get() && m_pViewportShadingDef->renderMode().controlVersion() == 5);
    m_pViewportShadingDef->renderMode().setStencilBufferEnabled(data);
    break;
  PK_CASE(ViewportShadingModified_PolygonOffset):
    ODA_ASSERT_ONCE(m_pViewportShadingDef.get() && m_pViewportShadingDef->renderMode().controlVersion() == 5);
    m_pViewportShadingDef->renderMode().setPolygonOffsetEnabled(data);
    break;
  PK_CASE(ViewportShadingModified_Lighting):
    ODA_ASSERT_ONCE(m_pViewportShadingDef.get() && m_pViewportShadingDef->renderMode().controlVersion() == 5);
    m_pViewportShadingDef->renderMode().setLightingEnabled(data);
    break;
  PK_CASE(ViewportShadingModified_FaceNormals):
    ODA_ASSERT_ONCE(m_pViewportShadingDef.get() && m_pViewportShadingDef->renderMode().controlVersion() == 5);
    m_pViewportShadingDef->renderMode().setFaceNormalsEnabled(data);
    break;
  PK_CASE(ViewportClipModified_NrcClip):
    ODA_ASSERT_ONCE(m_pViewportClipDef.get());
    m_pViewportClipDef->enableNrcClip(data);
    SETBIT_1(m_objectChangedFlag, 1);
    break;
  PK_CASE(ViewportClipModified_Border):
    ODA_ASSERT_ONCE(m_pViewportClipDef.get());
    m_pViewportClipDef->enableBorder(data);
    SETBIT_1(m_objectChangedFlag, 2);
    break;
  PK_CASE(ViewportBackgroundChanged_Visible):
    ODA_ASSERT_ONCE(m_pBackgroundDef.get() && m_pBackgroundDef->m_pStream.isNull() && data);
    metafile(); // create m_pBackgroundDef->m_pStream
    break;
  PK_CASE(ViewportFogChanged_Enable):
    ODA_ASSERT_ONCE(m_pFogDef.get());
    m_pFogDef->enableFog(data);
    break;
  PK_CASE(ViewportFogChanged_Background):
    ODA_ASSERT_ONCE(m_pFogDef.get());
    m_pFogDef->enableFogAtBackground(data);
    break;
  PK_CASE(ViewportSectioningChanged_EnableFilling):
    ODA_ASSERT_ONCE(m_pSectionDef.get());
    m_pSectionDef->enableFilling(data);
    break;
  PK_CASE(ViewportSectioningChanged_EnableStippling):
    ODA_ASSERT_ONCE(m_pSectionDef.get());
    m_pSectionDef->enableStippling(data);
    break;
  PK_CASE(ViewportDefaultLightsChanged_DefaultLightsOn):
    ODA_ASSERT_ONCE(m_pLightsListDef.get());
    m_pLightsListDef->m_bDefaultLightsOn = data;
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_On):
  PK_CASE(ViewportLightAttached_On):
  PK_CASE(LightAdded_On):
  PK_CASE(LightModified_On):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->setLightOn(data);
    if (!m_pLightOverDef.isNull())
      SETBIT_1(m_pLightOverDef->m_nKindOfMod, OdTrVisLightDef::kOverrideVisibility);
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_UseLimits):
  PK_CASE(ViewportLightAttached_UseLimits):
  PK_CASE(LightAdded_UseLimits):
  PK_CASE(LightModified_UseLimits):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->enableAttenuationLimits(data);
    if (!m_pLightOverDef.isNull())
      SETBIT_1(m_pLightOverDef->m_nKindOfMod, OdTrVisLightDef::kOverrideAttenuation);
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_ShadowsEnabled):
  PK_CASE(ViewportLightAttached_ShadowsEnabled):
  PK_CASE(LightAdded_ShadowsEnabled):
  PK_CASE(LightModified_ShadowsEnabled):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->enableShadows(data);
    if (!m_pLightOverDef.isNull())
      SETBIT_1(m_pLightOverDef->m_nKindOfMod, OdTrVisLightDef::kOverrideShadow);
    break;
  PK_CASE(VisualStyleAdded_Prop_Set): PK_CASE(VisualStyleModified_Prop_Set):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.get() && m_pVisualStyleProperty);
    m_pVisualStyleProperty->setOperation(data);
    ODA_ASSERT_ONCE(m_pVisualStyleProperty->operation() == data);
    break;
  PK_CASE(VisualStyleAdded_Prop_BVal): PK_CASE(VisualStyleModified_Prop_BVal):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && m_pVisualStyleProperty 
                    && m_typeVisualStyle == OdGiVariant::kBoolean);
    m_pVisualStyleProperty->set(data, m_pVisualStyleProperty->operation());
    ODA_ASSERT_ONCE(m_typeVisualStyle == m_pVisualStyleProperty->type());
    break;
  PK_CASE(OverlayVisibilityChanged_Visibility):
    ODA_ASSERT_ONCE(m_pOverlayDef.get());
    m_pOverlayDef->setOverlayEnabled(data);
    break;
  PK_CASE(OverlayViewParamsOverride_Override):
    ODA_ASSERT_ONCE(m_pOverlayViewParamsOverrideDef.get());
    m_pOverlayViewParamsOverrideDef->m_bOverride = data;
    break;
  PK_CASE(LayerVpDepPropsOverride_Override):
    ODA_ASSERT_ONCE(m_pLayerOverrideDef.get());
    m_pLayerOverrideDef->m_bOverrideLayer = data;
    break;
  PK_CASE(LayerAdded_Visibility):
    ODA_ASSERT_ONCE(m_pLayerDef.get());
    m_pLayerDef->propSetVisible(data); // DropLayerProps
    break;
  PK_CASE(LayerAdded_Faded):
    ODA_ASSERT_ONCE(m_pLayerDef.get());
    m_pLayerDef->propSetFaded(data); // DropLayerProps
    break;
  PK_CASE(LayerVpDepPropsOverride_VpDepSpec_Visibility):
    ODA_ASSERT_ONCE(m_pLayerOverrideDef.get());
    m_pLayerOverrideDef->m_overrideLayerProps.setLayerVisible(data);
    break;
  PK_CASE(LayerVpDepPropsOverride_VpDepSpec_Faded):
    ODA_ASSERT_ONCE(m_pLayerOverrideDef.get());
    m_pLayerOverrideDef->m_overrideLayerProps.setLayerFaded(data);
    break;
  PK_CASE_EX( _MetafileStream_Linestyle_Default ):
    ODA_ASSERT_ONCE( m_pWriter.get() );
    m_streamWriter.writeUInt8( (OdUInt8) OdTrVisMetaRecType_Linestyle );
    m_streamWriter.writeBool( data );
    break;
  PK_CASE(MetafilePropertyAttached_PropVal_Boolean):
  PK_CASE(GroupPropertyAttached_PropVal_Boolean):
  PK_CASE(LayerPropertyAttached_PropVal_Boolean):
  PK_CASE(VisualStylePropertyAttached_PropVal_Boolean):
  PK_CASE(MaterialPropertyAttached_PropVal_Boolean):
  PK_CASE(TexturePropertyAttached_PropVal_Boolean):
  PK_CASE(LightPropertyAttached_PropVal_Boolean):
  PK_CASE(StateBranchPropertyAttached_PropVal_Boolean):
  PK_CASE(ViewportPropertyAttached_PropVal_Boolean):
  PK_CASE(OverlayPropertyAttached_PropVal_Boolean):
  PK_CASE(ExtensionObjectPropertyAttached_PropVal_Boolean):
  PK_CASE(ExtensionObjectSpecificData_PropVal_Boolean):
    ODA_ASSERT_ONCE(m_pPropertyDef.get() && m_pPropertyDef->underlyingDataType() == OdTrVisPropertyDef::kBooleanProperty);
    static_cast<OdTrVisBooleanPropertyDef*>(m_pPropertyDef.get())->m_boolVal = data;
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropUInt8RGB(OdEnPathKey pk, const OdBinaryData& data)
{
  ODA_ASSERT_ONCE(data.size() == 4);
  if (data.size() != 4)
    return false;
  ODCOLORREF color = ODRGBA(data[0], data[1], data[2], data[3]);

  switch (pk)
  {
  PK_CASE_EX(_MetafileStream_Color_Color):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_Color),
    m_streamWriter.writeUInt8(data[0]), m_streamWriter.writeUInt8(data[1]),
    m_streamWriter.writeUInt8(data[2]), m_streamWriter.writeUInt8(data[3]);
    break;
  PK_CASE(SurfaceChanged_BackgroundColor):
    ODA_ASSERT_ONCE(m_pSurfaceDef.get());
    m_pSurfaceDef->m_backgroundColor = color;
    break;
  PK_CASE(ViewportLineStylesModified_LineStyleConfig):
    ODA_ASSERT_ONCE(m_pViewportLinestyleDef.get());
    m_pViewportLinestyleDef->setAsCref(color);
    break;
  PK_CASE(ViewportFadingModified_FadingConfig):
    ODA_ASSERT_ONCE(m_pViewportFadingDef.get());
    m_pViewportFadingDef->setAsCref(color);
    break;
  PK_CASE(ViewportShadingModified_FaceFillColor):
    ODA_ASSERT_ONCE(m_pViewportShadingDef.get());
    m_pViewportShadingDef->setFaceFillColor(color);
    break;
  PK_CASE(ViewportClipModified_BorderColor):
    ODA_ASSERT_ONCE(m_pViewportClipDef.get());
    m_pViewportClipDef->border().setColor(color);
    break;
  PK_CASE(ViewportFogChanged_Color):
    ODA_ASSERT_ONCE(m_pFogDef.get());
    m_pFogDef->setColor(color);
    break;
  PK_CASE(ViewportSectioningChanged_Color):
    ODA_ASSERT_ONCE(m_pSectionDef.get());
    m_pSectionDef->setFillColor(color);
    break;
  PK_CASE(ViewportSectioningChanged_StipplingColor):
    ODA_ASSERT_ONCE(m_pSectionDef.get());
    m_pSectionDef->setStippleColor(color);
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropUInt8(OdEnPathKey pk, OdUInt8 data)
{
  switch (pk)
  {
  PK_CASE_EX(_MetafileStream_EnableOpt_Mode):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_EnableOpt);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_DisableOpt_Mode):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_DisableOpt);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_EnableArray_Type):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_EnableArray);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_DisableArray_Type):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_DisableArray);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_DrawArrays_Mode):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_DrawArrays);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_DrawElements_Mode):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_DrawElements);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_CullFace_Mode):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8)OdTrVisMetaRecType_CullFace),
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_LStipple_Pattern):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_LStipple);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_PStipple_Pattern):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_PStipple);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_HLRStencil_State):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_HLRStencil);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_EnableShading_Mode):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_EnableShading);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_DisableShading_Mode):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_DisableShading);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_InitTexture_Repeat):
    ODA_ASSERT_ONCE(m_pWriter.get());
    //m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_InitTexture); // via "MetafileAdded/MetafileData/MetafileStream/InitTexture/TextureID"
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_EnableMarkerArray_Type):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_EnableMarkerArray);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_DisableMarkerArray_Type):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_DisableMarkerArray);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_VisibilityFlags_Flags):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_VisibilityFlags);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_Lineweight_Type):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_Lineweight);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_GeomMarker_Type):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_GeomMarker);
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_Linestyle_CapsType):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE_EX(_MetafileStream_Linestyle_JoinType):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8(data);
    break;
  PK_CASE(ViewportLineweightsModified_LineweightEnum_NPixels):
    ODA_ASSERT_ONCE(m_pViewportLwdDef.get());
    m_pViewportLwdDef->m_lineweightsEnum.push_back(data);
    break;
  PK_CASE(ViewportSectioningChanged_StipplePattern):
    ODA_ASSERT_ONCE(m_pSectionDef.get());
    m_pSectionDef->setStipplePattern(data);
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropUInt16(OdEnPathKey pk, OdUInt16 data)
{
  switch (pk)
  {
  PK_CASE(OverlayAdded_Flags):
    ODA_ASSERT_ONCE(m_pOverlayDef.get());
    m_pOverlayDef->resetOverlayFlags(data);
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_ShadowMapSize):
  PK_CASE(ViewportLightAttached_ShadowMapSize):
  PK_CASE(LightAdded_ShadowMapSize):
  PK_CASE(LightModified_ShadowMapSize):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->m_shadowMapSize = data;
    if (!m_pLightOverDef.isNull())
      SETBIT_1(m_pLightOverDef->m_nKindOfMod, OdTrVisLightDef::kOverrideShadow);
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_ShadowSoftness):
  PK_CASE(ViewportLightAttached_ShadowSoftness):
  PK_CASE(LightAdded_ShadowSoftness):
  PK_CASE(LightModified_ShadowSoftness):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->m_shadowSoftness = data;
    if (!m_pLightOverDef.isNull())
      SETBIT_1(m_pLightOverDef->m_nKindOfMod, OdTrVisLightDef::kOverrideShadow);
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_ShadowSamples):
  PK_CASE(ViewportLightAttached_ShadowSamples):
  PK_CASE(LightAdded_ShadowSamples):
  PK_CASE(LightModified_ShadowSamples):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->m_shadowSamples = data;
    if (!m_pLightOverDef.isNull())
      SETBIT_1(m_pLightOverDef->m_nKindOfMod, OdTrVisLightDef::kOverrideShadow);
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropUInt32(OdEnPathKey pk, OdUInt32 data)
{
  switch (pk)
  {
  PK_CASE(TextureAdded_Type):
    ODA_ASSERT_ONCE(m_pTextureDef.get() && m_curTexture.isValid);
    m_curTexture.fmt = (enum OdTrVisTexture::Format) data;
    break;
  PK_CASE(TextureAdded_PaletteType):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_curTexture.pal_fmt = (enum OdTrVisTexture::Format) data;
    break;
  PK_CASE(TextureAdded_Width):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_curTexture.nWidth = data;
    break;
  PK_CASE(TextureAdded_Height):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_curTexture.nHeight = data;
    break;
  PK_CASE(TextureAdded_ScanlineLength):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_curTexture.nScanLineLength = data;
    break;
  PK_CASE(TextureAdded_PaletteWidth):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_curTexture.pal_nWidth = data;
    break;
  PK_CASE(TextureAdded_PaletteScanlineLength):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_curTexture.pal_nScanLineLength = data;
    break;
  PK_CASE(TextureAdded_DataAlignment):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_curTexture.nAlign = data;
    break;
  PK_CASE(TextureAdded_PaletteDataAlignment):
    ODA_ASSERT_ONCE(m_pTextureDef.get());
    m_curTexture.pal_nAlign = data;
    break;
#define PK_CASE_FOR_MATERIAL_CHANNEL_(channelName, memberName, componentName, componentLwrCase) \
  PK_CASE(MaterialAdded_##channelName##Channel_##componentName##Component_UWrapType): \
    ODA_ASSERT_ONCE(m_pMaterialDef.get()); \
    m_pMaterialDef->m_##memberName##Channel.m_##componentLwrCase##Component.m_uWrap = (enum OdTrVisMaterialChannelComponent::WrapType) data; \
    break; \
  PK_CASE(MaterialAdded_##channelName##Channel_##componentName##Component_VWrapType): \
    ODA_ASSERT_ONCE(m_pMaterialDef.get()); \
    m_pMaterialDef->m_##memberName##Channel.m_##componentLwrCase##Component.m_vWrap = (enum OdTrVisMaterialChannelComponent::WrapType) data; \
    break
#define PK_CASE_FOR_MATERIAL_CHANNEL(channelName, memberName) \
    PK_CASE_FOR_MATERIAL_CHANNEL_(channelName, memberName, Realistic, realistic); \
    PK_CASE_FOR_MATERIAL_CHANNEL_(channelName, memberName, Shading, shading)
  PK_CASE_FOR_MATERIAL_CHANNEL(Ambient, ambient);
  PK_CASE_FOR_MATERIAL_CHANNEL(Diffuse, diffuse);
  PK_CASE_FOR_MATERIAL_CHANNEL(Specular, specular);
  PK_CASE_FOR_MATERIAL_CHANNEL(Emission, emission);
  PK_CASE_FOR_MATERIAL_CHANNEL(Opacity, opacity);
  PK_CASE_FOR_MATERIAL_CHANNEL(BumpMap, bumpMap);
  PK_CASE_FOR_MATERIAL_CHANNEL(Reflection, reflect);
  PK_CASE_FOR_MATERIAL_CHANNEL(Refraction, refract);
  PK_CASE_FOR_MATERIAL_CHANNEL(NormalMap, normMap);
  PK_CASE_FOR_MATERIAL_CHANNEL(Tint, tint);
#undef PK_CASE_FOR_MATERIAL_CHANNEL
#undef PK_CASE_FOR_MATERIAL_CHANNEL_
  PK_CASE(VisualStyleAdded_NProps): PK_CASE(VisualStyleModified_NProps):
    ODA_ASSERT_ONCE(m_pVisualStyleDef.get() && data > 0);
    m_pVisualStyleDef->setPropertiesNumber(data);
    break;
  PK_CASE(VisualStyleAdded_NPropsModified): PK_CASE(VisualStyleModified_NPropsModified): // usele for xml
    ODA_ASSERT_ONCE(m_pVisualStyleDef.get());
    break;
  PK_CASE(VisualStyleAdded_Prop_NProp): PK_CASE(VisualStyleModified_Prop_NProp):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && data < m_pVisualStyleDef->numProperties() 
                    && !m_pVisualStyleProperty && m_typeVisualStyle == OdGiVariant::kUndefined);
    m_pVisualStyleProperty = &m_pVisualStyleDef->property((OdGiVisualStyleProperties::Property) data);
    m_pVisualStyleProperty->setPropertyNumber(data);
    //m_pVisualStyleProperty->setModified(true);
    break;
  PK_CASE(VisualStyleAdded_Prop_PropType): PK_CASE(VisualStyleModified_Prop_PropType):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && m_pVisualStyleProperty 
                    && data >= OdGiVariant::kBoolean && data <= OdGiVariant::kString);
    m_typeVisualStyle = (OdGiVariant::VariantType) data;
    break;
  PK_CASE_EX(_NArrays):
    if (metafile())
    {
      m_typeElement = OdTrVisArrayWrapper::Type_Invalid,
      m_indexElement = 0,
      m_numDataElement = 0,
      metafile()->m_ArrayElements.resize(data);
    }
    break;
  PK_CASE_EX(_Array_Type):
    if (metafile())
      m_typeElement = (enum OdTrVisArrayWrapper::Type) data;
    break;
  PK_CASE_EX(_Array_NData):
    if (metafile())
      m_numDataElement = data;
    break;
  PK_CASE_EX(_Array_Data_Entry_From):
    if (metafile())
    {
      ODA_ASSERT_ONCE(   m_typeElement == OdTrVisArrayWrapper::Type_Marker
                      && m_indexElement < metafile()->m_ArrayElements.size()
                      && m_uaMarkerIndex < m_aMarkers.size());
      OdTrVisDefProcMark& nMark = m_aMarkers[m_uaMarkerIndex++];
      nMark.m_uFrom = data;
    }
    break;
  PK_CASE_EX(_MetafileStream_SelectionStyle_Index):
    m_streamWriter.writeUInt8((OdUInt8)OdTrVisMetaRecType_SelectionStyle); // + UInt32
  PK_CASE_EX(_MetafileStream_EnableArray_NArray): PK_CASE_EX(_MetafileStream_DrawElements_NArray):
  PK_CASE_EX(_MetafileStream_EnableMarkerArray_NArray):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt32(data);
    break;
  PK_CASE_EX( _MetafileStream_UserEntry_NumData ):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_mfUserEntryNumData = data;
    break;
  //PK_CASE_EX(_MetafileStream_TtfText_NText):
  //  ODA_ASSERT_ONCE(m_pWriter.get());
  //  break;
  PK_CASE(MetafileOrderChanged_InvalidFrom):
    ODA_ASSERT_ONCE(m_pMetafileOrderDef.get());
    m_pMetafileOrderDef->m_nInvalidFrom = data;
    break;
  PK_CASE(MetafileOrderChanged_InvalidLen):
    ODA_ASSERT_ONCE(m_pMetafileOrderDef.get());
    m_pMetafileOrderDef->m_nInvalidLen = data;
    break;
  PK_CASE(MetafileOrderChanged_ValidLen):
    ODA_ASSERT_ONCE(m_pMetafileOrderDef.get());
    break;
  PK_CASE(ViewportClipModified_NNrcCounts):
    ODA_ASSERT_ONCE(m_pViewportClipDef.get());
    m_pViewportClipDef->nrcClipRegion().m_counts.resize(0); // (data), idx = 0;
    break;
  PK_CASE(ViewportClipModified_NNrcPoints):
    ODA_ASSERT_ONCE(m_pViewportClipDef.get());
    m_pViewportClipDef->nrcClipRegion().m_points.resize(0); // (data * 2), idx = 0;
    break;
  PK_CASE(ViewportClipModified_NBorderCounts):
    ODA_ASSERT_ONCE(m_pViewportClipDef.get());
    m_pViewportClipDef->border().region().m_counts.resize(0); // (data), idx = 0;
    break;
  PK_CASE(ViewportClipModified_NBorderPoints):
    ODA_ASSERT_ONCE(m_pViewportClipDef.get());
    m_pViewportClipDef->border().region().m_points.resize(0); // (data * 2), idx = 0;
    break;
  PK_CASE(ViewportLineweightsModified_NLineweightEnum):
    ODA_ASSERT_ONCE(m_pViewportLwdDef.get());
    m_pViewportLwdDef->m_lineweightsEnum.resize(0); // (data), idx = 0;
    break;
  PK_CASE(ViewportDefaultLightsChanged_NLights):
    ODA_ASSERT_ONCE(m_pLightsListDef.get());
    break;
  PK_CASE(ViewportLightAttached_NOverrides):
    ODA_ASSERT_ONCE(m_pLightOverDef.get() && m_pLightDef.isNull());
    m_pLightOverDef->m_nKindOfMod = 0;
    if (data != 0)
    { m_pLightDef = new OdTrVisLightDef();
      m_pLightDef->setDefault(); }
    break;
  PK_CASE(LightModified_NModified):
    ODA_ASSERT_ONCE(m_pLightOverDef.get() && m_pLightDef.get());
    m_pLightOverDef->m_nKindOfMod = 0;
    break;
  PK_CASE(ViewportSectioningChanged_NPlanes):
    ODA_ASSERT_ONCE(m_pSectionDef.get());
    m_pSectionDef->m_planes.reserve(data);
    break;
  PK_CASE(VisualStyleAdded_Prop_CVal): PK_CASE(VisualStyleModified_Prop_IVal):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && m_pVisualStyleProperty 
                    && m_typeVisualStyle == OdGiVariant::kColor);
    m_pVisualStyleProperty->set(data, m_pVisualStyleProperty->operation(), true);
    ODA_ASSERT_ONCE(m_typeVisualStyle == m_pVisualStyleProperty->type());
    break;
  PK_CASE(GroupMetafilesAttached_NMetafiles):
  PK_CASE(GroupMetafilesDetached_NMetafiles):
    ODA_ASSERT_ONCE(!m_pGroupMetafilesDef.get());
    break;
  PK_CASE(ViewportAntiAliasingChanged_AntiAliasing):
  PK_CASE(ExtensionObjectSpecificData_DataID):
  PK_CASE(FrameSwitchInfo_Interval):
    m_mfUserEntryNumData = data;
    break;
  PK_CASE(ExtensionObjectAdded_ExtVersion):
    ODA_ASSERT_ONCE(m_pExtensionObject.get());
    m_pExtensionObject->setModuleVersion(data);
    break;
  PK_CASE(ExtensionObjectAdded_ObjectType):
    ODA_ASSERT_ONCE(m_pExtensionObject.get());
    m_pExtensionObject->setObjectType(data);
    break;
  PK_CASE(MetafilePropertyAttached_PropVal_StyleIndex):
  PK_CASE(GroupPropertyAttached_PropVal_StyleIndex):
  PK_CASE(LayerPropertyAttached_PropVal_StyleIndex):
  PK_CASE(VisualStylePropertyAttached_PropVal_StyleIndex):
  PK_CASE(MaterialPropertyAttached_PropVal_StyleIndex):
  PK_CASE(TexturePropertyAttached_PropVal_StyleIndex):
  PK_CASE(LightPropertyAttached_PropVal_StyleIndex):
  PK_CASE(StateBranchPropertyAttached_PropVal_StyleIndex):
  PK_CASE(ViewportPropertyAttached_PropVal_StyleIndex):
  PK_CASE(OverlayPropertyAttached_PropVal_StyleIndex):
  PK_CASE(ExtensionObjectPropertyAttached_PropVal_StyleIndex):
  PK_CASE(ExtensionObjectSpecificData_PropVal_StyleIndex):
    ODA_ASSERT_ONCE(m_pPropertyDef.get() && m_pPropertyDef->underlyingDataType() == OdTrVisPropertyDef::kIdProperty);
    static_cast<OdTrVisIdPropertyDef*>(m_pPropertyDef.get())->m_idVal = (OdTrVisId)data;
  break;

  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropUInt64(OdEnPathKey pk, OdUInt64 data)
{
  switch (pk)
  {
  PK_CASE_EX(_MetafileStream_SelectionMarker_Marker):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_SelectionMarker),
    m_streamWriter.writeInt64(data);
    break;
  PK_CASE_EX(_Array_Data_Entry_Marker):
    if (metafile())
    {
      ODA_ASSERT_ONCE(   m_typeElement == OdTrVisArrayWrapper::Type_Marker
                      && m_indexElement < metafile()->m_ArrayElements.size()
                      && m_uaMarkerIndex < m_aMarkers.size());
      OdTrVisDefProcMark& nMark = m_aMarkers[m_uaMarkerIndex];
      nMark.m_nMark = data;
    }
    break;
  PK_CASE(StateBranchAdded_NChilds):
  PK_CASE(StateBranchModified_NChilds):
    ODA_ASSERT_ONCE( m_pStateBranchDef.get() );
    m_pStateBranchDef->m_childs.reserve( data );
    break;
  PK_CASE(StateBranchAdded_NMarkers):
  PK_CASE(StateBranchModified_NMarkers):
    ODA_ASSERT_ONCE( m_pStateBranchDef.get() );
    m_pStateBranchDef->m_markers.reserve( data );
    break;
  PK_CASE(StateBranchAdded_Markers_Marker ):
  PK_CASE(StateBranchModified_Markers_Marker):
    ODA_ASSERT_ONCE( m_pStateBranchDef.get() );
    m_pStateBranchDef->m_markers.push_back( data );
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropInt16(OdEnPathKey pk, OdInt16 data)
{
  switch (pk)
  {
  PK_CASE_EX(_MetafileStream_Lineweight_Lw):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeInt16(data);
    break;
  PK_CASE(OverlayAdded_RenderOrder):
  PK_CASE(OverlayRenderOrderChanged_RenderOrder):
    ODA_ASSERT_ONCE(m_pOverlayDef.get());
    m_pOverlayDef->m_renderOrder = data;
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropInt32(OdEnPathKey pk, OdInt32 data)
{
  switch (pk)
  {
  PK_CASE_EX(_MetafileStream_DrawArrays_First):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeInt32(data);
    break;
  PK_CASE_EX(_MetafileStream_DrawArrays_Count): PK_CASE_EX(_MetafileStream_DrawElements_Count):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeInt32(data);
    break;
  PK_CASE_EX(_MetafileStream_IPoint_Index):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_IPoint),
    m_streamWriter.writeInt32(data);
    break;
  PK_CASE_EX(_MetafileStream_ILine_Index1):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_ILine),
    m_streamWriter.writeInt32(data);
    break;
  PK_CASE_EX(_MetafileStream_ILine_Index2):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeInt32(data);
    break;
  PK_CASE(SurfaceChanged_SurfaceSizeXMin):
    ODA_ASSERT_ONCE(m_pSurfaceDef.get());
    m_pSurfaceDef->m_surfaceSize.m_min.x = data;
    break;
  PK_CASE(SurfaceChanged_SurfaceSizeYMin):
    ODA_ASSERT_ONCE(m_pSurfaceDef.get());
    m_pSurfaceDef->m_surfaceSize.m_min.y = data;
    break;
  PK_CASE(SurfaceChanged_SurfaceSizeXMax):
    ODA_ASSERT_ONCE(m_pSurfaceDef.get());
    m_pSurfaceDef->m_surfaceSize.m_max.x = data;
    break;
  PK_CASE(SurfaceChanged_SurfaceSizeYMax):
    ODA_ASSERT_ONCE(m_pSurfaceDef.get());
    m_pSurfaceDef->m_surfaceSize.m_max.y = data;
    break;
  PK_CASE(ViewportClipModified_NrcCounts_Count):
    ODA_ASSERT_ONCE(m_pViewportClipDef.get());
    m_pViewportClipDef->nrcClipRegion().m_counts.push_back(data);
    break;
  PK_CASE(ViewportClipModified_BorderWidth):
    ODA_ASSERT_ONCE(m_pViewportClipDef.get());
    m_pViewportClipDef->border().setWidth(data);
    break;
  PK_CASE(ViewportClipModified_BorderCounts_Count):
    ODA_ASSERT_ONCE(m_pViewportClipDef.get());
    m_pViewportClipDef->border().region().m_counts.push_back(data);
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_Type):
  PK_CASE(ViewportLightAttached_Type):
  PK_CASE(LightAdded_Type):
  PK_CASE(LightModified_Type):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->m_type = (OdTrVisLightDef::LightType)data;
    if (!m_pLightOverDef.isNull())
      m_pLightOverDef->m_nKindOfMod = 0xFFFFFFFF;
    break;
  PK_CASE(VisualStyleAdded_Prop_IVal): PK_CASE(VisualStyleModified_Prop_IVal):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && m_pVisualStyleProperty 
                    && m_typeVisualStyle == OdGiVariant::kInt);
    m_pVisualStyleProperty->set(data, m_pVisualStyleProperty->operation());
    ODA_ASSERT_ONCE(m_typeVisualStyle == m_pVisualStyleProperty->type());
    break;
  PK_CASE(ViewportInserted_Index):
    m_nViewportInsertedIndex = data;
    break;
  PK_CASE(MetafilePropertyAttached_PropType):
  PK_CASE(MetafilePropertyDetached_PropType):
  PK_CASE(GroupPropertyAttached_PropType):
  PK_CASE(GroupPropertyDetached_PropType):
  PK_CASE(LayerPropertyAttached_PropType):
  PK_CASE(LayerPropertyDetached_PropType):
  PK_CASE(VisualStylePropertyAttached_PropType):
  PK_CASE(VisualStylePropertyDetached_PropType):
  PK_CASE(MaterialPropertyAttached_PropType):
  PK_CASE(MaterialPropertyDetached_PropType):
  PK_CASE(TexturePropertyAttached_PropType):
  PK_CASE(TexturePropertyDetached_PropType):
  PK_CASE(LightPropertyAttached_PropType):
  PK_CASE(LightPropertyDetached_PropType):
  PK_CASE(StateBranchPropertyAttached_PropType):
  PK_CASE(StateBranchPropertyDetached_PropType):
  PK_CASE(ViewportPropertyAttached_PropType):
  PK_CASE(ViewportPropertyDetached_PropType):
  PK_CASE(OverlayPropertyAttached_PropType):
  PK_CASE(OverlayPropertyDetached_PropType):
  PK_CASE(ExtensionObjectPropertyAttached_PropType):
  PK_CASE(ExtensionObjectPropertyDetached_PropType):
  PK_CASE(ExtensionObjectSpecificData_PropType):
    m_propertyType = (OdTrVisPropertyDef::PropertyType)data;
  break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropBinaryStream(OdEnPathKey pk, const OdBinaryData& data)
{
  switch (pk)
  {
  PK_CASE(TextureAdded_TextureData):
    {
      ODA_ASSERT_ONCE(m_pTextureDef.get());
      OdTrVisTexturePtr tex = OdTrVisTextureManager::createTexture( m_curTexture.fmt, m_curTexture.nWidth, m_curTexture.nHeight, m_curTexture.nAlign );
      memcpy( tex->textureData(), data.asArrayPtr(), m_curTexture.nScanLineLength * m_curTexture.nHeight );
      m_pTextureDef->m_pTexture = tex;
    }
    break;
  PK_CASE(TextureAdded_PaletteData):
    {
      ODA_ASSERT_ONCE(m_pTextureDef.get());
      OdTrVisTexturePtr tex = OdTrVisTextureManager::createTexture( m_curTexture.pal_fmt, m_curTexture.pal_nWidth, 1, m_curTexture.pal_nAlign );
      memcpy( tex->textureData(), data.asArrayPtr(), m_curTexture.pal_nScanLineLength );
      m_pTextureDef->m_pTexture->setPalette( tex );
    }
    break;
  PK_CASE(ExtensionObjectAdded_ObjectData):
    {
      ODA_ASSERT_ONCE(m_pExtensionObject.get());
      m_pExtensionObject->dataStreamForWrite()->putBytes(data.getPtr(), data.size());
    }
    break;
  PK_CASE_EX( _MetafileStream_UserEntry_Data ):
    {
      ODA_ASSERT_ONCE(m_pWriter.get());
      m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_UserEntry);
      m_streamWriter.writeUInt32( m_mfUserEntryNumData );
      m_streamWriter.write( data.asArrayPtr(), m_mfUserEntryNumData );
    }
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropInts(OdEnPathKey pk, const OdIntArray& data)
{
  ODA_FAIL_ONCE(); // TODO
  return false;
}

bool OdXmlGlesLoader::DropFloat(OdEnPathKey pk, float data)
{
  switch (pk)
  {
#define PK_CASE_FOR_MATERIAL_CHANNEL_(channelName, memberName, componentName, componentLwrCase) \
  PK_CASE(MaterialAdded_##channelName##Channel_##componentName##Component_BlendFactor): \
    ODA_ASSERT_ONCE(m_pMaterialDef.get()); \
    m_pMaterialDef->m_##memberName##Channel.m_##componentLwrCase##Component.m_blendFactor = data; \
    break; \
  PK_CASE(MaterialAdded_##channelName##Channel_##componentName##Component_OpacityLevel): \
    ODA_ASSERT_ONCE(m_pMaterialDef.get()); \
    m_pMaterialDef->m_##memberName##Channel.m_##componentLwrCase##Component.m_opacityLevel = data; \
    break; \
  PK_CASE(MaterialAdded_##channelName##Channel_##componentName##Component_ChannelValue): \
    ODA_ASSERT_ONCE(m_pMaterialDef.get()); \
    m_pMaterialDef->m_##memberName##Channel.m_##componentLwrCase##Component.m_channelValue = data; \
    break; \
  PK_CASE(MaterialAdded_##channelName##Channel_##componentName##Component_TextureBlend): \
    ODA_ASSERT_ONCE(m_pMaterialDef.get()); \
    m_pMaterialDef->m_##memberName##Channel.m_##componentLwrCase##Component.m_textureBlend = data; \
    break
#define PK_CASE_FOR_MATERIAL_CHANNEL(channelName, memberName) \
    PK_CASE_FOR_MATERIAL_CHANNEL_(channelName, memberName, Realistic, realistic); \
    PK_CASE_FOR_MATERIAL_CHANNEL_(channelName, memberName, Shading, shading)
  PK_CASE_FOR_MATERIAL_CHANNEL(Ambient, ambient);
  PK_CASE_FOR_MATERIAL_CHANNEL(Diffuse, diffuse);
  PK_CASE_FOR_MATERIAL_CHANNEL(Specular, specular);
  PK_CASE_FOR_MATERIAL_CHANNEL(Emission, emission);
  PK_CASE_FOR_MATERIAL_CHANNEL(Opacity, opacity);
  PK_CASE_FOR_MATERIAL_CHANNEL(BumpMap, bumpMap);
  PK_CASE_FOR_MATERIAL_CHANNEL(Reflection, reflect);
  PK_CASE_FOR_MATERIAL_CHANNEL(Refraction, refract);
  PK_CASE_FOR_MATERIAL_CHANNEL(NormalMap, normMap);
  PK_CASE_FOR_MATERIAL_CHANNEL(Tint, tint);
#undef PK_CASE_FOR_MATERIAL_CHANNEL
#undef PK_CASE_FOR_MATERIAL_CHANNEL_
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_ConstantAttenuation):
  PK_CASE(ViewportLightAttached_ConstantAttenuation):
  PK_CASE(LightAdded_ConstantAttenuation):
  PK_CASE(LightModified_ConstantAttenuation):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->m_constantAttenuation = data;
    if (!m_pLightOverDef.isNull())
      SETBIT_1(m_pLightOverDef->m_nKindOfMod, OdTrVisLightDef::kOverrideAttenuation);
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_LinearAttenuation):
  PK_CASE(ViewportLightAttached_LinearAttenuation):
  PK_CASE(LightAdded_LinearAttenuation):
  PK_CASE(LightModified_LinearAttenuation):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->m_linearAttenuation = data;
    if (!m_pLightOverDef.isNull())
      SETBIT_1(m_pLightOverDef->m_nKindOfMod, OdTrVisLightDef::kOverrideAttenuation);
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_QuadraticAttenuation):
  PK_CASE(ViewportLightAttached_QuadraticAttenuation):
  PK_CASE(LightAdded_QuadraticAttenuation):
  PK_CASE(LightModified_QuadraticAttenuation):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->m_quadraticAttenuation = data;
    if (!m_pLightOverDef.isNull())
      SETBIT_1(m_pLightOverDef->m_nKindOfMod, OdTrVisLightDef::kOverrideAttenuation);
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_StartAttenuationLimit):
  PK_CASE(ViewportLightAttached_StartAttenuationLimit):
  PK_CASE(LightAdded_StartAttenuationLimit):
  PK_CASE(LightModified_StartAttenuationLimit):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->m_startAttenuationLimit = data;
    if (!m_pLightOverDef.isNull())
      SETBIT_1(m_pLightOverDef->m_nKindOfMod, OdTrVisLightDef::kOverrideAttenuation);
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_EndAttenuationLimit):
  PK_CASE(ViewportLightAttached_EndAttenuationLimit):
  PK_CASE(LightAdded_EndAttenuationLimit):
  PK_CASE(LightModified_EndAttenuationLimit):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->m_endAttenuationLimit = data;
    if (!m_pLightOverDef.isNull())
      SETBIT_1(m_pLightOverDef->m_nKindOfMod, OdTrVisLightDef::kOverrideAttenuation);
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_SpotCutoff):
  PK_CASE(ViewportLightAttached_SpotCutoff):
  PK_CASE(LightAdded_SpotCutoff):
  PK_CASE(LightModified_SpotCutoff):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->m_spotCutoff = data;
    if (!m_pLightOverDef.isNull())
      SETBIT_1(m_pLightOverDef->m_nKindOfMod, OdTrVisLightDef::kOverrideAngles);
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_SpotExponent):
  PK_CASE(ViewportLightAttached_SpotExponent):
  PK_CASE(LightAdded_SpotExponent):
  PK_CASE(LightModified_SpotExponent):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->m_spotExponent = data;
    if (!m_pLightOverDef.isNull())
      SETBIT_1(m_pLightOverDef->m_nKindOfMod, OdTrVisLightDef::kOverrideAngles);
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_SpecularFactor):
  PK_CASE(ViewportLightAttached_SpecularFactor):
  PK_CASE(LightAdded_SpecularFactor):
  PK_CASE(LightModified_SpecularFactor):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->m_specularFactor = data;
    if (!m_pLightOverDef.isNull())
      SETBIT_1(m_pLightOverDef->m_nKindOfMod, OdTrVisLightDef::kOverrideSpecualFactor);
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropDouble(OdEnPathKey pk, double data)
{
  switch (pk)
  {
  PK_CASE(ViewportViewParamsModified_ViewParams_FieldWidth):
  PK_CASE(OverlayViewParamsOverride_ViewParams_FieldWidth):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_fieldWidth = data;
    break;
  PK_CASE(ViewportViewParamsModified_ViewParams_FieldHeight):
  PK_CASE(OverlayViewParamsOverride_ViewParams_FieldHeight):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_fieldHeight = data;
    break;
  PK_CASE(ViewportViewParamsModified_ViewParams_LensLength):
  PK_CASE(OverlayViewParamsOverride_ViewParams_LensLength):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_lensLength = data;
    break;
  PK_CASE(ViewportViewParamsModified_ViewParams_FrontClipDist):
  PK_CASE(OverlayViewParamsOverride_ViewParams_FrontClipDist):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_frontClip.m_clipDist = data;
    break;
  PK_CASE(ViewportViewParamsModified_ViewParams_BackClipDist):
  PK_CASE(OverlayViewParamsOverride_ViewParams_BackClipDist):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_backClip.m_clipDist = data;
    break;
  PK_CASE(ViewportLineweightsModified_LineweightCoef):
    ODA_ASSERT_ONCE(m_pViewportLwdDef.get());
    m_pViewportLwdDef->setLineweightsCoef(data);
    break;
  PK_CASE(VisualStyleAdded_Prop_DVal): PK_CASE(VisualStyleModified_Prop_DVal):
    ODA_ASSERT_ONCE(   m_pVisualStyleDef.get() && m_pVisualStyleProperty 
                    && m_typeVisualStyle == OdGiVariant::kDouble);
    m_pVisualStyleProperty->set(data, m_pVisualStyleProperty->operation());
    ODA_ASSERT_ONCE(m_typeVisualStyle == m_pVisualStyleProperty->type());
    break;
  PK_CASE_EX(_MetafileStream_Lineweight_Ps):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeDouble(data);
    break;
//PK_CASE_EX(_MetafileStream_TtfText_Text_Char_Multiplier):
//  ODA_ASSERT_ONCE(m_pWriter.get());
//  break;
  default:
    ODA_FAIL_ONCE(); // TODO
    //return false;
    return DropFloat(pk, (float) data);
  }
  return true;
}

bool OdXmlGlesLoader::DropUInts16(OdEnPathKey pk, const OdArray<OdUInt16, OdMemoryAllocator<OdUInt16> >& data)
{
  switch (pk)
  {
  PK_CASE_EX(_Array_ArrayData):
    if (metafile())
    {
      ODA_ASSERT_ONCE(   m_typeElement != OdTrVisArrayWrapper::Type_Invalid
                      && m_indexElement < metafile()->m_ArrayElements.size());
      OdUInt32 sz = data.size();
      ODA_ASSERT_ONCE(sz == m_numDataElement && m_typeElement == OdTrVisArrayWrapper::Type_Index);
      OdTrVisArrayWrapper& array = metafile()->m_ArrayElements[m_indexElement++];
      array.setArray(m_typeElement, data.getPtr(), sz * sizeof(OdUInt16));
      m_typeElement = OdTrVisArrayWrapper::Type_Invalid;
      m_numDataElement = 0;
    }
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropFloats(OdEnPathKey pk, const OdArray<float, OdMemoryAllocator<float> >& data)
{
  OdUInt32 sz = data.size();
  switch (pk)
  {
#define PK_CASE_FOR_MATERIAL_CHANNEL_(channelName, memberName, componentName, componentLwrCase) \
  PK_CASE(MaterialAdded_##channelName##Channel_##componentName##Component_ChannelColor): \
    ODA_ASSERT_ONCE(m_pMaterialDef.get() && sz == 3); \
    m_pMaterialDef->m_##memberName##Channel.m_##componentLwrCase##Component.m_channelColor[0] = data[0]; \
    m_pMaterialDef->m_##memberName##Channel.m_##componentLwrCase##Component.m_channelColor[1] = data[1]; \
    m_pMaterialDef->m_##memberName##Channel.m_##componentLwrCase##Component.m_channelColor[2] = data[2]; \
    break
#define PK_CASE_FOR_MATERIAL_CHANNEL(channelName, memberName) \
    PK_CASE_FOR_MATERIAL_CHANNEL_(channelName, memberName, Realistic, realistic); \
    PK_CASE_FOR_MATERIAL_CHANNEL_(channelName, memberName, Shading, shading)
  PK_CASE_FOR_MATERIAL_CHANNEL(Ambient, ambient);
  PK_CASE_FOR_MATERIAL_CHANNEL(Diffuse, diffuse);
  PK_CASE_FOR_MATERIAL_CHANNEL(Specular, specular);
  PK_CASE_FOR_MATERIAL_CHANNEL(Emission, emission);
  PK_CASE_FOR_MATERIAL_CHANNEL(Opacity, opacity);
  PK_CASE_FOR_MATERIAL_CHANNEL(BumpMap, bumpMap);
  PK_CASE_FOR_MATERIAL_CHANNEL(Reflection, reflect);
  PK_CASE_FOR_MATERIAL_CHANNEL(Refraction, refract);
  PK_CASE_FOR_MATERIAL_CHANNEL(NormalMap, normMap);
  PK_CASE_FOR_MATERIAL_CHANNEL(Tint, tint);
#undef PK_CASE_FOR_MATERIAL_CHANNEL
#undef PK_CASE_FOR_MATERIAL_CHANNEL_
  PK_CASE_EX(_Array_ArrayOffset):
    if (metafile())
    {
      ODA_ASSERT_ONCE(sz == 3);
      m_ArrayOffset.resize(3), 
      m_ArrayOffset[0] = data[0], m_ArrayOffset[1] = data[1], m_ArrayOffset[2] = data[2];
    }
    break;
  PK_CASE_EX(_Array_ArrayData):
    if (metafile())
    {
      ODA_ASSERT_ONCE(   m_typeElement != OdTrVisArrayWrapper::Type_Invalid
                      && m_indexElement < metafile()->m_ArrayElements.size());
      ODA_ASSERT_ONCE(  (   sz == (m_numDataElement * 3) 
                         && (   m_typeElement == OdTrVisArrayWrapper::Type_Vertex 
                             || m_typeElement == OdTrVisArrayWrapper::Type_Normal))
                      || (   sz == (m_numDataElement * 4) 
                          && m_typeElement == OdTrVisArrayWrapper::Type_Color)
                      || (   sz == (m_numDataElement * 2) 
                          && m_typeElement == OdTrVisArrayWrapper::Type_TexCoord)
                      || (  (sz ==  m_numDataElement)
                          && m_typeElement == OdTrVisArrayWrapper::Type_Depth));
      OdTrVisArrayWrapper& array = metafile()->m_ArrayElements[m_indexElement++];
      if (m_ArrayOffset.size() == 3)
      {
       float* pData = const_cast<float*>(data.getPtr());
       for (OdUInt32 idx = 0; idx < data.size(); idx++)
         pData[idx] += m_ArrayOffset[idx % 3];
      }
      m_ArrayOffset.clear();
      array.setArray(m_typeElement, data.getPtr(), sz * sizeof(float));
      m_typeElement = OdTrVisArrayWrapper::Type_Invalid;
      m_numDataElement = 0;
    }
    break;

  PK_CASE_EX(_MetafileStream_VPoint_Point):
    ODA_ASSERT_ONCE(m_pWriter.get() && sz == 3);
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_VPoint),
    m_streamWriter.writeFloat(data[0]), m_streamWriter.writeFloat(data[1]), m_streamWriter.writeFloat(data[2]);
    break;
  PK_CASE_EX(_MetafileStream_VLine_Point1):
    ODA_ASSERT_ONCE(m_pWriter.get() && sz == 3);
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_VLine),
    m_streamWriter.writeFloat(data[0]), m_streamWriter.writeFloat(data[1]), m_streamWriter.writeFloat(data[2]);
    break;
  PK_CASE_EX(_MetafileStream_VLine_Point2):
    ODA_ASSERT_ONCE(m_pWriter.get() && sz == 3);
    m_streamWriter.writeFloat(data[0]), m_streamWriter.writeFloat(data[1]), m_streamWriter.writeFloat(data[2]);
    break;

  PK_CASE(ViewportClipModified_NrcPoints_Point):
    ODA_ASSERT_ONCE(m_pViewportClipDef.get() && sz == 2);
    m_pViewportClipDef->nrcClipRegion().m_points.push_back(data[0]),
    m_pViewportClipDef->nrcClipRegion().m_points.push_back(data[1]);
    break;
  PK_CASE(ViewportClipModified_BorderPoints_Point):
    ODA_ASSERT_ONCE(m_pViewportClipDef.get() && sz == 2);
    m_pViewportClipDef->border().region().m_points.push_back(data[0]),
    m_pViewportClipDef->border().region().m_points.push_back(data[1]);
    break;

  PK_CASE(ViewportAmbientLightChanged_AmbientLight):
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_Color):
  PK_CASE(ViewportLightAttached_Color):
  PK_CASE(LightAdded_Color):
  PK_CASE(LightModified_Color):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->m_color.set(data[0], data[1], data[2]);
    m_pLightDef->m_color.setAlpha(data[3]);
    if (!m_pLightOverDef.isNull())
      SETBIT_1(m_pLightOverDef->m_nKindOfMod, OdTrVisLightDef::kOverrideColor);
    break;

  PK_CASE(ViewportSectioningChanged_PlanesList_Plane_Row0):
    ODA_ASSERT_ONCE(m_pSectionDef.get() && sz == 3);
    m_pSectionDef->m_planes.last().entry[0][0] = data[0],
    m_pSectionDef->m_planes.last().entry[0][1] = data[1],
    m_pSectionDef->m_planes.last().entry[0][2] = data[2];
    break;
  PK_CASE(ViewportSectioningChanged_PlanesList_Plane_Row1):
    ODA_ASSERT_ONCE(m_pSectionDef.get() && sz == 3);
    m_pSectionDef->m_planes.last().entry[1][0] = data[0],
    m_pSectionDef->m_planes.last().entry[1][1] = data[1],
    m_pSectionDef->m_planes.last().entry[1][2] = data[2];
    break;
  PK_CASE(ViewportSectioningChanged_PlanesList_Plane_Row2):
    ODA_ASSERT_ONCE(m_pSectionDef.get() && sz == 3);
    m_pSectionDef->m_planes.last().entry[2][0] = data[0],
    m_pSectionDef->m_planes.last().entry[2][1] = data[1],
    m_pSectionDef->m_planes.last().entry[2][2] = data[2];
    break;

  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}

bool OdXmlGlesLoader::DropDoubles(OdEnPathKey pk, const OdArray<double, OdMemoryAllocator<double> >& data)
{
  OdUInt32 sz = data.size();
  switch (pk)
  {
  PK_CASE(ViewportViewParamsModified_ViewParams_ViewPosition):
  PK_CASE(OverlayViewParamsOverride_ViewParams_ViewPosition):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get() && sz == 3);
    m_pViewParamsDef->m_viewBasis.setOrigin(OdGePoint3d(data[0], data[1], data[2]));
    break;
  PK_CASE(ViewportViewParamsModified_ViewParams_ViewBasisX):
  PK_CASE(OverlayViewParamsOverride_ViewParams_ViewBasisX):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get() && sz == 3);
    m_pViewParamsDef->m_viewBasis.setXAxis(OdGeVector3d(data[0], data[1], data[2]));
    break;
  PK_CASE(ViewportViewParamsModified_ViewParams_ViewBasisY):
  PK_CASE(OverlayViewParamsOverride_ViewParams_ViewBasisY):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get() && sz == 3);
    m_pViewParamsDef->m_viewBasis.setYAxis(OdGeVector3d(data[0], data[1], data[2]));
    break;
  PK_CASE(ViewportViewParamsModified_ViewParams_ViewBasisZ):
  PK_CASE(OverlayViewParamsOverride_ViewParams_ViewBasisZ):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get() && sz == 3);
    m_pViewParamsDef->m_viewBasis.setZAxis(OdGeVector3d(data[0], data[1], data[2]));
    break;
  PK_CASE(ViewportViewParamsModified_ViewParams_ViewTarget):
  PK_CASE(OverlayViewParamsOverride_ViewParams_ViewTarget):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get() && sz == 3);
    m_pViewParamsDef->m_viewTarget.set(data[0], data[1], data[2]);
    break;
  PK_CASE(ViewportClipModified_ScreenRect):
    ODA_ASSERT_ONCE(m_pViewportClipDef.get() && sz == 4);
    m_pViewportClipDef->setScreenRect(OdGsDCRectDouble(data[0], data[2], data[1], data[3]));
    m_pViewportClipDef->localXform().setViewportRect(m_pViewportClipDef->screenRect());
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_Position):
  PK_CASE(ViewportLightAttached_Position):
  PK_CASE(LightAdded_Position):
  PK_CASE(LightModified_Position):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->m_position.set(data[0], data[1], data[2]);
    if (!m_pLightOverDef.isNull())
      SETBIT_1(m_pLightOverDef->m_nKindOfMod, OdTrVisLightDef::kOverrideOrientation);
    break;
  PK_CASE(ViewportDefaultLightsChanged_LightsList_Light_Direction):
  PK_CASE(ViewportLightAttached_Direction):
  PK_CASE(LightAdded_Direction):
  PK_CASE(LightModified_Direction):
    ODA_ASSERT_ONCE(m_pLightDef.get());
    m_pLightDef->m_direction.set(data[0], data[1], data[2]);
    if (!m_pLightOverDef.isNull())
      SETBIT_1(m_pLightOverDef->m_nKindOfMod, OdTrVisLightDef::kOverrideOrientation);
    break;
  PK_CASE(ViewportFogChanged_Distance):
    ODA_ASSERT_ONCE(m_pFogDef.get() && sz == 2);
    m_pFogDef->setDistances(data[0], data[1]);
    break;
  PK_CASE(ViewportFogChanged_Density):
    ODA_ASSERT_ONCE(m_pFogDef.get() && sz == 2);
    m_pFogDef->setDensities(data[0], data[1]);
    break;
  PK_CASE(OverlayExtentsOverride_SceneMin):
    ODA_ASSERT_ONCE(m_pExtentsDef.get() && sz == 3);
    m_pExtentsDef->m_sceneExtents.set(OdGePoint3d(data[0], data[1], data[2]), m_pExtentsDef->m_sceneExtents.maxPoint());
    break;
  PK_CASE(OverlayExtentsOverride_SceneMax):
    ODA_ASSERT_ONCE(m_pExtentsDef.get() && sz == 3);
    m_pExtentsDef->m_sceneExtents.set(m_pExtentsDef->m_sceneExtents.minPoint(), OdGePoint3d(data[0], data[1], data[2]));
    break;
  PK_CASE(OverlayExtentsOverride_RealMin):
    ODA_ASSERT_ONCE(m_pExtentsDef.get() && sz == 3);
    m_pExtentsDef->m_realExtents.set(OdGePoint3d(data[0], data[1], data[2]), m_pExtentsDef->m_realExtents.maxPoint());
    break;
  PK_CASE(OverlayExtentsOverride_RealMax):
    ODA_ASSERT_ONCE(m_pExtentsDef.get() && sz == 3);
    m_pExtentsDef->m_realExtents.set(m_pExtentsDef->m_realExtents.minPoint(), OdGePoint3d(data[0], data[1], data[2]));
    break;
  PK_CASE(MetafileAdded_ExtMin):
    ODA_ASSERT_ONCE(m_pMetafileDef.get() && sz == 3);
    m_pMetafileDef->m_extents = OdGeExtents3d(OdGePoint3d(data[0], data[1], data[2]),
                                              m_pMetafileDef->m_extents.maxPoint());
    break;
  PK_CASE(MetafileAdded_ExtMax):
    ODA_ASSERT_ONCE(m_pMetafileDef.get() && sz == 3);
    m_pMetafileDef->m_extents = OdGeExtents3d(m_pMetafileDef->m_extents.minPoint(),
                                              OdGePoint3d(data[0], data[1], data[2]));
    break;
  default:
    {
      ODA_FAIL_ONCE(); // TODO
      //return false;
      OdArray<float, OdMemoryAllocator<float> > flts;
      flts.resize(data.size());
      for (OdUInt32 idx = 0; idx < data.size(); idx++)
        flts[idx] = (float) data[idx];
      return DropFloats(pk, flts);
    }
  }
  return true;
}

bool OdXmlGlesLoader::DropMatrix(OdEnPathKey pk, 
                                 const OdGeMatrix3d& data,
                                 const OdArray<float, OdMemoryAllocator<float> >* pDataFloats) // = NULL
{
  switch (pk)
  {
  PK_CASE_EX(_MetafileStream_PushMatrix_Matrix):
    ODA_ASSERT_ONCE(m_pWriter.get());
    m_streamWriter.writeUInt8((OdUInt8) OdTrVisMetaRecType_PushMatrix);
    //if (data == OdGeMatrix3d::kIdentity)
    //  m_streamWriter.writeUInt8((OdUInt8) OdTrVisPushMatrixType_Identity);
    //else
    {
      m_streamWriter.writeUInt8((OdUInt8) OdTrVisPushMatrixType_Full);
      m_streamWriter.writeMatrix3d(data); //m_streamWriter.writeMatrix3dArray(&data, sizeof(OdGeMatrix3d));
    }
    break;
//PK_CASE_EX(_MetafileStream_TtfText_Transform):
//  ODA_ASSERT_ONCE(m_pWriter.get() && pDataFloats);
//  m_streamWriter.writeFloatArray(pDataFloats->getPtr(), 16); // TODO OdUInt64 fontKeyA & OdUInt64 fontKeyB - are missing in xml
//  break;
  PK_CASE(ViewportViewParamsModified_ViewParams_ScreenMatrix):
  PK_CASE(OverlayViewParamsOverride_ViewParams_ScreenMatrix):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_screenMatrix = data;
    break;
  PK_CASE(ViewportViewParamsModified_ViewParams_ViewingMatrix):
  PK_CASE(OverlayViewParamsOverride_ViewParams_ViewingMatrix):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_viewingMatrix = data;
    break;
  PK_CASE(ViewportViewParamsModified_ViewParams_ProjectionMatrix):
  PK_CASE(OverlayViewParamsOverride_ViewParams_ProjectionMatrix):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_projectionMatrix = data;
    break;
  PK_CASE(ViewportViewParamsModified_ViewParams_CorrectionMatrix):
  PK_CASE(OverlayViewParamsOverride_ViewParams_CorrectionMatrix):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_correctionMatrix = data;
    break;
  PK_CASE(ViewportViewParamsModified_ViewParams_OutputMatrix):
  PK_CASE(OverlayViewParamsOverride_ViewParams_OutputMatrix):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_outputMatrix = data;
    break;
  PK_CASE(ViewportViewParamsModified_ViewParams_MetafileMatrix):
  PK_CASE(OverlayViewParamsOverride_ViewParams_MetafileMatrix):
    ODA_ASSERT_ONCE(m_pViewParamsDef.get());
    m_pViewParamsDef->m_metafileMatrix = data;
    break;
  PK_CASE(MetafilePropertyAttached_PropVal_Matrix):
  PK_CASE(GroupPropertyAttached_PropVal_Matrix):
  PK_CASE(LayerPropertyAttached_PropVal_Matrix):
  PK_CASE(VisualStylePropertyAttached_PropVal_Matrix):
  PK_CASE(MaterialPropertyAttached_PropVal_Matrix):
  PK_CASE(TexturePropertyAttached_PropVal_Matrix):
  PK_CASE(LightPropertyAttached_PropVal_Matrix):
  PK_CASE(StateBranchPropertyAttached_PropVal_Matrix):
  PK_CASE(ViewportPropertyAttached_PropVal_Matrix):
  PK_CASE(OverlayPropertyAttached_PropVal_Matrix):
  PK_CASE(ExtensionObjectPropertyAttached_PropVal_Matrix):
  PK_CASE(ExtensionObjectSpecificData_PropVal_Matrix):
    ODA_ASSERT_ONCE(m_pPropertyDef.get() && m_pPropertyDef->underlyingDataType() == OdTrVisPropertyDef::kMatrixProperty);
    static_cast<OdTrVisMatrixPropertyDef*>(m_pPropertyDef.get())->m_mtxVal = data;
    break;
  default:
    ODA_FAIL_ONCE(); // TODO
    return false;
  }
  return true;
}
