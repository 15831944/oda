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
#ifndef __DG_GIAUXDATAFORDGDATABASE_H__
#define __DG_GIAUXDATAFORDGDATABASE_H__

#include "GiDefaultContext.h"
#include "DgDatabase.h"
#include "DgView.h"
#include "DgReferenceAttach.h"

#include "TD_PackPush.h"

//============================================================================================
//                      Class OdGiLayerOverridesForDgDatabase
//============================================================================================
/** \details
  <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiLayerOverridesForDgDatabase : public OdGiLayerOverrides
{
public:
  ODRX_DECLARE_MEMBERS(OdGiLayerOverridesForDgDatabase);
  OdGiLayerOverridesForDgDatabase();
  virtual ~OdGiLayerOverridesForDgDatabase();

  virtual OdDbStub* getLayerOverride(OdDbStub* idNativeLayer) const;
  virtual void      setOverride(const OdDgElementId& idNativeLevel, const OdDgElementId& idOverridedLevel);
  virtual void      removeOverride(const OdDgElementId& idNativeLayer);

private:
  std::map<OdDgElementId, OdDgElementId> m_mapLevelOverrides;
};

typedef OdSmartPtr<OdGiLayerOverridesForDgDatabase> OdGiLayerOverridesForDgDatabasePtr;

//============================================================================================
//                      Struct OdGiAuxDataXRefInfoItem
//============================================================================================
/** \details
  <group OdGi_Classes>
*/
struct TG_EXPORT OdGiAuxDataXRefInfoItem
{
  OdDgElementId m_idXRef;
  OdUInt16      m_uNestedDepth;
  OdUInt8       m_uFlags;

  bool getSelfReferenceFlag() const;
  void setSelfReferenceFlag( bool bSet );
};

//============================================================================================
//                      Struct OdGiAuxDataXRefPaletteCorrection
//============================================================================================
/** \details
  <group OdGi_Classes>
*/
struct TG_EXPORT OdGiAuxDataXRefPaletteCorrection
{
  double m_dValue;
  double m_dSaturation;
  double m_dHue;
};

//============================================================================================
//                      Class OdGiAuxDataForDgDatabase
//============================================================================================
/** \details
  <group OdGi_Classes>
*/
class OdGiAuxDataForDgDatabase;

/** \details
This template class is a specialization of the OdSmartPtr class for OdGiAuxDataForDgDatabase object pointers.
*/
typedef OdSmartPtr<OdGiAuxDataForDgDatabase> OdGiAuxDataForDgDatabasePtr;

/** \details
    This class defines the operations and properties that are used in the
    ODA Drawings API for DGN vectorization of an OdDgDatabase.

    <group OdGi_Classes> 
*/
class TG_EXPORT OdGiAuxDataForDgDatabase : public OdGiAuxiliaryData
{
public:
  ODRX_DECLARE_MEMBERS(OdGiAuxDataForDgDatabase);
  OdGiAuxDataForDgDatabase();
  OdGiAuxDataForDgDatabase( const OdGiAuxDataForDgDatabase& obj );
  ~OdGiAuxDataForDgDatabase();

  OdGiAuxDataForDgDatabase& operator= (const OdGiAuxDataForDgDatabase& obj);

  static OdGiAuxDataForDgDatabasePtr getAuxData(OdGiDrawableTraits* pDrwTraits);
  static OdGiAuxDataForDgDatabasePtr getAuxData(OdDgDatabase* pDb);
  static OdGiAuxDataForDgDatabasePtr getAuxData(OdGiDrawableTraits* pDrwTraits, OdDgDatabase* pDb);
  static void                        setAuxData(OdDgDatabase* pDb, OdGiAuxDataForDgDatabasePtr& pAuxData);
  static void                        setAuxData(OdGiDrawableTraits* pDrwTraits, OdDgDatabase* pDb, OdGiAuxDataForDgDatabasePtr& pAuxData);
  static OdGiAuxDataForDgDatabasePtr createObject(OdDgDatabase* pDb);

  OdDgDatabase* getDatabase() const;
  void          setDatabase( OdDgDatabase* pDb );

  OdDgElement*  getNonDbroXRef() const;
  void          setNonDbroXRef(OdDgElement* pXRef);

  // Layer overrides

  virtual OdGiLayerOverridesPtr getLevelOverrides() const;
  virtual void setLevelOverrides(const OdGiLayerOverridesPtr& pLevelOverrides);

