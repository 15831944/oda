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
#include "DbUserIO.h"
#include "DbCommandContext.h"

#include "Gs/Gs.h"
#include "DbPolyline.h"
#include "DbBlockTableRecord.h"
#include "DbViewportTable.h"
#include "DbViewportTableRecord.h"
#include "DbDictionary.h"
#include "DbDictionaryVar.h"

#include "DbSortentsTable.h"
#include "DbBlockTableRecord.h"

#include "DbGeoData.h"
#include "DbGeoMap.h"
#include "OdDbGeoCoordinateSystem.h"

void setGeoDataValues(OdDbDatabase* pDb, OdDb::UnitsValue eUnits, const OdGePoint3d& ptGeoRSS, const OdGePoint3d& ptDesign, double dNorthDirectionRad, const OdString& sCSId)
{
  OdString sOldCSId;
  OdDbGeoDataPtr pGeoData;
  OdDbObjectId objIdGeoData;
  oddbGetGeoDataObjId(pDb, objIdGeoData);
  if (!objIdGeoData.isNull())
  {
    pGeoData = objIdGeoData.openObject(OdDb::kForWrite);
    OdDbGeoCoordinateSystemPtr pCS;
    if (eOk == OdDbGeoCoordinateSystem::create(pGeoData->coordinateSystem(), pCS) && !pCS.isNull())
    {
      pCS->getId(sOldCSId);
    }
  }
  else
  {
    pGeoData = OdDbGeoData::createObject();
    pGeoData->setBlockTableRecordId(pDb->getModelSpaceId());
    OdDbObjectId objId;
    ODA_VERIFY(eOk == pGeoData->postToDb(objId));
  }

  if (sOldCSId.compare(sCSId))
  {
    ODA_VERIFY(eOk == pGeoData->setCoordinateSystem(sCSId));
  }

  pGeoData->setCoordinateType(OdDbGeoData::kCoordTypLocal);
  pGeoData->setHorizontalUnits(eUnits);
  pGeoData->setVerticalUnits(eUnits);
  pGeoData->setDesignPoint(ptDesign);
  pGeoData->setUpDirection(OdGeVector3d::kZAxis);

  OdGeVector2d vNorthDirection = OdGeVector2d::kXAxis;
  vNorthDirection.rotateBy(dNorthDirectionRad);
  pGeoData->setNorthDirectionVector(vNorthDirection);

  OdGePoint3d ptReference;
  OdDbGeoCoordinateSystemTransformer::transformPoint(L"LL84", sCSId, OdGePoint3d(ptGeoRSS.y, ptGeoRSS.x, ptGeoRSS.z), ptReference);
  pGeoData->setReferencePoint(ptReference);
  pGeoData->setDoSeaLevelCorrection(false);
  pGeoData->setScaleEstimationMethod(OdDbGeoData::kScaleEstMethodUnity);

  double dEccentricity = 0.08181919092890674;
  OdDbGeoEllipsoid elipsoid;
  OdDbGeoCoordinateSystemPtr pCS;
  OdDbGeoCoordinateSystem::create(sCSId, pCS);
  if (!pCS.isNull() && eOk == pCS->getEllipsoid(elipsoid) && !elipsoid.id.isEmpty())
  {
    dEccentricity = elipsoid.eccentricity;
  }
  double dRadius = 6378137.;
  double dSinLat = sin(ptReference.y);
  double dCoordinateProjectionRadius = dRadius * (1 - dEccentricity * dEccentricity) / pow(1 - dEccentricity * dEccentricity * dSinLat * dSinLat, 1.5);
  pGeoData->setCoordinateProjectionRadius(dCoordinateProjectionRadius);

  pGeoData->setGeoRSSTag(OdString().format(L"<georss:point>%f %f</georss:point>", ptGeoRSS.x, ptGeoRSS.y));

  //database fields update:
  pDb->setLONGITUDE(ptGeoRSS.y);
  pDb->setLATITUDE(ptGeoRSS.x);
  //pDb->setELEVATION(ptGeoRSS.z);
  pDb->setNORTHDIRECTION(dNorthDirectionRad);

  pGeoData->updateTransformationMatrix();
}

