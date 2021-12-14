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
// XmlGlesLoader.h

#ifndef OD_XML_GLES_LOADER_H_
#define OD_XML_GLES_LOADER_H_

#include "TD_PackPush.h"

#include "Ge/GeMatrix3d.h"
#include "OdBinaryData.h"
#include "TrVisRendition.h"
#include "TrVisUniqueIdMapping.h"
#include "OdFakeDbStub.h"

/** <group !!RECORDS_tkernel_apiref>
*/
struct OdTrXmlMetafileOrderDef
{ 
  OdTrVisViewportId m_viewportId;
  OdTrVisOverlayId m_overlayId;
  OdUInt32 m_nInvalidFrom;
  OdUInt32 m_nInvalidLen;
  OdUInt64Vector m_metafileOrder;

  void setDefault()
  {
    m_viewportId = OdTrVisViewportId();
    m_overlayId = kTrVisMainOverlayId;
    m_nInvalidFrom = 0;
    m_nInvalidLen = 0;
    m_metafileOrder.clear();
  }
};

/** <group !!RECORDS_tkernel_apiref>
*/
struct OdTrXmlLightsListDef
{ 
  OdTrVisViewportId m_viewportId;
  OdArray<OdTrVisLightDef, OdMemoryAllocator<OdTrVisLightDef> > m_Defs;
  bool m_bDefaultLightsOn;

  void setDefault()
  {
    m_viewportId = kTrVisNullId;
    m_Defs.clear();
    m_bDefaultLightsOn = true;
  }
};

/** <group !!RECORDS_tkernel_apiref>
*/
struct OdTrXmlLightOverridesDef
{ 
  OdTrVisLightId m_lightId;
  OdUInt32 m_nKindOfMod;

  void setDefault()
  {
    m_lightId = kTrVisNullId;
    m_nKindOfMod = 0;
  }
};

/** <group !!RECORDS_tkernel_apiref>
*/
struct OdTrXmlBackgroundDef
{ 
  OdTrVisViewportId m_viewportId;
  OdTrVisFlatMetafileContainerPtr m_pStream;
  OdTrVisProgramId m_baseProgramId;

  void setDefault()
  {
    m_viewportId = OdTrVisViewportId();
    m_baseProgramId = kTrVisNegativeId;
  }
};

/** <group !!RECORDS_tkernel_apiref>
*/
struct OdTrXmlOverlayExtentsOverrideDef : public OdTrVisExtentsDef
{
  OdTrVisViewportId m_viewportId;
  OdTrVisOverlayId  m_overlayId;

  void setDefault()
  {
    m_viewportId = OdTrVisViewportId();
    m_overlayId = kTrVisMainOverlayId;
    OdTrVisExtentsDef::setDefault();
  }
};

/** <group !!RECORDS_tkernel_apiref>
*/
struct OdTrXmlOverlayViewParamsOverrideDef
{
  OdTrVisViewportId m_viewportId;
  OdTrVisOverlayId m_overlayId;
  bool m_bOverride;
  OdTrVisViewParamsDef m_viewParams;

  void setDefault()
  {
    m_viewportId = OdTrVisViewportId();
    m_overlayId = kTrVisMainOverlayId;
    m_bOverride = false;
    m_viewParams.setDefault();
  }
};

/** <group !!RECORDS_tkernel_apiref>
*/
struct OdTrXmlMetafileOrderInheritanceDef
{
  OdTrVisViewportId m_viewportId1;
  OdTrVisOverlayId m_overlayId;
  OdTrVisViewportId m_viewportId2;

  void setDefault()
  {
    m_viewportId1 = OdTrVisViewportId();
    m_viewportId2 = OdTrVisViewportId();
    m_overlayId = kTrVisMainOverlayId;
  }
};

/** <group !!RECORDS_tkernel_apiref>
*/
struct OdTrXmlLayerOverrideDef
{
  OdTrVisViewportId m_overrideViewportId;
  bool              m_bOverrideLayer;
  OdTrVisLayerProps m_overrideLayerProps;

  void setDefault()
  {
    m_overrideViewportId = 0;
    m_bOverrideLayer = false;
    m_overrideLayerProps.resetLayerFlags();
  }
};

/** <group !!RECORDS_tkernel_apiref>
*/
struct OdTrXmlGroupMetafilesDef
{ 
  OdUInt64Vector m_metafiles;

  void setDefault()
  {
    m_metafiles.clear();
  }
};

