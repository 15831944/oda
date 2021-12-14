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


#ifndef __OD_DB_POINCLOUDEX_HOSTPE__
#define __OD_DB_POINCLOUDEX_HOSTPE__

#include "DbPointCloudObj/DbPointCloudHostPE.h"
#include "Gi/GiGeometry.h"

#include "RcsFileServices/RxRcsFileServices.h"

#include "CmEntityColorArray.h"

#include "TD_PackPush.h"

class ExPointCloudExScanItem : public OdDbPointCloudExScanItem
{
public:
  ODRX_DECLARE_MEMBERS(ExPointCloudExScanItem);

  virtual ~ExPointCloudExScanItem();

  virtual OdGeExtents3d extents() const;
  
  virtual OdUInt64 pointsCount() const;

  virtual bool worldDrawPoints(OdGiWorldDraw* pWd) const;
  virtual void viewportDrawPoints(OdGiViewportDraw* pVd) const;
    
  virtual bool hasRGB() const;
  virtual bool hasNormals() const;
  virtual bool hasIntensity() const;

  virtual OdString getScanId() const;

private:
  OdPointCloudScanDatabasePtr m_pScanDb;
  mutable OdRcsDataManagerPtr m_pRcsDataMgr;

  friend class ExPointCloudExHostPE;
};

typedef OdSmartPtr<ExPointCloudExScanItem> ExPointCloudExScanItemPtr;

class ExPointCloudExProjItem : public OdDbPointCloudExProjItem
{
public:
  ODRX_DECLARE_MEMBERS(ExPointCloudExProjItem);

  virtual ~ExPointCloudExProjItem();

  virtual OdGeExtents3d extents() const;

  virtual OdUInt64 pointsCount() const;

  virtual bool worldDrawPoints(OdGiWorldDraw* pWd) const;
  virtual void viewportDrawPoints(OdGiViewportDraw* pVd) const;

  virtual void getAllRcsFilePaths( OdStringArray &list ) const;
  virtual void getAllRcsRelativeFilePaths( OdStringArray &list ) const;

  virtual OdUInt64 getTotalAmountOfScanItems() const;  

  virtual OdUInt32 getTotalRegionsCount() const;
  virtual OdUInt32 getTotalScansCount() const;
  virtual OdString getCoordinateSystemName() const;

  virtual OdInt8 hasRGB() const;
  virtual OdInt8 hasNormals() const;
  virtual OdInt8 hasIntensity() const;

  virtual OdString getRcsFilePath(const OdString &guid) const;
  virtual OdString getRcsRelativeFilePath(const OdString &guid) const;

private:
  OdPointCloudProjectDatabasePtr m_pProjDb;
  mutable OdArray<OdRcsDataManagerPtr> m_pRcsDataMgrs;

  friend class ExPointCloudExHostPE;
};

typedef OdSmartPtr<ExPointCloudExProjItem> ExPointCloudExProjItemPtr;


/** Description:

    {group:OdDbPointCloud_Classes}
*/
class ExPointCloudExHostPE : public OdDbPointCloudExHostPE
{
public:
  ODRX_DECLARE_MEMBERS(ExPointCloudExHostPE);

  ExPointCloudExHostPE() {}

  virtual OdResult loadRcs(const OdString& strPath, OdDbPointCloudExItemPtr& item);
  virtual OdResult loadRcp(const OdString& strPath, OdDbPointCloudExItemPtr& pItem);
};

#include "TD_PackPop.h"

#endif