//this function describes limitation for GeoData creation:
bool validateCSName(const OdString & sCS, OdDbUserIO* pIO)
{
  OdDbGeoCoordinateSystemPtr pCS;
  OdDbGeoCoordinateSystem::create(sCS, pCS);
  if (eOk != OdDbGeoCoordinateSystem::create(sCS, pCS) || pCS.isNull())
  {
    pIO->putString("Command Error: invalid coordinate system name.");
    return false;
  }
  return true;
}

//this function describes limitation for GeoMap creation:
bool validateGeoDataSettings(OdDbDatabase* pDb, OdDbUserIO * pIO)
{
  OdDbObjectId objIdGeoData;
  oddbGetGeoDataObjId(pDb, objIdGeoData);
  if (objIdGeoData.isNull())
  {
    pIO->putString("Command Error: There is no GeoData in drawing, specify it with GEO command.");
    return false;
  }
  return true;
}

void _Geo_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  //Lat, Lon, Elevation
  OdGePoint3d ptDefaultGeoRSS(pDb->getLATITUDE(), pDb->getLONGITUDE(), pDb->getELEVATION());
  OdGePoint3d ptGeoRSS = pIO->getPoint(L"Set GeoRSS (Lat, Lon, Elevation):", OdEd::kInpDefault, &ptDefaultGeoRSS);

  OdArray<OdDbGeoCoordinateSystemPtr> allCoordSys;
  if (eOk != OdDbGeoCoordinateSystem::createAll(OdGePoint3d(ptGeoRSS.y, ptGeoRSS.x, ptGeoRSS.z), allCoordSys) || !allCoordSys.size())
  {
    pIO->putString("Command Error: Can't create Coordinate System by current GeoRSS.");
    return;
  }

  {
    //dump supported Coordinate Systems for current GeoRSS:
    OdString sName;
    OdDbGeoCoordinateSystem::Unit eUnit = OdDbGeoCoordinateSystem::kUnitUnknown;
    int nEpsgCode = 0;
    OdDbGeoDatum datum;
    OdDbGeoEllipsoid ellipsoid;
    OdString sResult;
    pIO->putString(L"================================");
    pIO->putString(L"Name, Reference, Unit, EPSG code");
    for (OdUInt32 i = 0; i < allCoordSys.size(); ++i)
    {
      OdDbGeoCoordinateSystemPtr pCS = allCoordSys[i];
      if (eOk == pCS->getId(sName))
      {
        sResult = sName;
        sResult += ", ";
        sResult += (eOk == pCS->getDatum(datum) && !datum.id.isEmpty()) ? datum.id : (eOk == pCS->getEllipsoid(ellipsoid) && !ellipsoid.id.isEmpty()) ? ellipsoid.id : L"-";
        sResult += ", ";
        sResult += (eOk == pCS->getUnit(eUnit)) ? OdDbGeoCoordinateSystem::unitNameToString(eUnit) : L"-";
        sResult += ", ";
        sResult += (eOk == pCS->getEpsgCode(nEpsgCode) && nEpsgCode != 0) ? OdString().format(L"%d", nEpsgCode) : L"-";
        pIO->putString(sResult);
      }
    }
    pIO->putString(L"================================");
  }

  //pDb->setTIMEZONE(); // TODO: Do we need this?

  OdUInt32 nUnitType = pIO->getKeyword(L"Drawing Unit [Inches|Feet|Miles|Millimeters|Centimeters|Meters|Kilometers|Yards|Decimeters|Dekameters|Hectometers|US Survey Feet] <Millimeters>",
    L"Inches Feet Miles Millimeters Centimeters Meters Kilometers Yards Decimeters Dekameters Hectometers USSurveyFeet", 3);

  OdDb::UnitsValue eUnits = OdDb::kUnitsMillimeters;
  switch (nUnitType)
  {
    case 0: eUnits = OdDb::kUnitsInches; break;
    case 1: eUnits = OdDb::kUnitsFeet; break;
    case 2: eUnits = OdDb::kUnitsMiles; break;
    case 3: eUnits = OdDb::kUnitsMillimeters; break;
    case 4: eUnits = OdDb::kUnitsCentimeters; break;
    case 5: eUnits = OdDb::kUnitsMeters; break;
    case 6: eUnits = OdDb::kUnitsKilometers; break;
    case 7: eUnits = OdDb::kUnitsYards; break;
    case 8: eUnits = OdDb::kUnitsDecimeters; break;
    case 9: eUnits = OdDb::kUnitsDekameters; break;
    case 10: eUnits = OdDb::kUnitsHectometers; break;
    case 11: eUnits = OdDb::kUnitsUSSurveyFeet; break;
  }

  OdString sCSId = pIO->getString("Specify Coordinate System name <WORLD-MERCATOR>:", OdEd::kInpDefault, L"WORLD-MERCATOR");
  if (!validateCSName(sCSId, pIO))
  {
    return;
  }

  OdGePoint3d ptDesign = pIO->getPoint(L"Specify design point:");

  double dNorthDirectionRad = pIO->getAngle(L"Specify the north direction angle <90>:", OdEd::kInpDefault, OdaToRadian(90.));
  setGeoDataValues(pDb, eUnits, ptGeoRSS, ptDesign, dNorthDirectionRad, sCSId);
}

