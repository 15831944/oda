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
// GLES2 device xml server rendition

#include "OdaCommon.h"
#include "FlatMemStream.h"
#include "Gs/GsFiler.h"
#include "TrVisRenderClient.h"
#include "TrVisTtfFontsCacheClient.h"
#include "TrVisIdMap.h"
#include "TrXmlRendition.h"

#define OD_TRXML_DROPHLTASCLIENTONLY

// #define OD_TRXML_DROPSCD_TEST

#ifdef OD_TRXML_DROPSCD_TEST
#include "ExGsGLES2ShareableClientData.h"

struct DropSCDTest : public OdTrXmlServerStub
{
  OdTrXmlServer **m_ppWhere;
  OdTrXmlServer  *m_pWho;
  static OdTrVecDevice *m_pDevice;

  DropSCDTest(OdTrXmlServer **ppWhere)
    : m_ppWhere(ppWhere)
    , m_pWho(*ppWhere)
  {
    *ppWhere = this;
  }
  ~DropSCDTest()
  {
    if (*m_ppWhere != m_pWho)
      *m_ppWhere = m_pWho;
  }

  virtual bool UpdateClientDependentSectionOnly() const { return m_pWho->UpdateClientDependentSectionOnly(); }

  virtual bool DropShareableClientData(IdType /*idType*/, IdSubType /*idSubType*/, OdUInt64 resId, OdTrVisUniqueID* pRegId,
                                       OdUInt32 nBinaryFields, const void ** const pBinaryFields, const OdUInt32* nBinaryFieldsData,
                                       OdUInt32 /*nIdMappings*/ = 0, const IdMapping* pIdMappings = NULL)
  {
    OdUInt8Array pkgr;
    for (OdUInt32 n = 0; n < nBinaryFields; n++)
    {
      if (pBinaryFields[n])
        pkgr.insert(pkgr.end(), (const OdUInt8*)pBinaryFields[n], ((const OdUInt8*)pBinaryFields[n] + nBinaryFieldsData[n]));
    }
    *m_ppWhere = m_pWho;
    if (pkgr.size())
      OdGLES2ShareableClientData::process(m_pWho, resId, pRegId, pkgr.getPtr(), pIdMappings, m_pDevice->clientSettings(), &m_pDevice->outputRect());
    return true;
  }
};

OdTrVecDevice *DropSCDTest::m_pDevice = NULL;
#endif

OdTrXmlRendition::OdTrXmlRendition()
  : m_pXml()
  , m_mode(kIntermediate)
  , m_pRedir(NULL)
  , m_outputState(OdTrXmlServer::kIntermediateState)
  , m_pProcDevice(NULL)
{
}

OdTrXmlRendition::~OdTrXmlRendition()
{
  finalizeXmlServerUsage();
}

OdIntPtr OdTrXmlRendition::getClientSettings() const
{
  return (OdIntPtr)&m_clientSettings;
}

void OdTrXmlRendition::setClientSettings(OdIntPtr pClSet)
{
  m_clientSettings = *(ClientSettings*)pClSet;
}

OdIntPtr OdTrXmlRendition::getXmlServer() const
{
  return (OdIntPtr)m_pXml.get();
}

void OdTrXmlRendition::setXmlServer(OdIntPtr pXmlServer)
{
  if (m_pXml.get() != (OdTrXmlServer*)pXmlServer)
    finalizeXmlServerUsage();
  m_pXml = (OdTrXmlServer*)pXmlServer;
}

OdTrXmlServer *OdTrXmlRendition::xml()
{
  if (m_pXml.isNull()) throw OdError(eNullPtr);
  return m_pXml;
}

const OdTrXmlServer *OdTrXmlRendition::xml() const
{
  if (m_pXml.isNull()) throw OdError(eNullPtr);
  return m_pXml;
}

void OdTrXmlRendition::setIntermediateMode()
{
  m_mode = kIntermediate;
  m_pRedir->setForRecord();
}

void OdTrXmlRendition::setGsUpdateMode()
{
  m_mode = kGsUpdate;
  m_pRedir->setForPlay();
}

class OdGLES2XmlNestingLevel
{
  OdTrXmlServer *m_pXml;
  const char *m_pText;
  public:
    OdGLES2XmlNestingLevel(OdTrXmlServer *pXml, const char *pText)
      : m_pXml(pXml)
      , m_pText(pText)
    {
      pXml->IncreaseNesting(pText);
    }
    ~OdGLES2XmlNestingLevel()
    {
      m_pXml->DecreaseNesting(m_pText);
    }
};

#define XML_LEVEL_STATIC(pXml, nLevel, pText) \
  OdGLES2XmlNestingLevel _xmlNsL##nLevel(pXml, pText)
#define XML_LEVEL0_STATIC(pXml, pText) \
  XML_LEVEL_STATIC(pXml, 0, pText)

#define XML_LEVEL(nLevel, pText) \
  OdGLES2XmlNestingLevel _xmlNsL##nLevel(xml(), pText)
#define XML_LEVEL0(pText) \
  XML_LEVEL(0, pText)

void OdTrXmlRendition::beginDeviceProcessing(OdTrVisRenderClient *pDevice)
{
  ODA_ASSERT(!m_pProcDevice);
  m_pProcDevice = pDevice;
}

void OdTrXmlRendition::endDeviceProcessing(OdTrVisRenderClient *
#ifdef ODA_DIAGNOSTICS
  pDevice
#endif
  )
{
  ODA_ASSERT(m_pProcDevice == pDevice);
  m_pProcDevice = NULL;
}

OdTrVisRenderClient *OdTrXmlRendition::processingDevice() const
{
  ODA_ASSERT(m_pProcDevice);
  return m_pProcDevice;
}

inline OdTrXmlServer::IdType renditionDataType2ServerIdType(OdTrVisRendition::DataType type)
{
  OdTrXmlServer::IdType                   idType = OdTrXmlServer::kUnknownId;
  switch (type)
  {
    case OdTrVisRendition::kViewportData:    idType = OdTrXmlServer::kViewportId;    break;
    case OdTrVisRendition::kMetafileData:    idType = OdTrXmlServer::kMetafileId;    break;
    case OdTrVisRendition::kLightData:       idType = OdTrXmlServer::kLightId;       break;
    case OdTrVisRendition::kStateBranchData: idType = OdTrXmlServer::kStateBranchId; break;
    case OdTrVisRendition::kTextureData:     idType = OdTrXmlServer::kTextureId;     break;
    case OdTrVisRendition::kMaterialData:    idType = OdTrXmlServer::kMaterialId;    break;
    case OdTrVisRendition::kProgramData:     idType = OdTrXmlServer::kProgramId;     break;
    case OdTrVisRendition::kVisualStyleData: idType = OdTrXmlServer::kVisualStyleId; break;
    case OdTrVisRendition::kLayerData:       idType = OdTrXmlServer::kLayerId;       break;
    case OdTrVisRendition::kOverlayData:     idType = OdTrXmlServer::kOverlayId;     break;
    case OdTrVisRendition::kGroupData:       idType = OdTrXmlServer::kGroupId;       break;
    case OdTrVisRendition::kExtensionData:   idType = OdTrXmlServer::kExtensionId;   break;
    default:                                                                         break;
  }
  return idType;
}

bool OdTrXmlRendition::idRegTraverseCallback(OdTrVisRendition::DataType type, OdTrVisId id, OdTrVisUniqueID *pRegId)
{
  OdTrXmlServer::IdType idType = renditionDataType2ServerIdType(type);
  if (idType == OdTrXmlServer::kUnknownId) return true;
  xml()->UnregisterId(id, pRegId, idType); return true;
}

void OdTrXmlRendition::finalizeXmlServerUsage()
{
  if (m_pRedir && m_pXml.get())
  {
    m_pRedir->dropRecords(); // ???
    m_idReg.traverse(this);
    m_idReg.killAll();
  }
}

void OdTrXmlRendition::onDeviceUpdateBegin(OdTrVisRenderClient *pDevice)
{
  if (!getXmlServer())
  {
    if (pDevice && pDevice->hasProperty(OD_T("XmlServer")))
      setXmlServer(pDevice->getProperty(OD_T("XmlServer"))->getIntPtr());
  }

#ifdef OD_TRXML_DROPSCD_TEST
  DropSCDTest::m_pDevice = static_cast<OdTrVecDevice*>(pDevice);
#endif
  OutputState(OdTrXmlServer::kIntermediateState, true);

  xml()->IncreaseNesting("GsUpdate");
  setGsUpdateMode();

  beginDeviceProcessing(pDevice);
  if (!updateClientDependentSectionOnly())
    m_pRedir->dropRecords(); // Output all collected between GsUpdate's data
}

void OdTrXmlRendition::onDeviceUpdateEnd(OdTrVisRenderClient *pDevice, bool /*bRender*/)
{
  setIntermediateMode();
  OutputState(OdTrXmlServer::kIntermediateState);
  xml()->DecreaseNesting("GsUpdate");

  endDeviceProcessing(pDevice);
}

void OdTrXmlRendition::onFrameSwitchInfo(OdUInt32 nInterval)
{
  OutputState(OdTrXmlServer::kIntermediateState);
  XML_LEVEL0("FrameSwitchInfo");
    xml()->DropUInt32("Interval", nInterval);
}

void OdTrXmlRendition::obtainClientSettings(ClientSettings &clientSets)
{
  clientSets = m_clientSettings;
}

OdTrVisSharingProviderClientPtr OdTrXmlRendition::createSharingProvider()
{
  if (clientContext())
    return clientContext()->createSharingProvider();
  return OdTrVisSharingProviderClientPtr();
}

OdTrVisRendition::MtHandlerType OdTrXmlRendition::mtDataTypeProcessing(DataType &type) const
{
  switch (type)
  {
    case kSurfaceData:
    case kViewportData:    case kViewportPropsData:
    case kVpOverrideData:  case kVpOverridePropsData:
    case kProgramData:     case kPropertiesData:
    case kOverlayData:     case kOverlayPropsData:
    return kMtRedirect; // Redirect without Mt handler since not called from Mt
    case kMetafileData:    case kMetafilePropsData:
    case kLightData:       case kLightPropsData:
    case kStateBranchData: case kStateBranchPropsData:
    case kTextureData:     case kTexturePropsData:
    case kMaterialData:    case kMaterialPropsData:
    case kVisualStyleData: case kVisualStylePropsData:
    case kLayerData:       case kLayerPropsData:
    case kGroupData:       case kGroupPropsData:
    case kExtensionData:   case kExtensionPropsData:
    return kMtGlobal; // Secure by global mutex since all passed through one XmlServer
    default:
      ODA_FAIL();
      return kMtRedirect;
  };
}

// Returns set of bit flags which indicate rendition capabilities/settings
OdUInt32 OdTrXmlRendition::queryRenditionCaps(OdUInt32 requestFor) const
{
  OdUInt32 retCaps = kSupportGsStateSaving | kSupportCompositeMetafiles | kSupportVisualStyles | kSupportOverlayBuffers;
  if (GETBIT(requestFor, kUpdateClientSectionOnly))
    SETBIT(retCaps, kUpdateClientSectionOnly,    xml()->ClientProcessingMode() == OdTrXmlServer::kClientDependentSectionOnly);
  if (GETBIT(requestFor, kUpdateShareableSectionOnly))
    SETBIT(retCaps, kUpdateShareableSectionOnly, xml()->ClientProcessingMode() == OdTrXmlServer::kShareableSectionOnly);
  return retCaps;
}

bool OdTrXmlRendition::isSupportCompositeMetafiles() const
{
  ODA_ASSERT(processingDevice());
  return processingDevice()->useCompositeMetafiles();
}
bool OdTrXmlRendition::isSupportVisualStyles() const
{
  ODA_ASSERT(processingDevice());
  return processingDevice()->useVisualStyles();
}
bool OdTrXmlRendition::isSupportOverlayBuffers() const
{
  ODA_ASSERT(processingDevice());
  return processingDevice()->useOverlays();
}

// Returns Id registrator, or NULL if no registration supported
OdTrVisRendition::IdRegistrator *OdTrXmlRendition::idRegistrator() const
{
  if (!getXmlServer() || xml()->ClientProcessingMode() != OdTrXmlServer::kSimpleMode)
    return &m_idReg;
  return NULL;
}

bool OdTrXmlRendition::saveRenditionState(OdGsFiler *pFiler) const
{
  pFiler->wrChar('X');
  // Save client settings
  //if (!::odTrVisSaveClientSettings(pFiler, m_clientSettings))
  //  return false;
  // Save Id mapping
  if (!m_idReg.save(pFiler))
    return false;

  xml()->saveState(pFiler);
  return true;
}

