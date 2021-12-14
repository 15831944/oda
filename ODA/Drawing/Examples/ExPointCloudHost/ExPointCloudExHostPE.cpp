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
#include "RxObjectImpl.h"
#include "ExPointCloudExHostPE.h"
#include "Gi/GiWorldDraw.h"
#include "Gi/GiViewportDraw.h"

#include "../Extensions/ExServices/OdFileBuf.h"
#include "DbHostAppServices.h"
#include "OdPlatformStreamer.h"

ODRX_CONS_DEFINE_MEMBERS( ExPointCloudExScanItem, OdDbPointCloudExScanItem, RXIMPL_CONSTR);

void drawPoints(OdRcsDataManagerPtr pMgr, OdGiGeometry&, OdInt16 pointSize);

ExPointCloudExScanItem::~ExPointCloudExScanItem()
{
}

OdGeExtents3d ExPointCloudExScanItem::extents() const
{
  OdGeExtents3d res;
  
  if (!m_pScanDb.isNull())
  { 
    res = m_pScanDb->getTransformedExtents();
  }

  return res;
}

OdUInt64 ExPointCloudExScanItem::pointsCount() const
{
  OdUInt64 resSize = 0;
  
  if (!m_pScanDb.isNull())
  {
    resSize = m_pScanDb->getTotalAmountOfPoints();
  }

  return resSize;
}

bool ExPointCloudExScanItem::worldDrawPoints(OdGiWorldDraw* pWd) const
{
  return false;
}

void ExPointCloudExScanItem::viewportDrawPoints(OdGiViewportDraw* pVd) const
{
  if (!m_pRcsDataMgr.isNull())
  {
    OdDbDatabasePtr pDb = pVd->context()->database();
    OdInt16 pointSize = pDb->getPOINTCLOUDPOINTSIZE();

    pVd->geometry().pushModelTransform(m_pScanDb->getTransformMatrix());

    // New implementation:
    OdGiPointCloudPtr pGiPointCloud = m_pRcsDataMgr->newGiPointCloud(pointSize);
    pVd->geometry().pointCloud(*pGiPointCloud);
    //

    //Old implementation using polypoint:

    /*m_pRcsDataMgr->updateListOfVisibleVoxels(pVd->viewport(), pointSize);
    m_pRcsDataMgr->loadPointsForVisibleVoxels();
    drawPoints(m_pRcsDataMgr, pVd->geometry(), pointSize);*/

    pVd->geometry().popModelTransform();
  }
}

bool ExPointCloudExScanItem::hasRGB() const
{
  bool res = false;
  if (!m_pScanDb.isNull())
  { 
    res = m_pScanDb->hasRGB();
  }  
  return res;
}

bool ExPointCloudExScanItem::hasNormals() const
{
  bool res = false;
  if (!m_pScanDb.isNull())
  { 
    res = m_pScanDb->hasNormals();
  }  
  return res;
}

bool ExPointCloudExScanItem::hasIntensity() const
{
  bool res = false;
  if (!m_pScanDb.isNull())
  { 
    res = m_pScanDb->hasIntensity();
  }
  return res;
}

OdString ExPointCloudExScanItem::getScanId() const
{
  OdString res;
  if (!m_pScanDb.isNull())
  { 
    res = m_pScanDb->getScanId();
  }  
  return res;
}

//////////////////////////////////////////////////////////////////////////

ODRX_CONS_DEFINE_MEMBERS( ExPointCloudExProjItem, OdDbPointCloudExProjItem, RXIMPL_CONSTR);

ExPointCloudExProjItem::~ExPointCloudExProjItem()
{
}

OdGeExtents3d ExPointCloudExProjItem::extents() const
{
  OdGeExtents3d res;

  if (!m_pProjDb.isNull())
  {
    res = m_pProjDb->getExtents();
  }
  return res;
}

OdUInt64 ExPointCloudExProjItem::pointsCount() const
{
  OdUInt64 res = 0;
  if (!m_pProjDb.isNull())
  {
    res = m_pProjDb->getTotalAmountOfPoints();
  }
  return res;
}

bool ExPointCloudExProjItem::worldDrawPoints(OdGiWorldDraw* pWd) const
{
  return false;
}