void _GeoRemove_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();
  OdUInt32 nKeyword = pIO->getKeyword(
    L"Delete geodata from file [Yes/No] ",
    L"Yes No", 1
  );
  if (0 == nKeyword)
  {
    OdDbObjectId objIdGeoData;
    oddbGetGeoDataObjId(pDb, objIdGeoData);
    if (!objIdGeoData.isNull())
    {
      OdDbGeoDataPtr pOldGeoData = objIdGeoData.openObject(OdDb::kForWrite);
      pOldGeoData->eraseFromDb();
    }
  }
}

void _GeoReorientMarker_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  OdDbObjectId objIdGeoData;
  if (eOk != oddbGetGeoDataObjId(pDb, objIdGeoData) || objIdGeoData.isNull())
  {
    pIO->putString("Command Error: There is no GeoData in drawing, specify it with GEO command.");
    return;
  }

  OdDbGeoDataPtr pGeoData = objIdGeoData.openObject(OdDb::kForWrite);
  OdGePoint3d ptDesignCurrent = pGeoData->designPoint();
  OdGePoint3d ptDesign = pIO->getPoint(L"Select a point for the location:", OdEd::kInpDefault, &ptDesignCurrent);
  double dNorthDirectionRad = pIO->getAngle(L"Specify the north direction angle:", OdEd::kInpDefault, pGeoData->northDirection());
  OdString sCSId;
  OdDbGeoCoordinateSystemPtr pCS;
  if (eOk == OdDbGeoCoordinateSystem::create(pGeoData->coordinateSystem(), pCS) && !pCS.isNull())
  {
    pCS->getId(sCSId);
  }
  OdGePoint3d ptGeoRSS(pDb->getLATITUDE(), pDb->getLONGITUDE(), pDb->getELEVATION());
  setGeoDataValues(pDb, pGeoData->horizontalUnits(), ptGeoRSS, ptDesign, dNorthDirectionRad, sCSId);
}

void _GeoMapImageUpdate_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  if (!validateGeoDataSettings(pDb, pIO))
  {
    return;
  }

  OdDbSelectionSetPtr pSSet = pIO->select(L"Select map image(s) to update:", OdEd::kSelDefault);
  OdUInt32 nKeyword = pIO->getKeyword(
    L"Enter action [Optimize/Reload] ",
    L"Optimize Reload", 1
    );

  bool bMod;
  switch (nKeyword)
  {
  case 0:
    bMod = true;
    break;
  case 1:
  default:
    bMod = false;
    break;
  }

  if (pSSet->numEntities())
  {
    OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();

    pDb->startTransaction();
    while (!pIter->done())
    {
      OdDbObjectId objId = pIter->objectId();
      OdDbGeoMapPtr pEnt = OdDbGeoMap::cast(objId.openObject(OdDb::kForWrite));
      if (!pEnt.isNull())
      {
        pEnt->updateMapImage(bMod);
      }
      pIter->next();
    }
    pDb->endTransaction();
  }
}