bool OdTrXmlRendition::loadRenditionState(OdGsFiler *pFiler, OdTrVisIdMap *pIdMap)
{
  if (pFiler->rdChar() != 'X')
    return false;
  // Load client settings
  //if (!::odTrVisLoadClientSettings(pFiler, m_clientSettings))
  //  return false;
  // Load Id mapping
  if (!m_idReg.load(pFiler, pIdMap))
    return false;

  xml()->loadState(pFiler, pIdMap);
  return true;
}

void OdTrXmlRendition::onSurfaceChanged(const OdTrVisSurfaceDef &pDef)
{
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_LEVEL0("SurfaceChanged");
    xml()->DropInt32("SurfaceSizeXMin", pDef.m_surfaceSize.m_min.x);
    xml()->DropInt32("SurfaceSizeYMin", pDef.m_surfaceSize.m_min.y);
    xml()->DropInt32("SurfaceSizeXMax", pDef.m_surfaceSize.m_max.x);
    xml()->DropInt32("SurfaceSizeYMax", pDef.m_surfaceSize.m_max.y);
    xml()->DropUInt8RGB("BackgroundColor", ODGETRED(pDef.m_backgroundColor), ODGETGREEN(pDef.m_backgroundColor), ODGETBLUE(pDef.m_backgroundColor), 255);
    xml()->DropBooleanOpt("TransparentBackground", pDef.isTransparentBackground());
}

void OdTrXmlRendition::onSurfaceInvalidated()
{
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_LEVEL0("SurfaceInvalidated");
}

struct xml_RegistrationBase
{
  OdTrXmlServer *m_pXml;
  OdTrVisUniqueID *m_pRegId;
  OdTrVisRendition::DataType m_dataType;
  OdTrXmlServer::IdType m_idType;
  OdTrVisId m_resId;

  xml_RegistrationBase(OdTrXmlServer *pXml, OdTrXmlRegistratorImpl &idReg, OdTrVisRendition::DataType dataType, OdTrVisId resId, bool idDel = false)
    : m_pXml(pXml)
    , m_pRegId(idReg.get(dataType, resId, idDel))
    , m_dataType(dataType)
    , m_idType(renditionDataType2ServerIdType(dataType))
    , m_resId(resId)
  {
  }
};
struct xml_RegisterId : public xml_RegistrationBase
{
  xml_RegisterId(OdTrXmlServer *pXml, OdTrXmlRegistratorImpl &idReg, OdTrVisRendition::DataType dataType, OdTrVisId resId)
    : xml_RegistrationBase(pXml, idReg, dataType, resId)
  {
    m_pXml->RegisterId(m_resId, m_pRegId, m_idType);
  }
};
struct xml_UnregisterId : public xml_RegistrationBase
{
  xml_UnregisterId(OdTrXmlServer *pXml, OdTrXmlRegistratorImpl &idReg, OdTrVisRendition::DataType dataType, OdTrVisId resId)
    : xml_RegistrationBase(pXml, idReg, dataType, resId) { }
  ~xml_UnregisterId()
  {
    m_pXml->UnregisterId(m_resId, m_pRegId, m_idType);
  }
};
#define XML_REGISTERED(dataType, resId) \
  xml_RegistrationBase _xmlReg(xml(), m_idReg, dataType, resId)
#define XML_REGISTER(dataType, resId) \
  xml_RegisterId _xmlReg(xml(), m_idReg, dataType, resId)
#define XML_UNREGISTER(dataType, resId) \
  xml_UnregisterId _xmlReg(xml(), m_idReg, dataType, resId)
#define XML_DROP_REG_ID(pTagName) \
  xml()->DropId(pTagName, _xmlReg.m_resId, _xmlReg.m_pRegId, _xmlReg.m_idType)

void OdTrXmlRendition::onOverlayAdded(OdTrVisOverlayId overlayId, const OdTrVisOverlayDef &pDef)
{
  XML_REGISTER(kOverlayData, overlayId);
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_LEVEL0("OverlayAdded");
    XML_DROP_REG_ID("OverlayID");
    xml()->DropUInt16("Flags", pDef.m_overlayFlags);
    xml()->DropInt16("RenderOrder", pDef.m_renderOrder);
}

void OdTrXmlRendition::onOverlayDeleted(OdTrVisOverlayId overlayId)
{
  XML_UNREGISTER(kOverlayData, overlayId);
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_LEVEL0("OverlayDeleted");
    XML_DROP_REG_ID("OverlayID");
}

void OdTrXmlRendition::onOverlayVisibilityChanged(OdTrVisOverlayId overlayId, bool bVisibility)
{
  XML_REGISTERED(kOverlayData, overlayId);
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_LEVEL0("OverlayVisibilityChanged");
    XML_DROP_REG_ID("OverlayID");
    xml()->DropBoolean("Visibility", bVisibility);
}

void OdTrXmlRendition::onOverlayRenderOrderChanged(OdTrVisOverlayId overlayId, OdInt16 nRenderOrder)
{
  XML_REGISTERED(kOverlayData, overlayId);
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_LEVEL0("OverlayRenderOrderChanged");
    XML_DROP_REG_ID("OverlayID");
    xml()->DropInt16("RenderOrder", nRenderOrder);
}

void OdTrXmlRendition::onViewportAdded(OdTrVisViewportId viewportId, const OdTrVisViewportDef &pDef)
{
  XML_REGISTER(kViewportData, viewportId);
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_LEVEL0("ViewportAdded");
    XML_DROP_REG_ID("ViewportID");
    DropViewportData(pDef, !idRegistrator() || !!_xmlReg.m_pRegId);
}

void OdTrXmlRendition::onViewportInserted(OdTrVisViewportId viewportId, int nIndex, const OdTrVisViewportDef &pDef)
{
  XML_REGISTER(kViewportData, viewportId);
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_LEVEL0("ViewportInserted");
    XML_DROP_REG_ID("ViewportID");
    xml()->DropInt32("Index", nIndex);
    DropViewportData(pDef, !idRegistrator() || !!_xmlReg.m_pRegId);
}

void OdTrXmlRendition::DropViewportData(const OdTrVisViewportDef &pDef, bool bDropOverallVpId)
{
  xml()->DropBooleanOpt("PSModelViewport", pDef.isPSModelViewport());
  if (pDef.m_overallViewportId && bDropOverallVpId)
    xml()->DropId("OverallVpId", pDef.overallViewportId(), m_idReg.get(kViewportData, pDef.overallViewportId()));
  xml()->DropBooleanOpt("PSOverallViewport", pDef.isPSOverallViewport());
  xml()->DropBooleanOpt("PSHelperViewport", pDef.isPSHelperViewport());
  xml()->DropBooleanOpt("ViewportVisible", pDef.isViewportVisible());
}

void OdTrXmlRendition::onViewportDeleted(OdTrVisViewportId viewportId)
{
  XML_UNREGISTER(kViewportData, viewportId);
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_LEVEL0("ViewportDeleted");
    XML_DROP_REG_ID("ViewportID");
}

void OdTrXmlRendition::onViewportVisibilityChanged(OdTrVisViewportId viewportId, bool bVisibility)
{
  XML_REGISTERED(kViewportData, viewportId);
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_LEVEL0("ViewportVisibilityChanged");
    XML_DROP_REG_ID("ViewportID");
    xml()->DropBoolean("Visibility", bVisibility);
}

void OdTrXmlRendition::DropViewParams(OdTrXmlServer *pXml, const OdTrVisViewParamsDef &pDef)
{
  // @@@TODO: remove this section when client will be completed
  pXml->DropMatrix("ScreenMatrix", pDef.m_screenMatrix);
  pXml->DropMatrix("ViewingMatrix", pDef.m_viewingMatrix);
  pXml->DropMatrix("ProjectionMatrix", pDef.m_projectionMatrix);
  pXml->DropMatrix("CorrectionMatrix", pDef.m_correctionMatrix);
  pXml->DropMatrix("OutputMatrix", pDef.m_outputMatrix);
  // @@@eof tmp section
  pXml->DropMatrix("MetafileMatrix", pDef.m_metafileMatrix);
  pXml->DropDouble3("ViewPosition", pDef.m_viewBasis.origin().x, pDef.m_viewBasis.origin().y, pDef.m_viewBasis.origin().z);
  pXml->DropDouble3("ViewBasisX", pDef.m_viewBasis.xAxis().x, pDef.m_viewBasis.xAxis().y, pDef.m_viewBasis.xAxis().z);
  pXml->DropDouble3("ViewBasisY", pDef.m_viewBasis.yAxis().x, pDef.m_viewBasis.yAxis().y, pDef.m_viewBasis.yAxis().z);
  pXml->DropDouble3("ViewBasisZ", pDef.m_viewBasis.zAxis().x, pDef.m_viewBasis.zAxis().y, pDef.m_viewBasis.zAxis().z);
  pXml->DropBoolean("PerspectiveProjection", pDef.m_bPerspectiveProjection);
  pXml->DropDouble("FieldWidth", pDef.m_fieldWidth);
  pXml->DropDouble("FieldHeight", pDef.m_fieldHeight);
  pXml->DropDouble("LensLength", pDef.m_lensLength);
  pXml->DropDouble3("ViewTarget", pDef.m_viewTarget.x, pDef.m_viewTarget.y, pDef.m_viewTarget.z);
  pXml->DropBoolean("FrontClip", pDef.m_frontClip.m_bClipped);
  if (pDef.m_frontClip.m_bClipped)
    pXml->DropDouble("FrontClipDist", pDef.m_frontClip.m_clipDist);
  pXml->DropBoolean("BackClip", pDef.m_backClip.m_bClipped);
  if (pDef.m_backClip.m_bClipped)
    pXml->DropDouble("BackClipDist", pDef.m_backClip.m_clipDist);
}

void OdTrXmlRendition::onViewportViewParamsModified(OdTrVisViewportId viewportId, const OdTrVisViewParamsDef &pDef)
{
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_REGISTERED(kViewportData, viewportId);
  XML_LEVEL0("ViewportViewParamsModified");
    XML_DROP_REG_ID("ViewportID");
    XML_LEVEL(1, "ViewParams");
      DropViewParams(xml(), pDef);
}

void OdTrXmlRendition::onViewportLineStylesModified(OdTrVisViewportId viewportId, const OdTrVisViewportLineStyleDef &pDef)
{
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_REGISTERED(kViewportData, viewportId);
  XML_LEVEL0("ViewportLineStylesModified");
    XML_DROP_REG_ID("ViewportID");
    xml()->DropUInt8RGB("LineStyleConfig", ODGETRED(pDef.getAsCref()), ODGETGREEN(pDef.getAsCref()), ODGETBLUE(pDef.getAsCref()), ODGETALPHA(pDef.getAsCref()));
}

void OdTrXmlRendition::onViewportFadingModified(OdTrVisViewportId viewportId, const OdTrVisViewportFadingDef &pDef)
{
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_REGISTERED(kViewportData, viewportId);
  XML_LEVEL0("ViewportFadingModified");
    XML_DROP_REG_ID("ViewportID");
    xml()->DropUInt8RGB("FadingConfig", ODGETRED(pDef.getAsCref()), ODGETGREEN(pDef.getAsCref()), ODGETBLUE(pDef.getAsCref()), ODGETALPHA(pDef.getAsCref()));
}

void OdTrXmlRendition::onViewportLineweightsModified(OdTrVisViewportId viewportId, const OdTrVisViewportLineweightsDef &pDef)
{
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_REGISTERED(kViewportData, viewportId);
  XML_LEVEL0("ViewportLineweightsModified");
    XML_DROP_REG_ID("ViewportID");
    xml()->DropUInt32("NLineweightEnum", pDef.numLineweightsEnum());
    xml()->DropDouble("LineweightCoef", pDef.lineweightsCoef());
    if (pDef.hasLineweightsEnum())
    {
      XML_LEVEL(1, "LineweightEnum");
      for (OdUInt32 n = 0; n < pDef.numLineweightsEnum(); n++)
        xml()->DropUInt8("NPixels", pDef.lineweightsEnum()[n]);
    }
}