  // View Flags

  OdUInt32 getFlags1() const;
  void     setFlags1(OdUInt32 uFlag);
  OdUInt32 getFlags2() const;
  void     setFlags2(OdUInt32 uFlag);

  bool getFastCurveFlag() const;
  void setFastCurveFlag(bool);
  bool getHideTextsFlag() const;
  void setHideTextsFlag(bool);
  bool getHighQualityFontsFlag() const;
  void setHighQualityFontsFlag(bool);
  bool getShowLineWeightsFlag() const;
  void setShowLineWeightsFlag(bool);
  bool getShowPatternsFlag() const;
  void setShowPatternsFlag(bool);
  bool getShowTextNodesFlag() const;
  void setShowTextNodesFlag(bool);
  bool getShowDataFieldsFlag() const;
  void setShowDataFieldsFlag(bool);
  bool getShowGridFlag() const;
  void setShowGridFlag(bool);
  bool getShowLevelSymbologyFlag() const;
  void setShowLevelSymbologyFlag(bool);
  bool getShowPointsFlag() const;
  void setShowPointsFlag(bool);
  bool getShowConstructionFlag() const;
  void setShowConstructionFlag(bool);
  bool getShowDimensionsFlag() const;
  void setShowDimensionsFlag(bool);
  bool getFastCellsFlag() const;
  void setFastCellsFlag(bool);
  bool getShowFillsFlag() const;
  void setShowFillsFlag(bool);
  bool getRasterTextFlag() const;
  void setRasterTextFlag(bool);
  bool getShowAxisTriadFlag() const;
  void setShowAxisTriadFlag(bool);
  bool getOrientationDisplayFlag() const;
  void setOrientationDisplayFlag(bool);
  bool getViewRenderedFlag() const;
  void setViewRenderedFlag(bool);
  bool getShowBackgroundFlag() const;
  void setShowBackgroundFlag(bool);
  bool getShowBoundaryFlag() const;
  void setShowBoundaryFlag(bool);
  bool getFastBoundaryClipFlag() const;
  void setFastBoundaryClipFlag(bool);
  bool getDepthCueFlag() const;
  void setDepthCueFlag(bool);
  bool getInhibitDynamicsFlag() const;
  void setInhibitDynamicsFlag(bool);
  bool getShowShadowsFlag() const;
  void setShowShadowsFlag(bool);
  bool getShowTextureMapsFlag() const;
  void setShowTextureMapsFlag(bool);
  bool getShowHazeFlag() const;
  void setShowHazeFlag(bool);
  bool getTransparencyFlag() const;
  void setTransparencyFlag(bool);
  bool getIgnoreLineStylesFlag() const;
  void setIgnoreLineStylesFlag(bool);
  bool getAcceleratedFlag() const;
  void setAcceleratedFlag(bool);
  bool getPatternDynamicsFlag() const;
  void setPatternDynamicsFlag(bool);
  bool getHiddenLineFlag() const;
  void setHiddenLineFlag(bool);
  bool getShowTagsFlag() const;
  void setShowTagsFlag(bool);
  bool getDisplayEdgesFlag() const;
  void setDisplayEdgesFlag(bool);
  bool getDisplayHiddenEdgesFlag() const;
  void setDisplayHiddenEdgesFlag(bool);
  bool getOverrideBackgroundFlag() const;
  void setOverrideBackgroundFlag(bool);
  bool getShowClipFrontFlag() const;
  void setShowClipFrontFlag(bool);
  bool getShowClipBackFlag() const;
  void setShowClipBackFlag(bool);
  bool getShowClipVolumeFlag() const;
  void setShowClipVolumeFlag(bool);
  bool getUseDisplaySetFlag() const;
  void setUseDisplaySetFlag(bool);
  bool getShowConstructionOnlyFlag() const;
  void setShowConstructionOnlyFlag(bool);

  OdDgView::OdDgDisplayMode getDisplayMode() const;
  void setDisplayMode(OdDgView::OdDgDisplayMode uMode);

  void initViewFlags(OdDgViewPtr& pView);
  void initViewFlags(OdDgReferenceAttachmentViewFlags& xRefViewFlags);

  // Reference Attachment Data

  OdCmTransparency getXRefTransparency() const;
  void             setXRefTransparency(OdCmTransparency& trans);

  bool             getDrawNamedGroupFlag() const;
  void             setDrawNamedGroupFlag(bool bSet);