void _GeoMapImageSetResolution_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  if (!validateGeoDataSettings(pDb, pIO))
  {
    return;
  }

  OdDbSelectionSetPtr pSSet = pIO->select(L"Select map image(s) to change their resolution:", OdEd::kSelDefault);
  OdUInt32 nKeyword = pIO->getKeyword(
    L"Enter resolution [Cource/Optimal/Fine/VeryFine] ",
    L"Cource Optimal Fine VeryFine", 1
    );

  int nResolution;
  switch(nKeyword)
  {
  case 0:
    nResolution = kCoarse;
    break;
  case 2:
    nResolution = kFine;
    break;
  case 3:
    nResolution = kFiner;
    break;
  case 1:
  default:
    nResolution = kOptimal;
    break;
  }

  if (pSSet->numEntities())
  {
    OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();

    pDb->startTransaction();
    while (!pIter->done())
    {
      OdDbObjectId objId = pIter->objectId();
      OdDbGeoMapPtr pEnt = OdDbGeoMap::cast(objId.openObject(OdDb::kForWrite));
      if (!pEnt.isNull())
      {
        pEnt->setResolution(OdGeoMapResolution(nResolution));
        pEnt->updateMapImage();
      }
      pIter->next();
    }
    pDb->endTransaction();
  }
}

void _GeoMapImageSetMapType_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdSmartPtr<OdDbUserIO> pIO = pDbCmdCtx->userIO();

  if (!validateGeoDataSettings(pDb, pIO))
  {
    return;
  }

  OdDbSelectionSetPtr pSSet = pIO->select(L"Select map image(s) to change their type:", OdEd::kSelDefault);
  OdUInt32 nKeyword = pIO->getKeyword(
    L"Enter map type [Aerial/Road/Hybrid] ",
    L"Aerial Road Hybrid", 0
    );

  int nType;
  switch(nKeyword)
  {
  case 1:
    nType = kRoad;
    break;
  case 2:
    nType = kHybrid;
    break;
  case 0:
  default:  
    nType = kAerial;
    break;
  }

  if (pSSet->numEntities())
  {
    OdDbSelectionSetIteratorPtr pIter = pSSet->newIterator();

    pDb->startTransaction();
    while (!pIter->done())
    {
      OdDbObjectId objId = pIter->objectId();
      OdDbGeoMapPtr pEnt = OdDbGeoMap::cast(objId.openObject(OdDb::kForWrite));
      if (!pEnt.isNull())
      {
        pEnt->setMapType(OdGeoMapType(nType));
        pEnt->updateMapImage();
      }
      pIter->next();
    }
    pDb->endTransaction();
  }
}