void OdTrXmlRendition::onViewportShadingModified(OdTrVisViewportId viewportId, const OdTrVisViewportShadingDef &pDef)
{
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_REGISTERED(kViewportData, viewportId);
  XML_LEVEL0("ViewportShadingModified");
    XML_DROP_REG_ID("ViewportID");
    if (pDef.renderProgram() != kTrVisNegativeId) // Necessary for clients which support queryProgramId only
      xml()->DropId("ShaderId", pDef.renderProgram(), NULL, OdTrXmlServer::kProgramId);
    if (pDef.visualStyle() != kTrVisNegativeId) {
      xml()->DropId("VisualStyleId", pDef.visualStyle(), m_idReg.get(kVisualStyleData, pDef.visualStyle()));
    } else {
      xml()->DropId("VisualStyleId", kTrVisNegativeId);
    }
    ODA_ASSERT_ONCE(pDef.renderMode().controlVersion() == 5);
    xml()->DropBoolean("DepthBuffer", pDef.renderMode().depthBufferEnabled());
    xml()->DropBoolean("StencilBuffer", pDef.renderMode().stencilBufferEnabled());
    xml()->DropBoolean("PolygonOffset", pDef.renderMode().polygonOffsetEnabled());
    xml()->DropBoolean("Lighting", pDef.renderMode().lightingEnabled());
    xml()->DropBoolean("FaceNormals", pDef.renderMode().faceNormalsEnabled());
    xml()->DropUInt8RGB("FaceFillColor", ODGETRED(pDef.faceFillColor()), ODGETGREEN(pDef.faceFillColor()), ODGETBLUE(pDef.faceFillColor()), 255);
}

void OdTrXmlRendition::onViewportClipModified(OdTrVisViewportId viewportId, const OdTrVisViewportClipDef &pDef, bool bNrcClipModified, bool bBorderModified)
{
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_REGISTERED(kViewportData, viewportId);
  XML_LEVEL0("ViewportClipModified");
    XML_DROP_REG_ID("ViewportID");
    { OdGsDCRectDouble screenRect; pDef.localXform().localViewportRect(screenRect);
      xml()->DropDouble4("ScreenRect", screenRect.m_min.x, screenRect.m_min.y,
                                       screenRect.m_max.x, screenRect.m_max.y);
    }
    if (bNrcClipModified)
    {
      xml()->DropBoolean("NrcClip", pDef.isNrcClipEnabled());
      //if (pDef.isNrcClipEnabled()) // We must send clipping data even if it is disabled
      {
        xml()->DropUInt32("NNrcCounts", pDef.nrcClipRegion().numCounts());
        xml()->DropUInt32("NNrcPoints", pDef.nrcClipRegion().numPoints());
        if (pDef.nrcClipRegion().hasCounts())
        { XML_LEVEL(1, "NrcCounts");
          for (OdUInt32 n = 0; n < pDef.nrcClipRegion().numCounts(); n++)
            xml()->DropInt32("Count", pDef.nrcClipRegion().counts()[n]);
        }
        if (pDef.nrcClipRegion().hasPoints())
        { XML_LEVEL(1, "NrcPoints"); float pPoint[2];
          for (OdUInt32 n = 0; n < pDef.nrcClipRegion().numPoints() * 2; n += 2)
          { pDef.localXform().localPoint(pPoint, pDef.nrcClipRegion().points() + n);
            xml()->DropFloat2("Point", pPoint[0], pPoint[1]);
          }
        }
      }
    }
    if (bBorderModified)
    {
      xml()->DropBoolean("Border", pDef.isBorderEnabled());
      //if (pDef.isBorderEnabled()) // We must send border data even if it is disabled
      {
        xml()->DropUInt8RGB("BorderColor", ODGETRED(pDef.border().color()), ODGETGREEN(pDef.border().color()), ODGETBLUE(pDef.border().color()), 255);
        xml()->DropInt32("BorderWidth", pDef.border().width());
        xml()->DropUInt32("NBorderCounts", pDef.border().region().numCounts());
        xml()->DropUInt32("NBorderPoints", pDef.border().region().numPoints());
        if (pDef.border().region().hasCounts())
        { XML_LEVEL(1, "BorderCounts");
          for (OdUInt32 n = 0; n < pDef.border().region().numCounts(); n++)
            xml()->DropInt32("Count", pDef.border().region().counts()[n]);
        }
        if (pDef.border().region().hasPoints())
        { XML_LEVEL(1, "BorderPoints"); float pPoint[2];
          for (OdUInt32 n = 0; n < pDef.border().region().numPoints() * 2; n += 2)
          { pDef.localXform().localPoint(pPoint, pDef.border().region().points() + n);
            xml()->DropFloat2("Point", pPoint[0], pPoint[1]);
          }
        }
      }
    }
}

void OdTrXmlRendition::onViewportBackgroundChanged(OdTrVisViewportId viewportId, OdTrVisFlatMetafileContainerPtr pStream, OdTrVisProgramId baseProgramId)
{
  OutputState(OdTrXmlServer::kShareableClientState);
  XML_LEVEL0("ViewportBackgroundChanged");
    xml()->DropId("ViewportID", viewportId, m_idReg.get(kViewportData, viewportId), OdTrXmlServer::kViewportId);
    if (baseProgramId != kTrVisNegativeId) // Neccessary for clients which support queryProgramId only
      xml()->DropId("BaseProgramID", baseProgramId, NULL, OdTrXmlServer::kProgramId);
    xml()->DropBoolean("Visible", !pStream.isNull());
    if (!pStream.isNull())
      DropMetadata(pStream);
}

void OdTrXmlRendition::onViewportFogChanged(OdTrVisViewportId viewportId, const OdTrVisFogDef &pDef)
{
  OutputState(OdTrXmlServer::kShareableClientState);
  XML_REGISTERED(kViewportData, viewportId);
  XML_LEVEL0("ViewportFogChanged");
    XML_DROP_REG_ID("ViewportID");
    xml()->DropBooleanOpt("Enable", pDef.isFogEnabled());
    xml()->DropBooleanOpt("Background", pDef.isFogAtBackgroundEnabled());
    if (pDef.isFogEnabled())
    {
      xml()->DropUInt8RGB("Color", ODGETRED(pDef.m_color), ODGETGREEN(pDef.m_color), ODGETBLUE(pDef.m_color), 255);
      xml()->DropDouble2("Distance", pDef.nearDistance(), pDef.farDistance());
      xml()->DropDouble2("Density", pDef.nearDensity(), pDef.farDensity());
    }
}

void OdTrXmlRendition::onViewportSectioningChanged(OdTrVisViewportId viewportId, const OdTrVisSectionDef &pDef)
{
  OutputState(OdTrXmlServer::kShareableClientState);
  XML_REGISTERED(kViewportData, viewportId);
  XML_LEVEL0("ViewportSectioningChanged");
    XML_DROP_REG_ID("ViewportID");
    xml()->DropUInt32("NPlanes", pDef.numSectionPlanes());
    if (pDef.hasSectionPlanes())
    {
      { XML_LEVEL(1, "PlanesList");
        for (OdUInt32 n = 0; n < pDef.numSectionPlanes(); n++)
        {
          XML_LEVEL(2, "Plane");
            const OdTrVisSectionPlane &secPlane = pDef.sectionPlane(n);
            xml()->DropFloat3("Row0", secPlane.entry[0][0], secPlane.entry[0][1], secPlane.entry[0][2]);
            xml()->DropFloat3("Row1", secPlane.entry[1][0], secPlane.entry[1][1], secPlane.entry[1][2]);
            xml()->DropFloat3("Row2", secPlane.entry[2][0], secPlane.entry[2][1], secPlane.entry[2][2]);
        }
      }
      xml()->DropBooleanOpt("EnableFilling", pDef.isFillingEnabled());
      if (pDef.isFillingEnabled())
      {
        xml()->DropUInt8RGB("Color", ODGETRED(pDef.fillColor()), ODGETGREEN(pDef.fillColor()), ODGETBLUE(pDef.fillColor()), ODGETALPHA(pDef.fillColor()));
        xml()->DropBooleanOpt("EnableStippling", pDef.isStipplingEnabled());
        if (pDef.isStipplingEnabled())
        {
          xml()->DropUInt8("StipplePattern", pDef.stipplePattern());
          xml()->DropUInt8RGB("StipplingColor", ODGETRED(pDef.stippleColor()), ODGETGREEN(pDef.stippleColor()), ODGETBLUE(pDef.stippleColor()), ODGETALPHA(pDef.stippleColor()));
        }
      }
    }
}

void OdTrXmlRendition::onViewportAntiAliasingChanged(OdTrVisViewportId viewportId, OdUInt32 nAntiAliasing)
{
  XML_REGISTERED(kViewportData, viewportId);
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_LEVEL0("ViewportAntiAliasingChanged");
    XML_DROP_REG_ID("ViewportID");
    xml()->DropUInt32("AntiAliasing", nAntiAliasing);
}

void OdTrXmlRendition::onViewportAmbientLightChanged(OdTrVisViewportId viewportId, const OdTrVisMaterialColor &ambientLight)
{
  OutputState(OdTrXmlServer::kShareableClientState);
  XML_REGISTERED(kViewportData, viewportId);
  XML_LEVEL0("ViewportAmbientLightChanged");
    XML_DROP_REG_ID("ViewportID");
    xml()->DropFloat4("AmbientLight", ambientLight.red(), ambientLight.green(), ambientLight.blue(), ambientLight.alpha());
}

void OdTrXmlRendition::onViewportDefaultLightsChanged(OdTrVisViewportId viewportId, bool bDefaultLightsOn,
                                                      const OdTrVisLightDef *pLightsList, OdUInt32 nLightsListEntries)
{
  OutputState(OdTrXmlServer::kShareableClientState);
  XML_REGISTERED(kViewportData, viewportId);
  XML_LEVEL0("ViewportDefaultLightsChanged");
    XML_DROP_REG_ID("ViewportID");
    xml()->DropBoolean("DefaultLightsOn", bDefaultLightsOn);
    xml()->DropUInt32("NLights", nLightsListEntries);
    if (nLightsListEntries)
    {
      XML_LEVEL(1, "LightsList");
        for (OdUInt32 n = 0; n < nLightsListEntries; n++)
        {
          XML_LEVEL(2, "Light");
            DropLightData(pLightsList[n]);
        }
    }
}