/** <group !!RECORDS_tkernel_apiref>
*/
#define PATH_ENTRY(path, enKey, func) pk_##enKey,
typedef enum {
  pk_None = 0,
# include "PathEntryDefs.h"
  pk_UpperBound,
  pk_Count = pk_UpperBound - 1
} OdEnPathKey;
/** \details
  This interface should provide Id's map sharing between different OdXmlGlesLoader instances
  <group ExRender_Classes>
*/
class OdXmlGlesLoaderIdMapper
{
public:
  /** \details
  Obtain exist or generate new ID
  */
  virtual OdTrVisId getId( const OdAnsiString&, OdTrVisRendition::DataType type ) const = 0;
  /** \details
  Check is ID for specified key exist
  */
  virtual bool hasId( const OdAnsiString&, OdTrVisRendition::DataType type ) const = 0;
  /** \details
  Set special ID value by specified key
  */
  virtual void setSpecialId( const OdAnsiString&, OdTrVisId, OdTrVisRendition::DataType type ) = 0;
  /** \details
  Clear all amps
  */
  virtual void reset() = 0;
  /** \details
  Returns pointer to map <ID>-<OdFakeDbStub>
  */
  virtual OdSharedPtr<std::map<OdAnsiString, OdSharedPtr<class OdFakeDbStub> > > getMapFakeDbStub() = 0;
};
/** \details
  Simple implementation of OdXmlGlesLoaderIdMapper interface using OdTrVisUniqueIDMappingImpl
  <group ExRender_Classes>
*/
class OdXmlGlesIdMapper : public OdXmlGlesLoaderIdMapper
{
protected:
  OdTrVisUniqueIDMappingImpl< OdAnsiString > m_mappingImpl;
  OdSharedPtr<std::map<OdAnsiString, OdSharedPtr<class OdFakeDbStub> > > m_pMapFakeDbStub;
public:
  OdXmlGlesIdMapper()
  {
    m_pMapFakeDbStub = new std::map<OdAnsiString, OdSharedPtr<OdFakeDbStub> >();
  }

  ~OdXmlGlesIdMapper(){};

  virtual OdTrVisId getId( const OdAnsiString& key, OdTrVisRendition::DataType type ) const
  {
    return m_mappingImpl.getTrVisId( key, type );
  }
  virtual bool hasId( const OdAnsiString& key, OdTrVisRendition::DataType type ) const
  {
    return m_mappingImpl.hasUID( key, type );
  }
  virtual void reset()
  {
    m_mappingImpl.clean();
    m_pMapFakeDbStub->clear();
  }
  virtual void setSpecialId( const OdAnsiString& key, OdTrVisId id, OdTrVisRendition::DataType type )
  {
    m_mappingImpl.setSpecialId( key, id, type );
  }
  virtual OdSharedPtr<std::map<OdAnsiString, OdSharedPtr<class OdFakeDbStub> > > getMapFakeDbStub()
  {
    return m_pMapFakeDbStub;
  }
};


/** <group ExRender_Classes>
*/
class OdXmlGlesLoader
{
protected:
  OdTrVisRendition* m_pRendition;

  OdTrVisId m_id; // current Internal ID
  OdXmlGlesLoaderIdMapper* m_pIdMapper;

  //OdSharedPtr<OdGLES2ProgramDef> m_pProgramDef;
  OdSharedPtr<OdTrVisMaterialDef> m_pMaterialDef;
  struct TextureDescription
  {
    OdTrVisTexture::Format fmt;
    OdUInt32 nWidth, nHeight, nAlign, nScanLineLength;
    bool isValid;
    OdTrVisTexture::Format pal_fmt;
    OdUInt32 pal_nWidth, pal_nAlign, pal_nScanLineLength;
  };
  TextureDescription m_curTexture;
  OdSharedPtr<OdTrVisTextureDef> m_pTextureDef;
  OdSharedPtr<OdTrVisViewportDef> m_pViewportDef;
  OdInt32 m_nViewportInsertedIndex;
  OdSharedPtr<OdTrVisOverlayDef> m_pOverlayDef;
  OdSharedPtr<OdTrXmlOverlayViewParamsOverrideDef> m_pOverlayViewParamsOverrideDef;
  OdSharedPtr<OdTrVisViewParamsDef> m_pViewParamsDef;
  OdSharedPtr<OdTrVisMetafileDef> m_pMetafileDef;
  OdArray<float, OdMemoryAllocator<float> > m_ArrayOffset;
  OdSharedPtr<OdTrVisFlatMetafileContainerWriter> m_pWriter;
  OdBaseMetafileOutputStream m_streamWriter;
  OdSharedPtr<OdTrVisSurfaceDef> m_pSurfaceDef;
  OdSharedPtr<OdTrXmlOverlayExtentsOverrideDef> m_pExtentsDef;
  OdSharedPtr<OdTrXmlMetafileOrderDef> m_pMetafileOrderDef;
  OdSharedPtr<OdTrVisViewportLineStyleDef> m_pViewportLinestyleDef;
  OdSharedPtr<OdTrVisViewportFadingDef> m_pViewportFadingDef;
  OdSharedPtr<OdTrVisViewportLineweightsDef> m_pViewportLwdDef;
  OdSharedPtr<OdTrVisViewportShadingDef> m_pViewportShadingDef;
  OdSharedPtr<OdTrVisViewportClipDef> m_pViewportClipDef;
  OdSharedPtr<OdTrXmlBackgroundDef> m_pBackgroundDef;
  OdSharedPtr<OdTrVisFogDef> m_pFogDef;
  OdSharedPtr<OdTrVisSectionDef> m_pSectionDef;
  OdSharedPtr<OdTrXmlLightOverridesDef> m_pLightOverDef;
  OdSharedPtr<OdTrXmlLightsListDef> m_pLightsListDef;
  OdSharedPtr<OdTrVisLightDef> m_pLightDef;
  OdSharedPtr<OdTrVisVisualStyle> m_pVisualStyleDef;
  OdSharedPtr<OdTrXmlMetafileOrderInheritanceDef> m_pMetafileOrderInheritance;
  OdTrVisVisualStyleProperty* m_pVisualStyleProperty;
  OdGiVariant::VariantType m_typeVisualStyle;
  OdSharedPtr<OdTrVisLayerDef> m_pLayerDef;
  OdSharedPtr<OdTrXmlLayerOverrideDef> m_pLayerOverrideDef;
  OdSharedPtr<OdTrVisStateBranchDef> m_pStateBranchDef;
  OdSharedPtr<OdTrXmlGroupMetafilesDef> m_pGroupMetafilesDef;
  OdSmartPtr<OdTrVisExtensionObjectProxy> m_pExtensionObject;
  OdSharedPtr<OdTrVisPropertyDef> m_pPropertyDef;
  OdTrVisPropertyDef::PropertyType m_propertyType;
  OdUInt8 m_objectChangedFlag;
  OdUInt32 m_mfUserEntryNumData;