void _GeoMapImage_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  if (!validateGeoDataSettings(pDb, pIO))
  {
    return;
  }

  OdDbViewportTablePtr pVt = pDb->getViewportTableId().safeOpenObject();
  OdDbViewportTableRecordPtr pVp = pVt->getActiveViewportId().safeOpenObject();
  OdDbDictionaryPtr pExtDictionary = pVp->extensionDictionary().openObject();
  OdDbDictionaryVarPtr pDictionaryVar;
  if (pExtDictionary.get())
  {
    OdDbDictionaryPtr pExtVariableDictionary = pExtDictionary->getAt(L"AcDbVariableDictionary").openObject();
    if (pExtVariableDictionary.get())
    {
      pDictionaryVar = pExtVariableDictionary->getAt(L"GEOMAPMODE").openObject();
    }
  }
  if (pDictionaryVar.isNull())
  {
    pIO->putString("Command Error: GeoMap type is set as kNoMap, specify it with GEOMAP command");
    return;
  }

  OdGePoint3d ptStart = pIO->getPoint(L"Specify first corner:");

  struct GeoMapTracker : OdStaticRxObject<OdEdPointTracker>
  {
    OdDbPolylinePtr m_ClipBoundary;
    OdGePoint3d m_ptFirst;

    GeoMapTracker(const OdGePoint3d & ptFirst) 
    {
      m_ptFirst = ptFirst;
      m_ClipBoundary = OdDbPolyline::createObject();
      m_ClipBoundary->reset(false, 5);
      for (int i = 0; i < 5; ++i)
      {
        m_ClipBoundary->addVertexAt(i, OdGePoint2d(ptFirst.x, ptFirst.y));
      }
    }

    virtual void setValue(const OdGePoint3d & val)
    {
      m_ClipBoundary->reset(false, 5);
      m_ClipBoundary->addVertexAt(0, OdGePoint2d(m_ptFirst.x, m_ptFirst.y));
      m_ClipBoundary->addVertexAt(1, OdGePoint2d(m_ptFirst.x, val.y));
      m_ClipBoundary->addVertexAt(2, OdGePoint2d(val.x, val.y));
      m_ClipBoundary->addVertexAt(3, OdGePoint2d(val.x, m_ptFirst.y));
      m_ClipBoundary->addVertexAt(4, OdGePoint2d(m_ptFirst.x, m_ptFirst.y));
    }

    virtual int addDrawables(OdGsView* pView)
    {
      return pView->add(m_ClipBoundary, 0);
    }

    virtual void removeDrawables(OdGsView* pView)
    {
      pView->erase(m_ClipBoundary);
    }
  } GeoMapTracker(ptStart);

  OdGePoint3d ptEnd = pIO->getPoint(L"Specify opposite corner:", 0, 0, OdString::kEmpty, &GeoMapTracker);

  OdDbObjectIdArray arObjId;
  {
    OdDbGeoMapPtr pGeoMap = OdDbGeoMap::createObject();
    pGeoMap->setDatabaseDefaults(pDb);
    OdGeExtents3d ext;
    ext.addPoint(ptStart);
    ext.addPoint(ptEnd);
    pGeoMap->setOrientation(ext.minPoint(),
                            OdGeVector3d(ext.maxPoint().x - ext.minPoint().x, 0., 0.),
                            OdGeVector3d(0., ext.maxPoint().y - ext.minPoint().y, 0.));

    OdString sGeoMapType = pDictionaryVar->value();
    pGeoMap->setMapType(OdGeoMapType(Od_strtoint(sGeoMapType)));
    pGeoMap->setDisplayOpt(OdDbRasterImage::kShow, true);
    OdDbObjectId modelSpaceId = pDb->getModelSpaceId();
    OdDbBlockTableRecordPtr pM = modelSpaceId.openObject(OdDb::kForWrite);
    pM->appendOdDbEntity(pGeoMap);
    arObjId.append(pGeoMap->id());
    pGeoMap->updateMapImage(true);
  }
  
  OdDbBlockTableRecordPtr pModelSpace = pDb->getModelSpaceId().safeOpenObject();
  OdDbSortentsTablePtr pSortentsTable = pModelSpace->getSortentsTable(true);
  pSortentsTable->moveToBottom(arObjId);
}


void _GeoMap_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdUInt32 nKeyword = pIO->getKeyword(
    L"Select map type [Aerial/Road/Hybrid/Off] ",
    L"Aerial Road Hybrid Off", 0
    );

  int nType;
  switch(nKeyword)
  {
  case 1:
    nType = kRoad;
    break;
  case 2:
    nType = kHybrid;
    break;
  case 3:
    nType = kNoMap;
    break;
  case 0:
  default:  
    nType = kAerial;
    break;
  }

  OdDbViewportTablePtr pVt = pDb->getViewportTableId().safeOpenObject();
  OdDbViewportTableRecordPtr pVp = pVt->getActiveViewportId().safeOpenObject(OdDb::kForWrite);
  OdDbDictionaryPtr pExtDictionary = pVp->extensionDictionary().openObject(OdDb::kForWrite);
  OdDbDictionaryPtr pExtVariableDictionary;

  if (pExtDictionary.isNull())
  {
    pVp->createExtensionDictionary();
    pExtDictionary = pVp->extensionDictionary().safeOpenObject(OdDb::kForWrite);
    pExtVariableDictionary = pExtDictionary->setAt(L"AcDbVariableDictionary", OdDbDictionary::createObject()).safeOpenObject(OdDb::kForWrite);
  }
  else
  {
    pExtVariableDictionary = pExtDictionary->getAt(L"AcDbVariableDictionary").safeOpenObject(OdDb::kForWrite);
  }

  if (nType != kNoMap)
  {
    OdDbDictionaryVarPtr pDictionaryVar = OdDbDictionaryVar::createObject();
    pDictionaryVar->setValue(nType);
    pExtVariableDictionary->setAt(L"GEOMAPMODE", pDictionaryVar);
  }
  else
  {
    pExtVariableDictionary->remove(L"GEOMAPMODE");
  }
}
