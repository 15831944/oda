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

#if !defined(_HIO_DWG_IMPL_H)
#define _HIO_DWG_IMPL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OdaCommon.h"
#include "OdArray.h"
#include "DbDatabase.h"
#include "DgDatabase.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "ExDgnServices.h"
#include "ExDgnHostAppServices.h"
#include "HoopsExport.h"
#include "HoopsResultEx.h "
#include "RxInit.h"
#include "RxDynamicModule.h"
#include "DynamicLinker.h"
#include "hoops_3dgs/source/hc.h"

////////////////////////////////////////////////////////////////

class DwgExportServices : public ExSystemServices, public ExHostAppServices
{
protected:

  ODRX_USING_HEAP_OPERATORS(ExSystemServices);

public:
  bool m_bSkipAll;
  int  m_nProgressPos;
  int  m_nProgressLimit;
  int  m_nPercent;
  OdString m_Msg;
  CStatusBar* m_pWndStatusBar;

public:

  DwgExportServices():ExSystemServices(),ExHostAppServices()
  {
    m_bSkipAll = false;
    m_nProgressPos = 0;
    m_nProgressLimit = 100;
  };

  ///////////////////////////////

  OdString findFile(const OdString& pcFilename,
                    OdDbBaseDatabase* pDb = NULL,
                    OdDbBaseHostAppServices::FindFileHint hint = kDefault);

  OdDbHostAppProgressMeter* newProgressMeter();
  void start(const OdString& displayString);
  void stop();
  void meterProgress();
  void setLimit(int max);
};

////////////////////////////////////////////////////////////////

class CDwgDumper
{
  public:

    CDwgDumper();

///////////////////////////////

    virtual ~CDwgDumper();

///////////////////////////////

    bool initialize();
    void uninitialize();

///////////////////////////////

    bool loadToHoops();

///////////////////////////////

    HC_KEY    getModelKey();
    void      setModelKey( HC_KEY lNewKey );
    OdString  getFilePath();
    void      setFilePath( OdString strPath );
    OdString  getLastErrorDescription();

///////////////////////////////

  private:

    OdStaticRxObject<DwgExportServices> svcs;
    OdDbDatabasePtr   m_pDb;

    HC_KEY            m_lModelKey;
    OdString          m_strFileName;
    OdString          m_strErrorDescription;
    bool              m_bInitSuccessful;
};

////////////////////////////////////////////////////////////////

class DgnExportServices : public OdExDgnSystemServices, public OdExDgnHostAppServices
{
protected:

  ODRX_USING_HEAP_OPERATORS(OdExDgnSystemServices);

public:
  bool m_bSkipAll;
  int  m_nProgressPos;
  int  m_nProgressLimit;
  int  m_nPercent;
  OdString m_Msg;
  CStatusBar* m_pWndStatusBar;

public:

  DgnExportServices():OdExDgnSystemServices(),OdExDgnHostAppServices()
  {
    m_bSkipAll = false;
    m_nProgressPos = 0;
    m_nProgressLimit = 100;
  };

  ///////////////////////////////

  OdString findFile(const OdString& pcFilename,
    OdDbBaseDatabase* pDb = NULL,
    OdDbBaseHostAppServices::FindFileHint hint = kDefault);

  OdDbHostAppProgressMeter* newProgressMeter();
  void start(const OdString& displayString);
  void stop();
  void meterProgress();
  void setLimit(int max);
};

////////////////////////////////////////////////////////////////

class CDgnDumper
{
public:

  CDgnDumper();

  ///////////////////////////////

  virtual ~CDgnDumper();

  ///////////////////////////////

  bool initialize();
  void uninitialize();

  ///////////////////////////////

  bool loadToHoops();

  ///////////////////////////////

  HC_KEY    getModelKey();
  void      setModelKey( HC_KEY lNewKey );
  OdString  getFilePath();
  void      setFilePath( OdString strPath );
  OdString  getLastErrorDescription();

  ///////////////////////////////

private:

  OdStaticRxObject<DgnExportServices> svcs;
  OdDgDatabasePtr   m_pDb;

  HC_KEY            m_lModelKey;
  OdString          m_strFileName;
  OdString          m_strErrorDescription;
  bool              m_bInitSuccessful;
};

////////////////////////////////////////////////////////////////

#endif