void ExPointCloudExProjItem::viewportDrawPoints(OdGiViewportDraw* pVd) const
{
  OdDbDatabasePtr pDb = pVd->context()->database();
  OdInt16 pointSize = pDb->getPOINTCLOUDPOINTSIZE();

  OdGeMatrix3d globalTransformation = m_pProjDb->getGlobalTransformation();
  pVd->geometry().pushModelTransform(globalTransformation);

  OdArray<OdRcsDataManagerPtr>::iterator pMgrIt = m_pRcsDataMgrs.begin();
  for (; pMgrIt != m_pRcsDataMgrs.end(); ++pMgrIt)
  {
    OdRcsDataManagerPtr pCurManager = *pMgrIt;
    OdPointCloudScanDatabasePtr pCurScanDb = pCurManager->getScanDb();

    OdString scanId = pCurScanDb->getScanId();
    OdGeMatrix3d scanTransformation = m_pProjDb->getScanTransform(scanId);

    pVd->geometry().pushModelTransform(scanTransformation);

    OdGiPointCloudPtr pGiPointCloud = pCurManager->newGiPointCloud(pointSize);
    pVd->geometry().pointCloud(*pGiPointCloud);

    //pCurManager->updateListOfVisibleVoxels(pVd->viewport(), pointSize);
    //pCurManager->loadPointsForVisibleVoxels();
    //drawPoints(pCurManager, pVd->geometry(), pointSize);

    pVd->geometry().popModelTransform();
  }

  pVd->geometry().popModelTransform();  
}

void ExPointCloudExProjItem::getAllRcsFilePaths( OdStringArray &list ) const
{
  if (!m_pProjDb.isNull())
  {
    m_pProjDb->getAllRcsFilePaths(list);
  }
}

void ExPointCloudExProjItem::getAllRcsRelativeFilePaths( OdStringArray &list ) const
{
  if (!m_pProjDb.isNull())
  {
    m_pProjDb->getAllRcsRelativeFilePaths(list);
  }
}

OdUInt64 ExPointCloudExProjItem::getTotalAmountOfScanItems() const
{
  OdUInt64 res = 0;
  if (!m_pProjDb.isNull())
  {
    res = m_pProjDb->getTotalScansCount();
  }
  return res;
}

OdUInt32 ExPointCloudExProjItem::getTotalRegionsCount() const
{
  OdUInt32 res = 0;
  if (!m_pProjDb.isNull())
  {
    res = m_pProjDb->getTotalRegionsCount();
  }
  return res;
}

OdUInt32 ExPointCloudExProjItem::getTotalScansCount() const
{
  OdUInt32 res = 0;
  if (!m_pProjDb.isNull())
  {
    res = m_pProjDb->getTotalScansCount();
  }
  return res;
}

OdString ExPointCloudExProjItem::getCoordinateSystemName() const
{
  OdString res;
  if (!m_pProjDb.isNull())
  {
    res = m_pProjDb->getCoordinateSystemName();
  }
  return res;
}

OdInt8 ExPointCloudExProjItem::hasRGB() const
{
  OdInt8 res = -1;
  if (!m_pProjDb.isNull())
    res = m_pProjDb->hasRGB();
  return res;
}

OdInt8 ExPointCloudExProjItem::hasNormals() const
{
  OdInt8 res = -1;
  if (!m_pProjDb.isNull())
    res = m_pProjDb->hasNormals();
  return res;
}

OdInt8 ExPointCloudExProjItem::hasIntensity() const
{
  OdInt8 res = -1;
  if (!m_pProjDb.isNull())
    res = m_pProjDb->hasIntensity();
  return res;
}

OdString ExPointCloudExProjItem::getRcsFilePath(const OdString &guid) const
{
  OdString res;
  if (!m_pProjDb.isNull())
    res = m_pProjDb->getRcsFilePath(guid);
  return res;
}

OdString ExPointCloudExProjItem::getRcsRelativeFilePath(const OdString &guid) const
{
  OdString res;
  if (!m_pProjDb.isNull())
    res = m_pProjDb->getRcsRelativeFilePath(guid);
  return res;
}

/////////////////////////////////////////////////////////////////////
ODRX_NO_CONS_DEFINE_MEMBERS(ExPointCloudExHostPE, OdDbPointCloudExHostPE);

OdResult ExPointCloudExHostPE::loadRcs(const OdString& filename, OdDbPointCloudExItemPtr& pItem)
{ 
  OdRxRcsFileServicesPtr pRcsFileServices;

  pRcsFileServices = odrxDynamicLinker()->loadApp(RX_RCS_FILE_SERVICES);
  
  if (!pRcsFileServices.isNull())
  {
    OdPointCloudScanDatabasePtr pScanDb;

    try
    {
      pScanDb = pRcsFileServices->readRcsFile(filename);
    }
    catch (OdError& )
    {
      return eCantOpenFile;
    }

    ExPointCloudExScanItemPtr exItem = ExPointCloudExScanItem::createObject();    
    exItem->m_pScanDb = pScanDb;

    exItem->m_pRcsDataMgr = pRcsFileServices->getRcsDataManager(exItem->m_pScanDb);

    pItem = exItem;

    return eOk;
  }
  else
    return eCantOpenFile;  
}