  bool             getSkipXRefsForSheetModelFlag() const;
  void             setSkipXRefsForSheetModelFlag( bool bSet );

  bool             getUseAnnotationScaleFlag() const;
  void             setUseAnnotationScaleFlag(bool bSet);

  bool             getXRefPaletteFixedColorFlag() const;
  void             setXRefPaletteFixedColorFlag(bool bSet);

  bool             getXRefPaletteUseHueFlag() const;
  void             setXRefPaletteUseHueFlag(bool bSet);

  OdInt32          getXRefFileNumber() const;
  void             setXRefFileNumber(OdInt32 uXRefFileNumber);

  double           getAnnotationScale() const;
  void             setAnnotationScale(double dSet);

  void             addXRefInfo( const OdDgElementPtr& pXRef, bool bSelfReference );
  OdUInt32         getXRefPathItemsCount() const ;
  OdGiAuxDataXRefInfoItem getXRefPathItem( OdUInt32 uIndex ) const;
  bool             isXRefPathEmpty() const;

  const OdGiAuxDataXRefPaletteCorrection* getXRefPaletteCorrection() const;
  void  setXRefPaletteCorrection(double dValue, double dSaturation, double dHue);
  void  removeXRefPaletteCorrection();

  virtual OdRxObjectPtr clone() const;

private:
  OdUInt32            m_uViewFlags1;
  OdUInt32            m_uViewFlags2;
  OdUInt8             m_uExtraFlags;

  OdUInt8             m_uXRefTarnsparency;
  OdUInt8             m_uXRefFileNumber;
  OdGiAuxDataXRefPaletteCorrection* m_pXRefPaletteCorrection;
  OdDgDatabase*       m_pDb;
  OdDgElement*        m_pNonDbroXRef;
  double              m_dAnnotationScale;

  OdGiLayerOverridesForDgDatabasePtr m_pLevelOverrides;

  OdArray<OdGiAuxDataXRefInfoItem>    m_arrXRefInfo;
};

/** \details
  <group TD_Namespaces>
*/
namespace OdDgAuxDataDependantAccessors
{
  bool getFastCurveFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool getHideTextsFlag(const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getHighQualityFontsFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowLineWeightsFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowPatternsFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowTextNodesFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowDataFieldsFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowGridFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowLevelSymbologyFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowPointsFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowConstructionFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowDimensionsFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getFastCellsFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowFillsFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowAxisTriadFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getOrientationDisplayFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getViewRenderedFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowBackgroundFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowBoundaryFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getFastBoundaryClipFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getInhibitDynamicsFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowShadowsFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowTextureMapsFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowHazeFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getTransparencyFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getIgnoreLineStylesFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getAcceleratedFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getPatternDynamicsFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getHiddenLineFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowTagsFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getDisplayEdgesFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getDisplayHiddenEdgesFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getOverrideBackgroundFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowClipFrontFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowClipBackFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowClipVolumeFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getUseDisplaySetFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool getShowConstructionOnlyFlag( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  OdDgView::OdDgDisplayMode getDisplayMode( const OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);

  bool setFastCurveFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setHideTextsFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setHighQualityFontsFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowLineWeightsFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowPatternsFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowTextNodesFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowDataFieldsFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowGridFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowLevelSymbologyFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowPointsFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowConstructionFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowDimensionsFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowFillsFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setRasterTextFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowAxisTriadFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setOrientationDisplayFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setViewRenderedFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowBackgroundFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowBoundaryFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setFastBoundaryClipFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setFastCellsFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
  bool setDepthCueFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setInhibitDynamicsFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowShadowsFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowTextureMapsFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowHazeFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setTransparencyFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setIgnoreLineStylesFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setAcceleratedFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setPatternDynamicsFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setHiddenLineFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowTagsFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setDisplayEdgesFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setDisplayHiddenEdgesFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setOverrideBackgroundFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowClipFrontFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowClipBackFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowClipVolumeFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setUseDisplaySetFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  bool setShowConstructionOnlyFlag(bool bSet, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView );
  OdDgView::OdDgDisplayMode setDisplayMode(OdDgView::OdDgDisplayMode uMode, OdGiAuxDataForDgDatabasePtr& pAuxData, const OdDgViewPtr& pView);
};

#include "TD_PackPop.h"

#endif // __DG_GIAUXDATAFORDGDATABASE_H__