  // cache of fake DbStub
  OdSharedPtr<std::map<OdAnsiString, OdSharedPtr<class OdFakeDbStub> > > m_pMapFakeDbStub;

  OdTrVisMetafileContainer* metafile();
  enum OdTrVisArrayWrapper::Type m_typeElement;
  OdUInt32 m_indexElement,
           m_numDataElement;
  OdArray<OdTrVisDefProcMark, OdMemoryAllocator<OdTrVisDefProcMark> > m_aMarkers;
  OdUInt32 m_uaMarkerIndex;

  bool m_bDestroyIdMapped;
public:

  OdXmlGlesLoader( OdTrVisRendition* pRendition, OdXmlGlesLoaderIdMapper* pIdMapper = NULL);
  virtual ~OdXmlGlesLoader();

  bool DropBegin(OdEnPathKey pk);
  virtual bool DropEnd(OdEnPathKey pk);

  bool DropBoolean(OdEnPathKey pk, bool data);

  bool DropUInt8RGB(OdEnPathKey pk, const OdBinaryData& data);
  bool DropUInt8(OdEnPathKey pk, OdUInt8 data);
  bool DropUInt16(OdEnPathKey pk, OdUInt16 data);
  bool DropUInts16(OdEnPathKey pk, const OdArray<OdUInt16, OdMemoryAllocator<OdUInt16> >& data);
  bool DropUInt32(OdEnPathKey pk, OdUInt32 data);
  bool DropUInt64(OdEnPathKey pk, OdUInt64 data);
  bool DropBinaryStream(OdEnPathKey pk, const OdBinaryData& data);

  bool DropInt16(OdEnPathKey pk, OdInt16 data);
  bool DropInt32(OdEnPathKey pk, OdInt32 data);
  bool DropInts(OdEnPathKey pk, const OdIntArray& data);

  bool DropFloat(OdEnPathKey pk, float data);
  bool DropFloats(OdEnPathKey pk, const OdArray<float, OdMemoryAllocator<float> >& data);
  bool DropDouble(OdEnPathKey pk, double data);
  bool DropDoubles(OdEnPathKey pk, const OdArray<double, OdMemoryAllocator<double> >& data);
  bool DropMatrix(OdEnPathKey pk, const OdGeMatrix3d& data,
                  const OdArray<float, OdMemoryAllocator<float> >* pDataFloats = NULL);

//  bool DropStaticId(OdEnPathKey pk, OdTrVisId idValue); // prog
  virtual bool DropId(OdEnPathKey pk, const OdAnsiString& asValue);
  bool DropDbOwning(OdEnPathKey pk, const OdAnsiString& asValue);

  bool DropChars(OdEnPathKey pk, const OdAnsiString& asValue);
//  bool DropShareableClientData(OdEnPathKey pk, const OdAnsiString& asValue);
};

#include "TD_PackPop.h"

#endif // OD_XML_GLES_LOADER_H_