OdResult ExPointCloudExHostPE::loadRcp(const OdString& filename, OdDbPointCloudExItemPtr& pItem)
{
  OdRxRcsFileServicesPtr pRcsFileServices;

  pRcsFileServices = odrxDynamicLinker()->loadApp(RX_RCS_FILE_SERVICES);

  if (!pRcsFileServices.isNull())
  {
    OdPointCloudProjectDatabasePtr pProjDb;

    try
    {
      pProjDb = pRcsFileServices->readRcpFile(filename);
    }
    catch (OdError&)
    {
      return eCantOpenFile;
    }

    ExPointCloudExProjItemPtr exItem = ExPointCloudExProjItem::createObject();
    exItem->m_pProjDb = pProjDb;

    exItem->m_pRcsDataMgrs.clear();
    
    OdPointCloudScanIteratorPtr pScanIt = pProjDb->getScanIterator();
    for (; !pScanIt->done(); pScanIt->step())
    {
      OdPointCloudScanDatabasePtr pCurScanDb = pScanIt->getScanDb();
      if (!pCurScanDb.isNull())
      {
        OdRcsDataManagerPtr pMgr = pRcsFileServices->getRcsDataManager(pCurScanDb, exItem->m_pProjDb);
        exItem->m_pRcsDataMgrs.push_back(pMgr);
      }
    }

    pItem = exItem;

    return eOk;
  }
  else
    return eCantOpenFile;
}

//void drawPoints(OdRcsDataManagerPtr pMgr, OdGiGeometry& geom, OdInt16 pointSize)
//{
//  if (!pMgr.isNull())
//  {
//    OdRcsVisibleVoxelsIteratorPtr pIt = pMgr->newVisibleVoxelsIterator();
//    if (!pIt.isNull())
//    {
//      OdUInt64 totalAmountOfPointsToDraw = 0;
//      for (; !pIt->done(); pIt->step())
//      {
//        totalAmountOfPointsToDraw += pIt->getNumberOfPointsByCurrentLOD();
//      }
//
//      OdGePoint3dArray coordsArrayToCallGeom;
//      OdCmEntityColorArray colorsArrayToCallGeom;
//      coordsArrayToCallGeom.resize(totalAmountOfPointsToDraw);
//      colorsArrayToCallGeom.resize(totalAmountOfPointsToDraw);
//
//      OdGePoint3d* pCoordinates = coordsArrayToCallGeom.asArrayPtr();
//      OdCmEntityColor* pColors = colorsArrayToCallGeom.asArrayPtr();
//      OdUInt64 currentPosition = 0;
//
//      double buffersCollectionFullTime = 0;
//
//      for (pIt->start(); !pIt->done(); pIt->step())
//      {
//        OdGePoint3dArray coords;
//        OdCmEntityColorArray colors;
//        OdUInt32 numberOfPointsToDraw = 0;
//        OdUInt32 loadedPointsNumber = 0;
//
//        //
//        OdUInt64 voxelIndex = 0;
//        //
//
//        //
//        double curBufCollect = getCPUTime();
//        //
//
//        loadedPointsNumber = pIt->getVoxelBuffer(coords, colors, numberOfPointsToDraw, voxelIndex);//
//
//        //
//        double endBufCollect = getCPUTime();
//        buffersCollectionFullTime += endBufCollect - curBufCollect;
//        //
//
//        if (loadedPointsNumber)
//        {
//          ::memcpy((pCoordinates + currentPosition), coords.asArrayPtr(), 
//            numberOfPointsToDraw * sizeof(OdGePoint3d));
//
//          ::memcpy((pColors + currentPosition), colors.asArrayPtr(),
//            numberOfPointsToDraw * sizeof(OdCmEntityColor));
//
//          currentPosition += numberOfPointsToDraw;
//        }
//      }
//
//      //
//      ODA_TRACE(L"\nTIME OF VOXEL BUFFERS PREPARATION = %lf", buffersCollectionFullTime);
//      //
//      
//      geom.polypoint(totalAmountOfPointsToDraw, pCoordinates, pColors, NULL, NULL, NULL, pointSize);
//    }
//  }
//}
