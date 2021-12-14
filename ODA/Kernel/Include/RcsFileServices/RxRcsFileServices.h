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

// RxRcsFileServices.h - interface of module that handles .rcs/.rcp files

#ifndef __OD_RCSFILESERVICES_MODULE__
#define __OD_RCSFILESERVICES_MODULE__

#include "RxModule.h"
#include "RootExport.h"

#include "TD_PackPush.h"
#include "SharedPtr.h"
#include "OdStreamBuf.h"
#include "OdPointCloudScanDatabase.h"
#include "OdPointCloudProjectDatabase.h"
#include "OdPointCloudConverter.h"
#include "OdRcsDataManager.h"

#include "TPointCloudModuleNames.h"

/** <group OdRx_Classes>
  \details
  This is an interface class for the module that provides loading point cloud
  data from .rcs/.rcp files.

  \remarks
  Corresponding C++ library: RcsFileServices.
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdRxRcsFileServices : public OdRxModule
{
public:

  /** \details
    Creates a new point cloud scan database object and populates it with the
    data read from a point cloud scan file (.rcs file) specified by its file
    path.

    \param filePath [in]  Path to the .rcs file.

    \returns
    The pointer to the OdPointCloudScanDatabase object.
  */
  virtual OdPointCloudScanDatabasePtr readRcsFile(const OdString& filePath) const = 0;

  /** \details
    Creates a new point cloud scan database object and populates it with the
    data read from a point cloud scan file (.rcs file) specified by the
    corresponding file buffer.

    \param pFileBuff [in]  Poinert to the file buffer, associated with the .rcs file.

    \returns
    The pointer to the OdPointCloudScanDatabase object.
  */
  virtual OdPointCloudScanDatabasePtr readRcsFile(OdStreamBuf* pFileBuff) const = 0;


  /** \details
    Creates a new point cloud project database object and populates it with the
    data from the corresponding point cloud project file (.rcp file) specified
    by its file path.

    \param filePath [in] Path to the .rcp file.

    \returns
    The pointer to the OdPointCloudProjectDatabase object.
  */
  virtual OdPointCloudProjectDatabasePtr readRcpFile(const OdString& filePath) const = 0;


  /** \details
    Creates a new point cloud converter with user implemented data source
    and with converter parameters to convert user point cloud data to .rcs file.

    \param pDataSource [in] Pointer to point cloud data source instance.
    \param pParams     [in] Pointer to conversion parameters.

    \returns
    The pointer to the OdPointCloudConverter object.
  */
  virtual OdPointCloudConverterPtr getPointCloudConverter(OdPointCloudDataSourcePtr pDataSource, 
    OdPointCloudConverterParamsPtr pParams = NULL) const = 0;


  /** \details
    Creates a new data manager object for the specified point cloud scan
    database.

    \param pScanDb        [in]  Pointer to the point cloud scan database.
    \param pHostProjectDb [in]  Pointer to the host point cloud project database.

    \returns
    The pointer to OdRcsDataManager object.
  */
  virtual OdRcsDataManagerPtr getRcsDataManager(OdPointCloudScanDatabasePtr pScanDb,
    OdPointCloudProjectDatabasePtr pHostProjectDb = NULL) const = 0;
};

/** \details
  Smart pointer to objects of the <link OdRxRcsFileServices, OdRxRcsFileServices> class.
*/
typedef OdSmartPtr<OdRxRcsFileServices> OdRxRcsFileServicesPtr;

#include "TD_PackPop.h"

#endif // __OD_RCSFILESERVICES_MODULE__
