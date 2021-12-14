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

#ifndef _DGN_EXPORTIMPL_INCLUDED_ 
#define _DGN_EXPORTIMPL_INCLUDED_

#include "DgnExport.h"
#include <RxDispatchImpl.h>
#include <RxVariantValue.h>
#include <DbHostAppServices.h>
#define STL_USING_MAP
#include <OdaSTL.h>
#include <DgElementId.h>
#include <DgHostAppServices.h>
#include <DgModel.h>
#include "DbMText.h"
#include "DgTextExtendedProperties.h"
#include "DgView.h"
#include <DbMaterial.h>
#include "DgMaterialTableRecord.h"
#include "DgDimStyleTableRecord.h"
#include <DbDimStyleTableRecord.h>
#include <DbRay.h>
#include <DbXline.h>
#include "DgDatabase.h"

class OdDbDatabase;
class OdDgLevelTableRecord;
class OdDbLayerTableRecord;
class OdDgElement;
class OdDgLineStyleResource;
class OdDgModel;


/** \details
  <group OdExport_Classes> 
*/
namespace TD_DGN_EXPORT {

//-------------------------------------------------------------------------------------------------------------

  class OdDgnExportLineWeightsMapImpl : public OdDgnExportLineWeightsMap
  {
  public:
    OdDgnExportLineWeightsMapImpl();

    virtual OdResult         setDgnIndexForDwgLineWeight(OdDb::LineWeight uDwgLineWeigth, OdUInt32 uDgnIndex);
    virtual void             copyLineWeightsMap(const OdDgnExportLineWeightsMapPtr& pMap);
    virtual OdUInt32         getDgnIndexByDwgLineWeight(OdDb::LineWeight uDwgLineWeigth) const;
    virtual void             resetToDefaults();
    virtual OdArray< OdDb::LineWeight> getOverridedDwgLineWeights() const;

    static  OdUInt32         getDefaultDgnIndexByDwgLineWeight(OdDb::LineWeight uDwgLineWeight);

  private:
    std::map< OdDb::LineWeight, OdUInt32> m_mapLineWeights;
  };

/** \details
  This class implements the properties for DGN importer.
*/
class DgnExportProperties : public OdRxDispatchImpl<>
{
  OdString _path;
  OdDbDatabasePtr _dwgdb;
  OdDgDatabasePtr _dgndb;
  OdSmartPtr<OdDbHostAppServices> _dbSvc;
  OdSmartPtr<OdDgHostAppServices> _dgSvc;
  OdDgnExportLineWeightsMapPtr _pLineWeightMap;
  ODCOLORREF                   _backgroundColor;
  bool                         _recomputeDimBlocks;
public:
  DgnExportProperties()
  {
    _pLineWeightMap = OdRxObjectImpl<OdDgnExportLineWeightsMapImpl>::createObject();
    _backgroundColor = ODRGB(0, 0, 0);
    _recomputeDimBlocks = false;
  }

  ~DgnExportProperties()
  {
    _pLineWeightMap.release();
  }

  ODRX_DECLARE_DYNAMIC_PROPERTY_MAP( DgnExportProperties );
  static OdRxDictionaryPtr createObject();

  OdRxObjectPtr get_DwgDatabase() const { return _dwgdb.get(); }
  void put_DwgDatabase( OdRxObject* obj ){ _dwgdb = obj; }
  OdRxObjectPtr get_DgnDatabase() const { return _dgndb.get(); }
  void put_DgnDatabase( OdRxObject* obj ){ _dgndb = obj; }
  OdRxObjectPtr get_DwgServices() const { return _dbSvc.get(); }
  void put_DwgServices( OdRxObject* obj ){ _dbSvc = obj; }
  OdString get_DwgPath() const { return _path; }
  void put_DwgPath( OdString path ){ _path = path; }
  OdRxObjectPtr get_DgnServices() const { return _dgSvc.get(); }
  void put_DgnServices( OdRxObject* obj ){ _dgSvc = obj; }
  OdRxObjectPtr get_LineWeightsMap() const { return _pLineWeightMap; }
  void put_LineWeightsMap(OdRxObject* obj) { throw(OdError(eNotImplemented)); }
  const bool &get_RecomputeDimensionsAfterExport() const { return _recomputeDimBlocks; }
  void put_RecomputeDimensionsAfterExport(const bool& bSet) { _recomputeDimBlocks = bSet; }
  const OdUInt32 &get_BackgroundColor() const { return _backgroundColor; }
  void put_BackgroundColor(const OdUInt32& uColor) { _backgroundColor = uColor; }
};

typedef OdSmartPtr<DgnExportProperties> DgnExportPropertiesPtr;

//-------------------------------------------------------------------------------------------------------------

/** \details
  This class implements the DGN importer.
*/
class DgnExporter : public OdDgnExport
{
  DgnExportPropertiesPtr _properties;

public:
  DgnExporter();
  virtual OdRxDictionaryPtr properties() ODRX_OVERRIDE;
  virtual ExportResult exportDb() ODRX_OVERRIDE;

  OdDgElementId getDgnElementId(const OdDbObjectId& idDwg);

public:
  void setDwgColorTable(OdDgDatabase* pDgnDb, bool bLightColorSchema, ODCOLORREF bgColor );
  void copyRegApps(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb);
  void copyTextStyles(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb, double dMasterUnitsToUorsScale = 1.0, bool bRenameIfEqual = false);
  void copyMaterials(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb, bool bRenameIfEqual = false);
  void copyLineStyles(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb, double dMasterUnitsToUorsScale = 1.0);
  void copyLayers(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb, bool bUpdateDefaultLevelInfo = true, bool bRenameIfEqual = false );
  void copyMultilineStyles(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb);
  void copyDimensionStyles(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb);
  void copyVisualStyles(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb);
  void copyNamedViews(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb);
  void copyUCSs(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb);
  void copyGroups(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb);
  void copyDatabaseDefaults(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb);
  void setActiveModelSettings(const OdDbDatabase* pDwgDb, OdDgModel* pDgModel);

  void copyModelSpace(const OdDbDatabase* pDwgDb, OdDgModel* pActiveModel);
  void copyPaperSpaces(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb);
  void copyBlocks(const OdDbDatabase* pDwgDb, OdDgDatabase* pDgnDb);
  static void copyBlock(const OdDbBlockTableRecord* pDwgBlock, OdDgElement* pDgnBlock, OdDbObjectId* idOverallVP = NULL);

  void registerElementLoaders();
  void unregisterElementLoaders();
  static void setExportPEToElements();
  static void removeExportPEFromElements();

  OdUInt32 getDgnIndexByLineWeight(OdDb::LineWeight uLineWeight);

  void addIdPair(const OdDbObjectId& idDwg, const OdDgElementId& idDgn);

private:
  typedef std::map<OdDbObjectId, OdDgElementId> IdDbToDgMap;

  IdDbToDgMap          m_idMap;
  OdDbDatabase*        m_pDwgDb;
  OdDgHostAppServices* m_pDgHostAppServices;
};

}
#endif // _DGN_EXPORTIMPL_INCLUDED_