void OdTrXmlRendition::onViewportLightAttached(OdTrVisViewportId viewportId, OdTrVisLightId lightId,
                                               OdUInt32 nOverrides, const OdTrVisLightDef &pOverrides)
{
  XML_REGISTERED(kViewportData, viewportId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("ViewportLightAttached");
    XML_DROP_REG_ID("ViewportID");
    xml()->DropId("LightID", lightId, m_idReg.get(kLightData, lightId), OdTrXmlServer::kLightId);
    xml()->DropUInt32("NOverrides", nOverrides);
    if (nOverrides)
      DropLightData(pOverrides, nOverrides);
}

void OdTrXmlRendition::onViewportLightDetached(OdTrVisViewportId viewportId, OdTrVisLightId lightId)
{
  XML_REGISTERED(kViewportData, viewportId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("ViewportLightDetached");
    XML_DROP_REG_ID("ViewportID");
    xml()->DropId("LightID", lightId, m_idReg.get(kLightData, lightId), OdTrXmlServer::kLightId);
}

void OdTrXmlRendition::onOverlayViewParamsOverride(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, bool bOverride, const OdTrVisViewParamsDef &pDef)
{
  OutputState(OdTrXmlServer::kClientDependentState);
  XML_LEVEL0("OverlayViewParamsOverride");
    xml()->DropId("ViewportID", viewportId, m_idReg.get(kViewportData, viewportId), OdTrXmlServer::kViewportId);
    xml()->DropId("OverlayID", overlayId, m_idReg.get(kOverlayData, overlayId), OdTrXmlServer::kOverlayId);
    xml()->DropBoolean("Override", bOverride);
    if (bOverride)
    {
      XML_LEVEL(1, "ViewParams");
      DropViewParams(xml(), pDef);
    }
}

void OdTrXmlRendition::onOverlayExtentsOverride(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisExtentsDef &pDef)
{
  OutputState(OdTrXmlServer::kShareableClientState);
  XML_REGISTERED(kViewportData, viewportId);

  XML_LEVEL0("OverlayExtentsOverride");
    xml()->DropId("ViewportID", viewportId, _xmlReg.m_pRegId, OdTrXmlServer::kViewportId);
    xml()->DropId("OverlayID", overlayId, m_idReg.get(kOverlayData, overlayId), OdTrXmlServer::kOverlayId);
    xml()->DropDouble3("SceneMin", pDef.m_sceneExtents.minPoint().x, pDef.m_sceneExtents.minPoint().y, pDef.m_sceneExtents.minPoint().z);
    xml()->DropDouble3("SceneMax", pDef.m_sceneExtents.maxPoint().x, pDef.m_sceneExtents.maxPoint().y, pDef.m_sceneExtents.maxPoint().z);
    xml()->DropDouble3("RealMin", pDef.m_realExtents.minPoint().x, pDef.m_realExtents.minPoint().y, pDef.m_realExtents.minPoint().z);
    xml()->DropDouble3("RealMax", pDef.m_realExtents.maxPoint().x, pDef.m_realExtents.maxPoint().y, pDef.m_realExtents.maxPoint().z);
}

void OdTrXmlRendition::onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                                 const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen)
{
  onMetafileOrderChangedImpl(viewportId, overlayId, nInvalidFrom, nInvalidLen, pValidFrom, nValidLen, !!idRegistrator());
}

void OdTrXmlRendition::onMetafileOrderInheritance( OdTrVisViewportId viewportId1, OdTrVisOverlayId overlayId1, OdTrVisViewportId viewportId2 )
{
  onMetafileOrderInheritanceImpl( viewportId1, overlayId1, viewportId2 );
}

bool OdTrXmlRendition::DropDisplayList(const OdTrVisDisplayId *pDispList, OdUInt32 nListLen, DDL2ndPassInfo *p2ndPass)
{
  for (OdUInt32 n = 0; n < nListLen; n++)
  {
    ODA_ASSERT(pDispList[n] >= kDisplayCodeRange);
    OdTrVisUniqueID *pRegEntry = m_idReg.get(kMetafileData, pDispList[n]);
    if (!p2ndPass || pRegEntry)
      xml()->DropId("MetafileID", pDispList[n], pRegEntry, OdTrXmlServer::kDisplayId);
    else
    {
      if (!p2ndPass->m_bScan)
      {
        p2ndPass->m_bScan = true;
        p2ndPass->m_nTransLast = p2ndPass->m_nTransFirst = n;
      }
      else
        p2ndPass->m_nTransLast = n;
      xml()->DropUInt32("MetafileID", 0);
    }
  }
  if (p2ndPass)
    return p2ndPass->m_bScan;
  return false;
}

void OdTrXmlRendition::DropDisplayList2ndPass(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom,
                                                 const OdTrVisDisplayId *pDispList, OdUInt32 nListLen, DDL2ndPassInfo &p2ndPass)
{
  OdUInt32 nScan = p2ndPass.m_nTransFirst;
  while (nScan <= p2ndPass.m_nTransLast)
  {
    OdUInt32 nScanLast = nScan + 1;
    while (nScanLast <= p2ndPass.m_nTransLast)
    {
      if (m_idReg.get(kMetafileData, pDispList[nScanLast]))
        break;
      nScanLast++;
    }
    onMetafileOrderChangedImpl(viewportId, overlayId, nInvalidFrom + nScan, nScanLast - nScan, pDispList + nScan, nScanLast - nScan, false);
    nScan = nScanLast + 1;
    while (nScan <= p2ndPass.m_nTransLast)
    {
      if (!m_idReg.get(kMetafileData, pDispList[nScan]))
        break;
      nScan++;
    }
  }
}

void OdTrXmlRendition::onMetafileOrderChangedImpl(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                                     const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen, bool bFirstPass)
{
  OutputState((bFirstPass) ? OdTrXmlServer::kShareableClientState : OdTrXmlServer::kClientOnlyState);
  DDL2ndPassInfo s2ndPass;
  {
    XML_LEVEL0("MetafileOrderChanged");
      xml()->DropId("ViewportID", viewportId, m_idReg.get(kViewportData, viewportId), OdTrXmlServer::kViewportId);
      xml()->DropId("OverlayID", overlayId, m_idReg.get(kOverlayData, overlayId), OdTrXmlServer::kOverlayId);
      xml()->DropUInt32("InvalidFrom", nInvalidFrom);
      xml()->DropUInt32("InvalidLen", nInvalidLen);
      xml()->DropUInt32("ValidLen", nValidLen);
      if (nValidLen)
      {
        XML_LEVEL(1, "Ordering");
          DropDisplayList(pValidFrom, nValidLen, (bFirstPass) ? &s2ndPass : NULL);
      }
  }
  if (s2ndPass.m_bScan)
    DropDisplayList2ndPass(viewportId, overlayId, nInvalidFrom, pValidFrom, nValidLen, s2ndPass);
}

void OdTrXmlRendition::onMetafileOrderInheritanceImpl( OdTrVisViewportId viewportId1, OdTrVisOverlayId overlayId1, OdTrVisViewportId viewportId2 )
{
  OutputState(OdTrXmlServer::kShareableClientState);
  XML_LEVEL0("MetafileOrderInheritance");
    xml()->DropId("ViewportID1", viewportId1, m_idReg.get(kViewportData, viewportId1), OdTrXmlServer::kViewportId);
    xml()->DropId("OverlayID", overlayId1, m_idReg.get(kOverlayData, overlayId1), OdTrXmlServer::kOverlayId);
    xml()->DropId("ViewportID2", viewportId2, m_idReg.get(kViewportData, viewportId2), OdTrXmlServer::kViewportId);
}

OdTrVisDisplayId OdTrXmlRendition::onMetafileAdded(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef)
{
  XML_REGISTER(kMetafileData, metafileId);
  // OdAnsiString str = _xmlReg.m_pRegId->m_uId.transform();
  // printf("uId : %s\n", str.c_str());
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  {
    XML_LEVEL0("MetafileAdded");
    XML_DROP_REG_ID("MetafileID");
    xml()->DropId("GeneratorID", pDef.m_generatorId, m_idReg.get(kViewportData, pDef.m_generatorId), OdTrXmlServer::kViewportId);
    xml()->DropBooleanOpt("Visibility", pDef.m_pMetafile->isMetafileVisible());
#ifndef OD_TRXML_DROPHLTASCLIENTONLY
    xml()->DropBooleanOpt("Highlighted", pDef.m_pMetafile->isMetafileHighlighted());
#else
    xml()->DropBooleanOpt("Highlighted", pDef.m_pMetafile->isMetafileHighlighted() && !_xmlReg.m_pRegId);
#endif
    xml()->DropBooleanOpt("Temporary", pDef.m_pMetafile->isTemporaryMetafile());
    xml()->DropBooleanOpt("Nested", pDef.m_pMetafile->isNestedMetafile());
    xml()->DropBooleanOpt("BlockRef", pDef.m_pMetafile->isBlockRefMetafile());
    xml()->DropBooleanOpt("Utilitary", pDef.m_pMetafile->isUtilitaryMetafile());
    xml()->DropBooleanOpt("Sectionable", pDef.m_pMetafile->isSectionableMetafile());
    xml()->DropBooleanOpt("Composite", pDef.m_pMetafile->isCompositeMetafile());
    xml()->DropBooleanOpt("XRef", pDef.m_pMetafile->isXRefMetafile());
    xml()->DropBooleanOpt("Faded", pDef.m_pMetafile->isMetafileFaded());
    xml()->DropBooleanOpt("ShadowsCast", pDef.m_pMetafile->isMetafileCastShadows());
    xml()->DropBooleanOpt("ShadowsReceive", pDef.m_pMetafile->isMetafileReceiveShadows());
    xml()->DropDouble3("ExtMin", pDef.m_extents.minPoint().x, pDef.m_extents.minPoint().y, pDef.m_extents.minPoint().z);
    xml()->DropDouble3("ExtMax", pDef.m_extents.maxPoint().x, pDef.m_extents.maxPoint().y, pDef.m_extents.maxPoint().z);

    if (!xml()->DropMetafileAdded(*this, metafileId, pDef))
      DropMetadata(pDef.m_pMetafile.get());

#ifdef OD_TRXML_DROPHLTASCLIENTONLY
    if (_xmlReg.m_pRegId && pDef.m_pMetafile->isMetafileHighlighted())
      onMetafileHighlightingChanged(metafileId, true);
#endif
  }
  return (OdTrVisDisplayId)metafileId;
}

void OdTrXmlRendition::onMetafileDeleted(OdTrVisMetafileId metafileId)
{
  XML_UNREGISTER(kMetafileData, metafileId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("MetafileDeleted");
    XML_DROP_REG_ID("MetafileID");
}

void OdTrXmlRendition::onMetafileVisibilityChanged(OdTrVisMetafileId metafileId, bool bVisibility)
{
  XML_REGISTERED(kMetafileData, metafileId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("MetafileVisibilityChanged");
    XML_DROP_REG_ID("MetafileID");
    xml()->DropBoolean("Visibility", bVisibility);
}

void OdTrXmlRendition::onMetafileHighlightingChanged(OdTrVisMetafileId metafileId, bool bHighlight)
{
  XML_REGISTERED(kMetafileData, metafileId);
#ifndef OD_TRXML_DROPHLTASCLIENTONLY
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
#else
  OutputState(OdTrXmlServer::kClientOnlyState);
#endif
  XML_LEVEL0("MetafileHighlightingChanged");
    XML_DROP_REG_ID("MetafileID");
    xml()->DropBoolean("Highlight", bHighlight);
}

void OdTrXmlRendition::onMetafileFadingChanged(OdTrVisMetafileId metafileId, bool bFade)
{
  XML_REGISTERED(kMetafileData, metafileId);
#ifndef OD_TRXML_DROPHLTASCLIENTONLY
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
#else
  OutputState(OdTrXmlServer::kClientOnlyState);
#endif
  XML_LEVEL0("MetafileFadingChanged");
    XML_DROP_REG_ID("MetafileID");
    xml()->DropBoolean("Fade", bFade);
}

void OdTrXmlRendition::onLightAdded(OdTrVisLightId lightId, const OdTrVisLightDef &pDef)
{
  XML_REGISTER(kLightData, lightId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("LightAdded");
    XML_DROP_REG_ID("LightID");
    DropLightData(pDef);
}

void OdTrXmlRendition::onLightModified(OdTrVisLightId lightId, OdUInt32 nKindOfMod, const OdTrVisLightDef &pDef)
{
  XML_REGISTERED(kLightData, lightId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("LightModified");
    XML_DROP_REG_ID("LightID");
    xml()->DropUInt32("NModified", nKindOfMod);
    DropLightData(pDef, nKindOfMod);
}

void OdTrXmlRendition::onLightDeleted(OdTrVisLightId lightId)
{
  XML_UNREGISTER(kLightData, lightId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("LightDeleted");
    XML_DROP_REG_ID("LightID");
}

void OdTrXmlRendition::onStateBranchAdded(OdTrVisStateBranchId stateBranchId, const OdTrVisStateBranchDef &pDef)
{
  XML_REGISTER(kStateBranchData, stateBranchId);
#ifndef OD_TRXML_DROPHLTASCLIENTONLY
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
#else
  OutputState(OdTrXmlServer::kClientOnlyState);
#endif
  XML_LEVEL0("StateBranchAdded");
    XML_DROP_REG_ID("StateBranchID");
    DropStateBranch(pDef, true, true);
}

void OdTrXmlRendition::onStateBranchModified(OdTrVisStateBranchId stateBranchId, const OdTrVisStateBranchDef &pDef,
                                             bool bChildsModified, bool bMarkersModified)
{
  XML_REGISTERED(kStateBranchData, stateBranchId);
#ifndef OD_TRXML_DROPHLTASCLIENTONLY
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
#else
  OutputState(OdTrXmlServer::kClientOnlyState);
#endif
  XML_LEVEL0("StateBranchModified");
    XML_DROP_REG_ID("StateBranchID");
    DropStateBranch(pDef, bChildsModified, bMarkersModified);
}

void OdTrXmlRendition::onStateBranchDeleted(OdTrVisStateBranchId stateBranchId)
{
  XML_UNREGISTER(kStateBranchData, stateBranchId);
#ifndef OD_TRXML_DROPHLTASCLIENTONLY
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
#else
  OutputState(OdTrXmlServer::kClientOnlyState);
#endif
  XML_LEVEL0("StateBranchDeleted");
    XML_DROP_REG_ID("StateBranchID");
}

void OdTrXmlRendition::onTextureAdded(OdTrVisTextureId textureId, const OdTrVisTextureDef &pDef)
{
  XML_REGISTER(kTextureData, textureId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableClientState : OdTrXmlServer::kClientOnlyState);

  OdTrVisTexturePtr pTexture = pDef.texture();
  if (pTexture.isNull())
    throw OdError(eNoDocument);

  XML_LEVEL0("TextureAdded");
    xml()->DropId("TextureID", textureId, _xmlReg.m_pRegId, OdTrXmlServer::kTextureId);
    xml()->DropUInt32("Type", (OdUInt32)pTexture->format());
    xml()->DropBooleanOpt("Smooth", pDef.smoothFilterEnabled());
    xml()->DropBooleanOpt("Modulate", pDef.colorModulationEnabled());
    xml()->DropBooleanOpt("IntensityMap", pDef.isIntensityMap());
    xml()->DropBooleanOpt("Negative", pDef.isNegativeImage());
    xml()->DropBooleanOpt("NormalMap", pDef.isNormalMap());
    xml()->DropBooleanOpt("SphereMap", pDef.isSphericalMap());
    xml()->DropBooleanOpt("CubeMap", pDef.isCubeMap());
    xml()->DropBooleanOpt("Grayscale", pDef.isGrayscaleImage());
    xml()->DropUInt32("Width", pTexture->getTextureWidth());
    xml()->DropUInt32("Height", pTexture->getTextureHeight());
    xml()->DropUInt32("ScanlineLength", pTexture->getScanlineLength());
    xml()->DropUInt32("DataAlignment", pTexture->getDataAlignment());
    xml()->DropBinaryStream("TextureData", pTexture->textureData(), pTexture->getScanlineLength() * pTexture->getTextureHeight() );
    const OdTrVisTexture* pal = pTexture->palette();
    if( pal )
    {
      xml()->DropUInt32("PaletteType", pal->format() );
      xml()->DropUInt32("PaletteWidth", pal->getTextureWidth());
      xml()->DropUInt32("PaletteScanlineLength", pal->getScanlineLength());
      xml()->DropUInt32("PaletteDataAlignment", pal->getDataAlignment());
      xml()->DropBinaryStream("PaletteData", pal->textureData(), pal->getScanlineLength() * pal->getTextureHeight() );
    }
}

void OdTrXmlRendition::onTextureDeleted(OdTrVisTextureId textureId)
{
  XML_UNREGISTER(kTextureData, textureId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableClientState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("TextureDeleted");
    XML_DROP_REG_ID("TextureID");
}

void OdTrXmlRendition::onMaterialAdded(OdTrVisMaterialId materialId, const OdTrVisMaterialDef &pDef)
{
  XML_REGISTER(kMaterialData, materialId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("MaterialAdded");
    XML_DROP_REG_ID("MaterialID");
    if (pDef.m_ambientChannel.isEnabled())
    { XML_LEVEL(1, "AmbientChannel");
      DropMaterialChannel(pDef.m_ambientChannel);
    }
    if (pDef.m_diffuseChannel.isEnabled())
    { XML_LEVEL(1, "DiffuseChannel");
      DropMaterialChannel(pDef.m_diffuseChannel);
    }
    if (pDef.m_specularChannel.isEnabled())
    { XML_LEVEL(1, "SpecularChannel");
      DropMaterialChannel(pDef.m_specularChannel);
    }
    if (pDef.m_emissionChannel.isEnabled())
    { XML_LEVEL(1, "EmissionChannel");
      DropMaterialChannel(pDef.m_emissionChannel);
    }
    if (pDef.m_opacityChannel.isEnabled())
    { XML_LEVEL(1, "OpacityChannel");
      DropMaterialChannel(pDef.m_opacityChannel);
    }
    if (pDef.m_bumpMapChannel.isEnabled())
    { XML_LEVEL(1, "BumpMapChannel");
      DropMaterialChannel(pDef.m_bumpMapChannel);
    }
    if (pDef.m_reflectChannel.isEnabled())
    { XML_LEVEL(1, "ReflectionChannel");
      DropMaterialChannel(pDef.m_reflectChannel);
    }
    if (pDef.m_refractChannel.isEnabled())
    { XML_LEVEL(1, "RefractionChannel");
      DropMaterialChannel(pDef.m_refractChannel);
    }
    if (pDef.m_normMapChannel.isEnabled())
    { XML_LEVEL(1, "NormalMapChannel");
      DropMaterialChannel(pDef.m_normMapChannel);
    }
    if (pDef.m_tintChannel.isEnabled())
    { XML_LEVEL(1, "TintChannel");
      DropMaterialChannel(pDef.m_tintChannel);
    }
}

void OdTrXmlRendition::onMaterialDeleted(OdTrVisMaterialId materialId)
{
  XML_UNREGISTER(kMaterialData, materialId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("MaterialDeleted");
    XML_DROP_REG_ID("MaterialID");
}

void OdTrXmlRendition::onVisualStyleAdded(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef)
{
  XML_REGISTER(kVisualStyleData, visualStyleId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("VisualStyleAdded");
    XML_DROP_REG_ID("VisualStyleID");
    DropVisualStyleData(pDef);
}

void OdTrXmlRendition::onVisualStyleModified(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef)
{
  XML_REGISTERED(kVisualStyleData, visualStyleId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("VisualStyleModified");
    XML_DROP_REG_ID("VisualStyleID");
    DropVisualStyleData(pDef, false);
}

void OdTrXmlRendition::onVisualStyleDeleted(OdTrVisVisualStyleId visualStyleId)
{
  XML_UNREGISTER(kVisualStyleData, visualStyleId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("VisualStyleDeleted");
    XML_DROP_REG_ID("VisualStyleID");
}

void OdTrXmlRendition::onLayerAdded(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef)
{
  XML_REGISTER(kLayerData, layerId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("LayerAdded");
    XML_DROP_REG_ID("LayerID");
    DropLayerProps(pDef.m_props);
}

void OdTrXmlRendition::onLayerDeleted(OdTrVisLayerId layerId)
{
  XML_UNREGISTER(kLayerData, layerId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("LayerDeleted");
    XML_DROP_REG_ID("LayerID");
}

void OdTrXmlRendition::onLayerVisibilityChanged(OdTrVisLayerId layerId, bool bVisibility)
{
  XML_REGISTERED(kLayerData, layerId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("LayerVisibilityChanged");
    XML_DROP_REG_ID("LayerID");
    xml()->DropBoolean("Visibility", bVisibility);
}

void OdTrXmlRendition::onLayerFadingChanged(OdTrVisLayerId layerId, bool bFade)
{
  XML_REGISTERED(kLayerData, layerId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("LayerFadingChanged");
    XML_DROP_REG_ID("LayerID");
    xml()->DropBoolean("Fade", bFade);
}

void OdTrXmlRendition::onLayerVpDepPropsOverride(OdTrVisLayerId layerId, OdTrVisViewportId viewportId, bool bOverride, const OdTrVisLayerProps &pProps)
{
  XML_REGISTERED(kLayerData, layerId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("LayerVpDepPropsOverride");
    XML_DROP_REG_ID("LayerID");
    xml()->DropId("ViewportID", viewportId, m_idReg.get(kViewportData, viewportId), OdTrXmlServer::kViewportId);
    xml()->DropBoolean("Override", bOverride);
    if (bOverride)
    {
      XML_LEVEL(1, "VpDepSpec");
        DropLayerProps(pProps);
    }
}

void OdTrXmlRendition::onExtensionObjectAdded(OdTrVisExtensionId extId, const OdTrVisExtensionObject *pObj)
{
  XML_REGISTER(kExtensionData, extId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("ExtensionObjectAdded");
    XML_DROP_REG_ID("ExtensionID");
    xml()->DropString("ModuleName", pObj->extensionModule()->moduleName());
    xml()->DropUInt32("ExtVersion", pObj->extensionModule()->extensionVersion());
    xml()->DropUInt32("ObjectType", pObj->moduleObjectType());
    {
      class FlatMemStream : public OdStaticRxObject<OdFlatMemStream>
      {
        protected:
          OdUInt8Vector m_vData;
        public:
          FlatMemStream() {}

          void putByte(OdUInt8 value)
          {
            if (m_nCurPos + 1 > m_nEndPos)
            {
              m_vData.resize(m_nCurPos + 1);
              init(m_vData.asArrayPtr(), 1, m_nCurPos);
            }
            OdFlatMemStream::putByte(value);
          }
          void putBytes(const void *buffer, OdUInt32 numBytes)
          {
            if (m_nCurPos + numBytes > m_nEndPos)
            {
              m_vData.resize(m_nCurPos + numBytes);
              init(m_vData.asArrayPtr(), numBytes, m_nCurPos);
            }
            OdFlatMemStream::putBytes(buffer, numBytes);
          }
      } memoryStream;
      pObj->serialize(&memoryStream);
      xml()->DropBinaryStream("ObjectData", memoryStream.data(), memoryStream.length());
    }
}

void OdTrXmlRendition::onExtensionObjectDeleted(OdTrVisExtensionId extId)
{
  XML_UNREGISTER(kExtensionData, extId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("ExtensionObjectDeleted");
    XML_DROP_REG_ID("ExtensionID");
}

void OdTrXmlRendition::onExtensionObjectSpecificData(OdTrVisExtensionId extId, OdUInt32 nDataId, const OdTrVisPropertyDef &propDef)
{
  XML_REGISTERED(kExtensionData, extId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("ExtensionObjectSpecificData");
    XML_DROP_REG_ID("ExtensionID");
    xml()->DropUInt32("DataID", nDataId);
    xml()->DropInt32("PropType", (OdInt32)propDef.propertyType());
    DropPropertyData(propDef);
}

void OdTrXmlRendition::onExtensionObjectVisibilityChanged(OdTrVisExtensionId extId, bool bVisibility)
{
  XML_REGISTERED(kExtensionData, extId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("ExtensionObjectVisibilityChanged");
    XML_DROP_REG_ID("ExtensionID");
    xml()->DropBoolean("Visibility", bVisibility);
}

void OdTrXmlRendition::onGroupAdded(OdTrVisGroupId groupId)
{
  XML_REGISTER(kGroupData, groupId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("GroupAdded");
    XML_DROP_REG_ID("GroupID");
}

void OdTrXmlRendition::onGroupDeleted(OdTrVisGroupId groupId)
{
  XML_UNREGISTER(kGroupData, groupId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("GroupDeleted");
    XML_DROP_REG_ID("GroupID");
}

void OdTrXmlRendition::onGroupMetafilesAttached(OdTrVisGroupId groupId, const OdTrVisMetafileId *pMetafilesList, OdUInt32 nMetafiles)
{
  XML_REGISTERED(kGroupData, groupId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("GroupMetafilesAttached");
    XML_DROP_REG_ID("GroupID");
    xml()->DropUInt32("NMetafiles", nMetafiles);
    if (nMetafiles)
    { XML_LEVEL(1, "Metafiles");
      do
      {
        xml()->DropId("MetafileID", *pMetafilesList, m_idReg.get(kMetafileData, *pMetafilesList), OdTrXmlServer::kMetafileId);
        pMetafilesList++; nMetafiles--;
      } while (nMetafiles);
    }
}

void OdTrXmlRendition::onGroupMetafilesDetached(OdTrVisGroupId groupId, const OdTrVisMetafileId *pMetafilesList, OdUInt32 nMetafiles)
{
  XML_REGISTERED(kGroupData, groupId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("GroupMetafilesDetached");
    XML_DROP_REG_ID("GroupID");
    xml()->DropUInt32("NMetafiles", nMetafiles);
    if (nMetafiles)
    { XML_LEVEL(1, "Metafiles");
      do
      {
        xml()->DropId("MetafileID", *pMetafilesList, m_idReg.get(kMetafileData, *pMetafilesList), OdTrXmlServer::kMetafileId);
        pMetafilesList++; nMetafiles--;
      } while (nMetafiles);
    }
}

//void OdTrXmlRendition::onGroupMetafilesDeleted(OdTrVisGroupId groupId)
//{
//  XML_REGISTERED(kGroupData, groupId);
//  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
//  XML_LEVEL0("GroupMetafilesDeleted");
//    XML_DROP_REG_ID("GroupID");
//}

void OdTrXmlRendition::onGroupMetafilesVisibilityChanged(OdTrVisGroupId groupId, bool bVisibility)
{
  XML_REGISTERED(kGroupData, groupId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("GroupMetafilesVisibilityChanged");
    XML_DROP_REG_ID("GroupID");
    xml()->DropBoolean("Visibility", bVisibility);
}

void OdTrXmlRendition::onGroupMetafilesHighlightingChanged(OdTrVisGroupId groupId, bool bHighlight)
{
  XML_REGISTERED(kGroupData, groupId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("GroupMetafilesHighlightingChanged");
    XML_DROP_REG_ID("GroupID");
    xml()->DropBoolean("Highlight", bHighlight);
}

void OdTrXmlRendition::onGroupMetafilesFadingChanged(OdTrVisGroupId groupId, bool bFade)
{
  XML_REGISTERED(kGroupData, groupId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("GroupMetafilesFadingChanged");
    XML_DROP_REG_ID("GroupID");
    xml()->DropBoolean("Fade", bFade);
}

void OdTrXmlRendition::onMetafilePropertyAttached(OdTrVisMetafileId metafileId, const OdTrVisPropertyDef &propDef)
{
  XML_REGISTERED(kMetafileData, metafileId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("MetafilePropertyAttached");
    XML_DROP_REG_ID("MetafileID");
    xml()->DropInt32("PropType", (OdInt32)propDef.propertyType());
    DropPropertyData(propDef);
}

void OdTrXmlRendition::onMetafilePropertyDetached(OdTrVisMetafileId metafileId, OdTrVisPropertyDef::PropertyType propType)
{
  XML_REGISTERED(kMetafileData, metafileId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("MetafilePropertyDetached");
    XML_DROP_REG_ID("MetafileID");
    xml()->DropInt32("PropType", (OdInt32)propType);
}

void OdTrXmlRendition::onGroupPropertyAttached(OdTrVisGroupId groupId, const OdTrVisPropertyDef &propDef)
{
  XML_REGISTERED(kGroupData, groupId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("GroupPropertyAttached");
    XML_DROP_REG_ID("GroupID");
    xml()->DropInt32("PropType", (OdInt32)propDef.propertyType());
    DropPropertyData(propDef);
}

void OdTrXmlRendition::onGroupPropertyDetached(OdTrVisGroupId groupId, OdTrVisPropertyDef::PropertyType propType)
{
  XML_REGISTERED(kGroupData, groupId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("GroupPropertyDetached");
    XML_DROP_REG_ID("GroupID");
    xml()->DropInt32("PropType", (OdInt32)propType);
}

void OdTrXmlRendition::onLayerPropertyAttached(OdTrVisLayerId layerId, const OdTrVisPropertyDef &propDef)
{
  XML_REGISTERED(kLayerData, layerId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("LayerPropertyAttached");
    XML_DROP_REG_ID("LayerID");
    xml()->DropInt32("PropType", (OdInt32)propDef.propertyType());
    DropPropertyData(propDef);
}

void OdTrXmlRendition::onLayerPropertyDetached(OdTrVisLayerId layerId, OdTrVisPropertyDef::PropertyType propType)
{
  XML_REGISTERED(kLayerData, layerId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("LayerPropertyDetached");
    XML_DROP_REG_ID("LayerID");
    xml()->DropInt32("PropType", (OdInt32)propType);
}

void OdTrXmlRendition::onVisualStylePropertyAttached(OdTrVisVisualStyleId visualStyleId, const OdTrVisPropertyDef &propDef)
{
  XML_REGISTERED(kVisualStyleData, visualStyleId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("VisualStylePropertyAttached");
    XML_DROP_REG_ID("VisualStyleID");
    xml()->DropInt32("PropType", (OdInt32)propDef.propertyType());
    DropPropertyData(propDef);
}

void OdTrXmlRendition::onVisualStylePropertyDetached(OdTrVisVisualStyleId visualStyleId, OdTrVisPropertyDef::PropertyType propType)
{
  XML_REGISTERED(kVisualStyleData, visualStyleId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("VisualStylePropertyDetached");
    XML_DROP_REG_ID("VisualStyleID");
    xml()->DropInt32("PropType", (OdInt32)propType);
}

void OdTrXmlRendition::onMaterialPropertyAttached(OdTrVisMaterialId materialId, const OdTrVisPropertyDef &propDef)
{
  XML_REGISTERED(kMaterialData, materialId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("MaterialPropertyAttached");
    XML_DROP_REG_ID("MaterialID");
    xml()->DropInt32("PropType", (OdInt32)propDef.propertyType());
    DropPropertyData(propDef);
}

void OdTrXmlRendition::onMaterialPropertyDetached(OdTrVisMaterialId materialId, OdTrVisPropertyDef::PropertyType propType)
{
  XML_REGISTERED(kMaterialData, materialId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("MaterialPropertyDetached");
    XML_DROP_REG_ID("MaterialID");
    xml()->DropInt32("PropType", (OdInt32)propType);
}

void OdTrXmlRendition::onTexturePropertyAttached(OdTrVisTextureId textureId, const OdTrVisPropertyDef &propDef)
{
  XML_REGISTERED(kTextureData, textureId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("TexturePropertyAttached");
    XML_DROP_REG_ID("TextureID");
    xml()->DropInt32("PropType", (OdInt32)propDef.propertyType());
    DropPropertyData(propDef);
}

void OdTrXmlRendition::onTexturePropertyDetached(OdTrVisTextureId textureId, OdTrVisPropertyDef::PropertyType propType)
{
  XML_REGISTERED(kTextureData, textureId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("TexturePropertyDetached");
    XML_DROP_REG_ID("TextureID");
    xml()->DropInt32("PropType", (OdInt32)propType);
}

void OdTrXmlRendition::onLightPropertyAttached(OdTrVisLightId lightId, const OdTrVisPropertyDef &propDef)
{
  XML_REGISTERED(kLightData, lightId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("LightPropertyAttached");
    XML_DROP_REG_ID("LightID");
    xml()->DropInt32("PropType", (OdInt32)propDef.propertyType());
    DropPropertyData(propDef);
}

void OdTrXmlRendition::onLightPropertyDetached(OdTrVisLightId lightId, OdTrVisPropertyDef::PropertyType propType)
{
  XML_REGISTERED(kLightData, lightId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("LightPropertyDetached");
    XML_DROP_REG_ID("LightID");
    xml()->DropInt32("PropType", (OdInt32)propType);
}

void OdTrXmlRendition::onStateBranchPropertyAttached(OdTrVisStateBranchId stateBranchId, const OdTrVisPropertyDef &propDef)
{
  XML_REGISTERED(kStateBranchData, stateBranchId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("StateBranchPropertyAttached");
    XML_DROP_REG_ID("StateBranchID");
    xml()->DropInt32("PropType", (OdInt32)propDef.propertyType());
    DropPropertyData(propDef);
}

void OdTrXmlRendition::onStateBranchPropertyDetached(OdTrVisStateBranchId stateBranchId, OdTrVisPropertyDef::PropertyType propType)
{
  XML_REGISTERED(kStateBranchData, stateBranchId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("StateBranchPropertyDetached");
    XML_DROP_REG_ID("StateBranchID");
    xml()->DropInt32("PropType", (OdInt32)propType);
}

void OdTrXmlRendition::onViewportPropertyAttached(OdTrVisViewportId viewportId, const OdTrVisPropertyDef &propDef)
{
  XML_REGISTERED(kViewportData, viewportId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("ViewportPropertyAttached");
    XML_DROP_REG_ID("ViewportID");
    xml()->DropInt32("PropType", (OdInt32)propDef.propertyType());
    DropPropertyData(propDef);
}

void OdTrXmlRendition::onViewportPropertyDetached(OdTrVisViewportId viewportId, OdTrVisPropertyDef::PropertyType propType)
{
  XML_REGISTERED(kViewportData, viewportId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("ViewportPropertyDetached");
    XML_DROP_REG_ID("ViewportID");
    xml()->DropInt32("PropType", (OdInt32)propType);
}

void OdTrXmlRendition::onOverlayPropertyAttached(OdTrVisOverlayId overlayId, const OdTrVisPropertyDef &propDef)
{
  XML_REGISTERED(kOverlayData, overlayId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("OverlayPropertyAttached");
    XML_DROP_REG_ID("OverlayID");
    xml()->DropInt32("PropType", (OdInt32)propDef.propertyType());
    DropPropertyData(propDef);
}

void OdTrXmlRendition::onOverlayPropertyDetached(OdTrVisOverlayId overlayId, OdTrVisPropertyDef::PropertyType propType)
{
  XML_REGISTERED(kOverlayData, overlayId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("OverlayPropertyDetached");
    XML_DROP_REG_ID("OverlayID");
    xml()->DropInt32("PropType", (OdInt32)propType);
}

void OdTrXmlRendition::onExtensionObjectPropertyAttached(OdTrVisExtensionId extId, const OdTrVisPropertyDef &propDef)
{
  XML_REGISTERED(kExtensionData, extId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("ExtensionObjectPropertyAttached");
    XML_DROP_REG_ID("ExtensionID");
    xml()->DropInt32("PropType", (OdInt32)propDef.propertyType());
    DropPropertyData(propDef);
}

void OdTrXmlRendition::onExtensionObjectPropertyDetached(OdTrVisExtensionId extId, OdTrVisPropertyDef::PropertyType propType)
{
  XML_REGISTERED(kExtensionData, extId);
  OutputState((_xmlReg.m_pRegId) ? OdTrXmlServer::kShareableState : OdTrXmlServer::kClientOnlyState);
  XML_LEVEL0("ExtensionObjectPropertyDetached");
    XML_DROP_REG_ID("ExtensionID");
    xml()->DropInt32("PropType", (OdInt32)propType);
}

void OdTrXmlRendition::DropPropertyData(const OdTrVisPropertyDef &propDef)
{
  XML_LEVEL(1, "PropVal");
  switch (propDef.propertyType())
  {
    case OdTrVisPropertyDef::kOwningProperty:
      xml()->DropDbOwning("OwnerID", static_cast<const OdTrVisOwningPropertyDef&>(propDef).propertyValue());
    break;
    case OdTrVisPropertyDef::kXformProperty:
      xml()->DropMatrix("Matrix", static_cast<const OdTrVisMatrixPropertyDef&>(propDef).propertyValue());
    break;
    case OdTrVisPropertyDef::kLayerProperty:
      xml()->DropId("LayerID", static_cast<const OdTrVisLayerPropertyDef&>(propDef).propertyValue(),
                    m_idReg.get(kLayerData, static_cast<const OdTrVisLayerPropertyDef&>(propDef).propertyValue()), OdTrXmlServer::kLayerId);
    break;
    case OdTrVisPropertyDef::kHlBranchProperty:
    case OdTrVisPropertyDef::kVsBranchProperty:
      xml()->DropId("StateBranchID", static_cast<const OdTrVisIdPropertyDef&>(propDef).propertyValue(),
                    m_idReg.get(kStateBranchData, static_cast<const OdTrVisIdPropertyDef&>(propDef).propertyValue()), OdTrXmlServer::kStateBranchId);
    break;
    case OdTrVisPropertyDef::kVisualStyleProperty:
      xml()->DropId("VisualStyleID", static_cast<const OdTrVisVisualStylePropertyDef&>(propDef).propertyValue(),
                    m_idReg.get(kVisualStyleData, static_cast<const OdTrVisVisualStylePropertyDef&>(propDef).propertyValue()), OdTrXmlServer::kVisualStyleId);
    break;
    case OdTrVisPropertyDef::kSelStyleProperty:
      xml()->DropUInt32("StyleIndex", static_cast<const OdTrVisSelStylePropertyDef&>(propDef).propertyValue());
    break;
    case OdTrVisPropertyDef::kNameProperty:
      xml()->DropString("Text", static_cast<const OdTrVisStringPropertyDef&>(propDef).propertyValue());
    break;
    case OdTrVisPropertyDef::kSysDefaultProperty:
      xml()->DropBoolean("Boolean", static_cast<const OdTrVisBooleanPropertyDef&>(propDef).propertyValue());
    break;
    default:
      ODA_FAIL();
  }
}

void OdTrXmlRendition::DropMaterialChannel(const OdTrVisMaterialChannelDef &pChannel)
{
  { XML_LEVEL(2, "RealisticComponent");
    DropMaterialChannelComponent(pChannel.m_realisticComponent);
  }
  if (pChannel.hasShadingComponent())
  { XML_LEVEL(2, "ShadingComponent");
    DropMaterialChannelComponent(pChannel.m_shadingComponent);
  }
}

void OdTrXmlRendition::DropMaterialChannelComponent(const OdTrVisMaterialChannelComponent &pComponent)
{
  xml()->DropFloat3("ChannelColor", pComponent.m_channelColor[0], pComponent.m_channelColor[1], pComponent.m_channelColor[2]);
  xml()->DropFloat("BlendFactor", pComponent.m_blendFactor);
  xml()->DropFloat("OpacityLevel", pComponent.m_opacityLevel);
  xml()->DropFloat("ChannelValue", pComponent.m_channelValue);
  xml()->DropFloat("TextureBlend", pComponent.m_textureBlend);
  xml()->DropId("TextureID", pComponent.m_textureId, m_idReg.get(kTextureData, pComponent.m_textureId), OdTrXmlServer::kTextureId);
  xml()->DropUInt32("UWrapType", (OdUInt32)pComponent.m_uWrap);
  xml()->DropUInt32("VWrapType", (OdUInt32)pComponent.m_vWrap);
}

void OdTrXmlRendition::DropLightData(const OdTrVisLightDef &pLight, OdUInt32 nKindOf)
{
  if (nKindOf == 0xFFFFFFFF)
    xml()->DropInt32("Type", (OdInt32)pLight.m_type);
  if (GETBIT(nKindOf, OdTrVisLightDef::kOverrideVisibility))
    xml()->DropBooleanOpt("On", pLight.isLightOn());
  if (GETBIT(nKindOf, OdTrVisLightDef::kOverrideOrientation))
    xml()->DropDouble3("Position", pLight.m_position.x, pLight.m_position.y, pLight.m_position.z),
    xml()->DropDouble3("Direction", pLight.m_direction.x, pLight.m_direction.y, pLight.m_direction.z);
  if (GETBIT(nKindOf, OdTrVisLightDef::kOverrideColor))
    xml()->DropFloat4("Color", pLight.m_color.red(), pLight.m_color.green(), pLight.m_color.blue(), pLight.m_color.alpha());
  if (GETBIT(nKindOf, OdTrVisLightDef::kOverrideAttenuation))
  {
    xml()->DropFloat("ConstantAttenuation", pLight.m_constantAttenuation);
    xml()->DropFloat("LinearAttenuation", pLight.m_linearAttenuation);
    xml()->DropFloat("QuadraticAttenuation", pLight.m_quadraticAttenuation);
    if (pLight.attenuationLimitsEnabled())
      xml()->DropBoolean("UseLimits", true),
      xml()->DropFloat("StartAttenuationLimit", pLight.m_startAttenuationLimit),
      xml()->DropFloat("EndAttenuationLimit", pLight.m_endAttenuationLimit);
  }
  if (GETBIT(nKindOf, OdTrVisLightDef::kOverrideAngles))
    xml()->DropFloat("SpotCutoff", pLight.m_spotCutoff),
    xml()->DropFloat("SpotExponent", pLight.m_spotExponent);
  if (GETBIT(nKindOf, OdTrVisLightDef::kOverrideSpecualFactor))
    xml()->DropFloat("SpecularFactor", pLight.m_specularFactor);
  if (GETBIT(nKindOf, OdTrVisLightDef::kOverrideShadow))
  {
    if (pLight.shadowsEnabled())
      xml()->DropBoolean("ShadowsEnabled", true),
      xml()->DropUInt16("ShadowMapSize", pLight.m_shadowMapSize),
      xml()->DropUInt16("ShadowSoftness", pLight.m_shadowSoftness),
      xml()->DropUInt16("ShadowSamples", pLight.m_shadowSamples);
  }
}

void OdTrXmlRendition::DropStateBranch(const OdTrVisStateBranchDef &pStateBranch, bool bDropChilds, bool bDropMarkers)
{
  if (bDropChilds)
  {
    OdUInt32 nChilds = pStateBranch.m_childs.size();
    const OdTrVisStateBranchId *pChilds = pStateBranch.m_childs.getPtr();
    xml()->DropUInt32("NChilds", nChilds);
    if (bDropMarkers)
      xml()->DropUInt32("NMarkers", pStateBranch.m_markers.size());
    if (nChilds)
    { XML_LEVEL(1, "Childs");
      for (OdUInt32 n = 0; n < nChilds; n++)
      {
        OdTrVisStateBranchId stateBranchId = pChilds[n];
        xml()->DropId("StateBranchID", stateBranchId, m_idReg.get(kStateBranchData, stateBranchId), OdTrXmlServer::kStateBranchId);
      }
    }
  }
  if (bDropMarkers)
  {
    OdUInt32 nMarkers = pStateBranch.m_markers.size();
    const OdTrVisGsMarker *pMarkers = pStateBranch.m_markers.getPtr();
    if (!bDropChilds)
      xml()->DropUInt32("NMarkers", nMarkers);
    if (nMarkers)
    {
      XML_LEVEL(1, "Markers");
      for (OdUInt32 n = 0; n < nMarkers; n++)
        xml()->DropUInt64("Marker", (OdUInt64)pMarkers[n]);
    }
  }
}

void OdTrXmlRendition::DropVisualStyleData(const OdTrVisVisualStyle &pVStyle, bool bDropNonModified)
{
  const OdUInt32 nProps = pVStyle.numProperties();
  xml()->DropUInt32("NProps", nProps);
  if (bDropNonModified) xml()->DropUInt32("NPropsModified", nProps);
  else xml()->DropUInt32("NPropsModified", pVStyle.countModified());
  for (OdUInt32 nProp = 0; nProp < nProps; nProp++)
  {
    const OdTrVisVisualStyleProperty &prop = pVStyle.property((OdGiVisualStyleProperties::Property)nProp);
    if (bDropNonModified || prop.isModified())
    {
      XML_LEVEL(1, "Prop");
        xml()->DropUInt32("NProp", prop.propertyNumber());
        xml()->DropUInt32("PropType", (OdUInt32)prop.type());
        xml()->DropBoolean("Set", prop.operation());
        switch (prop.type())
        {
          case OdGiVariant::kBoolean: xml()->DropBoolean("BVal", prop.getBool()); break;
          case OdGiVariant::kInt: xml()->DropInt32("IVal", prop.getInt()); break;
          case OdGiVariant::kDouble: xml()->DropDouble("DVal", prop.getDouble()); break;
          case OdGiVariant::kColor: xml()->DropUInt32("CVal", prop.getUInt()); break;
          case OdGiVariant::kString: xml()->DropString("SVal", OdString(prop.getString())); break;
          default: ODA_FAIL();
        }
    }
  }
}

void OdTrXmlRendition::DropLayerProps(const OdTrVisLayerProps &pLayerProps)
{
  xml()->DropBooleanOpt("Visibility", pLayerProps.isLayerVisible());
  xml()->DropBooleanOpt("Faded", pLayerProps.isLayerFaded());
}

void OdTrXmlRendition::DropMetadata(const OdTrVisFlatMetafileContainer *pMetafile)
{
  XML_LEVEL(1, "MetafileData");
    xml()->DropUInt32("NArrays", pMetafile->m_ArrayElements.size());
    for (OdUInt32 n = 0; n < pMetafile->m_ArrayElements.size(); n++)
    {
      const OdTrVisArrayWrapper &array = pMetafile->m_ArrayElements.getPtr()[n];
      XML_LEVEL(2, "Array");
        xml()->DropUInt32("Type", (OdUInt32)array.m_type);
        switch (array.m_type)
        {
          case OdTrVisArrayWrapper::Type_Vertex:
          case OdTrVisArrayWrapper::Type_Normal:
            {
              const float *pData = (const float*)array.m_pData;
              OdUInt32 nData = array.m_uSize / sizeof(float);
              xml()->DropUInt32("NData", nData / 3);
              xml()->DropFloats("ArrayData", nData, pData);
            }
          break;
          case OdTrVisArrayWrapper::Type_Color:
            {
              const float *pData = (const float*)array.m_pData;
              OdUInt32 nData = array.m_uSize / sizeof(float);
              xml()->DropUInt32("NData", nData / 4);
              xml()->DropFloats("ArrayData", nData, pData);
            }
          break;
          case OdTrVisArrayWrapper::Type_TexCoord:
            {
              const float *pData = (const float*)array.m_pData;
              OdUInt32 nData = array.m_uSize / sizeof(float);
              xml()->DropUInt32("NData", nData / 2);
              xml()->DropFloats("ArrayData", nData, pData);
            }
          break;
          case OdTrVisArrayWrapper::Type_Depth:
            {
              const float *pData = (const float*)array.m_pData;
              OdUInt32 nData = array.m_uSize / sizeof(float);
              xml()->DropUInt32("NData", nData);
              xml()->DropFloats("ArrayData", nData, pData);
            }
          break;
          case OdTrVisArrayWrapper::Type_Index:
            {
              const OdUInt16 *pData = (const OdUInt16*)array.m_pData;
              OdUInt32 nData = array.m_uSize / sizeof(OdUInt16);
              xml()->DropUInt32("NData", nData);
              xml()->DropInts("ArrayData", nData, pData);
            }
          break;
          case OdTrVisArrayWrapper::Type_Marker:
            {
              const OdTrVisDefProcMark *pData = (const OdTrVisDefProcMark*)array.m_pData;
              OdUInt32 nData = array.m_uSize / sizeof(OdTrVisDefProcMark);
              xml()->DropUInt32("NData", nData);
              { XML_LEVEL(3, "Data");
                for (OdUInt32 n = 0; n < nData; n++)
                {
                  XML_LEVEL(4, "Entry");
                    xml()->DropUInt64("Marker", pData[n].m_nMark);
                    xml()->DropUInt32("From",   pData[n].m_uFrom);
                }
              }
            }
          break;
          default: ODA_FAIL();
        }
    }
    ParseMetafileStream(pMetafile);
}

void OdTrXmlRendition::ParseMetafileStream(const OdTrVisFlatMetafileContainer *pMetafile)
{
  XML_LEVEL(2, "MetafileStream");
  OdUInt32 uSize = pMetafile->size();
  if (uSize == 0) return;

  OdUInt32 currArrayNum = 0;
  OdUInt8 currArrayType = 0;

  const OdUInt8 *pMemPtr = pMetafile->memoryPtr();
  const OdUInt8 *pMemPtrReadFor = pMemPtr + uSize;
  while (pMemPtr < pMemPtrReadFor)
  {
    OdTrVisMetaRecType recType = OD_OGL_RDR_READVALRETYPE(OdTrVisMetaRecType, OdUInt8, pMemPtr);
    OD_OGL_RDR_SEEKINC(pMemPtr);
    switch (recType)
    {
      case OdTrVisMetaRecType_EnableOpt:
        {
          XML_LEVEL(3, "EnableOpt");
            xml()->DropUInt8("Mode", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_DisableOpt:
        {
          XML_LEVEL(3, "DisableOpt");
            xml()->DropUInt8("Mode", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_Color:
        {
          XML_LEVEL(3, "Color");
            xml()->DropUInt8RGB("Color", pMemPtr[0], pMemPtr[1], pMemPtr[2], pMemPtr[3]);
            OD_OGL_RDR_SEEKFWD(pMemPtr, 4);
        }
      break;
      case OdTrVisMetaRecType_EnableArray:
        {
          XML_LEVEL(3, "EnableArray");
            xml()->DropUInt8("Type", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
            xml()->DropUInt32("NArray", OD_OGL_RDR_READVAL(OdUInt32, pMemPtr));
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32));
        }
      break;
      case OdTrVisMetaRecType_DisableArray:
        {
          XML_LEVEL(3, "DisableArray");
            xml()->DropUInt8("Type", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_DrawArrays:
        {
          XML_LEVEL(3, "DrawArrays");
            xml()->DropUInt8("Mode", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            xml()->DropInt32("First", OD_OGL_RDR_READVALOFFSET(OdInt32, pMemPtr, 1));
            xml()->DropInt32("Count", OD_OGL_RDR_READVALOFFSET(OdInt32, pMemPtr, 1 + sizeof(OdInt32)));
            OD_OGL_RDR_SEEKFWD(pMemPtr, (sizeof(OdInt32) << 1) + 1);
        }
      break;
      case OdTrVisMetaRecType_DrawElements:
        {
          XML_LEVEL(3, "DrawElements");
            xml()->DropUInt8("Mode", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            xml()->DropInt32("Count", OD_OGL_RDR_READVALOFFSET(OdInt32, pMemPtr, 1));
            xml()->DropUInt32("NArray", OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, 1 + sizeof(OdInt32)));
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdInt32) + 1 + sizeof(OdUInt32));
        }
      break;
      case OdTrVisMetaRecType_CullFace:
        {
          XML_LEVEL(3, "CullFace");
            xml()->DropUInt8("Mode", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_LStipple:
        {
          XML_LEVEL(3, "LStipple");
            xml()->DropUInt8("Pattern", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_PStipple:
        {
          XML_LEVEL(3, "PStipple");
            xml()->DropUInt8("Pattern", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_VPoint:
        {
          XML_LEVEL(3, "VPoint");
            const float *pFloats = (const float*)pMemPtr;
            xml()->DropFloat3("Point", pFloats[0], pFloats[1], pFloats[2]);
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, (sizeof(float) << 1) + sizeof(float));
      break;
      case OdTrVisMetaRecType_VLine:
        {
          XML_LEVEL(3, "VLine");
            const float *pFloats = (const float*)pMemPtr;
            xml()->DropFloat3("Point1", pFloats[0], pFloats[1], pFloats[2]);
            xml()->DropFloat3("Point2", pFloats[3], pFloats[4], pFloats[5]);
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, (sizeof(float) << 2) + (sizeof(float) << 1));
      break;
      case OdTrVisMetaRecType_IPoint:
        {
          XML_LEVEL(3, "IPoint");
            xml()->DropInt32("Index", OD_OGL_RDR_READVAL(OdInt32, pMemPtr));
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdInt32));
      break;
      case OdTrVisMetaRecType_ILine:
        {
          XML_LEVEL(3, "ILine");
            xml()->DropInt32("Index1", OD_OGL_RDR_READVAL(OdInt32, pMemPtr));
            xml()->DropInt32("Index2", OD_OGL_RDR_READVALOFFSET(OdInt32, pMemPtr, sizeof(OdInt32)));
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdInt32) * 2);
      break;
      case OdTrVisMetaRecType_HLRStencil:
        {
          XML_LEVEL(3, "HLRStencil");
            xml()->DropUInt8("State", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_EnableShading:
        {
          XML_LEVEL(3, "EnableShading");
            xml()->DropUInt8("Mode", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_DisableShading:
        {
          XML_LEVEL(3, "DisableShading");
            xml()->DropUInt8("Mode", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_Material:
        {
          XML_LEVEL(3, "Material");
            xml()->DropId("MaterialID", OD_OGL_RDR_READVAL(OdUInt64, pMemPtr),
              m_idReg.get(kMaterialData, OD_OGL_RDR_READVALRETYPE(OdTrVisId, OdUInt64, pMemPtr)), OdTrXmlServer::kMaterialId);
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, 8);
      break;
      case OdTrVisMetaRecType_UserEntry:
        {
          XML_LEVEL(3, "UserEntry");
            xml()->DropUInt32("NumData", OD_OGL_RDR_READVAL(OdUInt32, pMemPtr));
            xml()->DropBinaryStream("Data", OD_OGL_RDR_GETPTRNATIVEOFFSET(const OdUInt8, pMemPtr, sizeof(OdUInt32)),
                                            OD_OGL_RDR_READVAL(OdUInt32, pMemPtr));
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32) + OD_OGL_RDR_READVAL(OdUInt32, pMemPtr));
      break;
      case OdTrVisMetaRecType_InitTexture:
        {
          XML_LEVEL(3, "InitTexture");
            xml()->DropId("TextureID", OD_OGL_RDR_READVAL(OdUInt64, pMemPtr),
              m_idReg.get(kTextureData, OD_OGL_RDR_READVALRETYPE(OdTrVisId, OdUInt64, pMemPtr)), OdTrXmlServer::kTextureId);
            xml()->DropUInt8("Repeat", OD_OGL_RDR_READVALOFFSET(OdUInt8, pMemPtr, 8));
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, 8 + 1);
      break;
      case OdTrVisMetaRecType_UninitTexture: // Haven't data
        {
          XML_LEVEL(3, "UninitTexture");
          // No data in this entry
        }
      break;
      case OdTrVisMetaRecType_SelectionMarker:
        {
          XML_LEVEL(3, "SelectionMarker");
            xml()->DropUInt64("Marker", OD_OGL_RDR_READVALRETYPE(OdUInt64, OdTrVisGsMarker, pMemPtr));
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdTrVisGsMarker));
      break;
      case OdTrVisMetaRecType_EnableMarkerArray:
        {
          XML_LEVEL(3, "EnableMarkerArray");
            xml()->DropUInt8("Type", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
            xml()->DropUInt32("NArray", OD_OGL_RDR_READVAL(OdUInt32, pMemPtr));
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32));
        }
      break;
      case OdTrVisMetaRecType_DisableMarkerArray:
        {
          XML_LEVEL(3, "DisableMarkerArray");
            xml()->DropUInt8("Type", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_VisibilityFlags:
        {
          XML_LEVEL(3, "VisibilityFlags");
            xml()->DropUInt8("Flags", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
            OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_Lineweight:
        {
          XML_LEVEL(3, "Lineweight");
            const OdUInt8 lwdType = OD_OGL_RDR_READVAL(OdUInt8, pMemPtr);
            xml()->DropUInt8("Type", lwdType);
            if (OdTrVisLwdSetting::isPs((OdTrVisLwdSetting::LwdType)lwdType))
            {
              xml()->DropDouble("Ps", OD_OGL_RDR_READVALOFFSET(double, pMemPtr, sizeof(OdUInt8)));
              OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) + sizeof(double));
            }
            else
            {
              xml()->DropInt16("Lw", OD_OGL_RDR_READVALOFFSET(OdInt16, pMemPtr, sizeof(OdUInt8)));
              OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) + sizeof(OdInt16));
            }
        }
      break;
      case OdTrVisMetaRecType_Linestyle:
        {
          XML_LEVEL(3, "Linestyle");
            const bool defStyle = (OD_OGL_RDR_READVAL(OdUInt8, pMemPtr) != 0);
            xml()->DropBoolean("Default", defStyle);
            if (!defStyle)
            {
              xml()->DropUInt8("CapsType", OD_OGL_RDR_READVALOFFSET(OdUInt8, pMemPtr, sizeof(OdUInt8)));
              xml()->DropUInt8("JoinType", OD_OGL_RDR_READVALOFFSET(OdUInt8, pMemPtr, sizeof(OdUInt8) * 2));
              OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt8) * 3);
            }
            else
              OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_Program:
        {
          XML_LEVEL(3, "Program");
            xml()->DropId("ProgramID", OD_OGL_RDR_READVAL(OdUInt64, pMemPtr), NULL, OdTrXmlServer::kProgramId);
        }
        OD_OGL_RDR_SEEKFWD(pMemPtr, 8);
      break;
      case OdTrVisMetaRecType_TtfText:
        {
          XML_LEVEL(3, "TtfText");
            const OdTrVisTtfFontsCacheClient *pCache = processingDevice()->ttfFontsCache();
            if (pCache)
            { // In case if real vectorization device is connected
              OdUInt64 fontKeyA = OD_OGL_RDR_READVAL(OdUInt64, pMemPtr);
              OdUInt64 fontKeyB = OD_OGL_RDR_READVALOFFSET(OdUInt64, pMemPtr, sizeof(OdUInt64));
              const OdGeMatrix3d *pMtx = (const OdGeMatrix3d*)(pMemPtr + sizeof(OdUInt64) * 2);
              const OdGeVector3d *pVec = (const OdGeVector3d*)(pMemPtr + sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d));
              OdUInt32 nText = OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d));
              const OdUInt32 *pText = (const OdUInt32*)(pMemPtr + sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d) + sizeof(OdUInt32));
              OdTrVisTtfFontsCacheClient::TtfFont pFont = pCache->getFontCache(fontKeyA, fontKeyB);
              if (pFont && nText)
              {
                xml()->DropMatrix("Transform", *pMtx);
                xml()->DropDouble3("Direction", pVec->x, pVec->y, pVec->z);
                xml()->DropUInt32("NText", nText);
                { XML_LEVEL(4, "Text");
                  for (OdUInt32 n = 0; n < nText; n++)
                  {
                    double sideMult;
                    OdTrVisDisplayId mfId = pCache->getCharMetafile(pFont, pText[n], &sideMult);
                    XML_LEVEL(5, "Char");
                      xml()->DropId("MetafileID", mfId, m_idReg.get(kMetafileData, mfId), OdTrXmlServer::kDisplayId);
                      xml()->DropDouble("Multiplier", sideMult);
                  }
                }
              }
              else
                xml()->DropUInt32("NText", 0);
              OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt64) * 2 + sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d) + sizeof(OdUInt32) * (nText + 1));
            }
            else
            { // In case if there is no connection to real vectorization device
              const OdGeMatrix3d *pMtx = (const OdGeMatrix3d*)pMemPtr;
              const OdGeVector3d *pVec = (const OdGeVector3d*)(pMemPtr + sizeof(OdGeMatrix3d));
              OdUInt32 nText = OD_OGL_RDR_READVALOFFSET(OdUInt32, pMemPtr, sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d));
              OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdGeMatrix3d) + sizeof(OdGeVector3d) + sizeof(OdUInt32));
              if (nText)
              {
                xml()->DropUInt32("NText", nText);
                xml()->DropMatrix("Transform", *pMtx);
                xml()->DropDouble3("Direction", pVec->x, pVec->y, pVec->z);
                OdTrVisMetafileId mfId; double sideMult;
                while (nText)
                {
                  mfId = OD_OGL_RDR_READVAL(OdTrVisMetafileId, pMemPtr);
                  sideMult = OD_OGL_RDR_READVALOFFSET(double, pMemPtr, sizeof(OdTrVisMetafileId));
                  XML_LEVEL(5, "Char");
                    xml()->DropId("MetafileID", mfId, m_idReg.get(kMetafileData, mfId), OdTrXmlServer::kDisplayId);
                    xml()->DropDouble("Multiplier", sideMult);
                  nText--;
                  OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdTrVisMetafileId) + sizeof(double));
                }
              }
              else
                xml()->DropUInt32("NText", 0);
            }
        }
      break;
      case OdTrVisMetaRecType_PushMatrix:
        {
          XML_LEVEL(3, "PushMatrix");
          bool bIdentity = OD_OGL_RDR_READVALRETYPE(OdTrVisPushMatrixType, OdUInt8, pMemPtr) == OdTrVisPushMatrixType_Identity;
          xml()->DropBooleanOpt("Identity", bIdentity);
          OD_OGL_RDR_SEEKINC(pMemPtr);
          if (!bIdentity)
          {
            xml()->DropMatrix("Matrix", *OD_OGL_RDR_GETPTRNATIVE(const OdGeMatrix3d, pMemPtr));
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdGeMatrix3d));
          }
        }
      break;
      case OdTrVisMetaRecType_PopMatrix:
        {
          XML_LEVEL(3, "PopMatrix");
          // No data in this entry
        }
      break;
      case OdTrVisMetaRecType_Metafile:
        {
          XML_LEVEL(3, "Metafile");
            xml()->DropId("MetafileID", OD_OGL_RDR_READVAL(OdUInt64, pMemPtr),
              m_idReg.get(kMetafileData, OD_OGL_RDR_READVALRETYPE(OdTrVisId, OdUInt64, pMemPtr)), OdTrXmlServer::kDisplayId);
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdTrVisMetafileId));
        }
      break;
      case OdTrVisMetaRecType_GeomMarker:
        {
          XML_LEVEL(3, "GeomMarker");
            xml()->DropUInt8("Type", OD_OGL_RDR_READVAL(OdUInt8, pMemPtr));
          OD_OGL_RDR_SEEKINC(pMemPtr);
        }
      break;
      case OdTrVisMetaRecType_VisualStyle:
        {
          XML_LEVEL(3, "VisualStyle");
            xml()->DropId("VisualStyleID", OD_OGL_RDR_READVAL(OdUInt64, pMemPtr),
              m_idReg.get(kVisualStyleData, OD_OGL_RDR_READVALRETYPE(OdTrVisId, OdUInt64, pMemPtr)), OdTrXmlServer::kVisualStyleId);
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdTrVisVisualStyleId));
        }
      break;
      case OdTrVisMetaRecType_MetafileMarker:
        {
          XML_LEVEL(3, "MetafileMarker");
            xml()->DropId("Marker", OD_OGL_RDR_READVAL(OdUInt64, pMemPtr),
              m_idReg.get(kMetafileData, OD_OGL_RDR_READVALRETYPE(OdTrVisId, OdUInt64, pMemPtr)), OdTrXmlServer::kDisplayId);
            OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdTrVisMetafileId));
        }
      break;
      case OdTrVisMetaRecType_ExtensionObject:
        {
          XML_LEVEL(3, "ExtensionObject");
            xml()->DropId("ExtensionID", OD_OGL_RDR_READVAL(OdUInt64, pMemPtr),
              m_idReg.get(kExtensionData, OD_OGL_RDR_READVALRETYPE(OdTrVisId, OdUInt64, pMemPtr)), OdTrXmlServer::kExtensionId);
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdTrVisMetafileId));
        }
      break;
      case OdTrVisMetaRecType_SelectionStyle:
        {
          XML_LEVEL(3, "SelectionStyle");
            xml()->DropUInt32("Index", OD_OGL_RDR_READVAL(OdUInt32, pMemPtr));
          OD_OGL_RDR_SEEKFWD(pMemPtr, sizeof(OdUInt32));
        }
      break;
      case OdTrVisMetaRecType_Empty:
        // Skip
      break;
      default:
        ODA_FAIL();
    }
  }
}

void OdTrXmlRendition::OutputState(OdTrXmlServer::OutputState newState, bool bForce)
{
  if ((newState != m_outputState) || bForce)
  {
    m_outputState = newState;
    xml()->OnStateChanged(newState);
  }
}

//
